#include "scoreboard.h"


Scoreboard::Scoreboard(std::vector<Die*> &dice) : allDice(dice)
{

}

//Checks how many dice have value 'face' and returns the sum of them
int Scoreboard::upper_half(int face)
{
    int sum = 0;

    for(auto &die : allDice)
    {
        if(die->get_value() == face)
        {
           sum += die->get_value();
        }
    }

    return sum;
}


//Finds one pair is parameter is false, and two pairs if true.
int Scoreboard::pairs(bool twoPairs)
{
    int sumOne = 0;

    for(int i = 0; i < Consts::numOfDice; i++)
    {
        for(int j = i+1; j < Consts::numOfDice; j++)
        {
            //Finds pair of highest value
            if(allDice[i]->get_value() == allDice[j]->get_value() && sumOne < (allDice[i]->get_value() * 2))
            {
                sumOne = (allDice[i]->get_value() * 2);
            }
        }
    }

    //If it found one pair and we're supposed to find another
    if(twoPairs && sumOne)
    {
        int sumTwo = 100;

        for(int i = 0; i < Consts::numOfDice; i++)
        {
            for(int j = i+1; j < Consts::numOfDice; j++)
            {
                //Finds pair of lowest value
                if(allDice[i]->get_value() == allDice[j]->get_value() && sumTwo > (allDice[i]->get_value() * 2))
                {
                    sumTwo = (allDice[i]->get_value() * 2);
                }
            }
        }

        //If it found a second pair and it is different from the first
        if(sumTwo != 100 && sumTwo != sumOne)
        {
            return sumOne + sumTwo;
        }
    }

    return sumOne;
}


//Looks for three of a kind
int Scoreboard::three_a_kind()
{
    for(int i = 0; i < Consts::numOfDice; i++)
    {
        int kinds = 1;
        for(int j = i+1; j < Consts::numOfDice; j++)
        {
            if(allDice[i]->get_value() == allDice[j]->get_value())
            {
                kinds++;
            }
        }
        if(kinds >= 3)
        {
            return (allDice[i]->get_value() * 3);
        }
    }

    return 0;
}


//Looks for four of a kind
int Scoreboard::four_a_kind()
{
    for(int i = 0; i < Consts::numOfDice; i++)
    {
        int kinds = 1;
        for(int j = i+1; j < Consts::numOfDice; j++)
        {
            if(allDice[i]->get_value() == allDice[j]->get_value())
            {
                kinds++;
            }
        }
        if(kinds >= 4)
        {
            return (allDice[i]->get_value() * 4);
        }
    }

    return 0;
}


//Looks for small straight by going through each required number and checking if there's a die with the corresponding value
int Scoreboard::small_straight()
{
    bool straight = true;
    bool found;
    int start = 1;
    int stop = 5;

    for(int i = start; i <= stop; i++)
    {
        found = false;

        for(auto &die : allDice)
        {
            if(die->get_value() == i)
            {
                found = true;
            }
        }

        straight = straight && found;
    }

    if(straight)
    {
        return 15;
    }

    return 0;
}


//Looks for large straight. Same method as for small straight
int Scoreboard::large_straight()
{
    bool straight = true;
    bool found;

    for(int i = 2; i <= 6; i++)
    {
        found = false;

        for(auto &die : allDice)
        {
            if(die->get_value() == i)
            {
                found = true;
            }
        }

        straight = straight && found;
    }

    if(straight)
    {
        return 20;
    }

    return 0;
}


//Checks if it finds three of a kind, then checks if the remaining dice make a pair
int Scoreboard::full_house()
{
    int three = three_a_kind();
    int temp = 0;

    if(three)
    {
        for(auto &die : allDice)
        {
            if(die->get_value() != (three/3))
            {
                if(die->get_value() == temp)
                {
                    return three + (temp * 2);
                }

                temp = die->get_value();
            }
        }
    }

    return 0;
}


//Adds together all the dice and returns sum
int Scoreboard::chance()
{
    int sum = 0;

    for(auto &die : allDice)
    {
        sum += die->get_value();
    }

    return sum;
}


//Checks if all the dice are the same value
int Scoreboard::yahtzee()
{
    int faceValue = allDice[0]->get_value();

    for(int i = 1; i < Consts::numOfDice; i++)
    {
        if(allDice[i]->get_value() != faceValue)
        {
            return 0;
        }
    }

    return 30;
}
