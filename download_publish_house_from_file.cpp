#include "download_publish_house_from_file.h"
#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include "mainwindow.h"
#include <QApplication>
#include <QSqlTableModel>
#include <ui_mainwindow.h>

//void print_table_data_base(MainWindow *ob, QSqlDatabase &db){

//    QStringList str_list = db.tables();
//    for(int i=0; i < str_list.length(); ++i){
//        qDebug()<<str_list[i];
//    }
//}

//void download_table(MainWindow *ob, QSqlDatabase &db){
//    QSqlTableModel *model = new QSqlTableModel(ob);
//    model->setTable("publishing_house");
//    model->select();
//    ob->ui->pbh_tableView->setModel(model);
//    model->insertRow(model->rowCount());
//    ob->ui->textEdit->append("HERE");
//    delete model;
//}


//INSERT INTO `authorsdb`.`person` (`FirstName`) VALUES ('test1');
