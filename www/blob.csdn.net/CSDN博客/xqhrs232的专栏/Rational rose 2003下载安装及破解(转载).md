# Rational rose 2003下载安装及破解(转载) - xqhrs232的专栏 - CSDN博客
2009年06月07日 21:46:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2178
下午从一点钟就开始下载rose，用了一个多小时的时间下载完毕，安装吧，不错，顺利安装完毕！
然后＂开始＂－“程序”－“rationgalsoftware”－“Rational
 Rose Enterprise Edition”，开始出现“ratonalrose”界面，以为就此搞定，不过“啪“谈出一个对话框“MEN-BAD-POINTER”！完蛋，赶紧上网查看是什么问题！没破解！！！晕，那为老兄也是，也不给说明白点！哎，自己在查破解文件吧！后来在一个博客的主页上找到了。这时一看表又耗费了我一个小时的时间！
下面是我记录的下载安装及破解所查看的一些网站和资料：
rational rose下载：
[http://tseg.org/~dxiao/SEPractice/Rational2003/RationalRoseEnterpriseEditionforWindows.2003.06.00.391.000.exe](http://tseg.org/~dxiao/SEPractice/Rational2003/RationalRoseEnterpriseEditionforWindows.2003.06.00.391.000.exe)
rational
 rose解压密码：
          d3d3LnBocHYubmV0
rational rose破解文件为：
[http://zixi.bokee.com/inc/sdasdas.rar](http://zixi.bokee.com/inc/sdasdas.rar)
如果还不能破解，就按照下面的步骤来做：
1.安装Rose后,默认是需要许可证书的..去下载个破解的..
[http://www.cnblogs.com/Files/lixianhuei/rose2003crack.rar](http://www.cnblogs.com/Files/lixianhuei/rose2003crack.rar)
2.先用破解压缩包里的 rational.exe，lmgrd.exe
覆盖到你的 /安装目录的Rartional/commen/下
3.然后记事本打开
 license.dat, 修改里面的 SERVER yourPC ANY DAEMON rational "C:/Program Files/Rational/Common/rational.exe"
改成 SERVER
你的机器名 ANY DAEMON rational "你的安装目录/rational.exe"
 ,拷贝到Common目录下..
4.
将Flexlm.cpl拷贝到C:/winnt/system32/下，在控制面板里运行 FlexLm License Manager，
运行后,
在 Setup
面板配置文件路径，lmgrd.exe ->
你的安装目录 /Common/lmgrd.exe,
而 License File
为你改过的 license.dat ...
（我用的是xp，目录为C:/WINDOWS/system32）
5.在Control面板点击Start，如果成功的话点击Status按钮将显示你的机器名：license server UP (MASTER)
说明成功了
失败的话重启一下FlexLm License Manager就没问题了。
6.如果弹出对话框License
 Key Administrator Wizard后,
选定Point to a Rational License Server to get my licenses，单击下一步，
Server Name文本框中填写你的机器号(可能已经填好)，单击完成。
 (成功的话会出现两屏的licenses)
只是把自己的经历写出来而已，以求共享，也免的别人再走一些弯路
Trackback: [http://tb.blog.csdn.net/TrackBack.aspx?PostId=1326754](http://tb.blog.csdn.net/TrackBack.aspx?PostId=1326754)
**如果上面的方法还不行的话,请下载shw32.dll文件，把它替换你安装目录下的rose文件夹的那个：**[http://matrix.foresee.cn:8080/forum/servlet/JiveServlet/download/13-5186-42049-605/shw32.dll.zip](http://matrix.foresee.cn:8080/forum/servlet/JiveServlet/download/13-5186-42049-605/shw32.dll.zip)
我自己的做法:
我的安装文件有:
![](http://hiphotos.baidu.com/javajavajava/pic/item/d4f1c0f2da6ad718b17ec55d.jpg)
安装完成后,
           1    把[shw32.dll](http://matrix.foresee.cn:8080/forum/servlet/JiveServlet/download/13-5186-42049-605/shw32.dll.zip)(118k)文件放到D:/Program
 Files/Rational/Rose下,替换原有文件.
           2    把 rational_perm.dat文件放到D:/Program Files/Rational/Common下.
rational_perm.dat下载：[http://zixi.bokee.com/inc/sdasdas.rar](http://zixi.bokee.com/inc/sdasdas.rar)
一切OK!
当然上面的路径是我自己的.
