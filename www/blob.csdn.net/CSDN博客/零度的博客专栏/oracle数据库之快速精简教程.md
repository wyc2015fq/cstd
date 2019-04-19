# oracle数据库之快速精简教程 - 零度的博客专栏 - CSDN博客
2016年01月05日 15:48:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：729标签：[数据库																[oracle																[实例																[存储过程 游标](https://so.csdn.net/so/search/s.do?q=存储过程 游标&t=blog)](https://so.csdn.net/so/search/s.do?q=实例&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[数据库](https://blog.csdn.net/zmx729618/article/category/3108441)
**Oracle 使用教程**
# 1、安装Oracle数据库
[http://blog.itpub.net/29519108/viewspace-1443824/](http://blog.itpub.net/29519108/viewspace-1443824/)
# 2、创建数据库实例
[http://blog.itpub.net/29519108/viewspace-1443918/](http://blog.itpub.net/29519108/viewspace-1443918/)
# 3、忘记密码
打开sqlplus
第一步：以系统身份登录：用户名：sys 
密  码 ：manager as sysdba
指令：sys/manager as sysdba
[](http://static.oschina.net/uploads/space/2014/0529/084722_Jdy4_1540055.png)
图1 以系统身份登录       
第二步：创建新用户：
语法：create user
用户名 identified by 密码;
指令：create user C##pdborcl identified by root; 
 (后面要加”;”)
第三步：为刚创建的用户解锁语法：
语法：alter user
用户名 account unlock; 
指令：alter user  C##pdborcl account unlock;
//用户解锁
指令：alter user C##pdborcl account lock;  
   //用户锁住
第四步：授予新登陆的用户创建权限语法：grant create session to  用户名 ;
指令：grant create session to C##pdborcl;
授予新创建的用户数据库管理员权限语法：grant dba to
用户名;
指令：grant dba to C##pdborcl;
为用户授予其它权限：
指令：GRANT CREATE USER,DROP USER,ALTER USER ,CREATE ANY VIEW
 , 
                       DROP ANYVIEW,EXP_FULL_DATABASE,IMP_FULL_DATABASE, DBA,
CONNECT,RESOURCE,CREATE SESSION TO C##user1;
切换到新创建的用户登陆语法：connect
用户名/密码;
指令：connect C##pdborcl/root;
删除用户语法：drop user
用户名;
指令：drop user C##pdborcl;
如果用户拥有数据表，则不能直接删除，要用上关键字cascade：
指令：drop user user01 cascade;
注意所有操作都必须在结尾加上“;”才能执行成功。
# 4、建立数据库
Oracle安装完后，其中有一个缺省的数据库实例，除了这个缺省的数据库实例外，我们还可以创建自己的数据库。对于初学者来说，为了避免麻烦，可以用'Database Configuration Assistant'向导来创建数据库实例。
创建完数据库实例后，并不能立即在数据库中建表，必须先创建该数据库的用户，并且为该用户指定表空间。下面是创建数据库用户的具体过程：
1.假如现在已经建好名为'orcl'的数据库实例，此时在F:\oracle\product\10.1.0\oradata\目录下已经存在orcl目录（注意：我的Oracle10g安装在F:\oracle下，若你的Oracle安装在别的目录，那么你新建的数据库目录就在*\product\10.1.0\oradata\目录下）。首先，使用system登陆。
——登录——
conn system/Root0729@orcl
2. 
在创建用户之前，先要创建表空间：
其格式为：格式: 
create tablespace 表间名 datafile '数据文件名' size 表空间大小;
如：
SQL> create tablespace news_tablespace datafile 'F:\oracle\product\10.1.0\oradata\news\news_data.dbf' size 500M;
其中'news_tablespace'是你自定义的表空间名称，可以任意取名；'F:\oracle\product\10.1.0\oradata\news\news_data.dbf'是数据文件的存放位置，'news_data.dbf'文件名也是任意取；'size 500M'是指定该数据文件的大小，也就是表空间的大小。
———创建临时表空间——
sql>create temporary tablespace test_temp 
tempfile 'D:\app\zhangwenchao\oradata\orcl\test_temp01.dbf'   
size 32m   
autoextend on   
next 32m maxsize 2048m   
extent management local;
———创建表空间——
sql>create tablespace KSXT   
logging   --有 NOLOGGING
和 LOGGING 两个选项,创建表空间时,创不创建重做日志
datafile 'D:\oracle\product\10.2.0\oradata\testserver\KSXT01.dbf' size 32m     
--DATAFILE 用于指定数据文件的具体位置和大小.如果有多个文件,可以用逗号隔开，但是每个文件都需要指明大小.必须为绝对地址,不能使用相对地址.
autoextend on   
next 32m maxsize 2048m   
extent management local
l  EXTENT MANAGEMENT LOCAL
存储区管理方法，本地管理(LOCAL): 
用二进制的方式管理磁盘,有很高的效率,同进能最大限度的使用磁盘. 同时能够自动跟踪记录临近空闲空间的情况，避免进行空闲区的合并操作。
l  UNIFORM SEGMENT SPACE MANAGEMENT --磁盘扩展管理方法:SEGMENT SPACE MANAGEMENT:
使用该选项时区大小由系统自动确定。由于 Oracle 可确定各区的最佳大小，所以区大小是可变的。UNIFORM SEGMENT SPACE MANAGEMENT:指定区大小，也可使用默认值 (1 MB)。
l  AUTO --
段空间的管理方式: AUTO: 只能使用在本地管理的表空间中. 使用LOCAL管理表空间时,数据块中的空闲空间增加或减少后，其新状态都会在位图中反映出来。位图使 Oracle 管理空闲空间的行为更加自动化，并为管理空闲空间提供了更好的性,但对含有LOB字段的表不能自动管理.
l  MANUAL:
目前已不用,主要是为向后兼容.
3.创建用户并指定表空间：
现在建好了名为'news_tablespace'的表空间，下面就可以创建用户了，其格式为：格式:  create user  用户名 identified by 密码  default tablespace 表空间表;
如：
SQL> create user news identified by news default tablespace news_tablespace;
默认表空间'default tablespace'使用上面创建的表空间。
4.接着授权给新建的用户：
SQL> grant connect,resource to news;  --表示把 connect,resource权限授予news用户
SQL> grant dba to news;  --表示把 dba权**限**授予给news用户
授权成功。
**5、CRUD操作**
**     略！**
**6、PL/SQL块**
------------------PL/SQL块------------------------------------
declare 
   v_name  varchar2(4) := 'asd';
   v_plot_date t_plot.createddate%type;
begin 
   dbms_output.put_line(v_name);
end;
---------- -------PL/SQL块-------------------------------------
declare 
  v_id    t_plot.id%type;
  v_name  t_plot.name%type;
begin 
   select id, name into v_id,v_name from t_plot where id=25973;  
    dbms_output.put_line(v_id ||'   '|| v_name);   
end;
-----------------------PL/SQL块----------if-else(选择)-----------
declare 
  v_name  varchar2(20) :='smith';
begin 
  if v_name='smith' then  
     dbms_output.put_line('这是'|| v_name||'先生'); 
  elsif   v_name='kate' then  
     dbms_output.put_line('这是'|| v_name||'先生');
  else
      dbms_output.put_line('这是XXX先生');
  end if;
end;
-------------------------PL/SQL块--------------循环-------
-------（1）loop语句--------1..5---------  
declare 
  v_count  number(4) :=0;
begin 
  loop
    dbms_output.put_line(v_count); 
    v_count :=v_count+1;
    exit when(v_count > 5);
  end loop;
end;
---------（2）for语句-------------------
declare 
begin 
   for i in [reverse] 0..5 loop
      dbms_output.put_line(i); 
   end loop;
end;
------------(3)while语句------------
declare 
   v_count number(4) :=0;
begin 
   while v_count<6 loop
      dbms_output.put_line(v_count); 
      v_count := v_count+1;
   end loop;
end;
**7、存储过程**
----------（1）有入参的过程---------------------------
create or replace procedure  insertplotproc(
 v_id  in   number,
 v_name in  varchar2,
 v_position in  varchar2 
)  
is
begin 
  --body体--
  insert into t_plot(id,name,position) values(v_id,v_name,v_position);
  commit;
end;
----------（2）有出参的过程------------
create or replace procedure  updateplotproc(
 v_id   in number,
 v_name in varchar2,
 v_position in  varchar2, 
 v_out   out varchar2      --输出参数
)  
is
  v_msg  varchar2(20) :='success';   --同样可以声明变量
begin 
  --body体--
  update  t_plot set name=v_name, position= v_position where id=v_id;
  commit;
  v_out :=v_msg;
end;
----------------包含异常的存储过程--------------------------
create or replace procedure  insertplotproc(
 v_id  in   number,
 v_name in  varchar2,
 v_position in  varchar2 
)  
is
begin 
  //body体
  insert into t_plot(id,name,position) values(v_id,v_name,v_position);
  commit;
exception
    when too_many_rows  then  dbms_output.put_line('too_many_rows异常');
    when NO_DATA_FOUND  then  dbms_output.put_line('NO_DATA_FOUND异常');
    when other then  dbms_output.put_line('other exception');
end;
**8、框架hibernate中调用存储过程**
    SessionFactory sf = SessionFactoyUtil.getSessionFactory();  
    Session session = sf.openSession();  
    SQLQuery query =   session.createSQLQuery("{CALL proc_find_stu(?,?)}").addEntity(StuInfo.class); 
    query.setLong(0, 2);    
    query.setLong(1, 4);    
    List<StuInfo> list =query.list();   
     for(int i =0;i<list.size();i++){  
         System.out.println(list.get(i));  
     }
**9、触发器**
-------------------语句级触发器--------------------------------
create or replace trigger plotupdate_trigger  --触发器名
after  --触发时间
insert or delete or update --触发事件
on t_plot   --表名 
begin
   dbms_output.put_line('执行plotupdate_trigger触发器了');  --body语句
end;
-------------------行级触发器----------------------------------
create or replace trigger plotupdate_row_trigger  --触发器名
befor  --触发时间
insert or delete or update --触发事件
on t_plot   --表名 
for each row  --行级触发器
begin
   dbms_output.put_line('执行plotupdate_trigger触发器了');  --body语句
end;
**10、Cursor游标使用**
declare 
    --声明变量-- 
    v_id t_plot.id%type;
    v_name t_plot.name%type;
    --声明游标--
    cursor cursor_plot is 
         select id, name from t_plot where id < 20000;
begin
  open cursor_plot; --打开游标
  loop
     fetch cursor_plot into v_id, v_name;
     dbms_output.put_line(v_id ||'  '|| v_name);
     exit when(cursor_plot%NOTFOUND);
  end loop;
  close cursor_plot; --关闭游标
end;
