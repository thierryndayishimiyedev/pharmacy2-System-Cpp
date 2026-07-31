#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql.h>
#include "database.h"
#include "report.h"
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

void dailySalesReport(void)
{
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;

    snprintf(sql, sizeof(sql), "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales WHERE DATE(sale_date)=CURDATE()");

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    printHeader("TODAY'S SALES REPORT");
    printf("Number of Sales: %s\n", row[0]);
    printf("Total Revenue: %s\n", row[1]);

    mysql_free_result(res);

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
        "FROM sales s LEFT JOIN customers c ON s.customer_id=c.customer_id "
        "WHERE DATE(s.sale_date)=CURDATE() ORDER BY s.sale_date DESC");

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    printHeader("TODAY'S SALES DETAILS");
    while((row = mysql_fetch_row(res)))
    {
        printf("Sale ID: %s | Customer: %s | Total: %s | Time: %s\n",
            row[0], row[1] ? row[1] : "Walk-in", row[2], row[3]);
    }

    mysql_free_result(res);
    pauseForUser();
}

void totalSalesReport(void)
{
    char sql[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    snprintf(sql, sizeof(sql), "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales");

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    printHeader("ALL-TIME SALES REPORT");
    printf("Total Number of Sales: %s\n", row[0]);
    printf("Total Revenue: %s\n", row[1]);

    mysql_free_result(res);
    pauseForUser();
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

    if(threshold < 0)
    {
        printErrorFmt("Stock threshold cannot be negative.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT medicine_id, medicine_name, quantity FROM medicines WHERE quantity < %d ORDER BY quantity ASC",
        threshold);

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    printHeader("LOW STOCK REPORT");
    printf("Threshold: %d\n", threshold);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Qty Left: %s\n", row[0], row[1], row[2]);
    }

    if(!found)
        printInfoFmt("No medicines below that stock level.");

    mysql_free_result(res);
    pauseForUser();
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

    if(days < 0)
    {
        printErrorFmt("Number of days cannot be negative.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT medicine_id, medicine_name, expiry_date, quantity FROM medicines "
        "WHERE expiry_date <= DATE_ADD(CURDATE(), INTERVAL %d DAY) "
        "ORDER BY expiry_date ASC",
        days);

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    printHeader("EXPIRING SOON REPORT");
    printf("Window: %d days\n", days);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Expiry: %s | Qty: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printInfoFmt("No medicines expiring in that window.");

    mysql_free_result(res);
    pauseForUser();
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

    if(limit <= 0 || limit > 100)
    {
        printErrorFmt("Enter a limit from 1 to 100.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT m.medicine_id, m.medicine_name, SUM(si.quantity) as total_sold, SUM(si.subtotal) as revenue "
        "FROM sale_items si JOIN medicines m ON si.medicine_id = m.medicine_id "
        "GROUP BY m.medicine_id, m.medicine_name "
        "ORDER BY total_sold DESC LIMIT %d",
        limit);

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    printHeader("TOP SELLING MEDICINES");
    printf("Limit: %d\n", limit);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | Name: %s | Units Sold: %s | Revenue: %s\n", row[0], row[1], row[2], row[3]);
    }

    if(!found)
        printInfoFmt("No sales data yet.");

    mysql_free_result(res);
    pauseForUser();
}

void salesByDate(void)
{
    char date[32];
    char sql[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;
    float total = 0.0f;

    printf("\nEnter date (YYYY-MM-DD): ");
    if (fgets(date, sizeof(date), stdin) == NULL)
        return;
    stripNewline(date);

    if(!isValidDate(date))
    {
        printErrorFmt("Enter a real date in YYYY-MM-DD format.");
        pauseForUser();
        return;
    }

    snprintf(sql, sizeof(sql),
        "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
        "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
        "WHERE DATE(s.sale_date)='%s' ORDER BY s.sale_date ASC",
        date);

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }

    res = mysql_store_result(conn);

    printHeader("SALES BY DATE");
    printf("Date: %s\n", date);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("Sale ID: %s | Customer: %s | Total: %s | Time: %s\n",
            row[0], row[1] ? row[1] : "Walk-in", row[2], row[3]);
        total += atof(row[2]);
    }

    if(!found)
        printInfoFmt("No sales found on that date.");
    else
        printf("\nDay Total: %.2f\n", total);

    mysql_free_result(res);
    pauseForUser();
}

void expiredMedicinesReport(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    if(mysql_query(conn,
        "SELECT medicine_id, medicine_name, expiry_date, quantity, quantity * buy_price "
        "FROM medicines WHERE expiry_date < CURDATE() ORDER BY expiry_date ASC") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }
    res = mysql_store_result(conn);
    printHeader("EXPIRED MEDICINES REPORT");
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("ID: %s | %s | Expired: %s | Qty: %s | Cost at risk: %s\n",
            row[0], row[1], row[2], row[3], row[4]);
    }
    if(!found)
        printInfoFmt("No expired medicines found.");
    mysql_free_result(res);
    pauseForUser();
}

