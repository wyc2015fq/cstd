
# Linux基础：swap基础知识与常用操作 - 知行合一 止于至善 - CSDN博客

2019年03月14日 07:23:45[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：300


本文以CentOS7.4为例，介绍交换（Swap）分区相关的基础知识和常用操作。
# 为什么要使用交换（Swap）分区
回答这个问题首先要需要知道交换分区的作用，使用Swap可以在物理内存不够的情况下，将一部分不经常使用的内存临时保存到一个特定的分区中，然后将此部分内存释放出来，而这个特殊的分区就是Swap分区。需要用到这部分数据的时候再将内容从Swap分区中恢复到内存中即可，类似Windows下的虚拟内存的概念。
## 优点
节省成本是Swap分区采用的重要原因
## 缺点
虽然在设计上已经尽量避免频繁的交换，但会带来特定情况下的性能恶化。
## 原则
交换分区与物理内存以一定比例进行创建，比如一般可采用如下比例：
物理内存：Swap分区 = 1 ：2
# 环境准备
CentOS7.4 ： 最小安装即可
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# cat /etc/redhat-release
```
```python
CentOS Linux release 7.4.1708
```
```python
(
```
```python
Core
```
```python
)
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
# 常见操作
在本节将结合见命令对交换分区进行操作
## 查询交换分区
可以使用如下命令进行交换分区信息的查询
### free命令
使用free命令可以查看交换分区的信息如下
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# free
```
```python
total        used
```
```python
free
```
```python
shared  buff/cache   available
Mem:        4046744      801868     2438688       22148      806188     2804156
Swap:       4063228           0     4063228
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
total： 总量
used：已使用量
free：可用量
### vmstat
使用vmstat可以查看内存和磁盘之间交换的数据量信息，执行示例与说明如下所示
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# vmstat
```
```python
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd
```
```python
free
```
```python
buff  cache   si   so    bi    bo
```
```python
in
```
```python
cs us sy
```
```python
id
```
```python
wa st
 2  0      0 1577092   2104 1716328    0    0     8    64  148  320  1  0 99  0  0
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
swpd: 当前所使用的交换内存的大小
si: 每秒从磁盘中交换到内存的数据量（KB）
so: 每秒从内存中交换到磁盘的数据量（KB）
## swapon命令
可以使用swapon命令查看相关信息，详细如下所示
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# swapon -s
```
```python
Filename				Type		Size	Used	Priority
/dev/dm-1                              	partition	4063228	0	-1
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
### top命令
使用top命令也可查看Swap的信息
## 关闭swap
> 使用命令：swapoff -a
比如在kubernetes的安装时就建议将swap进行关闭，此时即可使用swapoff -a
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# free
```
```python
total        used
```
```python
free
```
```python
shared  buff/cache   available
Mem:        4046744      801868     2438688       22148      806188     2804156
Swap:       4063228           0     4063228
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# swapoff -a
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# free
```
```python
total        used
```
```python
free
```
```python
shared  buff/cache   available
Mem:        4046744      799188     2441648       22152      805908     2806848
Swap:             0           0           0
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
## 设定swap
按照如下顺序即可设定swap
### 创建swap块文件
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# dd if=/dev/zero of=/centos-swap bs=1M count=1024
```
```python
1024+0 records
```
```python
in
```
```python
1024+0 records out
1073741824 bytes
```
```python
(
```
```python
1.1 GB
```
```python
)
```
```python
copied, 1.09255 s, 983 MB/s
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# du -k /centos-swap
```
```python
1048576	/centos-swap
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
### 执行mkswap
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# swapon
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# free
```
```python
total        used
```
```python
free
```
```python
shared  buff/cache   available
Mem:        4046744      749420     1580732       22084     1716592     2861156
Swap:             0           0           0
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# mkswap /centos-swap
```
```python
Setting up swapspace version 1, size
```
```python
=
```
```python
1048572 KiB
no label, UUID
```
```python
=
```
```python
edb672bf-f918-4ca4-b098-26f440e0e4ca
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
### 设定swap
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# swapon /centos-swap
```
```python
swapon: /centos-swap: insecure permissions 0644, 0600 suggested.
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# free
```
```python
total        used
```
```python
free
```
```python
shared  buff/cache   available
Mem:        4046744      750724     1579344       22088     1716676     2859840
Swap:       1048572           0     1048572
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# swapon -s
```
```python
Filename				Type		Size	Used	Priority
/centos-swap
```
```python
file
```
```python
1048572	0	-1
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
## swappiness
swappiness是一个阈值，用于表示使用swap分区的积极程度，详细信息如下所示
|项目|说明|
|---|---|
|设定值|0-100|
|设定文件|/proc/sys/vm/swappiness|
|设定方式|直接将设定值写入设定文件即可|
确认方式1：确认文件
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# cat /proc/sys/vm/swappiness
```
```python
30
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
确认方式2: sysctl命令
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# sysctl -a 2>/dev/null |grep vm.swappiness
```
```python
vm.swappiness
```
```python
=
```
```python
30
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```

