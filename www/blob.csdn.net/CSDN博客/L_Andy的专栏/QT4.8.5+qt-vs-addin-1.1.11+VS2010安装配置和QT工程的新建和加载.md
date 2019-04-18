# QT4.8.5+qt-vs-addin-1.1.11+VS2010安装配置和QT工程的新建和加载 - L_Andy的专栏 - CSDN博客

2015年10月20日 20:55:06[卡哥](https://me.csdn.net/L_Andy)阅读数：3832


1.下载windows下的QT库 QT4.8.5
 for vs2010：

[http://download.qt-project.org/official_releases/qt/4.8/4.8.5/qt-win-opensource-4.8.5-vs2010.exe](http://download.qt-project.org/official_releases/qt/4.8/4.8.5/qt-win-opensource-4.8.5-vs2010.exe)；

![](https://img-blog.csdn.net/20130712153757937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://blog.csdn.net/kernlen/article/details/9311357)

2.下载VS2010 下的QT插件：

[http://download.qt-project.org/official_releases/vsaddin/qt-vs-addin-1.1.11-opensource.exe](http://download.qt-project.org/official_releases/vsaddin/qt-vs-addin-1.1.11-opensource.exe)

![](https://img-blog.csdn.net/20130712154151359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://blog.csdn.net/kernlen/article/details/9311357)

3.安装QT库：点击 **qt-win-opensource-4.8.5-vs2010.exe**；

![](https://img-blog.csdn.net/20130712154316328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一路Next~~

![](https://img-blog.csdn.net/20130712154329781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选中 I acceot the terms of the License Agreement，继续一路Next

![](https://img-blog.csdn.net/20130712154449140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

开始安装，等上大概15分钟的样子。。。

![](https://img-blog.csdn.net/20130712154501031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**至此，QT4.8.5****安装成功了！！！**

4.安装完QT4.8.5后，再安装QT插件：执行**qt-vs-addin-1.1.11-opensource.exe**

**![](https://img-blog.csdn.net/20130712154637453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

**![](https://img-blog.csdn.net/20130712154706578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

**![](https://img-blog.csdn.net/20130712154720562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

5.完成插件的安装后，打开VS2010，在文件-->新建-->项目中就出现了Qt4的模块，菜单条中也多了一项“Qt”：

![](https://img-blog.csdn.net/20130712155033656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6配置需要的Qt的库

![](https://img-blog.csdn.net/20151103233406917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20151103233417453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

配置助手

：![](https://img-blog.csdn.net/20151103233432950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

创建项目：

![](https://img-blog.csdn.net/20130712155046234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6.新建一个QT工程“QtTest1”：

![](https://img-blog.csdn.net/20130712155409484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

之后需要，给VS2010添加QT的库文件和Lib文件，在工程的工程名“QtTest1”上右键单击，选择最下面“属性”栏。

![](https://img-blog.csdn.net/20130712155647609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在  配置属性-->VC++目录的“包含目录”，“引用目录”以及“库目录”栏，相应的找到下载好的QT4.8.5的include,lib,bin文件夹路径

![](https://img-blog.csdn.net/20130712155913218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

至此就可以进行QT编程了，还是以“Hello,world！”作为例子：

**[cpp]**[view
 plain](http://blog.csdn.net/kernlen/article/details/9311357#)[copy](http://blog.csdn.net/kernlen/article/details/9311357#)

- #include "qttest1.h"
- #include <QtGui/QApplication>
- #include <QtGui/QLabel>
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication a(argc, argv);  
-     QLabel *hello_label = new QLabel("Hello, world!");  
-     hello_label->show();  
- return a.exec();  
- }  

启动调试（F5），运行结果如下：

![](https://img-blog.csdn.net/20130712160054625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

7.加载已有QT工程：

      当已经有了QT工程并且没有VS2010可直接打开的sln文件时，可以通过附加的Qt菜单项来打开。在加载之前，需要修改已有QT工程中的.pro文件，先在工程文件夹中找到它，用记事本打开如下：

![](https://img-blog.csdn.net/20130712160226250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20130712160336046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

将INCLUDEPATH和LIBS改为自己的OpenCV库库文件include的路径以及lib文件的路径和常用的lib文件，本人的OpenCV版本是2.4.4，因此需要更改，如下图所示：

![](https://img-blog.csdn.net/20130712160353609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

保存后，打开VS2010菜单上的QT菜单中的Open QtProjectFile(.pro)菜单项，选中并打开刚才修改后的.pro文件，VS2010会自动加载这个QT工程，如下：

![](https://img-blog.csdn.net/20130712160459156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](http://blog.csdn.net/kernlen/article/details/9311357)

启动调试（F5），运行结果如下：

![](https://img-blog.csdn.net/20130712160540953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2Vybmxlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


至此，整个从安装下载到配置，再到跑通第一个例子Hello world，加载已有QT工程的整个过程完成。

PS：第一次写自己的原创笔记，排版以及文字图片等做的不是很好，花了很长时间，但最终还是完成了，加油吧！

