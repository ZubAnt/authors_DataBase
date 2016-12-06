#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T00:36:48
#
#-------------------------------------------------

QT       += core gui axcontainer
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ADB
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ./table/myqsqltablemodel.cpp \
    ./table/myqtableview.cpp \
    ./table/indexcolumndb.cpp \
    ./table/init_tableview.cpp \
    ./table/createtable.cpp \
    init_connect.cpp \
    print_errors.cpp \
    ./connection_settings/connection_settings.cpp \
    connection_settings/myqtablewdiget.cpp \
    connection_settings/connections_data_changed.cpp \
    connection_settings/export_import.cpp \
    ./combobox/itemdelegate.cpp \
    ./generate_report/generate_doc.cpp \
    ./progress_readwrite/progress_readwrite.cpp \
    export_import/import_database.cpp \
    export_import/export_databases.cpp

HEADERS  += mainwindow.h \
    errors_authors_db.h \
    ./table/myqsqltablemodel.h \
    ./table/myqtableview.h \
    ./table/indexcolumndb.h \
    ./connection_settings/connection_settings.h \
    connection_settings/myqtablewidget.h \
    ./combobox/itemdelegate.h \
    ./progress_readwrite/progress_readwrite.h

FORMS    += mainwindow.ui \
    ./connection_settings/connection_settings.ui \
    ./progress_readwrite/progress_readwrite.ui

RESOURCES += \
    image/image.qrc

RC_FILE = ADB.rc
