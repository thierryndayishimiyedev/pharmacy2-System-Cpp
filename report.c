#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "report.h"

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

void dailySalesReport(void)
{
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;

    snprintf(sql, sizeof(sql), "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales WHERE DATE(sale_date)=CURDATE()");

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    printf("\n===== TODAY'S SALES REPORT =====\n");
    printf("Number of Sales: %s\n", row[0]);
    printf("Total Revenue: %s\n", row[1]);

    mysql_free_result(res);

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
        "FROM sales s LEFT JOIN customers c ON s.customer_id=c.customer_id "
        "WHERE DATE(s.sale_date)=CURDATE() ORDER BY s.sale_date DESC");

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n--- Details ---\n");
    while((row = mysql_fetch_row(res)))
    {
        printf("Sale ID: %s | Customer: %s | Total: %s | Time: %s\n",
            row[0], row[1] ? row[1] : "Walk-in", row[2], row[3]);
    }

    mysql_free_result(res);
}

void totalSalesReport(void)
{
    char sql[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    snprintf(sql, sizeof(sql), "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales");

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    printf("\n===== ALL-TIME SALES REPORT =====\n");
    printf("Total Number of Sales: %s\n", row[0]);
    printf("Total Revenue: %s\n", row[1]);

    mysql_free_result(res);
}

void lowStockReport(void)
{
    int threshold;
    char sql[512];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("\nShow medicines with stock below: ");
    if (scanf("%d", &threshold) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql),
        "SELECT medicine_id, medicine_name, quantity FROM medicines WHERE quantity < %d ORDER BY quantity ASC",
        threshold);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n===== LOW STOCK REPORT (below %d) =====\n", threshold);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Qty Left: %s\n", row[0], row[1], row[2]);
    }

    if(!found)
        printf("No medicines below that stock level.\n");

    mysql_free_result(res);
}

void expiringSoonReport(void)
{
    int days;
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("\nShow medicines expiring within how many days: ");
    if (scanf("%d", &days) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql),
        "SELECT medicine_id, medicine_name, expiry_date, quantity FROM medicines "
        "WHERE expiry_date <= DATE_ADD(CURDATE(), INTERVAL %d DAY) "
        "ORDER BY expiry_date ASC",
        days);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n===== EXPIRING WITHIN %d DAYS =====\n", days);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Expiry: %s | Qty: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printf("No medicines expiring in that window.\n");

    mysql_free_result(res);
}

void topSellingMedicines(void)
{
    int limit;
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    printf("\nShow top how many medicines: ");
    if (scanf("%d", &limit) != 1)
        return;
    clearInput();

    snprintf(sql, sizeof(sql),
        "SELECT m.medicine_id, m.medicine_name, SUM(si.quantity) as total_sold, SUM(si.subtotal) as revenue "
        "FROM sale_items si JOIN medicines m ON si.medicine_id = m.medicine_id "
        "GROUP BY m.medicine_id, m.medicine_name "
        "ORDER BY total_sold DESC LIMIT %d",
        limit);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n===== TOP %d SELLING MEDICINES =====\n", limit);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Units Sold: %s | Revenue: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printf("No sales data yet.\n");

    mysql_free_result(res);
}

void salesByDate(void)
{
    char date[32];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;
    float total = 0.0f;

    clearInput();

    printf("\nEnter date (YYYY-MM-DD): ");
    if (fgets(date, sizeof(date), stdin) == NULL)
        return;
    stripNewline(date);

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
        "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
        "WHERE DATE(s.sale_date)='%s' ORDER BY s.sale_date ASC",
        date);

    if(mysql_query(conn, sql) != 0)
    {
        printf("\nError: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);

    printf("\n===== SALES ON %s =====\n", date);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("Sale ID: %s | Customer: %s | Total: %s | Time: %s\n",
            row[0], row[1] ? row[1] : "Walk-in", row[2], row[3]);
        total += atof(row[2]);
    }

    if(!found)
        printf("No sales found on that date.\n");
    else
        printf("\nDay Total: %.2f\n", total);

    mysql_free_result(res);
}

void reportMenu(void)
{
    int choice;

    do
    {
        printf("\n===== REPORTS =====\n");
        printf("1. Today's Sales Report\n");
        printf("2. All-Time Sales Report\n");
        printf("3. Low Stock Report\n");
        printf("4. Expiring Soon Report\n");
        printf("5. Top Selling Medicines\n");
        printf("6. Sales By Specific Date\n");
        printf("7. Back\n");
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
                dailySalesReport();
                break;
            case 2:
                totalSalesReport();
                break;
            case 3:
                lowStockReport();
                break;
            case 4:
                expiringSoonReport();
                break;
            case 5:
                topSellingMedicines();
                break;
            case 6:
                salesByDate();
                break;
        }

    } while(choice != 7);
}
