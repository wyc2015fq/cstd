# 静态库不要strip 太厉害 - DoubleLi - 博客园






根据strip的功能表示,strip经常用来去除目标文件中的一些符号表、调试符号表信息,减少包的大小.
我自己做了一函数库,同样的代码生成了一个mylib.so和一个mylib.a文件,之后使用了 strip mylib.so 和
strip mylib.a 这两条命令去除一些符号表和调试符号表信息, 感觉这两个库文件的体积大小确实缩小了,
使用 nm 命令也不会输出这两个库文件的符号清单.
比较怪异的是我分别使用这两个去掉了符号信息的库文件时,效果竟然不一样。
gcc -o ta  test.c  mylib.a  和 gcc -o tso test.c  -L. -lmylib.so 我连接动态库文件时,编译可以通过,
使用静态库文件时,报告错误,在mylib.a中实现的函数找不到。如果不对mylib.a文件进行strip命令的话,
编译可以通过,一切正常。
  关于这个问题,想请教一下各位,发布静态库文件(.a)时,是否有需要使用strip命令去掉了所有的符号列表信息?
有没有什么方法保证使用strip命令后也能正常使用该静态库文件? 为什么对so文件使用了strip命令后不会影响它
和测试程序的链接?而且对静态库文件就会链接不成功?


对应 *.o *.a 加参数. 不给参数的话当日会出事了。
       -g
       -S
       -d
       --strip-debug
           Remove debugging symbols only.

       --strip-unneeded
           Remove all symbols that are not needed for relocation processing.


因为*.o 是relocatable ELF文件。 *.a 算是*.o的集合。所以最多是使用--strip-unneeded参数，符号不能删除的太彻底。

 file dio dio.o 
dio:   ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.15, not stripped

dio.o: ELF 64-bit LSB relocatable, x86-64, version 1 (SYSV), not stripped









