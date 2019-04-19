# ora-12541：TNS:无监听程序 - 左直拳的马桶_日用桶 - CSDN博客
2016年03月29日 19:39:30[左直拳](https://me.csdn.net/leftfist)阅读数：1548
今天在oracle 12c上创建了一个可插拔数据库，结果死活连不上，总报：ora-12541：TNS:无监听程序 的错误。
真是日了狗了，监听程序明明开启着，而我用 
```bash
sqlplus sys/pwd as sysdba
```
也能连上。后来发现，只要带上服务名，都连不上，比如：
`sqlplus sys/pwd@ORCL as sysdba`
经过网上苦苦寻觅，加上头脑偶尔灵光一闪，问题终于搞定： 
1、将listener.ora和tnsnames.ora里的本机IP地址改成本机计算机名 
2、重启监听服务
背景： 
我这台机是个虚拟机，也许是这个原因，安装oracle 12c的时候，它将监听的本机地址，全都写成了本机的IP:192.168.128.128：
```
LISTENER =
  (DESCRIPTION_LIST =
    (DESCRIPTION =
      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.128.128)(PORT = 1521))
      (ADDRESS = (PROTOCOL = IPC)(KEY = EXTPROC1521))
    )
  )
```
改成：
```
LISTENER =
  (DESCRIPTION_LIST =
    (DESCRIPTION =
      (ADDRESS = (PROTOCOL = TCP)(HOST = WIN-NH9JATPNTFA)(PORT = 1521))
      (ADDRESS = (PROTOCOL = IPC)(KEY = EXTPROC1521))
    )
  )
```
同理修改同目录下的tnsnames.ora。 
本机的sqlplus终于可以连了。
一波未平，一波又起。发现在其他机器上仍然无法连。
又是寻寻觅觅。原来是windows防火墙在作怪。关掉，可以鸟。
