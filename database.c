#include <mysql.h>
#include "database.h"

MYSQL *conn;

bool connectDB(void)
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

void disconnectDB(void)
{
    if(conn)
        mysql_close(conn);
}
