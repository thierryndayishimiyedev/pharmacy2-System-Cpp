// #include <iostream>
// #include <string>
// #include <mysql.h>
// #include "database.h"
// #include "medicine.h"

// using namespace std;

// void addMedicine()
// {
//     string name, category, expiry;
//     int supplier, quantity;
//     float buyPrice, sellPrice;

//     cin.ignore();

//     cout<<"Medicine Name: ";
//     getline(cin,name);

//     cout<<"Category: ";
//     getline(cin,category);

//     cout<<"Supplier ID: ";
//     cin>>supplier;

//     cout<<"Buying Price: ";
//     cin>>buyPrice;

//     cout<<"Selling Price: ";
//     cin>>sellPrice;

//     cout<<"Quantity: ";
//     cin>>quantity;

//     cout<<"Expiry Date (YYYY-MM-DD): ";
//     cin>>expiry;

//     string sql =
//     "INSERT INTO medicines(medicine_name,category,supplier_id,buy_price,sell_price,quantity,expiry_date) VALUES('"+
//     name+"','"+category+"',"+
//     to_string(supplier)+","+
//     to_string(buyPrice)+","+
//     to_string(sellPrice)+","+
//     to_string(quantity)+",'"+
//     expiry+"')";

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nMedicine Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void viewMedicines()
// {
//     if(mysql_query(conn,"SELECT * FROM medicines")!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res=mysql_store_result(conn);
//     MYSQL_ROW row;

//     cout<<"\n==============================\n";

//     while((row=mysql_fetch_row(res)))
//     {
//         cout<<"ID: "<<row[0]<<endl;
//         cout<<"Name: "<<row[1]<<endl;
//         cout<<"Category: "<<row[2]<<endl;
//         cout<<"Supplier: "<<row[3]<<endl;
//         cout<<"Buy: "<<row[4]<<endl;
//         cout<<"Sell: "<<row[5]<<endl;
//         cout<<"Qty: "<<row[6]<<endl;
//         cout<<"Expiry: "<<row[7]<<endl;
//         cout<<"------------------------\n";
//     }

//     mysql_free_result(res);
// }

// void searchMedicine()
// {
//     string keyword;
//     cin.ignore();
//     cout<<"Enter medicine name or ID to search: ";
//     getline(cin,keyword);

//     string sql =
//     "SELECT * FROM medicines WHERE medicine_name LIKE '%"+keyword+"%' OR medicine_id='"+keyword+"'";

//     if(mysql_query(conn,sql.c_str())!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res=mysql_store_result(conn);
//     MYSQL_ROW row;
//     bool found=false;

//     while((row=mysql_fetch_row(res)))
//     {
//         found=true;
//         cout<<"\nID: "<<row[0]<<endl;
//         cout<<"Name: "<<row[1]<<endl;
//         cout<<"Category: "<<row[2]<<endl;
//         cout<<"Supplier: "<<row[3]<<endl;
//         cout<<"Buy: "<<row[4]<<endl;
//         cout<<"Sell: "<<row[5]<<endl;
//         cout<<"Qty: "<<row[6]<<endl;
//         cout<<"Expiry: "<<row[7]<<endl;
//         cout<<"------------------------\n";
//     }

//     if(!found) cout<<"\nNo medicine found.\n";
//     mysql_free_result(res);
// }

// void updateMedicine()
// {
//     int id;
//     string field, value;

//     cout<<"Enter Medicine ID to update: ";
//     cin>>id;
//     cin.ignore();

//     cout<<"Which field? (medicine_name/category/supplier_id/buy_price/sell_price/quantity/expiry_date): ";
//     getline(cin,field);

//     cout<<"New value: ";
//     getline(cin,value);

//     string sql;

//     if(field=="supplier_id" || field=="buy_price" || field=="sell_price" || field=="quantity")
//         sql = "UPDATE medicines SET "+field+"="+value+" WHERE medicine_id="+to_string(id);
//     else
//         sql = "UPDATE medicines SET "+field+"='"+value+"' WHERE medicine_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo medicine found with that ID.\n";
//         else
//             cout<<"\nMedicine Updated Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void deleteMedicine()
// {
//     int id;
//     cout<<"Enter Medicine ID to delete: ";
//     cin>>id;

//     string sql = "DELETE FROM medicines WHERE medicine_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo medicine found with that ID.\n";
//         else
//             cout<<"\nMedicine Deleted Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void medicineMenu()
// {
//     int choice;

//     do
//     {
//         cout<<"\n===== MEDICINES =====\n";
//         cout<<"1. Add Medicine\n";
//         cout<<"2. View Medicines\n";
//         cout<<"3. Search Medicine\n";
//         cout<<"4. Update Medicine\n";
//         cout<<"5. Delete Medicine\n";
//         cout<<"6. Back\n";
//         cout<<"Choice: ";
//         cin>>choice;

//         switch(choice)
//         {
//             case 1: addMedicine(); break;
//             case 2: viewMedicines(); break;
//             case 3: searchMedicine(); break;
//             case 4: updateMedicine(); break;
//             case 5: deleteMedicine(); break;
//         }

