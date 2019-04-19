# Ubuntu 系统中设置环境变量 PATH 的方法 - xqhrs232的专栏 - CSDN博客
2016年03月21日 15:18:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：318
原文地址：[http://www.cnblogs.com/diyingyun/archive/2013/01/16/2862586.html](http://www.cnblogs.com/diyingyun/archive/2013/01/16/2862586.html)
相关文章
1、[Ubuntu下设置环境变量及PATH的方法](http://blog.chinaunix.net/uid-26285146-id-3138789.html) ----[http://blog.chinaunix.net/uid-26285146-id-3138789.html](http://blog.chinaunix.net/uid-26285146-id-3138789.html)
2、Ubuntu设置环境变量----[http://jingyan.baidu.com/article/db55b609a3f6274ba30a2fb8.html](http://jingyan.baidu.com/article/db55b609a3f6274ba30a2fb8.html)
在 Ubuntu 系统中有两种设置环境变量 PATH 的方法。第一种适用于为单一用户设置 PATH，第二种是为全局设置 PATH。
第一种方法：
在用户主目录下有一个 .bashrc 文件，可以在此文件中加入 PATH 的设置如下：
export PATH=”$PATH:/your path1/:/your path2/…..”
注意：每一个 path 之间要用 “:“ 分隔。
注销重启 X 就可以了。
第二种方法：
在 /etc/profile中增加。
PATH="$PATH:/home/zhengb66/bin"
export PATH
环境变量是 和Shell紧密相关的，用户登录系统后就启动了一个Shell。对于Linux来说一般是bash，但也可以重新设定或切换到其它的 Shell。对于UNIX，可能是CShelll。环境变量是通过Shell命令来设置的，设置好的环境变量又可以被所有当前用户所运行的程序所使用。对 于bash这个Shell程序来说，可以通过变量名来访问相应的环境变量，通过export来设置环境变量。下面通过几个实例来说明。
1)etc/profile:此文件为系统的每个用户设置环境信息,当用户第一次登录时,该文件被执行.
并从/etc/profile.d目录的配置文件中搜集shell的设置.
注：在这里我们设定是为所有用户可使用的全局变量。
2)/etc/bashrc:为每一个运行bash shell的用户执行此文件.当bash shell被打开时,该文件被读取.
3)~/.bash_profile:每个用户都可使用该文件输入专用于自己使用的shell信息,当用户登录时,该文件仅仅执行一次!默认情况下,他设置一些环境变量,执行用户的.bashrc文件.
注：~在LINUX下面是代表HOME这个变量的。
另外在不同的LINUX操作系统下，这个文件可能是不同的，可能是~/.bash_profile； ~/.bash_login或 ~/.profile其中的一种或几种，如果存在几种的话，那么执行的顺序便是：~/.bash_profile、 ~/.bash_login、 ~/.profile。比如我用的是Ubuntu，我的用户文件夹下默认的就只有~/.profile文件。
4)~/.bashrc:该文件包含专用于你的bash shell的bash信息,当登录时以及每次打开新的shell时,该文件被读取.
(注：这个文件是 .开头的，所以在文件夹中被隐藏了)
那么我们如何添加自己定义的环境变量呢？
用记事本打开这个文件，然后在里面最后写上:
xiaokang=kangkang
然后保存，这样每次打开一个新的terminal的时候，我们这个变量就生效了。记住，如果你已经打开一个terminal，然后你修改了这个文件，那么在这个terminal下是不会生效的。一般情况用户最好在这里进行修改，但是有时候会覆盖父级的变量，比如PATH是ROOT设定的，但是如果你在这个文件里面写了PATH=xx,那么将来所有的PATH都成了xx了，所以我们应该在这个文件中写为：  
PATH＝$PATH:xx
这样就把原来的和你自己的一起加上了。而且注意在LINUX系统下用：分割，而不是windo的；
3和4都是在用户目录下的，他们唯一的不同是: .bash_profile只能在登录的时候启动一次。在我的Ubuntu里面这个3文件似乎没有。
5)~/.bash_logout:当每次退出系统(退出bash shell)时,执行该文件.
另外,/etc/profile中设定的变量(全局)的可以作用于任何用户,而~/.bashrc等中设定的变量(局部)只能继承/etc/profile中的变量,他们是\"父子\"关系.
~/.bash_profile 是交互式、login 方式进入 bash 运行的
~/.bashrc 是交互式 non-login 方式进入 bash 运行的
通常二者设置大致相同，所以通常前者会调用后者。
好的，总结一下他们的执行方式：
当你登录并且登录shell是bash时,bash首先执行/etc/profile文件中的命令(如果该文件存在),然后它顺序寻找~ /.bash_profile,~/.bash_login或~/.profile文件,并执行找到的第一个可读文件中的命令.当登录bash退出时,它 将执行~/.bash_logout文件中的命令.
当启动一个交互的bash时,它将执行~/.bashrc文件中的命令(如果该文件存在并且可读).当非交互地启动以运行一个shell脚本时,bash将查找bash_env环境变量,确定执行文件的名称.
