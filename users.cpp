// // #include <iostream>
// // #include <string>
// // #include <mysql.h>
// // #include "database.h"
// // #include "users.h"

// // using namespace std;

// // void addUser()
// // {
// //     string name, username, password, phone, role;
// //     cin.ignore();

// //     cout<<"Full Name: ";
// //     getline(cin,name);
// //     cout<<"Username: ";
// //     getline(cin,username);
// //     cout<<"Password: ";
// //     getline(cin,password);
// //     cout<<"Phone: ";
// //     getline(cin,phone);
// //     cout<<"Role: ";
// //     getline(cin,role);

// //     string sql =
// //     "INSERT INTO users(full_name,username,password,phone,role) VALUES('"+
// //     name+"','"+username+"','"+password+"','"+phone+"','"+role+"')";

// //     if(mysql_query(conn,sql.c_str())==0)
// //         cout<<"\nUser Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
// //     else
// //         cout<<"\nError: "<<mysql_error(conn)<<endl;
// // }

// // void viewUsers()
// // {
// //     if(mysql_query(conn,"SELECT user_id,full_name,username,phone,role FROM users")!=0)
// //     {
// //         cout<<"\nError: "<<mysql_error(conn)<<endl;
// //         return;
// //     }

// //     MYSQL_RES *res=mysql_store_result(conn);
// //     MYSQL_ROW row;

// //     cout<<"\n==============================\n";

// //     while((row=mysql_fetch_row(res)))
// //     {
// //         cout<<"ID: "<<row[0]<<endl;
// //         cout<<"Name: "<<row[1]<<endl;
// //         cout<<"Username: "<<row[2]<<endl;
// //         cout<<"Phone: "<<row[3]<<endl;
// //         cout<<"Role: "<<row[4]<<endl;
// //         cout<<"------------------------\n";
// //     }

// //     mysql_free_result(res);
// // }

// // void searchUser()
// // {
// //     string keyword;
// //     cin.ignore();
// //     cout<<"Enter name, username or ID to search: ";
// //     getline(cin,keyword);

// //     string sql =
// //     "SELECT user_id,full_name,username,phone,role FROM users WHERE full_name LIKE '%"+
// //     keyword+"%' OR username LIKE '%"+keyword+"%' OR user_id='"+keyword+"'";

// //     if(mysql_query(conn,sql.c_str())!=0)
// //     {
// //         cout<<"\nError: "<<mysql_error(conn)<<endl;
// //         return;
// //     }

// //     MYSQL_RES *res=mysql_store_result(conn);
// //     MYSQL_ROW row;
// //     bool found=false;

// //     while((row=mysql_fetch_row(res)))
// //     {
// //         found=true;
// //         cout<<"\nID: "<<row[0]<<" | Name: "<<row[1]
// //             <<" | Username: "<<row[2]<<" | Role: "<<row[4]<<endl;
// //     }

// //     if(!found) cout<<"\nNo user found.\n";
// //     mysql_free_result(res);
// // }

// // void updateUser()
// // {
// //     int id;
// //     string field, value;

// //     cout<<"Enter User ID to update: ";
// //     cin>>id;
// //     cin.ignore();

// //     cout<<"Which field? (full_name/username/password/phone/role): ";
// //     getline(cin,field);

// //     cout<<"New value: ";
// //     getline(cin,value);

// //     string sql = "UPDATE users SET "+field+"='"+value+"' WHERE user_id="+to_string(id);

// //     if(mysql_query(conn,sql.c_str())==0)
// //     {
// //         if(mysql_affected_rows(conn)==0)
// //             cout<<"\nNo user found with that ID.\n";
// //         else
// //             cout<<"\nUser Updated Successfully!\n";
// //     }
// //     else
// //         cout<<"\nError: "<<mysql_error(conn)<<endl;
// // }

// // void deleteUser()
// // {
// //     int id;
// //     cout<<"Enter User ID to delete: ";
// //     cin>>id;

// //     string sql = "DELETE FROM users WHERE user_id="+to_string(id);

// //     if(mysql_query(conn,sql.c_str())==0)
// //     {
// //         if(mysql_affected_rows(conn)==0)
// //             cout<<"\nNo user found with that ID.\n";
// //         else
// //             cout<<"\nUser Deleted Successfully!\n";
// //     }
// //     else
// //         cout<<"\nError: "<<mysql_error(conn)<<endl;
// // }

// // void usersMenu()
// // {
// //     int choice;

