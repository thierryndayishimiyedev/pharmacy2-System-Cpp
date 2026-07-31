#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "login.h"
#include "medicine.h"
#include "supplier.h"
#include "customer.h"
#include "sale.h"
#include "users.h"
#include "report.h"
#include "dashboard_report.h"
#include "ui.h"

static void clearInput(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void dashboard(void)
{
    int choice;

    do
    {
        printHeader("PHARMACY MANAGEMENT SYSTEM");
        printf("1. Manage Users\n");
        printf("2. Manage Medicines\n");
        printf("3. Manage Suppliers\n");
        printf("4. Manage Customers\n");
        printf("5. Sell Medicine\n");
        printf("6. Reports\n");
        printf("7. System Overview (Dashboard)\n");
        printf("8. Logout\n");
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
                printSuccessFmt("Logged out successfully.");
                setActiveUserDisplay("");
                return;
            default:
                printErrorFmt("Invalid choice. Please select a number from the menu.");
        }

    } while(true);
}

void menu(void)
{
    int choice;

    while(true)
    {
        printHeader("MAIN MENU");
        printf("1. Login\n");
        printf("2. Exit\n");
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
                if(login())
                {
                    printSuccessFmt("Login successful. Welcome!");
                    dashboard();
                }
                else
                {
                    printErrorFmt("Invalid username or password. Please try again.");
                }
                break;

            case 2:
                printInfoFmt("Exiting the system. Goodbye!");
                disconnectDB();
                return;

            default:
                printErrorFmt("Invalid choice. Enter a valid option.");
        }
    }
}

int main(void)
{
    if(!connectDB())
    {
        printf("Database Connection Failed!\n");
        return 1;
    }

    menu();

    disconnectDB();

    return 0;
}
