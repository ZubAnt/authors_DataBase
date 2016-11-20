#ifndef DOWNLOAD_PUBLISH_HOUSE_FROM_FILE_H
#define DOWNLOAD_PUBLISH_HOUSE_FROM_FILE_H

#include <QSqlDatabase>
#include <QFile>
#include "mainwindow.h"
#include <QApplication>
#include <QSqlTableModel>
#include <QDebug>
#include <QUrl>

void print_table_data_base(MainWindow *ob,QSqlDatabase &db);
void download_table(MainWindow *ob, QSqlDatabase &db);

#endif // DOWNLOAD_PUBLISH_HOUSE_FROM_FILE_H
