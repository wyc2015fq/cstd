# gcc/g++ 静态动态库 混链接. - DoubleLi - 博客园






我的环境: centos6 x64. gcc4.4.7





在使用gcc/g++ 编译程序时我们希望指向一些库是使用静态的链接方式. 另外的一些是动态的方式.

我以boost 为例.

如果我们要使用静态库则是这样的:

# g++ main.cpp -lpthread /usr/lib64/libboost_thread.a /usr/lib64/libboost_system.a



静态库直接写路径. 动态前面加-l  这样也可以实现.



但有没有更好的办法呢. 有.

先参考个帖子:[http://stackoverflow.com/questions/3698321/g-linker-force-static-linking-if-static-library-exists](http://stackoverflow.com/questions/3698321/g-linker-force-static-linking-if-static-library-exists)

根据老外这个帖子 照猫画狗

g++   main.cpp -lrt -Wl,-Bstatic -lboost_system -lboost_thread -Wl,-Bdynamic

这样就可以了.



再来看一下老外的这个例子:

g++ main.cpp  -Wl,-Bstatic -lz -lfoo -Wl,-Bdynamic -lbar -Wl,--as-needed

Will link zlib and libfoo as static, and libbar as dynamic . --as-needed will drop any unused dynamic library.





但是这个过程中我用了将近1天时间才搞定的.

因为我刚开始是这样写的:

g++   main.cpp -lrt -Wl,-Bstatic -lboost_system -lboost_thread



这样它就会出现这样的错误提示:

/usr/bin/ld: cannot find -lgcc_s
collect2: ld returned 1 exit status



于是我就按照这个提示搞. 意思就是 libgcc_s.so 没找到. 于是 我就折腾这个. 明明它在.可是就是提示找不到这个错误.

搞了大半天就要放弃了..

我最后想是不是 要在后面加  -Wl,-Bdynamic   其它的库才能默认动态链接.

试了一下果然..

我在后面加了这个就成功了.

g++   main.cpp -lrt -Wl,-Bstatic -lboost_system -lboost_thread -Wl,-Bdynamic  -Wl,--as-needed



-Wl,--as-needed 这个选项是可选的是 这个选项的意思是去除没有使用的动态库



还有一个比较简单的错误就是 :

thread.cpp:(.text+0x1a93): undefined reference to `clock_gettime'
thread.cpp:(.text+0x1b08): undefined reference to `clock_gettime'
thread.cpp:(.text+0x1b6f): undefined reference to `clock_gettime'
thread.cpp:(.text+0x1bd6): undefined reference to `clock_gettime'
thread.cpp:(.text+0x1c3d): undefined reference to `clock_gettime'



一般动态链接时不会出现这个错.  是静态链接时才出这个错.

加上 -lrt 选项就好了.









