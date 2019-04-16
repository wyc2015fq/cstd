# Android开发中遇到的问题(一)——Android模拟器端口被占用问题的解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发中遇到的问题(一)——Android模拟器端口被占用问题的解决办法](https://www.cnblogs.com/xdp-gacl/p/4321012.html)



## 一、问题描述

　　今天在Eclipse中运行Android项目时遇到"The connection to adb is down, and a severe error has occured"这个问题，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/072139489458347.png)

　　第一次遇到这个问题，上网查了一下原因，原来"The connection to adb is down, and a severe error has occured"指的就是Android模拟器端口被占用了。

## 二、问题的排查过程

　　既然是端口被占用了，现在就要看看到底是谁占用了端口。

　　在sdk目录下的platform-tools目录中有一个adb.exe可执行程序，这个就是Android模拟器的启动程序，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/072156402896686.png)

　　打开CMD命令行窗口，进入sdk目录下的platform-tools目录，输入命令：adb start-serevr，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/072147467113575.png)

　　因为我在搭建Android开发环境时，已经将sdk目录下的platform-tools目录加入到Path环境变量中，如下：

![](https://images0.cnblogs.com/blog2015/289233/201503/072152190084963.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/072153197581923.png)

　　所以我在cmd命令中直接输入命令：adb start-serevr是可以正常运行的，不再需要进入到进入sdk目录下的platform-tools目录。

　　从上面的排查结果显示来看，确实是端口被占用了，接下来需要确定被占用的端口号是多少，输入命令：adb nodaemon server，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/072200118053872.png)

　　OK，现在确定了，端口5037被占用了，那就要找到究竟是谁敢占用5037端口。

　　在cmd中输入命令netstat -ano |findstr "5037"，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/072205546172169.png)

　　 原来是5040这个进程占用了端口，我们要把这个进程找出来，在cmd中输入命令tasklist /fi "pid eq 5040"，如下：

![](https://images0.cnblogs.com/blog2015/289233/201503/072209576496250.png)

　　OK，已经找到了，是一个名字为shuame_helper.exe，打开任务管理器，kill这个进程即可。









