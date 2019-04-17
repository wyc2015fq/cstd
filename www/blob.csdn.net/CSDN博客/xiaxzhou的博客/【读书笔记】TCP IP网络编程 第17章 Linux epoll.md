# 【读书笔记】TCP/IP网络编程  第17章 Linux epoll - xiaxzhou的博客 - CSDN博客





2017年08月10日 11:04:08[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：134








### 第17章 epoll的理解与应用

I/O复用：[http://blog.csdn.net/xiaxzhou/article/details/76944971](http://blog.csdn.net/xiaxzhou/article/details/76944971)
- epoll_create：

创建保存epoll文件描述符的空间 

select方式中为了保存监视对象，直接声明了fd_set变量。但epoll方式下由操作系统负责保存监视对象。因此需要向操作系统请求创建保存空间。
- epoll_ctl:

为了添加删除监视对象文件描述符，select方式需要FD_SET,FD_CLR函数。epoll方式中，通过epoll_ctl函数请求操作系统完成。
- epoll_wait:

声明足够大的epoll_event结构体数组后，传递给epoll_wait函数时，发生变化的文件描述符信息被填入该数组。无需向select函数那样针对所有文件描述符进行循环。



