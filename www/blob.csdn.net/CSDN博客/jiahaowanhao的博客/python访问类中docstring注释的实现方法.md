
# python访问类中docstring注释的实现方法 - jiahaowanhao的博客 - CSDN博客


2018年04月04日 21:51:51[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：473


[python访问类中docstring注释的实现方法](http://cda.pinggu.org/view/25179.html)
本文实例讲述了python访问类中docstring注释的实现方法。分享给大家供大家参考。具体分析如下：
python的类注释是可以通过代码访问的，这样非常利于书写说明文档
class Foo:
pass
class Bar:
"""Representation of a Bar"""
pass
assert Foo.__doc__ == None
assert Bar.__doc__ == "Representation of a Bar"
希望本文所述对大家的Python程序设计有所帮助。

