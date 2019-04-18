# Oracle ASM发展与实践 - weixin_33985507的博客 - CSDN博客
2013年04月21日 23:35:43[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

****更多精彩内容尽在****[www.leonarding.com](http://www.leonarding.com/)****
****《****Oracle ASM发展与实践****》****
**1.****说说你个人理解的存储解决方案的发展趋势。**
Oracle 7&8 ：这个时期的数据库备份方案已经可以使用RMAN 命令，它起源于Oracle 版本8，7的时候还不具备这个功能。可以把数据存储在操作系统的文件系统上和裸设备上。一提到裸设备可能刚进入DBA行当的朋友们不是很熟悉，下面随便简单介绍一下吧。
裸设备：我们把这三个字分开说吧，设备就是指的物理磁盘，像老一点的IDE磁盘，现在常用的SCSI磁盘SAS磁盘等。裸指的是这个磁盘上没有文件系统，一般磁盘在插入到主板上后必须为其创建文件系统（windows ext2&3&4）并格式化才能使用，但oracle可以不通过文件系统直接访问一块裸设备（要先创建为物理卷）这种直接访问设备的方式给我们提供了一定的便捷（不需要操作系统层处理可直接与设备进行数据交换，提高了交换速度），但同时带来的弊端也是显而易见的，例如不好管理，必须每个文件单独创建一个裸设备，非常麻烦，复制和备份也不方便。其实裸设备还是没有真正的绕过操作系统，因为它需要通过由操作系统驱动的设备接口而访问设备。Linux中有一种dd命令可以分配和管理裸设备，还是没有文件系统方便。
Oracle 9i&10g：Oracle9i开始开发出一套自己的文件系统叫ASM，中文叫“自动存储管理系统”，可以通过ASMCMD工具来管理，它可以不受操作系统层参数的影响直接处理磁盘上的数据，效率要比操作系统层处理一遍的高。层次越多开销越大层次越少开销越小，流行于Oracle 10g
ASM是由AU（allocationunit）和条带来决定数据分布单元的，如何存储分配。
AU：磁盘分配单元，数据存储的一个基本单位，默认1M，建议大一点
例如AU=2M  单次I/O=1M，需要2次连续I/O才能读出一个AU
    AU=1M 单次I/O=2M，1次分散IO读出两个AU，但是连续的比分散的I/O效率高
条带宽度：默认8 ，建议条带宽带与磁盘数相匹配最好
条带大小：可以自由指定 1M 512k 256k
条带总大小=条带宽度*条带大小
小AU小条带场景：适合在大并发插入中表现较好，可有助于分散热点块，缺点连续性不够
大AU大条带场景：适合在非大并发插入的情况下都可以使用，适合绝大多数场景
Oracle 11g：出现了ASM和OCFS（Oracle Cluster File System）并存的局面，OCFS叫做“Oracle 集群文件系统”用的最多就是RAC模式下，11g中RAC模式下要不使用ASM管理存储要不使用OCFS管理存储，已经不可以使用裸设备当共享存储了，切记切记。
注意：可能有的同学会说用Ext3&4也可以实现2台机器共享一个存储设备啊（NFS就可以），嗯是的你很聪明问到了这个问题，但这是操作系统层面的贡献，不是存储层面的贡献，而且也无法提供Oracle要求的共享访问模式
ASM 三种redundancy冗余模式
（1）外部冗余模式
ASM不提供数据冗余，由外部存储系统实现，例如 RAID技术
这是当前使用最多的方式，因为有了磁盘级别冗余，那么可以使用更多的磁盘空间来存放数据
（2）常规冗余模式
ASM提供2份镜像，会损失一半空间用于备份镜像，是extent级别冗余不是磁盘级别冗余，主备混合存放
（3）高冗余模式
ASM提供3份镜像，会损失2/3空间用于备份镜像，是extent级别冗余不是磁盘级别冗余，主备混合存放
小结：第一种方法使用的最多，后两种方法使用的最少。
**2.****画一张说明****ASM****实例同数据库实例协同工作的原理图，并给出语言描述。**
直接使用word画图工具完成了一张简单的工作流程图，功力浅薄，望海涵
![152250303.jpg](http://blog.51cto.com/attachment/201306/152250303.jpg)
说明
（1）这张图画了2个Oracle实例，连接1个ASM实例，ASM实例挂载了3个磁盘组
（2）1个ASM实例可以为多个Oracle实例提供服务的，同时可以挂载多个磁盘组，例如Disk Group A 存放数据文件，Disk Group B 存放归档日志文件，Disk Group C 存放CRS文件和Voting disk文件。磁盘组类似于操作系统的逻辑卷，可以根据用途进行划分
（3）Oracle实例把数据先传到ASM实例上就不用管了，ASM再把数据按用途分配到各自的磁盘组上，并且是以打散的状态平均分散到每个磁盘上，目的就是分散热点块并行读取提高I/O效率
（4）css集群同步服务进程就是负责ASM实例与Oracle实例相互通信的
（5）启动ASM实例后再启动数据库实例，先关闭数据库实例后再关闭ASM实例
（6）我们可以方便的给ASM磁盘组添加一块磁盘，如果磁盘组损坏我们还可以删除一个磁盘组
（7）ASM数据分为“元数据”“真正数据”，元数据描述对象的字典信息，就是物理磁盘信息例如块大小 AU大小条带宽度数据分布情况平衡情况冗余情况等。并且在再平衡时“元数据”和“数据”都会移动
（8）ASM可以自动条带化，均衡负载，各个磁盘保持数据的均匀，可以游刃有余管理不同大小的磁盘
**3.****分别用视图方式和****asmcmd****方式，分别计算出你所用****ASM****管理的存储大小，使用空间和剩余空间数，写出计算过程。**
文件系统作为存储模式：我们可以使用dba_tablespace来查看表空间的使用情况
ASM作为存储模式：我们可以使用v$asm_diskgroup查看数据文件的使用情况，因为文件块在ASM上都是平衡打散状态，你可以设置为数据文件自动扩展，然后只需观察磁盘组的增长情况即可
通过v$asm_diskgroup动态性能视图我们可以查询ASM磁盘组的状态
SQL> selectgroup_number,name,state,block_size,allocation_unit_size,total_mb,free_mb fromv$asm_diskgroup;
GROUP_NUMBER  NAME  STATE    BLOCK_SIZE ALLOCATION_UNIT_SIZE   TOTAL_MB   FREE_MB
-------------------------------- -------------------- ---------- ----------
1               ARCH   MOUNTED 4096    1048576                10228      10174
2               CRS     MOUNTED  4096     1048576               5114      5055
3               DATA1   MOUNTED  4096    1048576               10228     10174
NAME：我们创建了三个磁盘组ARCH  CRS  DATA1
STATE：全部挂载
BLOCK_SIZE：ASM块大小默认为4k
ALLOCATION_UNIT_SIZE：磁盘分配单元默认为1M
TOTAL_MB：可用空间10G  5G  10G
FREE_MB：空闲空间10174  5055  10174
通过ASMCMD工具管理磁盘，因为ASM管理的是裸设备，在操作系统下是看不见的
ASMCMD> ls –lrt                             罗列已创建磁盘组
WARNING:option 'r'is deprecated for 'ls'
please use 'reverse'
State    Type   Rebal  Name           
MOUNTED  EXTERN N      DATA1/
MOUNTED  EXTERN N      CRS/
MOUNTED  EXTERN N      ARCH/
状态已挂载，采用外部冗余，磁盘组名
ASMCMD> pwd                              显示当前路径
+
ASMCMD> cddata1                          进入data1磁盘组
ASMCMD> pwd                              显示当前路径
+data1
ASMCMD> mkdirasm_leo1              在data1下创建一个asm_leo1目录
ASMCMD> ls
asm_leo1/
ASMCMD> rmasm_leo1                删除目录
ASMCMD> ls
ASMCMD> help                       显示帮助信息
        asmcmd [-vV] [-a <sysasm|sysdba>][-p] [command]
ASMCMD> lsct                        显示有多少实例连接到ASM上
ASMCMD> lsdg                       显示当前有多少个磁盘组及空间大小
State    Type   Rebal  Sector  Block      AU  Total_MB  Free_MB Req_mir_free_MB Usable_file_MB  Offline_disks  Voting_files Name
MOUNTED  EXTERN N         512   4096 1048576     10228    10174                0           10174              0             N ARCH/
MOUNTED  EXTERN N         512   4096 1048576      5114     5055                0            5055              0             N CRS/
MOUNTED  EXTERN N         512   4096 1048576     10228    10174                0           10174              0             N DATA1/
Sector：磁盘扇区大小512
Usable_file_MB：可用文件大小10174  5055  10174
Offline_disks：离线磁盘没有
Voting_files：表决文件
我们从v$asm_disk这个视图中可以查看到ASM磁盘属性信息
v$asm_disk_stat 查看磁盘状态信息
v$asm_diskgroup 查看磁盘组属性信息
v$asm_diskgroup_stat查看磁盘组状态信息
v$asm_client 查看磁盘组管理ASM实例的DB实例的数据行
v$_asm_file 查看磁盘组中数据文件属性信息
v$_asm_operation 查看磁盘组操作信息
v$_asm_alias 查看安装的磁盘组别名信息
SQL> descv$asm_disk
Name                                     Null?    Type
------------------------------------------------- ----------------------------
GROUP_NUMBER                                       NUMBER
DISK_NUMBER                                        NUMBER
COMPOUND_INDEX                                     NUMBER
INCARNATION                                        NUMBER
MOUNT_STATUS                                       VARCHAR2(7)
HEADER_STATUS                                     VARCHAR2(12)
MODE_STATUS                                       VARCHAR2(7)
STATE                                             VARCHAR2(8)
小结：在Oracle实例中我们可以利用select table_name from dictionary wheretable_name like '%ASM%';
数据字典来检索ASM相关的所有视图。
**4.****查询****asm****实例正常情况下的运行状态****(nomount,mount****或者****open)****，说一说为什么？****ASM****实例有控制文件吗？为什么****?**
ASM实例和DB实例的作用是一样的，也是由若干内存区和一组后台进程组成的，ASM实例启动后也是先分配内存区（asm_buffer_cache）加载后台进程。下面我们就来演示一下ASM实例启动过程
[grid@leonarding1bin]$ echo $ORACLE_SID               检查SID环境变量
+ASM
[grid@leonarding1bin]$ export ORACLE_SID=+ASM         如果没有设置我们可以这样设置一下
[grid@leonarding1bin]$ sqlplus / as sysasm
SQL*Plus: Release11.2.0.1.0 Production on Sun Apr 21 10:33:34 2013
Copyright (c)1982, 2009, Oracle.  All rights reserved.
Connected to:
Oracle Database11g Enterprise Edition Release 11.2.0.1.0 - 64bit Production
With the AutomaticStorage Management option
SQL> select *from v$version;
BANNER
--------------------------------------------------------------------------------
Oracle Database11g Enterprise Edition Release 11.2.0.1.0 - 64bit Production
PL/SQL Release11.2.0.1.0 - Production
CORE    11.2.0.1.0      Production
TNS for Linux:Version 11.2.0.1.0 - Production
NLSRTL Version11.2.0.1.0 - Production
SQL> selectinstance_name,host_name,startup_time,status,archiver,database_status,loginsfrom v$instance;
INSTANCE_NAME   HOST_NAME  STARTUP_TIME   STATUS   ARCHIVER  DATABASE_STATUS
LOGINS
--------------------------------------------------------------------------------------------------------------------------------------------
+ASM     leonarding1.oracle.com  19-APR-13     STARTED  STOPPED   ACTIVE
ALLOWED
实例名：ASM        这里可以判断我们连接的是Oracle实例还是ASM实例
主机名：leonarding1.oracle.com
ASM启动时间：4月19日13点启动
状态：nomount，分配内存区和加载后台进程
归档状态：没有开
数据库状态：激活
关闭ASM实例顺序：先关闭Oracle实例->再关闭ASM实例->先启动ASM实例->再启动Oracle实例
SQL> shutdownimmediate
ASM diskgroupsdismounted       卸载磁盘组
ASM instanceshutdown           关闭实例
[grid@leonarding1bin]$ ps -ef | grep asm | grep -v grep        后台进程已经没有了
[grid@leonarding1bin]$
[grid@leonarding1bin]$ sqlplus / as sysasm
SQL*Plus: Release11.2.0.1.0 Production on Sun Apr 21 11:11:31 2013
Copyright (c) 1982,2009, Oracle.  All rights reserved.
Connectedto an idle instance.      一个空实例，我们来启动它
SQL>
SQL> startup
ASM instancestarted                          实例启动，分配内存，磁盘挂载
Total SystemGlobal Area  283930624 bytes
Fixed Size              2212656 bytes
Variable Size             256552144 bytes
ASM Cache              25165824 bytes
ASM diskgroupsmounted
如果是Oracle实例那么后续肯定会有访问控制文件，打开数据文件的动作，而ASM实例启动没有这些动作，因为ASM只是管理磁盘组分配与平衡，不关注数据的一致性和数据库结构（也不存在实例一致性和检查点问题），逻辑上只做数据查询和抽取工作。所以ASM不需要控制文件和数据文件，只需分配内存区和启动后台进程ASM就可以工作了。
ASM需要参数文件
参数文件目录：/u02/app/11.2.0/grid/dbs
[grid@leonarding1bin]$ ps -ef | grep asm | grep -v grep
grid     14920    1  0 11:12 ?        00:00:00 asm_pmon_+ASM
grid     14924    1  0 11:12 ?        00:00:03 asm_vktm_+ASM
grid     14930    1  0 11:12 ?        00:00:00 asm_gen0_+ASM
grid     14934    1  0 11:12 ?        00:00:00 asm_diag_+ASM
grid     14938    1  0 11:12 ?        00:00:00 asm_psp0_+ASM
grid     14942    1  0 11:12 ?        00:00:00 asm_dia0_+ASM
grid     14946    1  0 11:12 ?        00:00:00 asm_mman_+ASM
grid     14950    1  0 11:12 ?        00:00:00 asm_dbw0_+ASM
grid     14954    1  0 11:12 ?        00:00:00 asm_lgwr_+ASM
grid     14958    1  0 11:12 ?        00:00:00 asm_ckpt_+ASM
grid     14962    1  0 11:12 ?        00:00:00 asm_smon_+ASM
grid     14966    1  0 11:12 ?        00:00:00 asm_rbal_+ASM
grid     14970    1  0 11:12 ?        00:00:00 asm_gmon_+ASM
grid     14974    1  0 11:12 ?        00:00:00 asm_mmon_+ASM
grid     14978    1  0 11:12 ?        00:00:00 asm_mmnl_+ASM
**5.****比较****ASM****实例和数据库实例在监听器中注册的状态，并说明原因。**
ASM监听
[grid@leonarding1dbs]$ lsnrctl status
LSNRCTL for Linux:Version 11.2.0.1.0 - Production on 21-APR-2013 11:40:01
Copyright (c)1991, 2009, Oracle.  All rights reserved.
Connecting to(DESCRIPTION=(ADDRESS=(PROTOCOL=IPC)(KEY=EXTPROC1522)))
STATUS of theLISTENER
------------------------
Alias                     LISTENER
Version                   TNSLSNR for Linux: Version11.2.0.1.0 - Production
Start Date                19-APR-2013 07:25:17
Uptime                    2 days 4 hr. 14 min. 43 sec
Trace Level               off
Security                  ON: Local OS Authentication
SNMP                      OFF
Listener ParameterFile  /u02/app/11.2.0/grid/network/admin/listener.ora
Listener LogFile         /u02/app/grid/diag/tnslsnr/leonarding1/listener/alert/log.xml
ListeningEndpoints Summary...
(DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1522)))
(DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=leonarding1)(PORT=1522)))
ServicesSummary...
Service"+ASM" has 1 instance(s).
  Instance "+ASM", status READY, has1 handler(s) for this service...
