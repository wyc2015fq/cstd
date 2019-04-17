# 使用pandas批量处理矢量化字符串 - 小灰笔记 - CSDN博客





2017年04月23日 23:03:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2326








进行已经矢量化后的字符串数据，可以使用pandas的Series数据对象的map方法。这样，对于未经矢量化的数据也可以先进行数据的矢量化转换然后再进行相应的处理。

举例实现字符串数据的操作，编写代码如下：

  1 #!/usr/bin/python

  2 

  3 import numpy
as np

  4 import pandas
as pd

  5 from pandas
import Series,DataFrame

  6 

  7 seriers_data = Series(['Hello','Python','Data','World'])

  8 print(seriers_data.map(str.lower))

  9 




程序的运行结果如下：

GreydeMac-mini:chapter07 greyzhang$ vim series_map.py

GreydeMac-mini:chapter07 greyzhang$ python series_map.py 

0     hello

1    python

2      data

3     world

dtype: object




上面的例子中，先对列表形式的数据进行了向量化的处理，讲数据处理成pandas的Series对象类型，然后进行矢量化的操作。通过Series的map方法对每个字符串进行了全都转换成小写字母的操作。