// //     do
// //     {
// //         cout<<"\n===== MANAGE USERS =====\n";
// //         cout<<"1. Add User\n";
// //         cout<<"2. View Users\n";
// //         cout<<"3. Search User\n";
// //         cout<<"4. Update User\n";
// //         cout<<"5. Delete User\n";
// //         cout<<"6. Back\n";
// //         cout<<"Choice: ";
// //         cin>>choice;

// //         switch(choice)
// //         {
// //             case 1: addUser(); break;
// //             case 2: viewUsers(); break;
// //             case 3: searchUser(); break;
// //             case 4: updateUser(); break;
// //             case 5: deleteUser(); break;
// //         }

// //     }while(choice!=6);
// // }

// #include <iostream>
// #include <string>
// #include <mysql.h>
// #include "database.h"
// #include "users.h"

// using namespace std;

// void showUserListShort()
// {
//     if(mysql_query(conn,"SELECT user_id, full_name, username, role FROM users ORDER BY user_id")!=0)
//     {
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
//         return;
//     }

//     MYSQL_RES *res = mysql_store_result(conn);
//     MYSQL_ROW row;

//     cout<<"\n--- Available Users ---\n";
//     cout<<"ID | Name | Username | Role\n";

//     bool found=false;
//     while((row=mysql_fetch_row(res)))
//     {
//         found=true;
//         cout<<row[0]<<" | "<<row[1]<<" | "<<row[2]<<" | "<<row[3]<<endl;
//     }

//     if(!found) cout<<"(No users yet)\n";
//     cout<<"----------------------------\n";

//     mysql_free_result(res);
// }

// void addUser()
// {
//     string name, username, password, phone, role;
//     cin.ignore();

//     cout<<"Full Name: ";
//     getline(cin,name);
//     cout<<"Username: ";
//     getline(cin,username);
//     cout<<"Password: ";
//     getline(cin,password);
//     cout<<"Phone: ";
//     getline(cin,phone);
//     cout<<"Role: ";
//     getline(cin,role);

//     string sql =
//     "INSERT INTO users(full_name,username,password,phone,role) VALUES('"+
//     name+"','"+username+"','"+password+"','"+phone+"','"+role+"')";

//     if(mysql_query(conn,sql.c_str())==0)
//         cout<<"\nUser Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void viewUsers()
// {
//     if(mysql_query(conn,"SELECT user_id,full_name,username,phone,role FROM users")!=0)
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
//         cout<<"Username: "<<row[2]<<endl;
//         cout<<"Phone: "<<row[3]<<endl;
//         cout<<"Role: "<<row[4]<<endl;
//         cout<<"------------------------\n";
//     }

//     mysql_free_result(res);
// }

// void searchUser()
// {
//     string keyword;
//     cin.ignore();
//     cout<<"Enter name, username or ID to search: ";
//     getline(cin,keyword);

//     string sql =
//     "SELECT user_id,full_name,username,phone,role FROM users WHERE full_name LIKE '%"+
//     keyword+"%' OR username LIKE '%"+keyword+"%' OR user_id='"+keyword+"'";

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
//         cout<<"\nID: "<<row[0]<<" | Name: "<<row[1]
//             <<" | Username: "<<row[2]<<" | Role: "<<row[4]<<endl;
//     }

//     if(!found) cout<<"\nNo user found.\n";
//     mysql_free_result(res);
// }

// void updateUser()
// {
//     showUserListShort();

//     int id;
//     string field, value;

//     cout<<"Enter User ID to update: ";
//     cin>>id;
//     cin.ignore();

//     cout<<"Which field? (full_name/username/password/phone/role): ";
//     getline(cin,field);

//     cout<<"New value: ";
//     getline(cin,value);

//     string sql = "UPDATE users SET "+field+"='"+value+"' WHERE user_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo user found with that ID.\n";
//         else
//             cout<<"\nUser Updated Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void deleteUser()
// {
//     showUserListShort();

//     int id;
//     cout<<"Enter User ID to delete: ";
//     cin>>id;

//     string sql = "DELETE FROM users WHERE user_id="+to_string(id);

//     if(mysql_query(conn,sql.c_str())==0)
//     {
//         if(mysql_affected_rows(conn)==0)
//             cout<<"\nNo user found with that ID.\n";
//         else
//             cout<<"\nUser Deleted Successfully!\n";
//     }
//     else
//         cout<<"\nError: "<<mysql_error(conn)<<endl;
// }

// void usersMenu()
// {
//     int choice;

