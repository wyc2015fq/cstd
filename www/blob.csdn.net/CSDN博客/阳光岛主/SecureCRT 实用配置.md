
# SecureCRT 实用配置 - 阳光岛主 - CSDN博客

2013年07月27日 00:40:22[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：32095


**Secure****[CRT](http://baike.baidu.com/view/489722.htm)**，是一款支持 SSH2、SSH1、Telnet、Telnet/SSH、Relogin、Serial、TAPI、RAW 等协议的终端仿真程序，最吸引我的是，SecureCRT 支持标签化 SSH 对话，从而可方便地管理多个 SSH 连接，设置项也极为丰富。简单的说是Windows下登录UNIX、Linux服务器主机的软件。SecureCRT支持SSH，同时支持Telnet和rlogin协议。
SecureCRT是一款用于连接运行包括Windows、UNIX、VMS的理想工具。通过使用内含的VCP命令行程序可以进行加密文件的传输。有流行CRT Telnet客户机的所有特点，包括：自动注册、对不同主机保持不同的特性、打印功能、颜色设置、可变屏幕尺寸、用户定义的键位图和优良的VT100、VT102、VT220、ANSI竞争，能从命令行中运行或从浏览器中运行，其它特点包括文本手稿、易于使用的工具条、用户的键位图编辑器、可定制的ANSI颜色等.SecureCRT的SSH协议支持DES,3DES和RC4密码和密码与RSA鉴别。

**1、 SecureCRT**[下载](http://www.vandyke.com/download/securecrt/download.html)

**2、 SecureCRT 配置颜色**
直接修改全局选项，以免每加个服务器都要重新设置一次：
![](https://img-blog.csdn.net/20130727003813593)

全局设置：
![](https://img-blog.csdn.net/20130727003833328)

键盘方案的选择：终端——》仿真
终端选择“Linux”， 勾选“ANSI颜色” 和 “使用颜色方案”，终端颜色配置请见下面后续...
![](https://img-blog.csdn.net/20130727003915593)

字体的配置：终端——》外观
注意颜色方案选“白 / 黑”，设置字体大小：字体(F)
![](https://img-blog.csdn.net/20130727003941734)
如果出现中文乱码，修改上面的“字符编码(H)“，一般为”**UTF-8**“

注意下面的字符集一定要选择“中文 GB2312 ”：（中文选用，英文不必）
![](https://img-blog.csdn.net/20130727004139421)

配置完后，登录终端默认颜色如下：
![](https://img-blog.csdn.net/20130727140156468)
目录颜色与底色（黑色）非常相近，不易看清

**终端全局颜色设置：**
Options ——》 Global Options ——》 Terminal ——》 Appearance ——》 ANSI Color
设置 "Bold Colors" 蓝色的颜色为自己喜欢的清晰颜色，如下图：
![](https://img-blog.csdn.net/20130727135930125)
配出来的效果：
![](https://img-blog.csdn.net/20130727140205937)
注： shell 脚本中的颜色，也会由深蓝色变为上面的淡蓝色，如下图：
![](https://img-blog.csdn.net/20130727140404171)

SecureCRT连接后如果稍长时间不用就会掉线，往往会造成工作状态的丢失。如下设置可以始终保持SecureCRT的连接:
![](https://img-blog.csdn.net/20130727004323234)

如下图：
![](https://img-blog.csdn.net/20130727004351968)

大显示器上若SecureCRT显示不能占满整个屏幕，可通过 Global Options/Terminal/Appearance/Maximum columns 来调整：
![](https://img-blog.csdn.net/20130727004434140)


**3、 SecureCRT 上传下载**
SecureCR下的文件传输协议有：ASCII、Xmodem、Ymodem、Zmodem
![](https://img-blog.csdn.net/20130727111113859)
文件传输是数据交换的主要形式，在进行文件传输时，为使文件能被正确识别和传送，我们需要在两台计算机之间建立统一的传输协议，这个协议包括了文件的识别、传送的起止时间、错误的判断与纠正等内容。常见的传输协议有以下几种：
1）ASCII：这是最快的传输协议，但只能传送文本文件。
2）Xmodem：这种古老的传输协议速度较慢，但由于使用了CRC错误侦测方法，传输的准确率可高达99.6%。
3）Ymodem：这是Xmodem的改良版，使用了1024位区段传送，速度比Xmodem要快。
4）Zmodem：Zmodem采用了串流式（streaming）传输方式，传输速度较快，而且还具有自动改变区段大小和断点续传、快速错误侦测等功能。这是目前最流行的文件传输协议。
除以上几种外，还有Imodem、Jmodem、Bimodem、Kermit、Lynx等协议，由于没有多数厂商支持，这里就略去不讲。
SecureCRT可以使用Linux下的zmodem协议来快速的传送文件，你只要设置一下上传和下载的默认目录就行
options->session options ->Terminal->Xmodem/Zmodem 下，在右栏Directories设置上传和下载的目录：
![](https://img-blog.csdn.net/20130727111415625)

**上传文件**
使用Zmodem从客户端上传文件到linux服务器
1）先用SecureCRT登陆Linux终端的目录下，该目录是你存放上传文件的目录
2）在Linux下选中你要放置上传文件的路径，并在此目录下然后输入rz命令，SecureCRT会弹出文件选择对话框，在查找范围中找到你要上传的文件，按Add按钮；然后OK，就可以把文件上传到linux上了。
3）或者在Transfer->Zmodem Upoad list弹出文件选择对话框，选好文件后按Add按钮；然后OK，窗口自动关闭；最后在linux下选中存放文件的目录，输入rz命令；Liunx就把那个文件上传到这个目录下了。

**下载文件**
使用Zmodem下载文件到客户端：
sz filename
zmodem接收可以自行启动，下载的文件存放在你设定的默认下载目录下（上图Directories设置上传和下载的目录 设置的目录）
rz，sz是Linux/Unix同Windows进行ZModem文件传输的命令行工具，
Windows端只要支持ZModem的telnet/ssh客户端，SecureCRT就可以用SecureCRT登陆到Unix/Linux主机（telnet或ssh均可）
1） 上传：运行命令rz，是接收文件，SecureCRT就会弹出文件选择对话框，选好文件之后关闭对话框，文件就会上传到当前目录
2） 下载：运行命令sz file1 file2就是发文件到windows上（保存的目录是可以配置） 比ftp命令方便多了，而且服务器不用再开FTP服务了
3） rz上传大文件，需要添加参数 -e，例如上传 test.log大文件（超过100m），则用命令：rz-etest.log
注： 如果rz、sz命令无效，所以请大家先安装[rzsz-0.12.20-853.2.i586.rpm](http://rpm.pbone.net/index.php3/stat/4/idpl/18526630/dir/opensuse_12.x/com/rzsz-0.12.20-991.1.2.x86_64.rpm.html)



**参考推荐：**
[SecureCRT 配置](http://dearymz.blog.163.com/blog/static/20565742012997190599/)
[Secure CRT好用的设置](http://hi.baidu.com/dbaeyes/item/fdb1d196263a9ebecc80e54b)
[Linux下的SecureCRT破解方法](http://blog.chinaunix.net/uid-26760055-id-3130060.html)
[Windows连接Linux的常用工具](http://blog.csdn.net/sunboy_2050/article/details/6270003)



