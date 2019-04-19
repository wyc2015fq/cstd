# linux下如何查找函数所在的库文件 - xqhrs232的专栏 - CSDN博客
2016年04月28日 22:25:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1455
原文地址::[http://blog.csdn.net/iyangyi/article/details/5873375](http://blog.csdn.net/iyangyi/article/details/5873375)
相关文章
1、[linux 内核（驱动）常用函数](http://blog.csdn.net/stephen_yin/article/details/6576379)----[http://blog.csdn.net/stephen_yin/article/details/6576379](http://blog.csdn.net/stephen_yin/article/details/6576379)
1、man htons，里面讲的是arpa/inet.h；
2、假设你用的是RPM类型的系统：rpm -qa --filesbypkg | grep arpa/inet.h，你就看见了一个或者几个包提供这个文件，现在我看到的是glibc-headers（RHEL5）
3、貌似这个glibc-headers只是个包含头文件的软件包，看看其他类似的软件包是什么：rpm -qa | grep glibc
4、我看见了glibc，glibc-devel，glibc-common，觉得glibc-devel肯定是提供.a（静态链接）的
5、rpm -q --filesbypkg glibc-devel| grep /.a$ | awk '{print $2}' |xargs grep htons找到所有带htons的静态库文件
6、只有一个：/usr/lib/libc.a
7、如果有好几个的话，用objdump -x看看符号表 
有一些常识或者说平时的积累是必要的，终究常用的库很少。
当我遇到一个函数不知道所属的库的时候，我会先找到头文件所属的包，然后看看那个包（或者相关包）的.a或者.so文件有哪些，然后：
objdump -x 库文件 | grep 函数名
确认函数名是定义而不是引用以后，就知道需要链接这个库了。
