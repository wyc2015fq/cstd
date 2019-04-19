# 坚屏改横屏教程（apk反编译基础教程） - xqhrs232的专栏 - CSDN博客
2012年04月17日 22:04:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1922
原文地址::[http://www.google.com.hk/url?q=http://bbs.gfan.com/android-1164690-1-1.html&sa=U&ei=bnaNT6jYEIjqrQf32vXJCQ&ved=0CBgQFjAB&usg=AFQjCNGewTy8Pl1XKi7qfOb3YrWnt1USPw](http://www.google.com.hk/url?q=http://bbs.gfan.com/android-1164690-1-1.html&sa=U&ei=bnaNT6jYEIjqrQf32vXJCQ&ved=0CBgQFjAB&usg=AFQjCNGewTy8Pl1XKi7qfOb3YrWnt1USPw)
相关网帖
1.软件横屏+全屏修改教程----[http://bbs.gfan.com/mobile/android-3517394-1-1.html?extra=page%3D1%26amp%3Bfilter%3Dtype%26amp%3Btypeid%3D47](http://bbs.gfan.com/mobile/android-3517394-1-1.html?extra=page%3D1%26amp%3Bfilter%3Dtype%26amp%3Btypeid%3D47)
 2.onConfigurationChanged信息处理----[http://www.eoeandroid.com/thread-74654-1-1.html](http://www.eoeandroid.com/thread-74654-1-1.html)
**重要提示：本教程只对部分软件有效，本人也是菜鸟！绝大多数问题我也是一知半解或一无所知，所谓“教程”只是提供大家相互学习而已，不敢言教。**
**一、坚屏改横屏的基本思路（基本过程）**
Apk软件安装包里面有一个文件“AndroidManifest.xml”里面有一行Activity脚本来实施对屏幕的控制：其语句：android:screenOrientation=
 "landscape" 是将软件强制指定为横屏；
android:screenOrientation= "portrait"则将软件强制指定为竖屏。
我们先试将一个apk软件的扩展名“.apk”改为“.zip”，然后解压，看压缩包目录下是否有个AndroidManifest.xml文件，但这修文件是不能直接打开的，必须经过反编绎后才能打开进行编辑。我要做的就是将软件包转化为可以编辑的文件。
基本步骤如下：
1、准备所需软件，
2/安装软件（废话）
3、利用apktool软件将要改变的软件进行反编译，转化为可编译文件
4、利用文本编辑器改写“AndroidManifest.xml”文件中相关脚本变为横屏控制
5、再利用apktool软件将改变后的文件重新打包成apk文件
6、用签名软件AutoSign对新产生的apk文件签名（没有签名的软件是不能安装的）
7、余下的就是安装将你所得到的横屏版软件安装在你的爱机中进行试验了。
**二、反编译所需软件准备**
1、软件要在java环境下运行，所以第一步要下载安装JRE或者JDK，并在系统环境变量Path里加入java.exe所在路径。
软件下载地址：大小76.5M
[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
![](http://image.gfan.com/images/default/attachimg.gif)![1.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152242d0d24f8512680f43.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MTk4fGY4YjYyNTZhfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (50.67 KB) 
??java
2011-4-15 22:42
2、安装jdk-6u24-windows-i586.exe，然后并在系统环境变量Path里加入java.exe所在路径：过程如下：
右击“我的电脑”调出属性，
![](http://image.gfan.com/images/default/attachimg.gif)![2.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152246364ad9fd22697b03.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MjUxfGY2ODlmZDgyfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (52.94 KB) 
2
2011-4-15 22:46
![](http://image.gfan.com/images/default/attachimg.gif)![3.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152246d83286877db5bc34.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MjUyfGUyMWNiOTU3fDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (54.52 KB) 
3
2011-4-15 22:46
![](http://image.gfan.com/images/default/attachimg.gif)![4.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152246b667ba7d7885abd6.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MjUzfGNlODA4MmEzfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (51.84 KB) 
4
2011-4-15 22:46
输入安装路径：我的是：C:\Program Files\Java\jdk1.6.0_24\bin
然后确定：搞定运行环境
3、下载反编译工具apktools
（1）下载地址：[http://code.google.com/p/android-apktool/](http://code.google.com/p/android-apktool/)
下载apktool-install-windows-2.2_r01-3.tar.bz2和apktool1.3.2.tar.bz2两个包。
![](http://image.gfan.com/images/default/attachimg.gif)![5.jpg](http://attachments.gfan.com/attachments2/day_110415/110415224899a2396b4419a323.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MjY5fDBkNTA4NmY2fDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (11.59 KB) 
5
2011-4-15 22:48
（2）然后解压缩下载的两个文件包，共得到三个文件：aapt.exe，apktool.bat和apktool.jar（注意要把apktool1.3.2.tar.bz2解压后的一个apktool.jar文件copy进来）。把三个文件全部copy电脑C盘根目录下（我是这样做的，因为软件要在dos下运行，这样做方便输入路径，注意一定不能放在中文路径下，否则dos识别不了路径），保守点再将apktool.jar文件copy一份到C:\windows目录下。
（3）点:”开始——运行”,再输入”cmd”，调出dos，输”cd
 c:\” 转到C根目录下（你的apktool所在路径），输入
 “apktool”试一下，如果出现使用帮助，说明可以用了
![](http://image.gfan.com/images/default/attachimg.gif)![6-1.jpg](http://attachments.gfan.com/attachments2/day_110415/11041522510d2aa75d78109221.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MjkzfGY4ZWMwNjc0fDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (78.76 KB) 
6-1
2011-4-15 22:51
## 实施案例——将UC7.6竖屏改为横屏
本帖最后由 wengong 于 2011-4-18 15:26 编辑
**三实施案例——将UC7.6竖屏改为横屏**
（1）把网上下载到的“UCWEB-7.6.0.75-800-139-110301714.apk”文件更名为“UC.apk”这样做的目的仅仅是为了dos输入指令简单。并将文件UC.apk放到C般根目录下（与apktool放在同一目录下）
（2）打开命令窗口（开始>运行，输入cmd，回车。）进入到apktool.bat的文件夹里。
输入： apktool d uc.apk  c:\uc（命令行解释：apktool
 d要反编译的文件”uc.apk”输出到文件夹“uc”，注意“uc”文件夹是软件会自动建立的文件夹，不要你建立，否则软会因为文件夹重名而无法继续运行，特别注意：你要反编译的文件一定要放在C盘的根目录里）
成功后画面：
![](http://image.gfan.com/images/default/attachimg.gif)![6-2.jpg](http://attachments.gfan.com/attachments2/day_110415/11041522559e98b489d729f242.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MzMyfDdkYjZlNjUzfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (39.38 KB) 
2011-4-15 22:55
（3）打开 c:\uc文件夹就可以得到我们学习的可编译资源了。
（4）找到这个文件AndroidManifest.xml，用记事本打开
![](http://image.gfan.com/images/default/attachimg.gif)![7.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152256ec95058482540b42.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MzM3fGNlYjlkYTdmfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (15.43 KB) 
7
2011-4-15 22:56
然后查找一下看是否有android:screenOrientation= "landscape"语句，
![](http://image.gfan.com/images/default/attachimg.gif)![8.jpg](http://attachments.gfan.com/attachments2/day_110415/11041522575f591e77da3de56d.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3MzUzfDY5MzdiOGM5fDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (78.21 KB) 
8
2011-4-15 22:57
如果有接下来就是“用替换命令将全部“portrait”替换成“landscape”
[attach]987365[/attach]
很简单，只要输入：  apktool
 b c:\uc  （b命令是打包命令，打包你编译出来文件夹）
等一会，搞定如下：
![](http://image.gfan.com/images/default/attachimg.gif)![10.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152259571a8301090d0d06.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3Mzc3fDAwNTk4ODMzfDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (18.2 KB) 
10
2011-4-15 22:59
然后从C:\uc\dist文件夹下找到新得到的文件“uc.apk”
（6）还差一步，这个软件还不能安装，因为还没有签名，所以最后一步是签名
我是用AutoSignUpdatePackage.exe，签名的，这个软件会被360杀毒软件识别为恶意软件，我设置360信任这个软件，虽然运行时老提示恶意软件，但不会自动清除。就是算是恶意软件，知已知彼也不会伤害到我的。
这个软件一样不能在中文目录下运行的
![](http://image.gfan.com/images/default/attachimg.gif)![签名.jpg](http://attachments.gfan.com/attachments2/day_110415/1104152301eafc992a337b88c9.jpg)
[**下载**](http://bbs.gfan.com/attachment.php?aid=OTg3Mzk0fGY3YjgyNDE2fDEzMzQ2NzEzOTZ8ZjA4NThyc2dwcEorT0tnbXVDc25wNXBqNEt6VXA1eWU0bXJrRml1cFhUc1JyQUU%3D&nothumb=yes)
 (44.61 KB) 
11
2011-4-15 23:01
签名后得到重新命名的文件uc-Signed.apk
到此大功造成，安装吧！祝你成功
三楼有软件
## 附件下载网盘地址
本帖最后由 wengong 于 2011-4-18 15:35 编辑
1、 反习编绎软件apktool： ![](http://bbs.gfan.com/images/attachicons/binary.gif)[**apktool.apk**](http://bbs.gfan.com/attachment.php?aid=MTAwMjgwMHxjNmMzMGE5ZHwxMzM0NjcxMzk2fGYwODU4cnNncHBKK09LZ211Q3NucDVwajRLelVwNXllNG1ya0ZpdXBYVHNSckFF)
 (2.43 MB)
下载次数: 82
2011-4-18 15:35
（没有上传压缩文件的权限，请将apk改名为RAR文件解压得到三个文件即可，请放到C盘根目录下）
2、签名软件：—— ![](http://bbs.gfan.com/images/attachicons/binary.gif)[**签名软件.apk**](http://bbs.gfan.com/attachment.php?aid=MTAwMjc3NXw3NTFjNTFkYnwxMzM0NjcxMzk2fGYwODU4cnNncHBKK09LZ211Q3NucDVwajRLelVwNXllNG1ya0ZpdXBYVHNSckFF)
 (249.7 KB)
下载次数: 152
2011-4-18 15:25
(没有上传压缩文件的权限，请将apk改名为RAR文件解压即可，签名软件，要更改文件，360会报毒，请临时信任即可）
3、横屏版UC7.6—— ![](http://bbs.gfan.com/images/attachicons/binary.gif)[**横屏版UCWeb7.6.apk**](http://bbs.gfan.com/attachment.php?aid=MTAwMjczOHxiMDQzZWI3ZHwxMzM0NjcxMzk2fGYwODU4cnNncHBKK09LZ211Q3NucDVwajRLelVwNXllNG1ya0ZpdXBYVHNSckFF)
 (2.26 MB)
uc
下载次数: 57
2011-4-18 15:21
