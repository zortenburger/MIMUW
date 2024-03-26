#include <iostream>
#include "system.hpp"


std::vector<WorkerReport> System::shutdown() {
    running.store(false);
    cvWorker.notify_all();

    for (auto &worker: workers) {
        worker.join();
        worker = std::thread();
    }
    for (auto &machine: myMachines) {
        machine.second.checkingMutex.lock();
        if (!machine.second.machineBroken && machine.second.machineWorking)
            machine.second.stop();
        machine.second.checkingMutex.unlock();
    }
    return workerReports;
}

std::vector<std::string> System::getMenu() const {
    std::vector<std::string> menuResult;
    if(!running.load())
        return menuResult;
    for (auto &product: myMachines) {
        product.second.checkingMutex.lock();
        if (!product.second.machineBroken)
            menuResult.push_back(product.first);
        product.second.checkingMutex.unlock();
    }
    return menuResult;
}

std::vector<unsigned int> System::getPendingOrders() const {
    std::vector<unsigned int> result;
    std::unique_lock<std::mutex> lock(orderMutex);
    for (auto &order: orders) {
        if (!((order.second.expired.load())) &&
            !(order.second.broken.load()) &&
            !((order.second.collected.load())))
            result.push_back(order.first);
    }
    return result;
}

bool System::checkIfOrderCorrect(std::vector<std::string> &products) {
    if (products.empty())
        return false;
    for (auto &product: products) {
        if (myMachines.find(product) == myMachines.end())
            return false;
        else {
            myMachines.at(product).checkingMutex.lock();
            if (myMachines.at(product).machineBroken) {
                myMachines.at(product).checkingMutex.unlock();
                return false;
            }
            myMachines.at(product).checkingMutex.unlock();
        }
    }
    return true;
}

bool System::checkIfQueueNotEmpty() {
    std::unique_lock<std::mutex> lock(orderMutex);
    return !ordersQueue.empty();
}

std::unique_ptr<CoasterPager> System::order(std::vector<std::string> products) {
    if (!running.load()) {
        throw RestaurantClosedException();
    }

    if (!checkIfOrderCorrect(products)) {
        throw BadOrderException();
    }

    std::unique_lock<std::mutex> lock(orderMutex);
    unsigned int id = nextOrderId.load();
    nextOrderId++;
    orders.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, std::move(products)));
    MyPager *order = &orders.at(id);
    CoasterPager temp = CoasterPager(id, order);
    auto pager = std::make_unique<CoasterPager>(temp);
    ordersQueue.push(id);
    cvWorker.notify_one();
    return pager;
}

std::vector<std::unique_ptr<Product>>
System::collectOrder(std::unique_ptr<CoasterPager> CoasterPager) {
    if(CoasterPager == nullptr)
        throw BadPagerException();
    unsigned int id = CoasterPager->getId();
    MyPager &order = orders.at(id);
    if (orders.find(id) == orders.end()) {
        throw BadPagerException();
    }
    if ((order.expired.load())) {
        throw OrderExpiredException();
    }
    if ((order.broken.load())) {
        throw FulfillmentFailure();
    }
    if ((order.collected.load())) {
        throw BadPagerException();
    }
    if (!((order.ready.load()))) {
        throw OrderNotReadyException();
    } else {
        (order.collected.store(true));
        order.cv2.notify_one();
        std::unique_lock<std::mutex> lock(order.productsMutex);
        return std::move(order.collectedProducts);
    }
}

unsigned int System::getClientTimeout() const {
    return clientTimeout;
}

void System::work(unsigned int workerId) {
    while (running.load()) {
        std::unique_lock<std::mutex> lock1(workerMutex);
        cvWorker.wait(lock1,
                      [this] {
                          return !running.load() || checkIfQueueNotEmpty();
                      });
        if (!running.load()) {
            break;
        }
        orderMutex.lock();
        unsigned int orderId = ordersQueue.front();
        ordersQueue.pop();
        MyPager &order = orders.at(orderId);
        orderMutex.unlock();
        std::vector<std::future<std::unique_ptr<Product>>> futures(
                order.products.size());
        std::vector<std::promise<std::unique_ptr<Product>>> promises(
                order.products.size());
        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < order.products.size(); i++) {
            std::string &product = order.products[i];
            MachineWrapper &myMachine = myMachines.at(product);
            std::condition_variable myCv;
            myMachine.queueMutex.lock();
            myMachine.machineConditionVariables.emplace(std::ref(myCv), order.id);
            myMachine.queueMutex.unlock();
            promises[i] = (std::promise<std::unique_ptr<Product>>());
            futures[i] = (promises[i].get_future());
            threads.emplace_back(&System::waitForProduct, this, product,
                                 order.id, std::move(promises[i]),
                                 std::ref(myCv));
        }
        workerMutex.unlock();
        for (unsigned int i = 0; i < order.products.size(); i++) {

            try {
                threads[i].join();
                order.productsMutex.lock();
                order.collectedProducts.push_back(futures[i].get());
                order.whichProducts.push_back(order.products[i]);
                order.productsMutex.unlock();
            } catch (MachineFailure &e) {
                workerReports[workerId].failedProducts.push_back(
                        order.products[i]);
                order.productsMutex.unlock();
                (order.broken.store(true));
                order.cv.notify_one();
            }
        }
        if (order.broken.load()) {
            order.productsMutex.lock();
            returnAllProducts(order);
            workerReports[workerId].failedOrders.push_back(order.products);
            order.productsMutex.unlock();
        } else {
            order.ready.store(true);
            order.cv.notify_one();
            std::unique_lock<std::mutex> lock(mutex);
            auto end = std::chrono::steady_clock::now() +
                       std::chrono::milliseconds(clientTimeout);
            order.cv2.wait_until(lock, end,
                                 [&order] { return (order.collected.load()); });
            std::unique_lock <std::mutex> lock2(order.productsMutex);
            if (!((order.collected.load()))) {
                order.expired.store(true);
                workerReports[workerId].abandonedOrders.push_back(
                        order.products);
                returnAllProducts(order);
            } else {
                workerReports[workerId].collectedOrders.push_back(
                        order.products);
            }
        }
    }
}

