#include "ai.h"


AI::AI(std::vector<Die*> &dice, Scoreboard *score) : turn(1), currentDice(dice), scorecard(score)
{
    probability = new double[Consts::rows] {};
    rowsTaken = new bool[Consts::rows] {};
}


AI::~AI()
{
    delete[] probability;
    delete[] rowsTaken;
}


//Checks available rows and chooses the one that gives the highest score
int AI::choose_action()
{
    //Makes sure probability array is clean
    for(int i = 0; i < Consts::rows; i++)
    {
        probability[i] = 0;
    }

    //Fills the probability array to find best move
    for(int i = 0; i < Consts::rows; i++)
    {
        if(!rowsTaken[i])
        {
            calculate_best_move(i);
        }
    }

    int index = 0;
    float highestSum = 0;
    //Goes through the rows to decide which to choose
    for(int i = 0; i < Consts::rows; i++)
    {
        if(probability[i] > highestSum && !rowsTaken[i])
        {
            highestSum = probability[i];
            index = i;
        }
    }

    calculate_best_move(index);
    turn++;

    //Saves move if it's the third throw
    if(turn == 4)
    {
        rowsTaken[index] = true;
        turn = 1;
    }

    return index;
}


void AI::calculate_best_move(int index)
{
    switch (index)
    {
    case 0: check_upper(index+1);
        break;

    case 1: check_upper(index+1);
        break;

    case 2: check_upper(index+1);
        break;

    case 3: check_upper(index+1);
        break;

    case 4: check_upper(index+1);
        break;

    case 5: check_upper(index+1);
        break;

    case 8: check_pair(index);
        break;

    case 9: check_two_pairs(index);
        break;

    case 10: check_three_kinds(index);
        break;

    case 11: check_four_kinds(index);
        break;

    case 12: check_small_straight(index);
        break;

    case 13: check_large_straight(index);
        break;

    case 14: check_house(index);
        break;

    case 15: if(turn == 3)
        {
            use_chance(index);
        }
        break;

    case 16: check_yahtzee(index);
        break;

    default:
        break;
    }
}


//Frees all the dice that has been saved
void AI::free_all_dice()
{
    for(auto &die : currentDice)
    {
        if(die->is_saved())
        {
            die->toggle_saved();
        }
    }
}


//Clears 'rowsTaken' for a new round
void AI::flush_rows()
{
    for(int i = 0; i < Consts::rows; i++)
    {
        rowsTaken[i] = false;
    }
}


//Save the die with the highest value
int AI::save_highest()
{
    int index = 0;
    int highest = 0;

    for(int i = 1; i < Consts::numOfDice; i++)
    {
        if(currentDice[i]->get_value() > highest)
        {
            highest = currentDice[i]->get_value();
            index = i;
        }
    }

    currentDice[index]->toggle_saved();

    return highest;
}


//Checks which value it has the most of
void AI::check_upper(int face)
{
    int value = scorecard->upper_half(face);
    free_all_dice();

    if(value)
    {
        probability[face-1] = (1.0 * value);

        for(auto &die : currentDice)
        {
            if(die->get_value() == face)
            {
                die->toggle_saved();
            }
        }
    }
}


//Checks if it has a pair, if not, it saves dice with high values in an attempt to get a pair. Row 8
void AI::check_pair(int row)
{
//    int value = scorecard->one_pair();
    int value = scorecard->pairs();
    free_all_dice();

    if(value)
    {   
        int count = 0;
        for(auto &die : currentDice)
        {
            if(die->get_value() == (value/2) && count < 2)
            {
                die->toggle_saved();
                count++;
            }
        }

        probability[row] = 1.0 + value;
    }
    //Else it saves the die with highest value
    else
    {
        value = save_highest();

        probability[row] = (4.0/6.0) * value;
    }
}


