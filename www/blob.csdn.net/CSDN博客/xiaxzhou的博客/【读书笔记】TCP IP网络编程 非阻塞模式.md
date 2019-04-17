# 【读书笔记】TCP/IP网络编程 非阻塞模式 - xiaxzhou的博客 - CSDN博客





2017年08月10日 11:36:01[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：170








[http://blog.csdn.net/ithzhang/article/details/8274596](http://blog.csdn.net/ithzhang/article/details/8274596)

非阻塞模式是指：套接字在执行操作时，调用的函数不管操作是否完成都会立即返回的工作模式。

非阻塞套接字在处理同时建立的多个连接等方面具有明显的优势。但是使用过程中有一定的难度。由于函数在操作没有完成后也仍然会返回，为了实现某些功能必须循环调用，直到完成功能为止。因此非阻塞模式会使程序效率非常低。



