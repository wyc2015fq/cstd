# 使用pandas读取csv文件指定的前几行 - 小灰笔记 - CSDN博客





2017年04月15日 17:26:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：26113
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









用于存储数据的csv文件有时候数据量是十分庞大的，然而我们有时候并不需要全部的数据，我们需要的可能仅仅是前面的几行。这样就可以通过pandas中read_csv中指定行数读取的功能实现。

例如有data.csv文件，文件的内容如下：

GreydeMac-mini:chapter06 greyzhang$ cat data.csv 

,name_01,coment_01,,,,

2,name_02,coment_02,,,,

3,name_03,coment_03,,,,

4,name_04,coment_04,,,,

5,name_05,coment_05,,,,

6,name_06,coment_06,,,,

7,name_07,coment_07,,,,

8,name_08,coment_08,,,,

9,name_09,coment_09,,,,

10,name_10,coment_10,,,,

11,name_11,coment_11,,,,

12,name_12,coment_12,,,,

13,name_13,coment_13,,,,

14,name_14,coment_14,,,,

15,name_15,coment_15,,,,

16,name_16,coment_16,,,,

17,name_17,coment_17,,,,

18,name_18,coment_18,,,,

19,name_19,coment_19,,,,

20,name_20,coment_20,,,,

21,name_21,coment_21,,,,




如果我们需要的数据仅仅是前5行，那么读取方式可以通过nrows的方式进行指定。编写代码如下：

  1 #!/usr/bin/python

  2 

  3 import pandasas pd

  4 

  5 data = pd.read_csv('data.csv',nrows =5)

  6 print(data)

  7 

代码的运行结果如下：




GreydeMac-mini:chapter06 greyzhang$ python row_test.py 

   Unnamed: 0  name_01  coment_01  Unnamed: 3  Unnamed: 4  Unnamed: 5  \

0           2  name_02  coment_02         NaN         NaN         NaN   

1           3  name_03  coment_03         NaN         NaN         NaN   

2           4  name_04  coment_04         NaN         NaN         NaN   

3           5  name_05  coment_05         NaN         NaN         NaN   

4           6  name_06  coment_06         NaN         NaN         NaN   




   Unnamed: 6  

0         NaN  

1         NaN  

2         NaN  

3         NaN  

4         NaN  

GreydeMac-mini:chapter06 greyzhang$ 




从上面的结果中可以看出，通过指定读取行数实现了预期的功能。



