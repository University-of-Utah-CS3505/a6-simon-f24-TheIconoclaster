#ifndef SIMONMODEL_H
#define SIMONMODEL_H

#include <QObject>
#include <QTimer>
#include <queue>

/*
 * Simon Model
 * Model in the MV design. This class owns game and state logic:
 * Here we generate and store the Simon sequence, Drive coputer playback
 * timing using QTimers, and validate player input to update score.
 */


class SimonModel : public QObject
{
    Q_OBJECT

public:
    enum Colors { RED = 0, BLUE = 1 };      // The two button options for the player to pick
    explicit SimonModel(QObject *parent = nullptr);

    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }
    bool isPlayersTurn() const { return playersTurn; }

signals:
    // Game life cycle
    void gameStarted();
    void gameEnded(int finalScore);
    // Player turn
    void progressUpdated(int progress);
    void turnStarted(int sequenceLength);
    // Computer playback
    void colorToFlash(int colorIndex);
    // Creative addition -> scores
    void scoreChanged(int score);
    void highScoreChanged(int highScore);

public slots:
    // Called by the view
    void startGame();
    void playerPressed(int colorIndex);

private slots:
    void processNextFlash();
    void startPlayerTurn();

private:
    int currentScore;                   // For creative addition
    bool playersTurn;
    int highScore;                      // For creative addition
    bool gameActive;
    std::vector<Colors> sequence;       // Full sequence of colors
    std::queue<Colors> flashQueue;     // Remaining flashes
    int playerIndex = 0;                // position during turn


    int playerInputIndex = 0;
    QTimer flashTimer;
    QTimer playerTurnTimer;

    void nextTurn();        // Adds another coolor and replays sequence so far
    void endGame();         // Stop timers, emit final score
    void addRandomColor();
    void flashSequence();
    void resetGameState();  // Clear all states back to a new game

    static const int INITIAL_FLASH_DELAY = 1000;  // milliseconds
    static const int SPEED_DECREMENT = 50;       // how much faster each turn

};

#endif // SIMONMODEL_H
