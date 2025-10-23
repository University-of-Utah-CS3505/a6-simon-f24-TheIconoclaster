#ifndef SIMONMODEL_H
#define SIMONMODEL_H

#include <QObject>
#include <QTimer>
#include <queue>

class SimonModel : public QObject
{
    Q_OBJECT
private:
    enum Colors { RED, BLUE };
    int currentScore;
    bool playersTurn;
    //TODO: int highScore;
    std::vector<Colors> sequence;
    std::queue<Colors> currentTurn;
    float speedFactor;
    int progress;
    QTimer timer;

public:
    explicit SimonModel(QObject *parent = nullptr);

signals:
    void updateProgress();

public slots:
    void startGame();
    void nextTurn();
    void endGame();
};

#endif // SIMONMODEL_H
