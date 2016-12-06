#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../progress_readwrite/progress_readwrite.h"

#include <QDebug>
#include <QTextStream>
#include <QAxObject>
#include <QString>
#include <QFile>
#include <QFileDialog>

//http://www.wiki.crossplatform.ru/index.php/Работа_с_MS_Office_с_помощью_ActiveQt

void MainWindow::on_save_report_clicked()
{
    progressbar_write->show();
    progressbar_write->setWindowTitle("Генерация отчета");
    progressbar_write->set_progressbar_percent(0);
    progressbar_write->set_progressbar_process("Открытие Microsoft Word");

    report_docx_file = new QAxObject("Word.Application");
    progressbar_write->set_progressbar_percent(10);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "untitled.docx",
                                tr("Documents (*.doc *.docx)"));

    progressbar_write->activateWindow();

    if(fileName.isEmpty()){

        ui->statusBar->showMessage("Не был выбран файл");

        report_docx_file->dynamicCall("Quit()");
        delete report_docx_file;

        progressbar_write->close();
        return;
    }

    QFile conn_set(fileName);
    if(!conn_set.open(QIODevice::WriteOnly | QIODevice::Text)){

        ui->statusBar->showMessage("Файл не был открыт");

        report_docx_file->dynamicCall("Quit()");
        delete report_docx_file;

        progressbar_write->close();
        return;
    }

    conn_set.close();

    progressbar_write->set_progressbar_percent(20);

    QAxObject* document = report_docx_file->querySubObject("Documents");

    progressbar_write->set_progressbar_percent(40);
    progressbar_write->set_progressbar_process("Открытие документа");

    QVariant filename(fileName);
    QVariant confirmconversions(false);
    QVariant ReadOnlyRecommended(false);
    QVariant addtorecentfiles(false);
    QVariant passworddocument("");
    QVariant passwordtemplate("");
    QVariant revert(false);
    QAxObject* open_doc = document->querySubObject("Open(const QVariant&, const QVariant&, "
                                                   "const QVariant&, const QVariant&, const QVariant&, const QVariant&,"
                                                   "const QVariant&)", filename, confirmconversions, ReadOnlyRecommended,
                                                   addtorecentfiles, passworddocument, passwordtemplate, revert);


    progressbar_write->set_progressbar_percent(50);
    progressbar_write->set_progressbar_process("Применение параметров записи");

    QAxObject *ActiveDocument = report_docx_file->querySubObject("ActiveDocument");
    QAxObject *Range = ActiveDocument->querySubObject("Range()");
    QAxObject *font = Range->querySubObject("Font");

    Range->setProperty("Alignment", 1);
    font->setProperty("Size", 18);
    QAxObject *shrift = Range->querySubObject("Font");
    shrift->setProperty("Size", 12); //задаем размер шрифта
    shrift->setProperty("Name", "Times New Roman");

    QAxObject* selection = report_docx_file->querySubObject("Selection") ;
    QAxObject* Tables = selection->querySubObject("Tables()");

    int row_count = ui->report_tableWidget->rowCount();

    report_new_table = Tables->querySubObject("Add("
                                              "const QVariant&,"
                                              "const QVariant&, const QVariant&,"
                                              "const QVariant&, const QVariant&)",
                                              ActiveDocument->dynamicCall("Range()"),
                                              row_count + 1, rep_ind_max - 1,
                                              2, 2);

    progressbar_write->set_progressbar_percent(60);
    progressbar_write->set_progressbar_process("Запись отчета");

    insert_header_into_report_docx(); //first row
    for(int i = 0; i < row_count; ++i){

        insert_nstr_into_report_docx(i, 0, i);

        insert_text_into_report_docx(i, rep_ind_name, rep_ind_name);
        insert_text_into_report_docx(i, rep_ind_type_paper, rep_ind_type_paper);
        insert_text_into_report_docx(i, rep_ind_publish_hs, rep_ind_publish_hs);
        insert_volome_pl_into_report_docx(i, rep_ind_pl, rep_ind_pl, rep_ind_authors_pl);
        insert_text_into_report_docx(i, rep_ind_co_authors - 1, rep_ind_co_authors);

        progressbar_write->set_progressbar_percent(60 + ((((i + 1) * 100 / row_count) * 40)/ 100));
    }

    progressbar_write->set_progressbar_process("Закрытие активных документов...");

    QAxObject *new_ActiveDocument = report_docx_file->querySubObject("ActiveDocument");
    new_ActiveDocument->dynamicCall("Close()");
        report_docx_file->dynamicCall("Quit()");

    //http://www.wiki.crossplatform.ru/index.php/Работа_с_MS_Office_с_помощью_ActiveQt

    delete new_ActiveDocument;
    delete Tables;
    delete selection;
    delete shrift;
    delete font;
    delete Range;
    delete ActiveDocument;
    delete open_doc;
    delete document;

    delete report_docx_file;

    progressbar_write->close();
    ui->statusBar->showMessage("Успешное создание отчета");
}

void MainWindow::insert_header_into_report_docx()
{
    for(int col = 0; col < rep_ind_max - 1; ++col){
        QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", 1, col + 1) ;
        QAxObject* celR = cell->querySubObject("Range()");
        celR->dynamicCall("InsertAfter(Text)", report_headerlist[col]);

        delete celR;
        delete cell;
    }
}

///row and col >=1 for document
void MainWindow::insert_text_into_report_docx(const int row, const int col, const int rep_index)
{
    QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", row + 2, col + 1) ;
    QAxObject* celR = cell->querySubObject("Range()");
    celR->dynamicCall("InsertAfter(Text)",
                      ui->report_tableWidget->model()->data(ui->report_tableWidget->model()->index(row, rep_index)).toString());

    delete celR;
    delete cell;
}

void MainWindow::insert_nstr_into_report_docx(const int row, const int col, const int number_str)
{
    QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", row + 2, col + 1) ;
    QAxObject* celR = cell->querySubObject("Range()");
    celR->dynamicCall("InsertAfter(Text)", QString::number(number_str + 1));

    delete celR;
    delete cell;
}

void MainWindow::insert_volome_pl_into_report_docx(const int row, const int col, const int rep_index_pl, const int rep_index_auth_pl)
{
    QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", row + 2, col + 1) ;
    QAxObject* celR = cell->querySubObject("Range()");

    QString volome_pl = ui->report_tableWidget->model()->data(ui->report_tableWidget->model()->index(row, rep_index_pl)).toString() + \
            "\n" + \
            "-----\n" + \
            ui->report_tableWidget->model()->data(ui->report_tableWidget->model()->index(row, rep_index_auth_pl)).toString();
    celR->dynamicCall("InsertAfter(Text)", volome_pl);

    delete celR;
    delete cell;
}
