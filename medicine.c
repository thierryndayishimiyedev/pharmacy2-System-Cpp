#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "medicine.h"
#include "supplier.h"

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

void showMedicineListShort(void)
{
    if(mysql_query(conn, "SELECT medicine_id, medicine_name, quantity, sell_price FROM medicines ORDER BY medicine_id") != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printf("\n--- Available Medicines ---\n");
    printf("ID | Name | Qty | Sell Price\n");

    bool found = false;
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%s | %s | %s | %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printf("(No medicines yet - add one first!)\n");
    printf("----------------------------\n");

    mysql_free_result(res);
}

void addMedicine(void)
{
    char name[128];
    char category[128];
    char expiry[32];
    int supplier;
    int quantity;
    float buyPrice;
    float sellPrice;
    char sql[2048];

    showSupplierListShort();

    clearInput();

    printf("Medicine Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL)
        return;
    stripNewline(name);

    printf("Category: ");
    if (fgets(category, sizeof(category), stdin) == NULL)
        return;
    stripNewline(category);

    printf("Supplier ID (from list above): ");
    if (scanf("%d", &supplier) != 1)
        return;
    clearInput();

    printf("Buying Price: ");
    if (scanf("%f", &buyPrice) != 1)
        return;
    clearInput();

    printf("Selling Price: ");
    if (scanf("%f", &sellPrice) != 1)
        return;
    clearInput();

    printf("Quantity: ");
    if (scanf("%d", &quantity) != 1)
        return;
    clearInput();

    printf("Expiry Date (YYYY-MM-DD): ");
    if (fgets(expiry, sizeof(expiry), stdin) == NULL)
        return;
    stripNewline(expiry);

    snprintf(sql, sizeof(sql),
        "INSERT INTO medicines(medicine_name,category,supplier_id,buy_price,sell_price,quantity,expiry_date) VALUES('%s','%s',%d,%.2f,%.2f,%d,'%s')",
        name, category, supplier, buyPrice, sellPrice, quantity, expiry);

    if(mysql_query(conn, sql) == 0)
        printf("\nMedicine Added Successfully! (ID: %llu)\n", mysql_insert_id(conn));
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void viewMedicines(void)
{
    if(mysql_query(conn, "SELECT * FROM medicines") != 0)
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
        printf("Category: %s\n", row[2]);
        printf("Supplier: %s\n", row[3]);
        printf("Buy: %s\n", row[4]);
        printf("Sell: %s\n", row[5]);
        printf("Qty: %s\n", row[6]);
        printf("Expiry: %s\n", row[7]);
        printf("------------------------\n");
    }

    mysql_free_result(res);
}

void searchMedicine(void)
{
    char keyword[128];
    char sql[2048];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    clearInput();

    printf("Enter medicine name or ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    stripNewline(keyword);

    snprintf(sql, sizeof(sql),
        "SELECT * FROM medicines WHERE medicine_name LIKE '%%%s%%' OR medicine_id='%s'",
        keyword, keyword);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("\nID: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Category: %s\n", row[2]);
        printf("Supplier: %s\n", row[3]);
        printf("Buy: %s\n", row[4]);
        printf("Sell: %s\n", row[5]);
        printf("Qty: %s\n", row[6]);
        printf("Expiry: %s\n", row[7]);
        printf("------------------------\n");
    }

    if(!found)
        printf("\nNo medicine found.\n");

    mysql_free_result(res);
}

void updateMedicine(void)
{
    int id;
    char field[128];
    char value[256];
    char sql[2048];

    showMedicineListShort();

    printf("Enter Medicine ID to update: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    printf("Which field? (medicine_name/category/supplier_id/buy_price/sell_price/quantity/expiry_date): ");
    if (fgets(field, sizeof(field), stdin) == NULL)
        return;
    stripNewline(field);

    printf("New value: ");
    if (fgets(value, sizeof(value), stdin) == NULL)
        return;
    stripNewline(value);

    if (strcmp(field, "supplier_id") == 0 || strcmp(field, "buy_price") == 0 ||
        strcmp(field, "sell_price") == 0 || strcmp(field, "quantity") == 0)
    {
        snprintf(sql, sizeof(sql), "UPDATE medicines SET %s=%s WHERE medicine_id=%d", field, value, id);
    }
    else
    {
        snprintf(sql, sizeof(sql), "UPDATE medicines SET %s='%s' WHERE medicine_id=%d", field, value, id);
    }

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printf("\nNo medicine found with that ID.\n");
        else
            printf("\nMedicine Updated Successfully!\n");
    }
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void deleteMedicine(void)
{
    int id;
    char sql[256];

    showMedicineListShort();

    printf("Enter Medicine ID to delete: ");
    if (scanf("%d", &id) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql), "DELETE FROM medicines WHERE medicine_id=%d", id);

    if(mysql_query(conn, sql) == 0)
    {
        if(mysql_affected_rows(conn) == 0)
            printf("\nNo medicine found with that ID.\n");
        else
            printf("\nMedicine Deleted Successfully!\n");
    }
    else
        printf("\nError: %s\n", mysql_error(conn));
}

void medicineMenu(void)
{
    int choice;

    do
    {
        printf("\n===== MEDICINES =====\n");
        printf("1. Add Medicine\n");
        printf("2. View Medicines\n");
        printf("3. Search Medicine\n");
        printf("4. Update Medicine\n");
        printf("5. Delete Medicine\n");
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
                addMedicine();
                break;
            case 2:
                viewMedicines();
                break;
            case 3:
                searchMedicine();
                break;
            case 4:
                updateMedicine();
                break;
            case 5:
                deleteMedicine();
                break;
        }

    } while(choice != 6);
}