The commandcompleted successfully
这是ASM监听，ASM实例会自动注册到监听器中
Oracle监听
[oracle@leonarding1 ~]$ lsnrctl status
LSNRCTL for Linux: Version 11.2.0.1.0 - Production on 17-APR-2013 21:28:00
Copyright (c) 1991, 2009, Oracle.  All rights reserved.
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=IPC)(KEY=EXTPROC1521)))
STATUS of the LISTENER
------------------------
Alias                     LISTENER
Version                   TNSLSNR for Linux: Version 11.2.0.1.0 - Production
Start Date                14-APR-2013 18:38:00
Uptime                    3 days 2 hr. 50 min. 0 sec
Trace Level               off
Security                  ON: Local OS Authentication
SNMP                      OFF
Listener Parameter File   /u01/app/oracle/product/11.2.0.1/network/admin/listener.ora
Listener Log File         /u01/app/oracle/diag/tnslsnr/leonarding1/listener/alert/log.xml
Listening Endpoints Summary...
  (DESCRIPTION=(ADDRESS=(PROTOCOL=ipc)(KEY=EXTPROC1521)))
  (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=leonarding1)(PORT=1521)))
Services Summary...
Service "LEO1" has 1 instance(s).
  Instance "LEO1", status READY, has 1 handler(s) for this service...
