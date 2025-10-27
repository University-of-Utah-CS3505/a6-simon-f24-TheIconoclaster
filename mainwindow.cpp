#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simonmodel.h"

/*
 * Main Window
 * The view owns widgets and visuals. In this class we show status text, progress,
 * and flashes of the buttons. Another main function is to forward user input (Start/
 * Red/Blue) to the model using slots and signals
 */

MainWindow::MainWindow(SimonModel& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    // Initial UI state
    ui->setupUi(this);
    ui->HighScore->display(model.getHighScore());
    ui->StartButton->setEnabled(true);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->StatusLabel->setText("Push to Start");
    ui->progressBar->setValue(0);


    // Connect UI to Model
    connect(ui->StartButton, &QPushButton::clicked,
            &model, &SimonModel::startGame);

    connect(ui->RedButton, &QPushButton::clicked,
            this, [&model]() { model.playerPressed(0); });

    connect(ui->BlueButton, &QPushButton::clicked,
            this, [&model]() { model.playerPressed(1); });

    // Connect Model to UI updates
    connect(&model, &SimonModel::gameStarted,
            this, &MainWindow::onGameStarted);

    connect(&model, &SimonModel::gameEnded,
            this, &MainWindow::onGameEnded);

    connect(&model, &SimonModel::colorToFlash,
            this, &MainWindow::onColorToFlash);

    connect(&model, &SimonModel::turnStarted,
            this, &MainWindow::onTurnStarted);

    connect(&model, &SimonModel::progressUpdated,
            ui->progressBar, &QProgressBar::setValue);

    connect(&model, &SimonModel::highScoreChanged,
            this, [this](int hs){ ui->HighScore->display(hs); });

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * When the model determines a new game has started, the UI disables the buttons
 * and resets.
 */
void MainWindow::onGameStarted()
{
    ui->StartButton->setEnabled(false);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->StatusLabel->setText("Watch the pattern:");
}

/*
 * Whens a game ends, enable the Start button but disable the colors,
 * and show a message
 */
void MainWindow::onGameEnded(int score)
{
    ui->StartButton->setEnabled(true);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->StatusLabel->setText(QString("Game Over! Score: %1 - Push to Start").arg(score));
}

/*
 * When the model requests to flash a specific color, the UI temporarily
 * sets the button to a brighter style, then reverts after 300 ms.
 */
void MainWindow::onColorToFlash(int colorIndex)
{
    // Flash the button briefly
    QPushButton* button = nullptr;
    QString normalStyle, flashStyle;
    ui->StatusLabel->setText("Watch the pattern:");

    if (colorIndex == 0) { // Red button
        button = ui->RedButton;
        normalStyle = "background-color: darkred; border: 2px solid black;";
        flashStyle = "background-color: red; border: 2px solid white;";
    } else if (colorIndex == 1) { // Blue button
        button = ui->BlueButton;
        normalStyle = "background-color: darkblue; border: 2px solid black;";
        flashStyle = "background-color: blue; border: 2px solid white;";
    }

    if (button) {
        // Apply flash style
        button->setStyleSheet(flashStyle);

        // Return to normal after a brief period
        QTimer::singleShot(300, this, [button, normalStyle]() {
            button->setStyleSheet(normalStyle);
        });
    }
}

/*
 * When the model determine's it is the player's turn, the UI enables the
 * buttons, resets progress so far, and sets neutral styles.
 */
void MainWindow::onTurnStarted(int sequenceLength)
{
    ui->RedButton->setEnabled(true);
    ui->BlueButton->setEnabled(true);
    ui->progressBar->setMaximum(100); // For percentage
    ui->progressBar->setValue(0);
    ui->StatusLabel->setText("Your turn - Repeat the pattern!");

    // Reset button styles to normal
    ui->RedButton->setStyleSheet("background-color: darkred; border: 2px solid black;");
    ui->BlueButton->setStyleSheet("background-color: darkblue; border: 2px solid black;");
}


