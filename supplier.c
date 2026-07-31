#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "supplier.h"
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

void showSupplierListShort(void)
{
    if(mysql_query(conn, "SELECT supplier_id, company_name FROM suppliers ORDER BY supplier_id") != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printf("\n--- Available Suppliers ---\n");
    printf("ID | Company Name\n");

    bool found = false;
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%s | %s\n", row[0], row[1]);
    }

    if(!found)
        printf("(No suppliers yet - add one first!)\n");
    printf("----------------------------\n");

    mysql_free_result(res);
}

void addSupplier(void)
{
    char company[128];
    char contact[128];
    char phone[64];
    char email[128];
    char address[256];
    char sql[1536];

    printf("Company Name: ");
    if (fgets(company, sizeof(company), stdin) == NULL)
        return;
    stripNewline(company);

    printf("Contact Person: ");
    if (fgets(contact, sizeof(contact), stdin) == NULL)
        return;
    stripNewline(contact);

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

    if(!isValidBusinessName(company) || !isValidPersonName(contact) || !isValidPhone(phone) || !isValidEmail(email) || !isValidText(address, 250))
    {
        printErrorFmt("Enter a valid company, contact name, phone (at least 7 digits), email, and address.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "INSERT INTO suppliers(company_name,contact_person,phone,email,address) VALUES('%s','%s','%s','%s','%s')",
        company, contact, phone, email, address);

    if(mysql_query(conn, sql) == 0)
        printSuccessFmt("Supplier added successfully! ID: %llu", mysql_insert_id(conn));
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void viewSuppliers(void)
{
    if(mysql_query(conn,
        "SELECT s.supplier_id,s.company_name,s.contact_person,s.phone,s.email,s.address,COUNT(m.medicine_id),COALESCE(SUM(m.quantity),0),COALESCE(SUM(m.quantity*m.buy_price),0) "
        "FROM suppliers s LEFT JOIN medicines m ON s.supplier_id=m.supplier_id "
        "GROUP BY s.supplier_id,s.company_name,s.contact_person,s.phone,s.email,s.address ORDER BY s.company_name") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printHeader("SUPPLIER LIST");
    while((row = mysql_fetch_row(res)))
    {
        printf("ID: %s\n", row[0]);
        printf("Company: %s\n", row[1]);
        printf("Contact: %s\n", row[2]);
        printf("Phone: %s\n", row[3]);
        printf("Email: %s\n", row[4]);
        printf("Address: %s\n", row[5]);
        printf("Medicine Types: %s | Units Supplied In Stock: %s | Stock Cost Value: %s\n", row[6], row[7], row[8]);
        printf("------------------------\n");
    }
    mysql_free_result(res);
}

void searchSupplier(void)
{
    char keyword[128];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("Enter company name or ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    stripNewline(keyword);

    snprintf(sql, sizeof(sql),
        "SELECT * FROM suppliers WHERE company_name LIKE '%%%s%%' OR supplier_id='%s'",
        keyword, keyword);

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
        printf("\nID: %s | Company: %s | Contact: %s | Phone: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printInfoFmt("No supplier found.");

    mysql_free_result(res);
    pauseForUser();
}

void updateSupplier(void)
{
    int id;
    char field[128];
    char value[256];
    char sql[1024];

    showSupplierListShort();

    printf("Enter Supplier ID to update: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    printf("Which field? (company_name/contact_person/phone/email/address): ");
    if (fgets(field, sizeof(field), stdin) == NULL)
        return;
    stripNewline(field);

    printf("New value: ");
    if (fgets(value, sizeof(value), stdin) == NULL)
        return;
    stripNewline(value);

    if((strcmp(field, "company_name") == 0 && !isValidBusinessName(value)) ||
       (strcmp(field, "contact_person") == 0 && !isValidPersonName(value)) ||
       (strcmp(field, "phone") == 0 && !isValidPhone(value)) ||
       (strcmp(field, "email") == 0 && !isValidEmail(value)) ||
       (strcmp(field, "address") == 0 && !isValidText(value, 250)))
    {
        printErrorFmt("The new value is not valid for that field.");
        pauseForUser(); return;
    }
    if(strcmp(field, "company_name") != 0 && strcmp(field, "contact_person") != 0 && strcmp(field, "phone") != 0 && strcmp(field, "email") != 0 && strcmp(field, "address") != 0)
    {
        printErrorFmt("Choose a field shown in the prompt.");
        pauseForUser(); return;
    }

    snprintf(sql, sizeof(sql), "UPDATE suppliers SET %s='%s' WHERE supplier_id=%d", field, value, id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printInfoFmt("No supplier found with that ID.");
        else
            printSuccessFmt("Supplier updated successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void deleteSupplier(void)
{
    int id;
    char sql[256];

    showSupplierListShort();

    printf("Enter Supplier ID to delete: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql), "DELETE FROM suppliers WHERE supplier_id=%d", id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printInfoFmt("No supplier found with that ID.");
        else
            printSuccessFmt("Supplier deleted successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void supplierMenu(void)
{
    int choice;

    do
    {
        printHeader("MANAGE SUPPLIERS");
        printf("1. Add Supplier\n");
        printf("2. View Suppliers\n");
        printf("3. Search Supplier\n");
        printf("4. Update Supplier\n");
        printf("5. Delete Supplier\n");
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
                addSupplier();
                break;
            case 2:
                viewSuppliers();
                break;
            case 3:
                searchSupplier();
                break;
            case 4:
                updateSupplier();
                break;
            case 5:
                deleteSupplier();
                break;
        }

    } while(choice != 6);
}
