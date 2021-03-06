#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

void MainWindow::creeate_pb_house_table(){

    QSqlQuery query(db);

    query.prepare( "CREATE TABLE IF NOT EXISTS Publishing_house("
                   "pb_house_id INT AUTO_INCREMENT PRIMARY KEY,"
                   "Name varchar(255) NOT NULL,"
                   "UNIQUE KEY (Name)) DEFAULT CHARACTER SET = 'utf8'" );

    if(query.exec()){

        qDebug() << "Table Publishing_house was created";
    }
    else{
        error_create_table(query);
        throw -1;
    }
}

void MainWindow::creeate_type_of_pb_table(){

    QSqlQuery query(db);

    query.prepare( "CREATE TABLE IF NOT EXISTS Type_of_publication("
                   "type_of_pb_id INT AUTO_INCREMENT PRIMARY KEY,"
                   "Name varchar(255) NOT NULL,"
                   "UNIQUE KEY (Name)) DEFAULT CHARACTER SET = 'utf8'");
    if(query.exec()){

        qDebug() << "Table Type_of_publication was created";
    }
    else{
        error_create_table(query);
        throw -1;
    }
}

void MainWindow::create_main_table(){

    QSqlQuery query(db);

    query.prepare("CREATE TABLE IF NOT EXISTS `main` ("
                  "`main_id` INT(11) NOT NULL AUTO_INCREMENT,"
                  "`Название публикации` VARCHAR(500) NULL DEFAULT NULL,"
                  "`Вид публикации` INT(11) NULL DEFAULT NULL,"
                  "`Тип` VARCHAR(16) NULL DEFAULT NULL,"
                  "`Издательство` INT(11) NULL DEFAULT NULL,"
                  "`Год издания` INT(11) NULL DEFAULT NULL,"
                  "`Страницы` VARCHAR(32) NULL DEFAULT NULL,"
                  "`пл` VARCHAR(8) NULL DEFAULT NULL,"
                  "`Авт пл` VARCHAR(8) NULL DEFAULT NULL,"
                  "`Соавторы` VARCHAR(255) NULL DEFAULT NULL,"
                  "`Гриф` VARCHAR(4) NULL DEFAULT '-',"
                  "	`Примечание` VARCHAR(255) NULL DEFAULT NULL,"
                  ""
                  "PRIMARY KEY (`main_id`),"
                  "INDEX `FK_main_type_of_publication` (`Вид публикации`),"
                  "INDEX `FK_main_publishing_house` (`Издательство`),"
                  ""
                  "CONSTRAINT `FK_main_type_of_publication` FOREIGN KEY (`Вид публикации`)"
                  " REFERENCES `Type_of_publication` (`type_of_pb_id`),"
                  ""
                  "CONSTRAINT `FK_main_publishing_house` FOREIGN KEY (`Издательство`)"
                  " REFERENCES `Publishing_house` (`pb_house_id`)"
                  ") DEFAULT CHARACTER SET = 'utf8'");
    if(query.exec()){

        qDebug() << "Table main was created";
    }
    else{

        error_create_table(query);
        throw -1;
    }
}
