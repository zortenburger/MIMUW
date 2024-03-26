#ifndef WORLDCUP_DICE_H
#define WORLDCUP_DICE_H
#include "const.h"
#include "exceptions.h"
#include "worldcup.h"

class Dice {
private:
    std::vector<std::shared_ptr<Die>> dice_;
public:
    Dice() = default;

    void addDie(std::shared_ptr<Die> die) {
        dice_.emplace_back(die);
    }

    size_t size() {
        return dice_.size();
    }

    uint64_t roll() {
        uint64_t sum = 0;
        for (auto &die : dice_) {
            sum += die->roll();
        }
        return sum;
    }

    ~Dice() = default;
};


#endif //WORLDCUP_DICE_H
