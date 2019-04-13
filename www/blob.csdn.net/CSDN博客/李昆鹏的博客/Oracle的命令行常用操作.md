
# Oracle的命令行常用操作 - 李昆鹏的博客 - CSDN博客


2018年05月21日 15:30:33[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1627


----------------------------------------------------Oracle的命令行常用操作----------------------------------------
# 1. 命令行常用操作
## 1.  登录
运行cmd进入命令行
Sqlplus 用户名/密码 [as sysdba]
![](https://img-blog.csdn.net/20180521152654241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果是超级管理员需要在用户名/密码后面加上 as sysdba,是以系统管理员的身份来登录的，如图。
![](https://img-blog.csdn.net/20180521152707554?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果是普通用户不需要as sysdba

## 2． 查看当前连接数据库的用户
使用show user查看当前的用户
![](https://img-blog.csdn.net/20180521152731798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3.  用户的切换
在登录的状态下输入：conn 用户名/密码 [as sysdba]
如图：
Ø  切换为超级管理员
![](https://img-blog.csdn.net/20180521152750947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø  切换为普通的用户
![](https://img-blog.csdn.net/2018052115280449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 4.  查看用户下的表
为了防止命令行中数据展示表格错乱的情况可以设计行宽和列宽
Set  linesize 300;         每行展示300个字符
Col列名          for  a[数字]，某一列占几个字符
在用户登录的情况下输入：select * from tab;
![](https://img-blog.csdn.net/20180521152814484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
查看表中的数据
Col ename for a8
Col mgr for 9999
Col sal for 9999
Select * from emp;
![](https://img-blog.csdn.net/20180521152825496?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果使用dba用户登录查询该表无法查看到，因为sys用户下没有这个emp表
![](https://img-blog.csdn.net/20180521152831560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由于超级管理员的权限很大可以查看Scott用户下的表
可以使用select * from [用户名].[表名] 来查看某用户下的表的数据
![](https://img-blog.csdn.net/20180521152837977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5.  查看表的结构
Desc 表名
![](https://img-blog.csdn.net/20180521152845459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Number(4)  最大长度为4为的数值类型
Varchar2(10)           最大长度为10的字符串，varchar2用于存储可变长度的字符串，.varchar2把所有字符都占两字节处理(一般情况下)，varchar只对汉字和全角等字符占两字节，数字，英文字符等都是一个字节，VARCHAR2把空串等同于null处理，而varchar仍按照空串处理；建议在oracle中使用varchar2
Number(7,2)           数值类型整数位占5位，小数位占2位，一共7位。
Date                                   时间类型
# Scott用户下的表结构
![](https://img-blog.csdn.net/20180521152854398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521152911622?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521152920946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521152934835?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521152943990?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


