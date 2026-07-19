// // #include <iostream>
// // #include <string>
// // #include <mysql.h>
// // #include "database.h"
// // #include "supplier.h"

// // using namespace std;

// // void addSupplier()
// // {
// //     string company, contact, phone, email, address;
// //     cin.ignore();

// //     cout<<"Company Name: ";
// //     getline(cin,company);
// //     cout<<"Contact Person: ";
// //     getline(cin,contact);
// //     cout<<"Phone: ";
// //     getline(cin,phone);
// //     cout<<"Email: ";
// //     getline(cin,email);
// //     cout<<"Address: ";
// //     getline(cin,address);

// //     string sql =
// //     "INSERT INTO suppliers(company_name,contact_person,phone,email,address) VALUES('"+
// //     company+"','"+contact+"','"+phone+"','"+email+"','"+address+"')";

// //     if(mysql_query(conn,sql.c_str())==0)
// //         cout<<"\nSupplier Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
// //     else
// //         cout<<mysql_error(conn)<<endl;
// // }

// // void viewSuppliers()
// // {
// //     mysql_query(conn,"SELECT * FROM suppliers");
// //     MYSQL_RES *res=mysql_store_result(conn);
// //     MYSQL_ROW row;

// //     cout<<"\n==============================\n";
// //     while((row=mysql_fetch_row(res)))
// //     {
// //         cout<<"ID: "<<row[0]<<endl;
// //         cout<<"Company: "<<row[1]<<endl;
// //         cout<<"Contact: "<<row[2]<<endl;
// //         cout<<"Phone: "<<row[3]<<endl;
// //         cout<<"Email: "<<row[4]<<endl;
// //         cout<<"Address: "<<row[5]<<endl;
// //         cout<<"------------------------\n";
// //     }
// //     mysql_free_result(res);
// // }

// // void searchSupplier()
// // {
// //     string keyword;
// //     cin.ignore();
// //     cout<<"Enter company name or ID to search: ";
// //     getline(cin,keyword);

// //     string sql =
// //     "SELECT * FROM suppliers WHERE company_name LIKE '%"+keyword+"%' OR supplier_id='"+keyword+"'";

// //     if(mysql_query(conn,sql.c_str())!=0)
// //     {
// //         cout<<mysql_error(conn)<<endl;
// //         return;
// //     }

// //     MYSQL_RES *res=mysql_store_result(conn);
// //     MYSQL_ROW row;
// //     bool found=false;

// //     while((row=mysql_fetch_row(res)))
// //     {
// //         found=true;
// //         cout<<"\nID: "<<row[0]<<" | Company: "<<row[1]
// //             <<" | Contact: "<<row[2]<<" | Phone: "<<row[3]<<endl;
// //     }

// //     if(!found) cout<<"No supplier found.\n";
// //     mysql_free_result(res);
// // }

// // void updateSupplier()
// // {
// //     int id;
// //     string field, value;

// //     cout<<"Enter Supplier ID to update: ";
// //     cin>>id;
// //     cin.ignore();

// //     cout<<"Which field? (company_name/contact_person/phone/email/address): ";
// //     getline(cin,field);

// //     cout<<"New value: ";
// //     getline(cin,value);

// //     string sql =
// //     "UPDATE suppliers SET "+field+"='"+value+"' WHERE supplier_id="+to_string(id);

// //     if(mysql_query(conn,sql.c_str())==0)
// //         cout<<"\nSupplier Updated Successfully!\n";
// //     else
// //         cout<<mysql_error(conn)<<endl;
// // }

// // void deleteSupplier()
// // {
// //     int id;
// //     cout<<"Enter Supplier ID to delete: ";
// //     cin>>id;

// //     string sql = "DELETE FROM suppliers WHERE supplier_id="+to_string(id);

// //     if(mysql_query(conn,sql.c_str())==0)
// //         cout<<"\nSupplier Deleted Successfully!\n";
// //     else
// //         cout<<mysql_error(conn)<<endl; // will show FK error if medicines reference it
// // }

