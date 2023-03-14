CC=gcc
#CFLAGS=
CFLAGS= -Wall -Wextra -Werror 
#SRCE = main.c Encode.c Decode.c HelpfulFunc.c
SRCE = main1.c
OS=$(shell uname)
ifeq ($(OS), Linux)
	FLAGS=-lcheck -lm -lsubunit -lrt -lpthread -lm
	LEAKS = valgrind --leak-check=full
else
	FLAGS =-lcheck
	LEAKS = CK_FORK=no leaks --atExit --
endif

all: lab8

lab8:
	$(CC) $(CFLAGS) -g main1.c -lm  -o main.o
	./main.o
	
style: python cppcheck

clean:
	rm -rf *.o 

rebuild: clean all
	valgrind --leak-check=full ./main.o

rrebuild: clean all
	./main.o
python:
	#mv ../../lab7/cpplint.py cpplint.py
	python3 cpplint.py --extensions=c main.c

cppcheck: 
	cppcheck --enable=all --suppress=missingIncludeSystem ${SRCE} main.h

leaks: lab8
	$(LEAKS) ./main.o

test: lab8
	../test/testlab8-1 ./main.o

