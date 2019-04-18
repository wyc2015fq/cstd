# Linux下安装Oracle 9i - ljx0305的专栏 - CSDN博客
2008年08月02日 22:21:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：650标签：[oracle																[linux																[数据库																[disk																[path																[数据库服务器](https://so.csdn.net/so/search/s.do?q=数据库服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=disk&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)
Oracle9i 2000年10月在Oracle Open World上发布，为 Oracle 数据库、应用服务器和开发工具引进了许多新功能。Oracle9i是业界第一个完整、简单的用于互联网的新一代智能化的、协作各种应用的软件基础架构。Oracle9i 实际上是指 Oracle9i Database, Oracle9i Application Server 和Oracle9i Developer Suite的完整集成。随着软件逐渐开始转变为一种托管服务（hosted services）， 具有Internet上的高伸缩性能的、智能化的、和可靠的Oracle9i 将成为高质量的电子商务服务实现的关键软件。本文将介绍Oracle Database 9i在Linux下的安装过程，如果你是有过安装Oracle的经验本安装过程可以权当快速安装手册；如果你以前从未在Linux下安装过Oracle数据库，那我们就从这篇文章开始熟悉Oracle的安装过程。  
系统要求：  
以下的系统要求适用于典型的Oracle安装和创建简单数据库的方式。  
内存：  
安装Oralce 9i软件至少需要512M内存，用以下命令可以查看主机内存大小：  
grep MemTotal /proc/meminfo  
MemTotal: 900252 kB  
900252kB就是你系统的内存大小。  
交换区：  
交换区的大小一般要求是内存的两倍，至少要求达到400M以上，当然是越大越好，用以下的命令可以查看系统交换区的大小：  
/sbin/swapon -s  
Filename Type Size Used Priority  
/dev/sda6 partition 105221 686976 -1  
其中105221就是系统交换区的大小。  
光驱：  
如果你使用光盘安装Oracle9i则你的机子上需要8速以上的CDROM，如果你是下载了Oracle9i的包文件，则不需要使用的CDROM。  
硬盘空间：  
安装Oracle9i数据库至少要有2.5GB以上的剩余空间。  
临时硬盘空间：  
Oracle安装程序在安装过程中需要400M以上的临时硬盘空间，建议使用/tmp文件夹作为零时文件夹，如果/tmp文件没有足够的硬盘，可以新创建一个文件夹作为安装的临时目录，之后设置环境变量TEMP和TMPDIR指向相应的位置，例如：  
使用bash  
mkdir /home/temp  
TEMP=/home/temp ; export TEMP  
TMPDIR=/home/temp ; export TMPDIR  
使用csh  
mkdir /home/temp  
setenv TEMP /home/temp  
setenv TMPDIR /home/temp  
操作系统：  
Oracle公司官方公布的资料指出Oracle 9i只在安装SuSuSe 7.1, 内核 2.4.4 和glibc2.2的系统上测试通过，经过本人的测试，Oracle在Linux Mandrake release 8.0，内核2.4.3-20和glibc-2.2.2的版本上也可以顺利安装，本文将以Linux Mandrake8.0为例介绍Oracle9i的安装过程。  
虚拟x-windows软件：  
这个软件不是必要的！所谓虚拟x-windows软件指的是可在远程终端允许服务器x-windows的虚拟软件，现在流行的x-windows软件有exceed、x-win32等软件，如果你嫌在控制台安装Oracle系统麻烦，可以使用虚拟x-windows软件在远程终端在图形界面下安装oralce9i，本文将以x-win32 5.0为例介绍用虚拟x-windows安装Oracle9i的过程。  
JDK  
如果你要安装Oracle HTTP Server还需要用到blackdown的JDK1.3.1，请到以下地址下载ftp://ftp.progsoc.uts.edu.au/pub ... -linux-i386.tar.bz2  
配置内核参数  
Oracle9i使用Linux的共享内存、交换区等资源进行工作，如果你的内核参数设置不能满足Oracle的要求，那在安装oracel9i或使用过程就会频频出现问题，因此配置系统内核的参数就显得尤为重要和关键了。  
内核参数的配置一般在/proc文件夹下配置：  
1. 以root用户允许以下命令；  
2. 进入目录/proc/sys/kernel；  
3. 用cat命令或more命令查看semaphore当前参数的值：  
cat sem  
命令运行后将会出现如下的结果：  
250 32000 32 128  
其中, 250 是参数SEMMSL的值,32000是参数SEMMNS的值, 32是参数SEMOPM的值，而128则是参数SEMMNI的值。  
4. 用以下的命令可以对上述参数进行修改  
echo SEMMSL_value SEMMNS_value SEMOPM_value SEMMNI_value > sem  
其中SEMMSL_value、SEMMNS_value、SEMOPM_value、SEMMNI_value分别用相应的值进行替换，并且这些值的顺序不能调换  
5. 设置共享内存大小，共享内存大小一般设为物理内存的一半，在这里我们假设物理内存为512M则共享内存的值4294967295以此类推，如果你的物理内存是1G则这里的值则是8589934590：  
echo 4294967295 > shmmax  
添加用户  
Oracle在安装和使用中需要用特定用户（非root用户），按照Oracle的标准说明是需要添加三个专门用户和用户组，为了简便大家的安装和使用我们把Oracle的安装和使用归到一个特定用户来完成。  
首先创建Oracle用户组，我们架设这个用户组命名为dba：  
以root用户登陆系统；  
运行groupadd dba命令添加dba用户组；  
添加Oracle用户：  
以root用户登陆系统；运行如下命令：  
useradd –g dba –p password –d /Oracle –s /bin/bash Oracle  
运行后系统创建了一个属于dba用户组的用户Oracle，密码为password，主目录为/Oracle使用bash  
这个用户将作为系统的安装和使用指定用户，因此要妥善保存好！  
创建安装点(mount point)  
Oracle9i的典型安装需要至少两个安装点：一个安装基本的运行程序，要求至少要有850M的硬盘空间；一个为存放数据库，至少要求有450M的硬盘空间。为了简化安装我们可以把运行程序和数据库装在同一个安装点下。  
在你的文件系统上找到有足够空间的分区，在分区下创建文件夹，我们假设这个文件夹为/Oracle。  
配置系统环境变量  
很多网友安装Oracle失败都是因为环境变量没有配置正确，环境变量的配置直接影响到以后Oracle9i的安装和配置，在配置的时候要尤为小心！  
配置x-windows变量  
确认Oracle9i在安装过程中是否使用本地x-windows安装还是远程虚拟x－windows安装，如果需要远程x-windows安装，则需要配置DISPLAY变量，这个变量用于告诉系统屏幕的图形将输出到什么位置，默认情况下是本机，如果你使用虚拟x－windows进行安装，则在这里指明远程终端的显示情况，比如你远程终端的IP地址是xxx.xxx.xxx.xxx则DISPLAY的变量应设为“xxx.xxx.xxx.xxx:0”后面的“:0”表示该终端的第一个显示器。  
确定安装临时目录  
前面我们提到过Oracle9i的安装需要一个临时的可写空间，我们在这里把/tmp作为临时的可写目录。如果你不是使用/tmp作为临时可写目录则需要配置相应的值TMPDIR=/path。
 配置Oracle的环境变量  
下面提供一个例子可以供大家参照使用  
export DISPLAY="192.9.200.24:0.0"  
export BASH_ENV=$HOME/.bashrc  
Oracle_HOME=/Oracle/product/9.0.1; export Oracle_HOME  
Oracle_SID=Oracle; export Oracle_SID  
Oracle_TERM=xterm; export Oracle_TERM  
TNS_ADMIN=/home/Oracle/config/9.0.1; export TNS_ADMIN  
NLS_LANG=american_america.ZHS16GBK; export NLS_LANG  
ORA_NLS33=$Oracle_HOME/ocommon/nls/admin/data; export ORA_NLS33  
LD_LIBRARY_PATH=$Oracle_HOME/lib;export LD_LIBRARY_PATH  
PATH=$PATH:/bin:/usr/bin:/usr/sbin:/etc:/opt/bin:  
/usr/ccs/bin:/usr/openwin  
PATH=$PATH:/opt/local/bin:/opt/NSCPnav/binOracle_HOME/bin  
PATH=$PATH:/usr/local/samba/bin:/usr/ucb:  
export PATH  
CLASSPATH=$Oracle_HOME/JREOracle_HOME/jlib:  
$Oracle_HOME/rdbms/jlib  
CLASSPATH=$CLASSPATHOracle_HOME/network/jlib  
TMPDIR=/tmp;export TMPDIR  
umask 022  
其中：  
Oracle_HOME为系统软件的安装目录；  
Oracle_SID 为数据库的SID，这里可以自行设置；  
NLS_LANG 为数据库的字符集，为了保证数据库能够输出输入数据库，我们需要在这里把字符集设为american_america.ZHS16GBK，其中american_america英文字符集，ZHS16GBK为中文字符集。  
以Oracle用户登陆系统，  
vi $HOME/.bash_profile  
把以上环境变量的设置粘贴到文件中，确认相应的内容并修改，存盘退出。  
重新登陆Oracle用户  
使用set|more命令查看Oracle用户的环境变量是否生效  
CLASSPATH=/Oracle/product/9.0.1/JRE:/Oracle/product/9.0.1/jlib:  
/Oracle/product/9.0.1/rdbms/jlib:  
/Oracle/product/9.0.1/network/jlib  
DISPLAY=192.9.200.24:0.0  
LD_LIBRARY_PATH=/Oracle/product/9.0.1/lib:/lib:/usr/lib:  
NLS_LANG=american_america.ZHS16GBK  
Oracle_HOME=/Oracle/product/9.0.1  
Oracle_SID=Oracle  
Oracle_TERM=xterm  
ORA_NLS33=/Oracle/product/9.0.1/ocommon/nls/admin/data  
OSTYPE=linux-gnu  
PATH=/usr/local/bin:/bin:/usr/bin:/usr/X11R6/bin:/usr/games:  
/bin:/usr/bin:/usr/sbin:/etc:/opt/bin:/usr/ccs/bin:/usr/openwin:  
/opt/local/bin:/opt/NSCPnav/bin:  
/Oracle/product/9.0.1/bin:/usr/local/samba/bin:/usr/ucb:  
TNS_ADMIN=/home/Oracle/config/9.0.1  
仔细检查一下以上的几项，确保都设置正确了。  
安装Oralce9i  
安装JDK1.3.1  
把下载的j2sdk-1.3.0-RC1-linux-i386.tar.bz2文件上传到服务器的/usr/local/目录下，以root用户登陆，用bzip –d j2sdk-1.3.0-RC1-linux-i386.tar.bz2命令先把文件解成tar格式，再使用tar xvf j2sdk-1.3.0-RC1-linux-i386.tar.bz2解压出来，为了便于操作可以把文件夹名改成jdk.。  
配置x-windows  
Oracle9i的安装几乎支持所有的x-windows，也支持远程的虚拟x－windows安装，如果你要在本机安装在控制台上以我们先前创建的Oracle用户登陆（注意要先设置好环境变量，并把DISPLAY的值设为空export DISPLAY=””）运行startx命令进入x-windows。  
如果需要在远程终端使用虚拟x-windows进行安装，需要在客户端先安装x-win32软件，x-win32的安装过程我们就不多介绍了，安装完成后运行x－win32命令在你的任务栏会出现一个x的标致。使用neterm等终端攻击以Oracle用户登陆系统确认环境变量都已经生效并且DISPLAY变量的值为你终端机的IP地址，运行startkde命令启动x-windows，运行完毕后系统会出现一大堆的出错信息，忽略不管，过了几秒后在你的远程终端上会出现Linux的kde界面。  
下载Oracle安装软件  
Oracle网站（http://otn.Oracle.com）现在提供Oracle9i for Linux软件下载，在下载前请仔细阅读他的Licence，这样在今后的使用中才不会有版权问题。在下载前你需要一个otn的账户，申请是免费的，只要简单回答几个问题就可以，Oracle9i的安装程序共有三个文件包分别是：  
Linux9i_Disk1.cpio.gz (412,092kb)  
Linux9i_Disk2.cpio.gz (638,547kb)  
Linux9i_Disk3.cpio.gz (82,956kb)  
下载完这三个文件后，把这三个文件上传到服务器/Oracle目录下，并保证这三个文件的属主是Oracle用户。如果你有Oracle9i的安装CD那就可以省下大把下载时间了。  
安装Oracle 9i 数据库  
以Oracle用户登陆系统，启动本地x-windows或虚拟x-windows，打开一个控制台窗口，进入到刚才存放Oracle文件的目录下，分别使用  
gunzip Linux9i_Disk1.cpio.gz  
cpio -idmv gunzip Linux9i_Disk2.cpio.gz  
cpio -idmv gunzip Linux9i_Disk3.cpio.gz  
cpio -idmv 命令解包，把三个文件包解压缩成三个安装文件夹分别为Disk1、Disk2、Disk3。  
进入Disk1目录  
cd Disk1  
在控制台窗口敲入  
./runInstaller &  
运行后会出现一个OUI的图形界面，如下图所示：  
中间绿色的窗口就是Oracle的安装图形界面了。  
下面我们来进行Oracle9i最基本的安装，在进入安装界面后点Next进入下一步：  
Source指的是包含Oracle产品信息的文件，一般情况下他会自动识别到，如果找不到可以用Browse按钮来手工指定路径。  
Destination指的是9i将要安装的路径这里就是我们在环境变量里设的$Oracle_HOME，如果这一栏里是空白的则要重新检查环境变量中各值的设定是否有误。确认正确后按Next进行下一步：
这一步有三个安装选项供选择：  
Oracle9i Database 9.0.1.0.0，安装Oracle9i的数据库服务器版本、管理工具、网络服务以及基本的客户端软件；  
Oracle9i Client 9.0.1.0.0 ，企业版的客户端软件，网络服务以及开发工具等。  
Oracle9i Mangement and Integration 9.0.1.0.0，安装Management Server，管理工具Oracle的网络目录、综合服务、网络服务以及基本的客户端软件。  
我们选第一项安装Oracle9i数据库服务器，接着安Next按钮；  
这一步是选择Oracle安装的类型，有三个类型供选择Enterprise Edition，企业版，Standstard Edition标致版，Custom自定义安装，我们选择企业版的安装，如果你对Oracle这一系列的产品比较熟悉的化可以选择Custom自定义安装，按自己的需求选择组件进行安装，确认后安Next进入到下一步；  
这里可以选择一种适合你的数据库模版，一般我们选第一种通用的数据库模版，如果你需要使用数据仓库，则可以使用选择数据仓库的模版进行安装。确认后按Next进入下一步；  
这一步是确认Oracle9i的SID和全局数据库的名字，SID的值我们在环境变量中已经设好了，所以这里就自动显示了，全局数据库名（Global Database Name）我们可以也指定成和SID的值相同，确认后按Next进入下一步；  
前面我们提到了，数据库的字符类型在数据库超作中是很关键的，这一步就是设置数据库的字符集，前面我们设置的是NLS_LANG=american_america.ZHS16GBK，所以我们选择Simplifiled Chinese ZHS16GBK，按Next进入下一步；  
因为我们在前面选择了Enterprise的版本进行安装，系统会安装Oracle Web Server，安装Oracle Web Server需要使用JDK，我们使用Browse按钮把前面安装JDK的目录指定好以便系统能在安装过程中找到需要的应用程序，确认按Next进入下一步；  
进行完所有选择后，系统会给出一个安装概要，这里列举了你选择安装的组件，确认你要安装的东西都在列表内后，安Install钮进行安装，如果不需要安装其它的程序，则按Exit退出安装界面。  
Oracle的安装速度视服务器的性能一般来说需要装30分钟的时间，在安装过程中可能会有对话框弹出，对话框内会有一些需要root运行的命令要求你执行，这时候另外开一个控制台窗口，su成root并运行提示框内的命令，运行完毕后按确定继续安装；  
安装完数据库后系统会运行配置工具对系统进行网络和数据库的配置。配置完成后，系统会自动启动数据库，并开启Oracle Web Server。所有配置完后，按Next完成安装。  
如果一切正常，OUI会出现The Installation Of Oracle9i Database Was successful.的字样，这表明你的Oracle9i数据库安装正常了，如果需要安装其它的内容按Next Install钮进行其它内容的安装，否则按Exit退出安装。  
使用Oracle 9i 数据库  
安装完毕后Oracle数据库会自动启动，下面我们用实际超作来说明一下Oracle 9i数据库的启动和关闭。  
以Oracle用户登陆数据库，开个控制台窗口；  
关闭Oracle 9i 数据库 [Oracle@wing /Oracle]$ sqlplus " / as sysdba" //以sysdba用户登陆数据库  
SQL*Plus: Release 9.0.1.0.0 - Production on Wed Jul 11 15:35:31 2001  
(c) Copyright 2001 Oracle Corporation. All rights reserved.  
Connected to:  
Oracle9i Enterprise Edition Release 9.0.1.0.0 - Production  
With the Partitioning option  
JServer Release 9.0.1.0.0 - Production  
运行shudown命令关闭数据库  
SQL> shutdown  
Database closed.  
Database dismounted.  
Oracle instance shut down.  
SQL>  
启动Oracle 9i 数据库 [Oracle@wing bin]$ sqlplus " / as sysdba"  
SQL*Plus: Release 9.0.1.0.0 - Production on Wed Jul 11 16:00:59 2001  
(c) Copyright 2001 Oracle Corporation. All rights reserved.  
Connected to an idle instance.  
SQL> startup  
Oracle instance started.  
Total System Global Area 336356520 bytes  
Fixed Size 279720 bytes  
Variable Size 268435456 bytes  
Database Buffers 67108864 bytes  
Redo Buffers 532480 bytes  
Database mounted.  
Database opened.  
SQL>  
启动Oracle 9i监听程序  
Oracle的监听程序主要是为客户端的连接提供接口 [Oracle@wing bin]$ lsnrctl  
LSNRCTL for Linux: Version 9.0.1.0.0 - Production on 11-JUL-2001 16:12:17  
Copyright (c) 1991, 2001, Oracle Corporation. All rights reserved.  
Welcome to LSNRCTL, type "help" for information.  
LSNRCTL> start  
Starting /Oracle/product/9.0.1/bin/tnslsnr: please wait...  
TNSLSNR for Linux: Version 9.0.1.0.0 - Production  
System parameter file is /Oracle/product/9.0.1/network/admin/listener.ora  
Log messages written to /Oracle/product/9.0.1/network/log/listener.log  
Listening on: (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC)))  
Listening on: (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=wing)(PORT=1521)))  
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=IPC)(KEY=EXTPROC)))  
STATUS of the LISTENER  
------------------------  
Alias LISTENER  
Version TNSLSNR for Linux: Version 9.0.1.0.0 - Production  
Start Date 11-JUL-2001 16:12:58  
Uptime 0 days 0 hr. 0 min. 0 sec  
Trace Level off  
Security OFF  
SNMP OFF  
Listener Parameter File /Oracle/product/9.0.1/network/admin/listener.ora  
Listener Log File /Oracle/product/9.0.1/network/log/listener.log  
Listening Endpoints Summary...  
(DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC)))  
(DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=wing)(PORT=1521)))  
Services Summary...  
Service "LSExtProc" has 1 instance(s).  
Instance "LSExtProc", status UNKNOWN, has 1 handler(s) for this service...  
Service "Oracle" has 1 instance(s).  
Instance "Oracle", status UNKNOWN, has 1 handler(s) for this service...  
The command completed successfully  
LSNRCTL>  
关闭Oracle 9i监听程序 [Oracle@wing bin]$ lsnrctl  
LSNRCTL for Linux: Version 9.0.1.0.0 - Production on 11-JUL-2001 16:12:17  
Copyright (c) 1991, 2001, Oracle Corporation. All rights reserved.  
Welcome to LSNRCTL, type "help" for information.  
LSNRCTL> stop  
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=IPC)(KEY=EXTPROC)))  
The command completed successfully  
LSNRCTL>  
关闭Oracle Web Server  
cd $Oracle_HOME/Apache/Apache/bin  
./stopJServ.sh  
/Oracle/product/9.0.1/Apache/Apache/bin/apachectl stop: httpd stopped  
启动Oracle Web Server  
cd $Oracle_HOME/Apache/Apache/bin [Oracle@wing bin]$ ./startJServ.sh  
/Oracle/product/9.0.1/Apache/Apache/bin/apachectl start: httpd started  
启动Oracle Web Server后默认的端口号是7777  
在客户端浏览器地址栏输入http://xxx.xx.xxx.xxx:7777/  
如果浏览器出现以下界面则表示Oracle Web Server运行正常
转自:[http://www.longtengwang.com/Article/shujuku/Oracle/200610/3225.html](http://www.longtengwang.com/Article/shujuku/Oracle/200610/3225.html)
