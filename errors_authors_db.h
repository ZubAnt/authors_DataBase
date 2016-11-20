#ifndef ERRORS_AUTHORS_DB_H
#define ERRORS_AUTHORS_DB_H

#include <QStringList>

enum authors_db_err{
    ITS_OK = 0,
    MISSING_DRIVERS_FOR_MYSQL = -1,
    DB_NOT_CREATED = -2,
    DB_NOT_OPEN = -3,
    TABLE_NOT_CREATED = -4,
    TABLE_NOT_OPEN = -5,
    NULL_POINTER = -6,
};

const QStringList authors_db_err_string = {
    "ITS_OK",
    "MISSING_DRIVERS_FOR_MYSQL",
    "DB_NOT_CREATED",
    "DB_NOT_OPEN",
    "TABLE_NOT_CREATED",
    "TABLE_NOT_OPEN",
    "NULL_POINTER"
};

typedef authors_db_err db_err;

#endif // ERRORS_AUTHORS_DB_H
