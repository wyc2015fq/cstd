
# caffe+CPU︱虚拟机+Ubuntu16.04+CPU+caffe安装笔记 - 素质云笔记-Recorder... - CSDN博客

2016年12月07日 11:42:12[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：10386




由于本机是window10系统，所以想尝试caffe就在自己电脑上整了一个虚拟机（详情可见：[win10系统搭建虚拟机：VMware Workstation Player 12环境+Ubuntu Kylin 16.04 LTS系统](http://blog.csdn.net/sinat_26917383/article/details/53230390)），然后昨天在自己虚拟机上配置了一个caffe环境。
其中，只是尝试着搭建了一个CPU版本的caffe，Ubuntu16.04中自带了python 2.7。
在安装的过程中，真的会出现各种各样的报错，但是...
感慨天朝的实力... 每一个bug都能搜索到如何解，所以...  不要担心，百度、谷歌一起教你装caffe。

——————————————————————————————————————————————

本篇内容为两个：caffe安装流程+报错举例
一、caffe安装流程：安装依赖、配置python、配置caffe、配置caffe的makefile文件、配置python caffe
二、报错举例：linux系统的报错问题、caffe报错问题、python报错问题、虚拟机问题

—————————————————————————————————


# 一、caffe安装流程

caffe安装流程：安装依赖、配置python、配置caffe、配置caffe的makefile文件、配置python caffe


## 1、
## 安装依赖


```python
sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get install --no-install-recommends libboost-all-dev
sudo apt-get install libatlas-base-dev
sudo apt-get install libhdf5-serial-dev
```


## 2、
## 配置python

一般linux都自带python，看看python版本：python2.7 --version
之后还要执行：

```python
sudo apt-get install python-dev
```
加载之后：

```python
sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev
```
以上所有sudo apt-get在什么文件夹下输入都无所谓


## 3、
## 配置caffe

把CAFFE的源代码下载下来:

`git clone https://github.com/BVLC/caffe.git`如果没有git,还需要安装git：
`apt-get install git`

下载完成之后,进入CAFFE文件夹, 进入里面的PYTHON文件夹,然后输入

`for req in $(cat requirements.txt); do pip install $req; done`
如何寻找到linux的caffe的路径呢？
貌似直接cd caffe/python一般就能够直接转给去

pip没有安装，还需要安装pip或者easy_install（先修改环境，再sudo）：
修改python的环境变量：export PATH="$PATH:/usr/local/bin/ python
`sudo apt install python-pip`
更新pip:pip install -U pip


## 4、配置caffe的makefile文件

到CAFFE文件夹, 使用模板写个Makefile.config
`cp Makefile.config.example Makefile.config`
-因为CPU MODE, 所以在CPU_ONLY := 1前面的\#要去掉.
-两个路径要改成这样:(添加后面的两个hdf5的路径, 否则编译时报hdf5错误)
`# Whatever else you find you need goes here.
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial`
**（注意：/usr之前有一个“空格”,而且这个是在makefile中进行修改）**


设置好文档名称之后，就要设置make：
`make pycaffe
make all
make test
make runtest`
这个make默认是用CPU单核运算,如果想要快一点, 比如我想使用四核, 在make后面加上-j4标签.
如果上面4行某一行报错之后想要重试,建议先make clean再重新开始.
出现的问题：不能修改文件内容，那么可以用：
`sudo gedit Makefile.config`
报错：
`make: *** No rule to make target 'pycaffe'.  Stop.`
需要make clean一下，才能再次make all ，同时cd随便哪个目录都行



## 5、
## 配置python caffe

make之后，需要设置一下环境变量：
`export PYTHONPATH=/caffe/python:$PYTHONPATH`
去到CAFFE文件夹里面的python文件夹, 把当前路径记录下来(pwd).注意是cd caffe里面python文件夹下，才行，不然直接调用会报错
接下来就可以打开python,然后：
`import caffe`import之后如果没报错，就是完全安装好了。

当然，报错无数...本节主要内容可参考博客：[Ubuntu 16.04上安装Caffe(CPU only)](http://www.linuxidc.com/Linux/2016-09/135034.htm)

—————————————————————————————————


# 二、报错举例

报错举例：linux系统的报错问题、caffe报错问题、python报错问题、虚拟机问题

## 1、linux系统的报错问题

**（1）报错：E: Unable to locate package lib**
解决：apt-getupdate

**（2）报错： 无法获得锁 /var/lib/apt/lists/lock - open (11 资源临时不可用)**

一般都是有其他apt在使用，需要吊销：

`1、找出apt-get进程（下载进程）
一般ubu里面都会出现的吧，因为刚刚开始系统，系统后台都会默认帮你预装什么东西
查找进程代码：
ps -df | grep apt
然后kill：
kill -9 2542 2543（
报错：bash: kill: (2542) - Operation not permitted
bash: kill: (2543) - Operation not permitted
）
sudo kill -9 2542 2543`
参考链接：http://www.linuxidc.com/Linux/2014-06/103437.htm

**(3)不知道apt-get之后的安装包位置？**

apt-get安装之后，一般会在/var/cache/apt/archives文件夹之内
当然，如果sudo apt install之后，以及安装过的，就不会再次安装的。
参考：http://blog.csdn.net/u013797029/article/details/44492677

## 2、caffe报错问题

**（1）报错：error:"make all" "make test"**

`Error: 'make all' 'make test'
.build_release/lib/libcaffe.so: undefined reference to cv::imread(cv::String const&, int)' 
.build_release/lib/libcaffe.so: undefined reference tocv::imencode(cv::String const&, cv::_InputArray const&, std::vector >&, std::vector > const&)'`

原因：caffe代码中并没有build文件夹，需要新建build文件夹之后再进行编译：
`cd caffe-master　　#打开caffe所在文件夹
cp Makefile.config.example Makefile.config  #change setting in Makefile.config
make all -j8　　#在build文件夹下进行编译
make test -j8 
make runtest -j8　　#使用CPU多核同时进行编译`

## 3、python报错问题

**（1）pycaffe时报错：fatal error: numpy/arrayobject.h: No such file or directory**
其实numpy已经是安装的，anaconda2里面有，**Python**中import numpy也没有问题，但就是在此处报错，解决方法：
`sudo apt-get install python-numpy`参考：http://blog.csdn.net/wuzuyu365/article/details/52430657

**（2）****ImportError: No module named google.protobuf.internal**
再加载一次。
`sudo pip install protobuf`

**（3）Matplotlib is building the font cache using fc-list. This may take a moment**
再次调用import caffe时候，出现的问题
**需要设置一下路径export PYTHONPATH=/caffe/python:$PYTHONPATH  ,在这之前，很重要的是要去到caffe下的python文件夹下。**


## 4、虚拟机问题

（1）虚拟机的显卡是跟着本机的吗？
`lspci |grep VGA
00:0f.0 VGA compatible controller: VMware SVGA II Adapter`因为我是在VMware中运行的，所以显卡貌似是VMware SVGA II，好像跑不了GPU...

**（2）一些文件夹无法获得root权限**
笔者在使用的时候会出现只有通过命令先root，才能修改一些文件夹，进行复制和粘贴。
但是不能在界面中直接操作的问题？
这一问题的答案见博客：[ubuntu 16.04 启用root用户方法](http://blog.csdn.net/sunxiaoju/article/details/51993091)
可以很好地解决root问题，就是重新再成立一个root账号，这样你就可以直接拥有root界面啦。

# （3）VMware Player cannot connect to the virtual machine

![](https://img-blog.csdn.net/20161206120454928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用管理员身份运行即可.
针对这一问题，官网给出的解答是两个原因：
1、VMware授权服务没有运行，解决办法参考见blog：http://www.2cto.com/os/201111/112418.html
2、服务不具有管理员权限，用管理员身份运行

——————————————————————————————

延伸一：使用安装VMware Tools工具进行文件共享

跟虚拟机进行文件共享需要下载一下VMware Tools,那么步骤为：
**1、点击安装vmware tools**

![](https://img-blog.csdn.net/20170222115916361)
我的机器是“重新安装”

**2、光驱vmware tools**

点开之后，由于是不能在这个里面进行操作的，所以需要复制到机器
`cp /media/user/VMware\ Tools/VMwareTools-10.0.10-4301679.tar.gz /`
然后进行解压：
`tar -zxvf VMwareTools-10.0.10-4301679.tar.gz`
解压完成后进入解压文件夹vmware-tools-distrib
`cd vmware-tools-distrib`运行安装程序：
`./vmware-install.pl`一路y加回车
![](https://img-blog.csdn.net/20170222120137440)

**3、****设置共享文件夹**
在虚拟机->设置，添加自己的共享文件夹
![](https://img-blog.csdn.net/20170222120203499)

然后就完成了，你就可以在路径：
`cd /mnt/hgfs`
看到自己的共享文件夹了。


