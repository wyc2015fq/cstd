# 用脚本实现windows与linux之间文件的传输 - xqhrs232的专栏 - CSDN博客
2017年10月31日 14:34:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：13652
原文地址::[http://blog.csdn.net/shufac/article/details/51966276](http://blog.csdn.net/shufac/article/details/51966276)
相关文章
1、Xshell实现Windows上传文件到Linux主机----[http://www.linuxidc.com/Linux/2015-05/117975.htm](http://www.linuxidc.com/Linux/2015-05/117975.htm)
2、在WIN下向LINUX上传文件的几种方法----[http://blog.csdn.net/to_cm/article/details/3930637](http://blog.csdn.net/to_cm/article/details/3930637)
3、用rz,sz命令在windows和linux之间传输文件----[http://blog.csdn.net/jacklee1212/article/details/45026015](http://blog.csdn.net/jacklee1212/article/details/45026015)
4、windows和Linux互传文件----[http://blog.csdn.net/u012494321/article/details/53119907](http://blog.csdn.net/u012494321/article/details/53119907)
5、[Linux与Windows之间ftp自动传输文件](http://blog.csdn.net/lk_db/article/details/51556334)----[http://blog.csdn.net/lk_db/article/details/51556334](http://blog.csdn.net/lk_db/article/details/51556334)
6、利用securecrt在linux与windows之间传输文件----[http://blog.csdn.net/zhangxiaoyang0/article/details/67634728](http://blog.csdn.net/zhangxiaoyang0/article/details/67634728)
7、SecureCRT
 在linux和windows之间的文件传输----[http://blog.csdn.net/partner2016/article/details/52387934](http://blog.csdn.net/partner2016/article/details/52387934)
# 用脚本实现windows与linux之间文件的传输
## 需求
1.在Windows系统上的某个文件需要传送到Linux系统的设备上；
2.因为自动化的要求不能人工操作，需要编写脚本，然后在程序中调用cmd命令执行这个脚本。
下面总结一下这个需求的实现步骤。
## 实现
### 安装WinSCP工具
因为执行脚本需要用到WinSCP.exe。这个工具获取地址：[http://download.csdn.net/detail/shufac/9581003](http://download.csdn.net/detail/shufac/9581003)
安装完成之后登陆Linux主机，
![](https://img-blog.csdn.net/20160720112906505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图1 WinSCP登陆配置示意图
查看一下待传送文件的路径，用于操作完成之后后面验证。
### 编写脚本
[plain][view
 plain](http://blog.csdn.net/shufac/article/details/51966276#)[copy](http://blog.csdn.net/shufac/article/details/51966276#)
- # WinSCP.exe /console /script=sample.txt  
- # Automatically answer all prompts negatively not to stall  
- # the script on errors  
- # option echo  on|off  
- option echo off  
- # option batch on|off|abort|continue  
- option batch on  
- # option confirm  on|off   
- option confirm off  
- # option transfer  binary|ascii|automatic   
- # option synchdelete  on|off  
- # option exclude clear | [;...]  
- # option include clear | [;...]  
- # open [ sftp|ftp|scp:// ][ [ :password ] @ ] [ : ]  
- # open user:password@example.com  
- # Connect     
- open  scp://root:name@192.168.1.1:22  
- #open [scp:// ][ [ :password ] @ ] [ : ]  
- # Change remote directory  
- # cd /home/user              #远程工作目录  
- cd /tmp  
- # Change local directory  
- # set to Self's working dir   
- #lcd D:\XXXXXX.bin                   #本地工作目录  
- put C:\Users\Administrator\Desktop\XXXXXXXXX.bin  
- # Force binary mode transfer  
- option transfer binary  
- # Download file to the local directory d:\  
- # get examplefile.txt d:\  
- # option synchdelete  on|off  
- option synchdelete off       #是否同步删除    
- # option include clear | [;...]  
- # option include /2008-*-*/;/2009-*-*/;/2010-*-*/;/2011-*-*/;/2012-*-*/;/2013-*-*/  
- # synchronize local|remote|both [ [ ] ]   
- # 传输方式：Local为远程至本地，remote为本地到远程，both双向传输  
- #使用关键字synchronize的话，将会同步本地目录下的文件至远程目录  
- #synchronize remote  
- remote  
- # Disconnect  
- close  
- # Exit WinSCP  
- exit  
说明：
我这里需要传送的文件只有一个二进制文件，所以后面不需要同步，如果需要操作整个目录的文件可以用synchronize remote。
### 执行脚本
在Windows系统中运行cmd，通过WinSCP.exe执行这个脚本即可。WinSCP默认是安装在C:\Program Files\WinSCP目录下的，cmd命令如下：
![](https://img-blog.csdn.net/20160720113704653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2 cmd命令示意图
## 结果
脚本执行过程如下图所示，
![](https://img-blog.csdn.net/20160720113228319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3  文件传送过程示意图
通过WinSCP登陆Linux主机，在root/tmp目录下可以看到传送的那个.bin文件，如下图所示，
![](https://img-blog.csdn.net/20160720114323757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图4 文件传送结果示意图

