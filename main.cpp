#include <iostream>
#include "database.h"
#include "login.h"
#include "medicine.h"
#include "supplier.h"
#include "customer.h"
#include "sale.h"
#include "users.h"
#include "report.h"
#include "dashboard_report.h"

using namespace std;

void dashboard()
{
    int choice;

    do
    {
        cout << "\n========== PHARMACY MANAGEMENT SYSTEM ==========\n";
        cout << "1. Manage Users\n";
        cout << "2. Manage Medicines\n";
        cout << "3. Manage Suppliers\n";
        cout << "4. Manage Customers\n";
        cout << "5. Sell Medicine\n";
        cout << "6. Reports\n";
        cout << "7. System Overview (Dashboard)\n";
        cout << "8. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                usersMenu();
                break;
            case 2:
                medicineMenu();
                break;
            case 3:
                supplierMenu();
                break;
            case 4:
                customerMenu();
                break;
            case 5:
                saleMenu();
                break;
            case 6:
                reportMenu();
                break;
            case 7:
                systemOverview();
                break;
            case 8:
                cout << "Logged Out!\n";
                return;
            default:
                cout << "Invalid Choice!\n";
        }

    } while(true);
}

void menu()
{
    int choice;

    while(true)
    {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Login\n";
        cout << "2. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                if(login())
                {
                    cout << "\nLogin Successful!\n";
                    dashboard();
                }
                else
                {
                    cout << "\nInvalid Username or Password!\n";
                }
                break;

            case 2:
                disconnectDB();
                return;

            default:
                cout << "Invalid Choice!\n";
        }
    }
}

int main()
{
    if(!connectDB())
    {
        cout << "Database Connection Failed!\n";
        return 1;
    }

    menu();

    disconnectDB();

    return 0;
}