//     }while(choice!=6);
// }


#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "medicine.h"
#include "supplier.h"

using namespace std;

void showMedicineListShort()
{
    if(mysql_query(conn,"SELECT medicine_id, medicine_name, quantity, sell_price FROM medicines ORDER BY medicine_id")!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n--- Available Medicines ---\n";
    cout<<"ID | Name | Qty | Sell Price\n";

    bool found=false;
    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<row[0]<<" | "<<row[1]<<" | "<<row[2]<<" | "<<row[3]<<endl;
    }

    if(!found) cout<<"(No medicines yet - add one first!)\n";
    cout<<"----------------------------\n";

    mysql_free_result(res);
}

void addMedicine()
{
    string name, category, expiry;
    int supplier, quantity;
    float buyPrice, sellPrice;

    showSupplierListShort();

    cin.ignore();

    cout<<"Medicine Name: ";
    getline(cin,name);

    cout<<"Category: ";
    getline(cin,category);

    cout<<"Supplier ID (from list above): ";
    cin>>supplier;

    cout<<"Buying Price: ";
    cin>>buyPrice;

    cout<<"Selling Price: ";
    cin>>sellPrice;

    cout<<"Quantity: ";
    cin>>quantity;

    cout<<"Expiry Date (YYYY-MM-DD): ";
    cin>>expiry;

    string sql =
    "INSERT INTO medicines(medicine_name,category,supplier_id,buy_price,sell_price,quantity,expiry_date) VALUES('"+
    name+"','"+category+"',"+
    to_string(supplier)+","+
    to_string(buyPrice)+","+
    to_string(sellPrice)+","+
    to_string(quantity)+",'"+
    expiry+"')";

    if(mysql_query(conn,sql.c_str())==0)
        cout<<"\nMedicine Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void viewMedicines()
{
    if(mysql_query(conn,"SELECT * FROM medicines")!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res=mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n==============================\n";

    while((row=mysql_fetch_row(res)))
    {
        cout<<"ID: "<<row[0]<<endl;
        cout<<"Name: "<<row[1]<<endl;
        cout<<"Category: "<<row[2]<<endl;
        cout<<"Supplier: "<<row[3]<<endl;
        cout<<"Buy: "<<row[4]<<endl;
        cout<<"Sell: "<<row[5]<<endl;
        cout<<"Qty: "<<row[6]<<endl;
        cout<<"Expiry: "<<row[7]<<endl;
        cout<<"------------------------\n";
    }

    mysql_free_result(res);
}

void searchMedicine()
{
    string keyword;
    cin.ignore();
    cout<<"Enter medicine name or ID to search: ";
    getline(cin,keyword);

    string sql =
    "SELECT * FROM medicines WHERE medicine_name LIKE '%"+keyword+"%' OR medicine_id='"+keyword+"'";

    if(mysql_query(conn,sql.c_str())!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res=mysql_store_result(conn);
    MYSQL_ROW row;
    bool found=false;

    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<"\nID: "<<row[0]<<endl;
        cout<<"Name: "<<row[1]<<endl;
        cout<<"Category: "<<row[2]<<endl;
        cout<<"Supplier: "<<row[3]<<endl;
        cout<<"Buy: "<<row[4]<<endl;
        cout<<"Sell: "<<row[5]<<endl;
        cout<<"Qty: "<<row[6]<<endl;
        cout<<"Expiry: "<<row[7]<<endl;
        cout<<"------------------------\n";
    }

    if(!found) cout<<"\nNo medicine found.\n";
    mysql_free_result(res);
}

void updateMedicine()
{
    showMedicineListShort();

    int id;
    string field, value;

    cout<<"Enter Medicine ID to update: ";
    cin>>id;
    cin.ignore();

    cout<<"Which field? (medicine_name/category/supplier_id/buy_price/sell_price/quantity/expiry_date): ";
    getline(cin,field);

    cout<<"New value: ";
    getline(cin,value);

    string sql;

    if(field=="supplier_id" || field=="buy_price" || field=="sell_price" || field=="quantity")
        sql = "UPDATE medicines SET "+field+"="+value+" WHERE medicine_id="+to_string(id);
    else
        sql = "UPDATE medicines SET "+field+"='"+value+"' WHERE medicine_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo medicine found with that ID.\n";
        else
            cout<<"\nMedicine Updated Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void deleteMedicine()
{
    showMedicineListShort();

    int id;
    cout<<"Enter Medicine ID to delete: ";
    cin>>id;

    string sql = "DELETE FROM medicines WHERE medicine_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo medicine found with that ID.\n";
        else
            cout<<"\nMedicine Deleted Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void medicineMenu()
{
    int choice;

    do
    {
        cout<<"\n===== MEDICINES =====\n";
        cout<<"1. Add Medicine\n";
        cout<<"2. View Medicines\n";
        cout<<"3. Search Medicine\n";
        cout<<"4. Update Medicine\n";
        cout<<"5. Delete Medicine\n";
        cout<<"6. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: addMedicine(); break;
            case 2: viewMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: updateMedicine(); break;
            case 5: deleteMedicine(); break;
        }

    }while(choice!=6);
}