#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlTableModel>
#include <QComboBox>
#include <QString>

#include "./table/indexcolumndb.h"

class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxItemDelegate(QSqlTableModel *type_of_pb,
                         QSqlTableModel *pb_house,
                         QObject* parent = 0);
    ~ComboBoxItemDelegate();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    void insert_data_co_authors(QAbstractItemModel* model, const QModelIndex& index, const QString &cur_text) const;
    int get_count_id_type_of_pb_table(const QString &id_value) const;
    int get_count_id_pb_house_table(const QString &id_value) const;
    QString get_pars_co_author(const QString &cur_text) const;

private:

    indexColumnDb *columnDb;

    QString string_select_data;

    QSqlTableModel *type_of_pb;
    QSqlTableModel *pb_house;
};

#endif // ITEMDELEGATE_H
