# 学习python--基础语法 - wydbyxr的博客 - CSDN博客
2017年03月06日 21:13:52[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：208
# Python 标识符
以双下划线开头的（**foo）代表类的私有成员；以双下划线开头和结尾的（__foo**）代表python里特殊方法专用的标识，如**init**（）代表类的构造函数。
# 行和缩进
```php
#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：test.py
 if True:
    print "Answer"
    print "True"
else:
    print "Answer"
    # 没有严格缩进，在执行时会报错
  print "False"
```
# 多行语句
Python语句中一般以新行作为为语句的结束符。
但是我们可以使用斜杠（ \）将一行的语句分为多行显示。 
语句中包含[], {} 或 () 括号就不需要使用多行连接符。如下实例：
```bash
days = ['Monday', 'Tuesday', 'Wednesday',
        'Thursday', 'Friday']
```
# Python 引号
Python 可以使用引号( ’ )、双引号( ” )、三引号( ”’ 或 “”” ) 来表示字符串，引号的开始与结束必须的相同类型的。
其中三引号可以由多行组成，编写多行文本的快捷语法，常用语文档字符串，在文件的特定地点，被当做注释。
```
word = 'word'
sentence = "这是一个句子。"
paragraph = """这是一个段落。
包含了多个语句""
```
python 中多行注释使用三个单引号(”’)或三个双引号(“”“)。
# Python空行
书写时不插入空行，Python解释器运行也不会出错。但是空行的作用在于分隔两段不同功能或含义的代码，便于日后代码的维护或重构。
记住：空行也是程序代码的一部分。
# 同一行显示多条语句
Python可以在同一行中使用多条语句，语句之间使用分号(;)分割， 
以下是一个简单的实例：
```
#!/usr/bin/python
import sys; x = 'runoob'; sys.stdout.write(x + '\n')
```
