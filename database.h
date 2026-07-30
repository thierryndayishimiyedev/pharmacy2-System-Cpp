#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>
#include <mysql.h>

extern MYSQL *conn;

bool connectDB(void);
void disconnectDB(void);

#endif
