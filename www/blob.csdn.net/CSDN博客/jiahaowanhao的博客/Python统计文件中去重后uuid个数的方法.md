
# Python统计文件中去重后uuid个数的方法 - jiahaowanhao的博客 - CSDN博客


2018年05月31日 22:09:49[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：197


[Python统计文件中去重后uuid个数的方法](http://cda.pinggu.org/view/25693.html)
本文实例讲述了Python统计文件中去重后uuid个数的方法。分享给大家供大家参考。具体如下：
利用正则表达式按行获取日志文件中的的uuid，并且统计这些uuid的去重个数（去重利用set）
import re
pattern=re.compile(r'&uuid=.*&')
uuidset=set()
with open('request.log.2015-05-26','rt') as f:
for line in f:
all=pattern.findall(line)
if len(all)>0 and line.find('iphone')>0:
\# print(all[0])
uuidset.add(all[0])
print(len(uuidset))
希望本文所述对大家的Python程序设计有所帮助。