// // void supplierMenu()
// // {
// //     int choice;
// //     do
// //     {
// //         cout<<"\n===== SUPPLIERS =====\n";
// //         cout<<"1. Add Supplier\n";
// //         cout<<"2. View Suppliers\n";
// //         cout<<"3. Search Supplier\n";
// //         cout<<"4. Update Supplier\n";
// //         cout<<"5. Delete Supplier\n";
// //         cout<<"6. Back\n";
// //         cout<<"Choice: ";
// //         cin>>choice;

// //         switch(choice)
// //         {
// //             case 1: addSupplier(); break;
// //             case 2: viewSuppliers(); break;
// //             case 3: searchSupplier(); break;
// //             case 4: updateSupplier(); break;
// //             case 5: deleteSupplier(); break;
// //         }
// //     }while(choice!=6);
// // }

// #include <iostream>
// #include <string>
// #include <mysql.h>
// #include "database.h"
// #include "supplier.h"

// using namespace std;

// void showSupplierListShort()
// {
//     if(mysql_query(conn,"SELECT supplier_id, company_name FROM suppliers ORDER BY supplier_id")!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res = mysql_store_result(conn);
//     MYSQL_ROW row;

//     cout<<"\n--- Available Suppliers ---\n";
//     cout<<"ID | Company Name\n";

//     bool found=false;
//     while((row=mysql_fetch_row(res)))
//     {
//         found=true;
//         cout<<row[0]<<" | "<<row[1]<<endl;
//     }

//     if(!found) cout<<"(No suppliers yet - add one first!)\n";
//     cout<<"----------------------------\n";

//     mysql_free_result(res);
// }

// void addSupplier()
// {
//     string company, contact, phone, email, address;
//     cin.ignore();

//     cout<<"Company Name: ";
//     getline(cin,company);
//     cout<<"Contact Person: ";
//     getline(cin,contact);
//     cout<<"Phone: ";
//     getline(cin,phone);
//     cout<<"Email: ";
//     getline(cin,email);
//     cout<<"Address: ";
//     getline(cin,address);

//     string sql =
//     "INSERT INTO suppliers(company_name,contact_person,phone,email,address) VALUES('"+
//     company+"','"+contact+"','"+phone+"','"+email+"','"+address+"')";

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nSupplier Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void viewSuppliers()
// {
//     if(mysql_query(conn,"SELECT * FROM suppliers")!=0)
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
//         cout<<"Company: "<<row[1]<<endl;
//         cout<<"Contact: "<<row[2]<<endl;
//         cout<<"Phone: "<<row[3]<<endl;
//         cout<<"Email: "<<row[4]<<endl;
//         cout<<"Address: "<<row[5]<<endl;
//         cout<<"------------------------\n";
//     }
//     mysql_free_result(res);
// }

// void searchSupplier()
// {
//     string keyword;
//     cin.ignore();
//     cout<<"Enter company name or ID to search: ";
//     getline(cin,keyword);

//     string sql =
//     "SELECT * FROM suppliers WHERE company_name LIKE '%"+keyword+"%' OR supplier_id='"+keyword+"'";

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
//         cout<<"\nID: "<<row[0]<<" | Company: "<<row[1]
//             <<" | Contact: "<<row[2]<<" | Phone: "<<row[3]<<endl;
//     }

//     if(!found) cout<<"\nNo supplier found.\n";
//     mysql_free_result(res);
// }

// void updateSupplier()
// {
//     showSupplierListShort();

//     int id;
//     string field, value;

//     cout<<"Enter Supplier ID to update: ";
//     cin>>id;
//     cin.ignore();

//     cout<<"Which field? (company_name/contact_person/phone/email/address): ";
//     getline(cin,field);

//     cout<<"New value: ";
//     getline(cin,value);

//     string sql =
//     "UPDATE suppliers SET "+field+"='"+value+"' WHERE supplier_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo supplier found with that ID.\n";
//         else
//             cout<<"\nSupplier Updated Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void deleteSupplier()
// {
//     showSupplierListShort();

//     int id;
//     cout<<"Enter Supplier ID to delete: ";
//     cin>>id;

//     string sql = "DELETE FROM suppliers WHERE supplier_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo supplier found with that ID.\n";
//         else
//             cout<<"\nSupplier Deleted Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void supplierMenu()
// {
//     int choice;
//     do
//     {
//         cout<<"\n===== SUPPLIERS =====\n";
//         cout<<"1. Add Supplier\n";
//         cout<<"2. View Suppliers\n";
//         cout<<"3. Search Supplier\n";
//         cout<<"4. Update Supplier\n";
//         cout<<"5. Delete Supplier\n";
//         cout<<"6. Back\n";
//         cout<<"Choice: ";
//         cin>>choice;

