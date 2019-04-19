# nk.bin和nk.nb0文件格式分析 - xqhrs232的专栏 - CSDN博客
2012年09月02日 15:54:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：673标签：[image																[file																[microsoft																[windows																[嵌入式																[wince](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/wogoyixikexie/archive/2009/01/19/1378341.html](http://www.cnblogs.com/wogoyixikexie/archive/2009/01/19/1378341.html)
相关网帖
1.转:深入理解.bin文件和.nb0文件的结构----[http://blog.sina.com.cn/s/blog_541f70fa0100eg1e.html](http://blog.sina.com.cn/s/blog_541f70fa0100eg1e.html)

作者：[wogoyixikexie@gliet](mailto:wogoyixikexie@gliet)
借用别人的解释： 
     这里提到的bin是一种二进制镜像格式，以片断（section）为单位组织数据，每个片断都包括一个头，头里指定了起始地址，长度，校验值。Platform Builder调用工具将WINCE内核所有文件以bin格式合并成一个文件，默认文件名为nk.bin。BootLoader又以同样的格式将nk.bin分解成多个文件放到RAM中。可以在命令行中键入“viewbin nk.bin”来查看bin文件中具体包括了哪些内容。键入Cvrtbin命令转换.bin格式文件为.sre格式或者.abx格式。 
nb0格式是原始的二进制镜像，它不包括头，一般情况下将内核下载到设备的RAM中运行都采用nb0格式。要生成nbx格式的文件，需要在相关.bib文件中确定如下值：ROMSTART、ROMWIDTH、ROMSIZE
      那么nk.bin是如何生成的？
Romimage.exe is a locator application that creates Windows CE binary image (.bin) files, usually limited to a single file called Nk.bin.
 This means that Romimage
- Reads Ce.bib to determine which %_FLATRELEASEDIR% binaries and files to include in the run-time image.
- Assigns physical memory addresses to these binaries and files. （这部是如何实现的？）
- Creates the run-time image Nk.bin.
For more information about Ce.bib, see [Make
 Binary Image Tool](http://www.cnblogs.com/wogoyixikexie/admin/ms-help://MS.WindowsCE.500/wcepbguide5/html/wce50conmakebinaryimagetool.htm).
     最近想搞multi bin后来发现我的bootloader是优龙的，不支持binfs分区格式化， 我想把eboot下的代码移植过来，应该可以的。 
但是我对则个binfs有点疑惑。按照道理说，binfs是针对nk.bin的，但是我们下载的时候却是nk.nb0，有些人下载nk.bin，但是下载完还是要解压成nk.nb0的。 
我想问，binfs是怎么和nk.nb0联系起来的？ 
==============================来看看bin文件的格式=========================
一个bin 文件在存储上是按下面的结构存储的
     组成：标记(7)+Image开始地址(1)+Image长度(1)  
           记录0地址+记录0长+记录0校验和+记录0内容(文件内容)
        记录1地址+记录1长+记录1校验和+记录1内容(文件内容)
           ......
     最后一条记录是表示结束,Start = 0x00000000, Length = 0x8C072C3C是StartUp地址, Chksum = 0x00000000
     bin 文件的头部(不包括记录)可以用下面的结构表示
      struct BinFile{ 
                  BYTE signature[7]; // = { ''B'', ''0'', ''0'', ''0'', ''F'', ''F'', ''\a'' } 
                  DWORD ImageStart 
                  DWORD ImageLength 
            };
一般xipkernel.bin,nk.bin 都符合正常bin文件格式,包含记录开始0,1,2 记录为特殊记录,2做为cece的标记,其后4byte表示 TOC地址(指向ROMHDR结构的数据),3记录开始都是文件记录,
———但是，我们实际用到却是nk.nb0文件，这个文件才是和文件系统帮顶起来的。我等下要找出他的格式来。先去吃午饭。
再借别人的解释
        首先，NK.BIN中的内容是被压缩过的,NK.NB0中的内容是没有压缩的。 两者大小的区别是因为在生成过程中BIN会将你设定的后面的NULL自动去掉，而NB0就不会。
        现在来用微软自带工具来看看这两个文件的格式到底有什么不同。
![](http://images.cnblogs.com/cnblogs_com/wogoyixikexie/viewbin.jpg)
这幅图结合上面的解释可以知道，这个NK.bin文件的标志是BOOOFF，nk.nb0是没有这个标志的，要知道具体的信息，还是看MSDN好。
At   the   end   of   the   boot   loader   development   process   described   in   the   topic   How   to   Develop   a   Boot   Loader,   you   will   have   two   different   binary   images   for   the   boot
   loader:   a   .bin   file   and   an   .nb0   file.   The   Microsoft   Windows®   CE   binary   image   data   format   (.bin)   file   is   the   most   common   format   for   Windows   CE   binary   images.   It   is   a   binary   file   that  
 consists   of   a   number   of   individual   records   with   associated   per-record   bookkeeping   data.   The   format   is   convenient   for   minimizing   the   amount   of   data   to   be   downloaded   to   the   target   device   by   removing
   the   need   to   pad   between   records.   For   more   information   about   the   .bin   file   format,   see   Windows   CE   Binary   Image   Data   Format   (.bin).   
  The   .nb0   file   format   is   a   raw   binary   image   of   the   boot   loader.   The   image   is   as   it   appears   in   the   memory   on   the   target   device   and   does   not   contain   the   header   information   that   the   .bin  
 file   includes.   The   .nb0   file   is   typically   larger   than   the   .bin   file.   The   .nb0   file   is   useful   for   placing   the   initial   boot   loader   image   on   the   target   device.   This   is   usually   done   with   a   built-in
   monitor   program   provided   by   the   board   manufacturer.   You   can   also   place   the   initial   boot   loader   image   on   the   target   device   through   a   JTAG   connection   using   a   JTAG   probe.   Once   the   .nb0   image   is
   stored   on   the   device,   it   should   be   able   to   download   and   update   itself   using   the   .bin   file   format   from   then   on.   
我使用的是ADS开发的bootloader，这个bin文件不存在什么段的问题吧，应该和微软不一样的，全局变量的问题在哪里呢？但是现在造成偶尔不能启动是什么原因呢？最后发现又回到了原点。看来ADS下的bin文件格式也要搞清楚才行哦。
**注意事项：**
一、如果文章标题标有【原创】字眼的，gooogleman有原创著作权，请转载的时候不要删除文章的任何部分，并且商业网站转载必须经过gooogleman同意。
二、文章标题标有【转载】也请转载的时候标明原创作者的名字和原文地址，尊重原创作者。
三、本博客为gooogleman的官方博客，并且会和gooogleman官方网站[http://www.gooogleman.com/](http://www.gooogleman.com/)以及gooogleman
 CSDN 博客同步更新。
四、本博客原创文章所有权属于gooogleman嵌入式开发板联盟。
五、gooogleman嵌入式开发板联盟盟主旗舰店为[http://gooogleman.taobao.com/](http://gooogleman.taobao.com/) 旺旺ID 为：gooogleman2009 目前发现网上有类似的ID，请各位网友不要认错，gooogleman和gooogleman2009
 是唯一的！
