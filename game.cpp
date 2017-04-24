#include "game.h"


Game::Game() : turn(0), round(1), won(true), RNG(std::random_device()()), range(1, 6)
{
    scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, Consts::screenWidth, Consts::screenHeight);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(Consts::screenWidth + 2, Consts::screenHeight + 2);

    //Load textures
    for(int i = 1; i <= Consts::facesOfDie; i++)
    {
        dieTex.push_back(load_texture(":/images/Yahtzee/die" + QString::number(i) + ".png"));
    }
    scorecardTex = load_texture(":/images/Yahtzee/scorecard.png");
    throwButtonTex = load_texture(":/images/Yahtzee/throwButton.png");
    startButtonTex = load_texture(":/images/Yahtzee/startButton.png");
    rulesButtonTex = load_texture(":/images/Yahtzee/rulesButton.png");
    marked = load_texture(":/images/Yahtzee/savedDie.png");
    chosen = load_texture(":/images/Yahtzee/chooseRow.png");
    menu = load_texture(":/images/Yahtzee/menu.png");
    rules = load_texture(":/images/Yahtzee/rules.png");

    //Make objects
    for(int i = 0; i < Consts::numOfDice; i++)
    {
        dice.push_back(new Die(dieTex[0]));
        connect(dice[i], SIGNAL(save(QPointF)), this, SLOT(mark_die(QPointF)));
        connect(dice[i], SIGNAL(free(QPointF)), this, SLOT(unmark_die(QPointF)));
    }

    throwButton = new Button(throwButtonTex);
    connect(throwButton, SIGNAL(clicked()), this, SLOT(throw_dice()));

    takenRows = new bool[Consts::rows*2] {};
    scores = new int[Consts::rows*2] {};   
    scorecard = new Scoreboard(dice);
    opponent = new AI(dice, scorecard);
}


Game::~Game()
{
    //Delete objects
    for(auto &element : dice)
    {
        delete element;
    }
    delete scorecard;
    delete throwButton;
    delete opponent;
    delete[] takenRows;
    delete[] scores;

    //Delete textures
    for(auto &element : dieTex)
    {
        delete element;
    }
    delete scorecardTex;
    delete throwButtonTex;   
    delete marked;

    delete scene;
}


//Loads texture 'path'. Exits program if it fails
QPixmap* Game::load_texture(QString path)
{
    QPixmap *tempPixmap = new QPixmap(path);

    if(tempPixmap->isNull())
    {
        exit(1);
    }

    return tempPixmap;
}


void Game::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QGraphicsView::mousePressEvent(event);

        //If user click on a row on the scorecard after first throw while game is still ongoing
        if(floor(((event->pos().y() - Consts::rowStart) / Consts::rowHeight)) < Consts::rows && floor(((event->pos().y() - Consts::rowStart) / Consts::rowHeight)) >= 0 && turn > 1 && !won)
        {
            add_sum(floor((event->pos().y() - Consts::rowStart) / Consts::rowHeight), 0);
        }
    }
}


//Marks the row the cursor hovers over
void Game::mouseMoveEvent(QMouseEvent *event)
{
    int activeRow = floor(((event->pos().y() - Consts::rowStart) / Consts::rowHeight));

    if(activeRow < (Consts::rows - 1) && activeRow >= 0 && activeRow != 6 && activeRow != 7 && !won)
    {
        if(hover)
        {
            scene->removeItem(hover);
            delete hover;
        }
        hover = scene->addPixmap(*chosen);
        hover->setPos(0, (Consts::rowStart + (Consts::rowHeight * activeRow))+1);
    }
    else
    {
        hover->setVisible(false);
    }
}