void stockValueReport(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;

    if(mysql_query(conn,
        "SELECT COUNT(*),COALESCE(SUM(quantity),0),COALESCE(SUM(quantity*buy_price),0),"
        "COALESCE(SUM(quantity*sell_price),0) FROM medicines") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    printHeader("INVENTORY VALUE REPORT");
    printf("Medicine types: %s\nUnits in stock: %s\n", row[0], row[1]);
    printf("Stock cost total: %s\nPotential sales total: %s\nPotential gross margin: %.2f\n",
        row[2], row[3], atof(row[3]) - atof(row[2]));
    mysql_free_result(res);
    pauseForUser();
}

void monthlySalesReport(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    if(mysql_query(conn,
        "SELECT DATE_FORMAT(sale_date, '%Y-%m') AS month,COUNT(*),COALESCE(SUM(grand_total),0),"
        "COALESCE(AVG(grand_total),0) FROM sales GROUP BY DATE_FORMAT(sale_date, '%Y-%m') ORDER BY month DESC") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }
    res = mysql_store_result(conn);
    printHeader("MONTHLY SALES SUMMARY");
    printf("Month     Sales    Revenue       Average Sale\n");
    printf("------------------------------------------------\n");
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%-9s %-8s %-13s %s\n", row[0], row[1], row[2], row[3]);
    }
    if(!found)
        printInfoFmt("No sales data yet.");
    mysql_free_result(res);
    pauseForUser();
}

static void printAdviceQuery(const char *title, const char *sql, const char *emptyMessage)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    bool found = false;

    if(mysql_query(conn, sql) != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        return;
    }
    res = mysql_store_result(conn);
    printHeader(title);
    while((row = mysql_fetch_row(res)))
    {
        found = true;
        printf("%s\n", row[0]);
    }
    if(!found)
        printInfoFmt("%s", emptyMessage);
    mysql_free_result(res);
}

void smartPharmacyAdvice(void)
{
    printHeader("SMART PHARMACY ADVICE");
    printInfoFmt("Recommendations are based on sales and stock currently stored in the database.");

    printAdviceQuery("LIKELY NEXT PURCHASES",
        "SELECT CONCAT(m.medicine_name, ' | sold ', SUM(si.quantity), ' units in last 30 days | revenue ', FORMAT(SUM(si.subtotal),2), "
        "' | Advice: keep stock available and consider reordering.') "
        "FROM sale_items si JOIN sales s ON si.sale_id=s.sale_id JOIN medicines m ON si.medicine_id=m.medicine_id "
        "WHERE s.sale_date >= DATE_SUB(CURDATE(), INTERVAL 30 DAY) GROUP BY m.medicine_id,m.medicine_name "
        "ORDER BY SUM(si.quantity) DESC LIMIT 5",
        "No sales in the last 30 days, so demand cannot yet be predicted.");

    printAdviceQuery("BEST CUSTOMERS",
        "SELECT CONCAT(c.full_name, ' | purchases: ', COUNT(s.sale_id), ' | total spent: ', FORMAT(SUM(s.grand_total),2), "
        "' | Advice: offer a loyalty follow-up or notify them when regular items are available.') "
        "FROM customers c JOIN sales s ON c.customer_id=s.customer_id GROUP BY c.customer_id,c.full_name "
        "ORDER BY SUM(s.grand_total) DESC LIMIT 5",
        "No registered-customer sales yet. Record customer IDs to identify loyal customers.");

    printAdviceQuery("BEST SUPPLIERS FOR DEMAND",
        "SELECT CONCAT(s.company_name, ' | medicines supplied: ', COUNT(DISTINCT m.medicine_id), ' | units sold: ', COALESCE(SUM(si.quantity),0), "
        "' | sales value: ', FORMAT(COALESCE(SUM(si.subtotal),0),2), "
        "' | Advice: prioritize this supplier when restocking its fast-moving medicines.') "
        "FROM suppliers s JOIN medicines m ON s.supplier_id=m.supplier_id "
        "LEFT JOIN sale_items si ON m.medicine_id=si.medicine_id GROUP BY s.supplier_id,s.company_name "
        "ORDER BY COALESCE(SUM(si.quantity),0) DESC, COUNT(DISTINCT m.medicine_id) DESC LIMIT 5",
        "No supplier-linked medicines are available for analysis.");

    printAdviceQuery("STOCK MANAGEMENT ACTIONS",
        "SELECT CONCAT(medicine_name, ' | stock: ', quantity, ' | sold last 30 days: ', "
        "COALESCE((SELECT SUM(si.quantity) FROM sale_items si JOIN sales s ON si.sale_id=s.sale_id "
        "WHERE si.medicine_id=m.medicine_id AND s.sale_date >= DATE_SUB(CURDATE(), INTERVAL 30 DAY)),0), "
        "' | Advice: ', CASE WHEN quantity = 0 THEN 'OUT OF STOCK - reorder now' WHEN quantity < 10 THEN 'RESTOCK SOON' ELSE 'stock level is acceptable' END) "
        "FROM medicines m ORDER BY quantity ASC, medicine_name LIMIT 10",
        "No medicines are available for stock analysis.");

    printAdviceQuery("EXPIRY AND SAFETY ACTIONS",
        "SELECT CONCAT(medicine_name, ' | expiry: ', COALESCE(DATE_FORMAT(expiry_date,'%Y-%m-%d'),'not recorded'), ' | stock: ', quantity, "
        "' | Advice: ', CASE WHEN expiry_date < CURDATE() THEN 'EXPIRED - quarantine and do not sell' "
        "WHEN expiry_date <= DATE_ADD(CURDATE(), INTERVAL 30 DAY) THEN 'expires soon - sell first if clinically appropriate; do not over-order' "
        "ELSE 'expiry date is currently acceptable' END) "
        "FROM medicines WHERE expiry_date IS NULL OR expiry_date <= DATE_ADD(CURDATE(), INTERVAL 30 DAY) "
        "ORDER BY expiry_date ASC LIMIT 10",
        "No medicines are expired or due to expire in the next 30 days.");

    pauseForUser();
}

