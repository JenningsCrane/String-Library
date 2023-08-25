CC=gcc
STDFLAGS=-std=c11 -Wall -Werror -Wextra 
LINUX_FLAGS=-lpthread -lcheck -pthread -lrt -lm -lsubunit

TARGET=string.a

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o, ${SRC})

OS := $(shell uname -s)
USERNAME=$(shell whoami)

ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

all: clean $(TARGET)

$(TARGET): ${SRC}
	$(CC) -c $(STDFLAGS) $(SRC)
	ar rc $@ $(OBJ)
	ranlib $@
	cp $@ lib$@

clang:
	clang-format -style=Google -n *.c *.h

clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean: clean_lib clean_lib clean_obj
	rm -rf unit_test

