# 使用pandas read_table读取csv文件 - 小灰笔记 - CSDN博客





2017年04月13日 23:53:02[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：15866








read_csv是pandas中专门用于csv文件读取的功能，不过这并不是唯一的处理方式。pandas中还有读取表格的通用函数read_table。

接下来使用read_table功能作一下csv文件的读取尝试，使用此功能的时候需要指定文件中的内容分隔符。

查看csv文件的内容如下；

In [**10**]:
cat data.csv

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




使用pandas读取文件内容如下：In [**11**]:
data1 = pd.read_table('data.csv',sep=',')




In [**12**]: type(data1)

Out[**12**]:
pandas.core.frame.DataFrame




In [**13**]:
data1

Out[**13**]: 

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




不过在几番尝试下来，发现这个分隔符缺省的时候倒是也能够读出数据。

In [16]: data2 = pd.read_table('data.csv')




In [17]: data2

Out[17]: 

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




不知道此功能对其他格式的数据的读取功能会不会有自动识别的功能，需要继续确认。



