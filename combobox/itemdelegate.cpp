#include <QComboBox>
#include <QDebug>
#include <QString>
#include <QSqlRecord>
#include <QTextEdit>
#include <typeinfo.h>
#include <QTableView>
#include <QFont>

#include "itemdelegate.h"
#include "../table/indexcolumndb.h"

/*
 * данный класс создает комбобокс-ы для различных столбцов
 * эти столбцы хранят id из других таблиц.
 * некоторые столбцы принимают ограниченное количество значений, для них комбобокс делаеться по-своему
 *
 */

static void upper_first_symp(QString &to_upper_strig){

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

ComboBoxItemDelegate::ComboBoxItemDelegate(QSqlTableModel *type_of_pb,
                                           QSqlTableModel *pb_house,
                                           QObject* parent)
    : QStyledItemDelegate(parent), columnDb(new indexColumnDb())
{
    this->type_of_pb = type_of_pb;
    this->pb_house = pb_house;
    string_select_data = "Select the new value";
}


ComboBoxItemDelegate::~ComboBoxItemDelegate()
{
    delete columnDb;
}

QWidget* ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int column = index.column();

    // ComboBox ony in index_type_of_pb (type_ob_pb)
    if(column == columnDb->index_type_of_pb){

        // Create the combobox and populate it
        QComboBox *cb = new QComboBox(parent);

        //добавляем первое значение в комбобкс текущее значение
        cb->addItem(string_select_data);

        //Добавляем все остальное
        for(int i = 0; i < type_of_pb->rowCount(); ++i){
            cb->addItem(type_of_pb->record(i).value(1).toString());
        }

        return cb;
    }
    // ComboBox ony in index_type_paper (тип, рукописный или печатный)
    else if(column == columnDb->index_type_paper){
        QComboBox *cb = new QComboBox(parent);
        cb->addItem("рук.");
        cb->addItem("печ.");
        return cb;
    }
    // ComboBox ony in index_publish_hs (pb_house)
    else if(column == columnDb->index_publish_hs){
        QComboBox *cb = new QComboBox(parent);
        cb->addItem(string_select_data);
        for(int i = 0; i < pb_house->rowCount(); ++i){
            cb->addItem(pb_house->record(i).value(1).toString());
        }
        return cb;
    }
    else if(column == columnDb->index_co_authors){
        QTextEdit *txted = new QTextEdit(parent);
        return txted;
    }
    else if(column == columnDb->index_secur_clas){
        QComboBox *cb = new QComboBox(parent);
        cb->addItem("-");
        cb->addItem("c.");
        cb->addItem("дсп");
        return cb;
    }
    else if(column == columnDb->index_annotation){
        QTextEdit *txted = new QTextEdit(parent);
        return txted;
    }
    else{
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    return NULL;
}


void ComboBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
        // get the index of the text in the combobox that matches the current value of the itenm

        QString currentText = index.data(Qt::EditRole).toString();

        int cbIndex = cb->findText(currentText);

        // if it is valid, adjust the combobox
        if (cbIndex >= 0)
            cb->setCurrentIndex(cbIndex);
    }
    else if (QTextEdit* txted = qobject_cast<QTextEdit*>(editor)){
        txted->insertPlainText(index.data(Qt::EditRole).toString());
    }

    else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ComboBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)){

        int column = index.column();

        //type_of_pb
        if( column == columnDb->index_type_of_pb){
            // поиск id введенного текста
            int insert_id = get_count_id_type_of_pb_table(cb->currentText());
            // В нулевой строке находится инфо-строка
            if(insert_id >= 0){
                model->setData(index, insert_id, Qt::EditRole); //вводим id
            }
            return;
        }
        //тип, рук. или печ.
        else if(column == columnDb->index_type_paper){

            model->setData(index, cb->currentText(), Qt::EditRole);
            return;
        }
        //pb_house
        else if(column == columnDb->index_publish_hs){

            int insert_id = get_count_id_pb_house_table(cb->currentText());
            if(insert_id >= 0){
                model->setData(index, insert_id, Qt::EditRole); //вводим id
            }
            return;
        }
        else if(column == columnDb->index_secur_clas){
            model->setData(index, cb->currentText(), Qt::EditRole);
            return;
        }
    }

    else if(QTextEdit* txted = qobject_cast<QTextEdit*>(editor)){

        int column = index.column();

        if(column == columnDb->index_co_authors){
            QString text = txted->toPlainText();
            model->setData(index, QVariant(txted->toPlainText()), Qt::EditRole);
            return;
        }
        else if(column == columnDb->index_annotation){
            QString text = txted->toPlainText();
            model->setData(index, QVariant(txted->toPlainText()), Qt::EditRole);
            return;
        }
    }

    else
        QStyledItemDelegate::setModelData(editor, model, index);
}

void ComboBoxItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem op(option);

    bool check = false;
    emit signal_check_row_in_added_set(index.row(), check);
    if(check == false){

        QFont f("MS Shell Dlg 2");
        f.setPointSize(9);
        op.font.setStyle(f.style());
        op.palette.setColor(QPalette::Normal, QPalette::Background, Qt::white);
    }

    QStyledItemDelegate::paint(painter, op, index);
}

QString ComboBoxItemDelegate::get_pars_co_author(const QString &cur_text) const{

    QString return_text;
    int start = 0;
    for(int i = 0; i < cur_text.length(); ++i){
        if(cur_text[i] == ','){

            return_text.append(cur_text.mid(start, i - start));

            return_text.append("\n");

            if((i+1) != cur_text.length()){
                if(cur_text[i+1] == ' '){
                    start = i+2;
                }
                else{
                    start = i+1;
                }
            }
            else{
                goto skip; //Вдруг на конце поставят запятую, то тогда нужно скипнуть считывание
            }
        }
    }

    return_text.append(cur_text.mid(start, cur_text.length() - start));
    upper_first_symp(return_text);
    skip:
    return return_text;
}

//возвращает индекс значения, хранящееся в таблице type_of_publication
int ComboBoxItemDelegate::get_count_id_type_of_pb_table(const QString &id_value) const{

    int find_id = 0;
    bool find_check = false;
    int row_count =  type_of_pb->rowCount();

    for(int i = 0; i < row_count; ++i){
        if(type_of_pb->record(i).value(1).toString() == id_value){
            find_id = type_of_pb->record(i).value(0).toInt();
            find_check = true;
            break;
        }
    }

    if(!find_check){
        return -1;
    }

    return find_id;
}

//возвращает индекс значения, хранящееся в таблице publishing_house
int ComboBoxItemDelegate::get_count_id_pb_house_table(const QString &id_value) const{

    int find_id = 0;
    bool find_check = false;
    int row_count =  pb_house->rowCount();

    for(int i = 0; i < row_count; ++i){
        if(pb_house->record(i).value(1).toString() == id_value){
            find_id = pb_house->record(i).value(0).toInt();
            find_check = true;
            break;
        }
    }

    if(!find_check){
        return -1;
    }

    return find_id;
}
