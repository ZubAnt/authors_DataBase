#include "mainwindow.h"
#include <QApplication>
#include <QTableWidget>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("ADB");
    w.show();
    w.setWindowState(w.windowState() ^ Qt::WindowMaximized);

    return a.exec();
}
