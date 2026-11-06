#ifndef FILE_H
#define FILE_H
#include "customer.h"
#include "transaction.h"
int save_customer(const struct Customer *customer);
int search_customer(int acc_no, struct Customer *customer);
int update_customer(const struct Customer *customer);
int list_customers(void);
int delete_customer(int acc_no);
int save_transaction(const struct Transaction *transaction);
int get_next_account_number(void);
int search_customer_by_name(const char *name);
#endif

