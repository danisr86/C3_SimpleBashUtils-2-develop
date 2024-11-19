CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11

all: s21_cat test clean

s21_cat: 
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test: 
	@echo "CAT _____________________"
	./s21_cat text1.txt > cat1
	cat text1.txt > cat2
	diff -y cat1 cat2

	@echo "-b ______________________"
	./s21_cat -b text1.txt > catb1
	cat -b text1.txt > catb2
	diff -y catb1 catb2

	@echo "--number-nonblank ______________________"
	./s21_cat --number-nonblank text1.txt > number-nonblank
	diff -y number-nonblank catb2

	@echo "-e ______________________"
	./s21_cat -e text1.txt > cate1
	cat -e text1.txt > cate2
	diff -y cate1 cate2

	@echo "-E ______________________"
	./s21_cat -E text1.txt > catE1
	diff -y catE1 E

	@echo "-n ______________________"
	./s21_cat -n text1.txt > catn1
	cat -n text1.txt > catn2
	diff -y catn1 catn2

	@echo "--number ______________________"
	./s21_cat --number text1.txt > number
	diff -y number catn2

	@echo "-s ______________________"
	./s21_cat -s text1.txt > cats1
	cat -s text1.txt > cats2
	diff -y cats1 cats2

	@echo "--squeeze-blank ______________________"
	./s21_cat --squeeze-blank text1.txt > squeeze-blank
	diff -y squeeze-blank cats2

	@echo "-t ______________________"
	./s21_cat -t text1.txt > catt1
	cat -t text1.txt > catt2
	diff -y catt1 catt2

	@echo "-T ______________________"
	./s21_cat -T text1.txt > catt_T2
	diff -y catt_T2 T


clean:
	rm -rf *.o s21_cat cat1 cat2 catb1 catb2 number-nonblank cate1 cate2 catE1 catE2 catn1 catn2 number cats1 cats2 squeeze-blank catt1 catt2 catt_T2

rebuild: clean all
