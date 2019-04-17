# 使用pandas读取csv文件的指定列 - 小灰笔记 - CSDN博客





2017年04月15日 20:36:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：45311
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









根据教程实现了读取csv文件前面的几行数据，一下就想到了是不是可以实现前面几列的数据。经过多番尝试总算试出来了一种方法。

之所以想实现读取前面的几列是因为我手头的一个csv文件恰好有后面几列没有可用数据，但是却一直存在着。原来的数据如下：

GreydeMac-mini:chapter06 greyzhang$ cat data.csv 

1,name_01,coment_01,,,,

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




如果使用pandas读取出全部的数据，打印的时候会出现以下结果：

In [**41**]:
data = pd.read_csv('data.csv')




In [**42**]:
data

Out[**42**]: 

     1  name_01  coment_01  Unnamed: 3  Unnamed: 4  Unnamed: 5  Unnamed: 6

0    2  name_02  coment_02         NaN         NaN         NaN         NaN

1    3  name_03  coment_03         NaN         NaN         NaN         NaN

2    4  name_04  coment_04         NaN         NaN         NaN         NaN

3    5  name_05  coment_05         NaN         NaN         NaN         NaN

4    6  name_06  coment_06         NaN         NaN         NaN         NaN

5    7  name_07  coment_07         NaN         NaN         NaN         NaN

6    8  name_08  coment_08         NaN         NaN         NaN         NaN

7    9  name_09  coment_09         NaN         NaN         NaN         NaN

8   10  name_10  coment_10         NaN         NaN         NaN         NaN

9   11  name_11  coment_11         NaN         NaN         NaN         NaN

10  12  name_12  coment_12         NaN         NaN         NaN         NaN

11  13  name_13  coment_13         NaN         NaN         NaN         NaN

12  14  name_14  coment_14         NaN         NaN         NaN         NaN

13  15  name_15  coment_15         NaN         NaN         NaN         NaN

14  16  name_16  coment_16         NaN         NaN         NaN         NaN

15  17  name_17  coment_17         NaN         NaN         NaN         NaN

16  18  name_18  coment_18         NaN         NaN         NaN         NaN

17  19  name_19  coment_19         NaN         NaN         NaN         NaN

18  20  name_20  coment_20         NaN         NaN         NaN         NaN

19  21  name_21  coment_21         NaN         NaN         NaN         NaN




所说在学习的过程中这并不会给我带来什么障碍，但是在命令行终端界面呆久了总喜欢稍微清爽一点的风格。使用read_csv的参数usecols能够在一定程度上减少这种混乱感。

In [**45**]:
data = pd.read_csv('data.csv',usecols=[0,1,2,3])




In [**46**]:
data

Out[**46**]: 

     1  name_01  coment_01  Unnamed: 3

0    2  name_02  coment_02         NaN

1    3  name_03  coment_03         NaN

2    4  name_04  coment_04         NaN

3    5  name_05  coment_05         NaN

4    6  name_06  coment_06         NaN

5    7  name_07  coment_07         NaN

6    8  name_08  coment_08         NaN

7    9  name_09  coment_09         NaN

8   10  name_10  coment_10         NaN

9   11  name_11  coment_11         NaN

10  12  name_12  coment_12         NaN

11  13  name_13  coment_13         NaN

12  14  name_14  coment_14         NaN

13  15  name_15  coment_15         NaN

14  16  name_16  coment_16         NaN

15  17  name_17  coment_17         NaN

16  18  name_18  coment_18         NaN

17  19  name_19  coment_19         NaN

18  20  name_20  coment_20         NaN

19  21  name_21  coment_21         NaN




为了能够看到数据的“边界”，读取的时候显示了第一列无效的数据。正常的使用中，或许我们是想连上面结果中最后一列的信息也去掉的，那只需要在参数重去掉最后一列的列号。

In [**47**]:
data = pd.read_csv('data.csv',usecols=[0,1,2])




In [**48**]:
data

Out[**48**]: 

     1  name_01  coment_01

0    2  name_02  coment_02

1    3  name_03  coment_03

2    4  name_04  coment_04

3    5  name_05  coment_05

4    6  name_06  coment_06

5    7  name_07  coment_07

6    8  name_08  coment_08

7    9  name_09  coment_09

8   10  name_10  coment_10

9   11  name_11  coment_11

10  12  name_12  coment_12

11  13  name_13  coment_13

12  14  name_14  coment_14

13  15  name_15  coment_15

14  16  name_16  coment_16

15  17  name_17  coment_17

16  18  name_18  coment_18

17  19  name_19  coment_19

18  20  name_20  coment_20

19  21  name_21  coment_21



