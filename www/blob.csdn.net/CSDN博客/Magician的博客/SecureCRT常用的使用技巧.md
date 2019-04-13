
# SecureCRT常用的使用技巧 - Magician的博客 - CSDN博客


2018年08月31日 13:32:51[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：329所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)




### 文章目录
[前言：](#_2)
[1、SecureCRT 超时自动断开连接的解决办法](#1SecureCRT__7)
[2、SecureCRT连接Linux时，终端显示乱码的问题。](#2SecureCRTLinux_17)
[3、SecureCRT使用自动记录日志功能](#3SecureCRT_23)
[4、使用SecureCRT从Windows上传文件到Linux](#4SecureCRTWindowsLinux_32)
[5、SecureCRT配色推荐和永久设置](#5SecureCRT_49)

## 前言：
由于工作需要，要经常玩SecureCRT，所以还是希望能好好了解下这个软件，以下是一些常用的功能，后续后接着补充。
## 1、SecureCRT 超时自动断开连接的解决办法
在利用SecureCRT远程连接服务器的时候，经常会碰到一分钟不操作界面，就会断开连接的尴尬情况，如果公司登陆服务器的设置比较繁琐，你还得进行一连串繁琐的操作，很耗时，但是你可以通过SecureCRT设置定时发送空信息来保持连接。具体操作如下：
Options->Session Options->Terminal->Anti-idle->勾选Send protocol NO-OP
(中文版：选项->会话选项->终端->反空闲->发送空字符串保持连接)
后面的设置时间默认的是60秒，只要小于自动断开Linux设置的连接的时限就可以了。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/2018083113180996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、SecureCRT连接Linux时，终端显示乱码的问题。
当用SecureCRT连接linux时，总会遇到这样的问题，在服务器的终端的中文正常显示，但是通过SecureCRT连接后的终端显示的中文为乱码。许多人碰到这样问题时的第一反应就是Linux终端的编码问题，（也许是平时没留意服务器终端显示中文是正常的）进而去修改Linux的编码，但是实际问题确实SecureCRT终端显示的编码问题。具体解决方法如下：
Options->Session Options->appearance->Character encoding更改为UTF-8编码，重新连接即可。
![这里写图片描述](https://img-blog.csdn.net/20180831132712158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3、SecureCRT使用自动记录日志功能
很多人使用SecrueCRT时都有记录输出信息的习惯，以便进行问题分析。以前老版本的SecrueCRT仅支持手动记录日志问题，现在软件的易用性更好了，也同时支持了自动记录日志的功能。这里将简单介绍SecrueCRT自动记录日志的常用配置。
![这里写图片描述](https://img-blog.csdn.net/20180831133108220?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
详细的定时记录日志的操作请参考：
[https://jingyan.baidu.com/album/335530da88aa0b19cb41c3b9.html?picindex=1](https://jingyan.baidu.com/album/335530da88aa0b19cb41c3b9.html?picindex=1)
## 4、使用SecureCRT从Windows上传文件到Linux
**一、安装lrzsz**
`$ yum install lrzsz`**二、上传文件**
`$ rz
# 自动弹出对话框，进行选择文件，上传即可`**三、下载文件**
`$ sz FILE_NAME`说明：打开SecureCRT软件 -> Options -> session options -> X/Y/Zmodem 下可以设置上传和下载的目录。
![在这里插入图片描述](https://img-blog.csdn.net/20180921111707303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5、SecureCRT配色推荐和永久设置
参考：[https://blog.csdn.net/zq710727244/article/details/53909801](https://blog.csdn.net/zq710727244/article/details/53909801)

