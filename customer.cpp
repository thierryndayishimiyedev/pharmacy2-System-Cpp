// #include <iostream>
// #include <string>
// #include <mysql.h>
// #include "database.h"
// #include "customer.h"

// using namespace std;

// void addCustomer()
// {
//     string name, phone, email, address;
//     cin.ignore();

//     cout<<"Full Name: ";
//     getline(cin,name);
//     cout<<"Phone: ";
//     getline(cin,phone);
//     cout<<"Email: ";
//     getline(cin,email);
//     cout<<"Address: ";
//     getline(cin,address);

//     string sql =
//     "INSERT INTO customers(full_name,phone,email,address) VALUES('"+
//     name+"','"+phone+"','"+email+"','"+address+"')";

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nCustomer Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
//     else
//         cout<<mysql_error(conn)<<endl;
// }

// void viewCustomers()
// {
//     mysql_query(conn,"SELECT * FROM customers");
//     MYSQL_RES *res=mysql_store_result(conn);
//     MYSQL_ROW row;

//     cout<<"\n==============================\n";
//     while((row=mysql_fetch_row(res)))
//     {
//         cout<<"ID: "<<row[0]<<endl;
//         cout<<"Name: "<<row[1]<<endl;
//         cout<<"Phone: "<<row[2]<<endl;
//         cout<<"Email: "<<row[3]<<endl;
//         cout<<"Address: "<<row[4]<<endl;
//         cout<<"------------------------\n";
//     }
//     mysql_free_result(res);
// }

// void searchCustomer()
// {
//     string keyword;
//     cin.ignore();
//     cout<<"Enter name, phone or ID to search: ";
//     getline(cin,keyword);

//     string sql =
//     "SELECT * FROM customers WHERE full_name LIKE '%"+keyword+"%' OR phone LIKE '%"+keyword+"%' OR customer_id='"+keyword+"'";

//     if(mysql_query(conn,sql.c_str())!=0)
//     {
//         cout<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res=mysql_store_result(conn);
//     MYSQL_ROW row;
//     bool found=false;

//     while((row=mysql_fetch_row(res)))
//     {
//         found=true;
//         cout<<"\nID: "<<row[0]<<" | Name: "<<row[1]
//             <<" | Phone: "<<row[2]<<" | Email: "<<row[3]<<endl;
//     }

//     if(!found) cout<<"No customer found.\n";
//     mysql_free_result(res);
// }

// void updateCustomer()
// {
//     int id;
//     string field, value;

//     cout<<"Enter Customer ID to update: ";
//     cin>>id;
//     cin.ignore();

//     cout<<"Which field? (full_name/phone/email/address): ";
//     getline(cin,field);

//     cout<<"New value: ";
//     getline(cin,value);

//     string sql =
//     "UPDATE customers SET "+field+"='"+value+"' WHERE customer_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nCustomer Updated Successfully!\n";
//     else
//         cout<<mysql_error(conn)<<endl;
// }

// void deleteCustomer()
// {
//     int id;
//     cout<<"Enter Customer ID to delete: ";
//     cin>>id;

//     string sql = "DELETE FROM customers WHERE customer_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nCustomer Deleted Successfully!\n";
//     else
//         cout<<mysql_error(conn)<<endl;
// }

// void customerMenu()
// {
//     int choice;
//     do
//     {
//         cout<<"\n===== CUSTOMERS =====\n";
//         cout<<"1. Add Customer\n";
//         cout<<"2. View Customers\n";
//         cout<<"3. Search Customer\n";
//         cout<<"4. Update Customer\n";
//         cout<<"5. Delete Customer\n";
//         cout<<"6. Back\n";
//         cout<<"Choice: ";
//         cin>>choice;

//         switch(choice)
//         {
//             case 1: addCustomer(); break;
//             case 2: viewCustomers(); break;
//             case 3: searchCustomer(); break;
//             case 4: updateCustomer(); break;
//             case 5: deleteCustomer(); break;
//         }
//     }while(choice!=6);

    
// }
// #ifndef MEDICINE_H
// #define MEDICINE_H

// void medicineMenu();
// void addMedicine();
// void viewMedicines();
// void updateMedicine();
// void deleteMedicine();
// void searchMedicine();

// #endif


#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "customer.h"

using namespace std;

void showCustomerListShort()
{
    if(mysql_query(conn,"SELECT customer_id, full_name, phone FROM customers ORDER BY customer_id")!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n--- Available Customers ---\n";
    cout<<"ID | Name | Phone\n";

    bool found=false;
    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<row[0]<<" | "<<row[1]<<" | "<<row[2]<<endl;
    }

    if(!found) cout<<"(No customers yet)\n";
    cout<<"----------------------------\n";

    mysql_free_result(res);
}

void addCustomer()
{
    string name, phone, email, address;
    cin.ignore();

    cout<<"Full Name: ";
    getline(cin,name);
    cout<<"Phone: ";
    getline(cin,phone);
    cout<<"Email: ";
    getline(cin,email);
    cout<<"Address: ";
    getline(cin,address);

    string sql =
    "INSERT INTO customers(full_name,phone,email,address) VALUES('"+
    name+"','"+phone+"','"+email+"','"+address+"')";

    if(mysql_query(conn,sql.c_str())==0)
        cout<<"\nCustomer Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void viewCustomers()
{
    if(mysql_query(conn,"SELECT * FROM customers")!=0)
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
        cout<<"Phone: "<<row[2]<<endl;
        cout<<"Email: "<<row[3]<<endl;
        cout<<"Address: "<<row[4]<<endl;
        cout<<"------------------------\n";
    }
    mysql_free_result(res);
}

void searchCustomer()
{
    string keyword;
    cin.ignore();
    cout<<"Enter name, phone or ID to search: ";
    getline(cin,keyword);

    string sql =
    "SELECT * FROM customers WHERE full_name LIKE '%"+keyword+"%' OR phone LIKE '%"+keyword+"%' OR customer_id='"+keyword+"'";

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
        cout<<"\nID: "<<row[0]<<" | Name: "<<row[1]
            <<" | Phone: "<<row[2]<<" | Email: "<<row[3]<<endl;
    }

    if(!found) cout<<"\nNo customer found.\n";
    mysql_free_result(res);
}

void updateCustomer()
{
    showCustomerListShort();

    int id;
    string field, value;

    cout<<"Enter Customer ID to update: ";
    cin>>id;
    cin.ignore();

    cout<<"Which field? (full_name/phone/email/address): ";
    getline(cin,field);

    cout<<"New value: ";
    getline(cin,value);

    string sql =
    "UPDATE customers SET "+field+"='"+value+"' WHERE customer_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo customer found with that ID.\n";
        else
            cout<<"\nCustomer Updated Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void deleteCustomer()
{
    showCustomerListShort();

    int id;
    cout<<"Enter Customer ID to delete: ";
    cin>>id;

    string sql = "DELETE FROM customers WHERE customer_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo customer found with that ID.\n";
        else
            cout<<"\nCustomer Deleted Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void customerMenu()
{
    int choice;
    do
    {
        cout<<"\n===== CUSTOMERS =====\n";
        cout<<"1. Add Customer\n";
        cout<<"2. View Customers\n";
        cout<<"3. Search Customer\n";
        cout<<"4. Update Customer\n";
        cout<<"5. Delete Customer\n";
        cout<<"6. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
            case 3: searchCustomer(); break;
            case 4: updateCustomer(); break;
            case 5: deleteCustomer(); break;
        }
    }while(choice!=6);
}