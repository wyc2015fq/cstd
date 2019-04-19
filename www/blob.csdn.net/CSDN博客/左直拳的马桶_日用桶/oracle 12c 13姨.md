# oracle 12c 13姨 - 左直拳的马桶_日用桶 - CSDN博客
2016年03月19日 11:47:14[左直拳](https://me.csdn.net/leftfist)阅读数：2473
搞了一下oracle 12c。有些体会还是先记下来。
12c搞搞新意思，弄了个CDB（容器数据库，可不是商务中心CBD哟）和PDB（可插拔数据库）。PDB插在CDB里。
简单而言，CDB就是一个数据库实例，而PDB就是实例里一个个的数据库。这跟SQL SERVER是类似的。据说在oracle 11g或以前，一个数据库实例只能承载一个数据库（甚至一个数据库分布于不同的数据库实例中），然后不同的账户对应不同的表空间，在这里，表空间就类似SQL SERVER的数据库。
有位大虾说的好：“*其实大家如果对SQL SERVER比较熟悉的话，这种CDB与PDB是不是感觉和SQL SERVER的单实例多数据库架构是一回事呢。像PDB$SEED可以看成是master、msdb等系统数据库，PDBS可以看成用户创建的数据库。而可插拔的概念与SQL SERVER中的用户数据库的分离、附加其实就是那么一回事。看来ORACLE也“抄袭”了一把SQL SERVER的概念，只是改头换面的包装了一番。*”
![这里写图片描述](https://img-blog.csdn.net/20160319113354835)
以下是一些操作语句，仅作个人学习记录
**1、创建可插拔数据库**
可以通过界面来操作，命令行则如下：
```
--'pdbseed'是模板数据库 pdb$seed 所在路径最后一个文件夹的名称。猜测如果数据库文件存放在系统默认路径下，则可以用最后一个文件夹来作为参数。本来应该是 file_name_convert('模版库路径','新建库路径')
CREATE PLUGGABLE DATABASE HNHY12 ADMIN USER hydg IDENTIFIED BY hydg file_name_convert=('pdbseed','hnhy');
```
原本我开始是酱紫写的：
`CREATE PLUGGABLE DATABASE gdlhjd ADMIN USER coder IDENTIFIED BY coder STORAGE(MAXSIZE 2G MAX_SHARED_TEMP_SIZE 100M) DEFAULT TABLESPACE hydg DATAFILE 'E:\GDLHJD\gdlhjd.dbf' SIZE 25M AUTOEXTEND ON;`
这句代码会报错，因为还没有指定file_name_convert
file_name_convert是什么鬼？我找来找去，在网上寻寻觅觅，都找不到确切的答案。隐约得到的大意是，按照模板来生成PDB。这个倒好理解，SQL SERVER里就是这样的，有master、msdb等系统数据库。
file_name_convert=(‘模板路径’,’新PDB所在路径’) 
如果路径为oracle默认存放路径，可以简化为 
file_name_convert=(‘模板路径所在文件夹名称’,’新PDB所在文件夹名称’)
问题是，既然是系统模板，为啥还要用户指定？我们哪知道是哪个模板？或者模板文件存放在哪个路径？
可以运行以下语句,获得pdb$seed的文件路径
`select * from v$dbfile;`
细想，oracle的本意是，可以根据模板库来生成新的库，并且这个模版库不一定是系统默认那个。这点比SQL SERVER要灵活。但用途不大，且难度大。
**2、激活PDB**
```
SQL> col pdb_name for a20
SQL> select pdb_name,status from dba_pdbs;
SQL> alter pluggable database gdlhjd open;
SQL> select pdb_id,pdb_name,dbid,status,creation_scn from dba_pdbs;
SQL> alter pluggable database HNHY12 open;
```
有一个问题，就是这样的激活是临时性的，下次数据库重启的时候，PDB只是挂载（mounted）状态，仍然需要手动运行 开启语句。一劳永逸的做法是保存这个开启的状态：
```
col pdb_name for a20
select pdb_name,status from dba_pdbs;
show pdbs;
alter pluggable database PDBGZFBC open;--开启
show pdbs;
ALTER PLUGGABLE DATABASE PDBGZFBC SAVE STATE;--保存开启状态
```
**3、删除PDB**
```
alter pluggable database hnhy12 close;
drop pluggable database hnhy12 including datafiles;
```
但好像，物理文件并没删除。
**4、为默认用户指定表空间**
**请注意，以下语句必须切换到相应插拔数据库下：**
**请注意，以下语句必须切换到相应插拔数据库下：**
**请注意，以下语句必须切换到相应插拔数据库下：**
创建PDB语句中有指定管理（？）账户
`CREATE PLUGGABLE DATABASE HNHY12 ADMIN USER hydg IDENTIFIED BY hydg 。。。`
而我们通常都是创建用户的时候，同时指定表空间。那么创建PDB之时，这个ADMIN USER是否已经有表空间了呢？似乎并没有。
咋办？
建一个表空间，然后指定给它即可（记得先切换数据库。我们创建数据库的时候，应该是在容器数据库里；那么在这个新建的PDB里建表空间，要先切换到相应的PDB）：
```
create tablespace hydg datafile 'D:\HNHY\db\HNHY12C\hydg.ora' size 500m autoextend on;
ALTER USER hydg DEFAULT TABLESPACE hydg;
```
参考文章： 
[ORACLE 12C新特性——CDB与PDB](http://www.cnblogs.com/kerrycode/p/3386917.html)
