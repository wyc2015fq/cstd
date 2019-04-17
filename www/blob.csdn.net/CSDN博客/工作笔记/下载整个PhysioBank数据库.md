# 下载整个PhysioBank数据库 - 工作笔记 - CSDN博客





2012年01月13日 09:18:54[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6865








首先，不建议这么做。

推荐读取PhysioBank数据文件的方法是用PhysioToolkit软件连接WFDB库，或者自己写软件连接WFDB库。两种方法都需要连接WFDB库，WFDB库执行找和读取PhysioBank的任务。如果你已经有了一个PhysioBank 文件的副本在本地，WFDB库读取这个副本，否则，它会从网络上联网读取。

如果不想用WFDB库读取PhysioBank文件，就得需要对原来文件进行格式转换，变成高存储效率格式。实现这个，需要用WFDB库读取原始文件。这样，同样还是需要WFDB联网读取原始文件，同时，将格式转变后的文件保存在本地

综上，如何还是决定下载整个数据库，则有两种方法实现。

第一种，利用[PhysioNet mirrors](http://physionet.org/mirrors/)中的[rsync](http://rsync.samba.org/),如果没有，安装。

安装方法：

[http://www.samba.org/rsync/](http://www.samba.org/rsync/)


shell> tar zxvf rsync-x.x.x.tar.gz

shell> cd rsync-x.x.x

shell> ./configure && make && make install


目前大部分 Unix/Linux 默认即安装了 rsync。我用的cygwin，也是默认安装了rsync。

利用下面代码：
rsync physionet.org::



然后，通过rsync得到了一个数据库列表 例如下面的这些，实际现在要比这个多很多。

aami-ec13       ANSI/AAMI EC13 Test Waveforms (1 Mb)

afdb            MIT-BIH Atrial Fibrillation Database (607 Mb)

afpdb           PAF Prediction Challenge Database (195 Mb)

aftdb           AF Termination Challenge Database (3 Mb)


整个条目的前面，表示它的文件路径，或者文件单位。例如，下载AF Termination Challenge Database 数据库到它自带的子目录：



rsync -Cavz physionet.org::aftdb /usr/database/aftdb


你可以选择任何存储路径，但是，推荐的这个路径/usr/database，是WFDB库默认的搜索路径，因此选它是最好的。

如果你的网络不靠谱，利用rsync是最方便的方法，如果下载被打断，在网络连接建立后，简单的重复下下载命令，rsync会自动分析从什么地方开始继续下载。

还有一个优点是，rsync会记下PhysioNet上原始文件的时间，如果你回到PhysioNet，根据你上次下载文件的时间，很容易看到原始文件是否进行过更行升级。

如果有更新，你可以通过之前创建数据库时的命令，同步更行文件。

注意，rsync用自己的IANA-assigned port (873); 如果你用浏览器(port 80, HTTP) 连接PhysioNet，你的防火墙会阻止 port 873.

如果不用rsync，或者你连接到一个PhysioNet 镜像（通常不支持rsync）的速度比你连接到PhysioNet主服务器好很多，就用方法2：

其实，这个方法算不上整体下载了，因为它就是利用[PhysioBank ATM](http://physionet.org/cgi-bin/ATM)生成每个记录的压缩文件，然后下载这个压缩文件的。如果想整体下载，还是要用上面的方法，否则，只能在你感兴趣的文件中寻找tar或者zip压缩文件。如果你找不到压缩文件，只能单个文件保存了。有些文件是必须要保留的，但有些多余的留下了就是在浪费资源。

用浏览器一次性下载多个文件，有很多方案。尝试一下公用HTTP传输的方法，例如[wget](http://physionet.org/physiotools/utilities/wget/)

下载地址：[ftp://ftp.cs.cuhk.edu.hk/pub/gnu/gnu/wget](ftp://ftp.cs.cuhk.edu.hk/pub/gnu/gnu/wget)

安装： 

先把wget下载到的tar文件解压，然后cd到wget目录下 

#./configure 

#make 

#make install 

#make clean 

安装完后，敲入下面代码：
wget -r -np http://physionet.org/physiobank/database/mitdb/