//     do
//     {
//         cout<<"\n===== MANAGE USERS =====\n";
//         cout<<"1. Add User\n";
//         cout<<"2. View Users\n";
//         cout<<"3. Search User\n";
//         cout<<"4. Update User\n";
//         cout<<"5. Delete User\n";
//         cout<<"6. Back\n";
//         cout<<"Choice: ";
//         cin>>choice;

//         switch(choice)
//         {
//             case 1: addUser(); break;
//             case 2: viewUsers(); break;
//             case 3: searchUser(); break;
//             case 4: updateUser(); break;
//             case 5: deleteUser(); break;
//         }

//     }while(choice!=6);
// }

#include <iostream>
#include <string>
#include <mysql.h>
#include "database.h"
#include "users.h"

using namespace std;

void showUserListShort()
{
    if(mysql_query(conn,"SELECT user_id, full_name, username, role FROM users ORDER BY user_id")!=0)
    {
        cout<<"\nError: "<<mysql_error(conn)<<endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout<<"\n--- Available Users ---\n";
    cout<<"ID | Name | Username | Role\n";

    bool found=false;
    while((row=mysql_fetch_row(res)))
    {
        found=true;
        cout<<row[0]<<" | "<<row[1]<<" | "<<row[2]<<" | "<<row[3]<<endl;
    }

    if(!found) cout<<"(No users yet)\n";
    cout<<"----------------------------\n";

    mysql_free_result(res);
}

void addUser()
{
    string name, username, password, phone, role;
    cin.ignore();

    cout<<"Full Name: ";
    getline(cin,name);
    cout<<"Username: ";
    getline(cin,username);
    cout<<"Password: ";
    getline(cin,password);
    cout<<"Phone: ";
    getline(cin,phone);
    cout<<"Role: ";
    getline(cin,role);

    string sql =
    "INSERT INTO users(full_name,username,password,phone,role) VALUES('"+
    name+"','"+username+"','"+password+"','"+phone+"','"+role+"')";

    if(mysql_query(conn,sql.c_str())==0)
        cout<<"\nUser Added Successfully! (ID: "<<mysql_insert_id(conn)<<")\n";
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void viewUsers()
{
    if(mysql_query(conn,"SELECT user_id,full_name,username,phone,role FROM users")!=0)
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
        cout<<"Username: "<<row[2]<<endl;
        cout<<"Phone: "<<row[3]<<endl;
        cout<<"Role: "<<row[4]<<endl;
        cout<<"------------------------\n";
    }

    mysql_free_result(res);
}

void searchUser()
{
    string keyword;
    cin.ignore();
    cout<<"Enter name, username or ID to search: ";
    getline(cin,keyword);

    string sql =
    "SELECT user_id,full_name,username,phone,role FROM users WHERE full_name LIKE '%"+
    keyword+"%' OR username LIKE '%"+keyword+"%' OR user_id='"+keyword+"'";

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
            <<" | Username: "<<row[2]<<" | Role: "<<row[4]<<endl;
    }

    if(!found) cout<<"\nNo user found.\n";
    mysql_free_result(res);
}

void updateUser()
{
    showUserListShort();

    int id;
    string field, value;

    cout<<"Enter User ID to update: ";
    cin>>id;
    cin.ignore();

    cout<<"Which field? (full_name/username/password/phone/role): ";
    getline(cin,field);

    cout<<"New value: ";
    getline(cin,value);

    string sql = "UPDATE users SET "+field+"='"+value+"' WHERE user_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo user found with that ID.\n";
        else
            cout<<"\nUser Updated Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void deleteUser()
{
    showUserListShort();

    int id;
    cout<<"Enter User ID to delete: ";
    cin>>id;

    string sql = "DELETE FROM users WHERE user_id="+to_string(id);

    if(mysql_query(conn,sql.c_str())==0)
    {
        if(mysql_affected_rows(conn)==0)
            cout<<"\nNo user found with that ID.\n";
        else
            cout<<"\nUser Deleted Successfully!\n";
    }
    else
        cout<<"\nError: "<<mysql_error(conn)<<endl;
}

void usersMenu()
{
    int choice;

    do
    {
        cout<<"\n===== MANAGE USERS =====\n";
        cout<<"1. Add User\n";
        cout<<"2. View Users\n";
        cout<<"3. Search User\n";
        cout<<"4. Update User\n";
        cout<<"5. Delete User\n";
        cout<<"6. Back\n";
        cout<<"Choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: addUser(); break;
            case 2: viewUsers(); break;
            case 3: searchUser(); break;
            case 4: updateUser(); break;
            case 5: deleteUser(); break;
        }

    }while(choice!=6);
}