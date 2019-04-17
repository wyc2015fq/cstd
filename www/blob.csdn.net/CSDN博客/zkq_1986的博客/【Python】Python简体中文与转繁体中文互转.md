# 【Python】Python简体中文与转繁体中文互转 - zkq_1986的博客 - CSDN博客





2018年08月30日 16:11:43[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：642








其实利用python实现汉字的简体和繁体相互转早有人做过,并发布到github上了,地址:[https://github.com/skydark/nstools/tree/master/zhtools](https://github.com/skydark/nstools/tree/master/zhtools)

该项目还有其他很多跟汉字相关的功能,本文只介绍繁体和简体相互转换

具体方法很简单,下载该项目中的 zh_wiki.py  和 langconv.py 两个文件,放到python代码目录下就可以了.

我的python是3.5版本,所以在字符串的decode上和python2.x 有所不同,demo:

```
from langconv import *
import sys

print(sys.version)
print(sys.version_info)

# 转换繁体到简体
def cht_to_chs(line):
    line = Converter('zh-hans').convert(line)
    line.encode('utf-8')
    return line

# 转换简体到繁体
def chs_to_cht(line):
    line = Converter('zh-hant').convert(line)
    line.encode('utf-8')
    return line

line_chs='<>123asdasd把中文字符串进行繁体和简体中文的转换'
line_cht='<>123asdasd把中文字符串進行繁體和簡體中文的轉換'

ret_chs = "%s\n"%cht_to_chs(line_cht)
ret_cht = "%s\n"%chs_to_cht(line_chs)

print("chs='%s'",ret_cht)
print("cht='%s'",ret_cht)

file = open('ret.txt','w',encoding='utf-8')
file.write(ret_chs)
file.write(ret_cht)
file.close()
```

ret.txt文件内容

<>123asdasd把中文字符串进行繁体和简体中文的转换
<>123asdasd把中文字符串進行繁體和簡體中文的轉換


转载自：[https://www.jianshu.com/p/c3fd8629a0cc](https://www.jianshu.com/p/c3fd8629a0cc)



