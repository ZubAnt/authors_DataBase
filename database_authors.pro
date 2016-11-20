#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T00:36:48
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = database_authors
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    download_publish_house_from_file.cpp \
    itemdelegate.cpp \
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
    connection_settings/export_import.cpp

HEADERS  += mainwindow.h \
    download_publish_house_from_file.h \
    errors_authors_db.h \
    itemdelegate.h \
    ./table/myqsqltablemodel.h \
    ./table/myqtableview.h \
    ./table/indexcolumndb.h \
    ./connection_settings/connection_settings.h \
    connection_settings/myqtablewidget.h

FORMS    += mainwindow.ui \
    ./connection_settings/connection_settings.ui

RESOURCES += \
    wtf.qrc
