# 怎么使用Clutch进行ipa的破解工作 - xqhrs232的专栏 - CSDN博客
2019年02月28日 15:09:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：111
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/weixin_36667844/article/details/79818021](https://blog.csdn.net/weixin_36667844/article/details/79818021)
相关文章
1、iPhone ipa 软件破解工具 Clutch 使用教程----[https://blog.csdn.net/DavidHsing/article/details/7049866](https://blog.csdn.net/DavidHsing/article/details/7049866)
2、ipa文件解压/破解----[https://blog.csdn.net/sucen1987/article/details/42456101](https://blog.csdn.net/sucen1987/article/details/42456101)
3、苹果ipa软件包破解笔记----[https://blog.csdn.net/LANGZI7758521/article/details/52288214](https://blog.csdn.net/LANGZI7758521/article/details/52288214)
# 使用工具介绍
iFunBox --用于打开你的手机的各个文件夹，[通俗易懂](https://www.baidu.com/s?wd=%E9%80%9A%E4%BF%97%E6%98%93%E6%87%82&tn=24004469_oem_dg)的来说，它就是你的手机资源管理器。
Cluth--一个技术大牛实现的破解第三方软件，可以不需要你懂任何底层的语言，会它的几个命令就可以进行破解了。
ipa包--就是你要破解的APP。
# 破解工作
1.首先去github下载Cluth的最新版本，https://github.com/KJCracks/Clutch/releases
。
2.利用Ifunbox打开你的手机，在最左边的菜单栏打开我的MAC--越狱设备（如显示iPhone5C.IOS7.1.2）--文件系统。
![](https://img-blog.csdn.net/20180404152621535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zNjY2Nzg0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将下载下来的Clutch放到Device-usr-bin内部，就完成了手机的准备工作。
3.然后打开已经越狱的Iphone手机，点开设置，wifi,获取相应的IP地址。
打开终端，输入ssh root@192.168.5.52(这处应该替换成你的IP地址)。
然后继续，第一次进入要设置，有一个数字模式，输入yes（非常重要)，此时提示要输入密码 ，默认密码是“alpine”.
进入我们放的文件夹usr-bin内部，即在命令行输入cd /usr/bin.
此时应该就可以输入命令Clutch - i来查看你手机下的应用目录，如下图所示的蓝色和紫色部分。如果不可以，显示Permisson Deny,输入chmod a+x Clutch。
![](https://img-blog.csdn.net/20180404152636642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zNjY2Nzg0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们决定来破解4号文件，在命令行内输入Clthch2 -d 4.就可以看到命令行开始执行了。
执行完毕后，命令行会告诉你破解好的文件在哪个位置。
![](https://img-blog.csdn.net/20180404152649150?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zNjY2Nzg0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
