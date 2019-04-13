
# Oracle字符函数和数值函数 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:46:31[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：242


-----------------------------------------Oracle字符函数和数值函数--------------------------------------------
### 字符函数
**接收字符输入返回字符或者数值，dual是伪表**
1.    把小写的字符转换成大小的字符
upper('smith')
![](https://img-blog.csdn.net/20180526214437162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    把大写字符变成小写字符
lower('SMITH')
![](https://img-blog.csdn.net/20180526214442260?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.    把首字符大写
initcap('smith')
![](https://img-blog.csdn.net/20180526214447631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.    字符串的连接可以使用concat可以使用“||”建议使用“||”
concat('hello','world')
![](https://img-blog.csdn.net/20180526214454697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.    字符串的截取，使用substr，第一个参数是源字符串，第二个参数是开始索引，第三个参数结束的索引，开始的索引使用1和0效果相同
substr('hello',1,3)
![](https://img-blog.csdn.net/20180526214501290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.    获取字符串的长度
length('hello')
![](https://img-blog.csdn.net/20180526214511438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7.    字符串替换，第一个参数是源字符串，第二个参数被替换的字符串，第三个是替换字符串
replace('hello','l','x')
![](https://img-blog.csdn.net/20180526214518832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select 1 from dual --dual伪表查询
|select upper('smith') from dual --把小写字母转换为大写字母
|select lower('smith') from dual --把大写变成小写
|select initcap('smith') from dual --把首字母变为大写
|select concat('hello','world') from dual  --字符串连接
|select 'hello' || 'world' from dual --字符串连接，建议使用这种方式
|--因为concat函数是不跨数据库的，而||是跨数据库的，考虑到兼容性，选择||来连接字符串会更好
|select substr('hello', 1, 3) from dual --分割字符串，与java语言不同的是，最少索引是从1开始的
|select substr('hello',0,9) from dual  --如果开始索引设为0也是从1开始的，要分割的长度超过了字符串的长度，就会显示整个字符串
|select length('hello') from dual --计算字符串的长度
|select replace('hello','e','x') from dual  --字符串的替换，第一个参数是原来的字符串，第二个参数是要替换的字符，第三个参数是替换后的字符
|
-----------------------------------------------------------------------------------------------------------------------------------
### 数值函数
1.    四舍五入函数：ROUND()
默认情况下ROUND四舍五入取整，可以自己指定保留的位数。
![](https://img-blog.csdn.net/20180526214544145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018052621454896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    取整：TRUNC()，默认全部去掉小数，也可以指定保留的位数
![](https://img-blog.csdn.net/20180526214552171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180526214556707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.    取余数MOD()
![](https://img-blog.csdn.net/20180526214601281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select round(12.56) from dual --四舍五入
|select round(12.457,2) from dual --保留两位小数，根据舍弃的第一位来四舍五入
|select trunc(12.560) from dual --去掉小数位
|select trunc(12.567,2) from dual --保留两位小数
|select mod(10,3) from dual --取余
|


