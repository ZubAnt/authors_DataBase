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
#include <QTextStream>
#include <QtAlgorithms>

#include "errors_authors_db.h"
#include "./combobox/itemdelegate.h"
#include "./table/myqtableview.h"
#include "./table/myqsqltablemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connect_window(new connection_settings),
    progressbar_write(new progress_write),
    db(QSqlDatabase::addDatabase("QMYSQL")),
    columnDb(new indexColumnDb()),
    pb_house_table_name("Publishing_house"),
    type_of_pb_table_name("Type_of_publication"),
    main_table_name("main"),
    add_to_report_err_str("Добавьте публикацию в отчет"),
    choose_str_befor_inserting_err_str("Выберите строку, перед которой необходимо вставить"),
    choose_str_for_adding_to_report_err_str("Выберите строку для добавления в отчет"),
    choose_str_for_remove_from_report_err_str("Выберите строку для удаления из отчета"),
    rep_ind_max(8),
    rep_ind_main_id(0),
    rep_ind_name(1),
    rep_ind_type_of_pb(2),
    rep_ind_type_paper(3),
    rep_ind_publish_hs(4),
    rep_ind_pl(5),
    rep_ind_authors_pl(6),
    rep_ind_co_authors(7),
    report_columncount(6),
    delimer_symbol('|')
{    
    qDebug()<<"Constructor MainWindow";

    ui->setupUi(this);

    init_connect();

    init_report_tableWidget();
}

MainWindow::~MainWindow()
{
    qDebug()<<"Distructor ~MainWindow";
    delete connect_window;
    delete pb_house_table;
    delete type_of_pb_table;
    delete main_table;
    delete cmb_type_of_pb_table;
    delete columnDb;
    delete progressbar_write;
    delete ui;
}

bool MainWindow::check_shure_download_db(){

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Download to a publishing houses", "Are you shure?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes){

        return true;
    }

    return false;
}

void MainWindow::on_Connect_DB_clicked()
{
    if(db.isOpen()){

        db.close();
        clear_tables();
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

        if(db.open()){

            QSqlQuery Query(db);

            QString create_name_db = "CREATE DATABASE IF NOT EXISTS " + name +
                    " DEFAULT CHARACTER SET utf8 DEFAULT COLLATE utf8_general_ci;";
            if (Query.exec(create_name_db.toStdString().c_str())){

                qDebug() << "CREATE DATABASE IF NOT EXISTS is successful";
            }
            else{

                ui->statusBar->showMessage("Error CREATE DATABASE");
                db.close();
            }
        }
        else{

            ui->statusBar->showMessage(db.lastError().text());
            QMessageBox::critical(this, "Установка соединения с базой",
                                  "Некорректные параметры соединения.\n"
                                  "Проверьте настройки.");
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
            return;
        }

        connect_window->set_was_created(connect_window->get_trying_open_DB(), true);

        //Отображение таблиц
        init_type_of_pb_tableView();
        init_pb_house_tableView();
        init_main_tableView();

        ui->statusBar->showMessage("Соединение с базой данных успешно установлено");
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
    db.setHostName(host);
    db.setUserName(login);
    db.setPassword(password);
    db.setPort(port);
    db.setDatabaseName("");

    open_database(name);
}

void MainWindow::slot_check_row_in_added_set(const int row, bool &check)
{

    if(main_table->check_presence(main_table->record(row).value(columnDb->index_main_id).toInt())){

        check = true;
    }
    else{

        check = false;
    }
}

void MainWindow::slot_get_numb_tab(int &numb_tab){

    numb_tab = ui->tabWidget->currentIndex();
}

void MainWindow::slot_remove_rows(QTableView *tableView_change, const int numb_tab, const QModelIndexList &index_rem_rows){

    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    QSqlTableModel *table_change = get_table(numb_tab);

    if(!tableView_change || !table_change){

        print_err("on_remove_row_clicked", __FILE__, __LINE__, NULL_POINTER);
        return;
    }

    if(index_rem_rows.empty()){

        ui->statusBar->showMessage("Выберете строку для удаления");
        return;
    }

    for(auto it = index_rem_rows.cend() - 1;
        it >= index_rem_rows.cbegin();
        --it)
    {
        table_change->removeRow(it->row());        
    }

    ui->statusBar->showMessage("OK");
    table_change->select();
    tableView_change->setModel(table_change);
}

void MainWindow::on_remove_row_clicked()
{
    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
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

        ui->statusBar->showMessage("Please connect to the database");
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

        ui->statusBar->showMessage("Please connect to the database");
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

        ui->statusBar->showMessage(choose_str_befor_inserting_err_str);
        return;
    }

    table_change->insertRow(ins_rows[0].row());
}


