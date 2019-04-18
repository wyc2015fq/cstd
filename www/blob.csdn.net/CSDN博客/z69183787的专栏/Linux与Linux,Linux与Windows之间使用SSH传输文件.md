# Linux与Linux,Linux与Windows之间使用SSH传输文件 - z69183787的专栏 - CSDN博客
2015年06月25日 15:13:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：17895
Linux与Linux之间传送文件:
scp file username@hostIP:文件地址 例:
scp abc.txt endall@IP:/home/endall/abc.txt
windows与Linux之间传送文件:
笔者用putty的内置PSCP。其它如 SSH Secure File Transfer Client
先在WIN的DOS下，PUTTY目录下及PSCP文件所在目录。
pscp file username@hostIP:文件地址
例:pscp d:abc.txt endall@IP:/home/endall/abc.txt
pscp endall@IP:/home/endall/abc.txt d:abc
Linux到Windows类似
Windows如何用 putty的 psftp 从 Linux上传与下载
Windows 下运行 psftp.exe ，登录后改变在Linux下的目录： 
cd /usr/
改变在Windows下上传和下载的目录:
lcd d:/
这个目录在 psftp 中不能列出来，我觉得这点不太好，如果不改变的话就是默认putty的安装目录，默认下载到这儿，上传的文件也是要在这个文件夹里面
比如在 d：盘有一个 5.rmvb
上传文件到Linux： psftp > put 5.rmvb
这个命令会把 d:\5.rmvb 上传到 /usr/5.rmvb
从Linux下载文件： psftp > get 4.rmvb
这个命令会把 /usr/4.rmvb 下载到 d:\4.rmvb
许多人使用简易的SSH连接工具，有时候需要在SSH下复制文件到本地查看比较方便，我给大家介绍一个简单的命令SCP。[@more@]scp是有Security的文件copy，基于ssh登录。操作起来比较方便，比如要把当前一个文件copy到远程另外一台主机上，可以如下命令。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)scp /home/daisy/full.tar.gz
 root@172.19.2.75:/home/root
然后会提示你输入另外那台172.19.2.75主机的root用户的登录密码，接着就开始copy了。
如果想反过来操作，把文件从远程主机copy到当前系统，也很简单。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)scp root@172.19.2.75:/home/root /home/daisy/*.tar.gz
