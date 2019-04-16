# Windows：安装cygwin教程 - 去向前方的博客 - CSDN博客





2018年07月13日 10:34:18[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：6499








# 目录



- [目录](#目录)
- [前言](#前言)
- [我们看到有三种安装模式：](#我们看到有三种安装模式)
- [2、验证Cygwin安装成功](#2验证cygwin安装成功)
- [3、在windows命令下使用cygwin](#3在windows命令下使用cygwin)
- [常见错误](#常见错误)




# 前言

本篇文章参考这篇： 
[cygwin安装](https://blog.csdn.net/chunleixiahe/article/details/55666792)

但自从博主写后，这个东西发生了一些变化，因此，根据最新版的重新写了一遍。 

我们可以到Cygwin的[官方网站](http://www.cygwin.com/)下载Cygwin的安装程序
或者直接使用[来下载安](http://www.cygwin.com/setup.exe)来下载安装程序.

下载完成后，运行setup.exe程序，首先是同意安装，第三方的软件在windows上不受信任，出现安装画面。直接点“下一步”，出现安装模式的对话框，如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20180713101416650?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 我们看到有三种安装模式：

1 Install from Internet，这种模式直接从Internet安装，适合网速较快的情况； 

   2 Download Without Installing，这种模式只从网上下载Cygwin的组件包，但不安装； 

   3 Install from Local Directory，这种模式与上面第二种模式对应，当你的Cygwin组件包已经下载到本地，则可以使用此模式从本地安装Cygwin。
从上述三种模式中选择适合你的安装模式，因为博主网速还不错（搭了梯子），选择第一种安装模式，直接从网上安装，如果网速不行可以选择下载到本地后再处理，当然在下载的同时，Cygwin组件也保存到了本地，以便以后能够再次安装。选中后，点击“下一步”， 

这里也有所不同，新版本的选项没了。 
![这里写图片描述](https://img-blog.csdn.net/20180713102219696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这一步选择Cygwin的安装目录，以及一些参数的设置。默认的安装位置是C:\cygwin\，你也可以选择自己的安装目录，然后选择“下一步”， 
![这里写图片描述](https://img-blog.csdn.net/20180713102333772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这一步我们可以选择安装过程中从网上下载的Cygwin组件包的保存位置，选择完以后，点击“下一步”， 
![这里写图片描述](https://img-blog.csdn.net/20180713102426416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这一步选择连接的方式，选择你的连接方式，然后点击下一步，会出现选择下载站点的对话框，如下图所示， 
![这里写图片描述](https://img-blog.csdn.net/20180713102544586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

不同的镜像存放了不同的包。 

在这一步，需要注意，为了获得最快的下载速度，我们首先在列表中寻找Cygwin中国镜像的地址：[http://www.cygwin.cn](http://www.cygwin.cn)，如果找到就选中这个地址；如果找不到这个地址，就选一个国内的镜像地址，或者使用阿里云镜像（[http://mirrors.aliyun.com/cygwin/](http://mirrors.aliyun.com/cygwin/)）选择完成后，点击“下一步”，
这一步，我们选择需要下载安装的组件包，为了使我们安装的Cygwin能够编译程序，我们需要安装gcc编译器，默认情况下，gcc并不会被安装，我们需要选中它来安装。为了安装gcc，我们用鼠标点开组件列表中的“Devel”分支，在该分支下，有很多组件，我们必须的是： 

binutils  

gcc  

gcc-mingw  

gdb 

鼠标点击组件前面的循环按钮，会出现组建的版本日期，我们选择最新的版本安装，下图是选中后的四类组件的截图： 

注意搜索出来的结果可能有所不同： 
![这里写图片描述](https://img-blog.csdn.net/20180713103658966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选完以后，我们选择下一步，进入安装过程，如下图所示， 
![这里写图片描述](https://img-blog.csdn.net/20180713103830669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装的时间依据你选择的组件以及网络情况而定。安装完成后，安装程序会提示是否在桌面上创建Cygwin图标等，点击完成退出安装程序。 
![这里写图片描述](https://img-blog.csdn.net/20180713103856178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 2、验证Cygwin安装成功

运行cygwin，在弹出的命令行窗口输入：

```
cygcheck -c cygwin

    会打印出当前cygwin的版本和运行状态，如果status是ok的话，则cygwin运行正常。
```

1、Cygwin安装和验证 - 

```
然后依次输入gcc –version，g++ --version，
注意    **make –version，** 要看看你下的时候是什么 mingw64-make 之类的。
    gdb –version进行测试，如果都打印出版本信息和一些描述信息，非常高兴的告诉你，你的cygwin安装完成了！
```

![这里写图片描述](https://img-blog.csdn.net/20180713104235288?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 3、在windows命令下使用cygwin

要想在Windows命令提示符下使用Cygwin，你需要添加Cygwin到Windows环境变量。

　　通过Win+Pause/Break键或者在我的电脑上右键选择属性打开系统属性。 

　　在左边一列选择高级系统属性打开系统属性窗口：

　　在高级选项里点击下面的环境变量。

　　在系统变量里，找到Path变量并单击进行编辑。

　　在变量值选项的最后面添加Cygwin下bin路径: 

;C:cygwinbin

注意：确保前面要加个分号来和其它的变量值进行区分。

　　点击“OK”关闭对话，然后打开命令提示符来测试几个Linux命令。

　　正如下面的图片里所看到的，pwd和ls在Windows命令提示符工作正常。你还会看到/cygdrive/c自动被添加到当前目录显示里面了。  

# 常见错误

注意所有代码都要打全 
![g++](https://img-blog.csdn.net/20180713101211678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







