CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := main
CFLAGS := -g -Wall -std=c++11 -pthread -I include/

all: main

server:
	@mkdir -p build/server/
	$(CC) $(CFLAGS) -c src/server/server.cpp -o build/server/server.o

main: server
	@mkdir -p build/server/
	$(CC) $(CFLAGS) build/server/server.o src/main.cpp -o main

clean:
	$(RM) -r $(BUILDDIR)/* $(TARGET)

.PHONY: clean
