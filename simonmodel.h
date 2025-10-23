#ifndef SIMONMODEL_H
#define SIMONMODEL_H

#include <QObject>
#include <QTimer>
#include <queue>

class SimonModel : public QObject
{
    Q_OBJECT

public:
    enum Colors { RED = 0, BLUE = 1 };
    explicit SimonModel(QObject *parent = nullptr);

    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }
    bool isPlayersTurn() const { return playersTurn; }

signals:
    void gameStarted();
    void gameEnded(int finalScore);
    void progressUpdated(int progress);
    void colorToFlash(int colorIndex);
    void turnStarted(int sequenceLength);
    void scoreChanged(int score);
    void highScoreChanged(int highScore);

public slots:
    void startGame();
    void playerPressed(int colorIndex);

private slots:
    void processNextFlash();
    void startPlayerTurn();

private:
    int currentScore;
    bool playersTurn;
    int highScore;
    bool gameActive;
    std::vector<Colors> sequence;       // Full sequence of colors
    std::queue<Colors> flashQueue;     // Remaining flashes
    int playerIndex = 0;                // position during turn


    float speedFactor;
    int playerInputIndex = 0;
    QTimer flashTimer;
    QTimer playerTurnTimer;

    void nextTurn();
    void endGame();
    void addRandomColor();
    void flashSequence();
    void resetGameState();

    static const int INITIAL_FLASH_DELAY = 1000;  // milliseconds
    static const int MIN_FLASH_DELAY = 300;      // minimum speed
    static const int SPEED_DECREMENT = 50;       // how much faster each turn

};

#endif // SIMONMODEL_H
