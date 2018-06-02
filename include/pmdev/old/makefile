


all: clear test_linux
	@chmod 777 test_linux
	./test_linux

clear:
	-@rm test_linux
	
test_linux:
	gcc -o $@ test_linux.c -lX11 -lpthread `pkg-config --cflags `
	