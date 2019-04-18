# Qt 5.7 版本+2013VS环境配置 - L_Andy的专栏 - CSDN博客

2017年10月10日 14:37:31[卡哥](https://me.csdn.net/L_Andy)阅读数：609



原本是使用MSVC5.5.1的版本，碍于习惯，之前一直使用creator，后面才转向VS，因为它的调试实在是太棒了，小程序还是creator调试(比如抽出大程序中的小模块之类的)

不知道是版本问题还是我自己笔记本问题，5.5.1这个在添加的资源中使用图片一直路径失败，4.6又能成功调用到资源，在VS中也可以，然后手残卸载了MSVC5.5.1，同理，原本配置好在2013VS中Qt也随之毁了。接下来我要向大家说下怎么配置5.7+MSVC2013(2015一样的)

    这里下载qt5.7 windows64位的 x86是32位的，注意下，info后面有信息备注是2013MSVC还是2015MSVC

    我的是2013_64位的

    https://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-windows-x86-msvc2013_64-5.7.0.exe.mirrorlist

    然后下载对应的2013VS的Qt5插件  [Visual Studio Add-in 1.2.5 for Qt5](http://download.qt.io/official_releases/vsaddin/qt-vs-addin-1.2.5.exe)  在other
 download下载可以找到对应的(1.2.5对应是5.7的其他版本使用1.2.4要不然会出现版本不对应 this qt version xxxx····)

    https://download.qt.io/official_releases/vsaddin/qt-vs-addin-1.2.5.exe.mirrorlist

    msvc自己选路径安装，我则是跟4.6一样默认安装于 C:\Qt\...  中

    这时候打开VS看到这边工具栏出现了Qt5![](https://img-blog.csdn.net/20171010143732890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后点击qt option

在这里找到你刚才安装的qt5.7的路径将msvc2013_64文件夹路径选定即可。![](https://img-blog.csdn.net/20171010143738176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171010143742640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后ok完成，即可创建一个qt的项目。直接创建一个helloword即可，这里就不在展示。

另外附上一个炒鸡好用的插件，【懒人插件，自动补全的，也可以自定义head file】

## Visual Assist X

![](https://img-blog.csdn.net/20171010143746437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171010143750317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

完成之后生成的规则就在这里，可以在更改，也有官方手册，当前就琢磨到这里，后面有增加在补充。

使用方法如下：![](https://img-blog.csdn.net/20171010143754201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171010143758311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后天下对应的参数，对比如下：

![](https://img-blog.csdn.net/20171010143802313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

$这里的参数则是显示上图左边的字符$

![](https://img-blog.csdn.net/20171010143806132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击ok则插入了注释。

![](https://img-blog.csdn.net/20171010143809575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

