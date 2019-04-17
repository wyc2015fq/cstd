# Python中的字符串序列拼接方法 - 小灰笔记 - CSDN博客





2017年04月17日 08:05:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1607








最初看到这个功能的时候，不由得来了积分亲切感。因为在Perl中也有一个用于字符串拼接的join，而那个功能我用了不知道多少次。

先写一个小脚本如下：

  1 #!/usr/bin/python

  2 

  3 str_list = ['aa','cc','bb','12']

  4 lines = '\n'.join(str_list)

  5 print lines

  6 print(type(lines))

  7 




第4行就是join的用法示范，前面是分隔符，后面是即将要合并的字符串序列。当然，语法上Python和Perl之间肯定是有差异的。

代码的执行结果如下：

aa

cc

bb

12

<type ‘str'>




从结果可以看出，字符串的拼接成功。儿字符串拼接的结果对象类型不再是一个序列（这里是字典）类型，而是一个字符串。



