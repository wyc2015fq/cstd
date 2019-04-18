# 大数据学习笔记(四) -- linux常用命令（二） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月23日 15:53:34[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：69标签：[管道																[查看文件																[重定向																[xargs																[tee](https://so.csdn.net/so/search/s.do?q=tee&t=blog)](https://so.csdn.net/so/search/s.do?q=xargs&t=blog)](https://so.csdn.net/so/search/s.do?q=重定向&t=blog)](https://so.csdn.net/so/search/s.do?q=查看文件&t=blog)](https://so.csdn.net/so/search/s.do?q=管道&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


**Table of Contents**

[cat](#cat)

[head 和 tail](#head%20%E5%92%8C%C2%A0tail)

[more 和 less](#more%20%E5%92%8C%20less)

[管道操作    |](#%E7%AE%A1%E9%81%93%E6%93%8D%E4%BD%9C%C2%A0%20%C2%A0%20%7C)

[xargs](#xargs)

[重定向输出流  >>](#%E9%87%8D%E5%AE%9A%E5%90%91%E8%BE%93%E5%87%BA%E6%B5%81%C2%A0%20%3E%3E)

[tee](#tee)

# **cat**

读取文件并显示到屏幕上，显示完毕以后会退出到交互行。

![](https://img-blog.csdn.net/20180923144715510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 缺点是如你你是在VMware中操作的话只能看到文件内容的最后一部分内容。

## head 和 tail

head显示文件的前10行，并退出到命令行

![](https://img-blog.csdn.net/20180923144852892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

tail显示文件最后10行，并退出

![](https://img-blog.csdn.net/2018092314494052?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

head 和 tail 可以加上  - num 来设置显示前几行或后几行

```bash
[root@node001 etc]# head -3 profile
# /etc/profile

# System wide environment and startup programs, for login setup
[root@node001 etc]# tail -3 profile

unset i
unset -f pathmunge
```

 tail -f  首先输出文件后10行 ，并持续监控 文件夹最近追加的内容并输出到控制台，ctrl + c  退出

![](https://img-blog.csdn.net/20180923150537605?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 上图为阻塞状态，时刻读取log文件中的最新内容。

我们新建一个窗口来向 log中写入数据

```bash
[root@node001 ~]# echo 'hello linux' >> log
[root@node001 ~]# echo 'hello linux' >> log
[root@node001 ~]# echo 'hello linux' >> log
```

返回看结果

![](https://img-blog.csdn.net/20180923150705311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ctrl + c 退出，并查看log 内容 

![](https://img-blog.csdn.net/20180923150812772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## more 和 less

分页显示文件内容，空格下一页，b 上一页。more翻页到最后会自动退出到交互模式，less不会，less手动按q退出

![](https://img-blog.csdn.net/20180923145207397?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180923145304842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# **管道操作    |**

我们通过管道实现 多个命令的结合， 比如我们查看一个文本的 前 3行，然后再把这前3行内容中的最后一行输出出来，最终达到的效果就是显示一个文本的第3行，管道的原理就是将 一个程序的输出流 放入到一个 程序的输入流当中。

示意如下，显示 profile文件的第四行

```bash
[root@node001 ~]# head -3 profile 
# /etc/profile

# System wide environment and startup programs, for login setup
[root@node001 ~]# head -3 profile | tail -1
# System wide environment and startup programs, for login setup
```

有一点需要注意，并不是所有的程序都会对输入流做处理，比如 ls，ls 是根据参数来去列出指定目录下的内容的。

如下是一个无效示例

```bash
[root@node001 ~]# echo "/" | ls -l
total 36
drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root   72 Sep 21 17:33 log
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
```

我们本想 将echo 输出流中的 /  放入到 ls输入流当中，最终达到如下效果

```bash
[root@node001 ~]# ls -l /
total 90
dr-xr-xr-x.  2 root root  4096 Sep 21 03:27 bin
dr-xr-xr-x.  5 root root  1024 Sep 21 03:27 boot
drwxr-xr-x. 18 root root  3700 Sep 21 06:01 dev
drwxr-xr-x. 61 root root  4096 Sep 21 14:50 etc
drwxr-xr-x.  5 root root  4096 Sep 21 14:50 home
dr-xr-xr-x.  8 root root  4096 Sep 21 03:27 lib
dr-xr-xr-x.  9 root root 12288 Sep 21 03:27 lib64
drwx------.  2 root root 16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root  4096 Sep 23  2011 media
dr-xr-xr-x.  7 root root  4096 Nov 29  2013 mnt
drwxr-xr-x.  2 root root  4096 Sep 21 17:05 opt
dr-xr-xr-x. 87 root root     0 Sep 21 06:00 proc
dr-xr-x---.  4 root root  4096 Sep 21 17:27 root
dr-xr-xr-x.  2 root root 12288 Sep 21 03:27 sbin
drwxr-xr-x.  7 root root     0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root  4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root     0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root  4096 Sep 21 15:32 tmp
drwxr-xr-x. 13 root root  4096 Sep 21 03:26 usr
drwxr-xr-x. 17 root root  4096 Sep 21 03:26 var
```

但显然结果并不是这样，这就是因为ls并有对输入流做处理，ls是根据参数来决定查看哪个目录下的内容，参数和输入流不是一个东西。

# xargs

根据上面的管道命令操作ls命令遇到问题，要想达到效果我们需要 引入 一个命令 xargs

xargs命令可以接受一个输入流，并把接受到的内容放入下一个命令的 参数位。

如下

```bash
[root@node001 ~]# echo "/" | xargs ls -l
total 90
dr-xr-xr-x.  2 root root  4096 Sep 21 03:27 bin
dr-xr-xr-x.  5 root root  1024 Sep 21 03:27 boot
drwxr-xr-x. 18 root root  3700 Sep 21 06:01 dev
drwxr-xr-x. 61 root root  4096 Sep 21 14:50 etc
drwxr-xr-x.  5 root root  4096 Sep 21 14:50 home
dr-xr-xr-x.  8 root root  4096 Sep 21 03:27 lib
dr-xr-xr-x.  9 root root 12288 Sep 21 03:27 lib64
drwx------.  2 root root 16384 Sep 21 03:25 lost+found
drwxr-xr-x.  2 root root  4096 Sep 23  2011 media
dr-xr-xr-x.  7 root root  4096 Nov 29  2013 mnt
drwxr-xr-x.  2 root root  4096 Sep 21 17:05 opt
dr-xr-xr-x. 88 root root     0 Sep 21 06:00 proc
dr-xr-x---.  4 root root  4096 Sep 21 17:27 root
dr-xr-xr-x.  2 root root 12288 Sep 21 03:27 sbin
drwxr-xr-x.  7 root root     0 Sep 21 06:00 selinux
drwxr-xr-x.  2 root root  4096 Sep 23  2011 srv
drwxr-xr-x. 13 root root     0 Sep 21 06:00 sys
drwxrwxrwt.  3 root root  4096 Sep 21 15:32 tmp
drwxr-xr-x. 13 root root  4096 Sep 21 03:26 usr
drwxr-xr-x. 17 root root  4096 Sep 21 03:26 var
```

# **重定向输出流  >>**

我们使用 ls 可以查看目录下内容，并显示出来，如果我们想让输出的内容，不显示在控制台上而直接输出到某个文件当中，我们就可以使用 >> 来改变输出流，如下

```bash
[root@node001 ~]# ll
total 32
drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
[root@node001 ~]# ls -l >> log
[root@node001 ~]# cat log
total 32
drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root    0 Sep 21 18:16 log
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
```

# tee

根据上面的 重定向 操作，展开思路，如果我们既想输出到控制台又想输出到文件，我们可以使用tee命令

如下

```bash
[root@node001 ~]# ls -l | tee log1
total 36
drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root  375 Sep 21 18:16 log
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
[root@node001 ~]# cat log1
total 36
drwxr-xr-x.  2 root root 4096 Sep 21 16:31 abc
-rw-------.  1 root root  900 Sep 21 03:28 anaconda-ks.cfg
-rw-r--r--.  1 root root 8815 Sep 21 03:28 install.log
-rw-r--r--.  1 root root 3384 Sep 21 03:27 install.log.syslog
-rw-r--r--.  1 root root  375 Sep 21 18:16 log
-rw-r--r--.  1 root root 1796 Sep 21 15:38 profile
drwxr-xr-x. 10 root root 4096 Sep 21 16:05 x
```

过程就是 将ls -l 输出的内容放入tee的输入流，tee会把 内容输入到文件当中，并输出到控制台。


