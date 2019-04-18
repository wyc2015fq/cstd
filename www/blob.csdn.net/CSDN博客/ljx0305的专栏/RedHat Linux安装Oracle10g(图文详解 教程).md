# RedHat Linux安装Oracle10g(图文详解 教程) - ljx0305的专栏 - CSDN博客
2008年08月08日 11:09:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：11269标签：[oracle10g																[linux																[redhat																[oracle																[数据库																[linux内核](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=redhat&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle10g&t=blog)
个人分类：[linux下软件安装																[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)](https://blog.csdn.net/ljx0305/article/category/440323)
如需转载，请注明出处，[http://www.川江号子.cn](/)  。
另，本人有Word电子文档格式，如需要，请联系本人：[asima127@gmail.com](mailto:asima127@gmail.com)
**1安装RedHat Enterprise Linux 3**
**1.1准备安装介质**
安装介质一共4张光盘（CD版），版本号为：2.4.21-27.ELsmp
设置BIOS为光盘启动，放入第一张光盘即可。
进入到RedHat Enterprise Linux3（以下均简称为Linux）的启动界面，屏幕上出现[boot]字样，如果采用图形化方式安装，直接按回车继续，如果想采用命令行模式进行，输入“linux text”后回车，如有其他需要，按屏幕提示选择选项进行（比如安装SATA硬盘或网卡时可能需要先安装其驱动）。
**1.2安装过程**
1.欢迎界面，点击Next继续
2.选择安装语言界面，可以选择“Chinese(Simplified)简体中文”，点击Next继续
3.选择键盘界面，默认即可（U.S. English），点击“下一步”继续
4.选择鼠标界面，默认即可（3键鼠标（USB）），点击“下一步”继续
5.磁盘配置界面，选择“用Disk Druid手工分区”，点击“下一步”继续
6.设置分区：（以下为160G硬盘）
分区需注意：最多只能4个主分区，其中逻辑分区也是一个主分区，同时，还需要注意/tmp分区，建议系统有/tmp目录，因为很多软件在安装的时候都需要往此目录写文件，比如Oracle10g就要求/tmp目录有400M以上的空间。
![](http://photo1.bababian.com/upload6/20071218/923B2E38D6B405F2216A18F8CEB9252B.jpg)
设置好以后，点击“下一步”继续
7.设置引导装载程序配置
默认保留“Red Hat Enterprise Linux AS…”勾选框，点击“下一步”继续
8.防火墙设置，选择“无防火墙”，点击“下一步”继续
9.系统默认的语言设置，默认（Chinese (P.R. of China)），点击“下一步”继续
10. 选择时区，默认，点击“下一步”继续
11. 设置root用户密码，点击“下一步”继续
12. 软件包组设置，选择“定制要安装的软件包集合”，点击“下一步”继续
13. 选择要安装的软件包
特别注意：在“遗留网络服务器”中的细节中，勾选“telnet”服务
点击“下一步”继续
14. 确认界面，点击“下一步”继续
15. 等待安装
首先会根据第6步的设置进行磁盘分区和格式化
然后会根据第13步的设置进行软件安装，安装过程会提示换光盘进行，按提示进行
16. 安装完成
17. 设置图形化界面（X）配置，默认即可
18. 设置显示器配置，默认即可
19. 设置图形化配置，默认即可（如有需要，可以把登录类型改成“文本”）
20. 配置完成，退出重启就可以了
**2配置RedHat Enterprise 3**
**2.1安装网卡驱动**
有些机器的网卡可能在安装操作系统时就能自动安装好，因此安装好Linux系统以后，可以通过ifconfig来查看是否有eth0设备，如果只有lo设备，说明网卡驱动未成功。
![](http://photo1.bababian.com/upload6/20071218/DEA6423141691D07C405763FC51240DE.jpg)
安装网卡驱动，首先需要确定机器中的网卡是什么品牌什么型号，如果是品牌电脑，最好最快的方法是到官方网站下载机器型号对应的驱动程序，然后阅读其中的readme.txt进行安装配置。
如：联想开天 M6680型号的机器。
A.到官方网站下载网卡驱动程序NET_BROADCOM_57XX_952.exe（下载地址：[http://211.100.43.194/DriverFilesUploadFloder/business/NET_BROADCOM_57XX_952.exe](http://211.100.43.194/DriverFilesUploadFloder/business/NET_BROADCOM_57XX_952.exe)）
B.解压后，把其中Linux目录拷贝到U盘中
C.在Linux系统中，使用fdisk -l可以查看当前U盘盘符，如图，为/dev/sda1
![](http://photo1.bababian.com/upload6/20071218/2D4ACD170D73D9B1127A299D6E534DC8.jpg)
D.在/mnt下新建目录udisk，然后使用mount /dev/sda1 /mnt/udisk就可以使用U盘了，注意最好不要使用中文，否则在Linux下可能会出现乱码
![](http://photo1.bababian.com/upload6/20071218/E780F1A7DC50D20558F9DFE779F809A8.jpg)
E.拷贝Linux目录到/tmp下，可以看到如下文件
![](http://photo1.bababian.com/upload6/20071218/C2C356B12E64C29D24521AB1BB07FFF2.jpg)
F.参考readme.txt，进行网卡驱动安装
 tar xvzf tg3-3.58b.tar.gz  解压源代码tar包
 进入源代码目录后，使用make进行编译得到tg3.o（如果是REHL4版本，将得到tg3.ko）
![](http://photo1.bababian.com/upload6/20071218/ABCB1F89BB85F553F006F50B2723BD71.jpg)
G.使用insmod tg3.o命令把此文件插入到linux核心，再使用make install进行编译，使用uname –r可以看到Linux的版本号，使用insmod后，看到tg3.o被插入到/lib/modules/2.4.21-27.ELsmp/kernel/drivers/net中。
![](http://photo1.bababian.com/upload6/20071218/70E44D43335F2204D84D4CFAFF5D31D1.jpg)
H.rpm文件的编译
使用rpm -bb SPECS/tg3.spec或
rpmbuild -bb SPECS/tg3.spec (for RPM version 4.x.x)进行
![](http://photo1.bababian.com/upload6/20071218/A7F2AF8C6D060B7C1B63012AC162B166.jpg)
I.安装rpm文件
rpm -ivh RPMS/<arch>/tg3-<version>.<arch>.rpm
这个地方的<arch>为i386或i486等，在RPMS目录下可以看到，本例中为：
rpm -ivh RPMS/i386/tg3-3.58b-1.i386.rpm
J.装载tg3.o驱动程序
还可以使用 insmod tg3.ko (on 2.6.x kernels)，或modprobe tg3
![](http://photo1.bababian.com/upload6/20071218/74E590E4ABC5EAF0E5C6DDA8366C5ACD.jpg)
K.安装完毕，使用setup进行配置
使用root用户登录，然后输入setup命令，如下图：
选择“Network configuration”，然后使用“Tab”键将光标移到“运行工具”处，回车。
![](http://photo1.bababian.com/upload6/20071218/90BC81191472302CE2D6AA8E005EE0AD.jpg)
选择“是”，回车，如图：
![](http://photo1.bababian.com/upload6/20071218/3B3EA50BFBB19BF62EF28BDF912AED92.jpg)
配置IP地址信息后，确定，如图：
![](http://photo1.bababian.com/upload6/20071218/4B8B54D49F329A40E8C7DA7C0EB4F917.jpg)
然后运行 /etc/init.d/network restart，可以看到eth0已经被加载了
![](http://photo1.bababian.com/upload6/20071218/6241CAC6FF83B9B2C52EBE7B572D2913.jpg)
L.安装配置完毕。
**2.2配置服务**
尽管安装Linux的时候选择了telnet，但是服务默认是没有开启的，选择左下角的“红帽Logo”->“系统设置”->“服务器设置”->“服务”。
在图形界面中，左栏里找到“telnet”，可以看到，默认是不启动的，勾选后，点启动，并保存，这样，下次启动的时候，在5这个运行级，telnet服务会自动启动
其他服务还应根据需要打开：
vsftp：ftp服务
smb：Linux与window共享samba服务
同时，在此界面还可以设置其他级别默认启动的服务，在“工具栏”->“编辑运行级别”中进行选择。
注意关闭sendmail和sm-client服务，如果不需要这样的服务，在启动的时候会更快速，否则需要漫长的等待。
有些服务开启后系统资源开销比较大，建议只开启以下服务：
![](http://photo1.bababian.com/upload6/20071218/1B39DF01C971DB8599B4484D5716059E.jpg)
配置管理界面：
![](http://photo1.bababian.com/upload6/20071218/3DAF7D7134A1FCE646A3C4B92EBB00FF.jpg)
以下命令用于测试服务：
![](http://photo1.bababian.com/upload6/20071218/35876262D6CC437F77160551754191A7.jpg)
**2.3修改默认运行级别**
如果不希望Linux启动的时候就进入图形级别（图形级别为5），那么修改/etc/inittab，找到“id:5:initdefault:”，修改为“id:3:initdefault:”即可，3表示文本级别。
Linux一共7个级别，分别如下：
![](http://photo1.bababian.com/upload6/20071218/3A206A9F2BB4ADBF5E25E165C92837C4.jpg)
各个运行级的详细解释为：
![](http://photo1.bababian.com/upload6/20071218/560E263FD08170FEFC50CC432F7A6694.jpg)
运行命令“startx”可以启动到运行级5（即X window系统）
**2.4修改机器名字**
使用hostname 主机名，这样只能是临时修改当前的hostname,但是重新启动系统之后,就又会恢复回去，如果要永久修改，那么应该修改/etc/sysconfig/network里面的HOSTNAME值，如果没有就加上HOSTNAME=主机名，因为在/etc/rc.sysinit中我们可以看到系统启动的时候会从这个文件中去读取这个HOSTNAME值，然后再运行hostname 主机名，实际动作是修改/proc/sys/kernel/hostname文件，但因为是不可写的，所以使用hostname来设定，最后再修改或者添加 /etc/hosts里面的内容。
设置之后，可能再次telnet时会等待很长一段时间，是因为在/etc/resolv.conf中nameserver的值发生了变化，可以使用#进行屏蔽，也可以把 nameserver 的值修改为本机IP地址，这样，登录时间会缩短。
1.vi /etc/sysconfig/network
2.hostname name
3.vi /etc/hosts
**2.5设置全局格式**
使用root用户修改/etc/profile内容，在末尾添加如下几行：
![](http://photo1.bababian.com/upload6/20071218/2F9D96AC2EAADA169155316E5409DD14.jpg)
其中，TERM设置为xterm以后，vi就会有颜色区分。
保存后，以后登录，都将显示登录用户名，主机名和当前路径，如：
![](http://photo1.bababian.com/upload6/20071218/16D105DA8288A4EC46FC0BAA7C739B79.jpg)
/etc/profile文件可以根据需要设置一些环境变量，比如JAVA_HOME等，所有的用户都将生效，如果只是希望某些用户具备特定设置，可以修改$USER_HOME/.bash_profile文件进行设置。
**2.6安装补丁**
主要是如果有多CPU的机器，（略）
**3安装Oracle10g**
**3.1准备介质**
准备安装介质10201_database_linux32.zip
**3.2检查Liunx系统**
检查Linux系统是否包含安装Oracle10g需要的安装包，通过rpm –q –a|grep xxx进行检查，xxx为需要检查的包，列表如下：
RedHat Enterprise Linux 3.0 安装oracle10g需要的包
![](http://photo1.bababian.com/upload6/20071218/83D19E9E632D6B3E7F5D215B99347915.jpg)
RedHat Enterprise Linux 4.0 安装oracle10g需要的包
![](http://photo1.bababian.com/upload6/20071218/A04868D68F06D75782FD95404B0636ED.jpg)
系统硬件要求：
1.Oracle10g推荐使用512M内存
2.至少3.7GB的硬盘空间
3.至少1GB的swap space
4.至少400MB的/tmp目录
**3.3创建用户组和用户**
![](http://photo1.bababian.com/upload6/20071218/F3F5936C20C67CA16002D67E7E6E8351.jpg)
创建oracle用户时，指定其主目录为/bank/oracle，所属组为oinstall和dba，并使用passwd oracle为oracle用户设置密码，切换到oracle用户（su – oracle），mkdir inst_package，把oracle10g安装文件（10201_database_linux32.zip）拷贝到inst_package目录下（使用ftp或别的方式拷贝到linux系统中），再使用unzip解压：
$unzip 10201_database_linux32.zip      
解压后，得到database目录
**3.4创建Oracle目录**
![](http://photo1.bababian.com/upload6/20071218/45895F7B4C43F5027F83565BA6CA40B1.jpg)
在/bank/oracle下创建产品目录oracle10g和数据文件存放目录oradata，并设置对应的用户所属。
**3.5配置Linux内核参数**
使用root用户修改Linux内核参数，vi /etc/sysctl.conf文件，在末尾增加以下内容：
![](http://photo1.bababian.com/upload6/20071218/AB38203C89B57B6A96D5E7A449096D26.jpg)
配置好以后重新启动系统或使用命令/sbin/sysctl –p使其生效
其中shmmax推荐为物理内存的一半，计算方法：
1.grep MemTotal /proc/meminfo （得到2053996KB）
2.2053996 × 1024 ÷ 2 ＝ 1051645952
**3.6为oracle用户设置shell限制**
修改/etc/security/limits.conf文件，在该文件中添加以下内容：
![](http://photo1.bababian.com/upload6/20071218/5F72ED7946ACC3E363617646BE1B005F.jpg)
同时，修改/etc/profile，末尾添加以下内容：
![](http://photo1.bababian.com/upload6/20071218/6DFD8FFF4B7599FCAB07911C5860F797.jpg)
**3.7其他设置**
修改/etc/pam.d/login文件，在该文件中添加以下参数：
![](http://photo1.bababian.com/upload6/20071218/E49CF66759D6571BE03DA336279ABE15.jpg)
**3.8修改oracle用户的环境变量**
使用oracle用户修改/bank/oracle/.bash_profile，添加以下内容：
![](http://photo1.bababian.com/upload6/20071218/127B16A0E25161F133D875557F66EC37.jpg)
**3.9配置vncserver**
如果是在机器上直接安装，可以略过此步。
使用oracle用户登录，运行vncserver，第一次运行会要求输入登录vncserver的密码，在使用vnc客户端进行连接的时候要求输入，如图：
![](http://photo1.bababian.com/upload6/20071218/C3DE8678AB246DE2830CCB0DEE508A3B.jpg)
可以看到，启动了一个图形终端编号1，在此目录下，会创建一个隐藏的目录.vnc，进入此目录，编辑xstartup文件，可以看到默认是字符界面twm，删除所有内容，添加一行内容：gnome-session即可。
使用vncserver –kill :1关闭此图形终端，再重新运行vncserver
使用vnc客户端工具（RealVNC viewer），输入机器IP地址加图形终端号，如图：
![](http://photo1.bababian.com/upload6/20071218/B743408EBC66F344DBA54C7637D0D49E.jpg)
输入设置的vncserver密码：
![](http://photo1.bababian.com/upload6/20071218/19AF722DE9C1208530D7F03BF141EBDD.jpg)
会看到图形界面：
![](http://photo1.bababian.com/upload6/20071218/43B08BF2ADD0D0D5BFC1A0C08C6B0602.jpg)
**3.10安装Oracle10g**
A.使用oracle用户登录到图形界面（如远程安装，参考[3.9节）](http://blog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_%E9%85%8D%E7%BD%AEvncserver)
B.打开终端，![](http://photo1.bababian.com/upload6/20071218/38BD70B0E5BED77ECEE0904CDCF324B9.jpg) ，进入到oracle10g安装文件目录（本例为/bank/oracle/inst_package/database）
C.运行./runinstaller，可以看出，安装程序会先释放安装内容到/tmp目录下，因此要求/tmp下有足够的空间存放，当然，可以在oracle用户的.bash_profile中修改
![](http://photo1.bababian.com/upload6/20071218/0629A97361D6B931C5EC400283CEF35E.jpg)
D.Oracle的安装界面
可以看到相关配置会根据Oracle用户的.bash_profile中定义的环境变量默认出现，注意UNIX DBA组，应为oinstall。修改“全局数据库名SID”为：bkserver（根据自己的需要，可以任意取名），并输入SYS密码。点下一步继续。注意，根据需要是否勾选“创建启动数据库”选项，建议不勾选，后面再通过dbca来创建数据库，这里以勾选作为示例。
![](http://photo1.bababian.com/upload6/20071218/A14F09E65EB55B8BD289A3CB23B5A23B.jpg)
E.指定产品清单目录和身份证明（注意操作系统组名为oinstall）
如果以前在安装中途取消过，再到这一步会有个警告信息，提示目录已经存在，仔细检查后，可以点击“是（Y）”继续。
![](http://photo1.bababian.com/upload6/20071218/ACEC022181027F76404CE1615C395CF5.jpg)
![](http://photo1.bababian.com/upload6/20071218/50F19AC3506800BCD5D0C2BA49EEFD67.jpg)
F.此步可能会出错，根据页面内容检查配置Linux内核参数（[见3.5节）](http://blog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_%E9%85%8D%E7%BD%AELinux%E5%86%85%E6%A0%B8%E5%8F%82%E6%95%B0)
![](http://photo1.bababian.com/upload6/20071218/0FB6ACDA7EF5B1DA4A5AEC7C1E37F3BB.jpg)
如果成功会是如下界面，点击下一步继续
![](http://photo1.bababian.com/upload6/20071218/2E2F0570CFEA1A918AC106602466D008.jpg)
G.安装概要，可以简单浏览一下，点“安装”继续
![](http://photo1.bababian.com/upload6/20071218/19EBF58C60D1FD569DB13E55AB92A7B5.jpg)
H.等待安装过程
![](http://photo1.bababian.com/upload6/20071218/70AB764294ECFCE4D840DBB09D088BD9.jpg)
I.安装完成后，由于第一步勾选了“创建启动数据库”，因此，安装程序会在安装完成后自动启动数据库创建程序。
![](http://photo1.bababian.com/upload6/20071218/1CE4C98369F7330C623AE523A9CBE40C.jpg)
J.等待数据库创建过程
![](http://photo1.bababian.com/upload6/20071218/C903284F6A7623BF6F66331576B52D54.jpg)
K.创建数据库完成，可以点击“口令管理”查看内容，点击“确定”继续
![](http://photo1.bababian.com/upload6/20071218/8027AEEC718FEEC7273F47210052D3FA.jpg)
L.执行配置脚本，根据界面提示，使用root用户登录执行即可。
![](http://photo1.bababian.com/upload6/20071218/FECBBAFEFA182A6B50B79C60CD60E87F.jpg)
![](http://photo1.bababian.com/upload6/20071218/F314A958E9E578C0AA32249CB3560388.jpg)
M.安装结束，可以看出Enterprise Manager的管理URL地址
![](http://photo1.bababian.com/upload6/20071218/AF3283A9B6585616A571708D39AA7B91.jpg)
N.退出即可
**3.11创建数据库**
如果在上面第一步没有勾选“创建启动数据库”，那么可以通过此步进行数据库创建
A.使用oracle用户，在图形界面下（使用vncserver登录也可）运行dbca命令
![](http://photo1.bababian.com/upload6/20071218/B1483A187C4FC6C2796739FC1DB4C013.jpg)
B.选择操作“创建数据库”。（注意：不建议同一台机器上创建多个数据库instance）
![](http://photo1.bababian.com/upload6/20071218/F25B477347897FB135E4C56F6BD24F36.jpg)
C.选择数据库模板，选择“一般用途”就可以了。
![](http://photo1.bababian.com/upload6/20071218/7CBEABDD6437F238D5E48C9F473CA8D7.jpg)
D.指定SID
![](http://photo1.bababian.com/upload6/20071218/578D3CB460102BEBFA9FAB2022858500.jpg)
E.配置是否可以使用Enterprise Manager配置数据库
![](http://photo1.bababian.com/upload6/20071218/8CBBBE6A356DD6575BB84916BC7BADEE.jpg)
F.配置数据库管理员密码，根据需要选择并指定密码
![](http://photo1.bababian.com/upload6/20071218/B1616687860442BC1E7AD8CB24EE08B8.jpg)
G.指定数据存放方式，选择“文件系统”
![](http://photo1.bababian.com/upload6/20071218/32945D55B7780CCE3783E87BD41EE278.jpg)
H.指定数据文件存放位置，选择“所有数据库文件使用公共位置”，并通过浏览指定“/bank/oracle/oradata”，此目录在[3.4节已创建，当然，也可根据实际需要指定。](http://blog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_%E5%88%9B%E5%BB%BAOracle%E7%9B%AE%E5%BD%95)
![](http://photo1.bababian.com/upload6/20071218/5ED45AEC13CA9503D2A46BC0D7A5AAAA.jpg)
I.指定恢复配置，默认即可。
![](http://photo1.bababian.com/upload6/20071218/EAD8C0A09A1918C9286D382530CB5C64.jpg)
J.数据库内容，默认即可。
![](http://photo1.bababian.com/upload6/20071218/76CD306D9B9B8B849D8CD81368F52375.jpg)
K.初始化参数，指定字符集为“ZHS16GBK – GBK16位简体中文”。
![](http://photo1.bababian.com/upload6/20071218/B737CA496BF567FD148AAA3E701198AE.jpg)
L.存储信息，默认即可。
![](http://photo1.bababian.com/upload6/20071218/7B206771DEEABD437583D8091DD99DB7.jpg)
![](http://photo1.bababian.com/upload6/20071218/3431147F18622C452866A767FFC25095.jpg)
M.创建选项，默认即可，点击“完成”
![](http://photo1.bababian.com/upload6/20071218/E4E038483B68E1D6AAC3EAD87E153BE7.jpg)
N.确认界面，点击“确定”
![](http://photo1.bababian.com/upload6/20071218/62B9C93247C681B29C6FDB24762DE9FA.jpg)
O.等待创建数据库
![](http://photo1.bababian.com/upload6/20071218/A02839244616CF4EF5EF22481589153C.jpg)
P.安装结束，可以看到数据库的管理URL地址
![](http://photo1.bababian.com/upload6/20071218/31C06B5657121EF3F8389352ABEF1BAC.jpg)
Q.点击“退出”，安装完毕
R.使用oracle10g提供的web管理平台登录测试，在IE中输入“[http://ip”即可（ip为安装oracle10g的机器的ip地址）。:1158/em](http://ip:1158/em)
![](http://photo1.bababian.com/upload6/20071218/8B735F3945EEC151C4823D9A361290CD.jpg)
![](http://photo1.bababian.com/upload6/20071218/DC4141C0307DDC89E5E0374B7041441A.jpg)
**4配置Oracle10g**
**4.1启动加载监听程序**
在$ORACLE_HOME/bin/下，修改dbstart文件，在末尾添加lsnrctl start即可。
**4.2启动关闭shell程序**
启动shell：可以命名为startdb，将其拷贝到$ORACLE_HOME/bin/下，注意+x可执行。
![](http://photo1.bababian.com/upload6/20071218/9C144B5FB3BC5BEFD6A0AE5BF4CEB920.jpg)
关闭shell：可以命名为stopdb，将其拷贝到$ORACLE_HOME/bin/下，注意+x可执行。
![](http://photo1.bababian.com/upload6/20071218/D0C114F0658E0861505508B1DE763898.jpg)
**4.3配置Oracle自动启动**
![](http://photo1.bababian.com/upload6/20071218/C668892711D4C294DA7CB43E7D6E3361.jpg)
**4.4权限设置**
修改TNS相关的文件权限：>bX-!<S（是否必须还有待进一步研究）
![](http://photo1.bababian.com/upload6/20071218/7F75C8978A44F7A9D14EDE8E7B20344A.jpg)
引用:http://blog.csdn.net/yuanfen127/archive/2007/12/18/1944525.aspx
