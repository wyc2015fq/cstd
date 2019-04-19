# oracle 12c误删pdb数据文件导致整个数据库无法打开问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月26日 09:16:19[左直拳](https://me.csdn.net/leftfist)阅读数：920
同事误删了某个插件数据库（PDB）的数据文件，结果整个数据库，包括容器数据库（CDB），以及其他插件数据库都用不了了。
**1、用sys账号进入实例**，可见容器数据库处于mounted状态；插在上面的所有PDB当然也都是mounted。
`sqlplus sys/pwd@orcl12c as sysdba;`
**2、试图打开CDB**，提示有数据库文件未能加载。
```
SQL> alter database open;
alter database open
*
第 1 行出现错误:
ORA-01157: 无法标识/锁定数据文件 101 - 请参阅 DBWR 跟踪文件
ORA-01110: 数据文件 101:
'D:\APP\ADMINISTRATOR\ORADATA\ORCL12C\PDBDTJK\NMEMC.DBF'
```
又关闭又启动，来回折腾，都是如此：
```
SQL> shutdown immediate 
ORA-01109: 数据库未打开
已经卸载数据库。
ORACLE 例程已经关闭。
SQL> startup
ORACLE 例程已经启动。
Total System Global Area 2147483648 bytes
Fixed Size                  3834152 bytes
Variable Size             671092440 bytes
Database Buffers         1459617792 bytes
Redo Buffers               12939264 bytes
数据库装载完毕。
ORA-01157: 无法标识/锁定数据文件 101 - 请参阅 DBWR 跟踪文件
ORA-01110: 数据文件 101:
'D:\APP\ADMINISTRATOR\ORADATA\ORCL12C\PDBDTJK\NMEMC.DBF'
```
**3、**查阅网上资料，应该是**将此遗失文件标为脱机、删除。但提示找不到该文件！**
```bash
SQL> alter database datafile 101 offline;
alter database datafile 101 offline
*
第 1 行出现错误:
ORA-01516: 不存在的日志文件, 数据文件或临时文件 "101"
```
这就奇怪了，明明是你提示了该文件的，现在又说不知道这个文件。
**4、**估计是当前所在数据库是CDB，而这个文件属于PDB，所以应该**切换到相关PDB**，再进行文件脱机。
```
SQL> alter session set container=PDBDTJK;
会话已更改。
SQL> show pdbs;
    CON_ID CON_NAME                       OPEN MODE  RESTRICTED
---------- ------------------------------ ---------- ----------
        13 PDBDTJK                        MOUNTED
```
**5、脱机（offline,drop）**，成功！
```
SQL> alter database datafile 'D:\APP\ADMINISTRATOR\ORADATA\ORCL12C\PDBDTJK\NMEMC.DBF' offline;
alter database datafile 'D:\APP\ADMINISTRATOR\ORADATA\ORCL12C\PDBDTJK\NMEMC.DBF' offline
*
第 1 行出现错误:
ORA-01145: 除非启用了介质恢复, 否则不允许立即脱机
SQL> alter database datafile 'D:\APP\ADMINISTRATOR\ORADATA\ORCL12C\PDBDTJK\NMEMC.DBF' offline drop;
数据库已更改。
```
**6、切换回CDB，打开**，成功！
```bash
SQL> alter session set container=CDB$ROOT;
会话已更改。
SQL> alter database open;
数据库已更改。
```
哎，我的天呐！呐啊~~~
**7、总结**
1）一个PDB损坏，导致整个CDB以及其他PDB都不能用，乃至于无法恢复？不应该啊，这可是oracle啊，不可能这么弱智的
还真是。听说oracle 12c r1版本就是，PDB宕机，CDB也会陪着一起宕；到r2就好一点，只宕PDB。
2）本次恢复，从CDB切换到PDB是关键。网上给出的答案，都是11g或以前的，一个实例对应一个数据库，所以没有数据库切换的概念，文件说脱机就脱机了。
3）只要数据库实例能启动，不管CDB是否已打开，sys都能进去:
`sqlplus sys/pwd@servername as sysdba;`
而PDB则不行。在未打开的情况下，你用 conn user/pwd@servername 这种方式已经进不去了，但切换会话还可以：
```bash
SQL> alter session set container=PDBDTJK;
会话已更改。
```
这是孙悟空的一根救命毫毛。
4）以上问题的解决，现在看起来一步一步的，清晰明了，但这中间经过了多少煎熬和痛苦。昨天傍晚出现的问题，搞到21点，还没搞定。回家继续查资料到晚上快24点。早上七点醒来，才查了一下会话切换。早上是人最清醒的时候。
我都不清楚这个文件是怎么删除的。在继续使用的情况下，删除文件系统会提示被锁定，估计是停掉数据库再删除，叉啊，这不是找死吗。我都不敢想象，如果今天数据库没起来，会有一翻怎样的折腾。虽然是开发数据库，但好多项目在使用，各有各的PDB。
