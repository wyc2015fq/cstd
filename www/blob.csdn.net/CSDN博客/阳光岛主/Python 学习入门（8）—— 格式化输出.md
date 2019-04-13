
# Python 学习入门（8）—— 格式化输出 - 阳光岛主 - CSDN博客

2013年11月19日 22:33:04[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5516个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



python中也有类似于c中的printf()的格式输出标记，在python格式化使用的是%运算符，示例如下：

```python
﻿#!/usr/bin/env python
# it-homer in 2013
import sys
reload(sys)
sys.setdefaultencoding("utf-8")

# %s 格式化为字符串
from math import pi
def test_format():
  str = "hello %s, http://blog.%s.net"
  val = ("it-homer", "ithomer")
  print str % val               # hello it-homer, http://blog.ithomer.net
  print("%s %s %s" % (1, 2.3, ['one', 'two', 'three']))         # 1 2.3 ['one', 'two', 'three']
  print("PI: %.10f" % pi)       # PI: 3.1415926536

  # -：左对齐; +：在转换值之前加上正负号; 
  # “ ”：正数之前保留空格; 0：转换值若位数不够用0填充
  print("PI: %010.2f" % pi)     # PI: 0000003.14
  print("PI: %010.6f" % pi)     # PI: 003.141593
  print("PI: %-10.2f" % pi)     # PI: 3.14
  print("PI: %-10.6f" % pi)     # PI: 3.141593

  print("PI: %10.2f" % pi)     # PI:       3.14
  print("PI: %10.6f" % pi)     # PI:   3.141593
  print("PI: %010.2f" % pi)     # PI: 0000003.14
  print("PI: % 10.2f" % pi)     # PI:       3.14
  
  print("PI: %+10.2f" % pi)     # PI:      +3.14
  print("PI: %-+10.2f" % pi)    # PI: +3.14
  print("PI: %+-10.2f" % pi)    # PI: +3.14

# 模板字符串
from string import Template
def test_format2():
  s = Template('$x, hello $x')
  f = s.substitute(x = 'ithomer')
  print(f)              # ithomer, hello ithomer
  
  # 单词字母替换
  s = Template("It's ${x}homer blog")
  f = s.substitute(x='it')
  print(f)              # It's ithomer blog

  # 用$$插入一个$符号
  s = Template("It's $$ blog $x")
  f = s.substitute(x='ithomer')
  print(f)              # It's $ blog ithomer

  s = Template('A $name go to $blog')
  d = {}
  d['name'] = 'sunboy'
  d['blog'] = 'blog.ithomer.net'
  f = s.substitute(d)
  print(f)              # A sunboy go to blog.ithomer.net

def test_format3():
  num = 20
  # 十进制
  print("%d" % num)     # 20
  print("%u" % num)     # 20
  # 八进制
  print("%o" % num)     # 24
  # 十六进制
  print("%x" % num)     # 14
  print("%X" % num)     # 14
  # 浮点数 
  print("%f" % num)     # 20.000000
  # 科学计数法
  print("%e" % num)     # 2.000000e+01
  print("%E" % num)     # 2.000000E+01
  print("%g" % num)     # 20

def test_format4():
  print("'ithomer'")    # 'ithomer'
  print("\'ithomer")    # 'ithomer
  print("\"ithomer\"")  # "ithomer"
  print("\\ithomer\\")  # \ithomer\
  print("/ithomer/")    # /ithomer/
  print("aa\nbb")       # aa next line bb
  print("aa\bbb")       # abb
  print("aa\rbb")       # bb
  print("aa\tbb")       # aa    bb
  print("aa\000bb")     # aabb

if __name__ == "__main__":
  test_format()
  test_format2()
  test_format3()
  test_format4()
```

**Python格式化字符串的替代符以及含义**
|符   号|说     明|
|%c|格式化字符及其ASCII码|
|%s|格式化字符串|
|%d|格式化整数|
|%u|格式化无符号整型|
|%o|格式化无符号八进制数|
|%x|格式化无符号十六进制数|
|%X|格式化无符号十六进制数（大写）|
|%f|格式化浮点数字，可指定小数点后的精度|
|%e|用科学计数法格式化浮点数|
|%E|作用同%e，用科学计数法格式化浮点数|
|%g|根据值的大小决定使用%f活%e|
|%G|作用同%g，根据值的大小决定使用%f活%e|
|%p|用十六进制数格式化变量的地址|
**Python的转义字符及其含义**
|符    号|说     明|
|\'|单引号|
|\"|双引号|
|\a|发出系统响铃声|
|\b|退格符|
|\n|换行符|
|\t|横向制表符|
|\v|纵向制表符|
|\r|回车符|
|\f|换页符|
|\o|八进制数代表的字符|
|\x|十六进制数代表的字符|
|\000|终止符，\000后的字符串全部忽略|
如果在字符串中输出"\",需使用"\\"

