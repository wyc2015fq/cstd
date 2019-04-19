# oracle dblink-数据同步（使用trigger） - 零度的博客专栏 - CSDN博客
2016年07月12日 16:25:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：3769
       物理上存放于网络的多个Oracle数据库，逻辑上可以看成一个单个的大数据库。 用户可以通过网络对异地数据库中的数据同时进行存取，而服务器之间的协同处理对于工作站用户及应用程序而言是完全透明的：开发人员无需关心网络的链接细节、无需关心数据在网络接点中的具体分布情况、也无需关心服务器之间的协调工作过程。
　　实现可分为四步。
　　1、在本地tnsnames.ora（该配置文件一般在"oracle主目录\ora90\network\ADMIN"中）里面配置一个服务命名，用于连接远程数据库。
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
　  2、建立db_link（create public database link）
例：create public database link db21　
　　connect to jdc　　//用户
　　identified by angel　　//密码
　　using 'toORADB10G'
    3、创建同义词（create sysnonym）
         例：create or replace synonym SYN_GRADING for
[GRADING@db21](mailto:GRADING@db21)
    4、使用（结合上同义词举例）。
　　  1）查询select * from SYN_GRADING, 与select * from [GRADING@db21](mailto:GRADING@db21)一样。查询的10.10.10.21主机上oradb10g数据库中的GRADING表
　　  2）增、删、改需要建立触发器。
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
