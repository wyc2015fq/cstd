# FTP连接树莓派（Linux）进行文件传输----需要修改vsftpd.conf文件，使能可写 - xqhrs232的专栏 - CSDN博客
2017年03月02日 23:35:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2959
原文地址::[http://blog.csdn.net/madrabbit1987/article/details/53750272](http://blog.csdn.net/madrabbit1987/article/details/53750272)
相关文章
1、树莓派开启FTP传输 SFTP 
 ----[http://stayrunning.blog.163.com/blog/static/196758002201321833946996/](http://stayrunning.blog.163.com/blog/static/196758002201321833946996/)
2、[树莓派安装ftp服务器(转)](http://www.cnblogs.com/jikexianfeng/p/5862130.html)----[http://www.cnblogs.com/jikexianfeng/p/5862130.html](http://www.cnblogs.com/jikexianfeng/p/5862130.html)
3、向vsftp服务器上传文件报“550 Permission denied”错误的解决办法----[http://blog.sina.com.cn/s/blog_8345cdd901014yx6.html](http://blog.sina.com.cn/s/blog_8345cdd901014yx6.html)

1. 在[Linux](http://lib.csdn.net/base/linux)上安装和启动FTP服务
在使用FTP功能之前，树莓派需要安装FTP Server，请输入以下命令安装vsftpd
sudo apt-get install vsftpd
使用以下命令启动FTP服务
sudo service vsftpd start
2. 在主机上下载FlashFXP 
    然后按照如下方式配置：
![](https://img-blog.csdn.net/20161219230900941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFkcmFiYml0MTk4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20161219230936771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFkcmFiYml0MTk4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20161219230930863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFkcmFiYml0MTk4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
就可以进行文件的传输啦~
3. 进行传输的时候，会发现，可以从Linux上下载文件，但是没法上传到Linux的文件夹上。
    提示：
“550 permission denied.”
这个是由于 vsftp的默认配置，不允许上传造成的。
输入指令：sudo nano /etc/vsftpd.conf
找到"write_enable=YES"配置，删除前面的‘#’符号
按下：Ctrl+x，选择y，回车保存；
（修改的时候要非常小心，不要修改到无关的配置）
重启 vsftp服务器：
输入：sudo /etc/init.d/vsftpd restart
再次尝试，已经可以上载文件啦。开心：）
参考文章：http://blog.sina.com.cn/s/blog_8345cdd901014yx6.html
