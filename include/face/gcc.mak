
libpath=/mnt/hgfs/E/pub/cstd/lib
CC=gcc
tags=libfacefeature.so test_libfacefeature
CC1=/home/yw/ti-processor-sdk-linux-am437x-evm-02.00.02.11/linux-devkit/sysroots/x86_64-arago-linux/usr/bin
CC1=/mnt/hgfs/F/VM/dvsdk/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
CC=$(CC1)  -I/mnt/hgfs/E/code/cstd/include -I/mnt/hgfs/E/code/cstd

all: libfacefeature.c
	$(CC) libfacefeature.c -fPIC -shared -o libfacefeature.so
	$(CC) test_libfacefeature.c -lfacefeature -o test_libfacefeature
	-@cp test_libfacefeature.c libfacefeature*.so test_libfacefeature* $(libpath)
	cd $(libpath) & tar -cvf libfacefeature_am437x.tar.gz *libfacefeature*.* adfasdf.bmp >nul

am437x: libfacefeature.c
	$(CC) libfacefeature.c -fPIC -shared -o libfacefeature.so
	$(CC) test_libfacefeature.c -lfacefeature -o test_libfacefeature
	-@cp test_libfacefeature.c libfacefeature*.so test_libfacefeature* $(libpath)
	cd $(libpath) & tar -cvf libfacefeature_am437x.tar.gz *libfacefeature*.* adfasdf.bmp >nul
	
linux: libfacefeature.c
	$(CC) libfacefeature.c -fPIC -shared -o libfacefeature.so
	$(CC) test_libfacefeature.c -lfacefeature -o test_libfacefeature
	-@cp test_libfacefeature.c libfacefeature.so test_libfacefeature $(libpath)
	cd $(libpath) & tar -cvf libfacefeature.tar.gz *libfacefeature*.* adfasdf.bmp >nul

clean:
	$(RM) *.def *.exe *.dll *.lib

