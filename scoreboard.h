#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "die.h"
#include "consts.h"
#include <vector>

class Scoreboard
{
    public:
        Scoreboard(std::vector<Die*> &dice);
        int upper_half(int face);
        int pairs(bool twoPairs = false);
        int three_a_kind();
        int four_a_kind();
        int small_straight();
        int large_straight();
        int full_house();
        int chance();
        int yahtzee();

    private:
        const std::vector<Die*> &allDice;
};

#endif // SCOREBOARD_H
