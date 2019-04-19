# QT5打包教程(QT自带Windep打包工具)----试了可行，但老是要解压！！！ - xqhrs232的专栏 - CSDN博客
2018年07月17日 17:39:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：191
原文地址::[https://blog.csdn.net/csdn_qiwshao/article/details/53516843](https://blog.csdn.net/csdn_qiwshao/article/details/53516843)
软件环境：Qt 5.7.0 
操作系统：Win7 X86(32位)
第一步：打开QT构建设置成Release状态运行，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161211195424203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二步：打开我们构建之后项目所在的位置，一般位置在C:\Qt\Myproject\build-xxx-Release，具体要根据你构建设置的位置打开，全部选择，复制。如下图我们可以看到构建的一些设置。
![这里写图片描述](https://img-blog.csdn.net/20161211200304691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第三步：为了使用的方便，以为现在的项目为例，我在D盘建了个新的文件夹D:\qtfabu，将我们刚才复制的文件粘贴到这个文件夹，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161211200846881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第四步:打开QT自带软件Qt 5.7 for Desktop (MinGW 5.3.0 32 bit)，输入“cd /d D:\qtfabu” 转到项目所在的位置，使用QT自带windep功能将自动建立该项目所需的一些库文件，输入”windeployqt xxx.exe”，这里的xxx代表你点exe文件的名称。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161211201642298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161211201513170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161211204726888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第五步：打开我们刚刚的文件件会发现，比原来多出好的的.dl文件，这时我们将其打包就可以了。打包软件在下面的连接中，也可在网上自行下载，首先选择我们将要打包文件的位置，下一步选择打包的.exe文件，最后的选项可以根据自己的需要设置打包的等级等，其余都可以设置成默认，最后就大功告成了，这时我们就可以生成我们所需要的的.exe文件了，文件位置默认在原文件夹。
![这里写图片描述](https://img-blog.csdn.net/20161211202327091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161211202344466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161211202355512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20161211202407403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3Nkbl9xaXdzaGFv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为工作原因呢，也是第一次接触到QT软件，感觉还是挺容易上手的，对于新手的我还是蛮好的。下面是打包软件的连接，希望大家多多指导交流学习。
[打包软件连接：](http://download.csdn.net/detail/csdn_qiwshao/9708329)[http://download.csdn.net/detail/csdn_qiwshao/9708329](http://download.csdn.net/detail/csdn_qiwshao/9708329)
