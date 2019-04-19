# mysql字符函数简析 - fanyun的博客 - CSDN博客
2016年03月23日 15:33:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：527
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)

##         MySQL字符串截取函数：left(), right(),substring(), substring_index(),还有 mid(),substr()。其中，mid(),substr() 等价于 substring() 函数，substring()的功能非常强大和灵活。
1. 字符串截取：left(str,length) 
<span style="font-family:宋体;font-size:14px;">mysql> select left('sqlstudy.com', 3);</span>+-------------------------+| left('sqlstudy.com', 3) |+-------------------------+| sql                     |+-------------------------+
2. 字符串截取：right(str,length) 
<span style="font-family:宋体;font-size:14px;">mysql> select right('sqlstudy.com', 3);</span>+--------------------------+| right('sqlstudy.com', 3) |+--------------------------+| com                      |+--------------------------+
3. 字符串截取：substring(str,pos); substring(str, pos, len) 
3.1 从字符串的第 4 个字符位置开始取，直到结束。
<span style="font-family:宋体;font-size:14px;">mysql> select substring('sqlstudy.com', 4);</span>+------------------------------+| substring('sqlstudy.com', 4) |+------------------------------+| study.com                    |+------------------------------+
3.2 从字符串的第 4 个字符位置开始取，只取 2 个字符。 
<span style="font-family:宋体;font-size:14px;">mysql> select substring('sqlstudy.com', 4, 2);</span>+---------------------------------+| substring('sqlstudy.com', 4, 2) |+---------------------------------+| st                              |+---------------------------------+
3.3 从字符串的第 4 个字符位置（倒数）开始取，直到结束。
<span style="font-family:宋体;font-size:14px;">mysql> select substring('sqlstudy.com', -4);</span>+-------------------------------+| substring('sqlstudy.com', -4) |+-------------------------------+| .com                          |+-------------------------------+
3.4 从字符串的第 4 个字符位置（倒数）开始取，只取 2 个字符。
<span style="font-family:宋体;font-size:14px;">mysql> select substring('sqlstudy.com', -4, 2);</span>+----------------------------------+| substring('sqlstudy.com', -4, 2) |+----------------------------------+| .c                               |+----------------------------------+
我们注意到在函数 substring(str,pos, len)中， pos 可以是负值，但len 不能取负值。
4. 字符串截取：substring_index(str,delim,count)
4.1 截取第二个 '.' 之前的所有字符。
<span style="font-family:宋体;font-size:14px;">mysql> select substring_index('www.sqlstudy.com.cn', '.', 2);</span>+------------------------------------------------+| substring_index('www.sqlstudy.com.cn', '.', 2) |+------------------------------------------------+| www.sqlstudy                                   |+------------------------------------------------+
4.2 截取第二个 '.' （倒数）之后的所有字符。
<span style="font-family:宋体;font-size:14px;">mysql> select substring_index('www.sqlstudy.com.cn', '.', -2);</span>+-------------------------------------------------+| substring_index('www.sqlstudy.com.cn', '.', -2) |+-------------------------------------------------+| com.cn                                          |+-------------------------------------------------+
4.3 如果在字符串中找不到 delim 参数指定的值，就返回整个字符串
<span style="font-family:宋体;font-size:14px;">mysql> select substring_index('www.sqlstudy.com.cn', '.coc', 1);</span>+---------------------------------------------------+| substring_index('www.sqlstudy.com.cn', '.coc', 1) |+---------------------------------------------------+| www.sqlstudy.com.cn                               |+---------------------------------------------------+
