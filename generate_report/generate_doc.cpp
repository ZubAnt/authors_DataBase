#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "./generate_report/progress_write.h"

#include <QDebug>
#include <QTextStream>
#include <QAxObject>
#include <QString>
#include <QFile>
#include <QFileDialog>

//http://www.wiki.crossplatform.ru/index.php/Работа_с_MS_Office_с_помощью_ActiveQt

void MainWindow::on_save_report_clicked()
{
    progressbar_write_report->show();
    progressbar_write_report->set_progressbar(0);

    report_docx_file = new QAxObject("Word.Application");
    progressbar_write_report->set_progressbar(10);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "untitled.docx",
                                tr("Documents (*.doc *.docx)"));

    progressbar_write_report->activateWindow();

    if(fileName.isEmpty()){

        ui->statusBar->showMessage("Не был выбран файл");

        report_docx_file->dynamicCall("Quit()");
        delete report_docx_file;

        progressbar_write_report->close();
        return;
    }

    QFile conn_set(fileName);
    if(!conn_set.open(QIODevice::WriteOnly | QIODevice::Text)){

        ui->statusBar->showMessage("Файл не был открыт");

        report_docx_file->dynamicCall("Quit()");
        delete report_docx_file;

        progressbar_write_report->close();
        return;
    }

    conn_set.close();

    progressbar_write_report->set_progressbar(20);

    QAxObject* document = report_docx_file->querySubObject("Documents");

    progressbar_write_report->set_progressbar(40);

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


    progressbar_write_report->set_progressbar(50);

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
                                              row_count, rep_ind_max + 1,
                                              2, 2);

    progressbar_write_report->set_progressbar(60);

    for(int i = 0; i < row_count; ++i){

        insert_nstr_into_report_docx(i, 0, i);

        insert_text_into_report_docx(i, rep_ind_name       + 1, rep_ind_name);
        insert_text_into_report_docx(i, rep_ind_type_paper + 1, rep_ind_type_paper);
        insert_text_into_report_docx(i, rep_ind_publish_hs + 1, rep_ind_publish_hs);
        insert_text_into_report_docx(i, rep_ind_volome_pl  + 1, rep_ind_volome_pl);
        insert_text_into_report_docx(i, rep_ind_co_authors + 1, rep_ind_co_authors);

        progressbar_write_report->set_progressbar(60 + ((((i + 1) * 100 / row_count) * 40)/ 100));
    }

    QAxObject *new_ActiveDocument = report_docx_file->querySubObject("ActiveDocument");
    new_ActiveDocument->dynamicCall("Close()");
    report_docx_file->dynamicCall("Quit()");

    //http://www.wiki.crossplatform.ru/index.php/Работа_с_MS_Office_с_помощью_ActiveQt

    delete new_ActiveDocument;
    delete Tables;
    delete selection;
    delete font;
    delete Range;
    delete ActiveDocument;
    delete open_doc;
    delete document;
    delete report_docx_file;

    progressbar_write_report->close();
    ui->statusBar->showMessage("Успешное создание отчета");
}

///row and col >=1 for document
void MainWindow::insert_text_into_report_docx(const int row, const int col, const int rep_index)
{
    QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", row + 1, col + 1) ;
    QAxObject* celR = cell->querySubObject("Range()");
    celR->dynamicCall("InsertAfter(Text)",
                      ui->report_tableWidget->model()->data(ui->report_tableWidget->model()->index(row, rep_index)).toString());

    delete celR;
    delete cell;
}

void MainWindow::insert_nstr_into_report_docx(const int row, const int col, const int number_str)
{
    QAxObject* cell = report_new_table->querySubObject("Cell(Row, Column)", row + 1, col + 1) ;
    QAxObject* celR = cell->querySubObject("Range()");
    celR->dynamicCall("InsertAfter(Text)", QString::number(number_str + 1));

    delete celR;
    delete cell;
}
