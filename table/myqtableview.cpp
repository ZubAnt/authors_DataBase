#include <QDebug>
#include <QModelIndex>
#include <QModelIndexList>
#include <QString>

#include "myqtableview.h"

myQTableView::myQTableView(QWidget *parent) :
    QTableView(parent)
{
    columnDb = new indexColumnDb();
}
myQTableView::~myQTableView(){
    delete columnDb;
}

void myQTableView::keyPressEvent(QKeyEvent *event)
{
    int cur_key = event->key();

    if(cur_key == Qt::Key_Delete){
        QModelIndexList rows_select = this->selectionModel()->selectedIndexes();
        qDebug()<< "DELETE ROWS";
        int numb_tab = 0;
        if(!rows_select.isEmpty()){
            emit signal_get_numb_tab(numb_tab);
            emit signal_remove_rows(this, numb_tab, rows_select);
        }
    }
    else if(cur_key == Qt::Key_F5){
        emit signal_fullscreen_enable();
    }
    else if(cur_key == Qt::Key_Escape){
        emit signal_fullscreen_disable();
    }
}

void myQTableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint){

/* The problem we're trying to solve here is the edit-and-go-away problem.
 * When ending the editing with submit or return, there's no problem, the
 * model's submit()/revert() is correctly called. However, when ending
 * editing by clicking away, submit() is never called. Fortunately,
 * closeEditor is called and, AFAIK, it's the only case where it's called
 * with NoHint (0). So, in these cases, we want to call model.submit()
 */
       if (hint == QAbstractItemDelegate::NoHint){
           QTableView::closeEditor(editor, QAbstractItemDelegate::SubmitModelCache);
       }
       QTableView::closeEditor(editor, hint);
       // можно включить, чтобы изменял следующую ячейку
       //QTableView::closeEditor(nullptr, QAbstractItemDelegate::EditNextItem);
}
