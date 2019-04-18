# 修改clob字段并插入新表|转| - z69183787的专栏 - CSDN博客
2013年12月10日 11:25:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2230

链接：[http://www.xifenfei.com/2143.html](http://www.xifenfei.com/2143.html)
标题：[修改clob字段并插入新表](http://www.xifenfei.com/2143.html)
作者：[惜分飞](http://www.xifenfei.com/)©版权所有[文章允许转载,但必须以链接方式注明源地址,否则追究法律责任.]
最近开发有个需求，需要替换一张表的clob字段中的某些字符串，然后插入的一张新表中，我查询了一些资料，利用function结合匿名块模拟实现
**0、数据库版本**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
SQL> select * from v$version;
 
BANNER
-----------------------------------------------------------------------
 
Oracle Database 11g Enterprise Edition Release **11.2**.**0.1**.**0** - Production
PL/SQL Release **11.2**.**0.1**.**0** - Production
CORE    **11.2**.**0.1**.**0**      Production
TNS for **32**-bit Windows: Version **11.2**.**0.1**.**0** - Production
NLSRTL Version **11.2**.**0.1**.**0** - Production
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**1、创建替换clob中字符串函数**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
create or replace function replaceClob_new (
srcClob IN CLOB,
replaceStr IN VARCHAR2,
replaceWith IN VARCHAR2
--newClob OUT CLOB
) RETURN CLOB
IS
 
vBuffer    VARCHAR2 (**32767**);
l_amount   BINARY_INTEGER := **32767**;
l_pos      PLS_INTEGER := **1**;
l_clob_len PLS_INTEGER;
newClob  CLOB;
 
BEGIN
   newClob  := EMPTY_CLOB;
  -- initalize the new clob
 dbms_lob.createtemporary(newClob,TRUE);
 l_clob_len := dbms_lob.getlength(srcClob);
 WHILE l_pos < l_clob_len
  LOOP
    dbms_lob.read(srcClob, l_amount, l_pos, vBuffer);
    IF vBuffer IS NOT NULL THEN
      -- replace the text
      vBuffer := replace(vBuffer, replaceStr, replaceWith);
      -- write it to the new clob
      dbms_lob.writeappend(newClob, LENGTH(vBuffer), vBuffer);
    END IF;
    l_pos := l_pos + l_amount;
  END LOOP;
  RETURN newclob;
 
 EXCEPTION
  WHEN OTHERS THEN
    RAISE;
END;
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**2、创建原表，并测试该函数**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
SQL> CREATE TABLE t_clob(ID NUMBER,clob_xff CLOB);
 
Table created
 
--手工通过plsql dev插入一些数据（大于4000）
 
SQL> select dbms_lob.getlength(clob_xff) from t_clob;
 
DBMS_LOB.GETLENGTH(CLOB_XFF)
----------------------------
                        **4856**
 
SQL> select dbms_lob.getlength(replaceClob_new(clob_xff,'function','1')) from t_clob;
 
DBMS_LOB.GETLENGTH(REPLACECLOB
------------------------------
                          **4814**
 
SQL> set long **50**
SQL> select clob_xff from t_clob where id=**1**;
 
CLOB_XFF
--------------------------------------------------
create or replace function replaceClob_new (
srcCl
 
SQL> select replaceClob_new(clob_xff,'function','1')
  **2**  from t_clob where id=**1**;
 
REPLACECLOB_NEW(CLOB_XFF,'FUNCTION','1')
--------------------------------------------------
create or replace **1** replaceClob_new (
srcClob IN C
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**3、编写匿名块实现插入功能**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
DECLARE
  A_CLOB CLOB;
  R_CLOB CLOB;
  VSTRT  NUMBER(**4**);
BEGIN
--否则会在新表的clob字段头部会有空格
  VSTRT := **1**;
  SELECT REPLACECLOB_NEW(CLOB_XFF, 'function', 'xifenfei')
    INTO R_CLOB
    FROM T_CLOB
   WHERE ID = **1**;
  INSERT INTO T_CLOB_NEW
    (ID, CLOB_XFF)
  VALUES
    (**1**, EMPTY_CLOB())
  RETURNING CLOB_XFF INTO A_CLOB;
  DBMS_LOB.WRITE(A_CLOB, DBMS_LOB.GETLENGTH(R_CLOB), VSTRT, R_CLOB);
  COMMIT;
END;
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**4、测试匿名块功能**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
SQL> set long **50**
SQL> select clob_xff from t_clob where id=**1**;
 
CLOB_XFF
--------------------------------------------------
create or replace function replaceClob_new (
srcCl
 
SQL> select clob_xff from t_clob_new where id=**1**;
 
CLOB_XFF
--------------------------------------------------
create or replace xifenfei replaceClob_new (
srcCl
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
参考:http://space.itpub.net/111631/viewspace-605827
