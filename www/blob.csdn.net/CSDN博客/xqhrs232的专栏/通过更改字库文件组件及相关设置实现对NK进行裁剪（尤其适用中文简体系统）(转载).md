# 通过更改字库文件组件及相关设置实现对NK进行裁剪（尤其适用中文简体系统）(转载) - xqhrs232的专栏 - CSDN博客
2009年07月09日 21:09:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：743
最近有朋友在论坛询问如果将WINCE的NK裁剪得小一些，回复他之后顺便在此写篇文章说明一下。
通常我们在定制系统的时候会发现中文系统要比英文系统NK大10M左右，由此bootloader将内核copy到内存的时间相应变长一些，也导致占用额外的内存空间，其实可以通过选择不同的字库来对NK进行一定的裁剪。
首先我们来看当加入中文简体语言后系统默认选择的字体文件simsun.ttc（10MB）（文件目录：*:/WINCE500/PUBLIC/COMMON/OAK/FILES），有了它NK怎么可能不大呢，再看看其它几个类似的文件simsun_2_50.ttc（2.91MB）、simsun_2_50.ac3（1.91MB），如果用这个两个其中的一个NK就可以小多了，并且对中文显示没有影响。
OK，下边就是我们的工作内容了。
1、settings中设置中文。这个就不多说了。
2、选择组件：
  在Catalog中，选择“Core OS”—>“International”—>“Locale Spectific Support —> “Chinese[Simplified]”
    SimSun & NSimSun (Subset 2_50)：添加该组件替换默认的simsun.ttc，这样至少可以瘦身7MB。
    Agfa AC3 Font Compression：使用AC3对字库进行压缩，若CPU有300MHZ以上，建议添加该组件。添加则使用simsun_2_50.ac3，否则使用simsun_2_50.ttc，又减少了1MB。
    GB18030 Data Converter：汉字编码标准集转换器，必须选上。
上边的工作都完成了重新sysgen，看一下生成的NK比先前小了很多。
另外，我们可以通过将字库文件保存到其它存储器使NK瘦身效果更加明显。通常我们的WINCE设备都会有个存储介质，再怎么小额外的几M空间还是会有的，我们就将字库文件保存在这个存储介质中。需要注意的是在加载显示驱动之前就要加载这个存储器的驱动，否则系统启动后所有的字都不能正常显示。
1、在platform.reg中添加如下
[HKEY_LOCAL_MACHINE/SYSTEM/GDI]
    "FontLinkMethods"=dword:1
[HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/FontPath]
    "FontPath"="//Storage Card"  ; 字库文件所在目录(我的一个NAND Flash盘符是这个名字，用这个名字的好处是在Visual studio中使用的Emulator有同样效果)
[HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/FontLink/SystemLink]
      "Tahoma"="//Storage Card//simsun.ac3,NSimSun" ;选择了压缩字库就用simsun.ac3，否则用simsun.ttc
      "Courier New"="//Storage Card//simsun.ac3,NSimSun"
      "Arial"="//Storage Card//simsun.ac3,NSimSun"
      "Times New Roman"="//Storage Card//simsun.ac3,NSimSun"
      "Symbol"=//Storage Card//simsun.ac3,NSimSun
2、在PBWorkspaces相应工程目录中common.bib文件中删掉simsun.ac3或simsun.ttc相关项，确认该文件不会编到内核中。
3、把字库文件copy到Storage Card中重新烧写刚make好的NK，再启动就完成了。
注意，如果bootloader中copy内核不是通过读取TOC等方式来确定要读取NK的大小就需要自己更改bootloader的内容，确保copy的大小尽量小且为完整的NK，这样启动时间便可以缩短，这也是我们要对NK瘦身的主要目的。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Veabol/archive/2009/04/24/4105975.aspx](http://blog.csdn.net/Veabol/archive/2009/04/24/4105975.aspx)
