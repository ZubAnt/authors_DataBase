#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFile>
#include <QApplication>
#include <QSqlTableModel>
#include <QList>
#include <QTableView>
#include <QAxObject>

#include "mainwindow.h"
#include "errors_authors_db.h"
#include "./combobox/itemdelegate.h"
#include "./table/myqsqltablemodel.h"
#include "./table/indexcolumndb.h"
#include "./connection_settings/connection_settings.h"
#include "./generate_report/progress_write.h"

namespace Ui {
class MainWindow;
class Download_PB;
}

class ConnectingDatabase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);    
    ~MainWindow();

    void init_type_of_pb_tableView();
    void init_pb_house_tableView();
    void init_main_tableView();
    void init_connect();

    QTableView *get_tableView(int current_tab);
    QSqlTableModel *get_table(int current_tab);

    void creeate_pb_house_table();
    void creeate_type_of_pb_table();
    void create_main_table();

    void print_err(const QString func, const QString file, const int line, db_err err_code);
    void warning_connection();
    void error_create_table(QSqlQuery &query);

public slots:

    void slot_remove_rows(QTableView *tableView_change, const int numb_tab, const QModelIndexList &index_rem_rows);
    void slot_get_numb_tab(int &numb_tab);

    void slot_fullscreen_enable_slot();
    void slot_fullscreen_disable_slot();

    void slot_set_conections_settings_database(const QString &name,
                                               const QString &host,
                                               const QString &login,
                                               const QString &password,
                                               const int port);

private slots:

    void on_Connect_DB_clicked();

    bool check_shure_download_db();

    void on_remove_row_clicked();
    void on_append_row_clicked();
    void on_insert_row_clicked();

    void on_add_to_report_clicked();

    void on_del_row_in_report_clicked();

    void on_save_report_clicked();

    void on_open_report_clicked();

protected:
    void keyPressEvent(QKeyEvent * event) override;
    void closeEvent(QCloseEvent *event);
signals:
    void signal_close_connectoins_setttings_window();

private:
    Ui::MainWindow *ui;
    connection_settings *connect_window;
    progress_write *progressbar_write_report;

    QSqlDatabase db;

    indexColumnDb *columnDb;

    const QString pb_house_table_name;      //Издательство
    const QString type_of_pb_table_name;    //Вид публикации
    const QString main_table_name;          //Главная

    const QString add_to_report_err_str;
    const QString choose_str_befor_inserting_err_str;
    const QString choose_str_for_adding_to_report_err_str;
    const QString choose_str_for_remove_from_report_err_str;

    int rep_ind_max;            //Максимальное количество столбцов в таблице report
    int rep_ind_name;           //Индекс в таблице report названия публикации
    int rep_ind_type_paper;     //Индекс в таблице report рук./печ.
    int rep_ind_publish_hs;     //Индекс в таблице report издательства
    int rep_ind_volome_pl;      //Индекс в таблице report объёма пл
    int rep_ind_co_authors;     //Индекс в таблице report соавторов

    QSqlTableModel *pb_house_table;
    QSqlTableModel *type_of_pb_table;
    MyQSqlTableModel *main_table;

    QAxObject *report_docx_file;
    QAxObject *report_new_table;

    ComboBoxItemDelegate* cmb_type_of_pb_table;

    void open_database(const QString &name);
    void clear_tables();
    void setwidth_column_main_table();
    void freeze_column_main_table();
    void init_report_tableWidget();
    void insert_text_into_report_docx(const int row, const int col, const int rep_index);
    void insert_nstr_into_report_docx(const int row, const int col, const int number_str);
    const QString find_pb_house_by_id(const int id);
};

#endif // MAINWINDOW_H
