#ifndef CONNECTION_SETTINGS_H
#define CONNECTION_SETTINGS_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QModelIndex>

namespace Ui {
class connection_settings;
}

class connection_settings : public QWidget
{
    Q_OBJECT

public:
    explicit connection_settings(QWidget *parent = 0);
    void set_was_created(const QString &name, bool was_created);
    const QString get_trying_open_DB();
    ~connection_settings();

public slots:
    void slot_clean_all_data_connection();
    void slot_update_all_data_conenctions(const QModelIndex &index_update);
    void slot_close_connectoins_setttings_window();

private slots:
    void on_create_DB_clicked();

    void on_delete_DB_clicked();

    void on_save_DB_clicked();

    void on_tableWidget_nameDB_clicked(const QModelIndex &index);

    void update_lineEdit_host(const QString &str_host);
    void update_lineEdit_login(const QString &str_login);
    void update_lineEdit_password(const QString &str_password);
    void update_spinbox_port(const int &port);
    void update_lineEdit_comment(const QString &str_comment);

    void on_lineEdit_host_textChanged(const QString &arg1);

    void on_open_clicked();

    void on_lineEdit_login_textChanged(const QString &arg1);

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_textEdit_comment_textChanged();

    void on_spinBox_port_valueChanged(const QString &arg1);

signals:
    void signal_set_conections_settings_database(const QString &name,
                                                 const QString &host,
                                                 const QString &login,
                                                 const QString &password,
                                                 const int port);

private:
    Ui::connection_settings *ui;

    QString file_name_connections_settings;

    int index_data_connections_host;
    int index_data_connections_login;
    int index_data_connections_password;
    int index_data_connections_port;
    int index_data_connections_comment;

    QMap<int, QString> DB_name;
    QMap<int, bool> DB_was_created;

    QMap<QString, QString> map_host;
    QMap<QString, QString> map_login;
    QMap<QString, QString> map_password;
    QMap<QString, int> map_port;
    QMap<QString, QString> map_comment;

    QString selected_nameDB;
    QString try_open_name_DB;

    void init_tableWidget_nameDB();
    void update_tableWidget_nameDB(const int row_count);
    void set_data_connections(const QString &arg1, const int index_data_connections, const int row);
    void update_data_connections(const QString &arg1, const int index_data_connections);
    void data_connections_set_cur_data(const QString &arg1, const int index_data_connections);
    void export_data();
    void import_data();
};

#endif // CONNECTION_SETTINGS_H