void System::waitForProduct(const std::string &product, unsigned int id,
                            std::promise<std::unique_ptr<Product>> promise,
                            std::condition_variable &cv) {
    MachineWrapper &myMachine = myMachines.at(product);
    try {
        std::unique_ptr<Product> pom = myMachine.getProduct(id, cv);
        promise.set_value(std::move(pom));
    } catch (MachineFailure &e) {
        promise.set_exception(std::current_exception());
    }
}


void System::returnAllProducts(MyPager &order) {
    for (unsigned int i = 0; i < order.collectedProducts.size(); i++) {
        std::string &product = order.whichProducts[i];
        MachineWrapper &myMachine = myMachines.at(product);
        myMachine.returnProduct(std::move(order.collectedProducts[i]));
    }
}

void CoasterPager::wait() {

    if (myPager->ready.load()) {
        return;
    }
    if (myPager->broken.load()) {
        throw FulfillmentFailure();
    }
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    myPager->cv.wait(lock, [this] {
        return myPager->ready.load() || myPager->broken.load();
    });
    if (myPager->broken.load()) {
        throw FulfillmentFailure();
    }
}

void CoasterPager::wait(unsigned int timeout) {
    if (myPager->ready.load()) {
        return;
    }
    if (myPager->broken.load()) {
        throw FulfillmentFailure();
    }
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    auto end = std::chrono::steady_clock::now() +
               std::chrono::milliseconds(timeout);
    myPager->cv.wait_until(lock, end, [this] {
        return myPager->ready.load() || myPager->broken.load();
    });
    if (myPager->broken.load()) {
        throw FulfillmentFailure();
    }
}

unsigned int CoasterPager::getId() const {
    return id;
}

bool CoasterPager::isReady() const {
    return (myPager->ready.load() && !myPager->collected.load() &&
            !myPager->expired.load() && !myPager->broken.load());
}

std::unique_ptr<Product> MachineWrapper::getProduct(unsigned int id,
                                                    std::condition_variable &cv) {
    start();
    try {
        std::unique_lock<std::mutex> lock1(gettingMutex);
        if (machineConditionVariables.front().second != id) {
            cv.wait(lock1, [this, id]() {
                std::unique_lock<std::mutex> lock3(queueMutex);
                return machineConditionVariables.front().second == id;
            });
        }
        queueMutex.lock();
        machineConditionVariables.pop();
        queueMutex.unlock();
        std::unique_lock<std::mutex> lock2(checkingMutex);
        if (machineBroken) {
            std::unique_lock<std::mutex> lock3(queueMutex);
            if (!machineConditionVariables.empty()) {
                machineConditionVariables.front().first.notify_one();
            }
            lock3.unlock();
            throw MachineFailure();
        }
        lock2.unlock();
        std::unique_ptr<Product> product = machine.get()->getProduct();
        std::unique_lock<std::mutex> lock3(queueMutex);
        if (!machineConditionVariables.empty()) {
            machineConditionVariables.front().first.notify_one();
        }
        return product;
    } catch (MachineFailure &e) {
        checkingMutex.lock();
        machineBroken = true;
        checkingMutex.unlock();
        std::unique_lock<std::mutex> lock3(queueMutex);
        if (!machineConditionVariables.empty()) {
            machineConditionVariables.front().first.notify_one();
        }
        lock3.unlock();
        throw e;
    }
}

void MachineWrapper::returnProduct(std::unique_ptr<Product> product) {
    std::unique_lock<std::mutex> lock(returningMutex);
    machine->returnProduct(std::move(product));
}

void MachineWrapper::start() {
    std::unique_lock<std::mutex> lock(checkingMutex);
    if (machineWorking) {
        return;
    }
    machine.get()->start();
    machineWorking = true;
}

void MachineWrapper::stop() {
    machine.get()->stop();
    machineWorking = false;
}

