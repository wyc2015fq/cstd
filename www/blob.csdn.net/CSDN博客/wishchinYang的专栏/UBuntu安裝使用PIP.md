# UBuntu安裝使用PIP - wishchinYang的专栏 - CSDN博客
2014年10月07日 10:37:55[wishchin](https://me.csdn.net/wishchin)阅读数：696
Windows下安裝python包還是比較方便的,直接在FLD網站下載對應的EXE文件就可以安裝,在linux系統下,使用pip,easy egg 管理工具可以減輕安裝負擔.
原文鏈接:[http://www.2cto.com/os/201305/213725.html](http://www.2cto.com/os/201305/213725.html)
參       考: [linux-python安裝第三方包](http://www.crifan.com/summary_methods_of_install_third_party_library_or_package_of_python/)
Ubuntu下安装pip的方法：
Install pip and virtualenv for Ubuntu 10.10 Maverick and newer
$ sudo apt-get install python-pip python-dev build-essential 
$ sudo pip install --upgrade pip 
$ sudo pip install --upgrade virtualenv 
For older versions of Ubuntu
Install Easy Install
$ sudo apt-get install python-setuptools python-dev build-essential 
Install pip
$ sudo easy_install pip 
Install virtualenv
$ sudo pip install --upgrade virtualenv 
安裝擴展包示例:
     安装[mysql](http://www.2cto.com/database/MySQL/)拓展包可以：sudo pip install mysql-python
     安装其他的包只要pip + 包的名字 就行了 很方便。而且安装完的mysqlib用起来没一点问题,
     在[windows](http://www.2cto.com/os/windows/)下还要修改下源文件。写程序还是在linux下好啊。
安装mysql 
     apt-get install mysql-server
