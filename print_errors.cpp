#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QString>
#include <QtSql>

//#include "../database_authors_new_version/table/myqtableview.h"

void MainWindow::print_err(const QString func, const QString file, const int line, db_err err_code){

    QString len_str = QString::number(line);
    QString err_code_str = QString::number(err_code);
    ui->statusBar->showMessage("[" + file + ":" + len_str + "] " + func + ": " + err_code_str + " " +authors_db_err_string[-err_code]);
}

void MainWindow::warning_connection(){

    ui->statusBar->showMessage("WARNING:[RUS]Вы уже соединены с базой данных");
}

void MainWindow::error_create_table(QSqlQuery &query){

    QString error = "MySQL error:" + query.lastError().text() + \
                    "MySQL error code:" + QString::number(query.lastError().number());
    ui->statusBar->showMessage(error);
}
