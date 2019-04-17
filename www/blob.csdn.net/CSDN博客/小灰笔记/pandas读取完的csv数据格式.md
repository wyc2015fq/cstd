# pandas读取完的csv数据格式 - 小灰笔记 - CSDN博客





2017年04月14日 00:10:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3187








使用pandas读取csv文件读取出来的数据其实还是有一定附加格式的，那就是行列标题。

In [**18**]:
data1 = pd.read_csv('data.csv')




In [**19**]:
data1

Out[**19**]: 

    index     name    comment  Unnamed: 3  Unnamed: 4  Unnamed: 5  Unnamed: 6

0       1  name_01  coment_01         NaN         NaN         NaN         NaN

1       2  name_02  coment_02         NaN         NaN         NaN         NaN

2       3  name_03  coment_03         NaN         NaN         NaN         NaN

3       4  name_04  coment_04         NaN         NaN         NaN         NaN

4       5  name_05  coment_05         NaN         NaN         NaN         NaN

5       6  name_06  coment_06         NaN         NaN         NaN         NaN

6       7  name_07  coment_07         NaN         NaN         NaN         NaN

7       8  name_08  coment_08         NaN         NaN         NaN         NaN

8       9  name_09  coment_09         NaN         NaN         NaN         NaN

9      10  name_10  coment_10         NaN         NaN         NaN         NaN

10     11  name_11  coment_11         NaN         NaN         NaN         NaN

11     12  name_12  coment_12         NaN         NaN         NaN         NaN

12     13  name_13  coment_13         NaN         NaN         NaN         NaN

13     14  name_14  coment_14         NaN         NaN         NaN         NaN

14     15  name_15  coment_15         NaN         NaN         NaN         NaN

15     16  name_16  coment_16         NaN         NaN         NaN         NaN

16     17  name_17  coment_17         NaN         NaN         NaN         NaN

17     18  name_18  coment_18         NaN         NaN         NaN         NaN

18     19  name_19  coment_19         NaN         NaN         NaN         NaN

19     20  name_20  coment_20         NaN         NaN         NaN         NaN

20     21  name_21  coment_21         NaN         NaN         NaN         NaN




而查看原始数据的时候可以看出，最左侧的标号栏是没有的。原始数据如下：

GreydeMac-mini:chapter06 greyzhang$ cat data.csv 

index,name,comment,,,,

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




而第一行也被自动处理成了类似于标题的格式。再看一下read_table的数据可以看出，虽然同样读出了数据，但是这个功能中并没有扩展标题。不过，这个似乎是因为处理分隔符参数缺省原因造成的。

In [20]: data2 = pd.read_table('data.csv')




In [21]: data2

Out[21]: 

      index,name,comment,,,,

0    1,name_01,coment_01,,,,

1    2,name_02,coment_02,,,,

2    3,name_03,coment_03,,,,

3    4,name_04,coment_04,,,,

4    5,name_05,coment_05,,,,

5    6,name_06,coment_06,,,,

6    7,name_07,coment_07,,,,

7    8,name_08,coment_08,,,,

8    9,name_09,coment_09,,,,

9   10,name_10,coment_10,,,,

10  11,name_11,coment_11,,,,

11  12,name_12,coment_12,,,,

12  13,name_13,coment_13,,,,

13  14,name_14,coment_14,,,,

14  15,name_15,coment_15,,,,

15  16,name_16,coment_16,,,,

16  17,name_17,coment_17,,,,

17  18,name_18,coment_18,,,,

18  19,name_19,coment_19,,,,

19  20,name_20,coment_20,,,,

20  21,name_21,coment_21,,,,




尝试加上分隔符参数，得出的结果如下：

In [23]: data3 = pd.read_table('data.csv',',')




In [24]: data3

Out[24]: 

    index     name    comment  Unnamed: 3  Unnamed: 4  Unnamed: 5  Unnamed: 6

0       1  name_01  coment_01         NaN         NaN         NaN         NaN

1       2  name_02  coment_02         NaN         NaN         NaN         NaN

2       3  name_03  coment_03         NaN         NaN         NaN         NaN

3       4  name_04  coment_04         NaN         NaN         NaN         NaN

4       5  name_05  coment_05         NaN         NaN         NaN         NaN

5       6  name_06  coment_06         NaN         NaN         NaN         NaN

6       7  name_07  coment_07         NaN         NaN         NaN         NaN

7       8  name_08  coment_08         NaN         NaN         NaN         NaN

8       9  name_09  coment_09         NaN         NaN         NaN         NaN

9      10  name_10  coment_10         NaN         NaN         NaN         NaN

10     11  name_11  coment_11         NaN         NaN         NaN         NaN

11     12  name_12  coment_12         NaN         NaN         NaN         NaN

12     13  name_13  coment_13         NaN         NaN         NaN         NaN

13     14  name_14  coment_14         NaN         NaN         NaN         NaN

14     15  name_15  coment_15         NaN         NaN         NaN         NaN

15     16  name_16  coment_16         NaN         NaN         NaN         NaN

16     17  name_17  coment_17         NaN         NaN         NaN         NaN

17     18  name_18  coment_18         NaN         NaN         NaN         NaN

18     19  name_19  coment_19         NaN         NaN         NaN         NaN

19     20  name_20  coment_20         NaN         NaN         NaN         NaN

20     21  name_21  coment_21         NaN         NaN         NaN         NaN




加上分隔符参数之后，两个的功能倒是十分一致。在书中看到有一种缺少标题的数据格式，到现在为止还没有理解其中的功能，没有构造出这样的输入数据。后期再慢慢摸索一下。



