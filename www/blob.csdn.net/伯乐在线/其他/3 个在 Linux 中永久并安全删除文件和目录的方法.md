# 3 个在 Linux 中永久并安全删除文件和目录的方法 - 文章 - 伯乐在线
原文出处： [Aaron Kili](http://www.tecmint.com/permanently-and-securely-delete-files-directories-linux/)   译文出处：[geekpi](https://linux.cn/article-8123-1.html)
在大多数情况下，我们习惯于使用 `Delete` 键、垃圾箱或 `rm` 命令[从我们的计算机中删除文件](https://linux.cn/article-7954-1.html)，但这不是永久安全地从硬盘中（或任何存储介质）删除文件的方法。
该文件只是对用户隐藏，它驻留在硬盘上的某个地方。它有可能被数据窃贼、执法取证或其它方式来恢复。
假设文件包含密级或机密内容，例如安全系统的用户名和密码，具有必要知识和技能的攻击者可以轻松地[恢复删除文件的副本](https://linux.cn/article-8122-1.html)并访问这些用户凭证（你可以猜测到这种情况的后果）。
在本文中，我们将解释一些命令行工具，用于永久并安全地删除 Linux 中的文件。
### 1、 shred – 覆盖文件来隐藏内容
`shred` 会覆盖文件来隐藏它的内容，并且也可以选择删除它。

Shell
```
$ shred -zvu -n 5 passwords.list
```
在下面的命令中，选项有：

Shell
```
-z - 用零覆盖以隐藏碎片
-v - 显示操作进度
-u - 在覆盖后截断并删除文件
-n - 指定覆盖文件内容的次数（默认值为3）
```
![120646jie3i566n55ze9x5](http://jbcdn2.b0.upaiyun.com/2017/01/73cdc34abe411a815a15be13ee7e527d.png)
*shred – 覆盖文件来隐藏它的内容*
你可以在 `shred` 的帮助页中找到更多的用法选项和信息：

Shell
```
$ man shred
```
### 2、 wipe – 在 Linux 中安全删除文件
`wipe` 命令可以安全地擦除磁盘中的文件，从而不可能[恢复删除的文件或目录内容](https://linux.cn/article-7974-1.html)。
首先，你需要安装 `wipe` 工具，运行以下适当的命令：

Shell
```
$ sudo apt-get install wipe [Debian 及其衍生版]
$ sudo yum install wipe [基于 RedHat 的系统]
```
下面的命令会销毁 private 目录下的所有文件。

Shell
```
$ wipe -rfi private/*
```
当使用下面的标志时：

Shell
```
-r - 告诉 wipe 递归地擦除子目录
-f - 启用强制删除并禁用确认查询
-i - 显示擦除进度
```
![120646e9lw7uwtazwu7u9e](http://jbcdn2.b0.upaiyun.com/2017/01/ca1657c88ed4dab9aedcacd7ac547afd.png)
*wipe – 在 Linux 中安全擦除文件*
注意：`wipe` 仅可以在磁性存储上可以可靠地工作，因此对固态磁盘（内存）请使用其他方法。
阅读 `wipe` 手册以获取其他使用选项和说明：

Shell
```
$ man wipe
```
### 3、 Linux 中的安全删除工具集
secure-delete 是一个安全文件删除工具的集合，它包含用于安全删除文件的 `srm`（secure_deletion）工具。
首先，你需要使用以下相关命令安装它：

Shell
```
$ sudo apt-get install secure-delete [Debian 及其衍生版]
$ sudo yum install secure-delete [基于 RedHat 的系统]
```
安装完成后，你可以使用 `srm` 工具在 Linux 中安全地删除文件和目录。

Shell
```
$ srm -vz private/*
```
下面是使用的选项：

Shell
```
-v – 启用 verbose 模式
-z – 用0而不是随机数据来擦除最后的写入
```
![120647yxpx7t72uao3dd32](http://jbcdn2.b0.upaiyun.com/2017/01/00194b6601fcce220cf3947ebc84a576.png)
*srm – 在 Linux 中安全删除文件*
阅读 srm 手册来获取更多的使用选项和信息：

Shell
```
$ man srm
```
### 4、 sfill -安全免费的磁盘 / inode 空间擦除器
`sfill` 是 secure-deletetion 工具包的一部分，是一个安全免费的磁盘和 inode 空间擦除器，它以安全的方法删除可用磁盘空间中的文件。 `sfill` 会[检查指定分区上的可用空间](https://linux.cn/article-8024-1.html)，并使用来自 `/dev/urandom` 的随机数据填充它。
以下命令将在我的根分区上执行 `sfill`，使用 `-v’ 选项启用 verbose 模式：

Shell
```
$ sudo sfill -v /home/aaronkilik/tmp/
```
假设你创建了一个单独的分区 `/home` 来存储正常的系统用户主目录，你可以在该分区上指定一个目录，以便在其上应用 `sfill`：

Shell
```
$ sudo sfill -v /home/username
```
你可以在 sfill 的手册上看到一些限制，你也可以看到额外的使用标志和命令：

Shell
```
$ man sfill
```
注意：secure-deletetion 工具包中的另外两个工具（`sswap` 和 `sdmem`）与本指南的范围不直接相关，但是，为了将来的使用和传播知识的目的，我们会在下面介绍它们。
### 5、 sswap – 安全 swap 擦除器
它是一个安全的分区擦除器，`sswap` 以安全的方式删除 swap 分区上存在的数据。
警告：请记住在使用 `sswap` 之前卸载 swap 分区！ 否则你的系统可能会崩溃！
要找到交换分区（并检查分页和交换设备/文件是否已经使用，请使用 `swapon` 命令），接下来，使用`swapoff` 命令禁用分页和交换设备/文件（使 swap 分区不可用）。
然后在（关闭的） swap 分区上运行 `sswap` 命令：

Shell
```
$ cat /proc/swaps
$ swapon
$ sudo swapoff /dev/sda6
$ sudo sswap /dev/sda6 #这个命令要花费一些时间，默认要进行 38 遍擦除
```
![120647sklgbrpicbt22iy5](http://jbcdn2.b0.upaiyun.com/2017/01/d4ac5dbc6b957004c6308d65e840979e.png)
*sswap – 安全 swap 擦除器*
阅读 `sswap` 的手册来获取更多的选项和信息：

Shell
```
$ man sswap
```
### 6、 sdmem – 安全内存擦除器
`sdmem` 是一个安全的内存擦除器，其设计目的是以安全的方式删除存储器（RAM）中的数据。
它最初命名为 [smem](https://linux.cn/article-7681-1.html)，但是因为在 Debain 系统上存在另一个包 [smem – 报告每个进程和每个用户的内存消耗](https://linux.cn/article-7681-1.html)，开发人员决定将它重命名为 `sdmem`。

Shell
```
$ sudo sdmem -f -v
```
关于更多的使用信息，阅读 `sdmen` 的手册：

Shell
```
$ man sdmem
```
**推荐阅读：**[在 Linux 系统下使用 PhotoRec & TestDisk 工具来恢复文件](https://linux.cn/article-8122-1.html)。
