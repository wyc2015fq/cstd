# oracle创建表空间 - 零度的博客专栏 - CSDN博客
2016年04月28日 14:01:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：392
/*分为四步*/
/*第1步：创建临时表空间  */
create temporary tablespace user_temp  
tempfile 'D:\oracle\oradata\Oracle9i\user_temp.dbf' 
size 50m  
autoextend on  
next 50m maxsize 20480m  
extent management local;  
/*第2步：创建数据表空间  */
create tablespace user_data  
logging  
datafile 'D:\oracle\oradata\Oracle9i\user_data.dbf' 
size 50m  
autoextend on  
next 50m maxsize 20480m  
extent management local;  
/*第3步：创建用户并指定表空间  */
create user username identified by password  
default tablespace user_data  
temporary tablespace user_temp;  
/*第4步：给用户授予权限  */
grant connect,resource,dba to username;  
