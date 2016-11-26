#include <QModelIndexList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QMap>
#include <QModelIndexList>
#include <QTableWidgetItem>
#include <QMessageBox>

#include "connection_settings.h"
#include "ui_connection_settings.h"

connection_settings::connection_settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::connection_settings),
    file_name_connections_settings("connection_settings.ini"),
    index_data_connections_host(0),
    index_data_connections_login(1),
    index_data_connections_password(2),
    index_data_connections_port(3),
    index_data_connections_comment(4),
    DB_name(new QMap<int, QString>),
    DB_was_created(new QMap<int, bool>),
    map_host(new QMap<QString, QString>),
    map_login(new QMap<QString, QString>),
    map_password(new QMap<QString, QString>),
    map_port(new QMap<QString, int>),
    map_comment(new QMap<QString, QString>)
{
    qDebug()<<"Constructor connection_settings";
    ui->setupUi(this);


    init_tableWidget_nameDB();

    connect(ui->tableWidget_nameDB, SIGNAL(signal_update_all_data_conenctions(const QModelIndex &)),
            this, SLOT(slot_update_all_data_conenctions(const QModelIndex &)));
}

void connection_settings::set_was_created(const QString &name, bool was_created)
{
    int row = DB_name->key(name);
    (*DB_was_created)[row] = was_created;
    export_data();
}

const QString connection_settings::get_trying_open_DB()
{
    return try_open_name_DB;
}

connection_settings::~connection_settings()
{
    qDebug()<<"Distructor connection_settings";
    delete DB_name;
    delete DB_was_created;
    delete map_host;
    delete map_login;
    delete map_password;
    delete map_port;
    delete map_comment;
    delete ui;
}

void connection_settings::on_create_DB_clicked()
{
    int row_count = ui->tableWidget_nameDB->rowCount();
    if(row_count != 0){
        if(ui->tableWidget_nameDB->item(row_count - 1, 0)->text() != ""){
            update_tableWidget_nameDB(row_count);
        }
    }
    else{
        update_tableWidget_nameDB(row_count);
    }
}

void connection_settings::on_delete_DB_clicked()
{
    QModelIndexList index_remove_rows = ui->tableWidget_nameDB->selectionModel()->selectedIndexes();

//    QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
//                                                                    tr("Are you sure?\n"),
//                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
//                                                                    QMessageBox::Yes);

    if(index_remove_rows.isEmpty()){
        return;
    }
    else if (index_remove_rows.size() > 1){

        QMessageBox::critical(this, "Удаление базы данных", "Выберите только одну базу данных");

        return;
    }
    else if(index_remove_rows.size() == 1){

        int row = index_remove_rows[0].row();
        if(DB_was_created->value(row) == false){
            ui->tableWidget_nameDB->removeRow(row);

            int old_size = DB_name->size();
            QString old_name = DB_name->value(row);

            if(row != old_size - 1){

                for(int i = row; i < DB_name->size() - 1; ++i){
                    (*DB_name)[i] = DB_name->value(i + 1);
                    (*DB_was_created)[i] = DB_was_created->value(i + 1);
                }

                DB_name->remove(old_size - 1);
                DB_was_created->remove(old_size - 1);
            }
            else{
                DB_name->remove(old_size - 1);
                DB_was_created->remove(old_size - 1);
            }

            map_host->remove(old_name);
            map_login->remove(old_name);
            map_password->remove(old_name);
            map_port->remove(old_name);
            map_comment->remove(old_name);

            slot_clean_all_data_connection();
            export_data();
        }
        else{

            QMessageBox::critical(this, "Удаление базы данных", "Вы удаляете созданную базу данных\n"
                                                                "Все данные будут удалены");
        }
    }
}

void connection_settings::on_save_DB_clicked()
{
    export_data();
}

void connection_settings::init_tableWidget_nameDB()
{

    ui->tableWidget_nameDB->setColumnCount(1);
    ui->tableWidget_nameDB->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_nameDB->setHorizontalHeaderItem(0, new QTableWidgetItem("Имя сеанса"));
    ui->tableWidget_nameDB->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    import_data();
}

void connection_settings::update_tableWidget_nameDB(const int row_count)
{
    ui->tableWidget_nameDB->insertRow(row_count);
    DB_name->insert(row_count, "");
    DB_was_created->insert(row_count, false);
    ui->tableWidget_nameDB->setItem(row_count, 0, new QTableWidgetItem(DB_name->value(row_count)));
}

