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
    MainWindow(SimonModel& model, QWidget *parent = nullptr);
    ~MainWindow();


    // To do - add signals when user presses start and what was pressed

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
