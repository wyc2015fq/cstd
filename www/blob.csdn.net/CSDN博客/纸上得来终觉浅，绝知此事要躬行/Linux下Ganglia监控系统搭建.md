# Linux下Ganglia监控系统搭建 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月04日 15:23:19[boonya](https://me.csdn.net/boonya)阅读数：2379










Ganglia监控软件主要是用来监控系统性能的软件，如：cpu 、mem、硬盘利用率， I/O负载、网络流量情况等，通过曲线很容易见到每个节点的工作状态，对合理调整、分配系统资源，提高系统整体性能起到重要作用。支持浏览器方式访问，但不能监控节点硬件技术指标，ganglia 是分布式的监控系统，有两个Daemon, 分别是：客户端Ganglia Monitoring Daemon (gmond)和服务端Ganglia Meta Daemon (gmetad)，还有Ganglia PHP Web
 Frontend（基于web的动态访问方式）组成。是一个Linux下图形化监控系统运行性能的软件，界面美观、丰富，功能强大。RRDtool是系统存放和显示time-series (即网络带宽、温度、人数、服务器负载等) 。并且它提出有用的图表由处理数据强制执行有些数据密度。


Ganglia 是 UC Berkeley 发起的一个开源监视项目，设计用于测量数以千计的节点。每台计算机都运行一个收集和发送度量数据（如处理器速度、内存使用量等）的名为 gmond 的守护进程。它将从操作系统和指定主机中收集。接收所有度量数据的主机可以显示这些数据并且可以将这些数据的精简表单传递到层次结构中。正因为有这种层次结构模式，才使得 Ganglia 可以实现良好的扩展。gmond 带来的系统负载非常少，这使得它成为在集群中各台计算机上运行的一段代码，而不会影响用户性能。



## 1、lap（linux+apache+php）



（略）


## 2、需要的软件包



通过yum源安装，系统光盘就有：
yum -y install \
apr-devel \
apr-util \
check-devel \
cairo-devel \
pango-devel \
libxml2-devel \
rpmbuild \
glib2-devel \
dbus-devel \
freetype-devel \
fontconfig-devel \
gcc-c++ \
expat-devel \
python-devel \
libXrender-devel


## 3、安装libconfuse



32位os下载：
wget http://download.fedora.redhat.com/pub/epel/5/i386/libconfuse-2.5-4.el5.i386.rpm
wget http://download.fedora.redhat.com/pub/epel/5/i386/libconfuse-devel-2.5-4.el5.i386.rpm

64位os下载：
wget http://download.fedora.redhat.com/pub/epel/5/x86_64/libconfuse-2.5-4.el5.x86_64.rpm
wget http://download.fedora.redhat.com/pub/epel/5/x86_64/libconfuse-devel-2.5-4.el5.x86_64.rpm

安装
rpm –ivh libconfuse-2.5-4.el5.i386.rpm libconfuse-devel-2.5-4.el5.i386.rpm

## 4、安装RRDTool

wget http://oss.oetiker.ch/rrdtool/pub/rrdtool.tar.gz
tar zxvf rrdtool*
cd rrdtool-*
./configure --prefix=/usr
make
make install

## 5、安装ganglia



