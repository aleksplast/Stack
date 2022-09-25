CC := g++ -Wno-pointer-arith
SRC := main.cpp stack.cpp
SRC_TEST := tests.cpp stack.cpp
DIR := C:\Users\USER\Documents\Stack

.PHONY: all clean

main: $(SRC)
	$(CC) $^ -o $(DIR)\$@




