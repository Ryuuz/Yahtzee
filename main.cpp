#include <QApplication>
#include "game.h"

//Yahtzee game made By Camilla Merete Odegaard

//AI was inspired by TANSTAAFL's post at http://www.gamedev.net/topic/5372-ai-in-yahtzee/
//For the mouse hover functionality, I used mcosta's suggestion at https://forum.qt.io/topic/25861/mouse-cursor-over-qwidget
//To get the RNG correct, I followed juanchopanza post at http://stackoverflow.com/questions/25009179/generate-random-numbers-no-member-generate-exist-error
//Additionally, I used BeautiCode's example at http://www.qtcentre.org/threads/37276-How-do-I-append-and-int-to-my-QString to make an int to QString
//and the documentation for QT found at http://doc.qt.io/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Game *game;

    game = new Game();
    game->show();
    game->main_menu();

    return a.exec();
}
