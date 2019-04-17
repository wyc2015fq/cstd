# 在win7下使用zend studio 7.1.0，解决汉化、中文乱码、js等问题  - 战斗蜗牛的专栏 - CSDN博客





2011年06月13日 10:32:00[vbskj](https://me.csdn.net/vbskj)阅读数：1251标签：[zend																[aptana																[eclipse																[eclipse插件																[search																[features](https://so.csdn.net/so/search/s.do?q=features&t=blog)
个人分类：[java](https://blog.csdn.net/vbskj/article/category/643636)





在win7下使用zend studio 7.1.0，解决汉化、中文乱码、js等问题
            
			




# Zend Studio是一个屡获大奖的专业 PHP 集成开发环境，具备功能强大的专业编辑工具和调试工具，支持PHP语法加亮显示，支持语法自动填充功能，支持书签功能，支持语法自动缩排和代码复制功能，内置一个强大的PHP代码调试工具，支持本地和远程两种调试模式，支持多种高级调试功能。Zend Studio 7.0新增功能节选如下：


◆支持PHP 5.3

◆增强的源代码编辑——就地代码重构，语义分析和快速的解决办法

◆更快速与简单的调试——检测和诊断问题并传送到Zend服务器

◆加强Zend框架整合——Zend代码生成工具，定制的项目布局，更更好的性能

同时发布的Zend框架1.9的主要更新也是增添了PHP 5.3的支持。



**1、下载及安装**

官方下载连接：[http://downloads.zend.com/studio-eclipse/7.1.0/ZendStudio-7.1.0.exe![](http://hiphotos.baidu.com/pxc_/pic/item/251f95ca22d777bbc9176839.jpg)](http://downloads.zend.com/studio-eclipse/7.1.0/ZendStudio-7.1.0.exe)





PC Home下载连接：[http://download.pchome.net/development/sever/php/detail-39344.html![](http://hiphotos.baidu.com/pxc_/pic/item/d1c8a7863c08af0367096e39.jpg)](http://download.pchome.net/development/sever/php/detail-39344.html)

Zend Studio 7 注册码





User Name：mimiz_cn

License Key：3B9B9BB76C218B98BB76C218




**2、中文版/汉化**

◆2-1：下载汉化包，可汉化80%**（推荐）**

csdn下载连接：[http://download.csdn.net/source/1835153![](http://hiphotos.baidu.com/pxc_/pic/item/c9177f3e9016190070cf6c39.jpg)](http://download.csdn.net/source/1835153)

本地下载连接：[http://www.w2blog.net/download/Zend-Studio/zend-studio-7-zh.rar](http://www.w2blog.net/download/Zend-Studio/zend-studio-7-zh.rar)

下载该汉化包然后解压到你所安装的ZEND的同一目录下，最后重启ZEND就OK拉

◆2-2：官方Updates



Help 菜单下选择 Software Updates 然后选择 Find and Install... 选项. 在弹出的窗口里选择 
Search for new features to install , 之后next下一步. 在新的窗口里点击 New Remote Site
 按钮.

    在新窗口里的Name:框里输入任意一个名称. 比如: zendcn 
；URL里输入:http://download.eclipse.org/technology/babel/update-site
/ganymede 接下来选中这个新加的更新地址, 点击 finished 按钮结束,

    接下来 Zend Studio For Eclipse 开始分析更新站点的过程, 可能会弹出镜像站点的选择列表窗口, 这里选择刚才新建的那个更新站点(zendcn ), 不用选择从其他镜像站点读取, 这样会比较慢.

    Zend Studio For Eclipse 分析完后会弹出要下载的语言包. 在倒数第6位置的地方可以看到 ... Simple Chinese 选项. 简体中文勾选这个即可.

    接下来NEXT, 同意协议, 然后开始更新中文语言包.最后弹出窗口, 点 Install All 即可.

◆2-3：使用zend studio 6 的汉化包



1、语言包：http://downloads.zend.com/studio-eclipse/6.0.1/ZendStudio6.0-Language-Pack-it.zip下载

2、解压到一个目录，如C://Downloads//ZendStudio6.0-Language-Pack-it

3、打开Zend Studio for Eclipse，Help->Software Updates->Find and Install

4、在弹出的子窗体中选择Search for new features to install，Next

5、在Update sites to visit子窗体中，点击New Location 
Site，选择C://Downloads//ZendStudio6.0-Language-Pack-it，确定后会出现在Sites to 
include in search中，并且默认勾选，然后点击Finish，这时候更新管理器会自动查找。

6、查找完毕后，Search Results窗体展开后可以发现许多中文简体的语言包，勾选全部后发现一个问题：WYSIWYG 
PHP/HTML Editing NL Feature (1.0.0.200805051510) requires feature 
"com.xored.composer".所见即所得编辑器缺少一个关键部件，那么我们暂时先不安装它，取消勾选，点击Next。

7、同意条款，然后依次Next，Finish，Install All，重启eclipse后即可。这样就完成了整个安装过程，不过这个语言包并不是完全汉化，只是涵盖了一些常用的基本功能。

8、更改代码编写环境的字体属性可在首选项中设置。




**3、优化**

◆3-1：提高代码提示速度

在Window -> preferences -> PHP -> Editor -> Content assist -> Auto-Activation 中，可以调整自动提示的时间，把它改为0


**4、中文乱码**

project右键->properties->text    file encoding->other->UTF-8


**5、js提示(在zend studio 7.1里安装aptana)**



我们以本地文件安装为例，当然你的网速够好，可以选择eclipse update,方法是help菜单下的install new software，然后输入地址： [http://download.aptana.org/tools/studio/plugin/install/studio![](http://hiphotos.baidu.com/pxc_/pic/item/70cf3bc7642228e1d1006039.jpg)](http://download.aptana.org/tools/studio/plugin/install/studio)
 来完成这个步骤。aptana的eclipse插件的下载地址是：[http://www.aptana.org/studio/plugin![](http://hiphotos.baidu.com/pxc_/pic/item/d100baa1320ea2b647106439.jpg)](http://www.aptana.org/studio/plugin)
，找到下面的Download [Studio 2.0 Plugin![](http://hiphotos.baidu.com/pxc_/pic/item/cd11728b4630692fc8fc7a39.jpg)](http://download.aptana.org/tools/studio/plugin/install/studio/zip)
 zip.下载完毕后，打开zend studio 7.1，在help菜单下也是选择install new software，然后点击add，在弹出的对话框中点击Archive，然后浏览到刚刚下载回来的zip文件，如图1

![图1](http://hiphotos.baidu.com/pxc_/pic/item/4710b912cbdc13f3c3fd7839.jpg)

然后在work with的下拉菜单找到刚刚的添加，如图2

![图2](http://hiphotos.baidu.com/pxc_/pic/item/c8fcc3cec6dd2c3e92457e39.jpg)

此时就在列表中出现了aptana studio了，勾选之，next进入下一步.见图3

![图3](http://hiphotos.baidu.com/pxc_/pic/item/c3fdfc03956506b5d43f7c39.jpg)

这里确认一下将会安装的插件清单，再下一步，选择I 
accept同意授权协议，点击finish完成。经过一会儿的等待中间会出现一个警告对话框，点击yes，安装完成后，重启下zend studio 
7.1，aptana就已经被成功植入到zend 
studio中了，此时他们将是水乳交融，不分你我了哇！接下来，就是去preferences中把javascript，css，html什么的默认编
辑器都修改成aptana，这样就可以方便的使用aptana啦！~见下图

![图4](http://hiphotos.baidu.com/pxc_/pic/item/9245d688dd1f57a9a4c27239.jpg)

再来张效果图



![图5](http://hiphotos.baidu.com/pxc_/pic/item/d43f8794afe2ad23d21b7039.jpg)



参考资料：

[http://www.zendstudio.net/archives/zend-studio-7-1-and-aptana-are-good-brothers/![](http://hiphotos.baidu.com/pxc_/pic/item/a4c27d1edf3bdec91bd57639.jpg)](http://www.zendstudio.net/archives/zend-studio-7-1-and-aptana-are-good-brothers/)

[http://mimiz.cn/index.php/web/zend-studio-7/![](http://hiphotos.baidu.com/pxc_/pic/item/d21b0ef414f57d53ddc47439.jpg)](http://mimiz.cn/index.php/web/zend-studio-7/)

[http://wenda.tianya.cn/wenda/thread?tid=4c1364eeec79905a![](http://hiphotos.baidu.com/pxc_/pic/item/1bd5ad6ecce481e781cb4a39.jpg)](http://wenda.tianya.cn/wenda/thread?tid=4c1364eeec79905a)

[http://qeephp.com/bbs/viewthread.php?tid=2695![](http://hiphotos.baidu.com/pxc_/pic/item/ddc451da92ebe9e6b6fd4839.jpg)](http://qeephp.com/bbs/viewthread.php?tid=2695)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse插件&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=aptana&t=blog)](https://so.csdn.net/so/search/s.do?q=zend&t=blog)




