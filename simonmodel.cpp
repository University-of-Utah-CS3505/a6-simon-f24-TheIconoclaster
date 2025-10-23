#include "simonmodel.h"

SimonModel::SimonModel(QObject *parent)
    : QObject{parent}
{
    currentScore = 0;
    playersTurn = false;
    speedFactor = 1;
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &SimonModel::nextTurn);
}

void SimonModel::startGame() {}
// comment
