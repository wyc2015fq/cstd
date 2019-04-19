# 【转】yuv文件显示与测试 - 三少GG - CSDN博客
2010年10月01日 16:55:00[三少GG](https://me.csdn.net/scut1135)阅读数：2540

### [http://book.51cto.com/art/200902/111528.htm](http://book.51cto.com/art/200902/111528.htm)
**14.3.1 YUV原始视频数据及其显示**
[http://book.51cto.com](http://book.51cto.com/)  2009-02-25 10:31  路锦正/周冬梅  电子工业出版社  [我要评论(0)](http://www.51cto.com/php/feedbackt.php?id=111528)
- 摘要：《Visual C++音频/视频处理技术及工程实践》第14章XviD CODEC实现MPEG-4编/解码,本章首先介绍MPEG-4视频编码算法的原理，对MPEG-4编码算法的工程实现XviD CODEC的工作过程做详细的介绍。本节为大家讲述的是YUV原始视频数据及其显示。
- 标签：[YUV](http://www.51cto.com/php/search.php?keyword=YUV)[C++](http://www.51cto.com/php/search.php?keyword=C%2B%2B)[Visual C++](http://www.51cto.com/php/search.php?keyword=Visual%20C%2B%2B)[音频](http://www.51cto.com/php/search.php?keyword=%D2%F4%C6%B5)[视频](http://www.51cto.com/php/search.php?keyword=%CA%D3%C6%B5)
- 
**14.3  运行XviD CODEC系统**
本书采用的XviD CODEC是xvidcore-1.0.0版本，XviD网站有最新的代码xvidcore-1.1.3。xvidcore-1.0.0网址是[http://files.xvid.org/downloads/xvidcore-1.0.0.zip](http://files.xvid.org/downloads/xvidcore-1.0.0.zip)，作者使用迅雷下载软件成功下载。案例使用VC++ 2005和CCS 2.2软件运行XviD CODEC。
**14.3.1  YUV原始视频数据及其显示**
MPEG-4视频算法主要支持I420格式，即YUV420平面模式。网络上有许多此格式的标准测试序列文件：foreman.yuv、stefan.yuv、mobile.yuv等，分辨率是CIF（352像素 288像素），帧率是25f/s。**网站[http://trace.eas.asu.edu/yuv/index.html](http://trace.eas.asu.edu/yuv/index.html)****有QCIF、CIF格式的YUV测试序列文件。**
查看YUV420/I420的软件常用的是YUVviewer程序。光盘中有该程序的源代码，用户可以根据自己的需要进行修改。另外该程序也是VC 6.0平台下的，则在使用VC 2005运行时需要修改多个地方。
yuvviewerdlg.cpp(641):error C2440: "static_cast"无法从"int (__thiscall CYUVviewerDlg::* )(void)"转换为"AFX_PMSG"。由于VC++自动添加的消息处理函数返回值默认是void，所以需要把函数OnOpenfile()的返回值修改为void。
警告"_CRT_SECURE_NO_DEPRECATE"：从前面的分析知道，这是从旧版本的VC程序移植到VC++ 2005时一个典型的警告，即VC++ 2005认为某些函数的使用不安全，因为程序对用户来说是清楚的，所以可以关闭该警告。增加宏定义，过程是在项目"属性→配置属性→C/C++→预处理器"中的预处理器定义中添加";_CRT_SECURE_NO_DEPRECATE"。
=======================================================================================
====================================================================================
====================================================================================
[http://hi.baidu.com/yuezhichn/blog/item/6ed5c2fc389d3bfcfd037f79.html](http://hi.baidu.com/yuezhichn/blog/item/6ed5c2fc389d3bfcfd037f79.html)
**yuv文件测试**
2007年06月13日 星期三 16:00
我们要测试的文件foreman_part_qcif.yuv它包含三帧彩色图像。
![](http://yuyangemu.bokee.com/inc/Snap1.jpg)
以及文件foreman_part_qcif.y 注意后缀，它包含三帧灰度图像（只有y信号）
![](http://yuyangemu.bokee.com/inc/Snapheibai2.jpg)
qicf文件的分辨率为176*144，并且每个分量用一个字节存储
则对于foreman_part_qcif.yuv文件，用于表达颜色的字节大小应该为：
3[帧数]*{176*144[亮度]+(1/4)*176*144[u色度信号]+(1/4)*176*144[v色度信号]}= 114048
查看该文件的大小后发现确为114,048 字节
说明该文件不同于其他图像格式的文件(bmp,jpeg等)，它没有文件头，没有任何与表达颜色信号无关的内容。
用十六进制编辑器打开两个文件进行比对，如下图：
![](http://yuyangemu.bokee.com/inc/Snap3.jpg)
![](http://yuyangemu.bokee.com/inc/Snap4.jpg)
它们开始的内容是一样的。
从下面的两幅图可以看出：
两个文件从6300h（十六进制）之后开始出现不同，6300h转化为十进制是176*144=25344
刚好是y信号所占用的字节的大小。
对于文件foreman_part_qcif.yuv 它从6301h起开始排列第一帧的u信号。
对于文件foreman_part_qcif.y 它从6301h起开始排列第二帧的y信号。
![](http://yuyangemu.bokee.com/inc/Snap5.jpg)
由以上分析可以得出4：2：0的qcif.yuv文件的格式如下图：
cif文件的分辨率为352*288,
qcif即1/4的cif为176*144,
![](http://yuyangemu.bokee.com/inc/Snap2.jpg)
4：2：0的qcif.yuv文件结构
yuv文件不能直接在windows底下打开，通过工具YUVview可以显示该文件
![](http://yuyangemu.bokee.com/inc/Snap6.jpg)
![](http://yuyangemu.bokee.com/inc/Snap7.jpg)
