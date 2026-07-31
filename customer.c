#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "customer.h"
#include "ui.h"

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

void showCustomerListShort(void)
{
    if(mysql_query(conn, "SELECT customer_id, full_name, phone FROM customers ORDER BY customer_id") != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printf("\n--- Available Customers ---\n");
    printf("ID | Name | Phone\n");

    bool found = false;
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%s | %s | %s\n", row[0], row[1], row[2]);
    }

    if(!found)
        printf("(No customers yet)\n");
    printf("----------------------------\n");

    mysql_free_result(res);
}

void addCustomer(void)
{
    char name[128];
    char phone[64];
    char email[128];
    char address[256];
    char sql[1024];

    printf("Full Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL)
        return;
    stripNewline(name);

    printf("Phone: ");
    if (fgets(phone, sizeof(phone), stdin) == NULL)
        return;
    stripNewline(phone);

    printf("Email: ");
    if (fgets(email, sizeof(email), stdin) == NULL)
        return;
    stripNewline(email);

    printf("Address: ");
    if (fgets(address, sizeof(address), stdin) == NULL)
        return;
    stripNewline(address);

    if(!isValidPersonName(name) || !isValidPhone(phone) || !isValidEmail(email) || !isValidText(address, 250))
    {
        printErrorFmt("Enter a valid name, phone (at least 7 digits), email, and address. Quotes are not allowed.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "INSERT INTO customers(full_name,phone,email,address) VALUES('%s','%s','%s','%s')",
        name, phone, email, address);

    if(mysql_query(conn, sql) == 0)
        printSuccessFmt("Customer added successfully! ID: %llu", mysql_insert_id(conn));
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void viewCustomers(void)
{
    if(mysql_query(conn,
        "SELECT c.customer_id,c.full_name,c.phone,c.email,c.address,COUNT(s.sale_id),COALESCE(SUM(s.grand_total),0) "
        "FROM customers c LEFT JOIN sales s ON c.customer_id=s.customer_id "
        "GROUP BY c.customer_id,c.full_name,c.phone,c.email,c.address ORDER BY c.full_name") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printHeader("CUSTOMER LIST");
    while((row = mysql_fetch_row(res)))
    {
        printf("ID: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Phone: %s\n", row[2]);
        printf("Email: %s\n", row[3]);
        printf("Address: %s\n", row[4]);
        printf("Purchases: %s | Total Spent: %s\n", row[5], row[6]);
        printf("------------------------\n");
    }
    mysql_free_result(res);
}

void searchCustomer(void)
{
    char keyword[128];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("Enter name, phone or ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    stripNewline(keyword);

    snprintf(sql, sizeof(sql),
        "SELECT * FROM customers WHERE full_name LIKE '%%%s%%' OR phone LIKE '%%%s%%' OR customer_id='%s'",
        keyword, keyword, keyword);

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("\nID: %s | Name: %s | Phone: %s | Email: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printInfoFmt("No customer found.");

    mysql_free_result(res);
    pauseForUser();
}

void updateCustomer(void)
{
    int id;
    char field[128];
    char value[256];
    char sql[1024];

    showCustomerListShort();

    printf("Enter Customer ID to update: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    printf("Which field? (full_name/phone/email/address): ");
    if (fgets(field, sizeof(field), stdin) == NULL)
        return;
    stripNewline(field);

    printf("New value: ");
    if (fgets(value, sizeof(value), stdin) == NULL)
        return;
    stripNewline(value);

    if(strcmp(field, "full_name") == 0 && !isValidPersonName(value))
    {
        printErrorFmt("Full name must contain valid letters and be at least 2 characters.");
        pauseForUser(); return;
    }
    if(strcmp(field, "phone") == 0 && !isValidPhone(value))
    {
        printErrorFmt("Phone must contain at least 7 digits.");
        pauseForUser(); return;
    }
    if(strcmp(field, "email") == 0 && !isValidEmail(value))
    {
        printErrorFmt("Enter a valid email address.");
        pauseForUser(); return;
    }
    if(strcmp(field, "address") == 0 && !isValidText(value, 250))
    {
        printErrorFmt("Enter a valid address.");
        pauseForUser(); return;
    }
    if(strcmp(field, "full_name") != 0 && strcmp(field, "phone") != 0 && strcmp(field, "email") != 0 && strcmp(field, "address") != 0)
    {
        printErrorFmt("Choose one of: full_name, phone, email, address.");
        pauseForUser(); return;
    }

    snprintf(sql, sizeof(sql), "UPDATE customers SET %s='%s' WHERE customer_id=%d", field, value, id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printInfoFmt("No customer found with that ID.");
        else
            printSuccessFmt("Customer updated successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void deleteCustomer(void)
{
    int id;
    char sql[256];

    showCustomerListShort();

    printf("Enter Customer ID to delete: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql), "DELETE FROM customers WHERE customer_id=%d", id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printInfoFmt("No customer found with that ID.");
        else
            printSuccessFmt("Customer deleted successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void customerMenu(void)
{
    int choice;

    do
    {
        printHeader("MANAGE CUSTOMERS");
        printf("1. Add Customer\n");
        printf("2. View Customers\n");
        printf("3. Search Customer\n");
        printf("4. Update Customer\n");
        printf("5. Delete Customer\n");
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
                addCustomer();
                break;
            case 2:
                viewCustomers();
                break;
            case 3:
                searchCustomer();
                break;
            case 4:
                updateCustomer();
                break;
            case 5:
                deleteCustomer();
                break;
        }

    } while(choice != 6);
}