//Looks for two pairs. Row 9
void AI::check_two_pairs(int row)
{
//    int value= scorecard->two_pairs();
    int value= scorecard->pairs(true);
    free_all_dice();

    if(value)
    {
        bool saved = false;

        //Saves one pair
        check_pair(row-1);

        //Saves the pair that is different from the first pair
        for(int i = 0; i < Consts::numOfDice; i++)
        {
            for(int j = i+1; j < Consts::numOfDice; j++)
            {
                if(currentDice[i]->get_value() == currentDice[j]->get_value() && currentDice[i]->get_value() != (scorecard->pairs()/2) && !saved)
                {
                   currentDice[i]->toggle_saved();
                   currentDice[j]->toggle_saved();
                   saved = true;
                }
            }
        }

        probability[row] = 1.0 * value;
    }
    //Else if it can find one pair
    else if(scorecard->pairs())
    {
        int maxValue = 0;
        int index = 0;
        value = scorecard->pairs();

        //Saves one pair
        check_pair(row-1);

        //Saves die with highest value that is different than the value of the first pair
        for(int i = 0; i < Consts::numOfDice; i++)
        {
            if(currentDice[i]->get_value() > maxValue && currentDice[i]->get_value() != (value/2))
            {
                maxValue = currentDice[i]->get_value();
                index = i;
            }
        }

        currentDice[index]->toggle_saved();
        value += currentDice[index]->get_value();

        probability[row] = (2.0/6.0) * value;
    }
    //Else saves the die with highest value
    else
    {
        value = save_highest();

        probability[row] = ((4.0/6.0)*1.0*(2.0/6.0)) * value;
    }
}


//Checks for three of a kind. Row 10
void AI::check_three_kinds(int row)
{
    int value = scorecard->three_a_kind();
    free_all_dice();

    if(value)
    {
        int count = 0;

        //Saves the dice there are three of
        for(auto &die : currentDice)
        {
            if(die->get_value() == (value/3) && count < 3)
            {
                die->toggle_saved();
                count++;
            }
        }

        probability[row] = 1.0 * value;
    }
    //Else if it has two of a kind
    else if(scorecard->pairs())
    {
        value = scorecard->pairs();

        check_pair(row-2);
        probability[row] = (3.0/6.0) * value;
    }
    //Else just save the die with highest value
    else
    {
        value = save_highest();

        probability[row] = (4.0/6.0) * value;
    }
}


//Looks for four of a kind. Row 11
void AI::check_four_kinds(int row)
{
    int value = scorecard->four_a_kind();
    free_all_dice();

    if(value)
    {
        int count = 0;

        //Saves the dice there are four of
        for(auto &die : currentDice)
        {
            if(die->get_value() == (value/4) && count < 4)
            {
                die->toggle_saved();
                count++;
            }
        }

        probability[row] = 1.0 * value;
    }
    //Else if it has three of a kind
    else if(scorecard->three_a_kind())
    {
        value = scorecard->three_a_kind();

        //Saves the dice there are three of
        check_three_kinds(row-1);

        probability[row] = (2.0/6.0) * value;
    }
    //Else if it has two of a kind
    else if(scorecard->pairs())
    {
        value = scorecard->pairs();

        //Saves the dice there are two of
        check_pair(row-3);

        probability[row] = (3.0/6.0) * value;
    }
    //Else just save the die with highest value
    else
    {
        value = save_highest();

        probability[row] = (4.0/6.0) * value;
    }
}


//Checks for small straight. Row 12
void AI::check_small_straight(int row)
{
    int value = scorecard->small_straight();
    free_all_dice();

    if(value)
    {
        for(auto &die : currentDice)
        {
            die->toggle_saved();
        }

        probability[row] = 1.0 * 15.0;
    }
    //Else check how close it is to a small straight
    else
    {
        bool array[Consts::facesOfDie] = {};
        int count = 0;

        for(auto &die : currentDice)
        {
            if(!array[(die->get_value()-1)] && die->get_value() != 6)
            {
                die->toggle_saved();
                array[(die->get_value()-1)] = true;
                count++;
            }
        }

        //Gives probability[12] a different value than 0 for the for loop
        probability[row] = ((static_cast<double>(Consts::numOfDice) - count)/Consts::facesOfDie);

        //Each die has one less value it can be for small straight
        for(int i = (Consts::numOfDice - count) - 1; i > 0; i--)
        {
            probability[row] *= (static_cast<double>(i)/Consts::facesOfDie);
        }

        probability[row] *= 15.0;
    }
}


