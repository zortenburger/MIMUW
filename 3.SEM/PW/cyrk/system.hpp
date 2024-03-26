#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <exception>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <future>
#include <queue>
#include <atomic>

#include "machine.hpp"

class FulfillmentFailure : public std::exception {
};

class OrderNotReadyException : public std::exception {
};

class BadOrderException : public std::exception {
};

class BadPagerException : public std::exception {
};

class OrderExpiredException : public std::exception {
};

class RestaurantClosedException : public std::exception {
};

struct WorkerReport {
    std::vector<std::vector<std::string>> collectedOrders;
    std::vector<std::vector<std::string>> abandonedOrders;
    std::vector<std::vector<std::string>> failedOrders;
    std::vector<std::string> failedProducts;
};


class MachineWrapper {
public:
    explicit MachineWrapper(std::shared_ptr<Machine> machine) :
            machine(std::move(machine)) {};

    std::unique_ptr<Product>
    getProduct(unsigned int id, std::condition_variable &cv);

    void returnProduct(std::unique_ptr<Product> product);

    void start();

    void stop();

    std::mutex queueMutex;
    std::queue<std::pair<std::condition_variable &, unsigned int>> machineConditionVariables;
    mutable std::mutex checkingMutex;
    std::mutex gettingMutex;
    std::mutex returningMutex;
    std::shared_ptr<Machine> machine;
    bool machineBroken = false;
    bool machineWorking = false;
};

class MyPager {
public:
    unsigned int id;
    std::condition_variable cv;
    std::atomic<bool> ready;
    std::atomic<bool> expired;
    std::atomic<bool> broken;
    std::atomic<bool> collected;
    std::condition_variable cv2;
    std::mutex productsMutex;
    std::vector<std::string> products;
    std::vector<std::string> whichProducts;
    std::vector<std::unique_ptr<Product>> collectedProducts;

    MyPager(unsigned int id, std::vector<std::string> products) :
            id(id), products(std::move(products)) {
        ready.store(false);
        expired.store(false);
        broken.store(false);
        collected.store(false);
    };
};

class System;

class CoasterPager {
public:
    void wait();

    void wait(unsigned int timeout);

    [[nodiscard]] unsigned int getId() const;

    [[nodiscard]] bool isReady() const;

private:
    CoasterPager(unsigned int id, MyPager *myPager) : id(id), myPager(myPager) {};

    unsigned int id;
    MyPager *myPager;

    friend class System;
};

class System {
public:
    typedef std::unordered_map<std::string, std::shared_ptr<Machine>> machines_t;

    System(const machines_t machines, unsigned int numberOfWorkers,
           unsigned int clientTimeout) : clientTimeout(clientTimeout){
        std::unique_lock<std::mutex> lock(workerMutex);
        running.store(true);
        nextOrderId.store(0);
        for (auto &machine: machines) {
            myMachines.emplace(machine.first, machine.second);
        }
        for (unsigned int i = 0; i < numberOfWorkers; i++) {
            workers.emplace_back(&System::work, this, i);
            workerReports.emplace_back();
        }
    }

    std::vector<WorkerReport> shutdown();

    std::vector<std::string> getMenu() const;

    std::vector<unsigned int> getPendingOrders() const;

    std::unique_ptr<CoasterPager> order(std::vector<std::string> products);

    std::vector<std::unique_ptr<Product>>
    collectOrder(std::unique_ptr<CoasterPager> CoasterPager);

    unsigned int getClientTimeout() const;

private:
    bool checkIfOrderCorrect(std::vector<std::string> &products);

    bool checkIfQueueNotEmpty();

    void work(unsigned int workerId);

    void returnAllProducts(MyPager &order);

    void waitForProduct(const std::string& product, unsigned int id,
                        std::promise<std::unique_ptr<Product>> promise,
                        std::condition_variable &cv);

    std::vector<std::thread> workers;
    std::mutex mutex;
    mutable std::mutex orderMutex;
    std::mutex workerMutex;
    std::condition_variable cvWorker;
    std::map<unsigned int, MyPager> orders;
    std::queue<unsigned int> ordersQueue;
    std::map<std::string, MachineWrapper> myMachines;
    std::vector<WorkerReport> workerReports;
    std::atomic_uint nextOrderId;
    std::atomic<bool> running;
    const unsigned int clientTimeout;
};

#endif // SYSTEM_HPP