// #include <iostream>
// #include <string>
// #include <vector>
// #include <mysql.h>
// #include "database.h"
// #include "login.h"
// #include "sale.h"

// using namespace std;

// struct CartItem
// {
//     int medicineId;
//     int quantity;
//     float price;
//     float subtotal;
// };

// void sellMedicine()
// {
//     int customerId;
//     vector<CartItem> cart;
//     float grandTotal = 0;

//     cout<<"\nEnter Customer ID (0 for walk-in customer): ";
//     cin>>customerId;

//     if(customerId != 0)
//     {
//         string checkSql = "SELECT customer_id FROM customers WHERE customer_id="+to_string(customerId);
//         if(mysql_query(conn, checkSql.c_str())!=0)
//         {
//             cout<<"\nError: "<<mysql_error(conn)<<endl;
//             return;
//         }
//         MYSQL_RES *checkRes = mysql_store_result(conn);
//         if(mysql_num_rows(checkRes)==0)
//         {
//             cout<<"\nNo customer found with that ID. Cancelling sale.\n";
//             mysql_free_result(checkRes);
//             return;
//         }
//         mysql_free_result(checkRes);
//     }

//     char more = 'y';

//     while(more=='y' || more=='Y')
//     {
//         int medicineId, qty;

//         cout<<"\nEnter Medicine ID: ";
//         cin>>medicineId;

//         string medSql = "SELECT sell_price, quantity FROM medicines WHERE medicine_id="+to_string(medicineId);
//         if(mysql_query(conn, medSql.c_str())!=0)
//         {
//             cout<<"\nError: "<<mysql_error(conn)<<endl;
//             continue;
//         }

//         MYSQL_RES *medRes = mysql_store_result(conn);
//         MYSQL_ROW medRow = mysql_fetch_row(medRes);

//         if(!medRow)
//         {
//             cout<<"\nNo medicine found with that ID.\n";
//             mysql_free_result(medRes);
//             cout<<"\nAdd another item? (y/n): ";
//             cin>>more;
//             continue;
//         }

//         float sellPrice = atof(medRow[0]);
//         int availableQty = atoi(medRow[1]);
//         mysql_free_result(medRes);

//         cout<<"Available Stock: "<<availableQty<<endl;
//         cout<<"Quantity to sell: ";
//         cin>>qty;

//         if(qty > availableQty)
//         {
//             cout<<"\nNot enough stock! Only "<<availableQty<<" available.\n";
//             cout<<"\nAdd another item? (y/n): ";
//             cin>>more;
//             continue;
//         }

//         if(qty <= 0)
//         {
//             cout<<"\nQuantity must be greater than 0.\n";
//             cout<<"\nAdd another item? (y/n): ";
//             cin>>more;
//             continue;
//         }

//         CartItem item;
//         item.medicineId = medicineId;
//         item.quantity = qty;
//         item.price = sellPrice;
//         item.subtotal = qty * sellPrice;

//         cart.push_back(item);
//         grandTotal += item.subtotal;

//         cout<<"Added: Medicine ID "<<medicineId<<" x"<<qty<<" = "<<item.subtotal<<endl;

//         cout<<"\nAdd another item? (y/n): ";
//         cin>>more;
//     }

//     if(cart.empty())
//     {
//         cout<<"\nNo items in cart. Sale cancelled.\n";
//         return;
//     }

//     cout<<"\n===== SALE SUMMARY =====\n";
//     cout<<"Grand Total: "<<grandTotal<<endl;
//     cout<<"Confirm sale? (y/n): ";
//     char confirm;
//     cin>>confirm;

//     if(confirm!='y' && confirm!='Y')
//     {
//         cout<<"\nSale cancelled.\n";
//         return;
//     }

//     mysql_query(conn, "START TRANSACTION");

//     string customerPart = (customerId==0) ? "NULL" : to_string(customerId);

//     string saleSql =
//     "INSERT INTO sales(customer_id,user_id,sale_date,grand_total) VALUES("+
//     customerPart+","+to_string(currentUserId)+",NOW(),"+to_string(grandTotal)+")";

