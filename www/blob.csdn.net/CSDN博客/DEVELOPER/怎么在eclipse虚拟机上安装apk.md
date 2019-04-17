# 怎么在eclipse虚拟机上安装apk - DEVELOPER - CSDN博客





2014年03月27日 14:05:05[学术袁](https://me.csdn.net/u012827205)阅读数：5104








说来惭愧，这个操作我一直在身边的朋友那里学习。但是就是学不会！但是今天，交项目要用到这地方进行测试，所以我就要上网搜索自学了。呵呵，不巧我搜到了一篇文章是关于这方面的。然后看看结合之前的认知，最终学会了这门“高深”的技术！

接下来，就让我们一起看看，它原来也是如此简单：

在这里给大家分享一篇在Android 模拟器中安装apk文件的方法，本文以微信安装为例<.在这里，我把微信的apk包名改了---->wechat.apk>



1.下载和安装模拟器文件

本文在google官网下载的文件，

如下图示文件：<这个是我的文件>
![](https://img-blog.csdn.net/20140327133029140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在exception中启动一个虚拟机：就像下面这样


![](https://img-blog.csdn.net/20140327135133453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





当然最后出现模拟器的界面，在此期间要执行一段时间。启动之后是这样的：





![](https://img-blog.csdn.net/20140327134940500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2.将需要安装的apk文件复制到platform-tools目录下，新建文件夹newapk,把下载的apk文件放进去，该文件就是即将要安装的apk文件。如本实例的 “wechat.apk”

![](https://img-blog.csdn.net/20140327140008234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140327140032484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


3.执行控制台命令，进行安装。

点击开始→运行，输入**cmd**，打开cmd窗口。切换到D盘，输入**D:**,然后点击**Enter**，即切换到D盘，

输入**cd**，找到**platform-tools**的文件地址，即**adb.exe**的文件路径。（可以通过文件搜索adb.exe 或者platform-tools查找到）,粘贴在控制台中。

4.执行安装文件的命令：命令为：**adb install apk文件路径和地址**

输入**adb install**，然后

把要安装的apk文件地址复制过来，直接点击右键，即可粘贴，不用ctrl+v的方式。如下图示：

![](https://img-blog.csdn.net/20140327134748828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




瞧，下面的就是安装成功的微信了：


![](https://img-blog.csdn.net/20140327135433906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






