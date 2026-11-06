#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "customer.h"
struct Transaction {
    int Acc_No;
    char Transaction_Type[15];
    float Amount;
    float Balance_After;
    char Date_Time[20];
};
void credit_money(struct Customer *customer);
void debit_money(struct Customer *customer);
void transfer_money(void);
void balance_enquiry(struct Customer *customer);
void transaction_history(int acc_no);
#endif

