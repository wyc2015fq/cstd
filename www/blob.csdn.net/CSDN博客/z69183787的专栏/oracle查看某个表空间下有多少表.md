# oracle查看某个表空间下有多少表 - z69183787的专栏 - CSDN博客
2014年02月27日 16:57:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2966
oracle查看某个表空间下有多少表
oracle查看某个表空间下有多少表
   www.2cto.com  
select * from all_tables where tablespace_name='TRAIN';
oracle 查看某个表空间下,某个用户有多少表
select * from all_tables where tablespace_name='TRAIN' and owner='TRAIN';