//     if(mysql_query(conn, saleSql.c_str())!=0)
//     {
//         cout<<"\nError creating sale: "<<mysql_error(conn)<<endl;
//         mysql_query(conn, "ROLLBACK");
//         return;
//     }

//     unsigned long long saleId = mysql_insert_id(conn);
//     bool success = true;

//     for(size_t i=0; i<cart.size(); i++)
//     {
//         string itemSql =
//         "INSERT INTO sale_items(sale_id,medicine_id,quantity,price,subtotal) VALUES("+
//         to_string(saleId)+","+to_string(cart[i].medicineId)+","+
//         to_string(cart[i].quantity)+","+to_string(cart[i].price)+","+
//         to_string(cart[i].subtotal)+")";

//         if(mysql_query(conn, itemSql.c_str())!=0)
//         {
//             cout<<"\nError adding sale item: "<<mysql_error(conn)<<endl;
//             success = false;
//             break;
//         }

//         string updateStockSql =
//         "UPDATE medicines SET quantity = quantity - "+to_string(cart[i].quantity)+
//         " WHERE medicine_id="+to_string(cart[i].medicineId);

//         if(mysql_query(conn, updateStockSql.c_str())!=0)
//         {
//             cout<<"\nError updating stock: "<<mysql_error(conn)<<endl;
//             success = false;
//             break;
//         }
//     }

//     if(success)
//     {
//         mysql_query(conn, "COMMIT");
//         cout<<"\nSale Completed Successfully! (Sale ID: "<<saleId<<")\n";
//     }
//     else
//     {
//         mysql_query(conn, "ROLLBACK");
//         cout<<"\nSale rolled back due to error. No changes were made.\n";
//     }
// }

// void viewSales()
// {
//     string sql =
//     "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
//     "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
//     "ORDER BY s.sale_id DESC";

//     if(mysql_query(conn, sql.c_str())!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res = mysql_store_result(conn);
//     MYSQL_ROW row;

//     cout<<"\n==============================\n";

//     while((row=mysql_fetch_row(res)))
//     {
//         cout<<"Sale ID: "<<row[0]<<endl;
//         cout<<"Customer: "<<(row[2] ? row[2] : "Walk-in")<<endl;
//         cout<<"Served by User ID: "<<row[3]<<endl;
//         cout<<"Date: "<<row[4]<<endl;
//         cout<<"Grand Total: "<<row[5]<<endl;
//         cout<<"------------------------\n";
//     }

//     mysql_free_result(res);
// }

// void searchSales()
// {
//     string keyword;
//     cin.ignore();
//     cout<<"Enter Sale ID or Customer ID to search: ";
//     getline(cin,keyword);

//     string sql =
//     "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
//     "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
//     "WHERE s.sale_id='"+keyword+"' OR s.customer_id='"+keyword+"'";

//     if(mysql_query(conn, sql.c_str())!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res = mysql_store_result(conn);
//     MYSQL_ROW row;
//     bool found=false;

//     while((row=mysql_fetch_row(res)))
//     {
//         found=true;
//         cout<<"\nSale ID: "<<row[0]<<endl;
//         cout<<"Customer: "<<(row[2] ? row[2] : "Walk-in")<<endl;
//         cout<<"Served by User ID: "<<row[3]<<endl;
//         cout<<"Date: "<<row[4]<<endl;
//         cout<<"Grand Total: "<<row[5]<<endl;
//         cout<<"------------------------\n";
//     }

//     if(!found) cout<<"\nNo sale found.\n";
//     mysql_free_result(res);
// }

// void deleteSale()
// {
//     int saleId;
//     cout<<"Enter Sale ID to delete (this restores stock and cannot be undone): ";
//     cin>>saleId;

//     mysql_query(conn, "START TRANSACTION");

//     string itemsSql = "SELECT medicine_id, quantity FROM sale_items WHERE sale_id="+to_string(saleId);

//     if(mysql_query(conn, itemsSql.c_str())!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         mysql_query(conn, "ROLLBACK");
//         return;
//     }

