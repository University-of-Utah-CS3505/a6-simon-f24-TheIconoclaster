#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "simonmodel.h"


MainWindow::MainWindow(SimonModel& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->StartButton->setEnabled(true);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);
    ui->StatusLabel->setText("Push to Start");
    ui->progressBar->setValue(0);


    connect(ui->StartButton,
            &QPushButton::clicked,
            &model, &SimonModel::startGame);

    connect(ui->RedButton,
            &QPushButton::clicked,
            &model, [&model](int value){model.playerPressed(0);});

    connect(ui->BlueButton,
            &QPushButton::clicked,
            &model, [&model](int value){model.playerPressed(1);});

    connect(ui->progressBar,SIGNAL(progressUpdated(int progress)),
            ui->progressBar,SLOT(setValue(int)));

    connect(ui->StatusLabel, qOverload(&QLabel::setText),
            this, [](int num) { /* handle setNum */ });

}

MainWindow::~MainWindow()
{
    delete ui;
}
