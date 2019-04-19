# windows的映射驱动器 - Big Smile - CSDN博客
2017年01月23日 00:22:55[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1375
        我们都知道电脑中的驱动器是什么，就是我们电脑里面的C盘，D盘等都叫做驱动器，或者叫做磁盘。那么网络驱动器是什么呢，这篇文章主要讲解一下网络驱动器以及他的搭建和使用。![](https://img-blog.csdn.net/20170116204843746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 一、什么是映射驱动器
        定语最后是驱动器，那么他就一定和我们常见的驱动器有相同的地方，首先有一点就是网络这个词，既然是网络驱动器，他们的存储空间就会来自网络，具体是什么网络呢？是局域网内的共享的一个存储目录。我们可以把一个共享的目录映射到我们的电脑上来当做一个驱动器来使用。
        他们的使用方法和常规的驱动器是一样的，他们有专用的盘符，只要网络够好的的话，我们就可以通过盘符直接访问他们，填写路径的时候也直接填写指定的盘符加上文件路径就可以了。并且只要连接状态不断开，我们就可以一直远程访问这个远程的驱动器。
      这样可以提高我们访问一个特定地方的方便性，也可以让我们很方便的可以不通过网络地址来访问，直接通过盘符访问这样使得一些特殊情况下的文件处理变得异常方便。
# 二、驱动器的使用
        说了这么多了，我们要怎么建立映射驱动器呢？
        首先我们需要先再局域网上的一台电脑上设置一个共享的文件夹，用户是EveryOne，权限是读写（为了文件使用方便），然后再把共享需要输入的密码去掉就好了，详细步骤如下：
建立文件夹（win7以上系统快捷键：shift+ctrl+n）：![](https://img-blog.csdn.net/20170116210853404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后设置共享，在文件夹的属性中点击共享标签，然后点击共享按钮：
![](https://img-blog.csdn.net/20170116210900810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后添加EveryOne账户，并设置权限：
![](https://img-blog.csdn.net/20170116211156554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170116211202939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
设置好权限之后点击共享就共享成功了，接下来设置无密码共享，进入网络和共享中心，然后点击更改高级共享设置 ：
![](https://img-blog.csdn.net/20170116211402301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后把所有网络中的密码保护的共享关闭就可以了：
![](https://img-blog.csdn.net/20170116211539115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后在另外一台电脑上打开我的电脑或者这台电脑，在上方找到映射网络驱动器：
![](https://img-blog.csdn.net/20170116211940901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击打开映射网络驱动器，填写好驱动器盘符和映射的文件夹的网络路径（精确到文件夹）即可：
![](https://img-blog.csdn.net/20170116212208183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这时你的这台电脑或者我的电脑里面会出现你刚添加的驱动器：
![](https://img-blog.csdn.net/20170116212258937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样网络驱动器就算添加完毕了。
# 三、用处
        这样的文件共享一般用在局域网的文件传输和文件发布，比如学校的学生机就可以映射一个教师机，然后就可以随时上交作业和查看作业要求了，还有在企业中可以通过设置映射来对应各个部门来下发文件或者收取文件（一个部门映射一个文件夹）。映射驱动器的用处多多，欢迎同志们多多尝试。
