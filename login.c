#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "database.h"
#include "login.h"

int currentUserId = 0;

static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool login(void)
{
    char username[128];
    char password[128];
    char usernameEscaped[256];
    char passwordEscaped[256];
    char sql[512];

    clearInput();

    printf("\nUsername: ");
    if (fgets(username, sizeof(username), stdin) == NULL)
        return false;
    username[strcspn(username, "\r\n")] = '\0';

    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
        return false;
    password[strcspn(password, "\r\n")] = '\0';

    if (username[0] == '\0' || password[0] == '\0')
        return false;

    mysql_real_escape_string(conn, usernameEscaped, username, strlen(username));
    mysql_real_escape_string(conn, passwordEscaped, password, strlen(password));

    snprintf(sql, sizeof(sql), "SELECT user_id FROM users WHERE username='%s' AND password='%s'", usernameEscaped, passwordEscaped);

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
        mysql_free_result(result);
        return true;
    }

    mysql_free_result(result);
    return false;
}