//         switch(choice)
//         {
//             case 1: addSupplier(); break;
//             case 2: viewSuppliers(); break;
//             case 3: searchSupplier(); break;
//             case 4: updateSupplier(); break;
//             case 5: deleteSupplier(); break;
//         }
//     }while(choice!=6);
// }


#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "supplier.h"

using namespace std;

void showSupplierListShort()
{
    if(mysql_query(conn,"SELECT supplier_id, company_name FROM suppliers ORDER BY supplier_id")!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n--- Available Suppliers ---\n";
    cout<<"ID | Company Name\n";

    bool found=false;
    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<row[0]<<" | "<<row[1]<<endl;
    }

    if(!found) cout<<"(No suppliers yet - add one first!)\n";
    cout<<"----------------------------\n";

    mysql_free_result(res);
}

void addSupplier()
{
    string company, contact, phone, email, address;
    cin.ignore();

    cout<<"Company Name: ";
    getline(cin,company);
    cout<<"Contact Person: ";
    getline(cin,contact);
    cout<<"Phone: ";
    getline(cin,phone);
    cout<<"Email: ";
    getline(cin,email);
    cout<<"Address: ";
    getline(cin,address);

    string sql =
    "INSERT INTO suppliers(company_name,contact_person,phone,email,address) VALUES('"+
    company+"','"+contact+"','"+phone+"','"+email+"','"+address+"')";

    if(mysql_query(conn,sql.c_str())==0)
        cout<<"\nSupplier Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void viewSuppliers()
{
    if(mysql_query(conn,"SELECT * FROM suppliers")!=0)
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
        cout<<"Company: "<<row[1]<<endl;
        cout<<"Contact: "<<row[2]<<endl;
        cout<<"Phone: "<<row[3]<<endl;
        cout<<"Email: "<<row[4]<<endl;
        cout<<"Address: "<<row[5]<<endl;
        cout<<"------------------------\n";
    }
    mysql_free_result(res);
}

void searchSupplier()
{
    string keyword;
    cin.ignore();
    cout<<"Enter company name or ID to search: ";
    getline(cin,keyword);

    string sql =
    "SELECT * FROM suppliers WHERE company_name LIKE '%"+keyword+"%' OR supplier_id='"+keyword+"'";

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
        cout<<"\nID: "<<row[0]<<" | Company: "<<row[1]
            <<" | Contact: "<<row[2]<<" | Phone: "<<row[3]<<endl;
    }

    if(!found) cout<<"\nNo supplier found.\n";
    mysql_free_result(res);
}

void updateSupplier()
{
    showSupplierListShort();

    int id;
    string field, value;

    cout<<"Enter Supplier ID to update: ";
    cin>>id;
    cin.ignore();

    cout<<"Which field? (company_name/contact_person/phone/email/address): ";
    getline(cin,field);

    cout<<"New value: ";
    getline(cin,value);

    string sql =
    "UPDATE suppliers SET "+field+"='"+value+"' WHERE supplier_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo supplier found with that ID.\n";
        else
            cout<<"\nSupplier Updated Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void deleteSupplier()
{
    showSupplierListShort();

    int id;
    cout<<"Enter Supplier ID to delete: ";
    cin>>id;

    string sql = "DELETE FROM suppliers WHERE supplier_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo supplier found with that ID.\n";
        else
            cout<<"\nSupplier Deleted Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void supplierMenu()
{
    int choice;
    do
    {
        cout<<"\n===== SUPPLIERS =====\n";
        cout<<"1. Add Supplier\n";
        cout<<"2. View Suppliers\n";
        cout<<"3. Search Supplier\n";
        cout<<"4. Update Supplier\n";
        cout<<"5. Delete Supplier\n";
        cout<<"6. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: addSupplier(); break;
            case 2: viewSuppliers(); break;
            case 3: searchSupplier(); break;
            case 4: updateSupplier(); break;
            case 5: deleteSupplier(); break;
        }
    }while(choice!=6);
}