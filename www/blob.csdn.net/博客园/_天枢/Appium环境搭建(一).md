# Appium环境搭建(一) - _天枢 - 博客园
## [Appium环境搭建(一)](https://www.cnblogs.com/yhleng/p/7423689.html)
2017-08-24 16:17 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7423689)
python环境做测试，需要准备工具如下：
1、python2.7(这里使用的是python2你也可以选更高版本)
2、Appium(Window版)
3、Android SDK
4、Appium_Python_Client
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
**一、Android SDK**
下载地址：
[http://www.androiddevtools.cn/](http://www.androiddevtools.cn/)  此链接网面下边SDK TOOL
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907155701804-580202661.png)
这里下载的ZIP格式的，解压就可以使用，EXE还需要安装；解压后目录如下：
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907161156897-2101898242.png)
 由于是直接下载的ZIP，所以需要自己设置系统环境变量，如果是EXE安装的，会自己配置，无需手动
1）添加系统变量
变量名:ANDROID_HOME
变量值:SDK所在根目录
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907161142413-267456094.png)
 2）配置系统变量PATH
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907161727069-1188399223.png)
3） 双击:SDK Manager.exe,打开如下界面
从界面上看有一些下载失败了，这是因为天朝，把google给和谐了。
我们可以从国内镜像下载
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907162535819-1412149651.png)
 可以从国内镜像下载:
1、中科院开源协会镜像站地址:
IPV4/IPV6 : [http://mirrors.opencas.ac.cn](http://mirrors.opencas.ac.cn/) 端口：80
2、北京化工大学镜像服务器地址：
IPv4: [http://ubuntu.buct.edu.cn/](http://ubuntu.buct.edu.cn/)  端口：80
IPv4: [http://ubuntu.buct.cn/](http://ubuntu.buct.cn/)  端口：80
IPv6: [http://ubuntu.buct6.edu.cn/](http://ubuntu.buct6.edu.cn/)  端口：80
3、大连东软信息学院镜像服务器地址:
[http://mirrors.neusoft.edu.cn](http://mirrors.neusoft.edu.cn/)  端口：80
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907162813929-671094059.png)
我使用的是大连东软镜像:
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907162925804-968546629.png)
 Tool选择
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907163151507-1247221688.png)
 版本选择：新手直接选最新的了，向下兼容
<android模拟器> 这里版本选反了andriod8.0.0   ；image用于AVD Manager模拟使用，如果是连接真机，可以不安装
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907163235569-1539369079.png)
 下载安装完成界面
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907164344194-780722506.png)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907164405054-1912814719.png)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170911103206047-1990829494.png)
如上图，安装完成，点击AVD Manager.exe打开android模拟器界面
下面双击“AVD Manager.exe”创建android模拟器。
![](https://images0.cnblogs.com/blog2015/311516/201506/081030270982001.png)
点击“Create....”
![](https://images0.cnblogs.com/blog2015/311516/201506/081030434416326.png)
如果显示屏分辨率比较底的话，尽量选择低分辨率的“Device”。
点击“OK”，在AVD  Manager 窗口，点击“Start...”按钮启动android 模拟器。
![](https://images0.cnblogs.com/blog2015/311516/201506/081031005514754.png)
因为新虚拟机没了实体键，所以我们可以利用键盘按键来操作android虚拟机。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
二、安装appium
1.下载地址:
[https://bitbucket.org/appium/appium.app/downloads/](https://bitbucket.org/appium/appium.app/downloads/)
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170907160119382-1768644502.png)
2.直接双击appium-installer.exe文件安装就好，桌面会生成一个appium的图标
![](https://images2015.cnblogs.com/blog/765861/201608/765861-20160819151534125-818229577.png)
3.把node_modules的bin目录放到系统的Path路径里
*          C:\Program Files (x86)\Appium\node_modules\.bin*
*![](https://images2015.cnblogs.com/blog/765861/201608/765861-20160819151646562-563145021.png)*
4.检查appium所需的环境是否OK：
进入cmd命令行，输入appium-doctor ，出现以下提示，All Checks were successful ,说明环境成功。
![](https://images2015.cnblogs.com/blog/765861/201608/765861-20160819151734187-447930720.png)
 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
三、Appium-Python-Client 安装
cmd命令行模式下:
1) pip install appium-python-client
2) pip install selenium
安装成功，会有Seuccess等字样
