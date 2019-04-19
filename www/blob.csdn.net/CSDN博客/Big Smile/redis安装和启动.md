# redis安装和启动 - Big Smile - CSDN博客
2017年03月31日 16:57:41[王啸tr1912](https://me.csdn.net/tr1912)阅读数：429
        接触了一些redis的东西也相应的明白了一些关于redis的设置和集群的问题，所以在此和大家分享一下redis安装和启动。注：此文是在centOS环境下搭建的，如有问题还请联系作者。
# 一、下载并解压redis
        我们需要先进入到我们想要解压和下载的目录下面，首先我们先要创建一个可以供我们下载解压包和源文件的目录，以下面mkdir创建文件夹为例：
```
mkdir /usr/local/redis
cd /usr/local/redis
```
        然后我们可以用命令联网下载redis的tar包。
wget http://download.redis.io/releases/redis-3.0.7.tar.gz
        然后我们需要redis运行的环境gcc和Libstdc++安装好：
yum -y install cpp binutils glibc glibc-kernheaders glibc-commonglibc-devel gcc make gcc-c++ libstdc++-devel tcl
        然后我们需要在该目录下解压缩下载好的tar包：
tar -xvf redis-3.0.7.tar.gz
        然后我们需要进入到解压缩之后对应的目录下面：
cd  redis-3.0.7
#  二、编译和安装
        进行编译操作，使用make命令，直接输入make
make
如果提示gcc command不识别，请自行安装gcc;
如果提示couldn’t execute tcl : no such file or dicrectory，请自行安装tcl;
如果出现如图所示错误：
![](https://img-blog.csdn.net/20170331145930086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170331145930086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
则需要执行make distclean，然后再make
如果出现如下图则成功：
![](https://img-blog.csdn.net/20170331150214745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         执行过后可以发现在操作目录的src目录下面多出了一些文件，这些文件就是执行redis程序所需要的必须文件，如下：
![](https://img-blog.csdn.net/20170331150906030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
      可手动拷贝redis-server、redis-cli、redis-check-aof、redis-check-dump等至/usr/local/bin目录下，也可以使用make install命令复制到这个目录下。
然后我们cd到这个目录下：
cd  /usr/local/bin
       执行./redis-server -v，如果出现redis版本信息，则证明可以正常运行，如果你想让我们不用cd到这个目录下就可以直接执行redis-server的话，可以设置环境变量，这个很好设置，如下：
vi /etc/profile
       然后进行编辑，在这个文件内容的最下面添加上如下几句话：
REDIS_HOME=/usr/local/redis-3.2.8
PATH=${PATH}:/usr/local/bin
export REDIS_HOME
export PATH
      然后按ESC，输入:wq 保存退出即可，这个时候随便在哪个目录下面我们都可以使用redis-server命令了。
至此，redis安装完成，可以直接以默认的方式运行了，输入redis-server就可以运行了，配置为默认配置。
![](https://img-blog.csdn.net/20170331152557099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
出现上图说明你的redis已经可以正常工作了。
# 三、redis的配置
        如果你想自己使用持久化的redis并且可以自定义日志和运行时文件的地址的话，那么就跟随下面的步骤一下来吧！
        首先是设置持久化的配置：
在这之前我们需要复制一份配置文件到程序跟目录下，首先我们还是要到我们解压的目录下：
cd  /usr/local/redis/redis-3.0.7
然后复制默认的配置文件到程序目录
cp  redis.conf /usr/local/bin
然后修改配置文件：
vi /usr/local/bin/redis.conf
找到配置文件中的daemonize no  把no改成yes就可以了，保存退出，重新启动redis并且需要以此文件作为启动配置，需要输入一下命令：
redis-server  /usr/local/bin/redis.conf   (配置了环境变量)
如果没有配置环境变量，则需要cd   /usr/local/bin  ，然后再运行 ./redis-server ./redis.conf
启动完成之后就是持久化的redis了。
       然后是日志和运行时文件的设置（非必须）：
        首先我们需要建立几个文件夹，这个文件夹建立在哪里都可以，我们就建立在/usr/local/redis下面吧！
第一步需要cd  /usr/local/redis
然后执行mkdir data log
run  
我们还是需要打开配置文件
vi /usr/local/bin/redis.conf
找到如下图所示的配置项，改为下图所示的配置项和配置内容即可。
![](https://img-blog.csdn.net/20170331164457528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170331164505007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170331164923343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        配置好后，按照上面的带配置文件的启动命令启动，redis就可以执行配置好的操作了，可以查看redis下的run，data，log下面都会有对应的文件了。

