#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void read_line(const char *prompt, char *buffer, int size)
{
    size_t n;
    int ch;
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    n = strlen(buffer);
    if (n > 0 && buffer[n - 1] == '\n')
        buffer[n - 1] = '\0';
    else
        while ((ch = getchar()) != '\n' && ch != EOF) { }
}

int read_int(const char *prompt, int *value)
{
    char line[100], extra;
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL)
        return 0;
    return sscanf(line, " %d %c", value, &extra) == 1;
}

int read_amount(const char *prompt, float *amount)
{
    char line[100], extra;
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL)
        return 0;
    if (sscanf(line, " %f %c", amount, &extra) != 1)
        return 0;
    return *amount > 0.0f;
}

