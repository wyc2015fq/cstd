# Windows下搭建IOS开发环境（一） - xqhrs232的专栏 - CSDN博客
2018年12月04日 14:50:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：41
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/lizhenmingdirk/article/details/29850159](https://blog.csdn.net/lizhenmingdirk/article/details/29850159)
相关文章
1、Windows下搭建IOS开发环境(二）----[https://blog.csdn.net/lizhenmingdirk/article/details/31784441](https://blog.csdn.net/lizhenmingdirk/article/details/31784441)
原文：[http://blog.csdn.net/shangyuan21/article/details/18153605](http://blog.csdn.net/shangyuan21/article/details/18153605)
我们都知道开发iPhone等ios平台的移动应用时需要使用Mac本，但是Mac本都比较昂贵，所以我们可以采用Windows7上利用VMWare安装Mac操作系统的方法来模拟ios开发环境，达到降低成本的目的。
1.相关配置
操作系统：windows7旗舰版
Work station：VMWare10
Mac操作操作系统：OS X 10.8
2.相关资源下载地址
(1)VMWare10的资源比较多，可以到网上自己下载
[http://www.xp510.com/xiazai/ossoft/desktools/22610.html](http://www.xp510.com/xiazai/ossoft/desktools/22610.html)
(2)OS X 10.8下载地址
[http://download.csdn.net/detail/tianxuexuankui/6838565](http://download.csdn.net/detail/tianxuexuankui/6838565)
(3)VMWare上的Mac补丁
[http://download.csdn.net/detail/tianxuexuankui/6838579](http://download.csdn.net/detail/tianxuexuankui/6838579)
(4)dmg格式转换成iso格式的工具UltraISO
[http://www.cngr.cn/dir/209/271/2009032738116.html](http://www.cngr.cn/dir/209/271/2009032738116.html)
3.安装过程
a.安装VMWare
检测CPU是否支持虚拟化 检测工具 securable.exe  
我是Lenovo的机子，Win 7，
检测到
![](https://img-blog.csdn.net/20140610155529218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl6aGVubWluZ2Rpcms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
表明处于关闭状态（Locked OFF并不能表明CPU是否支持，到底支持不支持，请到主板BIOS中，看是否有VT的开启开关，开后，如显示为ON那就成功了，如还是OFF，那表明CPU不支持。
我的设置过程如下：重新开启——F1——BIOS——Advanced——cpu Setup——intel Virtualization Technology——Enabled——F10保存退出。
下载Vmware安装包后，直接安装就可以了。
b.安装VMWare的Mac补丁
将补丁文件下载之后，解压缩。打开安装文件的windows目录，右键以管理员身份运行install.cmd
c.创建虚拟机
VMWare安装好之后，选择"文件"---"新建虚拟机"，进入到创建虚拟机的界面，如下
![](https://img-blog.csdn.net/20140111225824109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择自定义，点击下一步
![](https://img-blog.csdn.net/20140111225901515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择“稍后安装操作系统”，点击“下一步”
![](https://img-blog.csdn.net/20140111225948140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
操作系统类型上选择“Apple Mac OS X”，版本上选择10.8，点击“下一步”
![](https://img-blog.csdn.net/20140111230119250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
设置虚拟机的名称和安装的路径，点击“下一步”
![](https://img-blog.csdn.net/20140111230225312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
设置好处理器数量之后，点击下一步
![](https://img-blog.csdn.net/20140111230306531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
设置内存，然后下一步
![](https://img-blog.csdn.net/20140111230345640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里，将网络连接方式最好设置成桥接，这样就可以和物理机实现同网段。点击下一步
![](https://img-blog.csdn.net/20140111230453312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来几步，都可以设置为默认方式
![](https://img-blog.csdn.net/20140111230526250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140111230546765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140111230604640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
设置好硬盘的容量好，点击下一步
![](https://img-blog.csdn.net/20140111230648781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
默认名称即可，点击下一步
![](https://img-blog.csdn.net/20140111230726203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击完成，结束创建
使用下载的工具UltraISO将dmg格式的mac os x文件转换成iso格式的。
1.打开UltraISO,加载dmg
![](https://img-blog.csdn.net/20140610165958640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl6aGVubWluZ2Rpcms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.打开dmg后，选择格式转换，选择标准ISO
![](https://img-blog.csdn.net/20140610170226015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl6aGVubWluZ2Rpcms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ISO转换完成后，右键刚刚创建好的虚拟机名称，选择设置选项，对虚拟机进行编辑，将iso文件进行关联，具体操作如下
![](https://img-blog.csdn.net/20140111232605781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上的所有的一切完毕之后，点击开启虚拟机就可以了。
首先进入的是apple的logo界面，如下
![](https://img-blog.csdn.net/20140111232804843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
稍等一段时间之后，进入到语言的选择界面
![](https://img-blog.csdn.net/20140111232841890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里我选择的是简体中文，然后点击向右的箭头，进入下一步
![](https://img-blog.csdn.net/20140111232922671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择使用工具里面的磁盘工具，进行分区
![](https://img-blog.csdn.net/20140111233021484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
按照上图的方式进行设置后，对分区进行命名
![](https://img-blog.csdn.net/20140111233140578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择刚刚创建的分区，对数据进行格式化，如下图所示操作。点击右下角抹掉
![](https://img-blog.csdn.net/20140111233246453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关闭分区工具，选择重新安装OS X
![](https://img-blog.csdn.net/20140111233346046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击继续按钮
![](https://img-blog.csdn.net/20140111233440671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择同意条款，进入下一项设置
![](https://img-blog.csdn.net/20140111233520031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择刚刚创建的分区，点击安装选项进行安装
![](https://img-blog.csdn.net/20140111233555015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装的过程大概需要半小时左右的时间，等待安装完成之后，进行简单的设置，包括开机密码、Apple ID和Icloud、find my Mac等操作。就可以进入到Mac OS X 10.8的操作系统了。界面如下
![](https://img-blog.csdn.net/20140111234517796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hhbmd5dWFuMjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装好Mac操作系统后，打开里面的浏览器就可以直接进入到Apple的官方网站，在网站里面搜索xcode就可以进入到Xcode的下载界面
这里需要注意下，不是所有的Xcode都可以使用的，Xcode和Mac OS X操作系统需要一定的相互匹配关系的，具体的情况可以参考下面
[Xcode和Mac os x对应关系](http://bbs.pcbeta.com/viewthread-1228156-1-1.html)
