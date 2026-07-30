#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "login.h"
#include "sale.h"
#include "customer.h"
#include "medicine.h"

#define MAX_CART_ITEMS 100

struct CartItem
{
    int medicineId;
    int quantity;
    float price;
    float subtotal;
};

static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void sellMedicine(void)
{
    int customerId;
    struct CartItem cart[MAX_CART_ITEMS];
    int cartCount = 0;
    float grandTotal = 0.0f;
    char sql[2048];
    char customerPart[32];
    char more = 'y';
    char confirm;
    MYSQL_RES *res;
    MYSQL_ROW row;

    showCustomerListShort();

    printf("\nEnter Customer ID from list above (0 for walk-in customer): ");
    if (scanf("%d", &customerId) != 1)
        return;
    clearInput();

    if(customerId != 0)
    {
        snprintf(sql, sizeof(sql), "SELECT customer_id FROM customers WHERE customer_id=%d", customerId);
        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError: %s\n", mysql_error(conn));
            return;
        }

        res = mysql_store_result(conn);
        if(mysql_num_rows(res) == 0)
        {
            printf("\nNo customer found with that ID. Cancelling sale.\n");
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);
    }

    while(more == 'y' || more == 'Y')
    {
        int medicineId;
        int qty;

        showMedicineListShort();
        printf("\nEnter Medicine ID from list above: ");
        if (scanf("%d", &medicineId) != 1)
            return;
        clearInput();

        snprintf(sql, sizeof(sql), "SELECT sell_price, quantity FROM medicines WHERE medicine_id=%d", medicineId);
        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError: %s\n", mysql_error(conn));
            continue;
        }

        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
        {
            printf("\nNo medicine found with that ID.\n");
            mysql_free_result(res);
            printf("\nAdd another item? (y/n): ");
            if (scanf(" %c", &more) != 1)
                return;
            clearInput();
            continue;
        }

        float sellPrice = atof(row[0]);
        int availableQty = atoi(row[1]);
        mysql_free_result(res);

        printf("Available Stock: %d\n", availableQty);
        printf("Quantity to sell: ");
        if (scanf("%d", &qty) != 1)
            return;
        clearInput();

        if(qty > availableQty)
        {
            printf("\nNot enough stock! Only %d available.\n", availableQty);
            printf("\nAdd another item? (y/n): ");
            if (scanf(" %c", &more) != 1)
                return;
            clearInput();
            continue;
        }

        if(qty <= 0)
        {
            printf("\nQuantity must be greater than 0.\n");
            printf("\nAdd another item? (y/n): ");
            if (scanf(" %c", &more) != 1)
                return;
            clearInput();
            continue;
        }

        if(cartCount >= MAX_CART_ITEMS)
        {
            printf("\nCart is full.\n");
            break;
        }

        cart[cartCount].medicineId = medicineId;
        cart[cartCount].quantity = qty;
        cart[cartCount].price = sellPrice;
        cart[cartCount].subtotal = qty * sellPrice;
        cartCount++;
        grandTotal += qty * sellPrice;

        printf("Added: Medicine ID %d x%d = %.2f\n", medicineId, qty, qty * sellPrice);
        printf("\nAdd another item? (y/n): ");
        if (scanf(" %c", &more) != 1)
            return;
        clearInput();
    }

    if(cartCount == 0)
    {
        printf("\nNo items in cart. Sale cancelled.\n");
        return;
    }

    printf("\n===== SALE SUMMARY =====\n");
    printf("Grand Total: %.2f\n", grandTotal);
    printf("Confirm sale? (y/n): ");
    if (scanf(" %c", &confirm) != 1)
        return;
    clearInput();

    if(confirm != 'y' && confirm != 'Y')
    {
        printf("\nSale cancelled.\n");
        return;
    }

    mysql_query(conn, "START TRANSACTION");

    if(customerId == 0)
        snprintf(customerPart, sizeof(customerPart), "NULL");
    else
        snprintf(customerPart, sizeof(customerPart), "%d", customerId);

    snprintf(sql, sizeof(sql),
        "INSERT INTO sales(customer_id,user_id,sale_date,grand_total) VALUES(%s,%d,NOW(),%.2f)",
        customerPart, currentUserId, grandTotal);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError creating sale: %s\n", mysql_error(conn));
        mysql_query(conn, "ROLLBACK");
        return;
    }

    unsigned long long saleId = mysql_insert_id(conn);
    bool success = true;
    int i;

    for(i = 0; i < cartCount; i++)
    {
        snprintf(sql, sizeof(sql),
            "INSERT INTO sale_items(sale_id,medicine_id,quantity,price,subtotal) VALUES(%llu,%d,%d,%.2f,%.2f)",
            saleId, cart[i].medicineId, cart[i].quantity, cart[i].price, cart[i].subtotal);

        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError adding sale item: %s\n", mysql_error(conn));
            success = false;
            break;
        }

        snprintf(sql, sizeof(sql),
            "UPDATE medicines SET quantity = quantity - %d WHERE medicine_id=%d",
            cart[i].quantity, cart[i].medicineId);

        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError updating stock: %s\n", mysql_error(conn));
            success = false;
            break;
        }
    }

    if(success)
    {
        mysql_query(conn, "COMMIT");
        printf("\nSale Completed Successfully! (Sale ID: %llu)\n", saleId);
    }
    else
    {
        mysql_query(conn, "ROLLBACK");
        printf("\nSale rolled back due to error. No changes were made.\n");
    }
}

