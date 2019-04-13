
# Python字符串拼接六种方法介绍 - jiahaowanhao的博客 - CSDN博客


2018年02月16日 20:19:13[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1310


[Python字符串拼接六种方法介绍](http://cda.pinggu.org/view/24756.html)
Python字符串拼接的6种方法：
1.加号
第一种，有编程经验的人，估计都知道很多语言里面是用加号连接两个字符串，Python里面也是如此直接用“+”来连接两个字符串；
print 'Python' + 'Tab'
结果：
PythonTab
2.逗号
第二种比较特殊，使用逗号连接两个字符串，如果两个字符串用“逗号”隔开，那么这两个字符串将被连接，但是，字符串之间会多出一个空格；
print 'Python','Tab'
结果：
Python Tab
3.直接连接
第三种也是，ython独有的，只要把两个字符串放在一起，中间有空白或者没有空白，两个字符串将自动连接为一个字符串；
print 'Python''Tab'
结果：
PythonTab
print 'Python'  'Tab'
结果：
PythonTab
4.格式化
第四种功能比较强大，借鉴了C语言中printf函数的功能，如果你有C语言基础，看下文档就知道了。这种方式用符号“%”连接一个字符串和一组变量，字符串中的特殊标记会被自动用右边变量组中的变量替换：
print '%s %s'%('Python', 'Tab')
结果：
Python Tab
第五种join
就属于技巧了，利用字符串的函数join。这个函数接受一个列表，然后用字符串依次连接列表中每一个元素：
str_list = ['Python', 'Tab']
a = ''
print a.join(str_list)
结果：
PythonTab
第六种 多行字符串拼接（）
s = ('select *'
'from atable'
'where id=888')
print s, type(s)
\#输出
select *from atablewhere id=888 <type 'str'>
python遇到未闭合的小括号，自动将多行拼接为一行,相比三个引号和换行符，这种方式不会把换行符、前导空格当作字符。
总结
以上就是本文关于Python字符串拼接六种方法介绍的全部内容，希望对大家有所帮助。