//Adds the sum for the player's chosen row
void Game::add_sum(int row, int player)
{
    if(!takenRows[row + (Consts::rows*player)] && row != 6 && row != 7 && row != 17)
    {
        int sum = 0;

        sum = scorecard->upper_half(row+1);

        switch (row) {
            case 0: sum = scorecard->upper_half(row+1);
                break;
            case 1: sum = scorecard->upper_half(row+1);
                break;
            case 2: sum = scorecard->upper_half(row+1);
                break;
            case 3: sum = scorecard->upper_half(row+1);
                break;
            case 4: sum = scorecard->upper_half(row+1);
                break;
            case 5: sum = scorecard->upper_half(row+1);
                break;
            case 8: sum = scorecard->pairs();
                break;
            case 9: sum = scorecard->pairs(true);
                break;
            case 10: sum = scorecard->three_a_kind();
                break;
            case 11: sum = scorecard->four_a_kind();
                break;
            case 12: sum = scorecard->small_straight();
                break;
            case 13: sum = scorecard->large_straight();
                break;
            case 14: sum = scorecard->full_house();
                break;
            case 15: sum = scorecard->chance();
                break;
            case 16: sum = scorecard->yahtzee();
                break;
            default:
                break;
        }

        takenRows[row + (Consts::rows*player)] = true;
        scores[row + (Consts::rows*player)] = sum;

        write_sum(row, player);

        if(player == 0)
        {
            turn = 4;
            block_input();
            setMouseTracking(false);
            AI_move();
        }
    }
}


//Writes the score on the board in row 'row' for 'player's column
void Game::write_sum(int row, int player)
{
    QGraphicsTextItem *drawSum = new QGraphicsTextItem(QString::number(scores[row+(Consts::rows*player)]));
    QFont textFont("Areal", 20);
    drawSum->setFont(textFont);
    drawSum->setPos(((Consts::columnStart + (Consts::columnWidth*player)) + ((Consts::columnWidth / 2) - (drawSum->boundingRect().width() / 2))), (Consts::rowStart + (Consts::rowHeight * row)) - 5);
    scene->addItem(drawSum);
}


//Adds together the score and writes it on the scorecard
void Game::check_score()
{
    //If all of the player's upper rows are full, it calculates the score
    for(int i = 0; i < 2; i++)
    {
        if(round > 6 && !takenRows[6+(Consts::rows*i)])
        {
            int sum = 0;
            bool upperFull = true;

            for(int j = 0; j < 6; j++)
            {
                if(takenRows[j+(Consts::rows*i)])
                {
                    sum += scores[j+(Consts::rows*i)];
                }
                else
                {
                    upperFull = false;
                }
            }

            if(upperFull)
            {
                scores[6+(Consts::rows*i)] = sum;
                scores[7+(Consts::rows*i)] = 0;

                //Adds bonus if sum is 63 or higher
                if(sum > 62)
                {
                    scores[7+(Consts::rows*i)] = 50;
                }

                takenRows[6+(Consts::rows*i)] = true;
                takenRows[7+(Consts::rows*i)] = true;

                write_sum(6, i);
                write_sum(7, i);
            }
        }
    }

    //Writes the total score of each player once game is finished
    if(round == 17)
    {
        int sum;

        for(int i = 0; i < 2; i++)
        {
            sum = 0;

            for(int j = 6; j < (Consts::rows - 1); j++)
            {
                sum += scores[j+(Consts::rows*i)];
            }

            scores[17+(Consts::rows*i)] = sum;
            takenRows[17+(Consts::rows*i)] = true;

            write_sum(17, i);
        }

        //Ends the game
        won = true;
        block_input();
        main_menu();
    }

    setMouseTracking(true);
}


//Resets the 'turn' count, frees dice, and makes throw button clickable
void Game::new_turn()
{
    turn = 1;

    for(auto &die : dice)
    {
        if(die->is_saved())
        {
            die->toggle_saved();
        }
    }

    throwButton->setAcceptedMouseButtons(Qt::LeftButton);
    throwButton->setOpacity(1.0);

    round++;
    check_score();
}


//User will no longer be able to throw or save/free dice
void Game::block_input()
{
    for(auto &die : dice)
    {
        die->setAcceptedMouseButtons(0);
        die->setOpacity(0.5);
        if(die->is_saved())
        {
            die->toggle_saved();
        }
    }

    throwButton->setAcceptedMouseButtons(0);
    throwButton->setOpacity(0.5);
}


//AI makes its move
void Game::AI_move()
{
    int row;
    for(int i = 0; i < 3; i++)
    {
        throw_dice();
        row = opponent->choose_action();
    }

    add_sum(row, 1);
    new_turn();
}