到[http://ganglia.info](http://ganglia.info/)网站下载最新版的ganglia，这里以3.1.2版为例

这里编译安装的时候需要注意：

安装之前用以下命令检查有没有rrd.h和librrd.a文件：

ll /usr/include/rrd.h
ll /usr/lib/librrd.a

如果存在以上两个文件，则继续安装否则使用find命令查找这两个文件，并创建指向这两个文件的软链接
find / -name rrd.h
find / -name librrd.a

比如说你找到的文件在/usr/local/下，则：
ln -s /usr/local/rrd.h /usr/include/rrd.h
ln -s /usr/local/librrd.a /usr/lib/librrd.a

安装：
tar –zxvf ganglia-3.1.2.tar.gz
cd ganglia-3.1.2.tar.gz
./configure --with-gmetad
make
make install


如果安装过程中出现读取相关lib文件读取错误，请在/usr/lib(32os)或/usr/lib64下查找是否存在该lib文件，如果不存在，查找该文件并在/usr/lib(32os)或/usr/lib64创建指向该lib文件的软链接。


## 6、配置ganglia



### step 1、处理命令行文件

cd /tmp/ganglia-3.1.2/                                //假设你的ganglia在此目录
mkdir -p /var/www/html/ganglia/                       //创建网站主目录下ganglia文件夹，用来访问ganglia
cp -a web/* /var/www/html/ganglia/                    //拷贝ganglia网站代码到该目录
cp gmetad/gmetad.init /etc/rc.d/init.d/gmetad         //拷贝gmetad服务启动脚本
cp gmond/gmond.init /etc/rc.d/init.d/gmond            //拷贝gmond服务启动脚本
mkdir /etc/ganglia                                    //创建配置文件主目录
gmond -t | tee /etc/ganglia/gmond.conf                //生成gmond服务配置文件
cp gmetad/gmetad.conf /etc/ganglia/                   //拷贝gmetad服务配置文件
mkdir -p /var/lib/ganglia/rrds                        //创建rrd文件存放目录
chown nobody:nobody /var/lib/ganglia/rrds             //属主和属组都为nobody
chkconfig --add gmetad                                //将服务交给chkconfig管理
chkconfig --add gmond                                 //同上


### step 2、修改配置文件

vi /etc/ganglia/gmond.conf //修改以下字段
cluster {
name = "cluster name" //你要监控的集群名，一般我们只改这一项，可以是任意值
owner = " unspecified "
latlong = "unspecified"
url = "unspecified"
}

### step3、两块网卡的设置



如果你的服务器有两块网卡，eth0使用公网地址，eth1使用局域网地址，而你的监控服务器和被监控服务器之间的通信你希望通过局域网地址实现以减少公网网卡的负载，那么可以使用以下命令：
ip route add 239.2.11.71 dev eth1

因为239.2.11.71是ganglia默认的多点传输通道，所以要加一条路由使它通过eth1,也就是内网网卡，239.2.11.71这个地址你可以在/etc/ganglia/gmond.conf中修改


### step4、启动服务

service gmond start
service gmetad start
service httpd restart

这时服务端的配置已经结束，通过网页http://You_IP/ganglia已经可以访问，并且可以监控到监控服务器的信息。


## 7、客户端的配置



在监控服务器编写一个脚本文件，执行它可以将服务快速的发布到被监控主机上


### step1、编写一个主机地址文件

vi mynodes

添加你的主机名或服务器ip到该文件，我这里只写了5台主机
192.168.10.1
192.168.10.2
192.168.10.3
192.168.10.4
192.168.10.5


### step2、编写脚本实现服务的发布

vi ganglia.sh


使用以下内容，这里使用的是64位OS，如果是32位OS，将下面的lib64改为lib
for i in `cat mynodes`; do
    scp /usr/bin/gmetric $i:/usr/bin
    scp /usr/sbin/gmond $i:/usr/sbin/gmond
    ssh $i mkdir -p /etc/ganglia/
    scp /etc/ganglia/gmond.conf $i:/etc/ganglia/
    scp /etc/init.d/gmond $i:/etc/init.d/
    scp /usr/lib64/libganglia-3.1.2.so.0 $i:/usr/lib64/
    scp /lib64/libexpat.so.0 $i:/lib64/
    scp /usr/lib64/libconfuse.so.0 $i:/usr/lib64/
    scp /usr/lib64/libapr-1.so.0 $i:/usr/lib64/
    scp -r /usr/lib64/ganglia $i:/usr/lib64/
    ssh $i service gmond start
done


### step3、赋予脚本可执行权限并执行

chmod 755 ganglia.sh
./ganglia.sh

至此ganglia客户端和服务器端的配置完成，可以通过web访问来监控你的群集了


## 8、使用gmetric定制你自己要监控的信息



### step1、准备工作



在使用gmetric前要确定自己被监控服务器有gmetri

ll /usr/bin/gmetric

如果没有，通过scp从监控服务器拷贝
scp root@192.168.10.100:/usr/bin/gmetric /usr/bin/ //假设你的监控服务器为192.168.10.100

### step2、编写脚本



定制你自己的脚本，执行，并且将结果返回监控服务器

你可以在[http://ganglia.info/gmetric/](http://ganglia.info/gmetric/)找到很多别人共享的脚本，以下是一个监控memcache服务的脚本：
vi /root/mcd_gmetric.sh

以下内容
#!/bin/bash

### $Id: mcd_gmetric.sh 16661 2006-11-07 00:56:33Z ben $

### This script queries a memcached server running
### on localhost and reports a few statistics to
### ganglia.
### It reports
### *mcd_curr_items - the number of objects stored
### *mcd_curr_bytes - current bytes used
### *mcd_curr_conns - current number of connections
### *mcd_hit_perc - hits / gets for current time duration
### (current hit percentage)
### For more description on any of these metrics,
### see the protocols.txt file in the MCD docs.

### Copyright Simply Hired, Inc. 2006
### License to use, modify, and distribute under the GPL
### http://www.gnu.org/licenses/gpl.txt

VERSION=1.0

GMETRIC="/usr/bin/gmetric"
GMETRIC_ARGS="-c /etc/ganglia/gmond.conf"
STATEFILE="/var/lib/ganglia/metrics/mcd.stats"
ERROR_NOTROOT="/tmp/mcd_gmetric_notroot"
ERROR_CANT_CONNECT="/tmp/mcd_gmetric_cant_connect"
ERROR_CREATE="/tmp/mcd_gmetric_create_statefile_failed"
ERROR_GETS_EMPTY="/tmp/mcd_gets_empty"

MCD_CONF="/etc/sysconfig/memcached"

# get system configuration
if [ -e ${MCD_CONF} ]
then
source ${MCD_CONF}
MCD_PORT=${PORT}
fi
MCD_PORT=${MCD_PORT:-11211}

date=`date +%s`

if [ $UID -ne 0 ]
then
if [ -e $ERROR_NOTROOT ] ; then exit 1; fi
echo "Error: this script must be run as root."
touch $ERROR_NOTROOT
exit 1
fi
rm -f $ERROR_NOTROOT

if [ "x$1" == "x-h" ]
then
echo "Usage: mcd_gmetric.sh [--clean]"
echo " --clean delete all tmp files"
exit 0
fi

if [ "x$1" == "x--clean" ]
then
rm -f $STATEFILE $ERROR_NOTROOT $ERROR_CANT_CONNECT $ERROR_CREATE
retval=$?
if [ $retval -ne 0 ]
then
echo "failed to clean up."
exit 1
else
echo "All cleaned up."
exit 0
fi
fi

# if the GMETRIC program isn't installed, compain
if [ ! -e $GMETRIC ]
then
if [ -e $ERROR_GMETRIC ] ; then exit 1; fi
echo ""
echo "Error: GMETRIC doesn't seem to be installed."
echo "$GMETRIC doesn't exist."
echo ""
touch $ERROR_GMETRIC
exit 1
fi

# get current statistics
exec 3>&2 #turn off STDERR
exec 2>/dev/null
stats_array=(`echo "stats" | nc localhost $MCD_PORT`)
retval=$?
exec 2>&1 #turn on STDERR
exec 3>&-

if [ $retval -ne 0 ]
then
if [ -e $ERROR_CANT_CONNECT ] ; then exit 1 ; fi
echo "I can't connect to mcd."
echo "Bummer. "
touch $ERROR_CANT_CONNECT
exit 1
fi

mcd_curr_items=`echo ${stats_array[23]}|tr -c -d [0-9]` #this tr thing is because there's a trailing ^M on the string from netcat that breaks bc.
mcd_curr_bytes=`echo ${stats_array[29]}|tr -c -d [0-9]`
mcd_curr_conns=`echo ${stats_array[32]}|tr -c -d [0-9]`
mcd_total_gets=`echo ${stats_array[41]}|tr -c -d [0-9]`
mcd_total_sets=`echo ${stats_array[44]}|tr -c -d [0-9]`
mcd_total_hits=`echo ${stats_array[47]}|tr -c -d [0-9]`

if [ -z "$mcd_total_gets" ]
then
# this actually happens rather often for some reason, so I'm just going to fail silently.
# if [ -e $ERROR_GETS_EMPTY ] ; then exit 1 ; fi
# echo ""
# echo "ERROR: mcd_total_gets empty."
# echo ""
exit 1
fi
rm -f $ERROR_GETS_EMPTY


# save and turn off /STDERR for the statefile tests
exec 3>&2
exec 2>/dev/null

# if the statefile doesn't exist, we either havn't
# run yet or there's something bigger wrong.
if [ ! -e $STATEFILE ]
then
if [ ! -d `dirname $STATEFILE` ]
then
mkdir -p `dirname $STATEFILE`
fi
echo "$date $mcd_curr_items $mcd_curr_bytes $mcd_curr_conns $mcd_total_gets $mcd_total_sets $mcd_total_hits" > $STATEFILE
if [ ! -e $STATEFILE ]
then
# if it didn't exist and we couldn't create
# it, we should just scream bloody murder and die.
# only scream once though...
if [ -e $ERROR_CREATE ]
then
exit 1
fi
echo ""
echo "ERROR: couldn't create $STATEFILE"
echo ""
touch $ERROR_CREATE
exit 1
fi
echo "Created statefile. Exitting."
exit 0
fi

# restore stderr
exec 2>&3
exec 3>&-

old_stats_array=(`cat $STATEFILE`)
old_date=${old_stats_array[0]}
old_mcd_curr_items=${old_stats_array[1]}
old_mcd_curr_bytes=${old_stats_array[2]}
old_mcd_curr_conns=${old_stats_array[3]}
old_mcd_total_gets=${old_stats_array[4]}
old_mcd_total_sets=${old_stats_array[5]}
old_mcd_total_hits=${old_stats_array[6]}

echo "$date $mcd_curr_items $mcd_curr_bytes $mcd_curr_conns $mcd_total_gets $mcd_total_sets $mcd_total_hits" > $STATEFILE

time_diff=$(($date - $old_date))
mcd_total_gets_diff=$(($mcd_total_gets - $old_mcd_total_gets))
mcd_total_sets_diff=$(($mcd_total_sets - $old_mcd_total_sets))
mcd_total_hits_diff=$(($mcd_total_hits - $old_mcd_total_hits))

if [ $time_diff -eq 0 ]
then
if [ -e $ERROR_TIMEDIFF ] ; then exit 1 ; fi
echo "something is broken."
echo "time_diff is 0."
touch $ERROR_TIMEDIFF
exit 1
fi

# none of these numbers should be less than 1, but if they are, just send back 1.
if [ $mcd_total_gets_diff -le 1 ] ; then mcd_total_gets_diff=1 ; fi
if [ $mcd_total_sets_diff -le 1 ] ; then mcd_total_sets_diff=1 ; fi
if [ $mcd_total_hits_diff -le 1 ] ; then mcd_total_hits_diff=1 ; fi

mcd_gets_per_sec=`echo "scale=3;${mcd_total_gets_diff}/${time_diff}"|bc`
mcd_sets_per_sec=`echo "scale=3;${mcd_total_sets_diff}/${time_diff}"|bc`
mcd_hits_per_sec=`echo "scale=3;${mcd_total_hits_diff}/${time_diff}"|bc`
mcd_hit_perc=`echo "scale=3; ${mcd_total_hits_diff} * 100 / ${mcd_total_gets_diff}" | bc`


$GMETRIC $GMETRIC_ARGS --name="mcd_seconds_measured" --value=${time_diff} --type=uint32 --units="secs"
$GMETRIC $GMETRIC_ARGS --name="mcd_items_cached" --value=${mcd_curr_items} --type=uint32 --units="items"
$GMETRIC $GMETRIC_ARGS --name="mcd_bytes_used" --value=${mcd_curr_bytes} --type=uint32 --units="bytes"
$GMETRIC $GMETRIC_ARGS --name="mcd_conns" --value=${mcd_curr_conns} --type=uint32 --units="connections"
$GMETRIC $GMETRIC_ARGS --name="mcd_gets" --value=${mcd_gets_per_sec} --type=float --units="gps"
$GMETRIC $GMETRIC_ARGS --name="mcd_sets" --value=${mcd_sets_per_sec} --type=float --units="sps"
$GMETRIC $GMETRIC_ARGS --name="mcd_cache_hits" --value=${mcd_hits_per_sec} --type=float --units="hps"
$GMETRIC $GMETRIC_ARGS --name="mcd_cache_hit%" --value=${mcd_hit_perc} --type=float --units="%"


赋予脚本可执行权限并执行：
chmod 755 mcd_gmetric.sh
./mcd_gmetric.sh

这时已经将结果通过gmetric返回到监控服务器，在服务器该主机的信息中已经可以看到memcached服务的相关信息了

将这个服务添加到crontab

crontab -e
/1 * * * * /root/mcd_gmetric.sh //设置每分钟将结果返回监控服务器

## 参考文档


[http://www.ibm.com/developerworks/cn/linux/l-ganglia-nagios-1/](http://www.ibm.com/developerworks/cn/linux/l-ganglia-nagios-1/)
[http://idcnews.net/html/edu/20070101/288092.html](http://idcnews.net/html/edu/20070101/288092.html)
[http://blog.csdn.net/Tobeabetterman_He/archive/2006/12/13/1441516.aspx](http://blog.csdn.net/Tobeabetterman_He/archive/2006/12/13/1441516.aspx)


原文地址：[http://www.howtocn.org/ganglia_how_to](http://www.howtocn.org/ganglia_how_to)




