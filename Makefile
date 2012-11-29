CC=cc
CFLAGS=-Wall

all: server

server: clean build_dir
	$(CC) $(CFLAGS) distrans.c -o build/distrans
	chmod 0744 build/distrans

build_dir:
	mkdir -p build

clean:
	rm -rf build

