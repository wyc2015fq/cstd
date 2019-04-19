# ubuntu  sun-java - 三少GG - CSDN博客
2010年12月21日 17:06:00[三少GG](https://me.csdn.net/scut1135)阅读数：1441标签：[ubuntu																[java																[oracle																[deb																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=deb&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
解决方案20170501 [http://topspeedsnail.com/ubuntu16-install-java-jdk/](http://topspeedsnail.com/ubuntu16-install-java-jdk/)
### 安装oracle Java JDK
首先，安装依赖包：
`$ sudo apt-get install python-software-properties`
添加仓库源：
`$ sudo add-apt-repository ppa:webupd8team/java`
更新软件包列表：
`$ sudo apt-get update`
安装java JDK：
`$ sudo apt-get install oracle-java8-installer`
安装过程中需要接受协议：
![java Ubuntu 16.04](http://topspeedsnail.com/images/2016/4/Screen%20Shot%202016-04-03%20at%2018.52.11.png)
查看java版本：
`$ java -version`
![java Ubuntu 16.04](http://topspeedsnail.com/images/2016/4/Screen%20Shot%202016-04-03%20at%2020.50.46.png)
如果你同时安装了以上两个版本，你可以自由的在这两个版本之间切换。执行：
`$ sudo update-alternatives --config java`
![java Ubuntu 16.04](http://topspeedsnail.com/images/2016/4/Screen%20Shot%202016-04-03%20at%2020.54.50.png)
前面带星号的是当前正在使用的java版本，键入编号选择使用哪个版本。
编辑/etc/profile，在文件尾添加java环境变量：
`$ sudo vim /etc/profile`
```
＃ 如果使用oracle java
export JAVA_HOME="/usr/lib/jvm/java-8-oracle/jre/bin"
```
OK，在Ubuntu 16.04上安装java完成。
##################DEPRECATED####################
原文地址：   [http://wiki.ubuntu.org.cn/Sun-java6](http://wiki.ubuntu.org.cn/Sun-java6)
Pku 地址为：[](http://wiki.ubuntu.org.cn.sixxs.org/Sun-java6)[http://wiki.ubuntu.org.cn.sixxs.org/Sun-java6](http://wiki.ubuntu.org.cn.sixxs.org/Sun-java6)   
 (2011.09.01)
# Sun-java6
### 出自Ubuntu中文
## 目录
[隐藏]
- [1区分](http://wiki.ubuntu.org.cn/Sun-java6#.E5.8C.BA.E5.88.86)
- [2源的安装方式](http://wiki.ubuntu.org.cn/Sun-java6#.E6.BA.90.E7.9A.84.E5.AE.89.E8.A3.85.E6.96.B9.E5.BC.8F)- [2.1安装sun-java6](http://wiki.ubuntu.org.cn/Sun-java6#.E5.AE.89.E8.A3.85sun-java6)
- [2.2设为默认Java](http://wiki.ubuntu.org.cn/Sun-java6#.E8.AE.BE.E4.B8.BA.E9.BB.98.E8.AE.A4Java)
- [3Oracle 官方bin安装包](http://wiki.ubuntu.org.cn/Sun-java6#Oracle_.E5.AE.98.E6.96.B9bin.E5.AE.89.E8.A3.85.E5.8C.85)
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Sun-java6&action=edit&section=1)]区分
[Java](http://wiki.ubuntu.org.cn/Java)有2种，一种是Ubuntu源自带的open-java（icedtea），一个是Oracle的sun-java6。前者是Ubuntu默认的Java，一般更新后系统就自带了，这里讲后者的安装配置。
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Sun-java6&action=edit&section=2)]源的安装方式
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Sun-java6&action=edit&section=3)]安装sun-java6
添加partner源
sudo add-apt-repository "deb http://archive.canonical.com/ubuntu maverick partner"
(注： 如果系统提示没有add-apt-repository命令，则需要先安装python-software-properties，命令sudo apt-get install python-software-properties。另外，有些版本的ubuntu即使安装了python-software-properties也无add-apt-repository命令,此时需要手动添加此行"deb[http://archive.canonical.com/ubuntu](http://archive.canonical.com/ubuntu)
 maverick partner"到/etc/apt/sources.list文件中。)
更新系统
sudo apt-get update
安装jre
sudo apt-get install sun-java6-jre sun-java6-plugin
安装jdk
sudo apt-get install sun-java6-jdk sun-java6-plugin
查看版本信息
java -version
### [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Sun-java6&action=edit&section=4)]设为默认Java
sudo update-alternatives --config java
选择即可
## Attention:
[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 10.10已经去掉 sun-java6-jdk 和 sun-java6-jre的软件包，ubuntu 官方声明：建议安装 openjdk-6 以取代 sun-java6-jre .
但是，我们就是要安装 sun-java6-jre。
三行命令即可装好 Sun Java Runtime Environment (JRE) 6 ：
在终端输入：
sudo add-apt-repository "deb http://archive.canonical.com/ lucid partner"
sudo apt-get update
sudo apt-get install sun-java6-jre sun-java6-plugin sun-java6-fonts
全部下载结束后，会弹出一个蓝底的界面。鼠标不能用，请用键盘上的 TAB 键跳转，用回车键确认。接受许可协议后安装会自动执行完。
完成后,终端 输入  java   测试。。一切OK！！
## [[编辑](http://wiki.ubuntu.org.cn/index.php?title=Sun-java6&action=edit&section=5)]Oracle 官方bin安装包
此外可以用Linux上通用的bin包安装，下载官方bin包，终端下面安装解压，然后修改环境变量指向那个jdk便可。
设置环境变量，修改.bashrc或.profile，参考自己系统解压的路径改改。
export JAVA_HOME=/home/user/jdk1.6.0_21/
export PATH=$JAVA_HOME/bin:$PATH
export classPath=/home/user/jdk1.6.0_21/
查看java版本
qii@ubuntu:~$ java -version
java version "1.6.0_21"
Java(TM) SE Runtime Environment (build 1.6.0_21-b06)
Java HotSpot(TM) Server VM (build 17.0-b16, mixed mode)
