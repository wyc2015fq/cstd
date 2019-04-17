# wpa_supplicant移植 - DoubleLi - 博客园







### 移植openssl-0.9.8za

cp ../wpa_supplicant-2.5/patches/openssl-0.9.8za-tls-extensions.patch .
patch -p1 < openssl-0.9.8za-tls-extensions.patch
./config --prefix=$PWD/out no-asm shared
no-asm是在交叉编译过程中不使用汇编代码代码加速编译过程.原因是它的汇编代码是对arm格式不支持的
修改Makefile

================================================================================
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
================================================================================
make clean
make
make install

### 移植wpa_supplicant


wget http://w1.fi/releases/wpa_supplicant-2.5.tar.gz
tar -zxvf wpa_supplicant-2.5.tar.gz
cp defconfig .config
修改Makefile

=================================================================
CC := arm-hisiv300-linux-gcc
ifndef CC
CC=gcc
endif
CFLAGS += -I ../arm-hisiv300-linux-/libnl-1.1/include
CFLAGS += -I ../arm-hisiv300-linux-/openssl-0.9.8za/include
LIBS += -L ../arm-hisiv300-linux-/libnl-1.1/lib -lnl
LIBS += -L ../arm-hisiv300-linux-/openssl-0.9.8za/lib -lssl
LIBDIR := ./out/lib/
INCDIR := ./out/include/
BINDIR := ./out/sbin/
#export LIBDIR ?= /usr/local/lib/
#export INCDIR ?= /usr/local/include/
#export BINDIR ?= /usr/local/sbin/
=============================================================
make clean
make
make install
备注：中途报错
cannot find -lcrypto
解决：修改.config
# Remove IEEE 802.11i/WPA-Personal ASCII passphrase support
# This option can be used to reduce code size by removing support for
# converting ASCII passphrases into PSK. If this functionality is removed, the
# PSK can only be configured as the 64-octet hexstring (e.g., from
# wpa_passphrase). This saves about 0.5 kB in code size.
CONFIG_NO_WPA_PASSPHRASE=y









