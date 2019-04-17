# static link:关于gcc连接静态库的几种方式 - DoubleLi - 博客园






开发一个应用程序不可避免要使用多个第三方库(library).
默认情况下，gcc采用动态连接的方式连接第三方库,比如指定-lpng，连接程序就会去找libpng.so。

gcc提供了一个-static参数,可以改变gcc默认的连接方式，GNU官网上关于gcc连接选项的手册《3.14 Options for Linking》中有说明：如下



这个-static选项是个大杀器，指定了这个选项，gcc在连接时对项目所有的依赖库都尝试去搜索名为lib<name>.a的静态库文件，完成静态连接，如果找不到就报错了。这里指的所有是不仅指我们常用的第三方库比如jpeg,png,opencv,zlib,...,还包括gcc编译器自带的库libgcc,libstdc++,libc,libm...，总之就是linux kernal之外的所有库。而且还要包括所有被间接引用的第三方库，比如png这个库在编译时还用到了zlib,那么静态连接png的时候，就要带上zlib的库：-lpng -lz,

这可麻烦大了,要把这些东西全静态连接，这得有多大？呵呵，这事儿我干过，十几兆字节总是有的，取决你的程序用到多少第三方库。
这种全静态连接有啥用处呢？也有用，就是你的程序自带干粮，只需要一个linux kernal就能跑了。

但是实际应用中，我们绝大多数应用场景不需要这么做，即使在嵌入式系统中也不一定必要，尤其是嵌入式系统的存储容量受限，这么一只大象装都装不下。所以我们大多数情况下需要有选择的进行静态编译，-static并不适合。

如何有选择的进行静态编译呢？
最简单的方式直接在连接参数中以全路径指定连接库就好了： your/path/lib<name>.a
但这种形式对管理结构简单而且自己写Makefile的小型项目还好，当一个项目结构复杂，有时需要静态连接有时需要动态连接，这种频繁的修改编译脚本的方式，可维护性就太差了。

-Bstatic
gnu的连接程序ld提供了一个-Bstatic选项用于对指定的库静态连接，ld的官方手册《2.1 Command Line Options》有说明，如下：



大意就是-Bstatic参数指定对跟在它后面的所有库执行静态连接,如下就指定LDFLAGS对png这个库静态连接，ld会自动去搜索libpng.a

-Bstatic -lpng -lz
1
-l:filename
如果你觉得上面一种静态连接方式不适合你，可以看看ld的官方手册《2.1 Command Line Options》中关于-l参数的说明，如下：



注意上面的说明中红框标注的内容，如果-l:filename格式指定一个文件名，连接程序直接去找这个文件名了，不会再像使用-lname时将name扩展成lib<name>.a格式的文件名.
所以使用 -l:libpng.a这样的形式来指定连接库，就指定了静态连接png库。
当然如果库的位置不在gcc默认搜索路径中，要用-L参数另外指定搜索库的路径，否则连接程序不知道该从哪里找到filename。

-L/your/library/path -l:libmylib.a
1
顺便贴出ld的官方手册《2.1 Command Line Options》中关于-L参数的说明



参考资料
以上内容只是我对最近做项目编译的一些总结和体会，可能会有所遗漏，权威资料还请参见gcc的官方文档
《3.14 Options for Linking》
《2.1 Command Line Options》
--------------------- 
作者：10km 
来源：CSDN 
原文：https://blog.csdn.net/10km/article/details/83239949 
版权声明：本文为博主原创文章，转载请附上博文链接！









