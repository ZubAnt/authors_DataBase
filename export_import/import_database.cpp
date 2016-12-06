#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QChar>
#include <QMessageBox>
#include <QList>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static void convert_QString_to_with_new_line_syb(const QString &str, QString &result_str){

    for(auto it = str.cbegin();
        it != str.cend();
        ++it)
    {
        if( *it == '\\' && (it + 1) !=str.cend() && *(it + 1) == 'n'){
            result_str.append('\n');
            ++it;
            continue;
        }
        result_str.append(*it);
    }
}

bool MainWindow::import_data_into_table(QList<QString> &list,
                                        QSqlTableModel *model,
                                        const QString &fileName_imp,
                                        const int shift_for_err)
{
    if(model == type_of_pb_table || model == pb_house_table)
    {
        if (list.length() != 2)
        {
            QMessageBox::StandardButton error_import;
            error_import = QMessageBox::question(this,
                                                 "Import database from .adb file",
                                                 "Возникла ошибка при чтении из " +
                                                 fileName_imp +
                                                 " в строке " +
                                                 QString::number(shift_for_err) +
                                                 ".\nЖелаете продолжить?",
                                                 QMessageBox::Yes | QMessageBox::No);
            if(error_import == QMessageBox::No){

                return false;
            }
            else{

                return true;
            }
        }

        int row = model->rowCount();

        QSqlRecord rec;

        if(model == type_of_pb_table){

            rec.append(QSqlField("type_of_pb_id", QVariant::Int));
        }
        else{

            rec.append(QSqlField("pb_house_id", QVariant::Int));
        }

        rec.append(QSqlField("Name", QVariant::String));

        rec.setValue(0, list[0].toInt());
        rec.setValue(1, list[1]);

        model->insertRecord(row, rec);

        return true;
    }
    else if (model == main_table)
    {
        if (list.length() != columnDb->index_max_lenght)
        {
            QMessageBox::StandardButton error_import;
            error_import = QMessageBox::question(this,
                                                 "Import database from .adb file",
                                                 "Возникла ошибка при чтении из " +
                                                 fileName_imp +
                                                 " в строке " +
                                                 QString::number(shift_for_err) +
                                                 ".\nЖелаете продолжить?",
                                                 QMessageBox::Yes | QMessageBox::No);
            if(error_import == QMessageBox::No){

                return false;
            }
            else{

                return true;
            }
        }

        int row = model->rowCount();
        QSqlRecord rec;

        rec.append(QSqlField("main_id",             QVariant::Int   ));
        rec.append(QSqlField("Название публикации", QVariant::String));
        rec.append(QSqlField("Вид публикации",      QVariant::Int   ));
        rec.append(QSqlField("Тип",                 QVariant::String));
        rec.append(QSqlField("Издательство",        QVariant::Int   ));
        rec.append(QSqlField("Год издания",         QVariant::Int   ));
        rec.append(QSqlField("Страницы",            QVariant::String));
        rec.append(QSqlField("пл",                  QVariant::String));
        rec.append(QSqlField("Авт пл",              QVariant::String));
        rec.append(QSqlField("Соавторы",            QVariant::String));
        rec.append(QSqlField("Гриф",                QVariant::String));
        rec.append(QSqlField("Примечание",          QVariant::String));

        QString conv_co_authors;
        QString conv_annotation;
        convert_QString_to_with_new_line_syb(list[columnDb->index_co_authors],
                                                      conv_co_authors);
        convert_QString_to_with_new_line_syb(list[columnDb->index_annotation],
                                                      conv_annotation);

        rec.setValue(columnDb->index_main_id,    list[columnDb->index_main_id].toInt());
        rec.setValue(columnDb->index_name_of_pb, list[columnDb->index_name_of_pb]);
        rec.setValue(columnDb->index_type_of_pb, list[columnDb->index_type_of_pb].toInt());
        rec.setValue(columnDb->index_type_paper, list[columnDb->index_type_paper]);
        rec.setValue(columnDb->index_publish_hs, list[columnDb->index_publish_hs].toInt());
        rec.setValue(columnDb->index_year_pb,    list[columnDb->index_year_pb].toInt());
        rec.setValue(columnDb->index_numberpage, list[columnDb->index_numberpage]);
        rec.setValue(columnDb->index_pl,         list[columnDb->index_pl]);
        rec.setValue(columnDb->index_authors_pl, list[columnDb->index_authors_pl]);
        rec.setValue(columnDb->index_co_authors, conv_co_authors);
        rec.setValue(columnDb->index_secur_clas, list[columnDb->index_secur_clas]);
        rec.setValue(columnDb->index_annotation, conv_annotation);

        model->insertRecord(row, rec);

        return true;
    }

    return false;
}

