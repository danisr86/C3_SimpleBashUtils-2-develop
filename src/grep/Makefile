CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11

FLAGS = -e -i -v -c -l -n -h -s -o

all: s21_grep test clean

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c -o s21_grep.o

test: s21_grep
	sh tests.sh

clean:
	rm -rf *.o s21_grep output_* expected_* text.txt

rebuild: clean all
