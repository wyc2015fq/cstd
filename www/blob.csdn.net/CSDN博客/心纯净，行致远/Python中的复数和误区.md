# Python中的复数和误区 - 心纯净，行致远 - CSDN博客





2019年02月22日 20:33:36[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：491








### 1 Python中的数据类型有复数类型：real+imagj

eg:1+2j等等，使用的时候发现了这个问题：

```python
>>> 1+2j.real
1.0
>>> 1+2j.imag
3.0
>>> 1+(2j).imag
3.0
>>> a=1+2j
>>> a.real
1.0
>>> a.imag
2.0
>>> 1+2j
(1+2j)
>>> (1+2j).imag
2.0
>>> 1+2j.imag
3.0
```

说明如果复数类型不加括号，会把复数的实数部分和虚数部分的数字相加作为虚部，因此写复数一定要注意写成（a+bj）的形式，括号不能不写。
|属性|描述|
|----|----|
|num.real|该复数的实数部分|
|num.imag|该复数的虚数部分|
|num.conjugat()|返回该复数的共轭算数|

### 2 complex（）函数

complex（）函数用于创建一个复数或者将一个数或字符串转换为复数形式，其返回值为一个复数。该函数的语法为：

> 
class complex(real,imag)


其中，real可以为int、long、float或字符串类型；而image只能为int、long、或float类型。
- 注意：如果第一个参数为字符串，第二个参数必须省略，若第一个参数为其他类型，则第二个参数可以选择。实例：



![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 >>> 1 + 1j
 2 (1+1j)
 3 >>> complex(1) #数字
 4 (1+0j)
 5 >>> complex(1,2)
 6 (1+2j)
 7 >>> complex("1") #当做字符串处理
 8 (1+0j)
 9 complex("1 + 2j") #会出错，+号两边不能有空格，否则会报错
10 >>> complex("1+2.0j")
11 (1+2j)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
- 第一个参数为字符串，还添加第二个参数时会报错：

>>> complex("x",15)
TypeError: complex() can't take second arg if first is a string




