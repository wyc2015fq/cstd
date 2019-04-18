# 【转】Oracle中使用WMSYS.WM_CONCAT 进行列转换行，并自动拼接 - weixin_33985507的博客 - CSDN博客
2012年05月29日 21:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
```
select  t.rank, t.Name from t_menu_item t; 
10 CLARK 
10 KING 
10 MILLER 
20 ADAMS 
20 FORD 
20 JONES 
20 SCOTT 
20 SMITH 
30 ALLEN 
30 BLAKE 
30 JAMES 
30 MARTIN 
30 TURNER 
30 WARD 
-------------------------------- 
我们通过 10g 所提供的 WMSYS.WM_CONCAT 函数即可以完成 行转列的效果 
select t.rank, WMSYS.WM_CONCAT(t.Name) TIME From t_menu_item t GROUP BY t.rank; 
DEPTNO ENAME 
------ ---------- 
10 CLARK, KING, MILLER 
20 ADAMS, FORD, JONES, SCOTT, SMITH 
30 ALLEN, BLAKE, JAMES, MARTIN, TURNER, WARD
```
