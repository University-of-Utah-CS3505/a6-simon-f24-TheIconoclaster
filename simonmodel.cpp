#include "simonmodel.h"
#include <QTimer>
#include <QDateTime>
#include <algorithm>
using namespace std;

SimonModel::SimonModel(QObject *parent)
    : QObject(parent)
    , currentScore(0)
    , highScore(0)
    , playersTurn(false)
    , gameActive(false)
{
    // Initialize random seed

    // Connect timer signals
    connect(&flashTimer, &QTimer::timeout, this, &SimonModel::processNextFlash);
    connect(&playerTurnTimer, &QTimer::timeout, this, &SimonModel::startPlayerTurn);
}

void SimonModel::startGame()
{
    //Precheck the game is not already active
    if (gameActive) {
        return;
    }

    resetGameState();
    gameActive = true;

    // Start with one color
    addRandomColor();

    emit gameStarted();
    emit scoreChanged(currentScore);
    emit highScoreChanged(highScore);

    // Start showing the sequence after a brief delay
    QTimer::singleShot(1000, this, &SimonModel::flashSequence);
}

void SimonModel::playerPressed(int colorIndex)
{
    if (!playersTurn || !gameActive || colorIndex < 0 || colorIndex > 1) {
        return;
    }

    Colors pressedColor = static_cast<Colors>(colorIndex);
    Colors expectedColor = sequence[playerInputIndex];

    // Check if the pressed color matches the sequence
    if (pressedColor == expectedColor) {
        playerInputIndex++;
        emit progressUpdated(static_cast<int>((playerInputIndex * 100) / sequence.size()));

        // Check if player completed the sequence
        if (playerInputIndex >= sequence.size()) {
            currentScore++;
            emit scoreChanged(currentScore);

            // Update high score if needed
            if (currentScore > highScore) {
                highScore = currentScore;
                emit highScoreChanged(highScore);
            }

            playersTurn = false;
            playerInputIndex = 0;

            // Start next turn after a delay
            QTimer::singleShot(1000, this, &SimonModel::nextTurn);
        }
    } else {
        // Wrong color pressed
        endGame();
    }
}

void SimonModel::processNextFlash()
{
    if (flashQueue.empty()) {
        flashTimer.stop();

        // Brief delay before player's turn
        playerTurnTimer.start(500);
        return;
    }

    Colors nextColor = flashQueue.front();
    flashQueue.pop();

    emit colorToFlash(static_cast<int>(nextColor));
}

void SimonModel::startPlayerTurn()
{
    playerTurnTimer.stop();
    playersTurn = true;
    playerInputIndex = 0;
    emit turnStarted(static_cast<int>(sequence.size()));
    emit progressUpdated(0);
}

void SimonModel::nextTurn()
{
    if (!gameActive) return;

    // Add new color to sequence
    addRandomColor();

    // Show the updated sequence
    flashSequence();
}

void SimonModel::endGame()
{
    if (!gameActive) return;

    gameActive = false;
    playersTurn = false;
    flashTimer.stop();
    playerTurnTimer.stop();

    emit gameEnded(currentScore);
}

void SimonModel::addRandomColor()
{
    Colors newColor = static_cast<Colors>(rand() % 2); // 0 or 1
    sequence.push_back(newColor);
}

void SimonModel::flashSequence()
{
    // Copy sequence to flash queue
    flashQueue = queue<Colors>();
    for (Colors color : sequence) {
        flashQueue.push(color);
    }

    // Calculate flash speed (gets faster as score increases)
    int flashDelay = max(INITIAL_FLASH_DELAY - (currentScore * SPEED_DECREMENT), MIN_FLASH_DELAY);
    flashTimer.start(flashDelay);

    // Process first flash immediately
    processNextFlash();
}

void SimonModel::resetGameState()
{
    currentScore = 0;
    playersTurn = false;
    gameActive = false;
    playerInputIndex = 0;
    sequence.clear();

    // Clear queues
    flashQueue = queue<Colors>();

    // Stop any active timers
    flashTimer.stop();
    playerTurnTimer.stop();
}
