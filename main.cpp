/*
 * A6: Simon game
 * Sinahi Leon and George Gaul
 * SinahiLeon and TheIconoclaster (respectively)
 *
 */

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
