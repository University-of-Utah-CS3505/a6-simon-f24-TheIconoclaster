
///===============================================
/// Sinahi Leon (GitHub: SinahiLeon)
/// George Gaul (GitHub: TheIconoclaster)
/// Repo- https://github.com/University-of-Utah-CS3505/a6-simon-f24-TheIconoclaster/tree/master
/// Course- CS3505
/// Assignment- A6: Simon Game
/// Date- 10/27/2025
/// Creative Element- Live high score tracker that displays the player's best performance
/// during the current session using QLCDNumber widget. If the player's current score
/// suprasses their previous best, the high score is immediatly updated and shown on
/// the display. The high score is stored in memory for the duration of the game session.
///===============================================

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SimonModel model;

    MainWindow w(model);
    w.show();
    return a.exec();
}
