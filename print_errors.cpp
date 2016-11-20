#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QString>
#include <QtSql>

void MainWindow::print_err(const QString func, const QString file, const int line, db_err err_code){
    QString len_str = QString::number(line);
    QString err_code_str = QString::number(err_code);
    ui->textEdit->append("[" + file + ":" + len_str + "] " + func + ": " + err_code_str + " " +authors_db_err_string[-err_code]);
}

void MainWindow::warning_connection(){
    ui->textEdit->append("WARNING:[RUS]Вы уже соединены с базой данных");
    ui->textEdit->append("WARNING:[ENG]You are already connected to the database");
}

void MainWindow::error_create_table(QSqlQuery &query){
    ui->textEdit->append("ERROR");
    ui->textEdit->append("MySQL error:" + query.lastError().text());
    ui->textEdit->append("MySQL error code:"+ QString::number(query.lastError().number()));
}
