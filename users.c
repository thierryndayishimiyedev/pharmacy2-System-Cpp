#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "users.h"

static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void stripNewline(char *str)
{
    size_t len = strlen(str);
    if (len == 0)
        return;
    if (str[len - 1] == '\n' || str[len - 1] == '\r')
        str[len - 1] = '\0';
}

void showUserListShort(void)
{
    if(mysql_query(conn, "SELECT user_id, full_name, username, role FROM users ORDER BY user_id") != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printf("\n--- Available Users ---\n");
    printf("ID | Name | Username | Role\n");

    bool found = false;
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%s | %s | %s | %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printf("(No users yet)\n");
    printf("----------------------------\n");

    mysql_free_result(res);
}

void addUser(void)
{
    char name[128];
    char username[128];
    char password[128];
    char phone[64];
    char role[64];
    char sql[1536];

    clearInput();

    printf("Full Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL)
        return;
    stripNewline(name);

    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL)
        return;
    stripNewline(username);

    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
        return;
    stripNewline(password);

    printf("Phone: ");
    if (fgets(phone, sizeof(phone), stdin) == NULL)
        return;
    stripNewline(phone);

    printf("Role: ");
    if (fgets(role, sizeof(role), stdin) == NULL)
        return;
    stripNewline(role);

    snprintf(sql, sizeof(sql),
        "INSERT INTO users(full_name,username,password,phone,role) VALUES('%s','%s','%s','%s','%s')",
        name, username, password, phone, role);

    if(mysql_query(conn, sql) == 0)
        printf("\nUser Added Successfully! (ID: %llu)\n", mysql_insert_id(conn));
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void viewUsers(void)
{
    if(mysql_query(conn, "SELECT user_id,full_name,username,phone,role FROM users") != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printf("\n==============================\n");

    while((row = mysql_fetch_row(res)))
    {
        printf("ID: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Username: %s\n", row[2]);
        printf("Phone: %s\n", row[3]);
        printf("Role: %s\n", row[4]);
        printf("------------------------\n");
    }

    mysql_free_result(res);
}

void searchUser(void)
{
    char keyword[128];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    clearInput();

    printf("Enter name, username or ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    stripNewline(keyword);

    snprintf(sql, sizeof(sql),
        "SELECT user_id,full_name,username,phone,role FROM users WHERE full_name LIKE '%%%s%%' OR username LIKE '%%%s%%' OR user_id='%s'",
        keyword, keyword, keyword);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("\nID: %s | Name: %s | Username: %s | Role: %s\n", row[0], row[1], row[2], row[4]);
    }

    if(!found)
        printf("No user found.\n");

    mysql_free_result(res);
}

void updateUser(void)
{
    int id;
    char field[128];
    char value[256];
    char sql[1024];

    showUserListShort();

    printf("Enter User ID to update: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    printf("Which field? (full_name/username/password/phone/role): ");
    if (fgets(field, sizeof(field), stdin) == NULL)
        return;
    stripNewline(field);

    printf("New value: ");
    if (fgets(value, sizeof(value), stdin) == NULL)
        return;
    stripNewline(value);

    snprintf(sql, sizeof(sql), "UPDATE users SET %s='%s' WHERE user_id=%d", field, value, id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printf("\nNo user found with that ID.\n");
        else
            printf("\nUser Updated Successfully!\n");
    }
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void deleteUser(void)
{
    int id;
    char sql[256];

    showUserListShort();

    printf("Enter User ID to delete: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql), "DELETE FROM users WHERE user_id=%d", id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printf("\nNo user found with that ID.\n");
        else
            printf("\nUser Deleted Successfully!\n");
    }
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void usersMenu(void)
{
    int choice;

    do
    {
        printf("\n===== MANAGE USERS =====\n");
        printf("1. Add User\n");
        printf("2. View Users\n");
        printf("3. Search User\n");
        printf("4. Update User\n");
        printf("5. Delete User\n");
        printf("6. Back\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clearInput();
            choice = 0;
        }
        clearInput();

        switch(choice)
        {
            case 1:
                addUser();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                searchUser();
                break;
            case 4:
                updateUser();
                break;
            case 5:
                deleteUser();
                break;
        }

    } while(choice != 6);
}