void connection_settings::set_data_connections(const QString &arg1,
                                               const int index_data_connections,
                                               const int row)
{
    if(index_data_connections == index_data_connections_host){
        (*map_host)[DB_name->value(row)] = arg1;
    }
    else if(index_data_connections == index_data_connections_login){
        (*map_login)[DB_name->value(row)] = arg1;
    }
    else if(index_data_connections == index_data_connections_password){
        (*map_password)[DB_name->value(row)] = arg1;
    }
    else if(index_data_connections == index_data_connections_port){
        (*map_port)[DB_name->value(row)] = arg1.toInt();
    }
    else if(index_data_connections == index_data_connections_comment){
        (*map_comment)[DB_name->value(row)] = arg1;
    }
}

void connection_settings::update_data_connections(const QString &arg1, const int index_data_connections)
{
    QModelIndexList index_list = ui->tableWidget_nameDB->selectionModel()->selectedIndexes();
    if(index_list.isEmpty()){

        ui->textEdit_errors->setText("Выберете базу данных");
    }
    else if(index_list.size() > 1){

        ui->textEdit_errors->setText("Выберете только одну базу данных");
    }
    else{

        int row = index_list[0].row();
        if(DB_was_created->value(row) == false){

            if(ui->tableWidget_nameDB->item(row, 0)->text() == ""){

                ui->textEdit_errors->setText("Введите имя базы данных");
            }
            else{

                set_data_connections(arg1, index_data_connections, row);
                ui->textEdit_errors->clear();
            }            
        }
        else{

            data_connections_set_cur_data(DB_name->value(row), index_data_connections);
            ui->textEdit_errors->setText("База данных создана, "
                                         "менять параметры соединение нельзя");
        }
    }
}

void connection_settings::data_connections_set_cur_data(const QString &selected_nameDB, const int index_data_connections)
{
    if(index_data_connections == index_data_connections_host){

        ui->lineEdit_host->setText(map_host->value(selected_nameDB));
    }
    else if(index_data_connections == index_data_connections_login){

        ui->lineEdit_login->setText(map_login->value(selected_nameDB));
    }
    else if(index_data_connections == index_data_connections_password){

        ui->lineEdit_password->setText(map_password->value(selected_nameDB));
    }
    else if(index_data_connections == index_data_connections_port){

        ui->spinBox_port->setValue(map_port->value(selected_nameDB));
    }
}


void connection_settings::on_tableWidget_nameDB_clicked(const QModelIndex &index)
{
    selected_nameDB = ui->tableWidget_nameDB->model()->data(index).toString();

    QString str_host =  map_host->value(selected_nameDB);
    QString str_login =  map_login->value(selected_nameDB);
    QString str_password =  map_password->value(selected_nameDB);
    QString str_comment = map_comment->value(selected_nameDB);
    int port = map_port->value(selected_nameDB);

    update_lineEdit_host(str_host);
    update_lineEdit_login(str_login);
    update_lineEdit_password(str_password);
    update_spinbox_port(port);
    update_lineEdit_comment(str_comment);
}


void connection_settings::slot_clean_all_data_connection()
{
    update_lineEdit_host("");
    update_lineEdit_login("");
    update_lineEdit_password("");
    update_spinbox_port(0);
    update_lineEdit_comment("");
}

void connection_settings::slot_update_all_data_conenctions(const QModelIndex &index_update)
{
    qDebug()<<"update: "<<QString::number(index_update.row());
    int row =index_update.row();
    if(DB_was_created->value(row) == false){

        QString old_name = DB_name->value(row);
        QString new_name = ui->tableWidget_nameDB->model()->data(index_update).toString();

        (*DB_name)[row] = new_name;

        map_host->remove(old_name);
        map_login->remove(old_name);
        map_password->remove(old_name);
        map_port->remove(old_name);
        map_comment->remove(old_name);

        map_host->insert(old_name, ui->lineEdit_host->text());
        map_login->insert(old_name, ui->lineEdit_login->text());
        map_password->insert(old_name, ui->lineEdit_password->text());
        map_port->insert(old_name, ui->spinBox_port->value());
        map_comment->insert(old_name, ui->textEdit_comment->toPlainText());
    }
    else{

        ui->tableWidget_nameDB->setItem(row, 0, new QTableWidgetItem(DB_name->value(row)));
    }
}

void connection_settings::slot_close_connectoins_setttings_window()
{
    this->close();
}

void connection_settings::on_open_clicked()
{
     QModelIndexList index_list = ui->tableWidget_nameDB->selectionModel()->selectedIndexes();
     if(index_list.isEmpty()){

         ui->textEdit_errors->setText("No selected");
     }
     else if(index_list.size() > 1){

         ui->textEdit_errors->setText("Many selected");
     }
     else{

         int row = index_list[0].row();
         QString name = DB_name->value(row);
         try_open_name_DB = name;
         emit signal_set_conections_settings_database(name,
                                                      map_host->value(name),
                                                      map_login->value(name),
                                                      map_password->value(name),
                                                      map_port->value(name));
         this->close();
     }
}
