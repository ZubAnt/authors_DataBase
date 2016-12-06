#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QSqlRecord>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static void convert_QString_to_without_new_line_syb(const QString &str, QString &result_str){

    for(auto it = str.cbegin();
        it != str.cend();
        ++it)
    {
        if( *it == '\n'){
            result_str.append("\\n");
            continue;
        }
        result_str.append(*it);
    }
}

void MainWindow::on_export_databases_triggered()
{
    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Export database to .adb file",
                                  "Вы уверены, что хотете сохранить базу данных в .adb файл?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No){

        return;
    }

    QString fileName_exp = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "dump.adb",
                                                    tr("ADB file (*.adb)"));
    QFile exp(fileName_exp);
    if(!exp.open(QIODevice::WriteOnly | QIODevice::Text)){

        ui->statusBar->showMessage("BAD OPEN FILE");
        return;
    }
    else{        

        QTextStream out_stream(&exp);
        out_stream.setCodec("UTF-8");

        int rowcount_main_table = main_table->rowCount();
        int rowcount_type_of_pb_table = type_of_pb_table->rowCount();
        int rowcount_pb_house_table = pb_house_table->rowCount();
        int summ_rowcount = rowcount_main_table + rowcount_type_of_pb_table + rowcount_pb_house_table;

        QString date_comment = QDate::currentDate().toString() + " " + QTime::currentTime().toString();
        QString line_comment(date_comment.length() + 2, '-');

        out_stream << line_comment << endl;
        out_stream << "|" + date_comment + "|" << endl;
        out_stream << line_comment << endl << endl;

        ///write type_of_pb_table==========================================================
        out_stream << "Name table = " << type_of_pb_table_name
                   << "; rowcount = " + QString::number(rowcount_type_of_pb_table) << endl;
        out_stream << QString::number(rowcount_type_of_pb_table) << endl;
        out_stream << "{" << endl;
        for(int i = 0; i < rowcount_type_of_pb_table; ++i){
            out_stream << type_of_pb_table->record(i).value(0).toString() << delimer_symbol
                       << type_of_pb_table->record(i).value(1).toString() << endl;
            ui->statusBar->showMessage(QString::number((i / summ_rowcount) * 100));
        }
        out_stream << "}" << endl << endl;
        ///================================================================================

        ///write type_of_pb_table========================================================
        progressbar_write->set_progressbar_process("Запись таблицы \"Издательство\"");
        out_stream << "Name table = " << pb_house_table_name
                   << "; rowcount = " + QString::number(rowcount_pb_house_table) << endl;
        out_stream << QString::number(rowcount_pb_house_table) << endl;
        out_stream << "{" << endl;
        for(int i = 0; i < rowcount_pb_house_table; ++i){
            out_stream << pb_house_table->record(i).value(0).toString() << delimer_symbol
                       << pb_house_table->record(i).value(1).toString() << endl;
            ui->statusBar->showMessage(QString::number(
                                           (i + rowcount_type_of_pb_table) /
                                            summ_rowcount * 100));

        }
        out_stream << "}" << endl << endl;
        ///==============================================================================

        ///write main_table==========================================================================
        progressbar_write->set_progressbar_process("Запись таблицы \"Главная\"");
        out_stream << "Name table = " << main_table_name
                   << "; rowcount = " + QString::number(rowcount_main_table) << endl;
        out_stream << QString::number(rowcount_main_table) << endl;
        out_stream << "{" << endl;
        for(int i = 0; i < rowcount_main_table; ++i){

            QString conv_co_authors;
            QString conv_annotation;
            convert_QString_to_without_new_line_syb(main_table->record(i).value(columnDb->index_co_authors).toString(),
                                                          conv_co_authors);
            convert_QString_to_without_new_line_syb(main_table->record(i).value(columnDb->index_annotation).toString(),
                                                          conv_annotation);

            out_stream << main_table->record(i).value(columnDb->index_main_id).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_name_of_pb).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_type_of_pb).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_type_paper).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_publish_hs).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_year_pb).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_numberpage).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_pl).toString() << delimer_symbol
                       << main_table->record(i).value(columnDb->index_authors_pl).toString() << delimer_symbol
                       << conv_co_authors << delimer_symbol
                       << main_table->record(i).value(columnDb->index_secur_clas).toString() << delimer_symbol
                       << conv_annotation << endl;

            ui->statusBar->showMessage(QString::number(
                                           (i + rowcount_type_of_pb_table +
                                            rowcount_pb_house_table / summ_rowcount) * 100));
        }
        out_stream << "}" << endl << endl;
        ///==========================================================================================
        progressbar_write->close();
        ui->statusBar->showMessage("Успешное сохранение базы данных");
        exp.close();
    }
}
