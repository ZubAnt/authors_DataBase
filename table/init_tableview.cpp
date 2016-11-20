#include "../mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::init_type_of_pb_tableView(){

    type_of_pb_table = new QSqlTableModel(this);
    type_of_pb_table->setTable(type_of_pb_table_name);

    type_of_pb_table->select();
    ui->type_of_pb_tableView->setModel(type_of_pb_table);

    ui->type_of_pb_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->type_of_pb_tableView->setColumnHidden(0,true);
    ui->type_of_pb_tableView->setSortingEnabled(false);
}

void MainWindow::init_pb_house_tableView(){

    pb_house_table = new QSqlTableModel(this);
    pb_house_table->setTable(pb_house_table_name);

    pb_house_table->select();
    ui->pbh_tableView->setModel(pb_house_table);

    ui->pbh_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->pbh_tableView->setColumnHidden(0,true);
    ui->pbh_tableView->setSortingEnabled(false);
}

void MainWindow::init_main_tableView(){

    cmb_type_of_pb_table = new ComboBoxItemDelegate(type_of_pb_table,
                                                    pb_house_table,
                                                    ui->main_tableView);

    main_table = new MyQSqlTableModel(this, &db, type_of_pb_table, pb_house_table);
    main_table->setTable(main_table_name);

    main_table->select();
    ui->main_tableView->setSortingEnabled(false);
    ui->main_tableView->setModel(main_table);

    //растянуть до конца последний столбец
    ui->main_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->main_tableView->setColumnHidden(0,true);    //скрыть id

    setwidth_column_main_table();   //set width column
    freeze_column_main_table();     //freeze column

    ui->main_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->main_tableView->setItemDelegate(cmb_type_of_pb_table);  //combobox
}

void MainWindow::setwidth_column_main_table(){

    //set width column
    ui->main_tableView->setColumnWidth(columnDb->index_name_of_pb, 255);  //Название публикации
    ui->main_tableView->setColumnWidth(columnDb->index_type_of_pb, 180);  //Вид публикации
    ui->main_tableView->setColumnWidth(columnDb->index_type_paper, 52);   //тип, рук. печ.
    ui->main_tableView->setColumnWidth(columnDb->index_publish_hs, 220);  //Издательство
    ui->main_tableView->setColumnWidth(columnDb->index_year_pb, 92);      //год издания
    ui->main_tableView->setColumnWidth(columnDb->index_numberpage, 80);   //Страницы
    ui->main_tableView->setColumnWidth(columnDb->index_pl, 60);           //пл.
    ui->main_tableView->setColumnWidth(columnDb->index_authors_pl, 60);   //авторских пл.
    ui->main_tableView->setColumnWidth(columnDb->index_co_authors, 128);  //Соавторы
    ui->main_tableView->setColumnWidth(columnDb->index_secur_clas, 45);   //гриф
    ui->main_tableView->setColumnWidth(columnDb->index_annotation, 200);  //примечание
}


void MainWindow::freeze_column_main_table(){

    //freeze column
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_name_of_pb, QHeaderView::Fixed);
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_type_paper, QHeaderView::Fixed);
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_year_pb,    QHeaderView::Fixed);
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_secur_clas, QHeaderView::Fixed);
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_pl,    QHeaderView::Fixed);
    ui->main_tableView->horizontalHeader()->setSectionResizeMode (columnDb->index_authors_pl, QHeaderView::Fixed);
}

