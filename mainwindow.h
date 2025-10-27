#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "simonmodel.h"


#include <QMainWindow>

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
    SimonModel& model;

};
#endif // MAINWINDOW_H
