#include <stdio.h>
#include "bank.h"
#include "customer.h"
#include "file.h"
#include "transaction.h"
#include "utils.h"

void customer_details(void)
{
    int acc_no;
    struct Customer c;
    if (!read_int("Enter account number: ", &acc_no)) {
        printf("Invalid account number.\n");
        return;
    }
    if (!search_customer(acc_no, &c)) {
        printf("Customer not found.\n");
        return;
    }
    printf("\n========== CUSTOMER DETAILS ==========\n");
    printf("Account Number : %d\n", c.Acc_No);
    printf("Full Name      : %s\n", c.Full_Name);
    printf("Age            : %d\n", c.Age);
    printf("Gender         : %c\n", c.Gender);
    printf("Mobile Number  : %s\n", c.Mo_No);
    printf("Address        : %s\n", c.Address);
    printf("Aadhaar Number : %s\n", c.Aadhar_No);
    printf("PAN Number     : %s\n", c.Pan_No);
    printf("Current Balance: Rs. %.2f\n", c.Current_Balance);
}

void search_account(void)
{
    int choice, acc_no;
    char name[100];
    struct Customer c;
    printf("\nSearch by:\n1. Account Number\n2. Customer Name\n");
    if (!read_int("Enter choice: ", &choice)) {
        printf("Invalid choice.\n");
        return;
    }
    if (choice == 1) {
        if (!read_int("Enter account number: ", &acc_no)) {
            printf("Invalid account number.\n");
            return;
        }
        if (search_customer(acc_no, &c)) {
            printf("Account Number: %d\nName: %s\nBalance: Rs. %.2f\n",
                   c.Acc_No, c.Full_Name, c.Current_Balance);
        } else printf("Customer not found.\n");
    } else if (choice == 2) {
        read_line("Enter customer name or part of name: ", name, sizeof(name));
        if (name[0] == '\0' || !search_customer_by_name(name))
            printf("Customer not found.\n");
    } else printf("Invalid choice.\n");
}

static void delete_account(void)
{
    int acc_no;
    char answer[10];
    struct Customer c;
    if (!read_int("Enter account number to delete: ", &acc_no)) {
        printf("Invalid account number.\n");
        return;
    }
    if (!search_customer(acc_no, &c)) {
        printf("Customer not found.\n");
        return;
    }
    printf("Account holder: %s\n", c.Full_Name);
    read_line("Confirm permanent deletion? (Y/N): ", answer, sizeof(answer));
    if (answer[0] == 'Y' || answer[0] == 'y') {
        if (delete_customer(acc_no))
            printf("Customer account deleted successfully.\n");
        else
            printf("Unable to delete customer account.\n");
    } else printf("Deletion cancelled.\n");
}

void bank_menu(void)
{
    int choice, acc_no;
    struct Customer c;
    do {
        printf("\n========================================\n");
        printf("          BANK MANAGEMENT SYSTEM        \n");
        printf("========================================\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. Balance Inquiry\n");
        printf("6. Customer Details\n");
        printf("7. Search Account\n");
        printf("8. List All Accounts\n");
        printf("9. Delete Account\n");
        printf("10. Transaction History\n");
        printf("11. Exit\n");
        printf("========================================\n");

        if (!read_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Please enter a number from 1 to 11.\n");
            continue;
        }

        switch (choice) {
            case 1:
                customer_registration();
                break;
            case 2:
            case 3:
            case 5:
            case 10:
                if (!read_int("Enter account number: ", &acc_no)) {
                    printf("Invalid account number.\n");
                    break;
                }
                if (!search_customer(acc_no, &c)) {
                    printf("Invalid Account Number / Customer Not Found.\n");
                    break;
                }
                if (choice == 2) credit_money(&c);
                else if (choice == 3) debit_money(&c);
                else if (choice == 5) balance_enquiry(&c);
                else transaction_history(acc_no);
                break;
            case 4:
                transfer_money();
                break;
            case 6:
                customer_details();
                break;
            case 7:
                search_account();
                break;
            case 8:
                (void)list_customers();
                break;
            case 9:
                delete_account();
                break;
            case 11:
                printf("Thank you for using BankEase.\n");
                break;
            default:
                printf("Invalid choice. Please select 1 to 11.\n");
        }
    } while (choice != 11);
}

