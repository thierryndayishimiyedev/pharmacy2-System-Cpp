#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

extern MYSQL *conn;

bool connectDB();
void disconnectDB();

#endif