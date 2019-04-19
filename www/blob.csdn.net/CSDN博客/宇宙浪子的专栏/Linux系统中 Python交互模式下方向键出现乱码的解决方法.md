# Linux系统中 Python交互模式下方向键出现乱码的解决方法 - 宇宙浪子的专栏 - CSDN博客
2014年12月16日 15:44:49[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：2563
**1.   安装readline模块**
readline 是一个强大的库，只要使用了它的程序，都可以用同一个配置文件配置，而且用同样的方法操作命令行，让你可以方便的编辑命令行。
    在shell中执行：
sudo apt-get install libreadline-dev
**2.重新安装python**
    $cd pythonHomeDir
    $ ./configure
    $cd Modules
    $vim Setup // 取消‘#readline readline.c -lreadline -ltermcap‘行的注释（line
 166 in V2.7.8)
    $sudo make install
