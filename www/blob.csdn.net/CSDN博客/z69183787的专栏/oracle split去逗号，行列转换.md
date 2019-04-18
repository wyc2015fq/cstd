# oracle split去逗号，行列转换 - z69183787的专栏 - CSDN博客
2014年11月19日 15:36:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4064
用10G开始支持的正则表达式 
1.针对  '1','2','3','4','5'（逗号在字符串外面）
**[sql]**[view plain](http://blog.csdn.net/wanglilin/article/details/7231712#)[copy](http://blog.csdn.net/wanglilin/article/details/7231712#)[print](http://blog.csdn.net/wanglilin/article/details/7231712#)[?](http://blog.csdn.net/wanglilin/article/details/7231712#)
- SQL> SELECT COLUMN_VALUE  FROMTABLE(SYS.ODCIVARCHAR2LIST('1','2','3','4','5'));  
- 
- COLUMN_VALUE  
- --------------------------------------------------------------------------------
- 1  
- 2  
- 3  
- 4  
- 5  
2.针对'1,2,3,4,5'（逗号在字符串里面）
**[sql]**[view plain](http://blog.csdn.net/wanglilin/article/details/7231712#)[copy](http://blog.csdn.net/wanglilin/article/details/7231712#)[print](http://blog.csdn.net/wanglilin/article/details/7231712#)[?](http://blog.csdn.net/wanglilin/article/details/7231712#)
- SQL> select regexp_substr('1,2,3,4,5','[^,]+',1,rownum) from dual  
-   2  connectby rownum<=length('1,2,3,4,5')-length(replace('1,2,3,4,5',','))+1  
-   3  ;  
- 
- REGEXP_SUBSTR('1,2,3,4,5','[^,  
- ------------------------------
- 1  
- 2  
- 3  
- 4  
- 5  
3.使用函数
**[sql]**[view plain](http://blog.csdn.net/wanglilin/article/details/7231712#)[copy](http://blog.csdn.net/wanglilin/article/details/7231712#)[print](http://blog.csdn.net/wanglilin/article/details/7231712#)[?](http://blog.csdn.net/wanglilin/article/details/7231712#)
- CREATEORREPLACE TYPE ty_str_split ISTABLEOF VARCHAR2 (4000);  
**[sql]**[view plain](http://blog.csdn.net/wanglilin/article/details/7231712#)[copy](http://blog.csdn.net/wanglilin/article/details/7231712#)[print](http://blog.csdn.net/wanglilin/article/details/7231712#)[?](http://blog.csdn.net/wanglilin/article/details/7231712#)
- CREATEORREPLACEFUNCTION fn_split (p_str IN CLOB, p_delimiter IN VARCHAR2)  
- RETURN ty_str_split  
- IS
-   j INT := 0;  
-   i INT := 1;  
-   len INT := 0;  
-   len1 INT := 0;  
-   str VARCHAR2 (4000);  
-   str_split ty_str_split := ty_str_split ();  
- BEGIN
-   len := LENGTH (p_str);  
-   len1 := LENGTH (p_delimiter);  
- 
-   WHILE j < len  
-   LOOP  
-     j := INSTR (p_str, p_delimiter, i);  
- 
-     IF j = 0  
- THEN
-         j := len;  
-         str := SUBSTR (p_str, i);  
-         str_split.EXTEND;  
-         str_split (str_split.COUNT) := str;  
- 
-         IF i >= len  
- THEN
-           EXIT;  
- END IF;  
- ELSE
-         str := SUBSTR (p_str, i, j - i);  
-         i := j + len1;  
-         str_split.EXTEND;  
-         str_split (str_split.COUNT) := str;  
- END IF;  
- END LOOP;  
- 
- RETURN str_split;  
- END fn_split;  
测试：
**[sql]**[view plain](http://blog.csdn.net/wanglilin/article/details/7231712#)[copy](http://blog.csdn.net/wanglilin/article/details/7231712#)[print](http://blog.csdn.net/wanglilin/article/details/7231712#)[?](http://blog.csdn.net/wanglilin/article/details/7231712#)
- <p>SQL> select * fromtable(fn_split('1,2,3,4,5',','));    --第二个单引号中是前面字符串中需要被分隔的字符</p><p>COLUMN_VALUE
- --------------------------------------------------------------------------------
- 1  
- 2  
- 3  
- 4  
- 5</p><p>SQL> select * fromtable(fn_split('1,2,3,4。5','。'));</p><p>COLUMN_VALUE  
- --------------------------------------------------------------------------------
- 1,2,3,4  
- 5</p><p>SQL></p>  
参考：
[http://www.itpub.net/thread-1346178-1-1.html](http://www.itpub.net/thread-1346178-1-1.html)
众大牛们已经总结了行列转换的若干方法。今天发现了一种新的方法(![](http://www.itpub.net/static/image/smiley/qqbiaoqin/ka.gif) )，和大家分享下。
1.SYS.ODCIVARCHAR2LIST：
SELECT COLUMN_VALUE  FROM TABLE(SYS.ODCIVARCHAR2LIST('1','2','3','4','5'));
COLUMN_VALUE
--------------------------------------------------------------------------------
1
2
3
4
5
[Oracle](http://www.itpub.net/pubtree/?node=1) 10G 以上版本才支持SYS.ODCIVARCHAR2LIST，其实SYS.ODCIVARCHAR2LIST只不过是一个TYPE，
所以在9I版本中可以通过创建一个TYPE来使用该功能：
CREATE OR REPLACE TYPE MY_ODCIVARCHAR2LIST AS VARRAY(32767) OF VARCHAR2(4000);
SELECT COLUMN_VALUE  FROM TABLE(MY_ODCIVARCHAR2LIST('1','2','3','4','5'));
COLUMN_VALUE
--------------------------------------------------------------------------------
1
2
3
4
5
但是，当'1','2','3','4','5'  作为一个字符串('1,2,3,4,5')就没有办法转换了：
SELECT COLUMN_VALUE  FROM TABLE(MY_ODCIVARCHAR2LIST('1,2,3,4,5'));
COLUMN_VALUE
--------------------------------------------------------------------------------
1,2,3,4,5
总结：(1)Table函数将数组里的内容通过SQL语句查询出来；
      (2)ODCIVARCHAR2LIST 在9I 及以上版本中均可使用。在9I中可通过创建TYPE，10G及以上直接使用SYS.ODCIVARCHAR2LIST；
      (3)ODCIVARCHAR2LIST 适用于字符集，不适用单个字符串，如果是单个字符串，可以通过参考2中(如下)方法实现。
欢迎大家讨论，提出更多更好的方法~~
参考----------------------------------------------------------------
2.其他方法实现列转行（大牛们早已经总结，仅供参考）
(1) 利用CONNECT BY （使用9I，10G,11G）
WITH T AS (SELECT  '1,2,3,4,5' AS STR FROM DUAL)
SELECT  STR1  
FROM ( SELECT  DISTINCT 
                SUBSTR(T.CA,INSTR(T.CA, ',', 1, C.LV) + 1,
                       INSTR(T.CA, ',', 1, C.LV + 1) -(INSTR(T.CA, ',', 1, C.LV) + 1)) AS STR1
       FROM (SELECT ',' || STR || ',' AS CA,LENGTH(STR || ',') -NVL(LENGTH(REPLACE(STR, ',')), 0) AS CNT FROM T) T,
            (SELECT LEVEL LV FROM DUAL CONNECT BY LEVEL <= 9) C
       WHERE C.LV <= T.CNT 
       ORDER BY STR1);
(2).正则表达式（使用10G及以上版本）
WITH TEST AS (SELECT  '1,2,3,4,5' AS STR FROM DUAL)
SELECT DISTINCT REGEXP_SUBSTR(STR, '[^,]+', 1, LEVEL)
FROM TEST
CONNECT BY ROWNUM <= 5;
