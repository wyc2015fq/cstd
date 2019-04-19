# QT5.5应用程序图标、exe图标设置 - xqhrs232的专栏 - CSDN博客
2018年01月12日 11:29:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：760
原文地址::[http://blog.csdn.net/hyw11002233/article/details/70240928](http://blog.csdn.net/hyw11002233/article/details/70240928)
相关文章
1、Qt5设置应用程序图标----[http://blog.csdn.net/CSDN_LQR/article/details/52195033](http://blog.csdn.net/CSDN_LQR/article/details/52195033)
2、Qt中修改应用程序和标题栏的图标----[http://blog.csdn.net/ywxk1314/article/details/51953184](http://blog.csdn.net/ywxk1314/article/details/51953184)
# QT5.5应用程序图标、exe图标设置
- 
设置应用程序图标（png文件）
在已有的工程中添加一个.qrc文件。
新建文件–>QT–>资源文件,即可添加，这里我们命名为update.qrc。 
添加前缀–>添加需要使用的图片。将图片放到源码目录下。保存一下项目，在pro文件中将自动将qrc文件添加进去。 
![添加前缀](http://blog.chinaunix.net/attachment/201205/22/20044771_1337678916nEmE.png)
![添加文件](http://blog.chinaunix.net/attachment/201205/22/20044771_1337678965xixH.png)
在主程序ui文件的修改属性windowsicon项。
![这里写图片描述](http://blog.chinaunix.net/attachment/201205/22/20044771_1337679040SO33.png)
选择自己需要的png图片，保存项目。至此完成应用程序的图标设置。
2. 设置.exe程序图标（ico文件）
将需要设置的图片放置到源码目录
新建一个.rc文件
新建文件–>C++–>source文件–>命名为XX.rc(不能掉了rc后缀)
打开rc文件添加如下一行代码。
```bash
IDI_ICON1 ICON DISCARDABLE "XXX.ico"
```
- 1
XXX.ico是你的exe程序图标名。
最后在pro文件添加一行代码
`RC_FILE += XXX.rc`- 1
XXX.rc是刚刚新建的rc文件名。 
保存项目重新编译，exe图标和执行时window图标都将发生变化。
推荐一个在线转换png、ico互转的工具。 
[https://converticon.com/](https://converticon.com/)
至此，设置完成。

