#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "simonmodel.h"

#include <QMainWindow>

/*
 * Main Window
 * The view owns widgets and visuals. In this class we show status text, progress,
 * and flashes of the buttons. Another main function is to forward user input (Start/
 * Red/Blue) to the model using slots and signals
 */


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SimonModel& model, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGameStarted();
    void onGameEnded(int score);
    void onColorToFlash(int colorIndex);
    void onTurnStarted(int sequenceLength);

private:
    Ui::MainWindow *ui;
    SimonModel& model;      // Store this reference for wiring

};
#endif // MAINWINDOW_H
