# 使用python pandas读取csv文件数据 - 小灰笔记 - CSDN博客





2017年04月13日 23:23:29[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：34131








csv是我接触的比较早的一种文件，比较好的是这种文件既能够以电子表格的形式查看又能够以文本的形式查看。最早接触是在别人的Perl脚本中，或许是为了充分利用Perl的文本处理能力。不过，日常的生活工作中我用到的比较多的倒还是电子表格。

创建一个电子表格如下：

![](https://img-blog.csdn.net/20170413232203353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




使用Mac中Numbers功能将其导出为csv文件，使用文本查看文件内容如下：

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




换用pandas尝试数据文件读取如下：

In [**1**]:
**import****pandas****as****pd**




In [**2**]:
ls

data.csv      data.numbers




In [**3**]:
data = pd.read_csv('data.csv')




In [**4**]:
data

Out[**4**]: 

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




查看读取出来的结果，看的出结果被处理成了pandas的DataFrame格式。




In [**6**]: type(data)

Out[**6**]:
pandas.core.frame.DataFrame




前阵子学习pandas的时候这部分功能学的一头雾水，如今看到这部分真有点束手无策。看来，后面还是得好好学习一下，补一下落下的课程了。