Service "LEO1XDB" has 1 instance(s).
  Instance "LEO1", status READY, has 1 handler(s) for this service...
The command completed successfully 
Oracle监听也会自动注册到监听器中，它们分别使用不同的参数文件和日志文件
小结：一个监听器可以为多个实例服务（ASM实例和Oracle实例）
一个ASM实例也可以为多个数据库实例服务
一个CRS（集群注册服务）可以为几个数据库实例服务
**6.****看一下你的****ASM****的后台进程包含哪些，分别说说它们的用途。**
查看ASM进程
[grid@leonarding1bin]$ ps -ef | grep asm | grep -v grep         
grid      4039    1  0 19:43 ?        00:00:03 asm_pmon_+ASM
grid      4043    1  0 19:43 ?        00:01:13 asm_vktm_+ASM
grid      4049    1  0 19:43 ?        00:00:01 asm_gen0_+ASM
grid      4053    1  0 19:43 ?        00:00:01 asm_diag_+ASM
grid      4057    1  0 19:43 ?        00:00:01 asm_psp0_+ASM
grid      4061    1  0 19:43 ?        00:00:05 asm_dia0_+ASM
grid      4065    1  0 19:43 ?        00:00:01 asm_mman_+ASM
grid      4069    1  0 19:43 ?        00:00:02 asm_dbw0_+ASM
grid      4073    1  0 19:43 ?        00:00:01 asm_lgwr_+ASM
grid      4077    1  0 19:43 ?        00:00:02 asm_ckpt_+ASM
grid      4081    1  0 19:43 ?        00:00:00 asm_smon_+ASM
grid      4085    1  0 19:43 ?        00:00:03 asm_rbal_+ASM
grid      4089    1  0 19:43 ?        00:00:05 asm_gmon_+ASM
grid      4093    1  0 19:43 ?        00:00:02 asm_mmon_+ASM
grid      4097    1  0 19:43 ?        00:00:04 asm_mmnl_+ASM
解释：
PMON:进程监控进程，用于监控ASM实例进程，对异常进程进行报警保护
PSPn: 启动其他ASM实例进程，一旦有问题将导致ASM实例故障
RBLA：DISKGROUP做REBALANCE的后台进程，该进程一旦有故障，将导致ASM实例宕机
DBWn：与RDBMS的dbwriter类似，将ASM CACHE中脏数据写回磁盘
MMAN:与自动SGA管理有关，负责自动管理SGA。
SMON：系统监控进程，监控ASM实例的状态，一个ASM实例只有一个SMON进程
CKPT：检查点进程，触发ASM检查点写脏数据的
GMON：磁盘组监控进程，用于磁盘组状态监控和状态表维护。
DIAG：ASM诊断进程，用于诊断ASM告警
LGRW：写redolog进程，凡是块更改就会触发进程写redo信息
MMON：ASM缓冲区自动调整进程
VKTM：用于执行online磁盘的临时任务进程
ASM  磁盘组 ASMCMD  存储解决方案监视器后台进程
[Leonarding](http://f.dataguru.cn/..:;)
2013.4.21
天津&spring
分享技术~成就梦想
Blog：[www.leonarding.com](http://www.leonarding.com/)

