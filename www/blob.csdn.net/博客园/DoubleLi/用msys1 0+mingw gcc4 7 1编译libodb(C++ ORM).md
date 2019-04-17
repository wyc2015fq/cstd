# 用msys1.0+mingw gcc4.7.1编译libodb(C++ ORM) - DoubleLi - 博客园












这里主要使用的是libodb 对sqlite数据库的支持.

(1)下载编译sqlite3,当初编译go-sqlite3的时候下载过sqlite3,就直接拿过来用了.
start msys.bat
gcc -c sqlite3.c -I./inc/sqlite3/
ar crvs libsqlite3.a sqlite3.o
cp libsqlite3.a /d/private/library/lib
cp sqlite3.h /d/private/library/inc/sqlite3
cp sqlite3ext.h /d/private/library/inc/sqlite3
rm libsqlite3.a
rm sqlite3.o


(2)下载libodb到/d/wxwidgets/wxcode/

libodb-2.3.0.zip libodb的库

libodb-sqlite-2.3.0.zip libodb对Sqlite数据库的支持

(3)编译libodb的通用库.
start msys.bat
cd /d/wxwidgets/wxcode/libodb-2.3.0
./configure
make


顺利编译成功.

(4)编译libodb的sqlite支持
start msys.bat
cd /d/wxwidgets/wxcode/libodb-sqlite-2.3.0
./configure


提示libsqlite3没有找到,因为上面对sqlite3的编译后,对头文件与静态库的存放,没有按msys的标准(或POSIX标准或类unix标准)存放.执行./configure --includedir=/d/private/library/inc/sqlite3或./configure --includedir=/d/private/library/inc/sqlite3 --libdir=/d/private/library/lib 还是提示找不到sqlite3, 提示信息如下:
config.lt: creating libtoolchecking for libsqlite3... noconfigure: error: libsqlite3 is not found; consider using CPPFLAGS/LDFLAGS to specify its location


直接在msys中执行下面的命令,不再传递--includedir与--libdir参数,这时对sqlite3的检查顺利通过.
export CXXFLAGS=-I/d/private/library/inc/sqlite3
export LDFLAGS=-L/d/private/library/lib
./configure


提示sqlite3的一个特性没有开启,可能是在编译的时候开启这个特性的控制宏没有定义.或当前代码的版本不支持libodb需要的特性,暂时不理会了.

checking for libsqlite3... yes

configure: WARNING: libsqlite3 is built without sqlite3_unlock_notify support; multi-threaded support will be limited

checking for libodb... no

configure: error: libodb is not found; consider using --with-libodb=DIR



需要指定libodb的路径
./configure --with-libodb=/d/wxwidgets/wxcode/libodb-2.3.0


之后顺利通过configure, 并且make 直接编译成功.
checking for libsqlite3... yesconfigure: WARNING: libsqlite3 is built without sqlite3_unlock_notify support; multi-threaded support will be limitedchecking for libodb... yeschecking whether to use rpath... yes
















