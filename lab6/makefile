CC = g++
FLAGS = -pedantic -Wall -std=c++17 -Werror -Wno-sign-compare -O2 -lm -g -Wextra
OUTPUT = solution

all:
    $(CC) $(FLAGS) main.cpp TBigInt.cpp -o $(OUTPUT)

clean:
    rm -rf $(OUTPUT)