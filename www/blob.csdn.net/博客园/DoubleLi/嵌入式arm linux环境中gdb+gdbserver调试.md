# 嵌入式arm linux环境中gdb+gdbserver调试 - DoubleLi - 博客园






一．前言
嵌入式Linux系统中，应用开发过程中，很多情况下，用户需要对一个应用程序进行反复调试，
特别是复杂的程序。采用GDB方法调试，由于嵌入式系统资源有限性，一般不能直接在目标系统上进行调试，
通常采用gdb+gdbserver的方式进行调试。 Gdbserver在目标系统中运行，gdb则在宿主机上运行。
目标系统必须包括gdbserver程序，宿主机也必须安装gdb程序。在此我们还不能直接采用linux
发行版自带的gdb，需要交叉编译gdb和gdbserver。

二．编译gdb和gdbserver工具
下载最新版本的gdb-6.8.tar.gz，可以到ftp://ftp.gnu.org/gnu/gdb下载.
准备好自己的工作目录，如下示
/home/user/gdb
            |--gdb-6.8
            |--obj-gdb
                    |--build
            |--obj-gdbserver
                    |--build
            |--bin

脚本obj-gdb/build内容如下：
#!/bin/sh

../gdb-6.8/configure --target=arm-linux --enable-shared --prefix=/usr/src/arm/gdb/build-gdb --without-x --disable-gdbtk --disable-tui --without-included-regex --without-included-gettext
make

echo "done"


脚本obj-gdbserver/build内容如下：
#!/bin/sh

CC=/opt/toolchain/arm-linux/bin/arm-linux-gcc 
../gdb-6.8/gdb/gdbserver/configure --host=arm-linux --without-included-regex --without-included-gettext

echo "done"

开始编译：
(1) cd obj-gdb
    ./build
(2) cd obj-gdbserver
    ./build
    这时会生成Makefile，修改Makefile如下
    LDFLAGS= -static
    然后make
(3) cd bin
    cp ../obj-gdb/gdb/gdb .
    cp ../obj-gdbserver/gdbserver .
    arm-linux-strip gdbserver
(4) 好了bin目录下就是最终编译出来的gdb+gdbserver了

三．gdbserver使用
(1) 目标机中
    执行命令gdbserver 10.0.12.144:1234 test
    注意test在编译的时候是要加-g选项的
(2) 宿主机中
    gdb test
    再输入命令target remote 10.0.12.143:1234
    接下来就可以gdb的调试了
(3) 说明
    目标机IP：10.0.12.143
    宿主机IP: 10.0.12.144
    端口是随意指定的，只要两端保持一致就行了
    test程序两端也必须相同









