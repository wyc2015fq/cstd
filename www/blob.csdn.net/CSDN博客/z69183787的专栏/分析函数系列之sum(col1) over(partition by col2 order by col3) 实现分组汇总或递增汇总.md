# 分析函数系列之sum(col1) over(partition by col2 order by col3):实现分组汇总或递增汇总 - z69183787的专栏 - CSDN博客
2018年12月25日 16:46:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：44
个人分类：[数据库-Sql&Hql																[大数据-Hive																[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)](https://blog.csdn.net/z69183787/article/category/5568487)](https://blog.csdn.net/z69183787/article/category/2185915)
[https://blog.csdn.net/rfb0204421/article/details/7672207](https://blog.csdn.net/rfb0204421/article/details/7672207)
[https://blog.csdn.net/wawmg/article/details/40840093](https://blog.csdn.net/wawmg/article/details/40840093)
语法:sum(col1) over(partition by col2 order by col3 )
 准备数据:
   DEPT_ID    ENAME          SAL
1 1000            A                     2500
2 1000            B                    3500
3 1000            C                    1500
4 1000            D                    2000
5 2000            E                    2500
6 2000            F                    2000
7 2000           G                    3500
主要有四种情况:
sum(sal) over (partition by deptno order by ename) 按部门“连续”求总和
sum(sal) over (partition by deptno) 按部门求总和
sum(sal) over (order by deptno，ename) 不按部门“连续”求总和
sum(sal) over () 不按部门，求所有员工总和，效果等同于sum(sal)。
1.有partition by有order by :   在partition by分组下,按照不同的order by col3实现递增汇总..    
SQL>>select DEPT_ID,ENAME,SAL,sum(SAL) over(partition by dept_id order by ENAME) AS TOTAL  from  dept_sal  
结果:按照部门分组,按名字排序实现递增汇总.       
DEPT_ID ENAME    SAL  TOTAL
1 1000       A           3500   3500
2 1000       B           3500  7000
3 1000       C          1500   8500
4 1000       D          2000  10500
5 2000       E          2500   2500
6 2000       F          2000   4500
7 2000       G         3500    8000       
如果col3重复会只加总一次(当然在本例中这种写法毫无意义):
SQL>> select DEPT_ID,ENAME,SAL,sum(SAL) over(partition by dept_id order by SAL) AS TOTAL from dept_sal     
DEPT_ID ENAME SAL TOTAL
11000 C 1500 1500
2 1000 D 2000 3500
3 1000 A 3500 10500
4 1000 B 3500 10500
5 2000 F 2000 2000
6 2000 E 2500 4500
7 2000 G 3500 8000
2.有partition by无order by:  实现分组内所有数据的汇总
SQL>>select DEPT_ID,ENAME,SAL,sum(SAL) over(partition by dept_id) AS TOTAL from dept_sal     
DEPT_ID ENAME SAL TOTAL
1 1000 A 3500 10500
2 1000 B 3500 10500
3 1000 C 1500 10500
4 1000 D 2000 10500
5 2000 E 2500 8000
6 2000 F 2000 8000
7 2000 G 3500 8000
 3.无partition by有order by : 直接按order by 字段实现递增汇总
SQL>>select DEPT_ID,ENAME,SAL,sum(SAL) over(order by ENAME) AS TOTAL from dept_sal
    DEPT_ID ENAME SAL TOTAL
1 1000 A 3500 3500
2 1000 B 3500 7000
3 1000 C 1500 8500
4 1000 D 2000 10500
5 2000 E 2500 13000
6 2000 F 2000 15000
7 2000 G 3500 18500
如果order by 的值相同,会进行汇总,但汇总后显示的值会是一样的,如下:
SQL>>select DEPT_ID,ENAME,SAL,sum(SAL) over(order by DEPT_ID) AS TOTAL from dept_sal
    DEPT_ID ENAME SAL TOTAL
1 1000 A 3500 10500
2 1000 B 3500 10500
3 1000 C 1500 10500
4 1000 D 2000 10500
5 2000 E 2500 18500
6 2000 F 2000 18500
7 2000 G 3500 18500
 4.无partition by无order by:  所有数据相加.
SQL>>select DEPT_ID,ENAME,SAL,sum(SAL) over() AS TOTAL from  dept_sal
    DEPT_ID ENAME SAL TOTAL
1 1000 A 3500 18500
2 1000 B 3500 18500
3 1000 C 1500 18500
4 1000 D 2000 18500
5 2000 E 2500 18500
6 2000 F 2000 18500
7 2000 G 3500 18500
--------------------- 
0、select * from wmg_test;     ---测试数据
1、select v1,v2,sum(v2) over(order by v2) as sum     --按照 v2排序，累计n+n-1+....+1
from wmg_test;
2、select v1,v2,sum(v2) over(partition by v1 order by v2) as sum     --先分组，组内在进行 1 中的操作
from wmg_test;
3、select v1,v2,sum(v2) over(partition by v1 order by v1) as sum   ---稳定排序
from wmg_test;
4、select v1,v2,sum(v2) over(partition by v1) as sum    --相同key的进行回填处理
from wmg_test;
5、select distinct v1,sum_01                                        --取一条
from (
select v1,sum(v2) over(partition by v1) as sum_01
from wmg_test
) a;
6、当然也可以逆序累加，只需order by desc 即可
总结区别：group by 和partition by的区别
group 单纯分组
partition 也能分组，但还具备累计的功能
--------------------- 
