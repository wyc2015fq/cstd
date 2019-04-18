# oracle数据库之间数据同步 DBLINK - z69183787的专栏 - CSDN博客
2014年03月10日 16:41:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3949
**之前想在同一个数据库中相同实例的不同用户也是用dblink同步数据，后来发现本机是不行的，DBLINK只适用于远程，**
**本机只需授予B用户读取A表的权限即可。grant select on c_project to stpt;**
这段时间负责某个项目开发的数据库管理工作，这个项目中开发库与测试数据库分离，其中某些系统表数据与基础资料数据经常需要进行同步，为方便完成指定数据表的同步操作，可以采用dblink与merge结合的方法完成，简单方便。
操作环境： 此数据库服务器ip为192.168.196.76,有center与branch两个库，一般需要将center的表数据同步到branch，center为源库，branch为目标库，具体步骤如下：
1.在源库创建到目标库的dblink
create database link branch     --输入所要创建dblink的名称，自定义 
  connect to dbuser identified by “password”  --设置连接远程数据库的用户名和密码 
  using '192.168.196.76/branch';  --指定目标数据库的连接方式，可用tns名称
![image](http://img1.51cto.com/attachment/201007/5/459544_1278298085aYGd.png) 在创建dblink时，要注意，有时候可能会报用户名和密码错误，但实际上我们所输入的账户信息是正确的，此时就注意将密码的大小写按服务器上所设置的输入，并在账号密码前号加上双引号(服务器版本不同造成的)。
2.成功后验证dblink
select * from tb_bd_action@branch;
![image](http://img1.51cto.com/attachment/201007/5/459544_1278298089CRFH.png)
正常情况下，如果创建dblink成功，可采用该方式访问到远程数据库的表.
3.通过merge语句完成表数据同步
此例中需要将center库中的tb_sys_sqlscripe表同步到branch，简单的语法如下:
merge into tb_sys_sqlscripe@branch b using tb_sys_sqlscripe c on (b.pk=c.pk)  --从center将表merge到branch,同步的依据是两个表的pk
when matched then  update set b.sqlscripe=c.sqlscripe,b.author=c.author  --如果pk值是相同则将指定表的值更新到目标表
when not matched then                                --如果pk值不一至，则将源表中的数据整条插入到目标表中
insert values (c.pk, c.fk, c.createtime, c.lastmodifytime,c.author,c.module,c.deleteflag, c.scripttype);
commit;               --记得merge后必须commit,否则更改未能提交
![image](http://img1.51cto.com/attachment/201007/5/459544_1278298089ho1t.png)
4.为方便每次需要同步时自动完成同步工作，可将该语句做成存储过程或脚本来定时执行或按要求手动执行,简单说一下创建脚本的方法:
a.创建merge文件夹
b.先将merge语句写完整后，存到merge.sql文件中
c.新建merge.bat文件，编辑后写入以下内容
  sqlplus user/password@serverip/database @"%cd%\merge.sql"
![image](http://img1.51cto.com/attachment/201007/5/459544_1278298090Scy2.png)
当运行merge.bat时就会自动完成所指定不同数据库间的同步工作
![image](http://img1.51cto.com/attachment/201007/5/459544_1278299155Yifz.png)
创建dblink：
```
create database link branch     --输入所要创建dblink的名称，自定义 
  connect to stptnew identified by "stptnew"  --设置连接远程数据库的用户名和密码 
  using '  (DESCRIPTION =
    (ADDRESS_LIST =
       (ADDRESS = (PROTOCOL = TCP)(HOST = 10.1.43.31)(PORT = 1521))
    )
    (CONNECT_DATA =
      (SID = stnic1)
    )
  )'  --指定目标数据库的连接方式，可用tns名称
```
示例
```
merge into t_contract_review_project t using c_project@branch c on (t.id=c.id) 
when matched then  update set 
t.project_name=c.project_name,t.project_identifier=c.PROJECT_NO,
t.project_num=c.DISPATCH_NO,t.project_company=c.PROJECT_COMPANY,
t.project_charge=c.PROJECT_ADDPERSON,t.project_charge_dept=c.PROJECT_ADDDEPT
when not matched then            
insert values (c.id,c.PROJECT_NAME,c.PROJECT_NO,c.DISPATCH_NO,c.PROJECT_COMPANY,c.PROJECT_ADDPERSON,'0',c.PROJECT_ADDDEPT);
commit;   
select c.id,c.PROJECT_NAME,c.PROJECT_NO,c.DISPATCH_NO,c.PROJECT_COMPANY,c.PROJECT_ADDPERSON,c.PROJECT_ADDDEPT from c_project@branch c
```
