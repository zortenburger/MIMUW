#ifndef WORLDCUP_PLAYER_H
#define WORLDCUP_PLAYER_H

#include "const.h"

// Class representing a player in the game
class Player {
private:
    uint64_t money_;
    uint64_t position_;
    uint64_t yellow_card_duration_;
    std::string name_;
    std::string square_;
    std::string status_;
    bool eliminated_;

public:
// Constructor
    explicit Player(std::string name_) : money_(START_MONEY), position_(0),
                                         yellow_card_duration_(0),
                                         name_(std::move(name_)),
                                         square_("Poczatek sezonu"),
                                         status_("w grze"), eliminated_(false) {};

    ~Player() = default;

// Getters
    uint64_t money() const { return money_; }

    uint64_t position() const { return position_; }

    uint64_t yellow_card_duration() const { return yellow_card_duration_; }

    std::string name() const { return name_; }

    std::string status() const { return status_; }

    std::string square() const { return square_; }

// Setters

    void set_status(std::string status) { status_ = std::move(status); }

    void set_yellow_card_duration(
            uint64_t yellow_card_duration) {
        yellow_card_duration_ = yellow_card_duration;
        if (yellow_card_duration <= 0) {
            set_status("w grze");
            return;
        }
        set_status("*** czekanie: " + std::to_string(yellow_card_duration) +
                   " ***");
    }


// Functions
    void move(uint64_t position) {
        position_ = position;
    }

    void moveToSquare(std::string square) {
        square_ = std::move(square);
    }

    uint64_t pay(uint64_t amount) {
        uint64_t money_to_pay = std::min(money_, amount);
        if (money_ < amount) {
            eliminated_ = true;
            set_status("*** bankrut ***");
            money_ = 0;
            return money_to_pay;
        }
        money_ -= amount;
        return money_to_pay;
    }

    void receive(uint64_t amount) {
        money_ += amount;
    }

    bool isEliminated() const {
        return eliminated_;
    }
};


#endif //WORLDCUP_PLAYER_H
