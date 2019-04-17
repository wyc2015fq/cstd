# hostapd移植与使用 - DoubleLi - 博客园






# 介绍

### 版本：[hostapd-2.5.tar.gz](http://w1.fi/releases/hostapd-2.5.tar.gz)

### 下载地址：http://w1.fi/releases/hostapd-2.5.tar.gz

### 依赖：libnl openssl 

# 移植

### libnl移植

wget http://www.infradead.org/~tgr/libnl/files/libnl-1.1.tar.gz 

tar -zxvf libnl-1.1.tar.gz

./configure  CC=arm-hisiv300-linux-gcc --prefix=$PWD/out --host=arm

make clean;
make ;
make install ;

### openssl移植

wget http://openssl.org/source/openssl-1.0.1c.tar.gz

tar -zxvf openssl-1.0.1c.tar.gz

./config --prefix=$PWD/out no-asm shared

修改Makefile

=============================================================================
#CC= gcc
#CFLAG= -fPIC -DOPENSSL_PIC -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H -march=pentium -DL_ENDIAN -DTERMIO -O3 -fomit-frame-pointer -Wall
CC= arm-hisiv300-linux-gcc
CFLAG= -fPIC -DOPENSSL_PIC -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H -march=armv5 -DL_ENDIAN -DTERMIO -O3 -fomit-frame-pointer -Wall
DEPFLAG= -DOPENSSL_NO_EC_NISTP_64_GCC_128 -DOPENSSL_NO_GMP -DOPENSSL_NO_JPAKE -DOPENSSL_NO_MD2 -DOPENSSL_NO_RC5 -DOPENSSL_NO_RFC3779 -DOPENSSL_NO_SCTP -DOPENSSL_NO_STORE
PEX_LIBS= 
EX_LIBS= -ldl
EXE_EXT= 
ARFLAGS= 
#AR= ar $(ARFLAGS) r
#RANLIB= /usr/bin/ranlib
#NM= nm
AR = arm-hisiv300-linux-ar $(ARFLAGS) r
RANLIB= arm-hisiv300-linux-ranlib
NM= arm-hisiv300-linux-nm
PERL= /usr/bin/perl
TAR= tar
TARFLAGS= --no-recursion
MAKEDEPPROG= gcc
LIBDIR=lib

=============================================================================

make clean;

make;

make install;

### hostapd移植

wget http://w1.fi/releases/hostapd-2.5.tar.gz

tar -zxvf hostapd-2.5.tar.gz

将交叉编译的libnl和openssl拷贝至hostapd目录

# ls  ../arm-hisiv300-linux-/ -lh
drwxr-xr-x 4 root root 4.0K May  9 17:39 libnl-1.1
drwxr-xr-x 6 root root 4.0K May  9 15:56 openssl-1.0.1c



cp defconfig .config

修改Makefile

=============================================================================

CC=arm-hisiv300-linux-gcc

CFLAGS += -I ../arm-hisiv300-linux-/libnl-1.1/include
LIBS += -L ../arm-hisiv300-linux-/libnl-1.1/lib -lnl
CFLAGS += -I ../arm-hisiv300-linux-/openssl-1.0.1c/include
LIBS += -L ../arm-hisiv300-linux-/openssl-1.0.1c/lib -lcrypto -lssl
BINDIR := ./out/bin

=============================================================================

make clean

make

make install

# 使用









