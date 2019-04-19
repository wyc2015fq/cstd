# cc debugger固件下载 - xqhrs232的专栏 - CSDN博客
2015年03月13日 09:48:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：977
原文地址::[http://blog.csdn.net/lgupen/article/details/18451061](http://blog.csdn.net/lgupen/article/details/18451061)
相关文章
1、smartRF04eb（CC2430仿真器） 0042 固件----[http://download.csdn.net/detail/lazycat53128/2307141](http://download.csdn.net/detail/lazycat53128/2307141)
2、smartrf04eb fw0045----[http://download.csdn.net/detail/zuyuanvip/5667477](http://download.csdn.net/detail/zuyuanvip/5667477)
3、**ZIGBEE仿真器——SmartRF04EB——制作----[http://www.amobbs.com/thread-5065131-1-1.html](http://www.amobbs.com/thread-5065131-1-1.html)**

由于在做zigbee项目，下载器用的是cc debugger，使用没什么问题，就是有次莫名其妙的电脑不能识别cc debugger了，红灯一直亮着，所以上网找了官方的固件进行了下载，终于修复好那个cc debugger，现在记录下修复过程，留着备用。
使用工具：    (1)SmartRF Flash Programmer软件
(2)SmartRF Studio 7软件
(注意，以上软件可到TI官网上下载)
第一步，我们先下载bootloader文件到板子上，我们可以使用另外一个好的cc debugger来完成bootloader下载这一步，步骤如下。
![](https://img-blog.csdn.net/20140118151517234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140118151639718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样我们就完成了bootloader的下载，这时我们看到红灯一直在闪烁，这表明我们完成第一步了。
有时我们好的cc debugger不能检测到坏的cc debugger，这时我们可以使用另外个工具来完成第一步。
使用操作同上，完成后红灯闪烁。
smartRF04EB仿真器如下图。
![](https://img-blog.csdn.net/20140410090638109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**也可以采用硬件方式复位**
![](https://img-blog.csdn.net/20150306085212433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用杜邦线短接GND和P1.6脚然后点击下复位按键，这时板子的红灯闪烁，然后我们接着按照下面的步骤进行更新固件。
第二步，我们将固件下载到板子上，我们拔掉那个好的cc debugger，使用USB线接上那个要修复的cc debuger上，插上电脑，接下来按以下步骤来
![](https://img-blog.csdn.net/20140118153337265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140118153441296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
双击下出现的设备，也就是CC Debugger这个选项，然后弹出的对话框中，点击YES，然后等他更新完成，然后点击Done。
第三步，也是最后一步。
![](https://img-blog.csdn.net/20140118152435390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140118152744921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGd1cGVu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样我们就完成了cc debugger固件的下载。
