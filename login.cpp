#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "login.h"

int currentUserId = 0;

using namespace std;

bool login()
{
    string username,password;

    cout<<"\nUsername: ";
    cin>>username;

    cout<<"Password: ";
    cin>>password;

    string sql="SELECT * FROM users WHERE username='"+username+"' AND password='"+password+"'";

    if(mysql_query(conn,sql.c_str()))
    {
        cout<<"Query Error\n";
        return false;
    }

    MYSQL_RES *result=mysql_store_result(conn);

    if(mysql_num_rows(result)>0)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        currentUserId = atoi(row[0]); // row[0] is user_id, since SELECT * starts with that column
        mysql_free_result(result);
        return true;
    }

    mysql_free_result(result);

    return false;
}