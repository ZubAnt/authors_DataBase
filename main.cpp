#include "mainwindow.h"
#include <QApplication>
#include <QTableWidget>
#include <QStringList>

#include "itemdelegate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.setWindowState(w.windowState() ^ Qt::WindowMaximized);

    return a.exec();
}