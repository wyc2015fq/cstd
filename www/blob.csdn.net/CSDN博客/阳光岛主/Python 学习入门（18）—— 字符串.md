
# Python 学习入门（18）—— 字符串 - 阳光岛主 - CSDN博客

2013年12月06日 19:21:28[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：12199个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**Python判断字符串只包含数字**
一种方法是 a.isdigit()，但这种方法对于包含正负号的数字字符串无效，更为准确的为：

```python
#!/usr/bin/python
# -*- coding:utf-8 -*-
#
# http://blog.ithomer.net
import sys
import math
# 判断是否为数字
def isNum(value):
    try:
        value + 1
    except TypeError:
        return False
    else:
        return True
# 判断是否为数字
def isNum2(value):
    try:
        x = int(value)
    except TypeError:
        return False
    except ValueError:
        return False
    except Exception, e:
        return False
    else:
        return True

def test1():
    
    a = "123abcDE"
    print a.isalnum()       # True, 所有字符都是数字或者字母
    a = "abcDE"
    print a.isalpha()       # True, 所有字符都是字母
    a = "123.3"
    print a.isdigit()       # False, 所有字符都是数字
    a = "abcde"
    print a.islower()       # True, 所有字符都是小写
    a = "ABCDE"
    print a.isupper()       # True, 所有字符都是大写
    a = "Abcde"
    print a.istitle()       # True, 所有单词都是首字母大写，像标题
    a = "\t"
    print a.isspace()       # True, 所有字符都是空白字符、\t、\n、\r
    arr = (1, 2.1, -3, -4.5, '123a', 'abc', 'aBC', 'Abc', 'ABC', '\t')
    for a in arr:
        print a,isNum(a)
    '''
    1 True
    2.1 True
    -3 True
    -4.5 True
    123a False
    abc False
    aBC False
    Abc False
    ABC False
        False
    '''
    for a in arr:
        print a,isNum2(a)
    '''
    1 True
    2.1 True
    -3 True
    -4.5 True
    123a False
    abc False
    aBC False
    Abc False
    ABC False
        False
    '''
```
这样更准确一些，适用性也更广。但如果你已经确信没有正负号，使用字符串的isdigit()方法则更为方便。

Python 字符串操作，如string替换、删除、截取、复制、连接、比较、查找、包含、大小写转换、分割等


```python
def test2():
    # 复制字符串 
    str1 = "ithomer.net"
    str2 = str1
    str1 = "blog"
    print str1, str2        # blog ithomer.net
    # 连接字符串
    str1 = "ithomer.net"
    str2 = "blog."
    str2 += str1
    print str1, str2        # ithomer.net blog.ithomer.net
    
    # 查找字符
    str1 = "ithomer.net"
    str2 = ".net"
    pos = str1.index(str2)
    print pos               # 7
    # 比较字符串
    str1 = "blog.ithomer.net"
    str2 = "forum.ithomer.net"
    pos = cmp(str1, str2)
    print pos               # -1
    # 是否包含指定的字符
    str1 = "blog.ithomer.net"
    str2 = ".ithomer."
    pos = len(str1 and str2)
    print pos               # 9
    # 字符串长度
    str1 = "blog.ithomer.net"
    pos = len(str1)
    print pos               # 16
    # 字符串大小写转换
    str1 = "blog.ithomer.net"
    str2 = "BLOG.ithomer.NET"
    print str1.upper()      # BLOG.ITHOMER.NET
    print str2.lower()      # blog.ithomer.net
    # 追加指定长度的字符串
    str1 = "blog.ithomer.net"
    str2 = "1234567"
    n = 3
    str1 += str2[0:n]
    print str1,str2         # blog.ithomer.net123 1234567
    # 字符串指定长度比较
    str1 = "blog.ithomer.net"
    str2 = "blog.ithomer.NET"
    n = 11
    print cmp(str1[0:n], str2[0:n])     # 0
    # 复制指定长度的字符
    str1 = "blog.ithomer.net"
    str2 = ""
    n = 12
    print str1[0:n]         # blog.ithomer
    # 将字符串前n个字符替换为指定的字符
    str1 = "blog.ithomer.net"
    ch = 'r'
    n = 3
    print n*ch + str1       # rrrblog.ithomer.net
    # 扫描字符串 
    sStr = 'cekjgdklab'   
    sStr1 = 'gka'   
    nPos = -1 
    for c in sStr1: 
        print c
    print nPos   
    # 翻转字符串 
    str1 = "ithomer.net"
    str2 = str1[::-1]
    print str1,str2         # ithomer.net ten.remohti
    # 查找字符串
    str1 = "ithomer.net"
    str2 = ".net"
    pos = str1.find(str2)
    print pos               # 7
    
    # 分割字符串
    str1 = "blog.ithomer.net"
    str2 = "."
    str3 = str1[str1.find(str2) + 1:]
    print str3              # ithomer.net
    print str1.split(str2)  # ['blog', 'ithomer', 'net']
    # 连接字符串
    str1 = ['blog', 'ithomer', 'net']
    str2 = "."
    str3 = str2.join(str1)
    print str3              # blog.ithomer.net
    # 只显示字母与数字
    str1 = "521.ithomer.NET $ @ # ! 1314"
    fmt = "abcdefghijklmnopqrstuvwxyz0123456789"
    for c in str1:
        if not c in fmt:
            str1 = str1.replace(c,'')
    print str1              # 521ithomer1314
```


**Python unicode字节串转成中文问题**
**Example 1:**
>>> s = r"\u65b0\u6d6a\u5fae\u535a\u6ce8\u518c"
>>> s
'\\u65b0\\u6d6a\\u5fae\\u535a\\u6ce8\\u518c'
>>> print s
\u65b0\u6d6a\u5fae\u535a\u6ce8\u518c
>>> s = s.decode("unicode_escape");     \#就是这个函数
>>> print s
新浪微博注册

**Example 2:**
>>> str_ = "Russopho\xe9bic, clichd and just pl\xe9ain stupid."
>>> print str_
Russopho?bic, clichd and just pl?ain stupid.
>>> str_ = str_.decode("unicode_escape")
>>> print str_
Russophoébic, clichd and just pléain stupid.


**参考推荐：**
[Python字符串操作](http://www.pythonclub.org/python-basic/string)
[Python difflib](http://www.pythonclub.org/python-basic/difflib)
[python unicode字节串转成中文问题](http://windkeepblow.blog.163.com/blog/static/1914883312013988185783/)




