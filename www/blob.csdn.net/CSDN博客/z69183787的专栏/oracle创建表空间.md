# oracle创建表空间 - z69183787的专栏 - CSDN博客
2014年02月10日 17:06:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1017
个人分类：[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)

注意点：
1.如果在PL/SQL 等工具里打开的话，直接修改下面的代码中[斜体加粗部分]执行
2.确保路径存在，比如【D:\oracle\oradata\Oracle9i\】也就是你要保存文件的路径存在
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
下面的代码和上面一样，可以直接复制。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/*分为四步 */
/*第1步：创建临时表空间  */
create temporary tablespace user_temp  
tempfile 'D:\oracle\oradata\Oracle9i\user_temp.dbf' 
size 50m  
autoextend on  
next 50m maxsize 20480m  
extent management local;  
 
/*第2步：创建数据表空间  */
create tablespace user_data  
logging  
datafile 'D:\oracle\oradata\Oracle9i\user_data.dbf' 
size 50m  
autoextend on  
next 50m maxsize 20480m  
extent management local;  
 
/*第3步：创建用户并指定表空间  */
create user username identified by password  
default tablespace user_data  
temporary tablespace user_temp;  
 
/*第4步：给用户授予权限  */
grant connect,resource,dba to username;
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
