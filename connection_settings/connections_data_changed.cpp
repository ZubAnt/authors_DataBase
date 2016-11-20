#include <QDebug>

#include "connection_settings.h"
#include "ui_connection_settings.h"


void connection_settings::update_lineEdit_host(const QString &str_host)
{
    ui->lineEdit_host->setText(str_host);
}

void connection_settings::update_lineEdit_login(const QString &str_login)
{
    ui->lineEdit_login->setText(str_login);
}

void connection_settings::update_lineEdit_password(const QString &str_password)
{
    ui->lineEdit_password->setText(str_password);
}

void connection_settings::update_spinbox_port(const int &port)
{
    ui->spinBox_port->setValue(port);
}

void connection_settings::update_lineEdit_comment(const QString &str_comment)
{
    ui->textEdit_comment->setText(str_comment);
}

void connection_settings::on_lineEdit_host_textChanged(const QString &arg1)
{
    update_data_connections(arg1, index_data_connections_host);
}

void connection_settings::on_lineEdit_login_textChanged(const QString &arg1)
{
    update_data_connections(arg1, index_data_connections_login);
}

void connection_settings::on_lineEdit_password_textChanged(const QString &arg1)
{
    update_data_connections(arg1, index_data_connections_password);
}
void connection_settings::on_spinBox_port_valueChanged(const QString &arg1)
{
    update_data_connections(arg1, index_data_connections_port);
}

void connection_settings::on_textEdit_comment_textChanged()
{
    update_data_connections(ui->textEdit_comment->toPlainText(), index_data_connections_comment);
}
