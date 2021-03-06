#include <QSqlRecord>
#include <QVariant>
#include <string>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QColor>

#include "myqsqltablemodel.h"

MyQSqlTableModel::MyQSqlTableModel(QObject* parent,
                                   QSqlDatabase *db,
                                   QSqlTableModel *type_of_pb,
                                   QSqlTableModel *pb_house) : QSqlTableModel(parent)
{
    this->db = db;
    this->type_of_pb = type_of_pb;
    this->pb_house = pb_house;

    columnDb = new indexColumnDb();
}
MyQSqlTableModel::~MyQSqlTableModel(){

    delete columnDb;
}

QVariant MyQSqlTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {

        QVariant v = QSqlTableModel::data(index);

        //для вида публикации (type of publication)
        if(index.column() == columnDb->index_type_of_pb){

            //поиск индекса id в таблице type_of pb
            int count_id = get_count_id_type_of_pb_table(QSqlTableModel::data(index).toInt());
            //возвращает наименование из type_of_pb, по соответсвующему id
            return type_of_pb->record(count_id).value(1).toString();
        }
        //для издательства (publishing house)
        if(index.column() == columnDb->index_publish_hs){
            int count_id = get_count_id_pb_house(QSqlTableModel::data(index).toInt());
            return pb_house->record(count_id).value(1).toString();
        }



        return QSqlTableModel::data(index).toString();
    }

    if (role == Qt::BackgroundRole) {

        int main_id = this->record(index.row()).value(columnDb->index_main_id).toInt();
        QColor color = calculateColorForRow(main_id);
        return QBrush(color);
    }

    return QSqlTableModel::data(index, role);
}

int MyQSqlTableModel:: get_count_id_type_of_pb_table(const int id) const{

    int count_id = 0;

    for(int i = 0; i < type_of_pb->rowCount(); ++i){
        if(type_of_pb->record(i).value(0).toInt() == id){
            count_id = i;
            break;
        }
    }

    return count_id;
}

int MyQSqlTableModel:: get_count_id_pb_house(const int id) const{

    int count_id = 0;

    for(int i = 0; i < pb_house->rowCount(); ++i){
        if(pb_house->record(i).value(0).toInt() == id){
            count_id = i;
            break;
        }
    }

    return count_id;
}

QColor MyQSqlTableModel::calculateColorForRow(const int main_id) const
{
    if(added_to_report_rows_set.isEmpty()){

        return Qt::white;
    }

    auto it = added_to_report_rows_set.find(main_id);
    if(it != added_to_report_rows_set.constEnd()){

        return QColor("lightgreen");
    }

    return Qt::white;
}

void MyQSqlTableModel::add_to_set_of_added_rows(const int main_id)
{
    added_to_report_rows_set.insert(main_id);
}

void MyQSqlTableModel::del_form_set_of_added_rows(const int main_id)
{
    added_to_report_rows_set.remove(main_id);
}

bool MyQSqlTableModel::check_presence(const int main_id)
{
    if(added_to_report_rows_set.isEmpty()){

        return false;
    }

    auto it = added_to_report_rows_set.find(main_id);
    if(it != added_to_report_rows_set.end()){

        return true;
    }

    return false;
}
