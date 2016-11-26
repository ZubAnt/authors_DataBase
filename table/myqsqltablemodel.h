#ifndef MY_QSQLTABLEMODEL_H
#define MY_QSQLTABLEMODEL_H

#include <QVariant>
#include <QSqlTableModel>
#include <QTableView>
#include <QModelIndex>
#include <QString>
#include <QSqlDatabase>
#include <QSet>

#include "myqtableview.h"
#include "indexcolumndb.h"

/*
 * Данный класс унаследован с целью отображения в ячейках, ссылающиеся на другие таблицы,
 * значений, соответствующие id-шникам, которые храняться в этих ячейках.
 * Грубо говоря таблица Main в столбце "1" содержить FOREIGN KEY типа INT, которые являються id-шниками
 * другой таблицы(делаем отношение между таблицами). После того, как мы выбрали из comboBox какой id
 * храниться в этой ячейке, данный класс переопределяет, какой значение отображать.
 * Если этого не делать, то будут отображаться id, а не значения соответствующие данным id
 * В конструктор передаються таблицы, у которых будет подмениваться отображение.
 * Qt::DisplayRole - флаг, отвечающий, за отображение в ячейке
 * Qt::EditRole - флаг, отвечающий, за значение, которе ячейка хранит.
 *
 * Таким образом, ячейка хранит одно, а отображает другое: хранит id, отображает Name соответствующее id
 *
 */

class MyQSqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:

    MyQSqlTableModel(QObject* parent,
                     QSqlDatabase *db,
                     QSqlTableModel *type_of_pb,
                     QSqlTableModel *pb_house);

    ~MyQSqlTableModel();

    QVariant data(const QModelIndex &index, int role) const;

    int get_count_id_type_of_pb_table(const int id) const;
    int get_count_id_pb_house(const int id) const;  
    QColor calculateColorForRow(const int row) const;
    void add_to_set_of_added_rows(const int row);
    bool check_presence(const int row); //проверка на присутствие
                                        //в множестве added_to_report_rows_set строки

public slots:
private:

    indexColumnDb *columnDb;

    QSqlTableModel *type_of_pb; //указатель на таблицу Вид публикации (type_of_publication)
    QSqlTableModel *pb_house;   //указатель на таблицу Издательство   (publishing_house)
    QSqlDatabase *db;           //указатель на базу данных

    QSet<int> *added_to_report_rows_set;    //Указатель на set хранящий индексы строк,
                                            //добавленные в отчет
};

#endif // MY_QSQLTABLEMODEL_H