void MainWindow::on_add_to_report_clicked()
{
    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    int numb_tab = ui->tabWidget->currentIndex();

    if(numb_tab != 2){

        ui->statusBar->showMessage("Please select the main tab");
        return;
    }

    ui->statusBar->showMessage("Ok");

    QModelIndexList add_to_rep_rows = ui->main_tableView->selectionModel()->selectedIndexes();

    if(add_to_rep_rows.empty()){

        ui->statusBar->showMessage(choose_str_for_adding_to_report_err_str);
        return;
    }

    for(auto it = add_to_rep_rows.cbegin();
        it != add_to_rep_rows.cend();
        ++it)
    {
        if(main_table->check_presence(main_table->record(it->row()).value(columnDb->index_main_id).toInt())){

            continue;
        }
        else{

            int row_count = ui->report_tableWidget->rowCount();
            ui->report_tableWidget->insertRow(row_count);

            ui->report_tableWidget->setItem(row_count, rep_ind_main_id,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_main_id).toString()));
            ui->report_tableWidget->setItem(row_count, rep_ind_name,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_name_of_pb).toString()));
            ui->report_tableWidget->setItem(row_count, rep_ind_type_of_pb,
                                            new QTableWidgetItem(find_type_of_pb_by_id(main_table->record(it->row()).value(columnDb->index_type_of_pb).toInt())));
            ui->report_tableWidget->setItem(row_count, rep_ind_type_paper,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_type_paper).toString()));
            ui->report_tableWidget->setItem(row_count, rep_ind_publish_hs,
                                            new QTableWidgetItem(find_pb_house_by_id(main_table->record(it->row()).value(columnDb->index_publish_hs).toInt())));
            ui->report_tableWidget->setItem(row_count, rep_ind_pl,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_pl).toString()));
            ui->report_tableWidget->setItem(row_count, rep_ind_authors_pl,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_authors_pl).toString()));
            ui->report_tableWidget->setItem(row_count, rep_ind_co_authors,
                                            new QTableWidgetItem(main_table->record(it->row()).value(columnDb->index_co_authors).toString()));

            main_table->add_to_set_of_added_rows(main_table->record(it->row()).value(columnDb->index_main_id).toInt());
        }

        ui->statusBar->showMessage("OK");
    }
}

const QString MainWindow::find_pb_house_by_id(const int id)
{
    QString find_pb_house_str;
    int row_count =  pb_house_table->rowCount();

    for(int i = 0; i < row_count; ++i){

        if(pb_house_table->record(i).value(0).toInt() == id){

            return pb_house_table->record(i).value(1).toString();
        }
    }

    return find_pb_house_str;
}

const QString MainWindow::find_type_of_pb_by_id(const int id)
{
    QString find_type_of_pb_str;
    int row_count =  type_of_pb_table->rowCount();

    for(int i = 0; i < row_count; ++i){

        if(type_of_pb_table->record(i).value(0).toInt() == id){

            return type_of_pb_table->record(i).value(1).toString();
        }
    }

    return find_type_of_pb_str;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
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
    emit signal_close_connectoins_setttings_window();
    event->accept();
}

void MainWindow::on_del_row_in_report_clicked()
{

    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    QModelIndexList del_from_rep_rows_rep = ui->report_tableWidget->selectionModel()->selectedIndexes();
    QModelIndexList del_from_rep_rows_tab = ui->main_tableView->selectionModel()->selectedIndexes();

    if(del_from_rep_rows_rep.empty() && del_from_rep_rows_tab.empty()){

        ui->statusBar->showMessage(choose_str_for_remove_from_report_err_str);
        return;
    }
    else if(!del_from_rep_rows_rep.empty()){


        for(auto it = del_from_rep_rows_rep.cend() - 1;
            it >= del_from_rep_rows_rep.cbegin();
            --it)
        {
            int row = it->row();
            int main_id = ui->report_tableWidget->item(row, rep_ind_main_id)->text().toInt();
            main_table->del_form_set_of_added_rows(main_id);

            ui->report_tableWidget->removeRow(row);
        }
    }
    else{

        for(auto it = del_from_rep_rows_tab.cbegin();
            it != del_from_rep_rows_tab.cend();
            ++it)
        {
            int row = it->row();
            int main_id = main_table->record(row).value(columnDb->index_main_id).toInt();
            main_table->del_form_set_of_added_rows(main_id);

            for(int i = 0; i < ui->report_tableWidget->rowCount(); ++i)
            {
                if(ui->report_tableWidget->item(i, rep_ind_main_id)->text().toInt() == main_id)
                {
                    ui->report_tableWidget->removeRow(i);
                }
            }
        }
    }
}

