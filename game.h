#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <random>

#include "button.h"
#include "ai.h"

class Game : public QGraphicsView
{
    Q_OBJECT

    public:
        Game();
        ~Game();
        QPixmap* load_texture(QString path);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void add_sum(int row, int player);
        void write_sum(int row, int player);
        void check_score();
        void new_turn();
        void block_input();
        void AI_move();
        void main_menu();

    public slots:
        void throw_dice();
        void mark_die(QPointF mousePos);
        void unmark_die(QPointF mousePos);
        void rules_menu();
        void start_game();

    private:
        int turn;
        int round;
        bool won;

        QGraphicsScene *scene;

        std::vector<QPixmap*> dieTex;
        QPixmap *scorecardTex;
        QPixmap *throwButtonTex;
        QPixmap *startButtonTex;
        QPixmap *rulesButtonTex;
        QPixmap *marked;
        QPixmap *chosen;
        QPixmap *menu;
        QPixmap *rules;

        QGraphicsPixmapItem *hover;
        Button *throwButton;
        Scoreboard *scorecard;
        AI *opponent;

        std::vector<Die*> dice;
        bool *takenRows;
        int *scores;

        std::mt19937 RNG;
        std::uniform_int_distribution<int> range;
};

#endif // GAME_H
