#ifndef SIMONMODEL_H
#define SIMONMODEL_H

#include <QObject>
#include <QTimer>
#include <queue>

class SimonModel : public QObject
{
    Q_OBJECT
private:
    enum Colors { RED = 0, BLUE = 1 };
    int currentScore;
    bool playersTurn;
    //TODO: int highScore;
    std::vector<Colors> sequence;       // Full sequence of colors
    std::queue<Colors> currentTurn;     // Remaining flashes
    int playerIndex = 0;                // position during turn


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
    void playerPressed(int colorIndex);
};

#endif // SIMONMODEL_H
