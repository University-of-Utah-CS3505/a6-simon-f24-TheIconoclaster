#include "simonmodel.h"
#include <QTimer>
#include <QDateTime>
#include <algorithm>
using namespace std;

/*
 * Simon Model
 * Model in the MV design. This class owns game and state logic:
 * Here we generate and store the Simon sequence, Drive coputer playback
 * timing using QTimers, and validate player input to update score.
 */

SimonModel::SimonModel(QObject *parent)
    : QObject(parent)
    , currentScore(0)
    , highScore(0)
    , playersTurn(false)
    , gameActive(false)
{
    // Connect timer signals
    connect(&flashTimer, &QTimer::timeout, this, &SimonModel::processNextFlash);
    connect(&playerTurnTimer, &QTimer::timeout, this, &SimonModel::startPlayerTurn);
}

/*
 * Begin a new game if one is not active
 */
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

/*
 * Handle a player click. Only validate during the player's turn.
 * Compare their input against expected item in the sequence
 */
void SimonModel::playerPressed(int colorIndex)
{
    if (!playersTurn || !gameActive || colorIndex < 0 || colorIndex > 1) {
        return;
    }

    Colors pressedColor = static_cast<Colors>(colorIndex);
    Colors expectedColor = sequence[playerInputIndex];

    // Check if the pressed color matches the sequence
    if (pressedColor == expectedColor) {
        playerInputIndex++;     // Correct input -> advance progress
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

            // Prepare for next round
            playersTurn = false;
            playerInputIndex = 0;

            // Start next turn after a delay
            QTimer::singleShot(1000, this, &SimonModel::nextTurn);
        }
    } else {
        // Wrong color pressed -> game over
        endGame();
    }
}


/*
 * Pop one color from the queue and tell the view to flash it
 * When the queue is empty, it is then the player's turn
 */
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

/*
 * Switch to the player's turn, reset round progress
 */
void SimonModel::startPlayerTurn()
{
    playerTurnTimer.stop();
    playersTurn = true;
    playerInputIndex = 0;
    emit turnStarted(static_cast<int>(sequence.size()));
    emit progressUpdated(0);
}

/*
 * Start the next sround: replay the sequence and add new color
 */
void SimonModel::nextTurn()
{
    if (!gameActive) return;

    // Add new color to sequence
    addRandomColor();

    // Show the updated sequence
    flashSequence();
}

/*
 * Game over: stop timers and report final score
 */
void SimonModel::endGame()
{
    if (!gameActive) return;

    gameActive = false;
    playersTurn = false;
    flashTimer.stop();
    playerTurnTimer.stop();

    emit gameEnded(currentScore);
}

/*
 * Append one color to the main sequence
 */
void SimonModel::addRandomColor()
{
    Colors newColor = static_cast<Colors>(rand() % 2); // 0 or 1
    sequence.push_back(newColor);
}

/*
 * Run the computer's playback using flashTimer. Copy the full sequence
 * into a queue and pop one item each tick. The delay decreases each round,
 * but not below 300 ms.
 */
void SimonModel::flashSequence()
{
    // Copy sequence to flash queue
    flashQueue = queue<Colors>();
    for (Colors color : sequence) {
        flashQueue.push(color);
    }

    // Calculate flash speed (gets faster as score increases)
    int flashDelay = max(INITIAL_FLASH_DELAY - (currentScore * SPEED_DECREMENT), 300);  // 300 indicates our minimum flash speed
    flashTimer.start(flashDelay);

    // Process first flash immediately
    processNextFlash();
}

/*
 * Resets back to new-game state
 */
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
