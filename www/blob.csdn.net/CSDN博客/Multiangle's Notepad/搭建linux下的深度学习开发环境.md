
# 搭建linux下的深度学习开发环境 - Multiangle's Notepad - CSDN博客


2016年08月07日 10:55:55[multiangle](https://me.csdn.net/u014595019)阅读数：9144


最近越来越感觉到在win下开发简直浑身难受，各种lib需要逐个下载安装不说，到现在干脆在win下跑不起来了。无奈，只能打算转战linux平台。在搭建环境过程中碰到了各种问题，因此把碰到的一些问题及其解决方法写下来作为备忘。
---

# 1. ubuntu的安装
由于之前ubuntu使用的比较多，所以这次也顺理成章的选择了ubuntu.
关于ubuntu的版本，**注意一定要选择新版本**。目前(2016.8)的最新版本是ubuntu16.04. 我一开始图方便，装的是ubuntu12.04, 因为可以使用wubi无脑安装。结果发现安装是方便了，结果后来越来越麻烦，apt-get什么都装不了, apt-get update以后，想装软件要么找不到装不了，要么依赖库版本太低，总之是越来越麻烦。后来在把系统搞崩溃以后，终于下决心装16.04
这里使用easybcd来安装ubuntu16.04，[安装指导](http://jingyan.baidu.com/article/e4d08ffdace06e0fd2f60d39.html)
注意原文有一段是这样的：
```python
然后点击配置，将menu
```
```python
.lst
```
```python
文件的内容替换成一下文本：
title Install Ubuntu
root (hd0,
```
```python
0
```
```python
)
kernel (hd0,
```
```python
0
```
```python
)/vmlinuz
```
```python
.efi
```
```python
boot=casper iso-scan/filename=/ubuntu-
```
```python
14.04
```
```python
-desktop-amd64
```
```python
.iso
```
```python
locale=zh_CN
```
```python
.UTF
```
```python
-
```
```python
8
```
```python
initrd (hd0,
```
```python
0
```
```python
)/initrd
```
```python
.lz
```
```python
title reboot
reboot
title halt
halt
说明：hd0表示c盘所处的硬盘号，一般电脑只有一个，所以都是hd0；如果有多个硬盘，则根据情况改为hd0、hd1等。
hd0后面的数字表示C盘在硬盘中的分区顺序，每个人的系统不大一样，不知道的可以在磁盘管理里面看一下，本人c盘是第三个分区，因此写为（hd0,
```
```python
2
```
```python
），如果是第一个，写为（hd0,
```
```python
0
```
```python
）即可。
```
其实除了(hd0,0)要根据自己情况修改以外，下面这一行的iso地址和名称也要根据自己的文件进行修改，注意提前将iso文件拷贝到C盘根目录下。
```python
kernel (hd0,
```
```python
0
```
```python
)/vmlinuz
```
```python
.efi
```
```python
boot=casper iso-scan/filename=/ubuntu-
```
```python
14.04
```
```python
-desktop-amd64
```
```python
.iso
```
```python
locale=zh_CN
```
```python
.UTF
```
```python
-
```
```python
8
```
接下来就按照流程走。注意在分区过程中要设一个swap分区，类似于win下的虚拟内存。因为听说如果不设置swap分区的话，有些程序会造成内存泄漏。所以我这里就象征性的设置了200M
此外还要把主要的分区格式设为ext4,用来挂载根目录 / ，我这次是把一个80G的分区拿来用了。
---

# 2. apt-get相关
装好系统以后，第一件事情自然是**apt-get update**了。原来我是用163源的，结果这次发现163的源有好多报错，所以就改用官方服务器了，发现速度也还可以，虽然确实比国内要慢一些，但是胜在省心。
另外，除了普通的apt-get install packname以外，apt-get还有许多很实用的功能，比如说**apt-get upgrade**，可以自动更新库。如果在安装中碰到有依赖库没有安装，则运行**apt-get -f install**就会自动安装所依赖的包了。之后再运行apt-get install pack 就可以顺利安装。此外，
**apt autoremove**能够自动清除没有用的包，
**apt-get remove pack**能够手动卸载包
**apt-get upgrade pack**手动更新指定包
---

# 3. 必要软件安装
工欲善其事必先利其器，一些必要的软件也是非常有必要安装的
---

## 3.1 tmux
tmux是一款终端中的分屏工具，很好用！
```python
apt-
```
```python
get
```
```python
install tmux
```
![](https://img-blog.csdn.net/20160807133409393)
tmux的使用方法见[tmux快捷键](http://blog.csdn.net/hcx25909/article/details/7602935)
---

## 3.2 flash插件
作为一台电脑，免不了要看看视频听听歌的。这时就需要flash插件。我之前试过从adobe官网下载tar包，不过不管用。最后还是apt-get解决的。
```python
apt
```
```python
-get
```
```python
install flashplugin
```
```python
-installer
```
Done
---

## 3.3 chrome
这个其实不是很强求，我因为在win下本身就是用的chrome，有很多书签和插件需要使用，所以需要安装chrome. 总的来讲，安装chrome的流程还是比较简单的，先从[这里](http://www.google.cn/chrome/browser/desktop/index.html)下载对应的deb包，然后使用dpkg安装。如果有提示缺少依赖包，就先运行下apt-get -f install 然后再用dpkg安装一下即可
---

## 3.4 IDEA
IDEA是IntelliJ开发的IDE，可以编写java,python等多种程序。我下载的是IDEA16 旗舰版，采用本地LicenseServer激活。破解的具体教程参考[这里](http://blog.csdn.net/lzh984294471/article/details/51019675)简单的来说，就是
先下载好idea的tar包，解压完以后先不要动
从[http://pan.baidu.com/s/1o7A05We](http://pan.baidu.com/s/1o7A05We)下载一个LicenseServer，解压，得到许多文件
![](https://img-blog.csdn.net/20160330213320251)
因为我是ubuntu64位，所以选择linux amd64版本。但是注意，此时文件是运行不了的，因为没有执行的权限，要先chmod u+x IntelliJIDEALicenseServer_linux_amd64来赋予可执行权限。接下来，在运行./ IntelliJIDEALicenseServer_linux_amd64来运行server. 注意，这个服务器占用1017端口，而只有root才能使用1024以下的端口，所以要先获得root权限
运行后关闭终端，打开idea，输入[http://127.0.0.1:1017](http://127.0.0.1:1017)如图：
![](https://img-blog.csdn.net/20160330214005300)
Done~
---
3.5 flux
flux是一款护眼工具，能根据一天中的不同时间自动调节屏幕亮度。安装这个有点麻烦，需要先导入flux的ppa源
sudo add-apt-repositoryppa:nathan-renniewaldock/flux
sudo apt-getupdate
sudo apt-getinstall fluxgui
---
4. python的安装
---
4.1 安装pip
既然是搭建深度学习环境，同时作为python玩家，python自然是少不得的。当然了，由于ubuntu在安装的时候已经装好了两个版本的python,分别是2.7和3.5. 所以主要讲相关库的安装。
首先是pip的安装。注意pip是安装python2.7的库，而pip3才是安装python3.5的库。运行
apt-getinstall python3-pip开始安装pip3. 这里我要再提一下，一定要用最新版本的ubuntu，我一开始就是在这一步死活装不上，换成16.04以后一步成功。
---
4.2 安装第三方库
有了pip,就可以进行接下来相关库的安装了。我用到的主要有
numpy 矩阵运算
scipy 科学运算
pandas 科学计算
gensim  自然语言处理
jieba 分词
networkx 网络分析库
redis Redis数据库的接口
pymongo MongoDB的接口
pymysql MySQL的接口
matplotlib 绘图库
seaborn 绘图库
以上这些库使用
pip3installpackagename即可进行安装。注意，如果下不动，可以考虑使用豆瓣的源
pip3 install -i http://pypi.douban.com/simple/ packname如果还是下不动，那可以考虑
apt-getinstall python3-packname如果还是不行，那就只能考虑像windows下大部分情况一样，跑到相关库的官网或者第三方站点下载whl 文件，然后
pip3installpackname.whl如果下到的是源码，即解压以后里面有setup.py文件，可以试试
python3 setup.pyinstall
---
4.3 TensorFlow安装
Tensorflow是Google开源的深度学习库。严格来说，tensorflow属于python的一个库，但是由于安装方式比较特殊以及太过重要，所以单独讲一下
首先进入tensorflow的[官网](https://www.tensorflow.org/get_started)，但是不知道什么原因，tensorflow居然被GFW给墙了，我也是搞不懂。。所以，下载tensorflow需要先翻墙。
进入官网以后，选择进行[pip安装](https://www.tensorflow.org/versions/r0.10/get_started/os_setup.html#pip-installation)然后按照流程来。我的系统是ubuntu16.04 64位，python版本是python3.5,因为版本太新所以暂时没法用CUDA,所以选择的是CPU-ONLY，所以综合各项情况，选定的版本是[这个](https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0rc0-cp35-cp35m-linux_x86_64.whl)
但是这里有个问题，原来的pip安装是直接从url下载的，也就是下载-安装一起完成，而我这里因为下载太慢，使用pip安装时报错timed out,所以我这里稍作改动，先用浏览器下载whl，然后再用pip安装，最后的效果是一样的
最后来测试一下
$ python3
...>>>importtensorflowastf>>>hello = tf.constant('Hello, TensorFlow!')>>>sess = tf.Session()>>>print(sess.run(hello))
Hello, TensorFlow!>>>a = tf.constant(10)>>>b = tf.constant(32)>>>print(sess.run(a + b))42>>>说明已经安装好了
---
5. linux 常用知识
5.1 一些常用linux指令
dpkg -i packname\# 安装deb文件su username\# 切换用户ln-s\# 建立软链接xdg-open\# 使用系统默认的应用打开当前文件ls-lfilename\# 查找link链fdisk-l\# 查看磁盘信息ps -ef && ps -ef | grep pname\# 查看各进程netstat\# 查看端口占用top\# cpu使用率iftop\# 流量使用率iotop\# 磁盘读写情况lsof\# 显示文件使用情况，包括进程，端口pwd\# 当前文件所在全路径chmod\# 改变文件权限chown\# 改变文件拥有者whereis\# 查找文件trickle\# 限制程序流量ssh                 
sshfs5.2 常用文件所在位置
/etc/apt/source.list\# 软件源位置/etc/apt/source.list.d/*\# 由第三方添加PPA源引入的软件源位置/etc/apparmor.d/*\# apparmor参数设置/etc/mysql/mysql.conf.d/mysqld.conf\# mysql的参数设置5.3 一些帖子
[Apparmor——Linux内核中的强制访问控制系统](http://www.cnblogs.com/-Lei/archive/2013/02/24/2923947.html)
[ linux添加开机自启动脚本示例详解](http://blog.chinaunix.net/uid-26000296-id-3986914.html)
[更新Ubuntu Kylin源遇到问题的解决办法（GPG错误等）](http://www.linuxdiyf.com/linux/9996.html)
[linux系统下 Ubuntu如何使用命令行移除PPA仓库](http://www.jb51.net/LINUXjishu/228069.html)
[Ubuntu基础知识：关于PPA那些事儿](http://www.linuxidc.com/Linux/2011-06/36808.htm)
6. Docker 安装
docker是一个新的虚拟化工具，具体的就不详述了。总的来讲，安装流程官网都有的，照着官网流程来就行。因为我的系统是ubuntu16.04 ，所以是按照[这个](https://docs.docker.com/engine/installation/linux/ubuntulinux/)来的。 不过为了便于查看，这边记录简化版的流程。
$ sudo apt-getupdate
$ sudo apt-getinstall apt-transport-httpsca-certificates$ sudo apt-keyadv--keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D`   \# 增加GPG key打开/etc/apt/sources.list.d/docker.list， 往其中写入deb https://apt.dockerproject.org/repo ubuntu-xenial main增加docker的apt源
$sudo apt-get update$sudo apt-get purge lxc-docker\# 去掉之前的repo$apt-cache policy docker-engine\# 确认apt能够获得对应的库\# 下面这条命令可以让我们使用aufs storage driver$sudo apt-get install linux-image-extra-$(uname -r) linux-image-extra-virtual$sudo apt-get install docker-engine\# 正式安装$sudo service docker start\# 启动docker$sudo docker run hello-world\# 确认docker正常安装\# 至此docker已经安装完毕，接下来是一些相关的配置$sudo groupadd docker\# 增加docker用户组$sudo usermod -aG docker$USER\# 将你常用账号加到docker组里，这样不用root就能运行docker了\# 登出，再登入$docker run hello-world\# 测试是否需要root权限注意上面那行$ sudo docker run hello-world那里，可能会报错，因为国内网络你懂得，有时候会提示下载失败，timeout之类的。此时docker已经安装完成需要下载images。在[docker hub](https://hub.docker.com/)上有很多images可以选择，我选了个最简单的ubuntu
$docker pull ubuntu\# 从docker hub下载ubuntu的images$docker images此时就能够看到下载好的ubuntu。因为docker使用了aufs，所以相关的images非常小，只有120M左右。
![这里写图片描述](https://img-blog.csdn.net/20160831223436561)
$ docker run ubuntuecho"23333">>>23333\# 说明可以通过docker来使用下载好的ubuntu了输入
$dockerrun-t-iubuntu可以进入虚拟机的shell中
7 Hadoop和Spark的安装
7.1 Hadoop安装
关于hadoop的安装步骤，可以参见我之前的文章[linux下命令行安装hadoop2.7.2过程](http://blog.csdn.net/u014595019/article/details/50804625)。下面主要讲讲spark的安装过程。spark与hadoop一样，解压以后再配置一下就能用了。
7.2 Scala安装
因为spark是用scala写的，虽然提供了python,java等接口，但是还是要安装scala才能使用。
1.从scala官网下载[下载地址](http://downloads.lightbend.com/scala/2.11.8/scala-2.11.8.tgz)
2.解压scala，放入安装位置。我是安装在/usr/lib/scala中的
tar -zxvf scala-2.11.8.tgz\# 解压mv scala-2.11.8/usr/lib/scala\# 将解压后的文件夹移入/usr/lib/scala3.将SCALA_HOME加入环境变量。打开/etc/profile,往最后写入
exportSCALA_HOME=/usr/lib/scalaexportPATH=.:$SCALA_HOME/bin:$PATH保存退出，在shell中执行source /etc/profile
4.最后测试一下是否安装成功
scala -version=>Scala code runnerversion2.11.8-- Copyright 2002-2016, LAMP/EPFL7.3 Spark安装
1.从[spark官网](http://spark.apache.org/downloads.html)下载spark,根据你的hadoop版本来。我选了prebuild for hadoop2.7 and later.
2.下载完以后解压，移到你想要安装的地方。我放在/home/hamaster/spark
tar-zxvfspark-2.0.1-bin-hadoop2.7.tgz
mv spark-2.0.1-bin-hadoop2.7/home/hamaster/spark4.将SPARK_HOME加入环境变量。跟之前一样，打开/etc/profile,写入
exportSCALA_HOME=/home/hamaster/sparkexportPATH=.:$SPARK_HOME/bin:$SPARK_HOME/sbin:$PATH保存退出，shell中执行source /etc/profile
5.修改spark的配置文件。
cd $SPARK_HOME/confcpspark-env.sh.templatespark-env.shcpslaves.templateslaves6.打开spark-env.sh,往末尾写入
\#\#\#\#\#\#\#\#\#\#\#\# my own spark conf \#\#\#\#\#\#\#\#\#\#\#\#\#\#\# the location of the dependencies\#export JAVA_HOME=/usr/lib/jvm/java-8-oracleexportHADOOP_HOME=$HADOOP_INSTALLexportHADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop\#\#\# the basic conf info of spark\#export SCALA_HOME=/usr/lib/scala\#export SPARK_HOME=$HOME/sparkexportSPARK_MASTER_IP=127.0.0.1exportSPARK_MASTER_PORT=7077exportSPARK_MASTER_WEBUI_PORT=8099\#\#\# the detail conf info of sparkexportSPARK_WORKER_CORES=1\#每个Worker使用的CPU核数exportSPARK_WORKER_INSTANCES=1\#每个Slave中启动几个Worker实例exportSPARK_WORKER_MEMORY=10G\#每个Worker使用多大的内存exportSPARK_WORKER_WEBUI_PORT=8081\#Worker的WebUI端口号exportSPARK_EXECUTOR_CORES=1\#每个Executor使用使用的核数exportSPARK_EXECUTOR_MEMORY=1G\#每个Executor使用的内存7.测试spark是否安装成功
cd /home/hamaster/hadoop/sbin
./start-dfs.sh
cd /home/hamaster/spark/sbin
./start-master.sh
./start-slaves.sh
jps8. 安装cuda和cudnn
cuda和cudnn都是nvidia家的使用gpu来对计算进行加速的工具，具体安装过程比较长，我后来装了一次，过程写在[ubuntu16.04下安装CUDA，cuDNN及tensorflow-gpu版本过程](http://blog.csdn.net/u014595019/article/details/53732015)

