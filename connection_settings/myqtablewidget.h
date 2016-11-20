#ifndef MYQTABLEWIGET_H
#define MYQTABLEWIGET_H

#include <QTableWidget>
#include <QModelIndexList>
#include <QModelIndex>

class myQTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit myQTableWidget(QWidget *parent = 0);
    ~myQTableWidget();

protected:
    void keyPressEvent(QKeyEvent * event);
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);

signals:
    void signal_remove_rows(QTableWidget *tableView_change, const QModelIndexList &index_update);
    void signal_clean_all_data_connection();
    void signal_update_all_data_conenctions(const QModelIndex &index_rem_rows);

public slots:

};

#endif // MYQTABLEWIGET_H
