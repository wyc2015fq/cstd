# 下载android的linux内核的方法 - h13 - 博客园
1、安装git
       android的linux内核可以从[http://android.git.kernel.org/](http://android.git.kernel.org/)下载，但下载需要使用git，windows版的git可以从http://code.google.com/p/msysgit/下载，有完全安装版和便携版(portable)两个版本可供选择，不常用git的话选portable版就可以了。
      下载portable版的git后，解压缩，然后运行git-bash，会出现一个console窗口。
2、android的kernel有多个部分，可根据需要运行以下命令下载需要的内容。
      在git的console窗口中执行以下命令即可下载内核：
    $ git clone https://android.googlesource.com/kernel/common.git  //这个是linux内核，也可以是git clone http://android.git.kernel.org/kernel/common，网上许多地方介绍的命令是git clone git://android.git.kernel.org/kernel/common，但现在似乎是不能用了。
    $ git clone https://android.googlesource.com/kernel/goldfish.git
    $ git clone https://android.googlesource.com/kernel/msm.git
    $ git clone https://android.googlesource.com/kernel/omap.git
    #git clone https://android.googlesource.com/kernel/samsung.git
    $ git clone https://android.googlesource.com/kernel/tegra.git
3、checkout出对应的内核
   进入samsung目录
   $git branch -a    //列出所有可用的内核版本
   $git checkout  remotes/origin/android-samsung-3.0-ics-mr1   //checkout出想要使用的内核
如果要下载所有的android源文件，下载方法见http://source.android.com/source/downloading.html
补充：关于android安装开发过程中的需要的一些开发包，在终端执行以下命令：
          sudo apt-get install git-core gnupg valgrind flex bison gperf libsdl-dev libesd0-dev libwxgtk2.6-dev build-essential zip curl libncurses5-dev zlib1g-dev
