CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11
RM := rm -rf

all: clean clang_format s21_cat

s21_cat: clang_format check_options.o print_file.o cat.o
	$(CC) $(CFLAGS) check_options.o print_file.o s21_cat.o -o s21_cat

cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c

check_options.o: check_options.c
	$(CC) $(CFLAGS) -c check_options.c

print_file.o: print_file.c
	$(CC) $(CFLAGS) -c print_file.c

clang_format:
	clang-format -i *.c *.h
clean:
	$(RM) *.o 
