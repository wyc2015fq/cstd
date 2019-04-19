# Qt 维护工具MaintenanceTool.exe 使用 - xqhrs232的专栏 - CSDN博客
2018年09月05日 14:40:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：144
原文地址::[https://blog.csdn.net/liulihuo_gyh/article/details/78583884](https://blog.csdn.net/liulihuo_gyh/article/details/78583884)
http://blog.sina.com.cn/s/blog_8564b95d0102ybpz.html
QT的组件管理软件并没有在开始菜单或者桌面添加快捷方式（5.9版本），也没有在代码编辑界面设置相关的选项，藏的比较深，因此我被坑了很多次（之前如果要添加组件，只能选择卸载了重装）
没有对比旧没有伤害，微软visual studio2017的组件管理软件​visual studio installer清晰明了
![](http://s4.sinaimg.cn/mw690/002rsiqVzy7firUCPGr03)*﻿visual studio installer*
在被坑了多次之后，在一次偶然的机会，我在QT安装目录发现了QT的组件管理软件**MaintenanceTool**
![](http://s16.sinaimg.cn/mw690/002rsiqVzy7fis48Lnhdf)*﻿MaintenanceTool。exe*
怀着无比激动的心情，我选择“添加或移除组件”点了下一步，结果mmp，提示“要继续此操作，至少需要一个有效且已启用的储存库”，只能点设置手动配置了（因为默认的储存库不能用，所以会提示）。
手动添加​储存库要定位一个储存有**QT在线安装镜像的地址**，这可难坏我了，但是经过不懈努力还是被我找到了（网址：http://download.qt.io/static/mirrorlist/）这个网站，显示了各国的qt镜像站点，中国有四个，我用的是中科大的站，点击HTTP会进入一个网络文件夹。
![](http://s6.sinaimg.cn/mw690/002rsiqVzy7fisGct1j85)*﻿qt镜像*
​然后依次进入/online/qtsdkrepository/windows_x86/root/qt/ 最终的文件夹显示如下，在这个界面**复制一下当前地址框地址**
![](http://s7.sinaimg.cn/mw690/002rsiqVzy7fisXHR4ib6)*﻿中科大QT镜像*
在储存库中选择**临时储存库**
![](http://s16.sinaimg.cn/mw690/002rsiqVzy7fit6ao7Jaf)*﻿临时储存库*
点击添加，在编辑界面写入刚刚复制的地址（http://mirrors.ustc.edu.cn/qtproject/online/qtsdkrepository/windows_x86/root/qt/）添加后可以进行一次网络测速，看是否连通。
![](http://s5.sinaimg.cn/mw690/002rsiqVzy7fit6k4FS54)*﻿连通成功*
OK现在点击下一步就正常了，耐心等待，等待时间取决于网络。
![](http://s9.sinaimg.cn/mw690/002rsiqVzy7fitmIoaY68)
之后我们期盼的界面终于出现了，在此勾选要新增的组件即可。
![](http://s14.sinaimg.cn/mw690/002rsiqVzy7fituihUVbd)
