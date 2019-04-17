# wireshark长时间抓包分多个文件 - DoubleLi - 博客园







**阅读目录**
- [前言](https://www.cnblogs.com/wangqiguo/p/5068602.html#_label0)
- [通过wireshark界面进行设置](https://www.cnblogs.com/wangqiguo/p/5068602.html#_label1)
- [通过命令行工具dumpcap.exe进行抓包](https://www.cnblogs.com/wangqiguo/p/5068602.html#_label2)
- [通过tcpdump的windows版本进行抓包](https://www.cnblogs.com/wangqiguo/p/5068602.html#_label3)


[回到顶部](https://www.cnblogs.com/wangqiguo/p/5068602.html#_labelTop)

### 前言

说一说这个问题的由来，一般使用wireshark不需要长时间抓包的，但是有时候遇到网络通信中非常棘手的问题，例如一个小时出现一次或者几个小时出现一次问题的情况，这种情况下就必须长时间抓包了。但是如果在wireshark中开始抓包之后等上几个小时肯定会出问题，因为这个时候抓包的内容都是存放在内存中的，几个小时的数据包，特别是如果涉及到音视频的数据包是很大的，几个小时可能会达到几个G的大小，这种情况下wireshark会内存溢出，程序直接异常。

这个时候就需要使用wireshark提供的自动分文件存储的功能了。

[回到顶部](https://www.cnblogs.com/wangqiguo/p/5068602.html#_labelTop)

### 通过wireshark界面进行设置

可以在wireshark的界面上进行一些设置之后再开启抓包过程，这个时候wireshark会自动根据我们指定的文件名并加上序号和时间来保存每个文件段了，具体过程如下：

![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_1.png)

选择Capture—Interfaces… 打开网络接口对话框

![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_2.png)

选择要对其进行抓包的网络设备，点击该条目后面的Options按钮

![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_3.png)

在该对话框中就可以设置使用多个文件存储抓到的数据，wireshark会根据我们指定的文件名自动指定每一段的文件名，其名字为 “指定的文件名_序号_日期时间.扩展名”，并且该对话框中可以设置各种文件分段的条件，以及停止抓包的条件，非常灵活。

通过这种方式设置进行多文件保存数据非常方便，但是有一个问题如下：
![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_4.png)

经过长时间抓包之后发现会出现多个 “Closing file!”的对话框，在任务栏上面每一个对话框都会多出一个wireshark的图标，抓包时间越长，文件分段越多该对话框越多，最终会占满整个任务栏。并且只能通过鼠标在任务栏wireshark的图标上右键关闭。通过google搜索发现这是wireshark的一个bug，没有好的解决办法。所以推荐使用命令行的方式进行长时间抓包。

[回到顶部](https://www.cnblogs.com/wangqiguo/p/5068602.html#_labelTop)

### 通过命令行工具dumpcap.exe进行抓包

在wireshark安装之后，在其安装目录下会有一个dumpcap.exe的命令行工具，实际上wireshark的界面程序也是使用这个命令行工具工作的，我们可以不需要使用界面，而直接使用该命令行工具进行抓包工作，其命令如下：

![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_1.%E5%91%BD%E4%BB%A4%E8%A1%8C.png)

命令的含义解释如下：
dumpcap.exe -i \Device\NPF_{845F9D1E-8F0B-4991-9F9A-C55D107A046B} -w d:\000\ddd.pcap -b filesize:50000

dumpcap.exe在wrieshark的安装根目录就可以看到，其中：

-i 表示指定捕获的网卡设备，这里指定的是网卡设备的标识，是一个字符串，可以在wireshark界面程序中查看，如下：
![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_2.%E6%9F%A5%E7%9C%8B%E7%BD%91%E5%8D%A1%E8%AE%BE%E5%A4%87%E7%9A%84%E6%A0%87%E8%AF%86.png)

-w 表示保存的路径以及文件名，如果是分文件保存，则会自动命名为ddd_00001_20151221213115.pcap 的形式，也就是“filename_序号_时间.扩展名”。

-b filesize:N  表示指定每个文件的大小是NKB，如上50000表示 50000 KB，也就是50M。

通过命令行的方式抓包长时间运行没有任何问题，推荐使用这种方式。

[回到顶部](https://www.cnblogs.com/wangqiguo/p/5068602.html#_labelTop)

### 通过tcpdump的windows版本进行抓包

通过dumpcap.exe命令行的方式进行抓包，可以解决文章开头提出的问题，但是dumpcap.exe该工具只有在安装wireshark之后才会有，该程序的运行也依赖wireshark安装目录下的其他的dll，如果不希望安装wireshark这么繁琐，可以选择使用tcpdump的windows版本，只有一个exe程序没有其他依赖，tcpdump.exe不是wireshark的一部分，需要单独下载，[tcpdump windows版本下载地址](http://files.cnblogs.com/files/wangqiguo/tcpdump_windows.zip)

![1](https://images.cnblogs.com/cnblogs_com/wangqiguo/770150/o_1.tcpdump%E5%91%BD%E4%BB%A4%E8%A1%8C%E6%8A%93%E5%8C%85.png)

通过tcpdump –D 命令可以列出所有网络设备列表，注意该列表中的网络设备标识与我们在wireshark的设备属性中拷贝的设备标识有一点区别，在wireshark中拷贝的设备标识都有NPF_的前缀，因为wireshark安装的是NPF驱动，这是不一样的，所以wireshark的命令行工具dumpcap.exe使用的网络设备标识以wireshark中查看的为准，而这里的tcpdump.exe使用的网络设备标识以 tcpdump -D命令列出的为准。

我们可以通过tcpdump –D 得出的网络设备列表每一项后面的描述信息，与我们当前使用的网络连接的描述信息比较，可以得出我们需要捕获的网络连接对应的设备标识。

命令说明：
tcpdump.exe -i \Device\{89515393-AC8F-4D23-9A03-AF35F9950E72} -w E:\000\test.pcap -C 2

-i 指定要捕获的网络设备的标识。
-w 指定保存的文件，如果分段则会自动保存为  test.pcap、test.pcap1、test.pcap2 等，这些文件都可以直接拖到wireshark中打开。
-C 注意这里是大写，表示每一个分段文件的大小，单位是M，这里 -C 2 表示2M一个文件。