void removeExpiredMedicines(void)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char confirmation[32];
    unsigned long long removed;

    if(mysql_query(conn,
        "SELECT COUNT(*) FROM medicines m WHERE m.expiry_date < CURDATE() "
        "AND NOT EXISTS (SELECT 1 FROM sale_items si WHERE si.medicine_id=m.medicine_id)") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    printHeader("EXPIRED MEDICINE AUTO-CLEANUP");
    printf("Expired medicines safe to remove: %s\n", row[0]);
    mysql_free_result(res);
    printf("Only medicines with no previous sale record will be deleted.\n");
    printf("Expired medicines with sale history remain blocked from selling to protect receipt history.\n");
    printf("Type DELETE to remove all safe expired records: ");
    if(fgets(confirmation, sizeof(confirmation), stdin) == NULL)
        return;
    confirmation[strcspn(confirmation, "\r\n")] = '\0';

    if(strcmp(confirmation, "DELETE") != 0)
    {
        printInfoFmt("Auto-cleanup cancelled. No medicines were deleted.");
        pauseForUser();
        return;
    }

    if(mysql_query(conn,
        "DELETE m FROM medicines m LEFT JOIN sale_items si ON m.medicine_id=si.medicine_id "
        "WHERE m.expiry_date < CURDATE() AND si.sale_item_id IS NULL") != 0)
    {
        printErrorFmt("%s", mysql_error(conn));
        pauseForUser();
        return;
    }
    removed = mysql_affected_rows(conn);
    printSuccessFmt("Auto-cleanup completed. %llu expired medicine record(s) were deleted.", removed);
    pauseForUser();
}

void reportMenu(void)
{
    int choice;

    do
    {
        printHeader("REPORTS");
        printf("1. Today's Sales Report\n");
        printf("2. All-Time Sales Report\n");
        printf("3. Low Stock Report\n");
        printf("4. Expiring Soon Report\n");
        printf("5. Top Selling Medicines\n");
        printf("6. Sales By Specific Date\n");
        printf("7. Expired Medicines Report\n");
        printf("8. Inventory Value Report\n");
        printf("9. Monthly Sales Summary\n");
        printf("10. Smart Pharmacy Advice\n");
        printf("11. Remove Expired Medicines (Safe Auto-Cleanup)\n");
        printf("12. Back\n");
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
            case 7:
                expiredMedicinesReport();
                break;
            case 8:
                stockValueReport();
                break;
            case 9:
                monthlySalesReport();
                break;
            case 10:
                smartPharmacyAdvice();
                break;
            case 11:
                removeExpiredMedicines();
                break;
        }

    } while(choice != 12);
}
