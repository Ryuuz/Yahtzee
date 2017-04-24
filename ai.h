#ifndef AI_H
#define AI_H

#include <cmath>
#include "scoreboard.h"

class AI
{
    public:
        AI(std::vector<Die*> &dice, Scoreboard *score);
        ~AI();
        int choose_action();
        void calculate_best_move(int index);
        void free_all_dice();
        void flush_rows();
        int save_highest();

        void check_upper(int face);
        void check_pair(int row);
        void check_two_pairs(int row);
        void check_three_kinds(int row);
        void check_four_kinds(int row);
        void check_house(int row);
        void check_small_straight(int row);
        void check_large_straight(int row);
        void use_chance(int row);
        void check_yahtzee(int row);

    private:
        int turn;
        const std::vector<Die*> &currentDice;
        double *probability;
        bool *rowsTaken;
        Scoreboard *scorecard;
};

#endif // AI_H
