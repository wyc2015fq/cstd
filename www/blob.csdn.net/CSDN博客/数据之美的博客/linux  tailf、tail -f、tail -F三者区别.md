# linux   tailf、tail -f、tail -F三者区别 - 数据之美的博客 - CSDN博客
2018年01月16日 19:56:48[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：3351
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)

tail -f      等同于--follow=descriptor，根据文件描述符进行追踪，当文件改名或被删除，追踪停止
tail -F     等同于--follow=name  --retry，根据文件名进行追踪，并保持重试，即该文件被删除或改名后，如果再次创建相同的文件名，会继续追踪
tailf        等同于tail -f -n 10（貌似tail -f或-F默认也是打印最后10行，然后追踪文件），与tail -f不同的是，如果文件不增长，它不会去访问磁盘文件，所以tailf特别适合那些便携机上跟踪日志文件，因为它减少了磁盘访问，可以省电
