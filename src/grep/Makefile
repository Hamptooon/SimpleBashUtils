CC := gcc
CFLAGS := -Wall -Wextra -Werror
RM := rm -rf

all: clean clang_format s21_grep

s21_grep: clang_format check_options.o print_file.o grep.o
	$(CC) $(CFLAGS) check_options.o print_file.o s21_grep.o -o s21_grep

grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c

check_options.o: check_options.c
	$(CC) $(CFLAGS) -c check_options.c

print_file.o: print_file.c
	$(CC) $(CFLAGS) -c print_file.c

clang_format:
	clang-format -i *.c *.h
clean:
	$(RM) *.o 
