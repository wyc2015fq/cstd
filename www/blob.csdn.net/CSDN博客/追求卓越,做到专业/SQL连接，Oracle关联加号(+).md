
# SQL连接，Oracle关联加号(+) - 追求卓越,做到专业 - CSDN博客


2016年01月25日 15:43:38[Waldenz](https://me.csdn.net/enter89)阅读数：5706


假设有
a表
id
1
2
3
4
b表
id
1
2
3
希望出来结果
a.id b.id
1 1
2 2
3 3
4
用left join 的
SQL codeselecta.id,b.id
   form aleftjion bona.id=b.id
用(+)的
SQL codeselecta.id,b.id
   form a ,bwherea.id=b.id(+)
用(+)的语法简洁，容易读懂，但只是ORACLE支持，LEFT JOIN是通用的，其它没有差别
**zengdan84：**
select * from a, b where a.id = b.id;
对于外连接，Oracle中可以使用“(+)”来表示，9i可以使用LEFT/RIGHT/FULL OUTER JOIN，下面将配合实例一一介绍。
1. LEFT OUTER JOIN：左外关联
SELECT e.last_name, e.department_id, d.department_name
FROM employees e
LEFT OUTER JOIN departments d
ON (e.department_id = d.department_id);
等价于
SELECT e.last_name, e.department_id, d.department_name
FROM employees e, departments d
WHERE e.department_id=d.department_id(+);

结果为：所有员工及对应部门的记录，包括没有对应部门编号department_id的员工记录。
2. RIGHT OUTER JOIN：右外关联
SELECT e.last_name, e.department_id, d.department_name
FROM employees e
RIGHT OUTER JOIN departments d
ON (e.department_id = d.department_id);
等价于
SELECT e.last_name, e.department_id, d.department_name
FROM employees e, departments d
WHERE e.department_id(+)=d.department_id;
结果为：所有员工及对应部门的记录，包括没有任何员工的部门记录。
3. FULL OUTER JOIN：全外关联
SELECT e.last_name, e.department_id, d.department_name
FROM employees e
FULL OUTER JOIN departments d
ON (e.department_id = d.department_id);
结果为：所有员工及对应部门的记录，包括没有对应部门编号department_id的员工记录和没有任何员工的部门记录。
**chensi05：**
说简单点就是哪边少加号放哪边
**wuchunyu002：**
外连接:
除了显示匹配相等连接条件的数据外,还可以显示某一个表中无法匹配相等连接条件的记录!
------------------------------------------------
1) 左条件(+) = 右条件
左条件所在的表必须严格进行相等连接条件的匹配,而右条件所在的表除了匹配相等连接条件外，还可以显示无法匹配连接条件的数据!
也称为右外连接.
--------------------------------
可以用下列语句取代:
SELECT...FROM 表1 RIGHT OUTER JOIN 表2 ON 条件;
2) 左条件 = 右条件(+)
右条件所在的表必须严格进行相等连接条件的匹配,而左条件所在的表除了匹配相等连接条件外，还可以显示无法匹配连接条件的数据!
也称为左外连接.
--------------------------------
可以用下列语句取代:
SELECT...FROM 表1 LEFT OUTER JOIN 表2 ON 条件;
REM 除了相等连接之外,显示没有员工的部门信息.
SELECT * FROM EMP,DEPT WHERE EMP.DEPTNO(+) = DEPT.DEPTNO;
SELECT * FROM EMP RIGHT OUTER JOIN DEPT ON EMP.DEPTNO = DEPT.DEPTNO;
REM 除了相等连接之外,显示没有部门的员工信息.
SELECT * FROM EMP,DEPT WHERE EMP.DEPTNO = DEPT.DEPTNO(+);
SELECT * FROM EMP LEFT OUTER JOIN DEPT ON EMP.DEPTNO = DEPT.DEPTNO;

shenliang1985:
Oracle 建议你用在From语句后用Outer Join语法 而不是Oracle的Join操作符(+).而且(+)是要受下面的规则限制的，但Outer Join语法则不受的～～
1）你不可以在查询块中使用(+) 当它同时包含 join的from语句中
2）(+)只是在where语句中,并且只能对应一个表或视图的一行字段
3）如果A和B做联接时有多个条件，那么(+)必须完善所有的匹配条件，
如果没有 ,oracle不会警告你～只是结果自然不同的
4）不可以在作出一个表外查询 另张表内查询的(+)联接操作～～
5）不可以用（+）外联接到自己 当然Self Join是可以的
6）含（+）的Where后的注意
OR不可用
IN不可用
子查询不可用
以下给些个例子：
SQL codeSQL>descpart
 NameNull?    Type----------------------------------------- -------- -----------------PART_IDNOTNULLVARCHAR2(**4**)
 SUPPLIER_IDVARCHAR2(**4**)
SQL>select*frompart;
PART SUPP---- ----P1   S1
P2   S2
P3
P4
SQL>descsupplier
 NameNull?    Type----------------------------------------- -------- -----------------SUPPLIER_IDNOTNULLVARCHAR2(**4**)
 SUPPLIER_NAMENOTNULLVARCHAR2(**20**)
SQL>select*fromsupplier;
SUPP SUPPLIER_NAME---- --------------------S1   Supplier\#**1**S2   Supplier\#**2**S3   Supplier\#**3**SQL>selectp.part_id, s.supplier_name**2**frompart p, supplier s**3**wherep.supplier_id=s.supplier_id (+);
PART SUPPLIER_NAME---- --------------------P1   Supplier\#**1**P2   Supplier\#**2**P3
P4--(+)是单向的SQL>selectp.part_id, s.supplier_name**2**frompart p, supplier s**3**wherep.supplier_id (+)=s.supplier_id (+);wherep.supplier_id (+)=s.supplier_id (+)*ERROR at line**3**:
ORA-**01468**: a predicate may referenceonlyoneouter-joinedtable--实现Full Join的方法SQL>selectp.part_id, s.supplier_name**2**frompart p, supplier s**3**wherep.supplier_id=s.supplier_id (+)**4**union**5**selectp.part_id, s.supplier_name**6**frompart p, supplier s**7**wherep.supplier_id (+)=s.supplier_id;
PART SUPPLIER_NAME---- --------------------P1   Supplier\#**1**P2   Supplier\#**2**P3
P4
     Supplier\#**3**--现在的语法SQL>selectp.part_id, s.supplier_name**2**frompart pfullouterjoinsupplier s**3**onp.supplier_id=s.supplier_id;
PART SUPPLIER_NAME---- --------------------P1   Supplier\#**1**P2   Supplier\#**2**P4
P3
     Supplier\#**3**另外的EG： 
SQL code
SQL>select*fromtesta;
        ID NAME  ADDR         AGE---------- ------ ------ ----------**3**电子   南京**23****5**物理   北京**23****6**物理   河北**25****1**电脑   上海**23****2**电子   北京**22****4**物理   芜湖**22****7**艺术   上海**21****8**歌剧   苏州**16****9**沈亮   上海**22**已选择9行。
SQL>select*fromtestb;
        ID ADDR---------- ----------**7**上海**1**北京**3**上海**5**合肥--匹配完全selecttesta.id,testa.addrfromtesta,testbwheretesta.id(+)=testb.idandtesta.addr(+)=testb.addr;
        ID ADDR---------- ------现在的Outer Join方法是Oracle9i时开始引用的 ANSI标准的联接语法，现在的则比较直观 简单 
通过上边Full Join的实现方式 不难看到ANSI的联接语法要比用含Union的(+)的执行效率更高

