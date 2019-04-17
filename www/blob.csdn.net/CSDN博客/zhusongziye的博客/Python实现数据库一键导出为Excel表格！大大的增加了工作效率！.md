# Python实现数据库一键导出为Excel表格！大大的增加了工作效率！ - zhusongziye的博客 - CSDN博客





2018年05月18日 21:18:09[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：898








                数据库数据导出为excel表格，也可以说是一个很常用的功能了。毕竟不是任何人都懂数据库操作语句的。 

下面先来看看完成的效果吧。

数据源

![](https://img-blog.csdn.net/20180518211712566?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

导出结果 



![](https://img-blog.csdn.net/20180518211724579?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



#### 依赖

由于是Python实现的，所以需要有Python环境的支持

#### Python2.7.11

我的Python环境是2.7.11。虽然你用的可能是3.5版本，但是思想是一致的。

##### xlwt

```
pip install xlwt
```

##### python2.7: MySQLdb

```
pip install MySQLdb 
如果上述方式不成功的话，可以到sourceforge官网上去下载windows上的msi版本或者使用源码自行编译。
```

#### python3.X:

请使用pip install mysqlclient

![](https://img-blog.csdn.net/20180518220955252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


#### 数据库相关

本次试验，数据库相关的其实也就是如何使用Python操作数据库而已，知识点也很少，下述为我们本次用到的一些简单的语句。

#### 连接

```
conn = MySQLdb.connect(host=’localhost’,user=’root’,passwd=’mysql’,db=’test’,charset=’utf8’)
```

这里值得我们一提的就是最后一个参数的使用，不然从数据库中取出的数据就会使乱码。关于乱码问题，如果还有不明白的地方，不妨看下这篇文章http://blog.csdn.net/marksinoberg/article/details/52254401。

#### 获取字段信息

```
fields = cursor.description
```

至于cursor，是我们操作数据库的核心。游标的特点就是一旦遍历过该条数据，便不可返回。但是我们也可以手动的改变其位置。

```
cursor.scroll(0,mode=’absolute’)来重置游标的位置
```

#### 获取数据

获取数据简直更是轻而易举，但是我们必须在心里明白，数据项是一个类似于二维数组的存在。我们获取每一个cell项的时候应该注意。

```
results = cursor.fetchall()
```

#### Excel基础

同样，这里讲解的也是如何使用Python来操作excel数据。

#### workbook

工作薄的概念我们必须要明确，其是我们工作的基础。与下文的sheet相对应，workbook是sheet赖以生存的载体。

```
workbook = xlwt.Workbook()
```

#### sheet

我们所有的操作，都是在sheet上进行的。

```
sheet = workbook.add_sheet(‘table_message’,cell_overwrite_ok=True)
```

对于workbook 和sheet，如果对此有点模糊。不妨这样进行假设。

> 
日常生活中记账的时候，我们都会有一个账本，这就是workbook。而我们记账则是记录在一张张的表格上面，这些表格就是我们看到的sheet。一个账本上可以有很多个表格，也可以只是一个表格。这样就很容易理解了吧。 :-)


#### 案例

下面看一个小案例。

```
*# coding:utf8*
import sys

reload(sys)
sys.setdefaultencoding('utf8')
*#    __author__ = '郭 璞'*
*#    __date__ = '2016/8/20'*
*#    __Desc__ = 从数据库中导出数据到excel数据表中*

import xlwt
import MySQLdb

conn = MySQLdb.connect('localhost','root','mysql','test',charset='utf8')
cursor = conn.cursor()

count = cursor.execute('select * from message')
print count
*# 重置游标的位置*
cursor.scroll(0,mode='absolute')
*# 搜取所有结果*
results = cursor.fetchall()

*# 获取MYSQL里面的数据字段名称*
fields = cursor.description
workbook = xlwt.Workbook()
sheet = workbook.add_sheet('table_message',cell_overwrite_ok=True)

*# 写上字段信息*
for field in range(0,len(fields)):
    sheet.write(0,field,fields[field][0])

*# 获取并写入数据段信息*
row = 1
col = 0
for row in range(1,len(results)+1):
    for col in range(0,len(fields)):
        sheet.write(row,col,u'%s'%results[row-1][col])

workbook.save(r'./readout.xlsx')
```

#### 封装

为了使用上的方便，现将其封装成一个容易调用的函数。

封装之后

```
*# coding:utf8*
import sys

reload(sys)
sys.setdefaultencoding('utf8')
*#    __author__ = '郭 璞'*
*#    __date__ = '2016/8/20'*
*#    __Desc__ = 从数据库中导出数据到excel数据表中*

import xlwt
import MySQLdb

def export(host,user,password,dbname,table_name,outputpath):
    conn = MySQLdb.connect(host,user,password,dbname,charset='utf8')
    cursor = conn.cursor()

    count = cursor.execute('select * from '+table_name)
    print count
    *# 重置游标的位置*
    cursor.scroll(0,mode='absolute')
    *# 搜取所有结果*
    results = cursor.fetchall()

    *# 获取MYSQL里面的数据字段名称*
    fields = cursor.description
    workbook = xlwt.Workbook()
    sheet = workbook.add_sheet('table_'+table_name,cell_overwrite_ok=True)

    *# 写上字段信息*
    for field in range(0,len(fields)):
        sheet.write(0,field,fields[field][0])

    *# 获取并写入数据段信息*
    row = 1
    col = 0
    for row in range(1,len(results)+1):
        for col in range(0,len(fields)):
            sheet.write(row,col,u'%s'%results[row-1][col])

    workbook.save(outputpath)


*# 结果测试*
if __name__ == "__main__":
    export('localhost','root','mysql','test','datetest',r'datetest.xlsx')
```

#### 测试结果

```
id  name    date
1   dlut    2016-07-06
2   清华大学    2016-07-03
3   北京大学    2016-07-28
4   Mark    2016-08-20
5   Tom  2016-08-19
6   Jane    2016-08-21
```

#### 总结

回顾一下，本次试验用到了哪些知识点。

1、Python简易操作数据库
2、Python简易操作Excel
3、数据库取出数据乱码问题解决之添加charset=utf-8
4、以二维数组的角度来处理获取到的结果集。
作者：Marksinoberg
原文链接：https://blog.csdn.net/marksinoberg/article/details/52263975



