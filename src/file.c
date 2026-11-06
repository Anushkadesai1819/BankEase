#include <stdio.h>
#include <string.h>
#include "file.h"

#define CUSTOMER_FILE "data/customers.dat"
#define TRANSACTION_FILE "data/transactions.dat"

int save_customer(const struct Customer *customer)
{
    FILE *fp = fopen(CUSTOMER_FILE, "ab");
    int ok;
    if (fp == NULL) {
        printf("File opening failed: customers.dat\n");
        return 0;
    }
    ok = fwrite(customer, sizeof(*customer), 1, fp) == 1;
    fclose(fp);
    return ok;
}

int search_customer(int acc_no, struct Customer *customer)
{
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    struct Customer temp;
    if (fp == NULL)
        return 0;
    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        if (temp.Acc_No == acc_no) {
            *customer = temp;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int update_customer(const struct Customer *customer)
{
    FILE *fp = fopen(CUSTOMER_FILE, "r+b");
    struct Customer temp;
    if (fp == NULL)
        return 0;
    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        if (temp.Acc_No == customer->Acc_No) {
            if (fseek(fp, -(long)sizeof(temp), SEEK_CUR) != 0 ||
                fwrite(customer, sizeof(*customer), 1, fp) != 1) {
                fclose(fp);
                return 0;
            }
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int list_customers(void)
{
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    struct Customer c;
    int count = 0;
    if (fp == NULL) {
        printf("No customer records found.\n");
        return 0;
    }
    printf("\n%-12s %-19s %-12s\n", "Account No", "Name", "Balance");
    printf("-----------------------------------------------\n");
    while (fread(&c, sizeof(c), 1, fp) == 1) {
        printf("%-12d %-19.19s Rs. %-10.2f\n",
               c.Acc_No, c.Full_Name, c.Current_Balance);
        count++;
    }
    fclose(fp);
    if (count == 0)
        printf("No customer records found.\n");
    return count;
}

int delete_customer(int acc_no)
{
    FILE *in = fopen(CUSTOMER_FILE, "rb");
    FILE *out;
    struct Customer c;
    int found = 0;
    if (in == NULL)
        return 0;
    out = fopen("data/customers.tmp", "wb");
    if (out == NULL) {
        fclose(in);
        return 0;
    }
    while (fread(&c, sizeof(c), 1, in) == 1) {
        if (c.Acc_No == acc_no)
            found = 1;
        else if (fwrite(&c, sizeof(c), 1, out) != 1) {
            fclose(in);
            fclose(out);
            remove("data/customers.tmp");
            return 0;
        }
    }
    fclose(in);
    if (fclose(out) != 0) {
        remove("data/customers.tmp");
        return 0;
    }
    if (!found) {
        remove("data/customers.tmp");
        return 0;
    }
    if (remove(CUSTOMER_FILE) != 0)
        return 0;
    if (rename("data/customers.tmp", CUSTOMER_FILE) != 0)
        return 0;
    return 1;
}

int save_transaction(const struct Transaction *transaction)
{
    FILE *fp = fopen(TRANSACTION_FILE, "ab");
    int ok;
    if (fp == NULL) {
        printf("File opening failed: transactions.dat\n");
        return 0;
    }
    ok = fwrite(transaction, sizeof(*transaction), 1, fp) == 1;
    fclose(fp);
    return ok;
}

int get_next_account_number(void)
{
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    struct Customer c;
    int highest = 100000;
    if (fp == NULL)
        return 100001;
    while (fread(&c, sizeof(c), 1, fp) == 1)
        if (c.Acc_No > highest)
            highest = c.Acc_No;
    fclose(fp);
    if (highest >= 999999999)
        return 0;
    return highest + 1;
}

int search_customer_by_name(const char *name)
{
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    struct Customer c;
    int found = 0;
    if (fp == NULL)
        return 0;
    printf("\n%-12s %-19s %-12s\n", "Account No", "Name", "Balance");
    printf("-----------------------------------------------\n");
    while (fread(&c, sizeof(c), 1, fp) == 1) {
        if (strstr(c.Full_Name, name) != NULL) {
            printf("%-12d %-19.19s Rs. %-10.2f\n",
                   c.Acc_No, c.Full_Name, c.Current_Balance);
            found++;
        }
    }
    fclose(fp);
    return found;
}

