#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "report.h"

using namespace std;

void dailySalesReport()
{
    string sql =
    "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales WHERE DATE(sale_date)=CURDATE()";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    cout<<"\n===== TODAY'S SALES REPORT =====\n";
    cout<<"Number of Sales: "<<row[0]<<endl;
    cout<<"Total Revenue: "<<row[1]<<endl;

    mysql_free_result(res);

    string detailSql =
    "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
    "FROM sales s LEFT JOIN customers c ON s.customer_id=c.customer_id "
    "WHERE DATE(s.sale_date)=CURDATE() ORDER BY s.sale_date DESC";

    if(mysql_query(conn, detailSql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res2 = mysql_store_result(conn);
    MYSQL_ROW row2;

    cout<<"\n--- Details ---\n";
    while((row2=mysql_fetch_row(res2)))
    {
        cout<<"Sale ID: "<<row2[0]
            <<" | Customer: "<<(row2[1] ? row2[1] : "Walk-in")
            <<" | Total: "<<row2[2]
            <<" | Time: "<<row2[3]<<endl;
    }

    mysql_free_result(res2);
}

void totalSalesReport()
{
    string sql = "SELECT COUNT(*), COALESCE(SUM(grand_total),0) FROM sales";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    cout<<"\n===== ALL-TIME SALES REPORT =====\n";
    cout<<"Total Number of Sales: "<<row[0]<<endl;
    cout<<"Total Revenue: "<<row[1]<<endl;

    mysql_free_result(res);
}

void lowStockReport()
{
    int threshold;
    cout<<"\nShow medicines with stock below: ";
    cin>>threshold;

    string sql =
    "SELECT medicine_id, medicine_name, quantity FROM medicines WHERE quantity < "+
    to_string(threshold)+" ORDER BY quantity ASC";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;

    cout<<"\n===== LOW STOCK REPORT (below "<<threshold<<") =====\n";

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"ID: "<<row[0]<<" | Name: "<<row[1]<<" | Qty Left: "<<row[2]<<endl;
    }

    if(!found) cout<<"No medicines below that stock level.\n";

    mysql_free_result(res);
}

void expiringSoonReport()
{
    int days;
    cout<<"\nShow medicines expiring within how many days: ";
    cin>>days;

    string sql =
    "SELECT medicine_id, medicine_name, expiry_date, quantity FROM medicines "
    "WHERE expiry_date <= DATE_ADD(CURDATE(), INTERVAL "+to_string(days)+" DAY) "
    "ORDER BY expiry_date ASC";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;

    cout<<"\n===== EXPIRING WITHIN "<<days<<" DAYS =====\n";

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"ID: "<<row[0]<<" | Name: "<<row[1]
            <<" | Expiry: "<<row[2]<<" | Qty: "<<row[3]<<endl;
    }

    if(!found) cout<<"No medicines expiring in that window.\n";

    mysql_free_result(res);
}

void topSellingMedicines()
{
    int limit;
    cout<<"\nShow top how many medicines: ";
    cin>>limit;

    string sql =
    "SELECT m.medicine_id, m.medicine_name, SUM(si.quantity) as total_sold, SUM(si.subtotal) as revenue "
    "FROM sale_items si JOIN medicines m ON si.medicine_id = m.medicine_id "
    "GROUP BY m.medicine_id, m.medicine_name "
    "ORDER BY total_sold DESC LIMIT "+to_string(limit);

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;

    cout<<"\n===== TOP "<<limit<<" SELLING MEDICINES =====\n";

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"ID: "<<row[0]<<" | Name: "<<row[1]
            <<" | Units Sold: "<<row[2]<<" | Revenue: "<<row[3]<<endl;
    }

    if(!found) cout<<"No sales data yet.\n";

    mysql_free_result(res);
}

void salesByDate()
{
    string date;
    cin.ignore();
    cout<<"\nEnter date (YYYY-MM-DD): ";
    getline(cin,date);

    string sql =
    "SELECT s.sale_id, c.full_name, s.grand_total, s.sale_date "
    "FROM sales s LEFT JOIN customers c ON s.customer_id=c.customer_id "
    "WHERE DATE(s.sale_date)='"+date+"' ORDER BY s.sale_date ASC";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;
    float total=0;

    cout<<"\n===== SALES ON "<<date<<" =====\n";

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"Sale ID: "<<row[0]
            <<" | Customer: "<<(row[1] ? row[1] : "Walk-in")
            <<" | Total: "<<row[2]
            <<" | Time: "<<row[3]<<endl;
        total += atof(row[2]);
    }

    if(!found)
        cout<<"No sales found on that date.\n";
    else
        cout<<"\nDay Total: "<<total<<endl;

    mysql_free_result(res);
}

void reportMenu()
{
    int choice;

    do
    {
        cout<<"\n===== REPORTS =====\n";
        cout<<"1. Today's Sales Report\n";
        cout<<"2. All-Time Sales Report\n";
        cout<<"3. Low Stock Report\n";
        cout<<"4. Expiring Soon Report\n";
        cout<<"5. Top Selling Medicines\n";
        cout<<"6. Sales By Specific Date\n";
        cout<<"7. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: dailySalesReport(); break;
            case 2: totalSalesReport(); break;
            case 3: lowStockReport(); break;
            case 4: expiringSoonReport(); break;
            case 5: topSellingMedicines(); break;
            case 6: salesByDate(); break;
        }

    }while(choice!=7);
}