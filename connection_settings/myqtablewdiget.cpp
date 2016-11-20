#include <QDebug>

#include "myqtablewidget.h"


myQTableWidget::myQTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

myQTableWidget::~myQTableWidget(){
}

void myQTableWidget::keyPressEvent(QKeyEvent *event)
{

}

void myQTableWidget::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    qDebug()<<"closeEditor";
    if (hint == QAbstractItemDelegate::NoHint){
        QTableWidget::closeEditor(editor, QAbstractItemDelegate::SubmitModelCache);
    }
    QTableWidget::closeEditor(editor, hint);

    QModelIndexList index_list = this->selectionModel()->selectedIndexes();
    emit signal_update_all_data_conenctions(index_list[0]);
}


