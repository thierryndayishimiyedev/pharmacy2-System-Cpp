#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "medicine.h"
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

    if(!isValidBusinessName(name) || !isValidBusinessName(category))
    {
        printErrorFmt("Medicine name and category must be 2-100 valid characters.");
        pauseForUser();
        return;
    }
    if(supplier <= 0 || !isPositiveAmount(buyPrice) || !isPositiveAmount(sellPrice) || !isNonNegativeNumber(quantity))
    {
        printErrorFmt("Supplier ID must be valid; prices must be above zero; quantity cannot be negative.");
        pauseForUser();
        return;
    }
    if(buyPrice > sellPrice)
    {
        printErrorFmt("Medicine cannot be sold below its buying price. Set a selling price equal to or above %.2f.", buyPrice);
        pauseForUser();
        return;
    }
    if(!isValidDate(expiry))
    {
        printErrorFmt("Expiry date must use a real YYYY-MM-DD date.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "INSERT INTO medicines(medicine_name,category,supplier_id,buy_price,sell_price,quantity,expiry_date) VALUES('%s','%s',%d,%.2f,%.2f,%d,'%s')",
        name, category, supplier, buyPrice, sellPrice, quantity, expiry);

    if(mysql_query(conn, sql) == 0)
        printSuccessFmt("Medicine added successfully! ID: %llu", mysql_insert_id(conn));
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void viewMedicines(void)
{
    if(mysql_query(conn, "SELECT m.*, s.company_name FROM medicines m LEFT JOIN suppliers s ON m.supplier_id=s.supplier_id ORDER BY m.medicine_name") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    printHeader("MEDICINE LIST");
    int medicineCount = 0;
    int totalUnits = 0;
    double costValue = 0.0, sellingValue = 0.0;

    while((row = mysql_fetch_row(res)))
    {
        printf("ID: %s\n", row[0]);
        printf("Name: %s\n", row[1]);
        printf("Category: %s\n", row[2]);
        printf("Supplier: %s\n", row[9] ? row[9] : "Not assigned");
        printf("Buy: %s\n", row[4]);
        printf("Sell: %s\n", row[5]);
        printf("Qty: %s\n", row[6]);
        printf("Expiry: %s\n", row[7]);
        printf("Stock Cost Value: %.2f | Potential Sales Value: %.2f\n",
            atof(row[4]) * atoi(row[6]), atof(row[5]) * atoi(row[6]));
        printf("------------------------\n");
        medicineCount++;
        totalUnits += atoi(row[6]);
        costValue += atof(row[4]) * atoi(row[6]);
        sellingValue += atof(row[5]) * atoi(row[6]);
    }

    printf("Total medicines: %d | Units in stock: %d\n", medicineCount, totalUnits);
    printf("Stock cost total: %.2f | Potential sales total: %.2f | Potential margin: %.2f\n",
        costValue, sellingValue, sellingValue - costValue);

    mysql_free_result(res);
}

void searchMedicine(void)
{
    char keyword[128];
    char sql[2048];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("Enter medicine name or ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    stripNewline(keyword);

    snprintf(sql, sizeof(sql),
        "SELECT * FROM medicines WHERE medicine_name LIKE '%%%s%%' OR medicine_id='%s'",
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
        printInfoFmt("No medicine found.");

    mysql_free_result(res);
    pauseForUser();
}

void updateMedicine(void)
{
    int id;
    char field[128];
    char value[256];
    char sql[2048];
    float numericValue;
    int quantityValue;

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

    if(id <= 0 || (strcmp(field, "medicine_name") != 0 && strcmp(field, "category") != 0 &&
        strcmp(field, "supplier_id") != 0 && strcmp(field, "buy_price") != 0 &&
        strcmp(field, "sell_price") != 0 && strcmp(field, "quantity") != 0 && strcmp(field, "expiry_date") != 0))
    {
        printErrorFmt("Enter a positive ID and choose a field shown in the prompt.");
        pauseForUser(); return;
    }
    if((strcmp(field, "medicine_name") == 0 || strcmp(field, "category") == 0) && !isValidBusinessName(value))
    {
        printErrorFmt("Name and category must use valid text.");
        pauseForUser(); return;
    }
    if(strcmp(field, "expiry_date") == 0 && !isValidDate(value))
    {
        printErrorFmt("Expiry date must use a real YYYY-MM-DD date.");
        pauseForUser(); return;
    }
    if(strcmp(field, "supplier_id") == 0 && (sscanf(value, "%d", &quantityValue) != 1 || quantityValue <= 0))
    {
        printErrorFmt("Supplier ID must be a positive number.");
        pauseForUser(); return;
    }
    if((strcmp(field, "buy_price") == 0 || strcmp(field, "sell_price") == 0) &&
        (sscanf(value, "%f", &numericValue) != 1 || !isPositiveAmount(numericValue)))
    {
        printErrorFmt("Price must be greater than zero.");
        pauseForUser(); return;
    }
    if(strcmp(field, "quantity") == 0 && (sscanf(value, "%d", &quantityValue) != 1 || !isNonNegativeNumber(quantityValue)))
    {
        printErrorFmt("Quantity must be zero or greater.");
        pauseForUser(); return;
    }

    if(strcmp(field, "buy_price") == 0)
        snprintf(sql, sizeof(sql), "UPDATE medicines SET buy_price=%.2f WHERE medicine_id=%d AND %.2f <= sell_price", numericValue, id, numericValue);
    else if(strcmp(field, "sell_price") == 0)
        snprintf(sql, sizeof(sql), "UPDATE medicines SET sell_price=%.2f WHERE medicine_id=%d AND %.2f >= buy_price", numericValue, id, numericValue);
    else if (strcmp(field, "supplier_id") == 0 || strcmp(field, "quantity") == 0)
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
            printInfoFmt("No medicine changed. Check the ID and ensure selling price is not below buying price.");
        else
            printSuccessFmt("Medicine updated successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
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
            printInfoFmt("No medicine found with that ID.");
        else
            printSuccessFmt("Medicine deleted successfully.");
    }
    else
        printErrorFmt("%s", mysql_error(conn));

    pauseForUser();
}

void medicineMenu(void)
{
    int choice;

    do
    {
        printHeader("MANAGE MEDICINES");
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
