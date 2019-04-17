# CentOS6.5 安装codeblocks-13.12 - DoubleLi - 博客园






安装环境CentOS6.5

启动命令行

1.先安装gcc和gcc++，这个可以直接安装
# yum install gcc
# yum install gcc-c++
2.再安装gtk2，也是直接安装
# yum install gtk2*
3.再安装wxGTK，一样的方法
# yum install wxGTK
4.本来要安装xterm的，最后我用的时候发现，显示中文有些显示的是方框，开始以为是codeblocks的事情，最后找到原因了，是这个xterm的问题，输出改成gnome的终端就行了，安装好后，进入”Settings”->”Environment settings”->”General settings”，在右边下面的”Terminal to launch console programs:”这里，后面下拉选择“gnome-terminal –disable-factory -t $TITLE -x”，或者直接输入也行，这样就ok了，显示中文也挺正常。

5.ok，所有都准备好了，现在下载codeblocks，下载地址：http://www.codeblocks.org/downloads/26#linux，我们下载codeblocks-13.12-1.el6.x86_64.tar.xz这个版本，
# xz -d codeblocks-13.12-1.el6.x86_64.tar.xz　codeblocks-13.12-1.el6.x86_64.tar.xz 去掉了xz  codeblocks-13.12-1.el6.x86_64.tar# tar -xvf codeblocks-13.12-1.el6.x86_64.tar
　解压出4个文件
# ls
codeblocks-12.11-1.el6.i686.rpm
codeblocks-contrib-12.11-1.el6.i686.rpm
codeblocks-contrib-devel-12.11-1.el6.i686.rpm
codeblocks-devel-12.11-1.el6.i686.rpm
现在进行安装，全部安装
# rpm -ivh codeblocks*
这样就安装完成了，在应用程序菜单的编程下面就会出现code::blocks IDE，并可以正常使用

启动后选择一个默认的编译环境 如GCC等

ps：转

但是在启动的时候会报“spellchecker needs a dictionary to work correctly”的错误，这个好解决，我们进入“/usr/share/myspell”下面，新建一个“dicts”的文件夹。看一看现在有多少东西。

![](http://www.ienter.org/wp-content/uploads/2013/07/Screenshot3.png)

ok，里面有三组，分别是”en*GB”,”en*CA”和”en_US”，我们先进入dicts文件夹下面，对前三组共6个文件创建软连接
[root@ienter dicts]# ln -s ../en_GB.aff en_GB.aff
[root@ienter dicts]# ln -s ../en_GB.dic en_GB.dic
[root@ienter dicts]# ln -s ../en_CA.aff en_CA.aff
[root@ienter dicts]# ln -s ../en_CA.dic en_CA.dic
[root@ienter dicts]# ln -s ../en_US.aff en_US.aff
[root@ienter dicts]# ln -s ../en_US.dic en_US.dic
现在打开codeblocks，打开“Settings”->“editor“，选择左边的”SpellChecker“，开启”Enable spelling suggestion tooltips“，在下面的”Language“里就可以选择自己想要的了。然后”确定“关闭，下次再用的时候就不会报这个问题了。









