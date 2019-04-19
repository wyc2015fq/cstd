# DM6467之视频采集(Linux)下MMAP - maopig的专栏 - CSDN博客
2012年01月08日 17:54:31[maopig](https://me.csdn.net/maopig)阅读数：2238标签：[linux																[buffer																[function																[disk																[文档																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=disk&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
做ｄｓｐ６４６７是在linux下做视频采集，仅ARM端
一共有三种视频采集方式：
1）使用read、write方式：直接使用 read 和 write 函数进行读写。这种方式最简单，但是这种方式会在用户空间和内核空间不断拷贝数据 ，同时在用户空间和内核空间占用 了 大量内存，效率不高。
2）内存映射方式（mmap）：把设备里的内存映射到应用程序中的内存控件，直接处理设备内存，这是一种有效的方式。上面的mmap函数就是使用这种方式。
3）用户指针模式：内存由用户空间的应用程序分配，并把地址传递到内核中的驱动程序，然后由 v4l2 驱动程序直接将数据填充到用户空间的内存中。这点需要在v4l2_requestbuffers里将memory字段设置成V4L2_MEMORY_USERPTR。
　　第一种方式效率是最低的，后面两种方法都能提高执行的效率，但是对于mmap 方式，文档中有这样一句描述 –Remember the buffers are allocated in physical memory， as opposed to virtual memory which can be swapped out to disk。 Applications should free the buffers as soon as possible with the munmap （） function
 。（使用mmap方法的时候，buffers相当于是在内核空间中分配的，这种情况下，这些buffer是不能被交换到虚拟内存中，虽然这种方法不怎么影响读写效率，但是它一直占用着内核空间中的内存，当系统的内存有限的时候，如果同时运行有大量的进程，则对系统的整体性能会有一定的影响。）
　　所以，对于三种视频采集方式的选择，推荐的顺序是 userptr 、 mmap 、 read-write 。当使用 mmap 或 userptr 方式的时候，有一个环形缓冲队列的概念，这个队列中，有 n 个 buffer ，驱动程序采集到的视频帧数据，就是存储在每个 buffer 中。在每次用
 VIDIOC_DQBUF 取出一个 buffer ，并且处理完数据后，一定要用 VIDIOC_QBUF 将这个 buffer 再次放回到环形缓冲队列中。环形缓冲队列，也使得这两种视频采集方式的效率高于直接 read/write 。
但在用ｍｍａｐ是要注意关闭线程是才可访问
            