void MainWindow::on_up_row_in_report_clicked()
{

    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    QModelIndexList up_in_rep_rows = ui->report_tableWidget->selectionModel()->selectedIndexes();

    if(up_in_rep_rows.empty()){

        ui->statusBar->showMessage("Выберите строки для перемещения вверх");
        return;
    }

    if(up_in_rep_rows[0].row() == 0){

        return;
    }

    QVector<QString> tmp_data;

    for(auto it = up_in_rep_rows.begin();
        it != up_in_rep_rows.end();
        ++it)
    {
        int row = it->row();
        ///===================================================================================
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_main_id)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_name)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_type_of_pb)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_type_paper)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_publish_hs)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_pl)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_authors_pl)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row - 1, rep_ind_co_authors)->text());
        ///===================================================================================
        ui->report_tableWidget->removeRow(row - 1);
        ui->report_tableWidget->insertRow(row - 1);
        ///==================================================================================================================
        ui->report_tableWidget->setItem(row - 1, rep_ind_main_id,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_main_id)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_name,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_name)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_type_of_pb,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_type_of_pb)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_type_paper,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_type_paper)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_publish_hs,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_publish_hs)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_pl,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_pl)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_authors_pl,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_authors_pl)->text()));
        ui->report_tableWidget->setItem(row - 1, rep_ind_co_authors,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_co_authors)->text()));
        ///==================================================================================================================
        ui->report_tableWidget->removeRow(row);
        ui->report_tableWidget->insertRow(row);
        ///=================================================================================
        ui->report_tableWidget->setItem(row, rep_ind_main_id,
                                        new QTableWidgetItem(tmp_data[rep_ind_main_id]));
        ui->report_tableWidget->setItem(row, rep_ind_name,
                                        new QTableWidgetItem(tmp_data[rep_ind_name]));
        ui->report_tableWidget->setItem(row, rep_ind_type_of_pb,
                                        new QTableWidgetItem(tmp_data[rep_ind_type_of_pb]));
        ui->report_tableWidget->setItem(row, rep_ind_type_paper,
                                        new QTableWidgetItem(tmp_data[rep_ind_type_paper]));
        ui->report_tableWidget->setItem(row, rep_ind_publish_hs,
                                        new QTableWidgetItem(tmp_data[rep_ind_publish_hs]));
        ui->report_tableWidget->setItem(row, rep_ind_pl,
                                        new QTableWidgetItem(tmp_data[rep_ind_pl]));
        ui->report_tableWidget->setItem(row, rep_ind_authors_pl,
                                        new QTableWidgetItem(tmp_data[rep_ind_authors_pl]));
        ui->report_tableWidget->setItem(row, rep_ind_co_authors,
                                        new QTableWidgetItem(tmp_data[rep_ind_co_authors]));
        ///=================================================================================
        tmp_data.clear();
    }
}

void MainWindow::on_down_row_in_report_clicked()
{

    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    QModelIndexList down_in_rep_rows = ui->report_tableWidget->selectionModel()->selectedIndexes();

    if(down_in_rep_rows.empty()){

        ui->statusBar->showMessage("Выберите строки для перемещения вниз");
        return;
    }

    if(down_in_rep_rows[down_in_rep_rows.length() - 1].row() == main_table->rowCount() - 1){

        return;
    }

    QVector<QString> tmp_data;

    for(auto it = down_in_rep_rows.end() - 1;
        it >= down_in_rep_rows.begin();
        --it)
    {

        int row = it->row();
        ///===================================================================================
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_main_id)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_name)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_type_of_pb)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_type_paper)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_publish_hs)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_pl)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_authors_pl)->text());
        tmp_data.push_back(ui->report_tableWidget->item(row + 1, rep_ind_co_authors)->text());
        ///===================================================================================
        ui->report_tableWidget->removeRow(row + 1);
        ui->report_tableWidget->insertRow(row + 1);
        ///==================================================================================================================
        ui->report_tableWidget->setItem(row + 1, rep_ind_main_id,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_main_id)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_name,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_name)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_type_of_pb,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_type_of_pb)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_type_paper,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_type_paper)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_publish_hs,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_publish_hs)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_pl,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_pl)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_authors_pl,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_authors_pl)->text()));
        ui->report_tableWidget->setItem(row + 1, rep_ind_co_authors,
                                        new QTableWidgetItem(ui->report_tableWidget->item(row, rep_ind_co_authors)->text()));
        ///==================================================================================================================
        ui->report_tableWidget->removeRow(row);
        ui->report_tableWidget->insertRow(row);
        ///=================================================================================
        ui->report_tableWidget->setItem(row, rep_ind_main_id,
                                        new QTableWidgetItem(tmp_data[rep_ind_main_id]));
        ui->report_tableWidget->setItem(row, rep_ind_name,
                                        new QTableWidgetItem(tmp_data[rep_ind_name]));
        ui->report_tableWidget->setItem(row, rep_ind_type_of_pb,
                                        new QTableWidgetItem(tmp_data[rep_ind_type_of_pb]));
        ui->report_tableWidget->setItem(row, rep_ind_type_paper,
                                        new QTableWidgetItem(tmp_data[rep_ind_type_paper]));
        ui->report_tableWidget->setItem(row, rep_ind_publish_hs,
                                        new QTableWidgetItem(tmp_data[rep_ind_publish_hs]));
        ui->report_tableWidget->setItem(row, rep_ind_pl,
                                        new QTableWidgetItem(tmp_data[rep_ind_pl]));
        ui->report_tableWidget->setItem(row, rep_ind_authors_pl,
                                        new QTableWidgetItem(tmp_data[rep_ind_authors_pl]));
        ui->report_tableWidget->setItem(row, rep_ind_co_authors,
                                        new QTableWidgetItem(tmp_data[rep_ind_co_authors]));
        ///=================================================================================
        tmp_data.clear();
    }
}
