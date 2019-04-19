# 【系列好文】ubuntu10.10下eclipse的安装及配置 - 三少GG - CSDN博客
2010年12月21日 19:56:00[三少GG](https://me.csdn.net/scut1135)阅读数：1925
**1、安装JDK**
[http://forum.ubuntu.org.cn/viewtopic.php?t=119815](http://forum.ubuntu.org.cn/viewtopic.php?t=119815)
安装JAVA环境支持
打开终端，执行以下命令：
**代码:**
sudo apt-get install sun-java6-jre
如果空间富裕，建议安装一个JDK(搞开发一定要装吧，要不然还搞咩[乜野])。
**代码:**
sudo apt-get install sun-java6-jdk
上面两个的安装也可以使用Adept/新立得软件管理器，在其中分别搜索"sun-java6-jre"和"sun-java6-jdk"并标记安装
提示：安装过程中需要你回答是否同意使用协议（终端中红蓝色的提示界面），此时按tab键至OK，再按回车即可正常安装。
设置当前默认的java解释器：
**代码:**
sudo update-alternatives --config java
执行后会出现类似如下的画面:
**代码:**
There are 2 alternatives which provide `java'.
Selection Alternative
-----------------------------------------------
1 /usr/bin/gij-wrapper-4.1
*+ 2 /usr/lib/jvm/java-6-sun/jre/bin/java
Press enter to keep the default[*], or type selection number:
输入 有包含 "sun" 的行的前面的数字。如上面显示，则输入2，然后回车确定。
注：如果你是新安装的8.04版本的ubuntu，那么会出现类似"系统只有一个java，不需要设置"的英文提示消息。如下所示：
**代码:**
There is only 1 program which provides java
(/usr/lib/jvm/java-6-sun/jre/bin/java). Nothing to configure.
==================================
================================
参考文章：[http://hi.baidu.com/fetchfire/blog/item/35d09432da30cf44ac4b5f40.html](http://hi.baidu.com/fetchfire/blog/item/35d09432da30cf44ac4b5f40.html)
方法1.环境配置方法似乎有几种，不过我用的是下面这种，试过了，成功了：
＄sudo gedit /etc/profile.d/java.sh
保存下面内容：
#set java environment
export JAVA_HOME=/home/zc/JAVA/jdk1.6.0_10 //这里改成自己JDK的安装目录
export CLASSPATH=./:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/htmlconverter.jar:$JAVA_HOME/jre/lib
export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
============================================================
方法2.配置JAVA环境变量:
**代码:**
sudo gedit /etc/environment
在其中添加如下两行：
**代码:**
CLASSPATH=.:/usr/lib/jvm/java-6-sun/lib#中间是以英文的冒号隔开，记得windows中是以英文的分号做为分隔的
JAVA_HOME=/usr/lib/jvm/java-6-sun
然后：
**代码:**
sudo gedit /etc/jvm
将文件中的
**代码:**
/usr/lib/jvm/java-6-sun(其实也就是JAVA_HOME)
这一行填入到配置块的顶部
关于具体的路径，安装java的时候都会给予提示和说明，请根据实际情况添加，且不可盲目照抄上面的
===========================================================
方法3)  配置java环境变量       [http://www.linuxidc.com/Linux/2009-03/19042.htm](http://www.linuxidc.com/Linux/2009-03/19042.htm)
sudo gedit /etc/environment
加入：
CLASSPATH=.:/usr/lib/jvm/java-6-sun/lib
JAVA_HOME=/usr/lib/jvm/java-6-sun
JRE=/usr/lib/jvm/java-6-sun/jre                                               （Grant：此次采用！！！）
===========================================================
安装浏览器的JAVA Plugin（可选）：
**代码:**
sudo apt-get install sun-java6-plugin
**顺便提一下Ubuntu10.10 里面的环境配置文件**：
（1）/etc/enviroment 是系统的环境变量。
（2）/etc/profile： 是所有用户的环境变量。当用户第一次登录时,该文件被执行. 并从/etc/profile.d目录的配置文件中搜集shell的设置。
（3）/etc/bashrc: 为每一个运行bash shell的用户执行此文件.当bash shell被打开时,该文件被读取。
（4）~/.bash_profile: 每个用户都可使用该文件输入专用于自己使用的shell信息,当用户登录时,该文件仅仅执行一次!默认情况下,他设置一些环境变量,执行用户的.bashrc文件。
（5）~/.bashrc: 该文件包含专用于你的bash shell的bash信息,当登录时以及每次打开新的shell时,该该文件被读取。
（6） ~/.bash_logout:当每次退出系统(退出bash shell)时,执行该文件. 另外,/etc/profile中设定的变量(全局)的可以作用于任何用户,而~/.bashrc等中设定的变量(局部)只能继承 /etc/profile中的变量,他们是"父子"关系。
（7）~/.bash_profile 是交互式、login 方式进入 bash 运行的~/.bashrc 是交互式 non-login 方式进入 bash 运行的通常二者设置大致相同，所以通常前者会调用后者。
**2、设置JAVA_HOME环境系统变量**
　　2.1、这里Ctrl+H可以看到隐藏的文件，比较有用的快捷键。
　　输入：sudo gedit /etc/environment
　　在文件中添加：
　　export JAVA_HOME=/home/administrator/Java/jdk1.6.0_23
　　export JRE_Home=/home/administrator/Java/jdk1.6.0_23/jre
　　export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
　　2.2、同样，修改第二个文件
　　输入：sudo gedit /etc/profile
　　在umask 022之前添加以下语句：
　　export JAVA_HOME=/home/administrator/Java/jdk1.6.0_23
　　export JRE_HOME=/home/administrator/Java/jdk1.6.0_23/jre
　　export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
　　export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH:$HOME/bin
3. 检测JDK版本。
　　输入：java -version
　　即如图：
　　(安装JRE、JDK成功)
==============================================================================
[http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)
eclipse
第一部分，安装。两种方法.
方法一：
- sudo apt-get install eclipse  
    或从ubuntu software center直接下载。这种傻瓜似的方法最方便，但是这种方法得到的往往不是最新版本。对于追求新特性的发烧友来说，可以用下面一种方法。
方法二：
    从官方[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/) 下载最新版本。比如下载的是:
![](http://www.eclipse.org/downloads/images/java.jpg)
[Eclipse IDE for Java Developers](http://www.eclipse.org/downloads/packages/eclipse-ide-java-developers/heliossr1)
。然后转到下载的目录下面：
[view plain](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[copy to clipboard](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[print](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[?](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)
- sudo tar -zxvf eclipse-java-helios-SR1-linux-gtk.tar.gz -C /usr/local   
将eclipse解压到/usr/local。剩下的就和在windows下一样了。直接双击就可运行。但是每次都到/usr/local/eclipse下面运行，很是麻烦。因此可以在桌面上创建启动器或在applications菜单中创建快捷方式。
1.在桌面上创建启动器：
    右键->Create Launcher，然后这样填：
    Type : Application    
    Name : eclipse
    Command : /usr/local/eclipse/eclipse  //即我们安装的地方
    然后点击那个简单的图标，找到/usr/local/eclipse/icon.xpm图标。最后点击OK结束。
2.在applications菜单中创建快捷方式即添加图标：
[view plain](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[copy to clipboard](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[print](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[?](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)
- sudo gedit /usr/share/applications/eclipse.desktop  
     然后添加如下内容：
[view plain](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[copy to clipboard](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[print](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)[?](http://blog.csdn.net/shiyanhui66/archive/2010/10/24/5962231.aspx)
- [Desktop Entry]  
- Encoding=UTF-8  
- Name=eclipse  
- Comment=Eclipse IDE  
- Exec=/usr/local/eclipse/eclipse  
- Icon=/usr/local/eclipse/icon.xpm  
- Terminal=false
- StartupNotify=true
- Type=Application  
- Categories=Application;Development;  
     保存，然后看一下Applications->Programing，怎么样？
第二部分，配置。
1.自动补全
     window->preferences->java->content assist，然后在右侧找到Auto activation triggers for java:可以看到里面只有一个“.”。     将"."的后面加上"abcdefghijklmnopqrstuvwxyz."。保存即可。       
2.常用插件的安装
     eclipse->help->Eclipse Marketplace在里边寻找你想要的各种插件。介绍几款：
     ADT：Android开发插件
     Pydev：Python编程环境
     CDT：C/C++编程环境
其他个性化配置，如字体，Formater等细节就不介绍了。
==========================================
==========================================
==========
**附录：安装Eclipse。**
　　3.1、到官网下载相关的Eclipse，这里下载的是 eclipse-jee-helios-SR1-linux-gtk.tar.gz(即javaEE)。
　　网站：http://www.eclipse.org/downloads/
　　3.2、将下载的 eclipse-jee-helios-SR1-linux-gtk.tar.gz放在Java文件夹内，解压。
　　运行解压后的eclipse，即如图：
**============///////////////////////===============**
**Attention！更改myeclipse-installer的权限：右键->属性->权限允许以程序执行文件 打勾(之前未改所以运行不了 2011.1.5）**
**============///////////////////////=================**
安装MyEclipse插件
MyEclipse相对于eclipse来说，集成了很多进行javaee开发需要的功能，相对于eclipse来说，MyEclipse还是很方便的，现在企业用的也比较多，不过MyEclipse是一个商业软件，只能免费试用30天，30天之后必须提交订阅码才能继续使用。授权费用为标准版是 32美元，专业版是53美元。如果你有兴趣并且也愿意为此掏钱，那么请购买授权，否则想要继续使用，请自行处理，反正我是绝对不会干出告诉你说让你到Google上搜索序列号这种事的。
首先下载MyEclipse Archived Update Site：
官方网址：[http://www.myeclipseide.com](http://www.myeclipseide.com/)
需要选择与相应Eclipse版本匹配的MyEclipse版本(网页上会有说明)，现在MyEclipse最新版为6.5.1 GA，Eclipse3.3可以直接使用这个版本，注意要下载的是MyEclipse Archived Update Site - 6.5.1 GA，界面如下图：
![图片](http://www.fileden.com/files/2007/12/2/1620731/MyEclipse6.5.1.PNG)
简单翻译一下使用方法：
下载后的文件为zip格式，不要解压缩，请确保已经安装Eclipse 3.3；
打开eclipse，在菜单栏依次点击Help > Software Updates > Find and Install > New> Features；
会提示让你添加一个新的Archived Update Site，直接指向你下载后的zip文件，然后一直点击next就可以了。
以前MyEclipse 6.0 GA是以plug-in方式提供对eclipse的升级的，安装方法如下：
下载完MyEclipse的Linux版的plug-in之后，得到的是一个.bin的文件，打开终端，使用cd命令进入MyEclipse安装文件所在目录，输入命令：
$sudo sh MyEclipse_6_0_1GA_E3_3_1_Installer.bin
随后就会启动一个GUI的安装程序。
单击“Next“按钮继续，选择“I accept...“：
在这里选择Eclipse所在目录，单击“Choose“按钮来选择，接下来选择MyEclipse的安装位置，接下来选择Link Folder，可以保持默认选择继续，接下来就可以安装了，单击“Install“按钮，安装好后进入eclipse目录直接双击执行eclipse即可自动识别出MyEclipse
好了，现在你就可以在MyEclipse里面编写你的JavaEE程序了。
=========================================================================
另附：
3.安装MyEclipse
1)启动Eclipse
2)Help > Software Updates > find and Install...
3)选择"Select for new features to install",点<Next>
4)点"New Remote Site."
5) Name输入 "MyEclipse 6 Update Site"
URL: 输入
http://downloads.myeclipseide.com/downloads/products/eworkbench/discovery-6.5/
点<OK>
6) 点 "Finish."
7) Search Results中可以看到"MyEclipse 6 Update Site"下有4个
ICEfaces AJAX Tooling for MyEclipse version 6.6.0
Matisse4MyEclipse version 6.6.0
MyEclipse Enterprise Workbench version 6.6.0
MyEclipse Reports version 6.6.0
我们只选择MyEclipse Enterprise Workbench version 6.6.0
7) As usual, we'll need to accept the license agreement before proceeding.
点<Next>
8) Feature License
点选： I accept the terms in the license agreement
<Next>
9)Installation
<Finish>
