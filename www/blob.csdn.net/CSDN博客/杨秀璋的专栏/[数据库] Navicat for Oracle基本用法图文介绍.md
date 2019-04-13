
# [数据库] Navicat for Oracle基本用法图文介绍 - 杨秀璋的专栏 - CSDN博客

2016年04月06日 01:18:41[Eastmount](https://me.csdn.net/Eastmount)阅读数：22627标签：[oracle																](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)[Navicat																](https://so.csdn.net/so/search/s.do?q=Navicat&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[基础用法																](https://so.csdn.net/so/search/s.do?q=基础用法&t=blog)[图文介绍																](https://so.csdn.net/so/search/s.do?q=图文介绍&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=基础用法&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)个人分类：[数据库																](https://blog.csdn.net/Eastmount/article/category/6071839)
[
																					](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)所属专栏：[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)[
							](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=Navicat&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)

引自百度百科：Navicat for Oracle是一套超强的Oracle数据库系统管理工具。它有极具巧思的图形化用户界面(GUI)，让你可以快速且容易的以安全且简单的方法建立、组织、存取及共享资讯。Navicat
 for Oracle可以让用户连接本地/远端Oracle Server、提供一些实用的工具如数据/结构同步、导入/导出、备份及报表以协助管理数据的流程。
本文主要通过图文的方法介绍了Navicat for Oracle的基本用法，因为最近做J2EE项目涉及到远程数据库连接，故使用了该工具。文章非常简单基础，但还是希望对你有所帮助，如果文章中存在错误或不足之处，还请海涵~

## 一. 连接远程数据库
安装Navicat for  Oracle软件后运行如下图所示：
![](https://img-blog.csdn.net/20160405044539537)
点击”连接“按钮填写相关信息如IP地址，然后连接远程数据库如下图所示。
![](https://img-blog.csdn.net/20160405045207320)
连接成功后的数据库如下图所示：
![](https://img-blog.csdn.net/20160405050055324)
PS：因项目原因，该文章讲述的是连接远程数据库，但相关操作与本地的一样。

## 二. 创建数据库表
新建数据库表通过如下图点击任务栏的”新建表“或右击空白处创建。
![](https://img-blog.csdn.net/20160405132415425)

新建表T_USER_INFO，其中“添加栏位”向表中插入相应字段，目前我插入的字段包括：ID（序号）、USERNAME（用户名）、PWD（密码）、STATUS（权限）、DW_NAME（单位名称）、DW_CODE（单位代码），同时可以设置主键、非空属性、添加注释等操作。
同时，任务栏中还有索引、外键、触发器、权限供我们设置。这篇文章主要介绍一些基础知识，会基本使用即可，后面有机会给大家深层次讲解下这些用法。
![](https://img-blog.csdn.net/20160405133736696)

同时如果想修改已经创建的表的信息，可以通过如下图所示的方法进行操作。右击表T_USER_INFO“设计表”可以对表结构就行修改。
![](https://img-blog.csdn.net/20160405140824396)
通过点击“打开表”可以查看用户信息，如下图所示，同时也可以删除表、清空表数据、复制表等操作。
![](https://img-blog.csdn.net/20160405143955377)


## 三. 数据库增删改查
下面我将结合SQL语句和Navicat for Oracle软件对数据库的增删改查进行讲解。点击“查询=》新建查询”后进行操作，SQl支持的命令包括：
数据定义语言（DDL） : create, alter, drop
数据操纵语言（DML）: insert, delete, update, select
数据控制语言（DCL） : grant, revoke
事务控制语言（TCL）  : commit, savepoint, rollback
![](https://img-blog.csdn.net/20160405151546047)
**1.插入操作**
```python
insert into T_USER_INFO 
	(ID,USERNAME,PWD,STATUS,DW_NAME,DW_CODE)
values('1', 'Eastmount', '123123', '2', '教务处', '10001');
```
运行结果如下图所示，同时可以设置结果升序显示。
![](https://img-blog.csdn.net/20160405154218104)
![](https://img-blog.csdn.net/20160405154948451)

PS：Oracle设置序号ID递增方法比较麻烦，不像MySQL勾选AUTO_INCREMENT。
**2.更新操作**
```python
update T_USER_INFO set USERNAME='杨秀璋' and PWD='123456'
	and STATUS='2' and DW_NAME='软件学院' and DW_CODE='08111003'
where ID='0';
```
但是会报错：[Err] ORA-00933: SQL command not properly ended
Orcale语法与MySQL的区别，需要使用逗号替代and。
```python
update T_USER_INFO 
set 
	USERNAME='杨秀璋',
	PWD='123456',
	STATUS='2',
	DW_NAME='软件学院',
	DW_CODE='08111003'
where ID='0';
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20160405161124193)
![](https://img-blog.csdn.net/20160405161011443)

**3.查询操作**
```python
select * from T_USER_INFO where USERNAME='杨秀璋';
```
查询结果如下图所示：
![](https://img-blog.csdn.net/20160405161703914)
**4.删除操作**
```python
delete T_USER_INFO where ID='0';
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20160405161910150)
PS：在数据库操作中，现实中的网站项目会结合SQL语句和Java变量，此时之间空格、换行建议都删除，否则很容易更新自动添加空格等错误。如：
```python
String sql="delete T_USER_INFO where ID='"+ID+"'";
```


## 四. 数据库导入和导出
**1.导出数据**
![](https://img-blog.csdn.net/20160405162636731)

选择Excel2007文件如下：
![](https://img-blog.csdn.net/20160405162905903)
![](https://img-blog.csdn.net/20160405163116654)
依次点击下一步进行导出：
![](https://img-blog.csdn.net/20160405163843376)
最好选择“包含列的标题”，Excel中添加标题：
![](https://img-blog.csdn.net/20160405163908470)
![](https://img-blog.csdn.net/20160405163931595)
输出如下图所示：
![](https://img-blog.csdn.net/20160405164202737)


**2.导入数据**
导入数据的步骤如下图所示：
![](https://img-blog.csdn.net/20160406010617731)
![](https://img-blog.csdn.net/20160406010643309)
![](https://img-blog.csdn.net/20160406010703731)
![](https://img-blog.csdn.net/20160406010729216)
![](https://img-blog.csdn.net/20160406010750794)



## 五. 数据库自动备份

数据库备份如下图所示，保存为SQL文件：
![](https://img-blog.csdn.net/20160406011507734)
![](https://img-blog.csdn.net/20160406011535344)

同样可以对具体的某张表进行备份SQL，如下图所示：
![](https://img-blog.csdn.net/20160407220351143)
![](https://img-blog.csdn.net/20160407220426457)
![](https://img-blog.csdn.net/20160407220449676)
打开具体的T_USER_INFO.sql文件代码如下所示：
```python
/*
Navicat Oracle Data Transfer
Oracle Client Version : 11.2.0
Source Server         : 远程oracle连接
Source Server Version : 110200
Source Host           : XXX
Source Schema         : XXXXXX
Target Server Type    : ORACLE
Target Server Version : 110200
File Encoding         : 65001
Date: 2016-04-07 22:00:50
*/

-- ----------------------------
-- Table structure for "T_USER_INFO"
-- ----------------------------
DROP TABLE "T_USER_INFO";
  CREATE TABLE "GUI"."T_USER_INFO" 
   (	"ID" NUMBER(22,0) NOT NULL ENABLE, 
	"USERNAME" VARCHAR2(50) NOT NULL ENABLE, 
	"PWD" VARCHAR2(50) NOT NULL ENABLE, 
	"STATUS" NUMBER(22,0) NOT NULL ENABLE, 
	"DW_NAME" VARCHAR2(100), 
	"DW_CODE" VARCHAR2(50), 
	 PRIMARY KEY ("USERNAME")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 COMPUTE STATISTICS 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT FLASH_CACHE DEFAULT CELL_FLASH_CACHE DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) SEGMENT CREATION IMMEDIATE 
  PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT FLASH_CACHE DEFAULT CELL_FLASH_CACHE DEFAULT)
  TABLESPACE "USERS" 
 ;
-- ----------------------------
-- Records of T_USER_INFO
-- ----------------------------
INSERT INTO "T_USER_INFO" VALUES ('100', 'test01', '000000', '1', '经济学院', '3001');
INSERT INTO "T_USER_INFO" VALUES ('101', 'test02', '000000', '1', '金融学院', '3002');
INSERT INTO "T_USER_INFO" VALUES ('102', 'test03', '000000', '1', '工商学院', '3003');
INSERT INTO "T_USER_INFO" VALUES ('103', 'test04', '000000', '1', '会计学院', '3004');
INSERT INTO "T_USER_INFO" VALUES ('104', 'test05', '000000', '1', '公管学院', '3005');
INSERT INTO "T_USER_INFO" VALUES ('105', 'test06', '000000', '1', '管科学院', '3006');
```

最后希望文章对你有所帮助，如果文章中存在错误或不足之处，还请海涵~后面可能会写一些更有深度的Orcale数据库相关文章，尤其是如何提高数据库的性能，结合实际项目。
(By:Eastmount 2016-04-06 深夜1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


