#ifndef WORLDCUP_FIELD_H
#define WORLDCUP_FIELD_H

#include "const.h"
#include "player.h"

// Structure representing a field on the board
class Field {
protected:
    std::string name_{};
public:
    virtual void standAction(Player &player) = 0;

    virtual void passAction(Player &player) {
        (void) player;
    };

    virtual ~Field() = default;

    void setSquare(Player &player) {
        player.moveToSquare(name_);
    }

    explicit Field(std::string name) {
        name_ = name;
    }
};

class FREE_DAY : public Field {
public:
    FREE_DAY() : Field("Dzień wolny od treningu") {};

    void standAction(Player &player) override {
        player.moveToSquare(name_);
    }
};

class PassableField : public Field {
public :
    explicit PassableField(std::string name) : Field(name) {};
    virtual void passAction(Player &player) = 0;
};

class SEASON_START : public PassableField {
private:
    uint64_t reward;
public:
    SEASON_START() : PassableField("Początek sezonu"),
                     reward(SEASON_START_REWARD) {};

    void standAction(Player &player) override {
        passAction(player);
    }

    void passAction(Player &player) override {
        player.receive(reward);
        player.moveToSquare(name_);
    }

};

class PENALTY : public Field {
private:
    uint64_t penalty;
public:
    explicit PENALTY(uint64_t penalty) : Field("Rzut karny"), penalty(penalty) {};

    void standAction(Player &player) override {
        player.pay(penalty);
        player.moveToSquare(name_);
    }
};

class BOOKMAKER : public Field {
private:
    uint64_t reward;
    uint64_t stood;
    uint64_t time;
public:
    explicit BOOKMAKER(uint64_t reward, uint64_t time) : Field("Bukmacher"), reward(reward),
                                     stood(0), time(time) {};

    void standAction(Player &player) override {
        if (stood == 0) {
            player.receive(reward);
        }
        else {
            player.pay(reward);
        }
        stood++;
        stood %= 3;
        player.moveToSquare(name_);
    }
};

class GOAL : public Field {
private:
    uint64_t reward;
public:
    explicit GOAL(uint64_t reward) : Field("Gol"), reward(reward) {};

    void standAction(Player &player) override {
        player.receive(reward);
        player.moveToSquare(name_);
    }
};

class YELLOW_CARD : public Field {
private:
    uint64_t duration;
public:
    explicit YELLOW_CARD(uint64_t duration) : Field("Żółta kartka"),
                                         duration(duration) {};

    void standAction(Player &player) override {
        player.set_yellow_card_duration(duration);
        player.moveToSquare(name_);
    }
};

class MATCH : public PassableField {
private:
    uint64_t payment;
    double weight;
    uint64_t waiting;

public:

    void standAction(Player &player) override {
        player.receive((uint64_t) (waiting * weight));
        player.moveToSquare(name_);
        waiting = 0;
    }

    void passAction(Player &player) override {
        uint64_t paid = player.pay(payment);
        waiting += paid;
    }

    MATCH(uint64_t payment, double weight, const std::string &team) :
            PassableField("Mecz z " + team), payment(payment),
            weight(weight),
            waiting(0) {};
};

#endif //WORLDCUP_FIELD_H
