# java程序内存泄漏排查 - Orisun - 博客园







# [java程序内存泄漏排查](https://www.cnblogs.com/zhangchaoyang/articles/5538628.html)





有个java程序越跑越慢，如何排查？

首先通过jps找到java进程ID。然后top -p [pid]发现内存占用达到了最大值（-Xmx）。开始怀疑是由于频繁Full GC导致的，于是通过jstat -gcutil [pid] 60000查看GC的情况，其中60000表示每隔1分钟输出一次。果然是Full GC次数太多，JVM大部分时间都进行Full GC，而此时JVM会暂停其他一切工作，所以程序运行得非常慢。

那到底的程序的哪一部分导致消耗了这么多的内存呢？通过jmap -histo:live [pid]查看进程中各种类型的对象创建了多少个，以及每种类型的对象占多少内存。当我看到有个对象被创建了5千多万个实例时，我就能定位到是哪儿的问题了。

顺带说一下，通过jmap还可以生成JVM的内存dump文件，命令为jmap -dump:format=b,file=文件名 [pid]，然后通过jhat命令在浏览器中查看，或者通过jvisualvm、eclipse memory analyzer等工具进行查看。使用jhat命令查看的方式为：jhat -J-Xmx1024M [file]，等控制台输出Started HTTP server on port 7000. Server is ready.后在浏览器中输入ip:7000就可以查看各上类中各种实例被创建了多少个。












