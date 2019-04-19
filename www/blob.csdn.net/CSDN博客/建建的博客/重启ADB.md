# 重启ADB - 建建的博客 - CSDN博客
2017年05月31日 10:04:03[纪建](https://me.csdn.net/u013898698)阅读数：196
adb服务有可能（在Windows进程中可找到这个服务，该服务用来为模拟器或通过USB数据线连接的真机服务）会出现异常。这时需要重新对adb服务关闭和重启。
当然，重启Eclipse可能会解决问题。但那比较麻烦。如果想手工关闭adb服务，可以使用下面的命令。
  adb kill-server
  在关闭adb服务后，要使用如下的命令启动adb服务。
  adb start-server
如图：
![](http://pic002.cnblogs.com/images/2011/302747/2011060216441658.jpg)
Eclipse显示
![](http://hi.csdn.net/attachment/201109/15/0_13160783099M1B.gif)
