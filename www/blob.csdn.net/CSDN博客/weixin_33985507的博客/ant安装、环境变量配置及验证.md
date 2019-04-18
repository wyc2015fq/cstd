# ant安装、环境变量配置及验证 - weixin_33985507的博客 - CSDN博客
2016年06月15日 14:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
一、安装ant
到官方主页http://ant.apache.org下载新版（目前为Ant1.8.1）的ant，得到的是一个apache-ant-1.8.1-bin.zip的压缩包。将其解压到你的硬盘上，例如：C:\apache-ant-1.8.1。
二、配置环境变量
**window中设置ant环境变量：**
ANT_HOME    C:/ apache-ant-1.8.1
path             C:/ apache-ant-1.8.1/bin
classpath      C:/apache-ant-1.8.1/lib
**注意：【**
**　　配置环境变量：我的电脑----属性-----高级----环境变量**
**      如：ANT_HOME：C:\apache-ant-1.8.1**
**      PATH：%ANT_HOME%\bin （为了方便在dos环境下操作）**
**     实验了总是失败，没办法换成地址 C:/ apache-ant-1.8.1/bin，而不再使用变量。。。成功了。。**
**】**
三、验证ant
为了验证ant是否成功安装，可以进行如下操作：
依次选择：开始->运行->cmd，输入如下命令：ant
如果出现如下内容，说明安装成功：
Buildfile: build.xml does not exist!
Build failed
![](https://img-my.csdn.net/uploads/201211/12/1352718174_5230.png)
【说明ant安装成功！因为ant默认运行build.xml文件，这个文件需要我们建立。】
查看版本：ant  -version
![](https://img-my.csdn.net/uploads/201211/12/1352718184_3525.png)
但如果出现如下内容，说明安装失败：（可以重复前述步骤，直至安装成功。）
'ant' 不是内部或外部命令，也不是可运行的程序或批处理文件。
从别处移动过来的：
使用：
(1)在D盘根目录下建立build.xml
1![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)<?xml version="1.0" encoding="GBK"?>
2![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)<project name="测试脚本" default="copyfile" basedir="." >
3![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)   <target name="copyfile">
4![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)      <copy file="d:/a.txt" todir="e:/Temp" overwrite="true" />
5![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)   </target>
6![](http://www.cnitblog.com/Images/OutliningIndicators/None.gif)</project>
（2）在D盘根目录下建立文件a.txt。
（3）进入dos，
         d:
         ant
![](http://www.cnitblog.com/images/cnitblog_com/intrl/1.jpg)
         此时可在E:/Temp目录下见到文件aa.txt，内容与a.txt一样，即拷贝成功！
