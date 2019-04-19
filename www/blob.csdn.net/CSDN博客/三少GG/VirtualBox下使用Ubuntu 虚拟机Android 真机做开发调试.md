# VirtualBox下使用Ubuntu 虚拟机Android 真机做开发调试 - 三少GG - CSDN博客
2013年04月09日 22:22:08[三少GG](https://me.csdn.net/scut1135)阅读数：2886

一。 virtualbox下很难将两个网络设在同一网段下，失败
Debugging Your Android App Wirelessly on an Android Smartphone
### Introduction
Developing for Android is very easy to set up and get started on your computer.  However, a gripe shared among others, not just me, is that the Android emulator*is too slow*.  On my old computer, it would take 1-2 minutes for it to boot up.  In debugging
 mode, the response time is very slow.
There are two methods:
- Connect your phone to your computer and find the drivers online (if needed) to allow adb to recognize your phone as a running Android device.  Eclipse and the ADT plugin will take care of the rest.
- *The super dumb way* is to take the .apk file that your Android project generates every time it compiles (at yourappfolder/bin/yourapp.apk), connect your phone to the computer via USB, push it to your phone, and install.  But that is also very inefficient
 (and dumb)!  What if you just wanted to test a small change real quick?  You would have to keep your phone tethered to the computer, and constantly overwrite your .apk file and re-install your application each time you wanted to test it on your phone.
I have a way that allows you to test your Android application on your Android smartphone without**physically**connecting your phone to your computer.  All it requires is a rooted Android smartphone, and a shared wifi network between the computer
 and the phone.
***Note: ***This method requires a phone with root access, and accessing adb over a wifi network might be a security concern for some.  Take this method for what it’s worth.  If you are doing this on your own home network that is trusted
 and encrypted there shouldn’t be a problem, but I would avoid doing this in a public wifi network.
### What to do on your phone
Make sure you are connected to the same local network that your computer is on via**WiFi.  You will then need an app called**[**adbWireless**](https://market.android.com/details?id=siir.es.adbWireless&feature=search_result)**, it allows a rooted phone to allow adb connection to your phone as if it was connected by USB.  Once you install adbWireless and allow root privileges, press the giant red button to begin.**
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/5568035211_7dcd1ed9ed.jpg)
It will provide you with a one-line command to run in your command prompt, something similar to “adb connect 192.168.1.106:5555″.  You can type “adb devices” afterwards to verify that your device is successfully connected.  That is all you have to do on
 the phone.
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/2011-03-16_2103.png)
### What to do on your computer
Go to Eclipse and click on the green play button so a drop-down list appears.  Press “Run Configurations”.
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/2011-03-16_2043.png)
Next, press “Target”.  Change the radio button from “Automatic” to “Manual”.  Press “Apply” then “Run”.
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/2011-03-16_2043_001.png)
An Android Device Chooser window should appear and your device should be listed as a running Android device.  Select it and press OK.
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/2011-03-16_2044.png)
Your phone should begin downloading the app and launching it as soon as it completes.  You can verify in the Console window on Eclipse that adb is running and uploading/installing the .apk on your phone.
### What about debugging?
It is pretty cool that you can wirelessly push your app to your phone to test.  However, what if you can run debug mode in Eclipse while you step through the app on your phone?  Well you can through this method, and it is faster than the emulator.  It is
 not that much different from the method described above.
Make sure in your AndroidManifest.xml that “android:debuggable=true” is there.  The easiest way to add this is to go to the “Application” tab in the manifest, and in “Application Attributes”, find “Debuggable”, set it to
 “true”, and then save.  By default, my phone denies the app from being run in debug mode if the application did not explicitly declare themselves as debuggable in their manifest.  So, make sure debuggable is set to true in your AndroidManifest!
