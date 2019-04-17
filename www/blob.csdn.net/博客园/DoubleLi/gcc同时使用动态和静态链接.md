# gcc同时使用动态和静态链接 - DoubleLi - 博客园






场景是这样的。我在写一个Nginx模块，该模块使用了MySQL的C客户端接口库libmysqlclient，当然mysqlclient还引用了其他的库，比如libm, libz, libcrypto等等。对于使用mysqlclient的代码来说，需要关心的只是mysqlclient引用到的动态库。大部分情况下，不是每台机器都安装有libmysqlclient，所以我想把这个库静态链接到Nginx模块中，但又不想把mysqlclient引用的其他库也静态的链接进来。
　　我们知道gcc的-static选项可以使链接器执行静态链接。但简单地使用-static显得有些’暴力’，因为他会把命令行中-static后面的所有-l指明的库都静态链接，更主要的是，有些库可能并没有提供静态库（.a），而只提供了动态库（.so）。这样的话，使用-static就会造成链接错误。

之前的链接选项大致是这样的，

|1|CORE_LIBS="$CORE_LIBS -L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -L/usr/lib64 -lssl -lcrypto"|
|----|----|


修改过是这样的，

|1 2|CORE_LIBS="$CORE_LIBS -L/usr/lib64/mysql -Wl,-Bstatic -lmysqlclient \ -Wl,-Bdynamic -lz -lcrypt -lnsl -lm -L/usr/lib64 -lssl -lcrypto"|
|----|----|


　　其中用到的两个选项：-Wl,-Bstatic和-Wl,-Bdynamic。这两个选项是gcc的特殊选项，它会将选项的参数传递给链接器，作为链接器的选项。比如-Wl,-Bstatic告诉链接器使用-Bstatic选项，该选项是告诉链接器，对接下来的-l选项使用静态链接；-Wl,-Bdynamic就是告诉链接器对接下来的-l选项使用动态链接。下面是man gcc对-Wl,option的描述，


-Wl,option Pass option as an option to the linker. If option contains commas, it is split into multiple options at the commas. You can use this syntax to pass an argument to the option. For example, -Wl,-Map,output.map passes -Map output.map to the linker. When using the GNU linker, you can also get the same effect with -Wl,-Map=output.map.

下面是man ld分别对-Bstatic和-Bdynamic的描述，


-Bdynamic -dy -call_shared Link against dynamic libraries. You may use this option multiple times on the command line: it affects library searching for -l options which follow it. -Bstatic -dn -non_shared -static Do not link against shared libraries. You may use this option multiple times on the command line: it affects library searching for -l options which follow it. This option also implies --unresolved-symbols=report-all. This option can be used with -shared. Doing so means that a shared library is being created but that all of the library's external references must be resolved by pulling in entries from static libraries.

　　值得注意的是对-static的描述：-static和-shared可以同时存在，这样会创建共享库，但该共享库引用的其他库会静态地链接到该共享库中。









