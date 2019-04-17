# win7下 python2和python3 安装和pip配置 - lijunweiyhn的博客 - CSDN博客





2018年01月11日 18:50:10[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：2826








转载：https://www.cnblogs.com/shanhua-fu/p/6912683.html



1、下载python3和python2


 进入python官网，链接[https://www.python.org/](https://www.python.org/)


选择Downloads--->Windows，点击进入就可以看到寻找想要的python版本

![](https://images2015.cnblogs.com/blog/1168035/201705/1168035-20170527141053904-876502659.png)


本文选择的是：


Python3.5.2,点击后面链接可直接下载，[https://www.python.org/ftp/python/3.5.2/python-3.5.2rc1-amd64.exe](https://www.python.org/ftp/python/3.5.2/python-3.5.2rc1-amd64.exe)


Python2.7.9, 点击后面链接可直接下载，[https://www.python.org/ftp/python/2.7.9/python-2.7.9.amd64.msi](https://www.python.org/ftp/python/2.7.9/python-2.7.9.amd64.msi)

2、python3安装

说明：本文先安装python3，然后安装python2

 首先选择安装目录，本文安装路径为C:\python35，然后点击下载好的python3软件包进行安装，具体流程如下：

在下图红色标记的地方Add Python3.5 to PATH前勾选，可以直接将python3添加到系统环境变量，然后在Customize installation中自定义安装路径。

![](https://images2015.cnblogs.com/blog/1168035/201705/1168035-20170527141226419-1135519116.png)

![](https://images2015.cnblogs.com/blog/1168035/201705/1168035-20170527141238247-1030497277.png)

3、python2安装


      点击python2安装包，点击运行，next，选择安装目录，本文安装路径为C:\python27，然后next


 点击finish完成安装。


至此，python2和python3安装完毕。

4.python2.exe、python3.exe和pip2、pip3设置

1、添加python2到系统环境变量


      打开，控制面板\系统和安全\系统，选择高级系统设置，环境变量，选择Path，点击编辑，新建，分别添加D:\Python\python27和D:\Python\python27\Scripts到环境变量。

注意：python3安装时可以选择自动添加到系统环境变量，如未选择，方法和python2添加过程相同。

2、修改python.exe名字为python2.exe和python3.exe


      找到python2和python3的安装目录，修改python2.7.9和python3.5.2中python.exe和pythonw.exe的名称为python2.exe、pythonw2.exe和python3.exe、pythonw3.exe。


      然后在运行cmd命令，输入python2即可运行python2.7.9版本

输入python3即可运行python3.5.2版本

3、pip2 和 pip3设置


      Python 安装包需要用到包管理工具pip，但是当同时安装python2和python3的时候，pip只是其中一个版本，以下将提供一个修改方式，即重新安装两个版本的pip，使得两个python版本的pip能够共存。


      在DOS命令框输入命令，python3 -m pip install --upgrade pip --force-reinstall，显示重新安装成功。

![](https://images2015.cnblogs.com/blog/1168035/201705/1168035-20170527141548200-893354587.png)


现在可以通过pip2 -V 和 pip3-V 查看两个版本的pip信息，以后只需运行pip2install XXX和pip3 install XXX即可安装各自的python包


至此，pip2和pip3 修改成功。