//     MYSQL_RES *res = mysql_store_result(conn);
//     MYSQL_ROW row;

//     if(mysql_num_rows(res)==0)
//     {
//         cout<<"\nNo sale found with that ID.\n";
//         mysql_free_result(res);
//         mysql_query(conn, "ROLLBACK");
//         return;
//     }

//     bool success = true;

//     while((row=mysql_fetch_row(res)))
//     {
//         string restoreSql =
//         "UPDATE medicines SET quantity = quantity + "+string(row[1])+
//         " WHERE medicine_id="+string(row[0]);

//         if(mysql_query(conn, restoreSql.c_str())!=0)
//         {
//             cout<<"\nError restoring stock: "<<mysql_error(conn)<<endl;
//             success = false;
//             break;
//         }
//     }
//     mysql_free_result(res);

//     if(success)
//     {
//         string delItemsSql = "DELETE FROM sale_items WHERE sale_id="+to_string(saleId);
//         if(mysql_query(conn, delItemsSql.c_str())!=0)
//         {
//             cout<<"\nError deleting sale items: "<<mysql_error(conn)<<endl;
//             success = false;
//         }
//     }

//     if(success)
//     {
//         string delSaleSql = "DELETE FROM sales WHERE sale_id="+to_string(saleId);
//         if(mysql_query(conn, delSaleSql.c_str())!=0)
//         {
//             cout<<"\nError deleting sale: "<<mysql_error(conn)<<endl;
//             success = false;
//         }
//     }

//     if(success)
//     {
//         mysql_query(conn, "COMMIT");
//         cout<<"\nSale Deleted Successfully! Stock has been restored.\n";
//     }
//     else
//     {
//         mysql_query(conn, "ROLLBACK");
//         cout<<"\nDelete rolled back due to error. No changes were made.\n";
//     }
// }

// void saleMenu()
// {
//     int choice;

//     do
//     {
//         cout<<"\n===== SELL MEDICINE =====\n";
//         cout<<"1. New Sale\n";
//         cout<<"2. View Sales\n";
//         cout<<"3. Search Sales\n";
//         cout<<"4. Delete Sale\n";
//         cout<<"5. Back\n";
//         cout<<"Choice: ";
//         cin>>choice;

//         switch(choice)
//         {
//             case 1: sellMedicine(); break;
//             case 2: viewSales(); break;
//             case 3: searchSales(); break;
//             case 4: deleteSale(); break;
//         }

//     }while(choice!=5);
// }

#include <iostream>
#include <string>
#include <vector>
#include <mysql.h>
#include "database.h"
#include "login.h"
#include "sale.h"
#include "customer.h"
#include "medicine.h"

using namespace std;

struct CartItem
{
    int medicineId;
    int quantity;
    float price;
    float subtotal;
};

