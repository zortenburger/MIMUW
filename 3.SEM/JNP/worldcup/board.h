#ifndef WORLDCUP_BOARD_H
#define WORLDCUP_BOARD_H
#include "const.h"
#include "field.h"

class Board{
private:
    std::vector<std::shared_ptr<Field>> fields_;
public:
    Board() {
        fields_.push_back(std::make_shared<SEASON_START>());
        fields_.push_back(std::make_shared<MATCH>(160, FRIENDLY_WEIGHT, "San Marino"));
        fields_.push_back(std::make_shared<FREE_DAY>());
        fields_.push_back(std::make_shared<MATCH>(220, FRIENDLY_WEIGHT, "Lichtensteinem"));
        fields_.push_back(std::make_shared<YELLOW_CARD>(3));
        fields_.push_back(std::make_shared<MATCH>(300, POINTS_WEIGHT, "Meksykiem"));
        fields_.push_back(std::make_shared<MATCH>(280, POINTS_WEIGHT, "Arabią Saudyjską"));
        fields_.push_back(std::make_shared<BOOKMAKER>(100, 3));
        fields_.push_back(std::make_shared<MATCH>(250, POINTS_WEIGHT, "Argentyną"));
        fields_.push_back(std::make_shared<GOAL>(120));
        fields_.push_back(std::make_shared<MATCH>(400, FINAL_WEIGHT, "Francją"));
        fields_.push_back(std::make_shared<PENALTY>(180));
    }

    Field &operator[](size_t index) {
        return *fields_[index];
    }

    ~Board() = default;
};

#endif //WORLDCUP_BOARD_H
