# [转帖] VS集成Qt环境搭建 - Grandyang - 博客园







# [[转帖] VS集成Qt环境搭建](https://www.cnblogs.com/grandyang/p/4278287.html)







[http://blog.sina.com.cn/s/blog_a6fb6cc90101gynd.html](http://blog.sina.com.cn/s/blog_a6fb6cc90101gynd.html)



 用了这么久的Qt，IDE一直都是VS与Creator并用（实际开发以VS为主），至于哪个更好这里不发表看法，各有所长，而且也因人而异，萝卜青菜，各有所爱。
    Qt5发布很久之后，才把版本从之前的4.8升级到5.1。现在5.2发布了，果断换上，先尝尝鲜。加上之前也一直有人问我关于VS与Qt的集成，在此一并记录。。。



**环境：VS2010 + Qt5.2**



关于VS的下载、安装，这里就不再做过多阐述。



**一、下载Qt5.2安装包（qt-windows-opensource）与Qt插件（Visual Studio Add-in）**

- Qt5.2、Qt插件下载地址：[http://qt-project.org/downloads](http://qt-project.org/downloads).

    Qt5.2也可以在这里下载：[http://download.qt-project.org/official_releases/qt/5.2/5.2.0/](http://download.qt-project.org/official_releases/qt/5.2/5.2.0/).

 
 
找到对应的版本进行下载，里面包含多种不同的版本，MSVC2010、MSVC2012分别是指支持VS2010与VS2012的Qt版本，而opengl与非opengl的区别是是否支持opengl，android、ios分别是指支持android、ios平台的Qt版本。

    这里我选择的版本（已标注），如下：

- 
qt-windows-opensource-5.2.0-msvc2010_opengl-x86-offline.exe（opengl版本）
- Visual Studio Add-in 1.2.2 for Qt5 


![VS集成Qt环境搭建](http://s10.sinaimg.cn/mw690/0033AMT7gy6EVYp5eIx29&690)

![VS集成Qt环境搭建](http://s16.sinaimg.cn/mw690/0033AMT7gy6EVVj5MoDdf&690)


**二、安装**

 
  qt-windows-opensource与Visual Studio
Add-in的安装顺序没什么要求，安装过程不再多说（傻瓜式-下一步），安装路径不要包含中文、空格、特殊字符（~<>|?*!@#$%^&:,;）。

 
  安装完成Visual Studio
Add-in之后，VS就会出现相应的Qt选项！



![VS集成Qt环境搭建](http://s11.sinaimg.cn/mw690/0033AMT7gy6EWfZZAMica&690)



**三、配置开发环境**

- 选择：Qt5->Qt Options->Add，配置VS的开发环境。





![VS集成Qt环境搭建](http://s10.sinaimg.cn/mw690/0033AMT7gy6EWiaLUGB99&690)




   
这里根据自己的Qt安装路径进行选择，Path为：D:\Software\Qt\Qt5.2.0\5.2.0\msvc2010_opengl。

**说明：**4.8.3是我之前的版本，因为我可能安装很多个不同的版本（假设发现新版本存在很多问题，我不想再使用，换一个版本，怎么办？）。

    如下，选择你想要使用的版本：



![VS集成Qt环境搭建](http://s1.sinaimg.cn/mw690/0033AMT7gy6EWiu54kgf0&690)



   
如果选择了4.8.3版本，那就必须重新下载一个Visual Studio Add-in for
Qt4的插件才可以使用（**原则：**版本对应）。



   
以上配置完成之后，就可以导入或者新建Qt项目了。



**导入Qt的pro项目**

- 选择：Qt5->Open Qt Project File(.pro)...





![VS集成Qt环境搭建](http://s12.sinaimg.cn/mw690/0033AMT7gy6EWjghPztfb&690)


**新建Qt项目**

- 选择：文件->新建->项目->Qt5 Projects->Qt
Application，输入工程名，下一步...进行新建。



如下：




![VS集成Qt环境搭建](http://s7.sinaimg.cn/mw690/0033AMT7gy6EWh2QWbk86&690)


- 
选择依赖的模块：最基础的QtCore、QtGui、QtWidgets还有一些音/视频、网络、数据库、XML、OpenGl相关的模块，需要使用的直接勾选即可。


![VS集成Qt环境搭建](http://s4.sinaimg.cn/mw690/0033AMT7zy6KkQJhJfB43&690)



    完成之后，就可以运行了！

- 选择：调试->开始执行 或者 Ctrl+F5。


如下：



![VS集成Qt环境搭建](http://s10.sinaimg.cn/mw690/0033AMT7gy6EWheCyzn29&690)




**添加环境变量**

   
在工程目录下找到可执行程序，直接运行，会出现丢失*.dll的提示信息。

如下：



![VS集成Qt环境搭建](http://s6.sinaimg.cn/mw690/0033AMT7gy6EWhuu6Sp95&690)



**解决方法：**

（1）右键计算机->属性->高级系统设置->高级->环境变量，添加环境变量（推荐）

- QTDIR：D:\Software\Qt\Qt5.2.0\5.2.0\msvc2010_opengl
- PATH：%QTDIR%\bin



（2）在Qt的安装目录下找到缺少的*.dll（D:\Software\Qt\Qt5.2.0\5.2.0\msvc2010_opengl\bin），将其与可执行程序放到同一目录下。



关于VS与Qt集成的更多配置请参考：

- [一款VS开发利器-Visual Assist X](http://blog.sina.com.cn/s/blog_a6fb6cc90101ct0o.html).
- [Qt之打包发布（NSIS详解）](http://blog.sina.com.cn/s/blog_a6fb6cc90101fer8.html).














