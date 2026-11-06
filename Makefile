CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = BankEase
OBJECTS = main.o customer.o file.o transaction.o bank.o utils.o

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

main.o: src/main.c include/bank.h
	$(CC) $(CFLAGS) -c src/main.c -o main.o

customer.o: src/customer.c include/customer.h include/file.h include/utils.h
	$(CC) $(CFLAGS) -c src/customer.c -o customer.o

file.o: src/file.c include/file.h include/customer.h include/transaction.h
	$(CC) $(CFLAGS) -c src/file.c -o file.o

transaction.o: src/transaction.c include/transaction.h include/file.h include/utils.h
	$(CC) $(CFLAGS) -c src/transaction.c -o transaction.o

bank.o: src/bank.c include/bank.h include/customer.h include/file.h include/transaction.h include/utils.h
	$(CC) $(CFLAGS) -c src/bank.c -o bank.o

utils.o: src/utils.c include/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -o utils.o

clean:
	rm -f *.o $(TARGET)

.PHONY: clean
