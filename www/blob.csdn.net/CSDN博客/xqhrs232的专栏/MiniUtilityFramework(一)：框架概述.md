# MiniUtilityFramework(一)：框架概述 - xqhrs232的专栏 - CSDN博客
2012年08月22日 20:47:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：629标签：[框架																[wince																[平台																[interface																[windows																[image](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/norains/article/details/3956926](http://blog.csdn.net/norains/article/details/3956926)
1.[MiniUtilityFramework(一)：框架概述](http://blog.csdn.net/norains/article/details/3956926)----[http://blog.csdn.net/norains/article/details/3956926](http://blog.csdn.net/norains/article/details/3956926)
2.[MiniUtilityFramework(二)：第一个程序](http://blog.csdn.net/norains/article/details/3956940)----[http://blog.csdn.net/norains/article/details/3956940](http://blog.csdn.net/norains/article/details/3956940)
3.[MiniUtilityFramework(三)：配置文件概述](http://blog.csdn.net/norains/article/details/3956980)----[http://blog.csdn.net/norains/article/details/3956980](http://blog.csdn.net/norains/article/details/3956980)
4.[MiniUtilityFramework(四)：CDominatorBase](http://blog.csdn.net/norains/article/details/3956982)----[http://blog.csdn.net/norains/article/details/3956982](http://blog.csdn.net/norains/article/details/3956982)
5.[MiniUtilityFramework(五)：CUserWnd](http://blog.csdn.net/norains/article/details/3956996)----[http://blog.csdn.net/norains/article/details/3956996](http://blog.csdn.net/norains/article/details/3956996)
6.[MiniUtilityFramework(六)：IMAGE_FILE](http://blog.csdn.net/norains/article/details/3957002)----[http://blog.csdn.net/norains/article/details/3957002](http://blog.csdn.net/norains/article/details/3957002)
7.[MiniUtilityFramework(七)：STRING](http://blog.csdn.net/norains/article/details/3957013)----[http://blog.csdn.net/norains/article/details/3957013](http://blog.csdn.net/norains/article/details/3957013)
8.[MiniUtilityFramework(八)：CImage和IMAGE](http://blog.csdn.net/norains/article/details/3957029)----[http://blog.csdn.net/norains/article/details/3957029](http://blog.csdn.net/norains/article/details/3957029)
9.[MiniUtilityFramework(九)：CText和TEXT](http://blog.csdn.net/norains/article/details/3957123)----[http://blog.csdn.net/norains/article/details/3957123](http://blog.csdn.net/norains/article/details/3957123)
10.[MiniUtilityFramework(十)：CButton和BUTTON](http://blog.csdn.net/norains/article/details/3957135)----[http://blog.csdn.net/norains/article/details/3957135](http://blog.csdn.net/norains/article/details/3957135)
//========================================================================
  //TITLE:
  //    MiniUtilityFramework(一)：框架概述
  //AUTHOR:
  //    norains
  //DATE:
  //    Friday 20-February-2009
  //Environment:
  //    VISUAL STUDIO 2005 + WINDOWS CE 5.0
  //========================================================================
    虽然在wince平台中有不少框架，令界面的编写非常方便，但如果想用特定的图片来替换相应的画面，则无一例外都会变得更复杂。而MiniUtilityFramework(以下简称MUF)和已有的框架相较，则完全是基于图片的形式。也就是，你可以在PhotoShop中设计出非常漂亮的图片，然后不用花费很大的力气就能运用到程序中。正如框架的前缀Mini所言，这是一个很迷你的框架。因为到目前为止，该框架也仅仅是实现了为数不多的几个控件，相对于已有的框架来说，完全是轻量级的。
    MUF有个唯一的特色，要实现一个华丽的界面，只需要编写相应的配置文件即可。界面和功能相分离，大大减轻了代码的工作量。
    接下来我们看一下目前版本的MUF的结构：
    ./Include
      Utility.h：框架所需要的一些通用变量。配置文件中的很多字段都能在此的KeyName命名空间中找到。
    ./Control
      CButton：按钮控件
      CCommon：封装了一些常用的操作
      CImage：图片控件
      CIniParse：ini配置文件的读写操作类
      CMemDC：内存DC的操作类，主要是用在图片的绘制上
      CPowerThread：获取电源状态的类，该类的功能层次的实现需要底层的支持。
      CProgress：进度条控件
      CReg：注册表的操作类
      CText：文本控件
    ./Document
      IniSettingFileManual.txt：配置文件的说明文档
    ./Dominate
      CDominatorBase：控制框架运行的类。在实际使用中，必须继承该类。
      COption：读取配置文件，调用相应的控件做相应的处理。
    ./Function
      CCommand：封装了部分命令操作，主要是给按钮使用。但有部分命令是基于特定的平台，无法在另外的平台中使用，比如：背光。
      CEffect：窗口移动的特效
      CSystem：系统的一些操作指令。有部分指令只能用在特定的平台。
    ./Table
      CImageTabBase：程序中所用到的图片资源，都从该类中获取。
      CStrTabBase：程序中所用到的字符串资源，都从该类中获取。
    ./Interface
      CAnimateWnd：动画控件。动画是由一连串的BMP图档连续显示而成
      CChildWndInterface：所有的子窗口都必须继承于该接口。
      Interface：定义了部分所需要的变量类型
      CListWnd：简单的列表控件
      CPowerWnd：电源信息显示控件
      CSliderWnd：滚动条控件
      CTimeWnd：时间显示控件
      CUserWnd：主窗口。如果程序中需要实现特定的功能，都必须继承于该类
      CWndBase：基本窗口
      WndInterface：所有的窗口（含子窗口）都必须继承于该类
    ./Project
      ./Library：编译MUF为lib的工程文件
      ./Simple：最简单的一个示例工程
