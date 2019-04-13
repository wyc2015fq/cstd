
# Oracle转换函数 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:50:32[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：49


-----------------------------------------------Oracle转换函数------------------------------------------------------------
1.    TO_CHAR:字符串转换函数
范例：查询所有的雇员将将年月日分开，此时可以使用TO_CHAR函数来拆分
拆分时需要使用通配符
年：y, 年是四位使用yyyy
月：m, 月是两位使用mm
日：d,    日是两位使用dd
时：HH是12进制，HH24进制
分：mi
秒：ss
![](https://img-blog.csdn.net/2018052621485473?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180526214858909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在结果中10以下的月前面被被补了前导零，可以使用fm去掉前导零
![](https://img-blog.csdn.net/20180526214904144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
TO_CHAR还可以给数字做格式化
范例：把雇员的工资按三位用“,”分隔，在oracle中“9”代表一位数字
![](https://img-blog.csdn.net/20180526214927431?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果在钱的前面加上国家的符号可以使用“$”代表是美元，如果要使用本地的钱的单位使用“L”
![](https://img-blog.csdn.net/20180526214944317?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180526214948619?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    TO_NUMBER:数值转换函数
TO_NUMBER可以把字符串转换成数值
![](https://img-blog.csdn.net/20180526214953584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.  TO_DATE:日期转换函数
TO_DATE可以把字符串的数据转换成日期类型
![](https://img-blog.csdn.net/20180526215000141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select ename,
|to_char(hiredate, 'yyyy') 年,
|to_char(hiredate, 'mm') 月,
|to_char(hiredate, 'dd') 日,
|to_char(hiredate, 'HH') 时,
|to_char(hiredate, 'mi') 分,
|to_char(hiredate, 'ss') 秒
|from emp --将入职日期转换为年月日时分秒
|select to_char(sysdate,'yyyy-mm-dd  HH24:mi:ss') from dual --以指定的格式来展示日期
|select to_char(hiredate,'fmyyyy-mm-dd')  from emp --以指定格式展示入职日期，在格式前面加fm可以去除前导零（像月份不满10的，显示01,03）
|select to_char(sal,'000,0') from emp --以指定的格式展示工资
|select to_char(sal,'$000,0') from emp --以美元展示
|select to_char(sal,'L000,0') from emp --以本地的货币符号展示
|select to_number('12')+to_number('13')  from dual --将字符串数值转换为数值
|select to_date('1993-01-01  13:24:12','yyyy-mm-dd HH24:mi:ss') from dual --将字符串日期转换成日期
|