void sellMedicine()
{
    int customerId;
    vector<CartItem> cart;
    float grandTotal = 0;

    showCustomerListShort();

    cout<<"\nEnter Customer ID from list above (0 for walk-in customer): ";
    cin>>customerId;

    if(customerId != 0)
    {
        string checkSql = "SELECT customer_id FROM customers WHERE customer_id="+to_string(customerId);
        if(mysql_query(conn, checkSql.c_str())!=0)
        {
            cout<<"\nError: "<<mysql_error(conn)<<endl;
            return;
        }
        MYSQL_RES *checkRes = mysql_store_result(conn);
        if(mysql_num_rows(checkRes)==0)
        {
            cout<<"\nNo customer found with that ID. Cancelling sale.\n";
            mysql_free_result(checkRes);
            return;
        }
        mysql_free_result(checkRes);
    }

    char more = 'y';

    while(more=='y' || more=='Y')
    {
        int medicineId, qty;

        showMedicineListShort();

        cout<<"\nEnter Medicine ID from list above: ";
        cin>>medicineId;

        string medSql = "SELECT sell_price, quantity FROM medicines WHERE medicine_id="+to_string(medicineId);
        if(mysql_query(conn, medSql.c_str())!=0)
        {
            cout<<"\nError: "<<mysql_error(conn)<<endl;
            continue;
        }

        MYSQL_RES *medRes = mysql_store_result(conn);
        MYSQL_ROW medRow = mysql_fetch_row(medRes);

        if(!medRow)
        {
            cout<<"\nNo medicine found with that ID.\n";
            mysql_free_result(medRes);
            cout<<"\nAdd another item? (y/n): ";
            cin>>more;
            continue;
        }

        float sellPrice = atof(medRow[0]);
        int availableQty = atoi(medRow[1]);
        mysql_free_result(medRes);

        cout<<"Available Stock: "<<availableQty<<endl;
        cout<<"Quantity to sell: ";
        cin>>qty;

        if(qty > availableQty)
        {
            cout<<"\nNot enough stock! Only "<<availableQty<<" available.\n";
            cout<<"\nAdd another item? (y/n): ";
            cin>>more;
            continue;
        }

        if(qty <= 0)
        {
            cout<<"\nQuantity must be greater than 0.\n";
            cout<<"\nAdd another item? (y/n): ";
            cin>>more;
            continue;
        }

        CartItem item;
        item.medicineId = medicineId;
        item.quantity = qty;
        item.price = sellPrice;
        item.subtotal = qty * sellPrice;

        cart.push_back(item);
        grandTotal += item.subtotal;

        cout<<"Added: Medicine ID "<<medicineId<<" x"<<qty<<" = "<<item.subtotal<<endl;

        cout<<"\nAdd another item? (y/n): ";
        cin>>more;
    }

    if(cart.empty())
    {
        cout<<"\nNo items in cart. Sale cancelled.\n";
        return;
    }

    cout<<"\n===== SALE SUMMARY =====\n";
    cout<<"Grand Total: "<<grandTotal<<endl;
    cout<<"Confirm sale? (y/n): ";
    char confirm;
    cin>>confirm;

    if(confirm!='y' && confirm!='Y')
    {
        cout<<"\nSale cancelled.\n";
        return;
    }

    mysql_query(conn, "START TRANSACTION");

    string customerPart = (customerId==0) ? "NULL" : to_string(customerId);

    string saleSql =
    "INSERT INTO sales(customer_id,user_id,sale_date,grand_total) VALUES("+
    customerPart+","+to_string(currentUserId)+",NOW(),"+to_string(grandTotal)+")";

    if(mysql_query(conn, saleSql.c_str())!=0)
    {
        cout<<"\nError creating sale: "<<mysql_error(conn)<<endl;
        mysql_query(conn, "ROLLBACK");
        return;
    }

    unsigned long long saleId = mysql_insert_id(conn);
    bool success = true;

    for(size_t i=0; i<cart.size(); i++)
    {
        string itemSql =
        "INSERT INTO sale_items(sale_id,medicine_id,quantity,price,subtotal) VALUES("+
        to_string(saleId)+","+to_string(cart[i].medicineId)+","+
        to_string(cart[i].quantity)+","+to_string(cart[i].price)+","+
        to_string(cart[i].subtotal)+")";

        if(mysql_query(conn, itemSql.c_str())!=0)
        {
            cout<<"\nError adding sale item: "<<mysql_error(conn)<<endl;
            success = false;
            break;
        }

        string updateStockSql =
        "UPDATE medicines SET quantity = quantity - "+to_string(cart[i].quantity)+
        " WHERE medicine_id="+to_string(cart[i].medicineId);

        if(mysql_query(conn, updateStockSql.c_str())!=0)
        {
            cout<<"\nError updating stock: "<<mysql_error(conn)<<endl;
            success = false;
            break;
        }
    }

    if(success)
    {
        mysql_query(conn, "COMMIT");
        cout<<"\nSale Completed Successfully! (Sale ID: "<<saleId<<")\n";
    }
    else
    {
        mysql_query(conn, "ROLLBACK");
        cout<<"\nSale rolled back due to error. No changes were made.\n";
    }
}

