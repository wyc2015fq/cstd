# 【Python学习系列六】Python语言基本语法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月08日 11:03:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2320
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









                
1. Python语言简介


Python是一种面向对象的解释型计算机程序设计语言，由Guido van Rossum于1989年发明，第一个公开发行版发行于1991年。

Python是纯粹的自由软件，它的源代码和解释器CPython遵循 GPL（GNU General Public License）协议。

Python和C语言不一样，它是一种脚本语言。C语言在写完源代码后是需要编译成二级制代码才能够执行的；Python则不用，它在生产环境中出现仍旧是源代码的.py文件形式，在执行的瞬间才由Python解释器将源代码转换为字节码，然后再由Python解释器来执行这些字节码。

这种形式的好处是不用考虑平台系统的问题，可以和Java语言一样“一次编写到处执行”。缺点也是显而易见的，就是每次进行字节码转换和字节码执行没有直接执行二进制的效率高。好在对于执行效率苛刻的场合毕竟较少，另外随着计算机硬件能力的提升，执行效率的矛盾也变得不明显了。

和其他计算机语言一样，Python语言也有自己的一套语法基础。有顺序、分支、循环、调用的程序组织结构，以及数字、字符串、列表、元组、集合等多种数据类型。



2.  Hello World

Python的Hello World与别的计算机语言没什么区别，而且更加简洁，可以直接在交互式编程环境中写：


```python
print ("Hello, Python!");
```


3. 行与缩进

Python脚本文件和普通的文本文件没有太大区别，一般写成.py作为后缀的方式。

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：test.py


if True:
    print "True"
else:
print "False"
```



其中#为注释标记，如果在一行中使用#，那么#后的内容是不会被解释执行的。

下面的if和else是分支型语句，当if后的内容为True（真实），则执行if所辖的部分，否则执行else所辖的部分。

注意Python语言中是不用begin/end或{}来表示执行段落的起止的，这里的if和else需要左侧对齐，用缩进来表示段落所辖范围界限。



4.  变量类型

Python语言中标准的数据类型有这样几种：

Numbers（数字）、String（字符串）、List（列表）、Tuple（元组）、Dictionary（字典）。


```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


counter = 100 # 赋值整型变量
miles = 1000.0 # 浮点型
name = "John" # 字符串


print counter
print miles
print name
```



这段代码演示了，整数型数字、浮点型数字以及字符串类型的赋值和打印操作。


```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


counter = 100 # 赋值整型变量
miles = 1000.0 # 浮点型
name = "John" # 字符串


print counter
print miles
print name
```


这段代码演示的是列表类型的操作，列表很像Java语言中的数组，只不过列表允许不同类型的数据放在同一个列表中，而数组不可以——它只能要求所有的元素类型一致。


```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


tuple = ( 'abcd', 786 , 2.23, 'john', 70.2 )
tinytuple = (123, 'john')


print tuple # 输出完整元组
print tuple[0] # 输出元组的第一个元素
print tuple[1:3] # 输出第二个至第三个的元素
print tuple[2:] # 输出从第三个开始至列表末尾的所有元素
print tinytuple * 2 # 输出元组两次
print tuple + tinytuple # 打印组合的元组
```


这段代码演示的是元组类型的操作。操作方法和列表很像，但是Python语法不允许对元组中的元素进行二次赋值。它相当于是只读类型的列表。


```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


dict = {}
dict['one'] = "This is one"
dict[2] = "This is two"


tinydict = {'name': 'john','code':6734, 'dept': 'sales'}


print dict['one'] # 输出键为'one' 的值
print dict[2] # 输出键为 2 的值
print tinydict # 输出完整的字典
print tinydict.keys() # 输出所有键
print tinydict.values() # 输出所有值
```


这段代码演示的是字典类型的操作。字典类型有些像Java中的HashMap，是通过主键Key来访问对于的Value值，而不是靠下标来访问。




5.  循环语句

```python
#!/usr/bin/python


count = 0
while (count < 9):
   print 'The count is:', count
   count = count + 1


print "Good bye!"
```



这段代码演示的是while循环，while循环后面的条件表示在满足条件的时候执行while所辖的程序段。在这段程序中表示count<9的情况下，执行下面的两行语句，不包括：

print "Good bye!"这一行。


```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


for num in range(10,20):  # 迭代 10 到 20 之间的数字
   for i in range(2,num): # 根据因子迭代
      if num%i == 0:      # 确定第一个因子
         j=num/i          # 计算第二个因子
         print '%d 等于 %d * %d' % (num,i,j)
         break            # 跳出当前循环
   else:                  # 循环的 else 部分
      print num, '是一个质数'
```



上面这段程序略显繁琐，不过内容仍然很简单。

这是循环的另一种写法——for循环，for循环也是一种循环，后面写出的是一个循环范围。这里是一个二重循环，也就是两个循环发生了嵌套——在一个循环的执行中有另一个循环。外层循环是让num在10和20之间做循环，内层循环是i在2和num之间做循环。




6.  函数

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


# 定义函数
def printme( str ):
   "打印任何传入的字符串"
   print str;
   return;


# 调用函数
printme("我要调用用户自定义函数!");
printme("再次调用同一函数");
```



函数是一种最小单位的代码段封装。关键字是def，def后面的printme是函数名，str是参数名称。这个函数的内容就是直接打印传入的变量值。

下面最后两句是对函数的调用。




7.  模块

```python
#!/usr/bin/python
# -*- coding: UTF-8 -*-


# 导入模块
import support


# 现在可以调用模块里包含的函数了
support.print_func("Zara")
```



模块是一种大单位的代码段集合，例如有一个support.py的文件中有多个函数定义，其中一个叫做print_func函数。在我不对support.py这个模块进行引用的时候是不能调用print_func函数的。上面这段代码中，import support是导入support.py模块，下面的support.print_func("Zara")是调用support中的print函数，并传入变量"Zara"作为参数。

from fib import Fibonacci

这是导入模块的另一种写法，区别是它能够导入一个模块的一部分而非全部模块代码。示例中是指从fib这个模块中只导入Fibonacci这个函数。



8.  小结

以上就是Python语言中所涉及到的最基本的语法，有兴趣可以再找一些专业介绍Python的资料来学习，尤其是下面常用库：




1)numpy

NumPy系统是python的一种开源的数值计算扩展库。它提供了许多高级的数值编程工具，如：矩阵数据类型、矢量处理，以及精密的运算库。




2)matplotlib

一个专业的绘图工具库，官方网址在：http://matplotlib.org/，调用简单，使用非常方便，在配合python进行数据挖掘和报表制作的过程中是一种利器。




3)scipy

SciPy是一款方便、易于使用、专为科学和工程设计的python工具包，提供的内容很丰富，文件输入输出、特殊函数、线性代数运算、快速傅里叶变换、统计与随机、微分和积分、图像处理等诸多封装内容。官方网址在：http://www.scipy.org/。




4)Scikit-learn

Scikit-learn是最著名的python机器学习库之一，官方网址在：http://scikit-learn.org/stable/。




这四个库大致理解下，numpy可用于数值处理，在其上，就是scipy具体函数计算（基于数值），再其上，就是scikit-learn机器学习库（要用到numpy和scipy），matplotlib则是视图展示。学习路径上，以scikit-learn机器学习算法为主，可以一并用到其他三个库，其他三个库是基础。