void viewSales(void)
{
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
        "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
        "ORDER BY s.sale_id DESC");

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n==============================\n");

    while((row = mysql_fetch_row(res)))
    {
        printf("Sale ID: %s\n", row[0]);
        printf("Customer: %s\n", row[2] ? row[2] : "Walk-in");
        printf("Served by User ID: %s\n", row[3]);
        printf("Date: %s\n", row[4]);
        printf("Grand Total: %s\n", row[5]);
        printf("------------------------\n");
    }

    mysql_free_result(res);
}

void searchSales(void)
{
    char keyword[128];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    clearInput();

    printf("Enter Sale ID or Customer ID to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL)
        return;
    keyword[strcspn(keyword, "\r\n")] = '\0';

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
        "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
        "WHERE s.sale_id='%s' OR s.customer_id='%s'",
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
        printf("\nSale ID: %s\n", row[0]);
        printf("Customer: %s\n", row[2] ? row[2] : "Walk-in");
        printf("Served by User ID: %s\n", row[3]);
        printf("Date: %s\n", row[4]);
        printf("Grand Total: %s\n", row[5]);
        printf("------------------------\n");
    }

    if(!found)
        printf("No sale found.\n");

    mysql_free_result(res);
}

void deleteSale(void)
{
    int saleId;
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool success = true;

    printf("Enter Sale ID to delete (this restores stock and cannot be undone): ");
    if (scanf("%d", &saleId) != 1)
        return;
    clearInput();

    mysql_query(conn, "START TRANSACTION");

    snprintf(sql, sizeof(sql), "SELECT medicine_id, quantity FROM sale_items WHERE sale_id=%d", saleId);
    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        mysql_query(conn, "ROLLBACK");
        return;
    }

    res = mysql_store_result(conn);
    if(mysql_num_rows(res) == 0)
    {
        printf("\nNo sale found with that ID.\n");
        mysql_free_result(res);
        mysql_query(conn, "ROLLBACK");
        return;
    }

    while((row = mysql_fetch_row(res)))
    {
        snprintf(sql, sizeof(sql),
            "UPDATE medicines SET quantity = quantity + %s WHERE medicine_id=%s",
            row[1], row[0]);

        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError restoring stock: %s\n", mysql_error(conn));
            success = false;
            break;
        }
    }
    mysql_free_result(res);

    if(success)
    {
        snprintf(sql, sizeof(sql), "DELETE FROM sale_items WHERE sale_id=%d", saleId);
        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError deleting sale items: %s\n", mysql_error(conn));
            success = false;
        }
    }

    if(success)
    {
        snprintf(sql, sizeof(sql), "DELETE FROM sales WHERE sale_id=%d", saleId);
        if(mysql_query(conn, sql) != 0)
        {
            printf("\nError deleting sale: %s\n", mysql_error(conn));
            success = false;
        }
    }

    if(success)
    {
        mysql_query(conn, "COMMIT");
        printf("\nSale Deleted Successfully! Stock has been restored.\n");
    }
    else
    {
        mysql_query(conn, "ROLLBACK");
        printf("\nDelete rolled back due to error. No changes were made.\n");
    }
}

void saleMenu(void)
{
    int choice;

    do
    {
        printf("\n===== SELL MEDICINE =====\n");
        printf("1. New Sale\n");
        printf("2. View Sales\n");
        printf("3. Search Sales\n");
        printf("4. Delete Sale\n");
        printf("5. Back\n");
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
                sellMedicine();
                break;
            case 2:
                viewSales();
                break;
            case 3:
                searchSales();
                break;
            case 4:
                deleteSale();
                break;
        }

    } while(choice != 5);
}
