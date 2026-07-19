#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "dashboard_report.h"

using namespace std;

void systemOverview()
{
    cout<<"\n==================== SYSTEM OVERVIEW ====================\n";

    // Total Users
    if(mysql_query(conn,"SELECT COUNT(*) FROM users")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Users:            "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Total Customers
    if(mysql_query(conn,"SELECT COUNT(*) FROM customers")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Customers:        "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Total Suppliers
    if(mysql_query(conn,"SELECT COUNT(*) FROM suppliers")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Suppliers:        "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Total Medicines (distinct items) and Total Stock Quantity
    if(mysql_query(conn,"SELECT COUNT(*), COALESCE(SUM(quantity),0) FROM medicines")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Medicine Types:   "<<row[0]<<endl;
        cout<<"Total Stock Units:      "<<row[1]<<endl;
        mysql_free_result(res);
    }

    // Total Stock Value (based on buy price) - what you spent to stock up
    if(mysql_query(conn,"SELECT COALESCE(SUM(buy_price * quantity),0) FROM medicines")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Stock Cost Value: "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Total Potential Revenue if all current stock sold (based on sell price)
    if(mysql_query(conn,"SELECT COALESCE(SUM(sell_price * quantity),0) FROM medicines")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Potential Stock Value:  "<<row[0]<<endl;
        mysql_free_result(res);
    }

    cout<<"-----------------------------------------------------------\n";

    // Total Sales Count and Total Revenue Earned (actual money made)
    if(mysql_query(conn,"SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Sales Made:       "<<row[0]<<endl;
        cout<<"Total Revenue Earned:   "<<row[1]<<endl;
        mysql_free_result(res);
    }

    // Total Units Sold (all-time)
    if(mysql_query(conn,"SELECT COALESCE(SUM(quantity),0) FROM sale_items")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Units Sold:       "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Total Profit (revenue from sales - cost of those sold items)
    string profitSql =
    "SELECT COALESCE(SUM(si.subtotal),0) - COALESCE(SUM(m.buy_price * si.quantity),0) "
    "FROM sale_items si JOIN medicines m ON si.medicine_id = m.medicine_id";

    if(mysql_query(conn, profitSql.c_str())==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Total Profit (est.):    "<<row[0]<<endl;
        mysql_free_result(res);
    }

    cout<<"-----------------------------------------------------------\n";

    // Today's Sales quick glance
    if(mysql_query(conn,"SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales WHERE DATE(sale_date)=CURDATE()")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Today's Sales:          "<<row[0]<<endl;
        cout<<"Today's Revenue:        "<<row[1]<<endl;
        mysql_free_result(res);
    }

    // Low stock alert count (below 10 as default threshold)
    if(mysql_query(conn,"SELECT COUNT(*) FROM medicines WHERE quantity < 10")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Low Stock Items (<10):  "<<row[0]<<endl;
        mysql_free_result(res);
    }

    // Expiring soon count (within 30 days as default)
    if(mysql_query(conn,"SELECT COUNT(*) FROM medicines WHERE expiry_date <= DATE_ADD(CURDATE(), INTERVAL 30 DAY)")==0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        cout<<"Expiring Soon (<=30d):  "<<row[0]<<endl;
        mysql_free_result(res);
    }

    cout<<"===========================================================\n";
}