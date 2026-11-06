#ifndef CUSTOMER_H
#define CUSTOMER_H
struct Customer {
    int Acc_No;
    int Age;
    char Full_Name[20];
    char Gender;
    char Mo_No[11];
    char Address[100];
    char Aadhar_No[13];
    char Pan_No[11];
    float Current_Balance;
};
void customer_registration(void);
#endif

