#ifndef MYQTABLEVIEW_H
#define MYQTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>
#include <QSqlTableModel>

#include "indexcolumndb.h"

class myQTableView : public QTableView
{
    Q_OBJECT

public:
    explicit myQTableView(QWidget *parent = 0);
    void tmp_close(QWidget *editor);
    ~myQTableView();

protected:
    void keyPressEvent(QKeyEvent * event);
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);

signals:
    void signal_remove_rows(QTableView *tableView_change, const int numb_tab, const QModelIndexList &index_rem_rows);
    void signal_get_numb_tab(int &numb_tab);

    void signal_fullscreen_enable();
    void signal_fullscreen_disable();

public slots:

private:
    indexColumnDb *columnDb;
};

#endif // MYQTABLEVIEW_H