void viewSales()
{
    string sql =
    "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
    "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
    "ORDER BY s.sale_id DESC";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n==============================\n";

    while((row=mysql_fetch_row(res)))
    {
        cout<<"Sale ID: "<<row[0]<<endl;
        cout<<"Customer: "<<(row[2] ? row[2] : "Walk-in")<<endl;
        cout<<"Served by User ID: "<<row[3]<<endl;
        cout<<"Date: "<<row[4]<<endl;
        cout<<"Grand Total: "<<row[5]<<endl;
        cout<<"------------------------\n";
    }

    mysql_free_result(res);
}

void searchSales()
{
    string keyword;
    cin.ignore();
    cout<<"Enter Sale ID or Customer ID to search: ";
    getline(cin,keyword);

    string sql =
    "SELECT s.sale_id, s.customer_id, c.full_name, s.user_id, s.sale_date, s.grand_total "
    "FROM sales s LEFT JOIN customers c ON s.customer_id = c.customer_id "
    "WHERE s.sale_id='"+keyword+"' OR s.customer_id='"+keyword+"'";

    if(mysql_query(conn, sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"\nSale ID: "<<row[0]<<endl;
        cout<<"Customer: "<<(row[2] ? row[2] : "Walk-in")<<endl;
        cout<<"Served by User ID: "<<row[3]<<endl;
        cout<<"Date: "<<row[4]<<endl;
        cout<<"Grand Total: "<<row[5]<<endl;
        cout<<"------------------------\n";
    }

    if(!found) cout<<"\nNo sale found.\n";
    mysql_free_result(res);
}

void deleteSale()
{
    int saleId;
    cout<<"Enter Sale ID to delete (this restores stock and cannot be undone): ";
    cin>>saleId;

    mysql_query(conn, "START TRANSACTION");

    string itemsSql = "SELECT medicine_id, quantity FROM sale_items WHERE sale_id="+to_string(saleId);

    if(mysql_query(conn, itemsSql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        mysql_query(conn, "ROLLBACK");
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    if(mysql_num_rows(res)==0)
    {
        cout<<"\nNo sale found with that ID.\n";
        mysql_free_result(res);
        mysql_query(conn, "ROLLBACK");
        return;
    }

    bool success = true;

    while((row=mysql_fetch_row(res)))
    {
        string restoreSql =
        "UPDATE medicines SET quantity = quantity + "+string(row[1])+
        " WHERE medicine_id="+string(row[0]);

        if(mysql_query(conn, restoreSql.c_str())!=0)
        {
            cout<<"\nError restoring stock: "<<mysql_error(conn)<<endl;
            success = false;
            break;
        }
    }
    mysql_free_result(res);

    if(success)
    {
        string delItemsSql = "DELETE FROM sale_items WHERE sale_id="+to_string(saleId);
        if(mysql_query(conn, delItemsSql.c_str())!=0)
        {
            cout<<"\nError deleting sale items: "<<mysql_error(conn)<<endl;
            success = false;
        }
    }

    if(success)
    {
        string delSaleSql = "DELETE FROM sales WHERE sale_id="+to_string(saleId);
        if(mysql_query(conn, delSaleSql.c_str())!=0)
        {
            cout<<"\nError deleting sale: "<<mysql_error(conn)<<endl;
            success = false;
        }
    }

    if(success)
    {
        mysql_query(conn, "COMMIT");
        cout<<"\nSale Deleted Successfully! Stock has been restored.\n";
    }
    else
    {
        mysql_query(conn, "ROLLBACK");
        cout<<"\nDelete rolled back due to error. No changes were made.\n";
    }
}

void saleMenu()
{
    int choice;

    do
    {
        cout<<"\n===== SELL MEDICINE =====\n";
        cout<<"1. New Sale\n";
        cout<<"2. View Sales\n";
        cout<<"3. Search Sales\n";
        cout<<"4. Delete Sale\n";
        cout<<"5. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: sellMedicine(); break;
            case 2: viewSales(); break;
            case 3: searchSales(); break;
            case 4: deleteSale(); break;
        }

    }while(choice!=5);
}