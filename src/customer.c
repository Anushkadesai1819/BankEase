#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "customer.h"
#include "file.h"
#include "utils.h"

static int digits_only(const char *s)
{
    int i;
    if (s[0] == '\0') return 0;
    for (i = 0; s[i] != '\0'; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}

static int valid_name(const char *s)
{
    int i, letter = 0;
    if (s[0] == '\0' || strlen(s) >= sizeof(((struct Customer *)0)->Full_Name))
        return 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (isalpha((unsigned char)s[i]))
            letter = 1;
        else if (s[i] != ' ' && s[i] != '.' && s[i] != '\'')
            return 0;
    }
    return letter;
}

void customer_registration(void)
{
    struct Customer c = {0};
    char input[150];
    int age, valid, i;

    c.Acc_No = get_next_account_number();
    if (c.Acc_No == 0) {
        printf("Unable to generate account number.\n");
        return;
    }
    printf("\nYour Account Number is: %d\n", c.Acc_No);

    do {
        read_line("Enter your full name: ", input, sizeof(input));
        valid = valid_name(input);
        if (!valid) printf("Name cannot be empty; use letters and spaces.\n");
    } while (!valid);
    strcpy(c.Full_Name, input);

    do {
        if (!read_int("Enter your age (18-100): ", &age)) age = 0;
        if (age < 18 || age > 100) printf("Age must be between 18 and 100.\n");
    } while (age < 18 || age > 100);
    c.Age = age;

    do {
        read_line("Enter gender (F/M/O): ", input, sizeof(input));
        c.Gender = (char)toupper((unsigned char)input[0]);
        valid = strlen(input) == 1 &&
                (c.Gender == 'F' || c.Gender == 'M' || c.Gender == 'O');
        if (!valid) printf("Please enter F, M or O.\n");
    } while (!valid);

    do {
        read_line("Enter mobile number: ", input, sizeof(input));
        valid = strlen(input) == 10 && digits_only(input);
        if (!valid) printf("Mobile number must contain exactly 10 digits.\n");
    } while (!valid);
    strcpy(c.Mo_No, input);

    do {
        read_line("Enter your address: ", input, sizeof(input));
        valid = input[0] != '\0' && strlen(input) < sizeof(c.Address);
        if (!valid) printf("Address cannot be empty.\n");
    } while (!valid);
    strcpy(c.Address, input);

    do {
        read_line("Enter Aadhaar number: ", input, sizeof(input));
        valid = strlen(input) == 12 && digits_only(input);
        if (!valid) printf("Aadhaar number must contain exactly 12 digits.\n");
    } while (!valid);
    strcpy(c.Aadhar_No, input);

    do {
        read_line("Enter PAN number: ", input, sizeof(input));
        valid = strlen(input) == 10;
        if (valid) {
            for (i = 0; i < 5; i++)
                if (!isalpha((unsigned char)input[i])) valid = 0;
            for (i = 5; i < 9; i++)
                if (!isdigit((unsigned char)input[i])) valid = 0;
            if (!isalpha((unsigned char)input[9])) valid = 0;
        }
        if (!valid) printf("Invalid PAN format. Example: ABCDE1234F\n");
    } while (!valid);

    for (i = 0; i < 10; i++)
        input[i] = (char)toupper((unsigned char)input[i]);
    input[10] = '\0';
    strcpy(c.Pan_No, input);

    c.Current_Balance = 500.0f;
    if (save_customer(&c)) {
        printf("\nAccount created successfully!\n");
        printf("Account Number: %d\n", c.Acc_No);
        printf("Initial Balance: Rs. %.2f\n", c.Current_Balance);
    } else {
        printf("Unable to save customer details.\n");
    }
}

