# Windows 7 远程登录 Fedora 16 - xqhrs232的专栏 - CSDN博客
2016年04月14日 15:38:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：549
原文地址::[http://blog.csdn.net/bitowang/article/details/10760755](http://blog.csdn.net/bitowang/article/details/10760755)
相关文章
1、[在windows通过Xrdp软件远程桌面连接Fedora](http://www.cnblogs.com/xkfz007/articles/2176867.html)----[http://www.cnblogs.com/xkfz007/articles/2176867.html](http://www.cnblogs.com/xkfz007/articles/2176867.html)
2、Win7远程控制fedora ——通过xrdp----[http://blog.sohu.com/s/MTU5MTY3OTE1/302888160.html](http://blog.sohu.com/s/MTU5MTY3OTE1/302888160.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
在windows 7 有时需要远程登录fedora的桌面，容易想到windows下的mstsc命令进行远程桌面的连接。
网上常有的教程就是使用vncserver软件进而解决，但是在高版本的Fedora中gnome桌面会有桌面分享功能,可以在终端中使用vino-preferences进而打开，既方便又快捷，使用VNCServer的软件肯定有其不一样的性能。在使用桌面分享功能时，需要安装xrdp，可以使用yum
 install xrdp命令进行安装，需要root用户的权限，安装完毕，打开xrdp的服务，可以使用service xrdp start命令进行，也是需要root权限，也可以通过setup命令对服务进行开机自启动。
完成安装后，在Windows7下在运行中输入mstsc即可打开远程桌面的连接，输入正确的ip地址即可，注：Fedora下xrdp默认是3389端口，故可以直接输入ip地址，如若改变，需要加上端口号，即 IP地址：端口号。
下面是图片的显示配置的整个过程：
1 打开 桌面分享 功能  
![](https://img-blog.csdn.net/20130831195136109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0b3dhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：图中根据需要选中
        2  安装xrdp
![](https://img-blog.csdn.net/20130831195538281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0b3dhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3 开启 xrdp 服务
![](https://img-blog.csdn.net/20130831195604062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0b3dhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4 开机自启动服务 
![](https://img-blog.csdn.net/20130831195744859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0b3dhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130831195808812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0b3dhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：在使用的过程中，空格是用来选择服务名称的（显示为*）。TAB键用来选择OK 等菜单。
[](http://blog.csdn.net/bitowang/article/details/10760755#)[](http://blog.csdn.net/bitowang/article/details/10760755#)[](http://blog.csdn.net/bitowang/article/details/10760755#)[](http://blog.csdn.net/bitowang/article/details/10760755#)[](http://blog.csdn.net/bitowang/article/details/10760755#)[](http://blog.csdn.net/bitowang/article/details/10760755#)
- 
顶
0- 
踩
