# 解决Qt中文乱码以及汉字编码的问题(UTF-8/GBK) - xqhrs232的专栏 - CSDN博客
2017年10月15日 13:35:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：320
原文地址::[http://blog.csdn.net/libaineu2004/article/details/19245205](http://blog.csdn.net/libaineu2004/article/details/19245205)
相关文章
1、[QT笔记79:QPainter指定位置写字](http://www.cppblog.com/gaimor/archive/2014/12/14/209180.html)----[http://www.cppblog.com/gaimor/archive/2014/12/14/209180.html](http://www.cppblog.com/gaimor/archive/2014/12/14/209180.html)
2、[Qt汉字显示乱码的一个简单解决方案](http://www.cnblogs.com/plRobotics-RD-Center/p/4092274.html)----[http://www.cnblogs.com/plRobotics-RD-Center/p/4092274.html](http://www.cnblogs.com/plRobotics-RD-Center/p/4092274.html)
3、qt无法正常输出中文怎么办----[https://jingyan.baidu.com/article/ab69b270b254332ca6189f46.html](https://jingyan.baidu.com/article/ab69b270b254332ca6189f46.html)
**一、Qt环境设置**
文件从window上传到Ubuntu后会显示乱码,原因是因为ubuntu环境设置默认是utf-8,Windows默认都是GBK.
Windows环境下,Qt Creator,菜单->工具->选项->文本编辑器->行为->文件编码:
默认编码:System(简体中文windows系统默认指的是GBK编码,即下拉框选项里的GBK/windows-936-2000/CP936/MS936/windows-936)
**二、编码知识科普**
Qt常见的两种编码是:UTF-8和GBK
★UTF-8：Unicode TransformationFormat-8bit，允许含BOM，但通常不含BOM。是用以解决国际上字符的一种多字节编码，它对英文使用8位（即一个字节），中文使用24为（三个字节）来编码。UTF-8包含全世界所有国家需要用到的字符，是国际编码，通用性强。UTF-8编码的文字可以在各国支持UTF8字符集的浏览器上显示。如，如果是UTF8编码，则在外国人的英文IE上也能显示中文，他们无需下载IE的中文语言支持包。
★GBK是国家标准GB2312基础上扩容后兼容GB2312的标准。GBK的文字编码是用双字节来表示的，即不论中、英文字符均使用双字节来表示，为了区分中文，将其最高位都设定成1。GBK包含全部中文字符，是国家编码，通用性比UTF8差，不过UTF8占用的数据库比GBD大。GBK是GB2312的扩展，除了兼容GB2312外，它还能显示繁体中文，还有日文的假名。
★GBK、GB2312等与UTF8之间都必须通过Unicode编码才能相互转换：
GBK、GB2312－－Unicode－－UTF8
UTF8－－Unicode－－GBK、GB2312 
★在简体中文windows系统下，ANSI编码代表GBK/GB2312编码，ANSI通常使用0x80~0xFF范围的2个字节来表示1个中文字符。0x00~0x7F之间的字符，依旧是1个字节代表1个字符。Unicode(UTF-16)编码则所有字符都用2个字节表示。
**三、编码转换**
UTF-8与ANSI(即GBK)的互转,可以使用EditPlus工具"文件另存为"或者Encodersoft编码转换工具对.cpp和.h源文件文本进行批量转换.
**四、Qt编码指定**
Qt需要在main()函数指定使用的字符编码:
#include <QTextCodec>
QTextCodec *codec = QTextCodec::codecForName("GBK");//情况1
QTextCodec::setCodecForTr(codec);
QTextCodec::setCodecForLocale(codec);
QTextCodec::setCodecForCStrings(codec);
或
QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
QTextCodec::setCodecForTr(codec);
QTextCodec::setCodecForLocale(codec);
QTextCodec::setCodecForCStrings(codec);
这里只列举大家最常用的3个编译器(微软VS的中的cl，Mingw中的g++，Linux下的g++)，源代码分别采用GBK和无BOM的UTF-8以及有BOM的UTF-8这3种编码进行保存,发生的现象如下表所示。
- |源代码的编码|编译器|显示正常|显示乱码|
|----|----|----|----|
|GBK|win vs cl|情况1|情况2|
|win mingw-g++|情况1|情况2| |
|linux g++|情况1|情况2| |
|UTF-8(无BOM)|win vs cl|编译失败error C2001: 常量中有换行符|编译失败error C2001: 常量中有换行符|
|win mingw-g++|情况2|情况1| |
|linux g++|情况2|情况1| |
|UTF-8(有BOM)|win vs cl|情况1|情况2|
|win mingw-g++|情况2|情况1| |
|linux g++|情况2|情况1| |
**五、应用案例**
QCom跨平台串口调试助手([http://www.qter.org/?page_id=203](http://www.qter.org/?page_id=203))
源代码qcom\mainwindow.cpp,aboutdialog.cpp等文件用的是UTF-8编码(无BOM);但是qcom\qextserial\*.*文件用的是ANSI编码.在linux环境编译完全OK.
笔者Windows环境的Qt Creator+微软VS编译器,环境设置用的是ANSI(即GBK)编码.编译源文件会报错.
错误提示"fatal error C1018: 意外的 #elif".
![](https://img-blog.csdn.net/20140228225314140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGliYWluZXUyMDA0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决方法由两种：
方法1：
![](https://img-blog.csdn.net/20140228225737562)
把qcom\的所有文件都用工具转换成ANSI编码,main()函数使用QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
方法2：
先把Qt Creator环境设置用的是UTF-8编码,
![](https://img-blog.csdn.net/20140228225803921)
再把qcom\的所有文件都用工具转换成UTF-8+BOM编码,请注意,如果文件转换成UTF-8(无BOM),编译仍会失败.main()函数使用QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));//注意,此处仍是"GBK",不是"UTF-8"
重新编译,OK!
**六、结论**
Windows环境下,Qt Creator+微软VS编译器,新建工程,
1、如果该工程不需要跨平台使用(只在win),那么工程设置请使用GBK的编码方式.
2、如果该工程要跨平台使用(win+linux),那么工程设置请使用UTF-8+BOM的编码方式.
Linux环境下,Qt Creator+gcc,新建工程,
没有GBK编码可选,默认是UTF-8(无BOM)编码方式,考虑到跨平台,建议选择UTF-8+BOM的编码方式.
**七、参考文献**
Qt中文乱码问题[http://blog.csdn.net/brave_heart_lxl/article/details/7186631](http://blog.csdn.net/brave_heart_lxl/article/details/7186631)
[](http://blog.csdn.net/libaineu2004/article/details/19245205#)[](http://blog.csdn.net/libaineu2004/article/details/19245205#)[](http://blog.csdn.net/libaineu2004/article/details/19245205#)[](http://blog.csdn.net/libaineu2004/article/details/19245205#)[](http://blog.csdn.net/libaineu2004/article/details/19245205#)[](http://blog.csdn.net/libaineu2004/article/details/19245205#)
- 
顶
9- 
踩
