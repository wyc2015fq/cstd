# IIS7的FTP设置 - 左直拳的马桶_日用桶 - CSDN博客
2010年12月16日 10:24:00[左直拳](https://me.csdn.net/leftfist)阅读数：7028
哎呀！IIS7的FTP搞死人啦！
大概是这样子的：
1、新建一个WINDOWS帐号，比如说，sbcoder
2、打开IIS7，新建FTP站点，按照向导，一步一步。有些选项需要勾选：
1）“SSL”允许
![](https://img-blog.csdn.net/20131217172920515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2）身份验证，勾上“基本”
3）授权：指定用户，sbcoder；权限：读、写
![](https://img-blog.csdn.net/20131217172946953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、在操作系统下，保证sbcoder对FTP目录有修改、读写的权限。
4、有一个前提是，服务器的端口21是开的（也学防火墙之类已经将它关闭鸟）
