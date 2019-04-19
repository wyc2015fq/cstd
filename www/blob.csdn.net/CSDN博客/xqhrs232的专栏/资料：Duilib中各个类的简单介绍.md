# 资料：Duilib中各个类的简单介绍 - xqhrs232的专栏 - CSDN博客
2012年09月22日 20:53:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1389
原文地址::[http://note.sdo.com/u/icez/n/mvO-X~jyVnpFnM01A0000m](http://note.sdo.com/u/icez/n/mvO-X~jyVnpFnM01A0000m)
DirectUI意为直接在父窗口上绘图(Paint on parent dc directly)。即子窗口不以窗口句柄的形式创建(windowless)，只是逻辑上的窗口，绘制在父窗口之上。微软的“DirectUI”技术广泛的应用于Windows XP、Vista、Windows 7，如浏览器左侧的TaskPanel，控制面板导航界面，Media Player播放器，即时通讯工具MSN Messager等。
DirectUI好处在于可以很方便的构建高效、绚丽的、非常易于扩展的界面。国外如微软，国内如腾讯、百度等公司的客户端产品多采用这种方式来组织界面，从而很好的将界面和逻辑分离，同时易于实现各种超炫的界面效果如换色、换肤、透明等。
DirectUI可以理解为一个轻量级的WPF，可以让C++做出C#般绚丽的界面。目前国内做DirectUI皮肤库的公司主要有两家：第一家是UIPower（即Skin++的公司），网址[www.uipower.com](http://www.uipower.com/)，其DirectUI产品网站[www.directui.com](http://www.directui.com/)，做的很专业；另一家是UIEASY，网址[www.uieasy.com](http://www.uieasy.com/)，一个轻量级皮肤库DSkinLite。但是，网上关于DirectUI的技术文章却很少；早在 6/26/2005，一个国外的程序员就已经写了一个开源的DirectUI例子，应该算是国内所有directui界面库的起源了，网址[http://www.viksoe.dk/code/windowless1.htm](http://www.viksoe.dk/code/windowless1.htm)，探讨一种实现的可能性和思路,和实际应用虽有一定距离,不过却是一个很好的学习资料。再后来，一个国内的程序员“当个傻瓜蛋”，基于此代码，进行重构和修正Bug，并放到了google上开源，网址[http://code.google.com/p/duilib/](http://code.google.com/p/duilib/)，下载地址svn
 checkout [http://duilib.googlecode.com/svn/trunk/](http://duilib.googlecode.com/svn/trunk/) duilib-read-only。
本人通过学习这个duilib的源代码，学到了很多知识、思路和经验，分享给看到此文的朋友，共同探讨DirectUI技术，并欢迎大家加入到开发开源DirectUI皮肤库的行列当中。分析难免有误，见谅！
一、核心类
1.        CWindowWnd，窗口对象管理父类，主要作用：
1)        创建窗口。
2)        窗口消息过程处理。
3)        提供窗口子类化与超类化接口。
2.        CDialogBuilder，控件布局类，主要作用：
1)        读取XML脚本，分析脚本，构建控件树。
2)        创建控件对象。
3.        CPaintManagerUI，窗口消息及图形绘制管理器类，与窗口绑定，主要作用：
1)        绘制控件。
2)        消息管理。
3)        事件通知。
4.        INotifyUI，事件通知抽象类，主要作用：
1)        重载Notify虚函数，处理事件通知。
二、控件类
1.        CControlUI，控件管理父类，主要作用：
1)        控件的通用基类，提供控件通用属性管理。
2.        CLabelUI，静态标签类，父类CControlUI。
3.        CButtonUI，按钮类，父类CLabelUI。
4.        COptionUI，选择按钮类，父类CButtonUI。
5.        CTextUI，静态文本类，父类CLabelUI。
6.        CProgressUI，进度条类，父类CLabelUI。
7.        CSliderUI，父类CProgressUI。
8.        CEditUI，编辑框类，父类CLabelUI。
9.        CListUI，列表框类，父类CVerticalLayoutUI、IListUI。
1)        CListHeaderUI，父类CHorizontalLayoutUI。
2)        CListHeaderItemUI，列表头类，父类CControlUI。
3)        CListTextElementUI，类表文本类，父类CListLabelElementUI。
4)        CListLabelElementUI，父类CListElementUI。
10.    CComboUI，组合框类，父类CContainerUI、IListOwnerUI。
11.    CActiveXUI，ActiveX控件类，父类CControlUI、 IMessageFilterUI。
12.    CContainerUI，容器类，父类CControlUI、IContainerUI。
13.    CTabLayoutUI，选项页布局类，父类CContainerUI。
14.    CTileLayoutUI，父类CContainerUI。
15.    CDialogLayoutUI，对话框布局类，父类CContainerUI。、
16.    CVerticalLayoutUI，垂直布局类，父类CContainerUI。
17.    CHorizontalLayoutUI，水平布局类，父类CContainerUI。
18.    CListExpandElementUI，父类CListTextElementUI。
19.    CListContainerElementUI，父类CContainerUI、IListItemUI。
三、辅助类
       1.        CStdPtrArray，指针数组。
2.        CStdValArray，数据数组。
3.        CStdString，字符串数组。
4.        CStdStringPtrMap，字符串指针映射数组。
