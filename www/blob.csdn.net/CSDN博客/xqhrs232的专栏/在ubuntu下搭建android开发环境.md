# 在ubuntu下搭建android开发环境 - xqhrs232的专栏 - CSDN博客
2012年05月02日 15:57:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：580
原文地址::[http://www.cnblogs.com/candycaicai/archive/2011/02/25/1964940.html](http://www.cnblogs.com/candycaicai/archive/2011/02/25/1964940.html)
相关网帖
1.Windows下Android开发环境搭建和配置----[http://www.metsky.com/archives/523.html](http://www.metsky.com/archives/523.html)
2. Android教程之一：Windows下搭建Android开发环境----[http://flysnow.iteye.com/blog/808189](http://flysnow.iteye.com/blog/808189)
3. Ubuntu下安装Android开发环境经历----[http://abc20899.iteye.com/blog/1044057](http://abc20899.iteye.com/blog/1044057)
一、Java环境安装配置
Ubuntu java安装配置的详细步骤
1.下载jdk
下载连接http://java.sun.com/javase/downloads/index.jsp
选择jdk-6u24-linux-i586.bin下载,我下载的是最新的版本，将jdk-6u24-linux-i586.bin放置于目录/home/candycai/work(这个自己定义文件夹，无所谓)。
2.解压文件
打开终端，进入放置jdk的目录：cd /home/candycai/candywork;
更改文件权限为可执行chmod +x jdk-6u24-linux-i586.bin,这样该目录下就会出现jdk1.6.0_24的文件。
3、配置环境变量
sudo gedit /etc/profile，在profile文件最后添加
export JAVA_HOME=/home/candycai/candywork/jdk1.6.0_24
export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar
export PATH=$PATH:$JAVA_HOME/bin
其中的意思是：载入java根目录；设置环境变量（应用文件）；设置环境变量（类文件）
然后必须重启系统！
4、查看版本
java -version
结果如下，可参考
java version "1.6.0_24"
Java(TM) SE Runtime Environment (build 1.6.0_24-b07)
Java HotSpot(TM) Server VM (build 19.1-b02, mixed mode)
二、Eclipse安装及android安装配置
1、下载Eclipse
下载地址：http://www.eclipse.org/downloads/ ,我下载的最新的Eclipse IDE for Java Developers，为eclipse-java-helios-SR1-linux-gtk.tar.gz。
2、解压
将其解压到/home/candycai/work这个之前设定好的目录。双击eclipse就能启动了。
下载Android SDK程序包最新的(android-sdk_r10-linux_86.tgz.tar)，把它解压到自己的工作目录下，解压后目录名为android-sdk-linux_86，按照之前的方法sudo
 gedit /etc/profile，在PATH添加tools目录，修改后如下：
export PATH=$PATH:$JAVA_HOME/bin:/home/candycai/candywork/android-sdk-linux_86/tools
3、配置ADT
启动Eclipse，菜单Help
 -> Install New Software… -> 选择选项卡上的Available Software，
![](http://pic002.cnblogs.com/images/2011/175362/2011022515272885.png)
(图一)
点击右侧按钮“Add ...”，弹出对话框，设置如下：
Name：ADT
Location :http://dl-ssl.google.com/android/eclipse/
单击“OK”返回，返回后单击“Select
 All”，再单击两次“Next >”，然后 就是漫长的等待，我的网速太慢。
也可以直接从网上下载好现成的文件，安装，我从网上找的最新的ADT-10.0.0.zip，按照同样的方法
![](http://pic002.cnblogs.com/images/2011/175362/2011022515280824.png)
（图二）
之后按照步骤安装便可以了。
4、设置SDK路径
Eclipse重启后，选择Window
 > Preferences...，在打开的属性框，选择Android,在SDK
 location中输入你的Android SDK所在的目录，然后按Apply按钮.
5、设置Android SDK and AVD Manager
AVD:Android Virtual Device Android虚拟设备 
选择 window--Android SDK and AVD Manager在弹出的
Android SDK and AVD Manager对话框。
![](http://pic002.cnblogs.com/images/2011/175362/2011022515282439.png)
（图三）
选择 Available Packages 然后选择你要安装的sdK包，最后按Install
 Selected按钮进行安装。 
由于下载的太慢了，我就用下载安装包的方式跟新sdk。
[android
 开发包的离线安装方式](http://www.cnblogs.com/candycaicai/archive/2011/02/25/1964881.html)
参考：[http://www.cnblogs.com/candycaicai/archive/2011/02/25/1964881.html](http://www.cnblogs.com/candycaicai/archive/2011/02/25/1964881.html)
通过以上的方式已经成功搭建好了环境。
6、创建一个模拟器
Window->Android SDK and AVD Manager”菜单项所弹出的对话框界面，左边切换到“Virtual
 Devices”项，在右边单击“New…”,自己按照自己的需求添加参数，创建一个sdk版本的模拟器（我是举例子截图，只是参考）。单击“Create
 AVD”就创建好了一个Android运行的虚拟设备。自己可以点击start..
![](http://pic002.cnblogs.com/images/2011/175362/2011022515285278.png)
（图四）
创建好后，可以start,跑起模拟器看看：
到此为止，整个搭建过程已经结束，下面就可以自己建立android工程，从hello
 world开始了。
![](http://pic002.cnblogs.com/images/2011/175362/2011022515293036.png)
参考文档
[http://blog.sina.com.cn/s/blog_3e4774e30100lfr7.html](http://blog.sina.com.cn/s/blog_3e4774e30100lfr7.html)
[http://hanhaotian87.javaeye.com/blog/755895](http://hanhaotian87.javaeye.com/blog/755895)
//=================================================
//备注
1.安装平台的话可以通过eclipse下的ADT插件来安装。在tools目录下有个叫android的文件，这个文件对应于windows下android sdk的SdkManager.exe.通过这个软件可以去下载安装SDK包
2.ADT在线安装不了，可以去下载离线包再安装
3.Eclipse在线更新不了，可以去下载新版本，这样就面去了更新的过程
4.代理上网的话就要设置代理什么的
$ export HTTP_PROXY=http://<proxy_user_id>:<proxy_password>@<proxy_server>:<proxy_port>
$ export HTTPS_PROXY=http://<proxy_user_id>:<proxy_password>@<proxy_server>:<proxy_port>
5.[通过代理上网怎么下载Android源代码](http://blog.csdn.net/xqhrs232/article/details/7226147)----[http://blog.csdn.net/xqhrs232/article/details/7226147](http://blog.csdn.net/xqhrs232/article/details/7226147)
