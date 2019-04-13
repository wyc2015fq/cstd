
# linux系统进程管理之glances工具的使用 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月06日 13:32:51[Jlins](https://me.csdn.net/dyllove98)阅读数：1841


上一个博文介绍了htop工具的使用，这里介绍一下glances工具的使用，我个人是比较喜欢这款工具的，主要就是由于glances这款工具可以将系统状态导出为文件，如果导出为html格式的则可以配合apache或者nginx等web程序来直接打开网页对服务器资源的查看。
既然有优点那就也有缺点，不能通过上下箭头来选择要查看的某指定进程，也无法查看所有进程，它只能显示对系统资源占用较大的进程。不过它能很好的查看当前计算机里的网络、磁盘读写、磁盘分区等情况。
glances的官方首页：[http://nicolargo.github.io/glances/](http://nicolargo.github.io/glances/)这里可下载对应的源码包
![wKioL1SBlfHSo_cNAAREqB739A4643.jpg](http://s3.51cto.com/wyfs02/M02/54/6A/wKioL1SBlfHSo_cNAAREqB739A4643.jpg)
此处为glances打开的界面，我把磁盘读写隐藏了。如果没有隐藏的话再左下角就会显示DISK I/O而不是现在的MOUNT。
先来说说glances的常用选项：
-f /path/to/somefile：设置输出文件的位置
-o {html/csv}：设置输出文件的格式
-m：隐藏挂载磁盘
还有一些常用选项也可以直接在可视化界面使用例如下面的
-b：以字节（byte）为单位显示网卡速率
-d：隐藏磁盘IO
-n：隐藏网络IO
-t：指定刷新时间间隔
-1：每个cpu的数据单独显示
接下来就针对以上内容做个演示
先对输出文件做演示吧，这个挺给力的.....这里需要注意的是-o 和-f需要同时使用，-o指定导出类型 -f则指定导出位置（我导出到web目录下）当执行这个命令后程序会在/var/www/html目录下生成一个glances.html文件并且程序会继续正常运行。
![wKioL1SBw0Wix0fBAAD1pQuZn9c707.jpg](http://s3.51cto.com/wyfs02/M02/54/6B/wKioL1SBw0Wix0fBAAD1pQuZn9c707.jpg)
执行效果图：（这里没办法演示动态图，请各位自行尝试下吧。）
![wKiom1SBw7rjYfIlAAX0dWKrfFo869.jpg](http://s3.51cto.com/wyfs02/M01/54/6D/wKiom1SBw7rjYfIlAAX0dWKrfFo869.jpg)
来进入glances获取一下帮助信息吧，在glances的左下角会有提示按下h获取帮助（我这里用的是ubuntu所以会显示中文）
![wKiom1SBxQbx2xLAAARUIkIXM34239.jpg](http://s3.51cto.com/wyfs02/M02/54/6D/wKiom1SBxQbx2xLAAARUIkIXM34239.jpg)
目测看到这张图后许多英语不好的朋友就看到了希望。。下面那些参数不做过多解释就只看下颜色高亮那里。分别代表三个级别：注意、警告、临界（其实到了第三级别目测就挂掉了。。可以理解为结束）
然后下面分别代表了限度所以请严格注意系统使用情况，当超过第一限度就需要注意下了。突然不指定该怎么写了。感觉这个都不用演示。哎。小伙伴们多动手吧，本来想再写点但是真不知道咋写了原谅我。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)更多关于[android开发](http://www.itmmd.com/mobile.html)的文章

