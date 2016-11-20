#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QCloseEvent>
#include <QPalette>

#include "errors_authors_db.h"
#include "download_publish_house_from_file.h"
#include "itemdelegate.h"
#include "./table/myqtableview.h"
#include "./table/myqsqltablemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connect_window(new connection_settings),
    pb_house_table_name("publishing_house"),
    type_of_pb_table_name("type_of_publication"),
    main_table_name("main")
{    
    qDebug()<<"Constructor MainWindow";
    columnDb = new indexColumnDb();

    db = QSqlDatabase::addDatabase("QMYSQL");

    ui->setupUi(this);
    init_connect();
}

MainWindow::~MainWindow()
{
    qDebug()<<"Distructor ~MainWindow";
    delete pb_house_table;
    delete type_of_pb_table;
    delete main_table;
    delete cmb_type_of_pb_table;
    delete columnDb;
    delete connect_window;
    delete ui;
}

bool MainWindow::check_shure_download_db(){
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Download to a publishing houses", "Are you shure?",
                                QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes) {
        return true;
  }
  return false;
}


void MainWindow::on_Connect_DB_clicked()
{
    if(db.isOpen()){

        db.close();
    }

    connect_window->setWindowTitle("Connection settings");
    connect_window->show();
}

void MainWindow::open_database(const QString &name)
{
    if(!db.isOpen()){

        if (!QSqlDatabase::drivers().contains("QMYSQL")){
            QMessageBox::critical(this, "Unable to load database", "This demo needs the QMYSQL driver");
            exit(1);
        }

        qDebug()<<"open_database";

        if(db.open()){

            QSqlQuery Query(db);

            QString create_name_db = "CREATE DATABASE IF NOT EXISTS " + name;
            if (Query.exec(create_name_db.toStdString().c_str())){

                ui->textEdit->append("CREATE DATABASE is successful");                
            }
            else{

                ui->textEdit->append("Error CREATE DATABASE");
                clear_tables();
                db.close();
            }
        }
        else{

            ui->textEdit->append(db.lastError().text());
            QMessageBox::critical(this, "Установка соединения с базой",
                                  "Некорректные параметры соединения.\n"
                                  "Проверьте настройки.");
            clear_tables();
            return;
        }

        db.setDatabaseName(name);
        db.open();
        //Создание таблиц
        try{
            creeate_pb_house_table();
            creeate_type_of_pb_table();
            create_main_table();
        }

        catch(int err){
            ui->textEdit->append("interception exception");
            return;
        }

        connect_window->set_was_created(connect_window->get_trying_open_DB(), true);

        //Отображение таблиц
        init_type_of_pb_tableView();
        init_pb_house_tableView();
        init_main_tableView();

        ui->textEdit->append("The database connection is successfully established, Enjoy ;)");

        print_err("on_Connect_DB_clicked", __FILE__, __LINE__, ITS_OK);
    }
    else{

        warning_connection();
    }
}

void MainWindow::clear_tables()
{
    pb_house_table->clear();
    type_of_pb_table->clear();
    main_table->clear();
}
//функция, которая загружает базу данных из файла
void MainWindow::on_download_pbh_clicked()
{

    db_test = QSqlDatabase::addDatabase("QMYSQL");

    db_test.setPort(3306);
    db_test.setHostName("127.0.0.1");
    db_test.setUserName("root");
    db_test.setPassword("admin");

//    QSqlQuery query(db_test);

    if(db_test.open()) {
      QSqlQuery Query(db_test);
      if (Query.exec("CREATE DATABASE IF NOT EXISTS newdatabase_test"))
        qDebug() << "Ok"; else qDebug() << "Error query";
    } else qDebug() << "Error open";

//    query.prepare( "CREATE DATABASE IF NOT EXISTS testDb");

//    if(query.exec()){
//       qDebug()<<"It's OK";
//    }
//    else{
//       qDebug()<<"ERROR";
//       qDebug()<<"MySQL error:" + query.lastError().text();
//       qDebug()<<"MySQL error code:"+ QString::number(query.lastError().number());
//    }



//    sr = query.exec("CREATE DATABASE IF NOT EXISTS "+dbname);
//    QString fileName_pbh = QFileDialog::getOpenFileName(this, tr("Open File"), "",
//            "Text Files (*.txt);;all files (*.*)");

//    QFileInfo fileName_pbh_info(fileName_pbh);

//    ui->textEdit->append(fileName_pbh_info.fileName() + " has been selected");

//    QFile pbh_file(fileName_pbh);
//    if(!pbh_file.open(QIODevice::ReadOnly | QIODevice::Text)){
//        ui->textEdit->append(fileName_pbh_info.fileName() + " is not opened");
//        return;
//    }
//    else{
//         ui->textEdit->append(fileName_pbh_info.fileName() + " open");
//    }
//    if(check_shure_download_db()){
//        ui->textEdit->append("ЗДЕСЬ БУДЕТ ЗАГРУЗКА В БАЗУ ДАННЫХ СЧИТАННЫХ СТРОКИ");
////        Download_PB::download_table(pbh_file);

//    }
//    else{
//        ui->textEdit->append("В другой раз ;)");
//        return;
//    }
}