void MainWindow::on_import_databases_triggered()
{
    if(!db.isOpen()){

        ui->statusBar->showMessage("Please connect to the database");
        return;
    }

    progressbar_write->show();
    progressbar_write->setWindowTitle("Импорт базы данных");
    progressbar_write->set_progressbar_percent(0);
    progressbar_write->set_progressbar_process("Таблицы 0 из 3. Импорт таблицы \"Вид публикации\"");

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Import database form .adb file",
                                  "Вы уверены, что хотете загрузить базу данных?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No){

        progressbar_write->close();
        return;
    }

    QString fileName_imp = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "dump.adb",
                                                    tr("ADB file (*.adb)"));
    QFile imp(fileName_imp);
    if(!imp.open(QIODevice::ReadOnly | QIODevice::Text)){

        ui->statusBar->showMessage("BAD OPEN FILE");
        progressbar_write->close();
        return;
    }
    else{

        progressbar_write->activateWindow();
        QTextStream stream(&imp);
        stream.setCodec("UTF-8");

        int rowcount_main_table = 0;
        int rowcount_type_of_pb_table = 0;
        int rowcount_pb_house_table = 0;

        /// Забираем комментарий
        int comment_count = 5;
        for(int i = 0; i < comment_count; ++i){
           stream.readLine();
        }

        ///read type_of_pb_table==========================================
        rowcount_type_of_pb_table = stream.readLine().toInt();
        stream.readLine(); // '{'
        for(int i = 0; i < rowcount_type_of_pb_table; ++i)
        {
            QList<QString> list = stream.readLine().split(delimer_symbol);
            bool check = import_data_into_table(list,
                                                type_of_pb_table,
                                                fileName_imp,
                                                i + comment_count + 1);
            if(!check){

                break;
            }
        }
        stream.readLine(); // '}'
        ///===============================================================

        stream.readLine(); // '\n'
        stream.readLine(); // 'Name table'

        ///read pb_house_table=====================================================================
        progressbar_write->set_progressbar_process("Таблицы 1 из 3. Импорт таблицы \"Издательство\"");
        progressbar_write->set_progressbar_percent(33);
        rowcount_pb_house_table = stream.readLine().toInt();
        stream.readLine(); // '{'
        for(int i = 0; i < rowcount_pb_house_table; ++i)
        {
            QList<QString> list = stream.readLine().split(delimer_symbol);
            bool check = import_data_into_table(list,
                                                pb_house_table,
                                                fileName_imp,
                                                i + comment_count + 3 + rowcount_type_of_pb_table);
            if(!check){

                break;
            }
        }
        stream.readLine(); // '}'
        ///========================================================================================

        stream.readLine(); // '\n'
        stream.readLine(); // 'Name table'

        ///read main_table=====================================================================
        progressbar_write->set_progressbar_process("Таблицы 2 из 3. Импорт таблицы \"Главная\"");        
        progressbar_write->set_progressbar_percent(66);
        rowcount_main_table = stream.readLine().toInt();
        stream.readLine(); // '{'
        for(int i = 0; i < rowcount_main_table; ++i)
        {
            QList<QString> list = stream.readLine().split(delimer_symbol);
            bool check = import_data_into_table(list,
                                                main_table,
                                                fileName_imp,
                                                i + comment_count + 5 +
                                                rowcount_type_of_pb_table +
                                                rowcount_pb_house_table);
            if(!check){

                break;
            }
            progressbar_write->set_progressbar_percent(66 + ((i + 1)* 34) / rowcount_main_table);
        }
        ///====================================================================================

        ui->statusBar->showMessage("Успешный импорт базы данных");
        imp.close();
        progressbar_write->close();
    }
}
