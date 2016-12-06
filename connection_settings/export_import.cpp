#include <QDebug>
#include <QTableWidgetItem>
#include <QFile>

#include "connection_settings.h"
#include "ui_connection_settings.h"


void connection_settings::export_data()
{
    QFile conn_set(file_name_connections_settings);
    if(!conn_set.open(QIODevice::WriteOnly | QIODevice::Text)){

        return;
    }
    else{

        QTextStream out_stream(&conn_set);

        int row = ui->tableWidget_nameDB->rowCount();
        out_stream << "Number of DB:\n  " << row << endl << endl;;

        for(int i = 0; i < row; ++i){

            QString Name_Db =  DB_name.value(i);
            out_stream << "Name Db:\n" << Name_Db << endl;
            out_stream << "WAS CREATED:\n   " << DB_was_created.value(i) << endl;
            out_stream << "Host:\n" << map_host.value(Name_Db) << endl;
            out_stream << "Login:\n" << map_login.value(Name_Db) << endl;
            out_stream << "Password:\n" << map_password.value(Name_Db) << endl;
            out_stream << "Port:\n" << map_port.value(Name_Db) << endl;
            out_stream << "Comment:\n" << map_comment.value(Name_Db) << endl << endl;
            ui->progressBar->setValue(((i + 1) / row) * 100);
        }

        conn_set.close();
    }
}

void connection_settings::import_data()
{
    QFile conn_set(file_name_connections_settings);
    if(!conn_set.open(QIODevice::ReadOnly | QIODevice::Text)){

        qDebug()<<"NOT OPEN";
        return;
    }
    else{

        QTextStream inp_stream(&conn_set);

        QString input_string;

        inp_stream.readLine();                  // забирает строку
        input_string = inp_stream.readLine();   // Количество БД
        inp_stream.readLine();                  // забирает пустую строку

        int number_of_DB = input_string.toInt();

        for(int i = 0 ; i < number_of_DB; ++i){

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // Название БД
            DB_name.insert(i, input_string);

            //Отображение названия базы данных
            ui->tableWidget_nameDB->insertRow(i);
            ui->tableWidget_nameDB->setItem(i, 0, new QTableWidgetItem(input_string));

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // was created
            DB_was_created.insert(i, input_string.toInt());

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // Host
            set_data_connections(input_string, index_data_connections_host, i);

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // Login
            set_data_connections(input_string, index_data_connections_login, i);

            inp_stream.readLine();                  //забирает строку
            input_string = inp_stream.readLine();   // Password
            set_data_connections(input_string, index_data_connections_password, i);

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // Port
            set_data_connections(input_string, index_data_connections_port, i);

            inp_stream.readLine();                  // забирает строку
            input_string = inp_stream.readLine();   // Comment
            set_data_connections(input_string, index_data_connections_comment, i);

            inp_stream.readLine();  //забирает пустую строку

            ui->progressBar->setValue(((i + 1) / number_of_DB) * 100);
        }
        conn_set.close();
    }
}