QTableView *MainWindow::get_tableView(int current_tab){
    switch(current_tab){
        case 0:
            return ui->pbh_tableView;
        case 1:
            return ui->type_of_pb_tableView;
        case 2:
            return ui->main_tableView;
        default:
            return NULL;
    }
    return NULL;
}

QSqlTableModel *MainWindow::get_table(int current_tab){
    switch(current_tab){
        case 0:
            return pb_house_table;
        case 1:
            return type_of_pb_table;
        case 2:
            return main_table;
        default:
            return NULL;
    }
    return NULL;
}

void MainWindow::slot_fullscreen_enable_slot()
{
    QMainWindow::showFullScreen();
}

void MainWindow::slot_fullscreen_disable_slot()
{
    if(this->isFullScreen()){
        this->setWindowState(Qt::WindowMaximized);
    }
}

void MainWindow::slot_set_conections_settings_database(const QString &name,
                                                       const QString &host,
                                                       const QString &login,
                                                       const QString &password,
                                                       const int port)
{
//    db.setDatabaseName(name);
    db.setHostName(host);
    db.setUserName(login);
    db.setPassword(password);
    db.setPort(port);

    open_database(name);
}

void MainWindow::slot_get_numb_tab(int &numb_tab){
    numb_tab = ui->tabWidget->currentIndex();
}

void MainWindow::slot_remove_rows(QTableView *tableView_change, const int numb_tab, const QModelIndexList &index_rem_rows){

    if(!db.isOpen()){
        ui->textEdit->append("Please connect to the database");
        print_err("on_remove_row_clicked", __FILE__, __LINE__, DB_NOT_OPEN);
        return;
    }

    QSqlTableModel *table_change = get_table(numb_tab);

    if(!tableView_change || !table_change){
        print_err("on_remove_row_clicked", __FILE__, __LINE__, NULL_POINTER);
        return;
    }

    if(index_rem_rows.empty()){
        ui->textEdit->append("Выберете строку для удаления");
        return;
    }
    int len  = index_rem_rows.length();
    for(int i =0; i < len; ++i){
        table_change->removeRow(index_rem_rows[i].row());
    }

    table_change->select();
    tableView_change->setModel(table_change);
}

void MainWindow::on_remove_row_clicked()
{
    if(!db.isOpen()){
        ui->textEdit->append("Please connect to the database");
        print_err("on_remove_row_clicked", __FILE__, __LINE__, DB_NOT_OPEN);
        return;
    }

    int numb_tab = ui->tabWidget->currentIndex();

    QTableView *tableView_change = get_tableView(numb_tab);
    QModelIndexList index_remove_rows = tableView_change->selectionModel()->selectedIndexes();

    slot_remove_rows(tableView_change, numb_tab, index_remove_rows);
}

void MainWindow::on_append_row_clicked()
{
    if(!db.isOpen()){
        ui->textEdit->append("Please connect to the database");
        print_err("on_append_row_clicked", __FILE__, __LINE__, DB_NOT_OPEN);
        return;
    }

    int numb_tab = ui->tabWidget->currentIndex();

    QSqlTableModel *table_change = get_table(numb_tab);

    if(!table_change || !table_change){
        print_err("on_append_row_clicked", __FILE__, __LINE__, NULL_POINTER);
        return;
    }

    table_change->insertRow(table_change->rowCount());
}

void MainWindow::on_insert_row_clicked()
{
    if(!db.isOpen()){
        ui->textEdit->append("Please connect to the database");
        print_err("on_insert_row_clicked", __FILE__, __LINE__, DB_NOT_OPEN);
        return;
    }

    int numb_tab = ui->tabWidget->currentIndex();

    QTableView *tableView_change = get_tableView(numb_tab);
    QSqlTableModel *table_change = get_table(numb_tab);

    if(!table_change || !table_change){
        print_err("on_insert_row_clicked", __FILE__, __LINE__, NULL_POINTER);
        return;
    }

    QModelIndexList ins_rows = tableView_change->selectionModel()->selectedIndexes();

    if(ins_rows.empty()){
        ui->textEdit->append("Выберете строку, перед которой необходимо вставить");
        return;
    }

    table_change->insertRow(ins_rows[0].row());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        slot_fullscreen_disable_slot();
        break;

    case Qt::Key_F5:
        slot_fullscreen_enable_slot();
        break;

    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"closeEvent";
    emit signal_close_connectoins_setttings_window();
    event->accept();
}
