# couchbase swappiness - 三少GG - CSDN博客
2014年09月22日 15:14:23[三少GG](https://me.csdn.net/scut1135)阅读数：954
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

[[Doc'd] Swap usage shoots up with view querying or indexing](http://www.couchbase.com/issues/browse/MB-7737)
<para> 
By default most Linux systems have swappiness set to 60. This will lead to overuse of disk swap with
Couchbase Server. Please follow our current recommendations on swappiness and swap space, see
<xref linkend="couchbase-bestpractice-cloud-swap"/>. 
</para> 
解决办法：** sudo sysctl vm.swappiness=0**
　linux 会使用硬盘的一部分做为SWAP分区，用来进行进程调度--进程是正在运行的程序--把当前不用的进程调成‘等待（standby）‘，甚至‘睡眠（sleep）’，一旦要用，再调成‘活动（active）’，睡眠的进程就躺到SWAP分区睡大觉，把内存空出来让给‘活动’的进程。
　　如果内存够大，应当告诉 linux 不必太多的使用 SWAP 分区， 可以通过修改 swappiness 的数值。swappiness=0的时候表示最大限度使用物理内存，然后才是 swap空间，swappiness＝100的时候表示积极的使用swap分区，并且把内存上的数据及时的搬运到swap空间里面。
　　在ubuntu 里面，默认设置swappiness这个值等于60。
　　!!!! 如果内存较小，而进程调度频繁，硬盘的响动就会大了 !!!!
　　现在一般1个G的内存可修改为10， 2个G的可改为5， 甚至是0。具体这样做：
　　1.查看你的系统里面的swappiness
**　　$ cat /proc/sys/vm/swappiness　　不出意外的话，你应该看到是 60　　2.修改swappiness值为10　$ sudo sysctl vm.swappiness=10**
　　但是这只是临时性的修改，在你重启系统后会恢复默认的60，为长治久安，还要更进一步：
$ sudo gedit /etc/sysctl.conf
　　在这个文档的最后加上这样一行:
　　vm.swappiness=10
　　然后保存，重启。ok，你的设置就永久生效了。
﻿﻿
