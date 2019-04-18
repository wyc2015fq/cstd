# netstat、tcpdump和gdb命令的配合使用debug多线程网络程序 - Likes的博客 - CSDN博客
2019年03月11日 16:48:38[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：11标签：[netstat																[tcpdump																[gdb																[网络																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=gdb&t=blog)](https://so.csdn.net/so/search/s.do?q=tcpdump&t=blog)](https://so.csdn.net/so/search/s.do?q=netstat&t=blog)
个人分类：[网络](https://blog.csdn.net/songchuwang1868/article/category/8235292)
不熟练某个命令时记得使用man查找手册
# 一、netstat
**netstat命令会截取执行命令那一瞬间的网络状态**
在调试网络bug时一般最先使用netstat查看网络状态，主要关注TCP状态是否正确，接收和发送区是否有大量数据堆积。
-t（tcp）显示tcp相关的网络信息
-p（process）  显示占用相应端口的程序名字以及进程号PID
-a（all） 显示所有状态的端口包括listening和non-listening状态的。默认不显示listening状态的端口
-n（numeric） 显示IP地址表示主机，而不是主机名
所以常如下用命令获取可执行程序a.out基本网络信息：
```bash
netstat -pant |grep a.out
```
输出如下：
![](https://img-blog.csdnimg.cn/20190311160246904.png)
第一列显示协议
第二列是接收队列的字节数，即还没有被用户从协议栈中拷贝出来的数据的字节数
第三列是发送队列的字节数，即发送后还没有收到ACK的数据的字节数
如果是listening状态的话，第二、第三列显示的都是堆积syn，也是就是还没有被accept处理的syn数
第四列是发送端的IP和端口
第五列是接收端的IP和端口
第六列是状态
![](https://img-blog.csdnimg.cn/20190311160917818.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==,size_16,color_FFFFFF,t_70)
第七列是进程号和进程名字
# 二、tcpdump
在使用完netstat查看网络状态后，使用tcpdump抓包。tcpdump会抓取聪明执行到现在为止符合条件的所有报文。通过分析报文可以调试网络协议上是否有问题
[https://www.cnblogs.com/maifengqiang/p/3863168.html](https://www.cnblogs.com/maifengqiang/p/3863168.html)
一般使用
`sudo tcpdump tcp -i lo port 40008`
tcp表示要抓包的协议
-i 后指定网卡，lo是本地环路
port指定端口
# 三、gdb
当初步定位问题后，可能需要进到相应的代码中进行查看，使用gdb attach 40008（pid）将gdb附着在进程上进行调试
使用bt查看堆栈，可以判断网络程序卡在了哪个地方，也可以打断点一步一步调试。
需要注意的是这种方式是将gdb附着在主线程上，如果想附着到子线程中，可以先使用命令获取子线程列表：
```bash
ps -T -p <pid>
```
也可以使用命名查看各个线程占用资源的情况：
`top -H`
之后使用gdb attach 附着线程。在Linux中线程本质上还是进程，所以线程的id和进程的id并无冲突，可以直接附着。
查看线程的方法详见如下链接
[https://www.cnblogs.com/EasonJim/p/8098217.html](https://www.cnblogs.com/EasonJim/p/8098217.html)

