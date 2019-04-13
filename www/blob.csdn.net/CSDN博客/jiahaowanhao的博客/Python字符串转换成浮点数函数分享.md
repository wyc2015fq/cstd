
# Python字符串转换成浮点数函数分享 - jiahaowanhao的博客 - CSDN博客


2018年02月01日 20:33:14[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：391


[Python字符串转换成浮点数函数分享](http://cda.pinggu.org/view/24625.html)
本文给大家分享的是一则使用Python实现字符串转换成浮点数的代码，主要是使用map和reduce方法来实现，有需要的小伙伴可以参考下。
![](http://cda.pinggu.org/uploadfile/image/20180201/20180201070043_15327.png)
利用map和reduce编写一个str2float函数，把字符串'123.456'转换成浮点数123.456
from functools import reduce
def str2float(s):
return reduce(lambda x,y:x+int2dec(y),map(str2int,s.split('.')))
def char2num(s):
return {'0':0,'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9}[s]
def str2int(s):
return reduce(lambda x,y:x*10+y,map(char2num,s))
def intLen(i):
return len('%d'%i)
def int2dec(i):
return i/(10**intLen(i))
print(str2float('123.456'))
以上就是本代码的全部内容了，希望对大家学习Python能够有所帮助。

