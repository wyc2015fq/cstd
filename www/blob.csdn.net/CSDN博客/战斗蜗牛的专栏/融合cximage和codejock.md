# 融合cximage和codejock - 战斗蜗牛的专栏 - CSDN博客





2012年04月17日 21:43:21[vbskj](https://me.csdn.net/vbskj)阅读数：829标签：[eclipse																[mfc																[dll																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)





（1）把demo拷贝至codejock目录（把cximage中的lib拷贝到一个独立目录下，加入工程；把Cximage\Cxiamge中的头文件放入一个独立目录下）

（2）Eclipse文件拷贝至src目录

（3）项目属性修改：c++：附加目录；在共享dll中使用MFC；在demo中，使用预编译头。（stdafx头文件加入<XTToolKitPro.h>）；在连接器中加入所有的lib；







==============开始加入codejock=================

2.1 CMainFrame类修改

（1）在Mainfrm.h中插入相应的头文件；

（2）修改MainFrame的类

（3）添加程序文件（10xEclipse, 2xApperarance）

（4）添加resource中的ID_PANE_HIDE or 字符表

（5）添加位图资源

（6）把Eclipse添加入demo工程，进行资源的添加。（对话框资源等）

（7）增加AddIcons函数--》 CreateGrayBitmap

（8）添加AppearanceSetEclipse.h头文件](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)




