# oracle 10g的备份和还原 - 左直拳的马桶_日用桶 - CSDN博客
2015年08月13日 19:21:29[左直拳](https://me.csdn.net/leftfist)阅读数：3199
采用 expdp备份，impdp还原。
注意这二者不等同于exp和imp。oracle 10g以前，可以采用exp、imp，10g及以后，expdp + impdp矣。据说10g里面，如果采用exp，没有记录的空表是导不出来的。
现在，想将源服务器A的某个数据库实例中的一些schema复制到目标服务器B。
有关oracle的schma、user、tablespace的关系，可以参照以下文章：
[ORACLE 中SCHEMA的概念以及数据库，表空间，数据文件等的区别(转)](http://langgufu.iteye.com/blog/1469055)
1、备份
## 在源服务器A下：
命令行方式下，登录sqlplus
> 
sqlplus sys/pwd@dbInstantName as sysdba
sqlplus下 
sql>create directory dbbak as ‘c:\bak’;
sql> grant read,write on directory dbbak to public;
退出sqlplus，回到命令行： 
Expdp  ‘sys/pwd@dbInstantName as sysdba’ directory=dbbak  dumpfile=bak.dmp  schemas=(sde,hydg) 
参数说明 
Expdp   导出数据泵  
system/orcl   连接参数 
directory=dbbak   存放导出文件的逻辑盘，指向刚才用户创建的文件夹 
dumpfile=bak.dmp  导出的DMP文件名  
schemas(sde,hydg)    导出的schema
***请注意，导出语句Expdp在DOS下执行，并且语句后面不能带上”;”，否则oracle会将它当成schemas的一部分！从而报“ORA-39165: 未找到方案”的错误！同理，下面的导入语句Impdp也一样！***
2、还原
## 在目标服务器下：
1）假定目标数据库还不存在相应的schema、表空间，那么创建之
```
--创建用户hydg及默认表空间、赋权限
create tablespace hydg datafile 'E:\chenqu\db\oracle\hydgts2\hydgts2\hydg.ora' size 500m autoextend on; 
create user hydg identified by hydg default tablespace hydg; 
grant connect, resource,dba to hydg;
--创建用户sde及默认表空间、赋权限
create tablespace sde datafile 'E:\chenqu\db\oracle\hydgts2\hydgts2\sde.ora' size 500m autoextend on; 
create user sde identified by sde default tablespace sde; 
grant connect, resource,dba to sde;
```
2)、impdp 
将导出的dmp文件拷贝到目标服务器，比如存放在e:\temp下。同样，也要创建一个逻辑文件夹，指向此dmp文件：
登录sqlplus 
![这里写图片描述](https://img-blog.csdn.net/20150813191432762)
注意主机字符串很别扭
SQL> create or replace directory db_bak as ‘e:\temp’;
目录已创建。
SQL> grant read,write on directory db_bak to public;
授权成功。
然后在DOS命令行方式下：
impdp ‘sys/pwd@hydgts as sysdba’ DIRECTORY=db_bak DUMPFILE=bak.dmp SCHEMAS=(sde,hydg)
（注意后面没有分号，否则报“ORA-39165: 未找到方案”的错误！参见上面导出部分）
然后我们就可以拿根甘蔗坐在电脑旁边啃边看结果了。