//Displays the main menu
void Game::main_menu()
{
    QGraphicsPixmapItem *tempImage = scene->addPixmap(*menu);
    tempImage->setPos(0, 0);

    Button *startButton = new Button(startButtonTex);
    startButton->setPos((Consts::screenWidth/2) - (startButton->boundingRect().width()/2), (Consts::screenHeight/2) - startButton->boundingRect().height());
    connect(startButton, SIGNAL(clicked()), this, SLOT(start_game()));
    scene->addItem(startButton);

    Button *rulesButton = new Button(rulesButtonTex);
    rulesButton->setPos((Consts::screenWidth/2) - (rulesButton->boundingRect().width()/2), (Consts::screenHeight/2) + 10);
    connect(rulesButton, SIGNAL(clicked()), this, SLOT(rules_menu()));
    scene->addItem(rulesButton);
}


//Throws the dice
void Game::throw_dice()
{
    int value;

    for(auto &die : dice)
    {
        if(!die->is_saved())
        {
            value = range(RNG);
            die->change_value(value, dieTex[value-1]);
        }
    }

    turn++;

    //Makes it possible to save dice after first throw
    if(turn == 2)
    {
        for(auto &die : dice)
        {
            die->setAcceptedMouseButtons(Qt::LeftButton);
            die->setOpacity(1.0);
        }
    }
    //Can only throw three times each round
    else if(turn == 4)
    {
        block_input();
    }
}

//------------------Slots------------------

//Marks the die as saved
void Game::mark_die(QPointF mousePos)
{
    mousePos.rx() -= Consts::markedBorder;
    mousePos.ry() -= Consts::markedBorder;

    QGraphicsPixmapItem *tempImage = scene->addPixmap(*marked);
    tempImage->setPos(mousePos);
}


//Removes the mark so the die can once again be thrown
void Game::unmark_die(QPointF mousePos)
{
    QGraphicsItem *tempItem = scene->itemAt(mousePos, QTransform());
    scene->removeItem(tempItem);
    delete tempItem;
}


//Displays the rules
void Game::rules_menu()
{
    auto allObjects = scene->items();

    for(int i = 0; i < 3; i++)
    {
        scene->removeItem(allObjects[i]);
        delete allObjects[i];
    }

    QGraphicsPixmapItem *tempImage = scene->addPixmap(*rules);
    tempImage->setPos(0, 0);

    Button *startButton = new Button(startButtonTex);
    startButton->setPos((Consts::screenWidth/2) - (startButton->boundingRect().width()/2), (Consts::screenHeight - startButton->boundingRect().height() - 90));
    connect(startButton, SIGNAL(clicked()), this, SLOT(start_game()));
    scene->addItem(startButton);
}


//Initiates all variables and draws all objects to screen
void Game::start_game()
{
    auto allObjects = scene->items();
    bool aDie;

    //Remove all objects from scene. Delete if it isn't a die or the throwButton
    for(auto &obj : allObjects)
    {
        aDie = false;
        scene->removeItem(obj);

        for(auto &die : dice)
        {
            if(obj == die)
            {
                aDie = true;
            }
        }

        if(!aDie && obj != throwButton)
        {
            delete obj;
        }
    }

    int xPos = 10;
    round = 1;
    won = false;
    opponent->flush_rows();

    QGraphicsPixmapItem *tempImage = scene->addPixmap(*scorecardTex);
    tempImage->setPos(0, 0);

    throwButton->setPos(xPos, Consts::screenHeight - throwButton->boundingRect().height() - 15);
    scene->addItem(throwButton);

    xPos += (throwButton->boundingRect().width() + 26);

    for(auto &die : dice)
    {
        die->change_pixmap(dieTex[0]);
        die->setPos(xPos, Consts::screenHeight - die->boundingRect().height() - 15);
        scene->addItem(die);

        xPos += (die->boundingRect().width() + 26);
    }

    for(int i = 0; i < (Consts::rows*2); i++)
    {
        takenRows[i] = false;
        scores[i] = 0;
    }

    setMouseTracking(true);
    hover = scene->addPixmap(*chosen);
    hover->setPos(0, -30);

    block_input();
    new_turn();
}
