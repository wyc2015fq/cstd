# Linux下文件描述符 - weixin_33985507的博客 - CSDN博客
2016年07月06日 17:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

# http://blog.csdn.net/kumu_linux/article/details/7877770
文件描述符是一个简单的整数，用以标明每一个被进程所打开的文件和socket。第一个打开的文件是0，第二个是1，依此类推。Unix操作系统通常给每个进程能打开的文件数量强加一个限制。更甚的是，unix通常有一个系统级的限制。在UNIX/Linux平台上，对于控制台(Console)的**标准输入**（0），**标准输出**（1），**标准错误**（2）输出也对应了三个文件描述符。
对于squid，因为**squid** 的工作方式，文件描述符的限制可能会极大的影响性能。当squid 用完所有的文件描述符后，它不能接收用户新的连接。也就是说，用完文件描述符导致拒绝服务。直到一部分当前请求完成，相应的文件和socket 被关闭，squid不能接收新请求。当squid发现文件描述符短缺时，它会发布警告。
对于Apache，当使用了很多虚拟主机，而每个主机又使用了不同的日志文件时，**Apache**可能会遭遇耗尽文件描述符（有时也称为file handles）的困境。 Apache使用的文件描述符总数如下：每个不同的错误日志文件一个、 每个其他日志文件指令一个、再加10～20个作为内部使用。Unix操作系统限制了每个进程可以使用的文件描述符数量。典型上限是64个，但可以进行扩充，直至到达一个很大的硬限制为止（a large hard-limit）。
linux下最大文件描述符的限制有两个方面，一个是用户级的限制，另外一个则是系统级限制。
以下是查看Linux文件描述符的三种方式：
[root@localhost ~]# **sysctl -a | grep -i file-max --color**
fs.file-max = 392036
[root@localhost ~]# **cat /proc/sys/fs/file-max**
392036
[root@localhost ~]# **ulimit -n**
1024
[root@localhost ~]#
系统级限制：sysctl命令和proc文件系统中查看到的数值是一样的，这属于系统级限制，它是限制所有用户打开文件描述符的总和
用户级限制：ulimit命令看到的是用户级的最大文件描述符限制，也就是说每一个用户登录后执行的程序占用文件描述符的总数不能超过这个限制
如何修改文件描述符的值？
1、修改用户级限制
[root@localhost ~]# **ulimit-SHn 10240**
[root@localhost ~]# **ulimit  -n**
10240
[root@localhost ~]#
以上的修改只对当前会话起作用，是临时性的，如果需要永久修改，则要修改如下：
[root@localhost ~]# **grep -vE'^$|^#' /etc/security/limits.conf**
*                hard nofile                  4096
[root@localhost ~]#
//默认配置文件中只有hard选项，soft 指的是当前系统生效的设置值，hard 表明系统中所能设定的最大值
[root@localhost ~]# **grep -vE'^$|^#' /etc/security/limits.conf**
*      hard         nofile       10240
*      soft         nofile      10240
[root@localhost ~]#
// soft<=hard soft的限制不能比hard限制高
2、修改系统限制
[root@localhost ~]# **sysctl -wfs.file-max=400000**
fs.file-max = 400000
[root@localhost ~]# **echo350000 > /proc/sys/fs/file-max  //重启后失效**
[root@localhost ~]# **cat /proc/sys/fs/file-max**
350000
[root@localhost ~]#
//以上是临时修改文件描述符
//永久修改把**fs.file-max=400000**添加到/etc/sysctl.conf中，使用sysctl -p即可
下面是摘自kernel document中关于file-max和file-nr参数的说明
file-max & file-nr:
The kernel allocates file handles dynamically, but as yet it doesn't free them again.
内核可以动态的分配文件句柄，但到目前为止是不会释放它们的
The value in file-max denotes the maximum number of file handles that the Linux kernel will allocate. When you get lots of error messages about running out of file handles, you might want to increase this limit.
file-max的值是linux内核可以分配的最大文件句柄数。如果你看到了很多关于打开文件数已经达到了最大值的错误信息，你可以试着增加该值的限制
Historically, the three values in file-nr denoted the number of allocated file handles, the number of allocated but unused file handles, and the maximum number of file handles. Linux 2.6 always reports 0 as the number of free file handles -- this is not an error, it just means that the number of allocated file handles exactly matches the number of used file handles.
在kernel 2.6之前的版本中，file-nr 中的值由三部分组成，分别为：1.已经分配的文件句柄数，2.已经分配单没有使用的文件句柄数，3.最大文件句柄数。但在kernel 2.6版本中第二项的值总为0，这并不是一个错误，它实际上意味着已经分配的文件句柄无一浪费的都已经被使用了
参考文档：维基百科 http://salogs.com/
