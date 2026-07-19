#include <iostream>
#include "database.h"

using namespace std;

MYSQL *conn;

bool connectDB()
{
    conn = mysql_init(NULL);

    if(conn == NULL)
        return false;

    conn = mysql_real_connect(
        conn,
        "127.0.0.1",
        "root",
        "",
        "pharmacy_db",
        3306,
        NULL,
        0
    );

    return conn != NULL;
}

void disconnectDB()
{
    if(conn)
        mysql_close(conn);
}