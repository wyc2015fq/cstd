# oracle12c 移动数据库文件 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月14日 19:40:58[左直拳](https://me.csdn.net/leftfist)阅读数：165
今天oracle12c的实例中新建一个插拔数据库（PDB），结果发现oracle所在盘只剩400M。硬着头皮闯关，结果不出所料，警告说磁盘空间不足，无法生成SYSAUX01.DBF之类。
看了下盘里的内容，主要是里面已经放了好多个PDB，加起来有70多G。
后来发现可以移动数据库文件。记录如下：
```
SQL> conn sys/****@pdbdghd as sysdba;#进入要移动数据文件的数据库
已连接。
SQL> select file_id,file_name from dba_data_files;#看看都有啥数据文件
   FILE_ID
----------
FILE_NAME
--------------------------------------------------------------------------------
        71
D:\APP\ORACLE\ORADATA\LTHY\PDBDGHD\SYSTEM01.DBF
        72
D:\APP\ORACLE\ORADATA\LTHY\PDBDGHD\SYSAUX01.DBF
        73
D:\APP\ORACLE\ORADATA\LTHY\PDBDGHD\PDBDGHD_USERS01.DBF
   FILE_ID
----------
FILE_NAME
--------------------------------------------------------------------------------
        74
D:\APP\ORACLE\ORADATA\LTHY\PDBDGHD\SDE.DBF
        75
E:\APP\LTHY\PDBDGHD\HYDG.DBF
SQL> alter database move datafile 74 to 'E:\app\LTHY\PDBDGHD\sde.dbf';#来，移动一下。运行完毕以后，它真的是被移动了，好神奇哟！
数据库已更改。
```
注意这是oracle 12C的写法。如果是11g或之前的版本，又是offline，又是 alter，最后online，麻烦得很。最要命的是，我将这一套施行在12C上，结果报错，差点万劫不复。
