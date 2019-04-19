# 一体化的Linux系统性能和使用活动监控工具–Sysstat - 文章 - 伯乐在线
原文出处： [Tecmint-Kuldeep Sharma](http://www.tecmint.com/install-sysstat-in-linux/)   译文出处：[Linux Story-天寒](http://www.linuxstory.org/install-sysstat-in-linux/)
在监控系统资源、系统性能和使用活动方面，**Sysstat**的确是一个方便的工具。在我们日常的基本工具中多多少少都用到了Sysstat软件包。它还提供了一个可以利用cron进行计划运行的工具，用以收集系统性能数据和活动记录。
![sysstat](http://jbcdn2.b0.upaiyun.com/2015/12/efee8a98053009c11ff5ae6605fa1dbf.png)
以下是在Sysstat软件包里的工具列表：
### **Sysstat的功能列表：**
- iostat：统计并报告你的设备的CPU状态和I/O状态数据。
- **mpstat**：监控和显示关于CPU的细节信息。
- **pidstat**：统计正在运行的进程/任务的CPU、内存等信息。
- **sar**：保存和报告不同资源（CPU、内存、输入输出、网络、内核等）的详细信息。
- **sadc**：系统活动数据收集器，用于为sar收集后台的数据。
- **sa1**：读取和存储sadc的数据文件的二进制数据。
- **sa2**：和sar协作，用于总结每日报告。
- **Sadf**：以不同的格式（CSV或XML）显示sar生成的数据。
- **Sysstat**：解释sysstat的各种作用。
- **nfsiostat-sysstat**:统计NFS协议的网络文件系统的 I/O状态数据。
- **cifsiostat**：统计CIFS协议的网络文件系统的 I/O状态数据。
2014年6月17日， **SYSSTAT11.0.0**（稳定版）正式发布。它带来了一些新的有趣的功能，如下所示。
pidstat命令得到了新的增强，新增了一些命令选项：
一个是“**-R**”，它可以提供有关策略和任务调度优先级。其次是“**-G**”，可以用来按名称搜索进程并得到所有相应匹配的线程列表。
同时，对sar、sadc、sadf这些命令也在数据文件方面作出了新的增强。现在，使用选项  **–D ** 可以把数据文件重命名为 “**saYYYYMMDD**” 格式，而不是 “**saDD**” ；也可以使用不同的目录，不再仅仅是“/var/log/sa”。我们还能通过设置变量“**SA_DIR**”定义给sa1和sa2使用的新目录。
### **在Linux环境下安装Sysstat**
这个“Sysstat”软件包当然是可以在主要的Linux发行版的软件包仓库中获得并安装。但是，这个从软件库里获得的软件包是缩小版的而且是旧版。所以，这就是为什么我们要在软件包的所在官网下载和安装最新版本（11.0.0）的理由。
首先，请使用下面的链接下载最新版本的sysstat软件包，或者你也可以用wget命令直接在终端下载。
- [http://sebastien.godard.pagesperso-orange.fr/download.html](http://sebastien.godard.pagesperso-orange.fr/download.html)

Shell
```
# wget http://pagesperso-orange.fr/sebastien.godard/sysstat-11.0.0.tar.gz
```
![Download-Sysstat-620x423](http://jbcdn2.b0.upaiyun.com/2015/12/5ac3540f03d35bd6948a3a76de59877f.png)
接下来，提取下载的软件包，然后进入该目录中，开始编译过程。

Shell
```
# tar -xvf sysstat-11.0.0.tar.gz 
# cd sysstat-11.0.0/
```
到这一步时，有两个选择：
a)其一，你可以使用**iconfig命令**（这将让你有为每个参数设置自定义值的自由）。

Shell
```
# ./iconfig
```
![Sysstat-iconfig-Command-620x423](http://jbcdn2.b0.upaiyun.com/2015/12/48b7ea8cf05bddc2006fbf9963f8609d.png)
b)其二，你可以使用标准的**configure**命令命令来定义单个行的选项。也就是运行**./configure –help**命令获得被支持的选项列表。


```
# ./configure --help
```
![Configure-Help-620x425](http://jbcdn2.b0.upaiyun.com/2015/12/411345cff4319ecf1e8cc753dee9176d.png)
这里，我们采用标准选项即使用**./configure**命令来编译sysstat软件包。


```
# ./configure
# make
# make install
```
![Configure-Sysstat-620x422](http://jbcdn2.b0.upaiyun.com/2015/12/591eda1b0d0bec7b1711811f9faf5f83.png)
编译完成后，你会看到类似上面的输出结果。现在通过运行一下命令验证sysstat的版本。

Shell
```
# mpstat -V
sysstat version 11.0.0
(C) Sebastien Godard (sysstat  orange.fr)
```
### **在Linux系统中更新Sysstat**
默认情况下，编译安装将会把软件包放在 **/usr/local**”目录。所以，所有的二进制/公共文件都会被安装在 “**/usr/local/bin**”目录。如果你已经安装了sysstat软件包，那么可以直接在 “**/usr/bin**”中找到那些文件。
如果你已经安装了sysstat软件包，并且你的系统中“**$PATH**”变量没有加入“**/usr/local/bin**”，那么你将不会收到有关版本更新的提示。所以在更新之前的编译或删除现有版本过程中，请一定确认好系统的“$PATH”变量中是否已经设置了“/usr/local/bin”或者已经能够给**–prefix **选项配置了“**/usr**”内容。

Shell
```
# yum remove sysstat			[On RedHat based System]
# apt-get remove sysstat		[On Debian based System]
```


Shell
```
# ./configure --prefix=/usr
# make
# make install
```
现在，使用相同的“”mpstat命令和“-V”选项再一次确认更新的版本。

Shell
```
# mpstat -V
sysstat version 11.0.0
(C) Sebastien Godard (sysstat  orange.fr)
```
目前就是这样。接下来，在即将发表的一篇文章里，我将会给大家展示一些实际的操作案例和sysstat命令的用法。敬请期待。同时也欢迎大家在文章下面的评论部分留下你的想法。
