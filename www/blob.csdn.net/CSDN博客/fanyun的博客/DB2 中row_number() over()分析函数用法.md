# DB2 中row_number() over()分析函数用法 - fanyun的博客 - CSDN博客
2016年11月30日 18:18:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3391
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
row_number() over()分析函数用法
row_number()over(partition by col1 order by col2)表示根据col1分组，在分组内部根据col2排序，而此函数计算的值就表示每组内部排序后的顺序编号（组内连续的唯一的）。 
与rownum的区别在于：使用rownum进行排序的时候是先对结果集加入伪劣rownum然后再进行排序，而此函数在包含排序从句后是先排序再计算行号码。 
row_number()和rownum差不多，功能更强一点（可以在各个分组内从1开始排序）。 
rank()是跳跃排序，有两个第二名时接下来就是第四名（同样是在各个分组内）。
dense_rank()也是连续排序，有两个第二名时仍然跟着第三名。相比之下row_number是没有重复值的。 
1.row_number() over (order by col_1[,col_2 ...]) 
 作用:按照col_1[,col_2 ...]排序,返回排序后的结果集， 
 此用法有点像rownum,为每一行返回一个不相同的值： 
 select rownum,ename,job,    
       row_number() over (order by rownum) row_number    
from emp;    
    ROWNUM ENAME      JOB       ROW_NUMBER    
---------- ---------- --------- ----------    
         1 SMITH      CLERK              1    
         2 ALLEN      SALESMAN           2    
         3 WARD       SALESMAN           3    
         4 JONES      MANAGER            4    
         5 MARTIN     SALESMAN           5    
         6 BLAKE      MANAGER            6    
         7 CLARK      MANAGER            7    
         8 SCOTT      ANALYST            8    
         9 KING       PRESIDENT          9    
        10 TURNER     SALESMAN          10    
        11 ADAMS      CLERK             11    
        12 JAMES      CLERK             12    
        13 FORD       ANALYST           13    
        14 MILLER     CLERK             14  
 如果没有partition by子句, 结果集将是按照order by 指定的列进行排序； 
 with row_number_test as(    
     select 22 a,'twenty two' b from dual union all    
     select 1,'one' from dual union all    
     select 13,'thirteen' from dual union all    
     select 5,'five' from dual union all    
     select 4,'four' from dual)    
select a,b,    
       row_number() over (order by b)    
from row_number_test    
order by a;  
 正如我们所期待的,row_number()返回按照b列排序的结果, 
 然后再按照a进行排序,才得到下面的结果: 
 A B          ROW_NUMBER()OVER(ORDERBYB)    
-- ---------- --------------------------    
1 one                                 3    
4 four                                2    
5 five                                1    
13 thirteen                            4    
22 twenty two                          5  
2.row_number() over (partition by col_n[,col_m ...] order by col_1[,col_2 ...]) 
 作用:先按照col_n[,col_m ...进行分组, 
 再在每个分组中按照col_1[,col_2 ...]进行排序(升序), 
 最后返回排好序后的结果集: 
 with row_number_test as(    
     select 22 a,'twenty two' b,'*' c from dual union all    
     select 1,'one','+' from dual union all    
     select 13,'thirteen','*' from dual union all    
     select 5,'five','+' from dual union all    
     select 4,'four','+' from dual)    
select a,b,    
       row_number() over (partition by c order by b) row_number    
from row_number_test    
order by a;  
 这个例子中，我们先按照c列分组，分为2组('*'组,'+'组)， 
 再按照每个小组的b列进行排序(按字符串首字母的ascii码排), 
 最后按照a列排序,得到下面的结果集: 
 A B          ROW_NUMBER    
-- ---------- ----------    
1 one                 3    
4 four                2    
5 five                1    
13 thirteen            1    
22 twenty two 
 示例：
有以下需求，实现抽取分组的随机前3条对某个字段不重复的数据？
 即按id1分组，随机抽取 id2不重复的前N条记录。
 例：
with temp as (                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551204 id2 from dual union all                                                
select 'dg' id1,13907551205 id2 from dual union all                                                
select 'dg' id1,13907551206 id2 from dual union all                                                
select 'dg' id1,13907551207 id2 from dual union all                                                
select 'dg' id1,13907551207 id2 from dual union all                                                
select 'dc' id1,13907551209 id2 from dual union all                                                
select 'dc' id1,13907551210 id2 from dual union all                                                
select 'dc' id1,13907551210 id2 from dual union all                                                
select 'dc' id1,13907551212 id2 from dual                                                 
) 
 实现方法：
with temp as (                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551201 id2 from dual union all                                                
select 'dg' id1,13907551204 id2 from dual union all                                                
select 'dg' id1,13907551205 id2 from dual union all                                                
select 'dg' id1,13907551206 id2 from dual union all                                                
select 'dg' id1,13907551207 id2 from dual union all                                                
select 'dg' id1,13907551207 id2 from dual union all                                                
select 'dc' id1,13907551209 id2 from dual union all                                                
select 'dc' id1,13907551210 id2 from dual union all                                                
select 'dc' id1,13907551210 id2 from dual union all                                                
select 'dc' id1,13907551212 id2 from dual                                                 
)                                                
select * from (
select  temp.*
,row_number() over(partition by id1 order by dbms_random.random) rid1
from (
select temp.*
--,row_number() over(partition by id1 order by id1) rid1
,row_number() over(partition by id1,id2 order by id1,id2) rn 
from temp
) temp
where rn=1)
where rid1<=3
但由于temp表的数据量很大，是否还有更优的SQL实现？
select * from (
select  temp.*
,row_number() over(partition by id1 order by dbms_random.random) rid1
from (
select distinct id1, id2 from temp)
)
where rid1<=3;





