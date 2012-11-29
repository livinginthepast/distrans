CC=cc
CFLAGS=-Wall

all: clean server

server: build_dir
	$(CC) $(CFLAGS) distrans.c -o build/distrans
	chmod 0744 build/distrans

build_dir:
	mkdir -p build

clean:
	rm -rf build

