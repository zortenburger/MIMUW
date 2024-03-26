#ifndef WORLDCUP_WORLDCUP2022_H
#define WORLDCUP_WORLDCUP2022_H \
#include "const.h"
#include "exceptions.h"
#include "field.h"
#include "player.h"
#include "board.h"
#include "worldcup.h"
#include "dice.h"


/*
 * TODO: sprawdzic jak robic ten vector pol
 * TODO: przetestowac
 */


class WorldCup2022 : public WorldCup {
private:
    Dice dice_;
    std::vector<Player> players_;
    uint64_t number_of_players_;
    std::shared_ptr<ScoreBoard> score_board_;
    Board board_;

    bool checkIfEliminated(Player &player, uint64_t position) {
        if (player.isEliminated()) {
            player.move(position);
            board_[position].setSquare(player);
            number_of_players_--;
            return true;
        }
        return false;
    }

    bool checkIfSomeoneWon() {
        if (number_of_players_ == 1) {
            for (auto &player : players_)
                if (!player.isEliminated())
                    score_board_->onWin(player.name());
            return true;
        }
        return false;
    }

    void pass(Player &player, uint64_t dice) {
        uint64_t position = player.position();
        uint64_t field;
        for (uint64_t i = 1; i < dice; i++) {
            field = (position + i) % BOARD_SIZE;
            board_[field].passAction(player);
            if (checkIfEliminated(player, (position + dice) % BOARD_SIZE)) {
                return;
            }
        }
        position = (position + dice) % BOARD_SIZE;
        player.move(position);
        board_[position].standAction(player);
        checkIfEliminated(player, position);
    }

public:
    WorldCup2022() {
        board_ = Board();
        number_of_players_ = 0;
    }

    void addDie(std::shared_ptr<Die> die) override {
        dice_.addDie(die);
    }

    void addPlayer(std::string const &name) override {
        players_.emplace_back(name);
        number_of_players_++;
    }

    void setScoreBoard(std::shared_ptr<ScoreBoard> scoreboard) override {
        score_board_ = scoreboard;
    }

    void play(unsigned int rounds) override {
        if (dice_.size() < DICES) {
            throw TooFewDiceException();
        }

        if (dice_.size() > DICES) {
            throw TooManyDiceException();
        }

        if (players_.size() < MIN_PLAYERS) {
            throw TooFewPlayersException();
        }

        if (players_.size() > MAX_PLAYERS) {
            throw TooManyPlayersException();
        }

        // Play the game
        unsigned int round = 0;
        while (round < rounds) {
            score_board_->onRound(round);

            // Loop through each player and have them take their turn
            for (auto &player: players_) {
                if (player.isEliminated()) {
                    continue;
                }

                // Check if the player is serving a yellow card penalty
                if (player.yellow_card_duration() < 1) {
                    unsigned int dice_roll = dice_.roll();

                    pass(player, dice_roll);


                    std::string current_square = player.square();
                }

                // Roll the dice

                score_board_->onTurn(player.name(), player.status(),
                                     player.square(), player.money());


                if (checkIfSomeoneWon()) {
                    return;
                }

                if (player.yellow_card_duration() > 0) {
                    player.set_yellow_card_duration(player.yellow_card_duration() - 1);
                }
            }
            round++;
        }

        //find winner
        uint64_t max_money = 0;
        std::string winner;
        for (auto &player: players_) {
            if (player.money() > max_money) {
                max_money = player.money();
                winner = player.name();
            }
        }
        score_board_->onWin(winner);
    }

    ~WorldCup2022() = default;
};
#endif //WORLDCUP_WORLDCUP2022_H
