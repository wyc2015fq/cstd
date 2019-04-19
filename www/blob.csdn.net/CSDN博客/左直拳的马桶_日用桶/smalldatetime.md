# smalldatetime - 左直拳的马桶_日用桶 - CSDN博客
2013年02月07日 16:56:33[左直拳](https://me.csdn.net/leftfist)阅读数：1611
SQL SERVER 的DateTime类型精度很高，精确到秒再往下，对于一般的系统，完全没有必要，因此日期类型，我通常设置为smalldatetime，精确到分就行了，它的秒恒为0。
但今天才知道，它的日期有范围，是
1900-01-01 到 2079-06-06
所以象下面这种句子就一定会报错：
declare @date SMALLDATETIME = '2099-01-01'
DATETIME:
1753 年 1 月 1 日到 9999 年 12 月 31 日
