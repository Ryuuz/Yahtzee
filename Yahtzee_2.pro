#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T00:57:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yahtzee_2
TEMPLATE = app

RC_FILE = yahtzeeIcon.rc

SOURCES += main.cpp \
    game.cpp \
    consts.cpp \
    die.cpp \
    button.cpp \
    scoreboard.cpp \
    ai.cpp

HEADERS  += \
    game.h \
    consts.h \
    die.h \
    button.h \
    scoreboard.h \
    ai.h

FORMS    +=

RESOURCES += \
    resources.qrc
