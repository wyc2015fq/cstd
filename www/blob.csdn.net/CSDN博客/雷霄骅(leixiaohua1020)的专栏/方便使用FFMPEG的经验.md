# 方便使用FFMPEG的经验 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年02月13日 00:29:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：93









FFMPEG是命令行工具，因此使用起来多少还是会有些不方便。在这记录两点方便使用FFMPEG的方法：



**1.任何目录下都可以使用FFMPEG**

问题描述：需要转码（播放）的时候，需要把ffmpeg.exe（ffplay.exe）拷贝到转码视频目录中。

解决方法：在系统环境变量的path中添加FFMPEG可执行程序的的目录。

例如FFMPEG位于D:\ffmpeg\bin中。右键“我的电脑”->属性->高级系统设置->高级->环境变量，找到名字为path的变量，在其后追加字符串：D:\ffmpeg\bin;（结尾是有分号的）。设置完一般重启后生效。

设置后，在任意目录下都可以用ffmpeg.exe（ffplay.exe）转码（播放）。



**2.系统右键添加Cmd**

问题描述：每次打开Cmd（命令提示符），当前目录都在系统的用户目录下，如果想换成自己的视音频文件目录还需要一点一点用DOS命令切换。

解决方法：在系统右键菜单中添加“Cmd”，可以直接从当前目录打开Cmd。

新建一个记事本，把下列代码拷贝进去，把记事本文件名改成command.bat，运行一下。



```
REG ADD "HKCR\*\shell\Cmd\command" /ve /t REG_EXPAND_SZ /d %ComSpec%
REG ADD "HKCR\Directory\shell\Cmd\command" /ve /t REG_EXPAND_SZ /d "%ComSpec% /k cd %1"
REG ADD "HKCR\Drive\shell\Cmd\command" /ve /t REG_EXPAND_SZ /d "%ComSpec% /k cd %1"
```

设置后，单击一个文件，右键菜单中可以选择“Cmd”。








