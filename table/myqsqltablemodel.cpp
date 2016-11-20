#include <QSqlRecord>
#include <QVariant>
#include <string>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>

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

static void first_symb_upper(QString &to_upper_strig){

    int up_pos = 0;
    int len = to_upper_strig.length();
    for(int i = 0; i < len; ++i){
        if(to_upper_strig[i] == '\n' || i == len - 1){
            QChar c = to_upper_strig[up_pos];
            if(c >= 'a' || c <= 'z'){
                to_upper_strig.replace(up_pos, 1, c.toUpper());
            }
            up_pos = i+1;
        }
    }
}
