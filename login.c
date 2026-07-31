#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "login.h"
#include "ui.h"

int currentUserId = 0;

bool login(void)
{
    char username[128];
    char password[128];
    char usernameEscaped[256];
    char passwordEscaped[256];
    char sql[640];

    printHeader("LOGIN");
    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL)
        return false;
    username[strcspn(username, "\r\n")] = '\0';

    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
        return false;
    password[strcspn(password, "\r\n")] = '\0';

    if (!isValidUsername(username) || !isValidPassword(password))
    {
        printErrorFmt("Enter a valid username and password.");
        return false;
    }

    mysql_real_escape_string(conn, usernameEscaped, username, strlen(username));
    mysql_real_escape_string(conn, passwordEscaped, password, strlen(password));

    snprintf(sql, sizeof(sql), "SELECT user_id, full_name FROM users WHERE username='%s' AND password='%s'", usernameEscaped, passwordEscaped);

    if (mysql_query(conn, sql))
    {
        printf("Login query failed: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if(result == NULL)
        return false;

    if(mysql_num_rows(result) > 0)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        currentUserId = atoi(row[0]);
        setActiveUserDisplay(row[1]);
        mysql_free_result(result);
        return true;
    }

    mysql_free_result(result);
    return false;
}