![](http://www.jessechen.net/blog/wp-content/uploads/2011/03/2011-03-16_2046.png)
Once you do that, just create breakpoints, and begin the debug mode.  Step through your app on your phone, and Eclipse will automatically switch to the Debug perspective when it hits a breakpoint.  This is seriously really cool.
### Conclusion
I tried searching on Google to see if anyone else had written about this cool technique, but I wasn’t able to find anything so I decided to write my own how-to.  I was in the process of uninstalling apps that I don’t use anymore, and when I stumbled upon
 adbWireless which I downloaded but never used before, I thought about this possibility.  I tried it, and it worked.  This is, in my opinion, better than the other two methods mentioned above.  Laziness is good.
二。尝试后 失败
## virtualbox ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项
[http://blog.sina.com.cn/s/blog_4868f98601017xnl.html](http://blog.sina.com.cn/s/blog_4868f98601017xnl.html)
原先整了一个ubuntu虚拟机环境，现在发现有个严重问题：虚拟硬盘创建的太大了（30G），不好拷贝。故决定重新整一个环境出来。顺便回忆一下当初环境是怎么搞的。
一） windows主机用virtualbox  装ubuntu系统
废话不多说， 网上virtualbox装ubuntu虚拟机的教程要多少有多少。说几点需要注意的：
1）操作系统版本的选择
**最好装ubuntu 12.04版本， 不要用12.10的。12.10 ,太耗资源而且速度慢。**
2）显卡的设置
配置一点显存： 大约64m足够了。 
另外，**如果你以后要用安卓模拟器的话，扩展特性两个对勾千万不要打**，不然安卓模拟器启动不了，会报硬件加速的错误。
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s1.sinaimg.cn/mw690/4868f986gd8baf2aeec90&690)
3） 存储配置
尽量选择动态分配存储， 空间不需要太大，8G基本可以满足需要， 而且以后容易拷。
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s5.sinaimg.cn/mw690/4868f986gd8baf564b1a4&690)
4） 网络设置
尽量选择桥接的方式， 这样最方便。网卡内外都能用，还能相互拷贝
**虚拟电脑可以允许混杂模式，方便以后抓包**
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s10.sinaimg.cn/mw690/4868f986gd8bb03966fd9&690)
二） 配置代理服务器
我们公司上网是要过代理并且需要口令的。所以要为虚拟机也配置上网代理
方法很简单：
修改 /etc/bash.bashrc 文件，在文件最后加上这样的三句
export http_proxy=http://username:passwd@proxy_addr:proxy_port 
export https_proxy=http://username:passwd@proxy_addr:proxy_port 
export ftp_proxy=http://username:passwd@proxy_addr:proxy_port 
username、passwd 、 proxy_addr、 proxy_port 根据实际情况修改就行了
完了之后别忘了. /etc/bash.bashrc 把这些内容加到当前环境变量里面
三）安装新立德软件管理器
ubuntu下面软件管理没有新立德怎么行？！ 
安装方法很简单：
sudo -s 切换到root之后
apt-get update;
apt-get install synaptic; 顺势安装即可
或者到 softwarecenter 里面安装就行了
说道这里记录一下查找软件包的方法：
**apt-cache search  xxx**
用这个命令可以很方便的查找到最新的ubuntu软件列表理念
四）安装sshd
sshd还用说么？ 主机和虚拟机之间通信就靠它了！
有了新立德，安装这个没有什么问题
或者 apt-get install ssh直接安装也很方便
五）安装jdk，jre
从官网上下载jdk 
http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html
gz打包的这种就行了
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s1.sinaimg.cn/mw690/4868f986gd8bb2e407c80&690)
下载之后， 直接利用ssh拷贝到虚拟机上面解压就完成安装了，比如我就喜欢拷贝到/opt下面
接下来就是一些环境变量的配置：
还是到/etc/bash.bashrc里面:
**export JAVA_HOME=/opt/jdk1.7.0_17/**
**export JRE_HOME=/opt/jdk1.7.0_17/jre/**
**export CLASSPATH=$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH**
**export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH**
配置完成之后如果在终端能用java 命令就行了
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s15.sinaimg.cn/mw690/4868f986gd8bb4d7e3bde&690)
六） 安装elcipse +adt +sdk
直接下载安卓官网的 
http://developer.android.com/sdk/index.html
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s12.sinaimg.cn/mw690/4868f986gd8bb585903db&690)
下载完了之后解压， eclipse + adt + sdk + cdt就都有了，真是省心
**注意eclipse执行的时候最好从命令行执行， 不然可能有些环境变量没有加进来**
**export PATH=$PATH:/opt/adt-bundle-linux-x86/sdk/platform-tools:/opt/adt-bundle-linux-x86/eclipse**
7） 安装ndk
这个也比较简单：
拷贝，解压， 修改环境变量
**export PATH=$PATH:/opt/android-ndk-r8c**
8） 配置usb
这个是最麻烦的：
首先手机把usb调试开关打开， 然后把这个设备映射到虚拟机，方法就是选中下面的usb设备，打上对勾。
![](https://img-my.csdn.net/uploads/201304/10/1365580931_7205.png)[](http://photo.blog.sina.com.cn/showpic.html#blogid=4868f98601017xnl&url=http://s4.sinaimg.cn/orignal/4868f986g7c12e4f1e1d3)
然后用lsusb就能看到设备：
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s1.sinaimg.cn/mw690/4868f986gd8bcf86cf9a0&690)
这里 XXXX：YYYY就分别是VendorID（18d1） 和productid（4e22）。
如果你观察一下/dev 目录会发现： 多了一个设备 sdc,
我们用命令 ：udevadm info  -a -n /dev/sdc看一下，打印的内容很长， 其中有一部分：
  looking at parent device '/devices/pci0000:00/0000:00:0b.0/usb1/1-1':
    KERNELS=="1-1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{configuration}==""
    ATTRS{bNumInterfaces}==" 2"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bmAttributes}=="80"
    ATTRS{bMaxPower}=="500mA"
    ATTRS{urbnum}=="4998"
**    ATTRS{idVendor}=="18d1"**
**    ATTRS{idProduct}=="4e22"**
    ATTRS{bcdDevice}=="0231"
    ATTRS{bDeviceClass}=="00"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bDeviceProtocol}=="00"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{speed}=="480"
    ATTRS{busnum}=="1"
    ATTRS{devnum}=="2"
    ATTRS{devpath}=="1"
    ATTRS{version}==" 2.00"
    ATTRS{maxchild}=="0"
    ATTRS{quirks}=="0x0"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{authorized}=="1"
    ATTRS{manufacturer}=="samsung"
    ATTRS{product}=="GT-I9000"
    ATTRS{serial}=="343296C22A7400EC"
蓝色的部分就是我们后面写规则的依据。
接下来，就如同很多网上的教程一样， 写/etc/udev/rules.d/50-android.rules (这个序号无所谓，50，51，30 随便，最好不要太大)
内容如下：
SUBSYSTEM=="usb",ATTRS{idVendor}=="18d1", ATTRS{idProduct}=="4e22", MODE="0666", SYMLINK ="andriod_adb_dev"
这几个字段的含义： 1）这是一个usb设备 2） idVendor 是18d1 3）idProduct是 4e22 4)挂载为可读写 5） 在/dev创建一个叫andriod_adb_dev的 符号链接
保存之后， 执行 restart udev  重启udev服务，之后我们可以再/dev里面看到 andriod_adb_dev：
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s9.sinaimg.cn/mw690/4868f986gd8bd64ecf248&690)
接着执行adb devices就能看到设备了：
![virtualbox <wbr>ubuntu虚拟机安装配置android开发环境（eclipse+ndk+sdk）及注意事项](http://s14.sinaimg.cn/mw690/4868f986gd8bd67e6d08d&690)
注意：如果adb不能识别的话， 可以试试重启下看看，方法：
adb kill-server; adb devices
最后，你可以在usb设置里面把你的手机添加到筛选器， 这样当你手机插入之后，会自动映射到虚拟机里面
[](http://photo.blog.sina.com.cn/showpic.html#blogid=4868f98601017xnl&url=http://s7.sinaimg.cn/orignal/4868f986gd8bd74970ab6)![](https://img-my.csdn.net/uploads/201304/10/1365580950_4843.jpg)
9） 环境配置完成
这样一来就能进行真机调试了。
谢谢！
附录：
**1.为 Ubuntu 虚拟机从设备列表中添加 USB 筛选器;**
![VirtualBox <wbr>下的 <wbr>Ubuntu <wbr>虚拟机中使用 <wbr>Android <wbr>真机做开发调试](http://www.shaoqun.com/userfiles/Article/54777-1.jpg)
**2.将真机与 PC 连接，开启真实手机中的[设置]->[应用程序]->[开发]->[USB调试]和[USB调试通知]; 3.在 Ubuntu 虚拟机中查看已附加的设备列表：  $cd ~/android-sdk-linux_x86/platform-tools $./adb devices List of devices attached  HT02TL901063 device 注：出现以上信息说明真机已经附加成功！ 如果出现以下信息，则说明真机附加失败！ List of devices attached  ???????????? no permissions 通过以下命令重启服务即可使真机附加成功： $sudo ./adb kill-server $sudo ./adb start-server * daemon not running. starting it now on port 5037 * * daemon started successfully ***
