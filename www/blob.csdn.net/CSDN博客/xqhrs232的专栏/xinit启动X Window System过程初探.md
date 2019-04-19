# xinit启动X Window System过程初探 - xqhrs232的专栏 - CSDN博客
2016年11月07日 17:31:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：265
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/hunanchenxingyu/article/details/50925642](http://blog.csdn.net/hunanchenxingyu/article/details/50925642)
相关文章
1、[startx 及xinit 介绍](http://blog.csdn.net/sjin_1314/article/details/40148853)----[http://blog.csdn.net/sjin_1314/article/details/40148853](http://blog.csdn.net/sjin_1314/article/details/40148853)
启动X主要有两种方法：一是Display Manager，如XDM、GDM、KDM，此种方法通过图形界面登录;另一种是通过xinit，此种方法适用于字符界面登录。我们常用于登录X的startx命令也是通过传递参数给xinit来启动X的，也就是说，最终启动X的是xinit。startx只是一个bash脚本。下面主要分析xinit一下初始化X的过程：
　　首先来看一看xinit命令的用法：
　　xinit [[client] options ] [-- [server] [display] options]
　　可以看出，xinit初始化X是通过两个层面来进行的，即X client程序和X server程序。上面[client]和[server]分别称为X client程序和X server程序。它们代表的是程序的具体路径，其必须以/或者./开头。
　　默认情况下，当我们CLI下执行不带任何参数的xinit命令时，xinit会在目录下搜索.xinitrc文件当作X client来执行，也就是说，你可以在其中设置进入X时便运行的X client程序，具体的设置见后。根据$HOME/.xinitrc的有无，xinit会有不同的运行情况。
　　1.若存在$HOME/.xinitrc，相当于执行：
　　xinit $HOME/.xinitrc -- /usr/bin/X
　　2.若不存在$HOME/.xinitrc，相当于执行：
　　xinit xterm -- /usr/bin/X
　　如果不存在$HOME/.xinitrc或是未正确设置$HOME/.xinitrc都不会进入Desktop Environment(桌面环境，如GNOME、KDE等)，呈现在我们面前的只有一个xterm和"X"状的鼠标。那么怎样才能进入Desktop Environment呢?让我们来分析一下startx命令的运行情况吧。 通过分析startx命令文件的内容，得知startx主要是置X
 client和X server所在的位置，并处理相关参数，最后交给xinit处理。可以看出startx 设置X client的位置是先搜寻$HOME/.xinitrc，然后是/etc/X11/xinit/xinitrc;设置X server的位置是先搜寻$HOME/.xserverrc，然后是/etc/X11/xinit/xserverrc。
　　即相当于运行了以下命令：
　　xinit /etc/X11/xinit/xinitrc -- /etc/X11/xinit/xserverrc
　　进一步分析：
　　xinit /etc/X11/Xsession -- /usr/bin/X11/X -dpi 100
　　Xsessin是一个重要的文件，为什么要怎么说呢?因为不管是通过Display Manager登录X，还是通过xinit(startx)登录X，它都会被执行。Xsession是一个全局文件。
　　Xsession文件中以下内容：
　　/usr/bin/x-session-manager/*启动Desktop Environment*/
　　/usr/bin/x-window-manager/*启动Window Manager*/
　　/usr/bin/x-terminal-emulator/*启动Terminal*/
　　这就是为什么我们可以用一个startx命令直接进入X的原因所在了。如果我们要进入GNOME我们只需要将/usr/bin/x-session-manager所链接的目标文件改为/usr/bin/gnome-session，然后把/usr/bin/x-window-manager所链接的目标文件改为/usr/bin/metacity，把/usr/bin/x-terminal-emulator所链接的目标文件改为/usr/bin/gnome-terminal就可以了。
弄清楚了以上的内容，我们就可以修改$HOME/.xinitrc，通过xinit来启动Desktop Environment了，下面以启动GNOME为例：
　　首先在$HOME下创建.xinitrc文件，然后添加以下内容：
　　gnome-session&
　　metacity/*除了最后一个命令在前台运行以外，其他的都应运行在后台*/
　　保存后，你就可以在终端通过执行xinit来启动GNOME了，是不是很酷呢?
　　利用xinit启动X的时候应注意，如果第一个运行的X client结束，xinit将自动结束X，返回到CLI。下面以具体的例子来说明：
　　当不存在$HOME/.xinitrc时，执行xinit metacity，将会出现一个可移动的xterm(无$HOME/.xinitrc，xinit默认启动xterm)，当然也有一个X
　　server。关闭xterm将退出X，因为xterm是第一个运行的X client。当存在$HOME/.xinitrc且包含如下内容：
　　xeyes&
　　metacity&
　　xterm
　　执行xinit将会出现一个xterm和一个xeyes，关闭xterm将退出X，因为其他的X client(xeyes、metacity)都是运行在后台的，而xterm是前台运行的第一个X client。
　　同理，我们也可以通过以下的命令来启动Desktop Environment：
　　xinit gdm
　　xinit /etc/X11/Xsession
