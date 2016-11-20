#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::init_connect(){

    //remove rows
    connect(ui->main_tableView, SIGNAL(signal_remove_rows(QTableView *, const int , const QModelIndexList &)),
            this, SLOT(slot_remove_rows(QTableView *, const int , const QModelIndexList &)));
    connect(ui->type_of_pb_tableView, SIGNAL(signal_remove_rows(QTableView *, const int , const QModelIndexList &)),
            this, SLOT(slot_remove_rows(QTableView *, const int , const QModelIndexList &)));
    connect(ui->pbh_tableView, SIGNAL(signal_remove_rows(QTableView *, const int , const QModelIndexList &)),
            this, SLOT(slot_remove_rows(QTableView *, const int , const QModelIndexList &)));

    //get current tabWidget
    connect(ui->main_tableView, SIGNAL(signal_get_numb_tab(int &)),
            this, SLOT(slot_get_numb_tab(int &)));
    connect(ui->type_of_pb_tableView, SIGNAL(signal_get_numb_tab(int &)),
            this, SLOT(slot_get_numb_tab(int &)));
    connect(ui->pbh_tableView, SIGNAL(signal_get_numb_tab(int &)),
            this, SLOT(slot_get_numb_tab(int &)));

    //fullsreen enable
    connect(ui->pbh_tableView, SIGNAL(signal_fullscreen_enable()),
            this, SLOT(slot_fullscreen_enable_slot()));
    connect(ui->type_of_pb_tableView, SIGNAL(signal_fullscreen_enable()),
            this, SLOT(slot_fullscreen_enable_slot()));
    connect(ui->main_tableView, SIGNAL(signal_fullscreen_enable()),
            this, SLOT(slot_fullscreen_enable_slot()));

    //fullsreen disable
    connect(ui->pbh_tableView, SIGNAL(signal_fullscreen_disable()),
            this, SLOT(slot_fullscreen_disable_slot()));
    connect(ui->type_of_pb_tableView, SIGNAL(signal_fullscreen_disable()),
            this, SLOT(slot_fullscreen_disable_slot()));
    connect(ui->main_tableView, SIGNAL(signal_fullscreen_disable()),
            this, SLOT(slot_fullscreen_disable_slot()));

    //close connectons_settings widow
    connect(this, SIGNAL(signal_close_connectoins_setttings_window()),
            connect_window, SLOT(slot_close_connectoins_setttings_window()));

    //close open_DB
    connect(connect_window, SIGNAL(signal_set_conections_settings_database(QString,QString,QString,QString,int)),
            this, SLOT(slot_set_conections_settings_database(QString,QString,QString,QString,int)));
}
