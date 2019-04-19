# 利用DB Link实现Oracle两个数据库间的表同步 - 零度的博客专栏 - CSDN博客
2016年04月28日 13:57:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：4745
1，在目标机上建立Oracle DB Link：
A，在network/admin/tnsname.ora文件中加入源库的连接信息，如：
AAA=
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 10.5.1.3)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = AAA)
    )
  )
B，在目标机上用sqlplus user/pwd登录。
C，用如下命令建立DB Link:
create public database link AAA_LINK connect to user identified by pwd using 'AAA';
命令说明：
CREATE PUBLIC DATABASE LINK 数据库链接名 CONNECT TO 用户名 IDENTIFIED BY 密码 USING ‘本地配置的数据的实例名’;
如果建立成功，会提示：Database link created.
2，使用如下脚本，即可同步数据表：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)#!/bin/sh
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)# to sync table A to BBB database from AAA database
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)sqlplus user/pwd@BBB <<EOF    #这里是BBB上的数据库和密码还有实例名，请按照实际情况修改
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)truncate table A;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)insert into A 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)select * from b_schema.A@AAA_LINK; #这里是指向要同步的来源表,表名必须是<表所有者>.<表名>@<dblink name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)commit;
PS：需要DB支持Advanced replication功能，是否支持，可用如下SQL查看：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)select * from v$option where PARAMETER='Advanced replication';
如果是返回True就表示支持。
      物理上存放于网络的多个Oracle数据库，逻辑上可以看成一个单个的大数据库。 用户可以通过网络对异地数据库中的数据同时进行存取，而服务器之间的协同处理对于工作站用户及应用程序而言是完全透明的：开发人员无需关心网络的链接细节、无需关心数据在网络接点中的具体分布情况、也无需关心服务器之间的协调工作过程。
　　实现可分为四步。
　　１、在本地tnsnames.ora（该配置文件一般在"oracle主目录\ora90\network\ADMIN"中）里面配置一个服务命名，用于连接远程数据库。
toORADB10G =   //dblink的连接的服务名
(DESCRIPTION =
    (ADDRESS_LIST =
      (ADDRESS = 
　　 (PROTOCOL = TCP) //采用tcp/ip协议 
　　(HOST = 10.10.10.21) //欲链接主机名称或IP地址 
　　 (PORT = 1521))   //网络端口1521 
    )
    (CONNECT_DATA =
      　 (SERVICE_NAME = oradb10g) //安装ORACLE采用的sid，即远程数据库服务名
    )
)
　２、建立db_link（create public database link）
例：create public database link db21　
　　connect to jdc　　//用户
　　identified by angel　　//密码
　　using 'toORADB10G'
   ３、创建同义词（create sysnonym）
例：
create or replace synonym SYN_GRADING
for GRADING@db21
　４、使用（结合上同义词举例）。
　　１）、查询select * from SYN_GRADING, 与select * from [GRADING@db21](mailto:GRADING@db21)一样。查询的10.10.10.21主机上oradb10g数据库中的GRADING表
　　２）、增、删、改需要建立触发器。
create or replace trigger TRI_GRADING
after INSERT OR DELETE OR UPDATE
ON GRADING   　　//本地数据库表
FOR EACH ROW 
BEGIN
      IF inserting THEN
         insert into SYN_GRADING (ID, DEVICEID, GRADE, INTHEYEAR, GRADINGMAN, CARID, STATE, DEPTNAME, DEPTNO)
                values (:NEW.ID, :NEW.DEVICEID, :NEW.GRADE, :NEW.INTHEYEAR, :NEW.GRADINGMAN, :NEW.CARID, :NEW.STATE, :NEW.DEPTNAME, :NEW.DEPTNO);
      ELSIF updating THEN
        update SYN_GRADING set DEVICEID = :NEW.DEVICEID, GRADE = :NEW.GRADE, INTHEYEAR = :NEW.INTHEYEAR, GRADINGMAN = :NEW.GRADINGMAN, CARID = :NEW.CARID, STATE = :NEW.STATE, DEPTNAME = :NEW.DEPTNAME, DEPTNO = :NEW.DEPTNO 
           where id = :old.id;    
      ELSIF deleting THEN      
            delete from SYN_GRADING where id = :old.id;            
      END IF;
END TRI_GRADING;
这样，在往本地数据表GRADING中增、删、改数据时，10.10.10.21主机上oradb10g数据库中的GRADING表也将得到同步。
