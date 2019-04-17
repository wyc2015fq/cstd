# 解决 liblog4cpp.a: could not read symbols: Bad value - DoubleLi - 博客园






将 liblog4cpp.a 链接进一个 so， 编译时出现 ：

liblog4cpp.a(RollingFileAppender.o): relocation R_X86_64_32 against `a local symbol' can not be used when making a shared object; recompile with -fPIC
1
重新编译log4cpp

$ ./configure --enable-shared CFLAGS=-fPIC CXXFLAGS=-fPIC 
$ make -j8

from：https://blog.csdn.net/kevin_darkelf/article/details/56016208 











