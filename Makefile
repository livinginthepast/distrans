CC=cc
CFLAGS=-c -Wall

all: server

server: build_dir
	$(CC) $(CFLAGS) server.c -o build/server

build_dir:
	mkdir -p build

clean:
	rm -rf build

