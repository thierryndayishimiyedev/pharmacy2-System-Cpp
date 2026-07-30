#include <stdio.h>
#include <mysql.h>
#include "database.h"
#include "dashboard_report.h"

void systemOverview(void)
{
    printf("\n==================== SYSTEM OVERVIEW ====================\n");

    if(mysql_query(conn, "SELECT COUNT(*) FROM users") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Users:            %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COUNT(*) FROM customers") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Customers:        %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COUNT(*) FROM suppliers") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Suppliers:        %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COUNT(*), COALESCE(SUM(quantity),0) FROM medicines") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Medicine Types:   %s\n", row[0]);
        printf("Total Stock Units:      %s\n", row[1]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COALESCE(SUM(buy_price * quantity),0) FROM medicines") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Stock Cost Value: %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COALESCE(SUM(sell_price * quantity),0) FROM medicines") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Potential Stock Value:  %s\n", row[0]);
        mysql_free_result(res);
    }

    printf("-----------------------------------------------------------\n");

    if(mysql_query(conn, "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Sales Made:       %s\n", row[0]);
        printf("Total Revenue Earned:   %s\n", row[1]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COALESCE(SUM(quantity),0) FROM sale_items") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Units Sold:       %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn,
        "SELECT COALESCE(SUM(si.subtotal),0) - COALESCE(SUM(m.buy_price * si.quantity),0) "
        "FROM sale_items si JOIN medicines m ON si.medicine_id = m.medicine_id") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Total Profit (est.):    %s\n", row[0]);
        mysql_free_result(res);
    }

    printf("-----------------------------------------------------------\n");

    if(mysql_query(conn, "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales WHERE DATE(sale_date)=CURDATE()") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Today's Sales:          %s\n", row[0]);
        printf("Today's Revenue:        %s\n", row[1]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COUNT(*) FROM medicines WHERE quantity < 10") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Low Stock Items (<10):  %s\n", row[0]);
        mysql_free_result(res);
    }

    if(mysql_query(conn, "SELECT COUNT(*) FROM medicines WHERE expiry_date <= DATE_ADD(CURDATE(), INTERVAL 30 DAY)") == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        printf("Expiring Soon (<=30d):  %s\n", row[0]);
        mysql_free_result(res);
    }

    printf("===========================================================\n");
}
