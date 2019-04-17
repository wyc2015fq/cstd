# 编译在arm板上使用的sqlite3的静动态库 - DoubleLi - 博客园






采用的是sqlite-autoconf-3080002.tar.gz 

解压 tar xvf sqlite-autoconf-3080002.tar.gz

进入 cd sqlite-autoconf-3080002/



采用的是 arm-none-linux-gnueabi-gcc(g++)编译。



 ./configure --host=arm-none-linux-gnueabi --prefix=/home/liukang/sqlite-arm-linux LD=arm-none-linux-gnueabi-ld
 make

 make install

生成libsqlite3.a 静态库文件，在/home/liukang/sqlite-arm-linux/lib/ 文件夹下。



./configure --host=arm-none-linux-gnueabi --prefix=/home/liukang/sqlite-arm-linux-so --disable-tcl
 make
 make install

 生成四个文件，在/home/liukang/sqlite-arm-linux-so/lib/ 文件夹下：

libsqlite3.la

libsqlite3.so -> libsqlite3.so.0.8.6

libsqlite3.so.0 -> libsqlite3.so.0.8.6

libsqlite3.so.0.8.6



压缩： arm-none-linux-gnueabi-strip libsqlite3.so.0.8.6

cp  libsqlite3.so 到你工程所需的库文件夹下。
cp  libsqlite3.so.0  到ARM板上的/lib/下

cp bin/sqlite3 到ARM板上的/usr/bin/下

编译时 加上  -lsqlite3，用到的是 libsqlite3.so

在ARM板上运行时，将 libsqlite3.so.0 放到 /lib/ 下即可使用动态库。









