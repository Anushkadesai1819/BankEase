#include <stdio.h>
#include <string.h>
#include <time.h>
#include "transaction.h"
#include "file.h"
#include "utils.h"

static void create_transaction(struct Transaction *t, int acc_no,
                               const char *type, float amount, float balance)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    memset(t, 0, sizeof(*t));
    t->Acc_No = acc_no;
    snprintf(t->Transaction_Type, sizeof(t->Transaction_Type), "%s", type);
    t->Amount = amount;
    t->Balance_After = balance;
    if (tm_info != NULL)
        strftime(t->Date_Time, sizeof(t->Date_Time), "%Y-%m-%d %H:%M:%S", tm_info);
    else
        strcpy(t->Date_Time, "Unavailable");
}

void credit_money(struct Customer *customer)
{
    float amount, old_balance;
    struct Transaction t;
    if (!read_amount("Enter amount to deposit: ", &amount)) {
        printf("Invalid amount. Enter an amount greater than zero.\n");
        return;
    }
    old_balance = customer->Current_Balance;
    customer->Current_Balance += amount;
    if (!update_customer(customer)) {
        customer->Current_Balance = old_balance;
        printf("Unable to update customer record.\n");
        return;
    }
    create_transaction(&t, customer->Acc_No, "DEPOSIT", amount,
                       customer->Current_Balance);
    if (!save_transaction(&t))
        printf("Warning: deposit saved, but transaction history could not be saved.\n");
    else
        printf("Deposit successful. Current balance: Rs. %.2f\n",
               customer->Current_Balance);
}

void debit_money(struct Customer *customer)
{
    float amount, old_balance;
    struct Transaction t;
    if (!read_amount("Enter amount to withdraw: ", &amount)) {
        printf("Invalid amount. Enter an amount greater than zero.\n");
        return;
    }
    if (amount > customer->Current_Balance) {
        printf("Insufficient balance.\n");
        return;
    }
    old_balance = customer->Current_Balance;
    customer->Current_Balance -= amount;
    if (!update_customer(customer)) {
        customer->Current_Balance = old_balance;
        printf("Unable to update customer record.\n");
        return;
    }
    create_transaction(&t, customer->Acc_No, "WITHDRAW", amount,
                       customer->Current_Balance);
    if (!save_transaction(&t))
        printf("Warning: withdrawal saved, but transaction history could not be saved.\n");
    else
        printf("Withdrawal successful. Remaining balance: Rs. %.2f\n",
               customer->Current_Balance);
}

void transfer_money(void)
{
    int from_no, to_no;
    float amount, old_from, old_to;
    struct Customer from, to;
    struct Transaction out, in;

    if (!read_int("Enter source account number: ", &from_no) ||
        !read_int("Enter destination account number: ", &to_no)) {
        printf("Invalid account number.\n");
        return;
    }
    if (from_no == to_no) {
        printf("Source and destination accounts must be different.\n");
        return;
    }
    if (!search_customer(from_no, &from)) {
        printf("Source account not found.\n");
        return;
    }
    if (!search_customer(to_no, &to)) {
        printf("Destination account not found.\n");
        return;
    }
    if (!read_amount("Enter transfer amount: ", &amount)) {
        printf("Invalid amount.\n");
        return;
    }
    if (amount > from.Current_Balance) {
        printf("Insufficient balance.\n");
        return;
    }

    old_from = from.Current_Balance;
    old_to = to.Current_Balance;
    from.Current_Balance -= amount;
    to.Current_Balance += amount;

    if (!update_customer(&from)) {
        printf("Unable to update source account.\n");
        return;
    }
    if (!update_customer(&to)) {
        from.Current_Balance = old_from;
        (void)update_customer(&from);
        printf("Unable to update destination account; source balance restored.\n");
        return;
    }

    create_transaction(&out, from.Acc_No, "TRANSFER OUT", amount,
                       from.Current_Balance);
    create_transaction(&in, to.Acc_No, "TRANSFER IN", amount,
                       to.Current_Balance);

    if (!save_transaction(&out) || !save_transaction(&in))
        printf("Transfer completed, but transaction history saving failed.\n");
    else
        printf("Transfer successful.\n");

    (void)old_to;
    printf("Source balance: Rs. %.2f\n", from.Current_Balance);
    printf("Destination balance: Rs. %.2f\n", to.Current_Balance);
}

void balance_enquiry(struct Customer *customer)
{
    printf("\nCustomer Name: %s\n", customer->Full_Name);
    printf("Account Number: %d\n", customer->Acc_No);
    printf("Current Balance: Rs. %.2f\n", customer->Current_Balance);
}

void transaction_history(int acc_no)
{
    FILE *fp = fopen("data/transactions.dat", "rb");
    struct Transaction t;
    int found = 0;
    if (fp == NULL) {
        printf("No transaction history found.\n");
        return;
    }
    printf("\n%-20s %-14s %-12s %-12s\n",
           "Date & Time", "Type", "Amount", "Balance");
    printf("---------------------------------------------------------------\n");
    while (fread(&t, sizeof(t), 1, fp) == 1) {
        if (t.Acc_No == acc_no) {
            printf("%-20s %-14s %-12.2f %-12.2f\n",
                   t.Date_Time, t.Transaction_Type,
                   t.Amount, t.Balance_After);
            found++;
        }
    }
    fclose(fp);
    if (!found) printf("No transactions found for this account.\n");
}

