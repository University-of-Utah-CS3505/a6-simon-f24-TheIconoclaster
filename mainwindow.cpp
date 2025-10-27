#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simonmodel.h"


MainWindow::MainWindow(SimonModel& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);
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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onGameStarted()
{
    ui->StartButton->setEnabled(false);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->StatusLabel->setText("Watch the pattern:");
}

void MainWindow::onGameEnded(int score)
{
    ui->StartButton->setEnabled(true);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->StatusLabel->setText(QString("Game Over! Score: %1 - Push to Start").arg(score));
}

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


