# win8及以上系统的.NET Framwork3.5的安装 - Big Smile - CSDN博客
2015年12月24日 17:29:26[王啸tr1912](https://me.csdn.net/tr1912)阅读数：816
         这两天每当有同学重装系统，就会有人遇到.NET Framwork3.5的安装问题，其实这个问题在百度上已经有了很方便的答案了，那就是用装系统的镜像文件来添加功能，具体步骤如下：
首先要先加载系统镜像：
![](https://img-blog.csdn.net/20151224173137233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后把镜像的盘符记录下来，按win+x调出开始右击菜单（最高权限系统直接win+r）输入cmd：
![](https://img-blog.csdn.net/20151224173522977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后输入镜像管理的命令：dism.exe /online /enable-feature /featurename:NetFX3 /Source:X:\sources\sxs 其中X是你的虚拟光驱盘符。等待大概10分钟左右
![](https://img-blog.csdn.net/20151224173833201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后.NET Framework功能就添加完成了，就可以装SQLserver了。
具体上这个映像部署和管理工具是什么，怎么用，请看下面链接；
[http://blog.csdn.net/pkgfs/article/details/8300966](http://blog.csdn.net/pkgfs/article/details/8300966)
[命令大全](http://zhanqiguang.blog.163.com/blog/static/46507962201332844141766/)
