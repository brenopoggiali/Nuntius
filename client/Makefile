CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := main
CFLAGS := -g -Wall -std=c++11 -pthread -I include/

all: main

client:
	@mkdir -p build/client/
	$(CC) $(CFLAGS) -c src/client/client.cpp -o build/client/client.o

main: client
	@mkdir -p build/client/
	$(CC) $(CFLAGS) build/client/client.o src/main.cpp -o main

clean:
	$(RM) -r $(BUILDDIR)/* $(TARGET)

.PHONY: clean
