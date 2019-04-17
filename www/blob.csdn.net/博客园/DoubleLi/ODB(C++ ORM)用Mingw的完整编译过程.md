# ODB(C++ ORM)用Mingw的完整编译过程 - DoubleLi - 博客园






用mingw官方的GCC4.7.2编译libodb后,并用odb compiler对hello示例生成odb的"包裹"代码,编译链接总是不能通过,下面是编译example/hello报错的信息如下:
libodb-sqlite-2.3.0/odb/sqlite -o driver.exe driver.o person-odb.o  -lodb-sqlite -lboost_date_time -lboost_system -lodb
libtool: link: g++ -I/d/odb-sqlite/sqlite-amalgamation-3080200 -mthreads -o .libs/driver.exe driver.o person-odb.o  -L/d/odb-sqlite/sqlite-amalgamation-3080200 -L/d/odb-sqlite/libodb-2.3.0/odb -L/d/odb-sqlite/libodb-sqlite-2.3.0/odb/sqlite  /d/odb-sqlite/libodb-sqlite-2.3.0/odb/sqlite/.libs/libodb-sqlite.a -lsqlite3 -lboost_date_time -lboost_system /d/odb-sqlite/libodb-2.3.0/odb/.libs/libodb.dll.a -mthreads -L/usr/local/lib
driver.o:driver.cxx:(.text$_ZN3odb6sqlite11query_paramC2EPKv[__ZN3odb6sqlite11qu
ery_paramC2EPKv]+0x1b): undefined reference to `_imp___ZTVN3odb6sqlite11query_pa
ramE'
driver.o:driver.cxx:(.text$_ZN3odb6sqlite10query_baseC1EPKcS3_[__ZN3odb6sqlite10
query_baseC1EPKcS3_]+0x5f): undefined reference to `_imp___ZN3odb6sqlite10query_
base6appendEPKcS3_'
d:/lbe/mingw/bin/../lib/gcc/mingw32/4.7.2/../../../../mingw32/bin/ld.exe: driver
.o: bad reloc address 0x5f in section `.text$_ZN3odb6sqlite10query_baseC1EPKcS3_
[__ZN3odb6sqlite10query_baseC1EPKcS3_]'
collect2.exe: error: ld returned 1 exit status
make[1]: *** [driver.exe] Error 1
make[1]: Leaving directory `/d/lbe/oscode/odb-sqlite/odb-examples-2.3.0/hello'
make: *** [all] Error 2

尝试多种方法不能解决,发现在odb compiler目录中自带一个mingw编译器,这个编译版本是GCC4.7.3的,gcc -v的输出信息如下:
Using built-in specs.
COLLECT_GCC=D:\wxwidgets\wxcode\odb-2.3.0-i686-windows\mingw\bin\gcc
.exe
COLLECT_LTO_WRAPPER=d:/wxwidgets/wxcode/odb-2.3.0-i686-windows/mingw
/bin/../libexec/gcc/i686-w64-mingw32/4.7.3/lto-wrapper.exe
Target: i686-w64-mingw32
Configured with: ../gcc/configure --host i686-w64-mingw32 --target i686-w64-ming
w32 --prefix=/mingw --with-sysroot=/mingw --enable-languages=c,c++ --enable-shar
ed --enable-threads=posix --disable-libssp --disable-libgomp --disable-graphite
--disable-multilib --enable-libstdcxx-time --enable-fully-dynamic-string --disab
le-dw2-exceptions --enable-sjlj-exceptions --disable-libstdcxx-pch --disable-lib
stdcxx-debug --disable-rpath --disable-win32-registry --disable-nls --disable-we
rror --with-pkgversion='ODB special' --with-bugurl=http://www.codesynthesis.com/
products/odb/ --enable-static-plugin --with-stage1-libs=libplugin-stub.a CFLAGS=
-O2 CXXFLAGS=-O2 LDFLAGS=-s
Thread model: posix
gcc version 4.7.3 20130209 (prerelease) (ODB special)
?
而Mingw GCC4.7.2的gcc -v的输出信息如下:


Using built-in specs.
COLLECT_GCC=d:\mingw\bin\gcc.exe
COLLECT_LTO_WRAPPER=d:/mingw/bin/../libexec/gcc/mingw32/4.7.2/lto-wrapper.ex
e
Target: mingw32
Configured with: ../gcc-4.7.2/configure --enable-languages=c,c++,ada,fortran,obj
c,obj-c++ --disable-sjlj-exceptions --with-dwarf2 --enable-shared --enable-libgo
mp --disable-win32-registry --enable-libstdcxx-debug --disable-build-poststage1-
with-cxx --enable-version-specific-runtime-libs --build=mingw32 --prefix=/mingw
Thread model: win32
gcc version 4.7.2 (GCC)




现在则使用odb compiler自带的mingw完成所有的编译链接过程,并记录编译的所有过程,

步骤是:

(1)编译sqlite

gcc -c sqlite3.c

ar crvs libsqlite3.a sqlite3.o

(2)编译libodb

./configure & make 即可

(3)编译libodb-sqlite

./configure CXXFLAGS=-I/d/odb-sqlite/sqlite-amalgamation-3080200 LDFLAGS=-L/d/odb-sqlite/sqlite-amalgamation-3080200 --with-libodb=/d/odb-sqlite/libodb-2.3.0



(4)编译libodb-example

./configure CXXFLAGS=-I/d/odb-sqlite/sqlite-amalgamation-3080200 LDFLAGS=-L/d/odb-sqlite/sqlite-amalgamation-3080200 --with-libodb=/d/odb-sqlite/libodb-2.3.0 ODB=/d/odb-2.3.0-i686-windows/bin/odb.exe --with-database=sqlite --with-libodb-sqlite=/d/odb-sqlite/libodb-sqlite-2.3.0

make
在example/hello下启动测试的程序,并用SqliteAdmin这个软件查看数据库文件中的数据,是正确的.

$ ./driver --database hello.db --create
Hello, John Doe!
Hello, Jane Doe!


count  : 3
min age: 31
max age: 33









