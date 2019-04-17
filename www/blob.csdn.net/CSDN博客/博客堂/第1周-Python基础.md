# 第1周-Python基础 - 博客堂 - CSDN博客





2017年05月30日 19:02:12[最小森林](https://me.csdn.net/u012052268)阅读数：670








这个博客系列是coursera上的公开课《用Python玩转数据》的学习笔记。希望可以学到有用的数据分析知识。

# Python基础



- [Python基础](#python基础)- [1走进Python](#11走进python)- [Python简介](#1-python简介)
- [Hello World](#2-hello-world)
- [Python基础语法](#3-python基础语法)
- [Python数据类型](#4-python数据类型)
- [Python基本运算](#5-python基本运算)
- [python的模块函数](#6python的模块函数)

- [2 more about Python](#12-more-about-python)- [条件if](#1条件if)
- [range](#2range)
- [循环loops](#3循环loops)
- [breakcontinue](#4breakcontinue)
- [自建函数](#5自建函数)
- [递归](#6递归)
- [变量作用域](#7变量作用域)






## 1.1走进Python

### 1. Python简介

Python程序设计语言的设 

计哲学是 简单 明确 优雅。

Python非常吸引人的一大功能 就是它具有极其便捷强大的数据统计和处理能力 这也是本课程重点要介绍的内容 除此之外呢 Python还有其他很多方面的应用 

### 2. Hello World

Python 文件的之行方式： 

1. shell的输入输出方式 

2. 如果文件较长，保存为.py格式执行
可用的IDE： 

1. Ipython、QT、IDLE等交互式控制台 

2. Spyder等可视化工具 
### 3. Python基础语法

Python都是变量，常量可以通过特殊方法实现。 

不要用下划线明明变量，那是Python中的内建标识符（私有变量）。

Python是一种动态的强类型语言 它不需要做显式声明 那类型怎么确定呢 类型是根据值来确定的 它是以一种称为引用的方式 来实现赋值 在Python中 所有的赋值都是 将一个引用指向相应的内存单元

### 4. Python数据类型

Python中标准的数据类型有 整型 浮点型 复数型 布尔型 字符串 列表 元组和字典 我们这节先对 整型 浮点型 复数型 布尔型以及字符串作一个介绍 列表 元组和字典 等到我们后面的章节再给大家来作介绍 

### 5. Python基本运算

平方**；余数%；除/；整除（地板除）//

在Python中还有一类比较特殊的运算 称为是字符运算 ：字符运算主要是用两个操作符 一个是r 一个u。它的功能就是 把相应的字符串转成unicode的字符串 而r呢 它称为原始字符串操作符。

```python
r呢 它称为原始字符串操作符,用于不希望转义的地方。下面两个语句相同。

f = open('c:\\oython\\text.txt','w')
f = open(r'c:\oython\text.txt','w')
```

运算优先级：
- 算数运算符 +
- 位运算符 <<
- 比较运算符 <
- 逻辑运算符 not

### 6.python的模块、函数

一个完整的py文件就是一个模块（库）。可以通过 import来引入模块。

```
import math
math.pi
```

包：有层次的文件目录结构。一般有子包和py文件组成

## 1.2 more about Python

### 1.条件if

if 和 elif 和 else 别忘记加冒号。

### 2.range

range（5） 会打印0 到4

range(1,5) 会打印1到4

range(1,5,2)会打印1，3 

注意：返回的是列表

### 3.循环loops

python有while循环与for循环

其中for很强大，可以循环任何东西（字符串、列表、元组、字典、文件）。

### 4.break、continue

continue终止当前的一层，继续下一步循环

break终止整个循环

### 5.自建函数

```python
def fun_name():
    11111l
    111111
```

### 6.递归

### 7.变量作用域

一句话，定义在函数中的为局部变量；

定义在类 或者 代码主体中的变量为全局变量。





