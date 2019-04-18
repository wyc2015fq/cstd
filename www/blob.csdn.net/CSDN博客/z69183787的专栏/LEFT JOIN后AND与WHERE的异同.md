# LEFT JOIN后AND与WHERE的异同 - z69183787的专栏 - CSDN博客
2013年11月26日 14:54:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1683
**1、AND 过滤之后再连接**
**2、WHERE 连接之后再过滤**
下面以具体例子来说明：
**(1)、建表及插入测试数据**
[c-sharp][view plain](http://blog.csdn.net/nsj820/article/details/6001718#)[copy](http://blog.csdn.net/nsj820/article/details/6001718#)[print](http://blog.csdn.net/nsj820/article/details/6001718#)[?](http://blog.csdn.net/nsj820/article/details/6001718#)
- --建测试表  
- create table FACT_TAB  
- ( ID     INTEGER,  
-   STATUS VARCHAR2(8)  
- );  
- create table DIM_STATUS  
- ( STSTUS_CLASS VARCHAR2(8),  
-   STATUS_CODE  VARCHAR2(8),  
-   STATUS_DESC  VARCHAR2(8)  
- );  
- --插入测试数据  
- insert into FACT_TAB (ID, STATUS)values (1, '1');  
- insert into FACT_TAB (ID, STATUS)values (2, '1');  
- insert into FACT_TAB (ID, STATUS)values (3, '2');  
- insert into FACT_TAB (ID, STATUS)values (4, '1');  
- insert into FACT_TAB (ID, STATUS)values (5, '2');  
- insert into FACT_TAB (ID, STATUS)values (6, '3');  
- insert into FACT_TAB (ID, STATUS)values (7, '1');  
- insert into FACT_TAB (ID, STATUS)values (8, '2');  
- insert into FACT_TAB (ID, STATUS)values (9, '3');  
- insert into FACT_TAB (ID, STATUS)values (10, '3');  
- insert into FACT_TAB (ID, STATUS)values (11, '2');  
- insert into FACT_TAB (ID, STATUS)values (12, '1');  
- 
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('1', '1', '正常');  
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('1', '2', '注销');  
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('2', '1', '正常');  
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('2', '2', '注销');  
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('2', '3', '遗失');  
- insert into DIM_STATUS (STSTUS_CLASS, STATUS_CODE, STATUS_DESC)values ('2', '4', '未知');  
- 
- commit;  
**(2)、on后面and 条件表示先过滤之后，再连接**
以下两种写法，所得的结果相同：
[c-sharp][view plain](http://blog.csdn.net/nsj820/article/details/6001718#)[copy](http://blog.csdn.net/nsj820/article/details/6001718#)[print](http://blog.csdn.net/nsj820/article/details/6001718#)[?](http://blog.csdn.net/nsj820/article/details/6001718#)
- SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  
-   FROM FACT_TAB A  
-   LEFT JOIN DIM_STATUS B  
-     ON A.STATUS = B.STATUS_CODE  
-    AND B.STSTUS_CLASS = '2'
-    AND B.STATUS_CODE = '1'
-  ORDER BY A.ID;  
- 
- SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  
-   FROM FACT_TAB A  
-   LEFT JOIN (SELECT * FROM DIM_STATUS WHERE STATUS_CODE = '1') B  
-     ON A.STATUS = B.STATUS_CODE  
-    AND B.STSTUS_CLASS = '2'
- --AND B.STATUS_CODE = '1'
-  ORDER BY A.ID;  
|**ID**|**STATUS_CODE**|**STATUS_DESC**|
|----|----|----|
|1|1|正常|
|2|1|正常|
|3|-1||
|4|1|正常|
|5|-1||
|6|-1||
|7|1|正常|
|8|-1||
|9|-1||
|10|-1||
|11|-1||
|12|1|正常|
**(3)、on后面where条件表示先关联之后，再过滤**
[c-sharp][view plain](http://blog.csdn.net/nsj820/article/details/6001718#)[copy](http://blog.csdn.net/nsj820/article/details/6001718#)[print](http://blog.csdn.net/nsj820/article/details/6001718#)[?](http://blog.csdn.net/nsj820/article/details/6001718#)
- SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  
-   FROM FACT_TAB A  
-   LEFT JOIN DIM_STATUS B  
-     ON A.STATUS = B.STATUS_CODE  
-    AND B.STSTUS_CLASS = '2'
-  WHERE B.STATUS_CODE = '1'
-  ORDER BY A.ID;  
|**ID**|**STATUS_CODE**|**STATUS_DESC**|
|----|----|----|
|1|1|正常|
|2|1|正常|
|4|1|正常|
|7|1|正常|
|12|1|正常|
**(4)、全值的情况**
[c-sharp][view plain](http://blog.csdn.net/nsj820/article/details/6001718#)[copy](http://blog.csdn.net/nsj820/article/details/6001718#)[print](http://blog.csdn.net/nsj820/article/details/6001718#)[?](http://blog.csdn.net/nsj820/article/details/6001718#)
- --全值的情况  
- SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  
-   FROM FACT_TAB A  
-   LEFT JOIN DIM_STATUS B  
-     ON A.STATUS = B.STATUS_CODE  
-    AND B.STSTUS_CLASS = '2'
-  ORDER BY A.ID;  
|**ID**|**STATUS_CODE**|**STATUS_DESC**|
|----|----|----|
|1|1|正常|
|2|1|正常|
|3|2|注销|
|4|1|正常|
|5|2|注销|
|6|3|遗失|
|7|1|正常|
|8|2|注销|
|9|3|遗失|
|10|3|遗失|
|11|2|注销|
|12|1|正常|
**(5)、说明：**
**用到此类连接的情况，多为事实表为主表，维表为次表的代码关联的连接；JOIN后AND与WHERE的区别，其实就是主次表过滤与联接的先后问题，这一点能认识到，所有得出的结果，都不难理解了；另外，如果主表或次表的联接关键字，有多个重复记录，则联接的结果会催生出多条重复记录，这就要求联接的关键字根据需要须是事实上的主键。**
**附：上面讨论的是对辅表限制的情况，下面的是对事实表限制用on和where的情况**
**[c-sharp][view plain](http://blog.csdn.net/nsj820/article/details/6001718#)[copy](http://blog.csdn.net/nsj820/article/details/6001718#)[print](http://blog.csdn.net/nsj820/article/details/6001718#)[?](http://blog.csdn.net/nsj820/article/details/6001718#)**
- **1、对事实表(主表)用on  **
- **将在结果集中过滤不满足主表条件的辅表信息，但因为是左连接主表的所有记录都会显示出来。  **
- **SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  **
- **  FROM FACT_TAB A  **
- **  LEFT JOIN DIM_STATUS B  **
- **    ON A.STATUS = B.STATUS_CODE  **
- **   AND B.STSTUS_CLASS = '2'**
- **      --AND B.STATUS_CODE = '1'**
- **   AND A.ID IN ('1', '2', '3')  **
- ** ORDER BY A.ID;  **
- **2、对事实表(主表)用where  **
- **如果对主表的限制放在where里，则不满足主表条件的所有主表辅表信息记录将都不会体现在结果集中。  **
- **SELECT A.ID, NVL(B.STATUS_CODE, '-1') STATUS_CODE, B.STATUS_DESC  **
- **  FROM FACT_TAB A  **
- **  LEFT JOIN DIM_STATUS B  **
- **    ON A.STATUS = B.STATUS_CODE  **
- **   AND B.STSTUS_CLASS = '2'**
- **--AND B.STATUS_CODE = '1'**
- ** WHERE A.ID IN ('1', '2', '3')  **
- ** ORDER BY A.ID;  **
- **注：因为对主表用ON限制没什么意义，所以基本上用不到。  **
[](http://blog.csdn.net/nsj820/article/details/6001718#)