//Checks for large straight. Row 13
void AI::check_large_straight(int row)
{
    int value = scorecard->large_straight();
    free_all_dice();

    if(value)
    {
        for(auto &die : currentDice)
        {
            die->toggle_saved();
        }

        probability[row] = 1.0 * 20.0;
    }
    //Checks how many different values it has to know how close it is to a large straight
    else
    {
        bool array[Consts::facesOfDie] = {};
        int count = 0;

        for(auto &die : currentDice)
        {
            if(!array[(die->get_value()-1)] && die->get_value() != 1)
            {
                die->toggle_saved();
                array[(die->get_value()-1)] = true;
                count++;
            }
        }

        //To give probability[13] a different value than 0
        probability[row] = ((static_cast<double>(Consts::numOfDice) - count)/Consts::facesOfDie);

        //Each die has one value less it can be for large straight
        for(int i = (Consts::numOfDice - count) - 1; i > 0; i--)
        {
            probability[row] *= (static_cast<double>(i)/Consts::facesOfDie);
        }

        probability[row] *= 20.0;
    }
}


//Checks if it has a full house. Row 14
void AI::check_house(int row)
{
    int value = scorecard->full_house();
    free_all_dice();

    if(value)
    {
        for(auto &die : currentDice)
        {
            die->toggle_saved();
        }

        probability[row] = 1.0 * value;
    }
    //Else look for three of a kind
    else if(scorecard->three_a_kind())
    {
        value = scorecard->three_a_kind();

        check_three_kinds(row-4);

        //Save the one with the highest value among the unsaved ones
        int maxValue = 0;
        int index = 0;

        for(int i = 0; i < Consts::numOfDice; i++)
        {
            if(currentDice[i]->get_value() > maxValue && !currentDice[i]->is_saved())
            {
                index = i;
                maxValue = currentDice[i]->get_value();
            }
        }

        currentDice[index]->toggle_saved();

        probability[row] = (1.0/6.0) * (value + maxValue);
    }
    //Else if it has two of a kind
    else if(scorecard->pairs())
    {
        value = scorecard->pairs();

        check_pair(row-6);

        //Saves the die with the highest value that hasn't already been saved
        int maxValue = 0;
        int index = 0;

        for(int i = 0; i < Consts::numOfDice; i++)
        {
            if(currentDice[i]->get_value() > maxValue && !currentDice[i]->is_saved())
            {
                index = i;
                maxValue = currentDice[i]->get_value();
            }
        }

        currentDice[index]->toggle_saved();

        probability[row] = (pow((1.0/6.0), 2)*(5.0/6.0)) * (value + maxValue);
    }
    //Else just save the dice with highest value
    else
    {
        value = save_highest();
        probability[row] = (pow((1.0/6.0), 3)*(5/6)) * value;
    }
}


//Takes whatever values it has. Only used after third throw when no other option is better. Row 15
void AI::use_chance(int row)
{
    int value = 0;

    for(auto &die : currentDice)
    {
        value += die->get_value();
    }

    //Don't want the probability too high so that it'll choose too easily
    probability[row] = (1.0/6.0) * value;
}


//Checks for yahtzee. Row 16
void AI::check_yahtzee(int row)
{
    free_all_dice();
    int value = scorecard->yahtzee();

    if(value)
    {
        for(auto &die : currentDice)
        {
            die->toggle_saved();
        }
        probability[row] = 1.0 * 30.0;
    }
    //Else find the dice with the highest value that it has the most of and save them
    else
    {
        int faceValue = 0;
        int maxCount = 0;

        for(int i = 1; i <= Consts::facesOfDie; i++)
        {
            int count = 0;
            for(auto &die : currentDice)
            {
                if(die->get_value() == i)
                {
                    count++;
                }
            }

            if(count > maxCount)
            {
                maxCount = count;
                faceValue = i;
            }
        }

        for(auto &die : currentDice)
        {
            if(die->get_value() == faceValue)
            {
                die->toggle_saved();
            }
        }

        probability[row] = pow((1.0/6.0), (Consts::numOfDice - maxCount)) * 30.0;
    }
}
