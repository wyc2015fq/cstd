# Python学习之路一--（基础知识） - BlueSky - CSDN博客
2015年10月23日 16:52:34[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：440
## 基本知识
Python有四种数据类型–整数、长整数、浮点数、和复数。 
print不想换行的话在后面加个,  如：print ‘asia’, 
**空白**在Python中是重要的。事实上行首的空白是重要的。它称为**缩进**。在逻辑行首的空白（空格和制表符）用来决定逻辑行的缩进层次，从而用来决定语句的分组。这意味着同一层次的语句必须有相同的缩进。每一组这样的语句称为一个块。 
**if语句**在结尾处包含一个冒号——我们通过它告诉Python下面跟着一个语句块。elif和else从句都必须在逻辑行结尾处有一个冒号，下面跟着一个相应的语句块（当然还包括正确的缩进）。在Python中没有switch语句。你可以使用if..elif..else语句来完成同样的工作（在某些场合，使用**字典**会更加快捷。） 
记住，你可以在**while循环**中使用一个else从句。
```
while running:
    guess = int(raw_input('Enter an integer : '))
    if guess == number:
        print 'Congratulations, you guessed it.' 
        running = False # this causes the while loop to stop
    elif guess < number:
        print 'No, it is a little higher than that' 
    else:
        print 'No, it is a little lower than that' 
else:
    print 'The while loop is over.' 
    # Do anything else you want to do here
```
## **for循环**
for..in是另外一个循环语句，它在一序列的对象上 递归 即逐一使用队列中的每个项目。 
在C/C++中，如果你想要写for (int i = 0; i < 5; i++)，那么用Python，你写成for i in range(0,5)。你会注意到，Python的for循环更加简单、明白、不易出错。
```
for i in range(1, 5):
    print i
else:
    print 'The for loop is over'
```
## **break语句**
break语句是用来 终止 循环语句的，即哪怕循环条件没有称为False或序列还没有被完全递归，也停止执行循环语句。 
*一个重要的注释是，如果你从for或while循环中 终止 ，任何对应的循环else块将不执行*
## **continue**
continue语句用来告诉Python跳过当前循环块中的剩余语句，然后进行下一轮循环。
```php
while True:
    s = raw_input('Enter something : ')
    if s == 'quit':
        break
    if len(s) < 3:
        continue
    print 'Input is of sufficient length'
    # Do other kinds of processing here...
```
如果len(s)<3时，继续执行while循环，而下面的那句print则不打印，即跳过当前循环块的剩余的语句，继续执行下一个循环
## **函数**
函数是重用的程序段。它允许你给一块语句一个名称，然后你可以在你的程序的任何地方使用这个名称任意多次的运行这个语句块。这被称为*调用*函数。 
定义函数如下：
```python
def sayHello():
    print "Hello world!"  #the block belonging to the function
#调用的时候直接如下
sayHello()
```
## **使用global语句**
如果为定义在函数外的变量赋值，那么就得告诉Python这个变量名不是局部的，而是全局的。这就需要用global来完成这一功能，就相当于全局变量。
## **默认参数值**
函数中参数是可选的，如果用户不想为这些参数提供值的话，这些参数就使用默认参数值完成。可以在函数定义的形参后加上赋值运算符(=)和默认值，从而给形参指定默认参数值。如：
```python
def say(message, times = 1):
    print message * times
say('Hello')
say('World', 5)
```
输出则为： 
```
Hello 
WorldWorldWorldWorldWorld
```
调用say(“Hello”)时因为就一个参数所以使用默认的参数即1；而第二次调用传入了参数5则利用传入的值。
## **重要**
只有在形参表末尾的那些参数可以有默认参数值，即你不能在声明函数形参的时候，先声明有默认值的形参而后声明没有默认值的形参。这是因为赋给形参的值是根据位置而赋值的。例如，def func(a, b=5)是有效的，但是def func(a=5, b)是 无效 的。
## **模块**
如果想要在其他程序中重用很多函数，那么可以考虑使用模块。模块基本上就是一个包含了所有你定义的函数和变量的文件。为了在其他程序中重用模块，模块的文件名必须以.py为扩展名。模块也可以从其他程序引入以便利用它的功能。如：
```python
import sys  #表明引入sys模块，告诉python想使用这个模块，sys模块包含了与python解释器和它的环境有关的函数。sys为system的缩写。
print sys.argv  #打印参数
```
## **from .. import 语句**
如果你想要直接输入argv变量到你的程序中（避免在每次使用它时打sys.），那么你可以使用from sys import argv语句。如果你想要输入所有sys模块使用的名字，那么你可以使用from sys import *语句。这对于所有模块都适用。一般说来，应该避免使用from..import而使用import语句，因为这样可以使你的程序更加易读，也可以避免名称的冲突。 
**模块的*_name_***#双下划线 
**name** 就相当于对每个模块起了个名字，比如**name** == ‘**main**‘则这个模块可以加一些测试语句，而别的模块引用的时候则不执行被引用的模块中测试部分的代码。 
**制作自己的模块**#模块就相当于函数库 
创建一个.py程序，则它的文件名则为模块被引入的名字，同事这个模块应被放置在我们输入它的程序的同一个目录中或者sys.path所列的目录之一。比如：
```python
#!/usr/bin/python
# Filename: mymodule.py
def sayhi():
    print 'Hi, this is mymodule speaking.'
version = '0.1'
# End of mymodule.py
```
```python
#!/usr/bin/python
# Filename: mymodule_demo.py
import mymodule
mymodule.sayhi()
print 'Version', mymodule.version
```
输出为：
```
Hi, this is mymodule speaking.
Version 0.1
```
```python
#!/usr/bin/python
# Filename: mymodule_demo2.py
from mymodule import sayhi, version
# Alternative:
# from mymodule import *
利用from .. import 
sayhi()
print 'Version', version
```
## **dir（）函数**
内建的dir()函数用来列出模块定义的标识符（包括函数、类、变量）。当为dir()提供一个模块名的时候，它返回模块定义的名称列表，如果不提供参数，它返回当前模块中定义的名称列表。
```bash
dir(sys) #将把sys内的变量 函数 类 显示出来，相当于c中的.h文件
```
## **数据结构**
python有三种内建的数据结构： 
 1. 列表 
 2. 元组 
 3. 字典
需要记住的只是如果你想要复制一个列表或者类似的序列或者其他复杂的对象（不是如整数那样的简单 对象 ），那么你必须使用切片操作符来取得拷贝。如果你只是想要使用另一个变量名，两个名称都 参考 同一个对象，那么如果你不小心的话，可能会引来各种麻烦。即： 
如果想复制一个对象需要用切片来操作，直接复制只是起了个别名指向的是同一块的内存地址，修改的时候会都改变。 
time.strftime(‘%Y%m%d%H%M%S’) #表示当前时间的数字组合； 
字符串join方法把source列表转换为字符串； 
比如：
```bash
source = ['hello','world']
' '.join(source) #即表示 hello world (中间用空格连接起来)
```
‘C:\Documents’或r’C:\Documents’等价于’C:\Documents’ 在windows下； 
os.sep变量的用法——这会根据你的操作系统给出目录分隔符，即在Linux、Unix下它是’/’，在Windows下它是’\’，而在Mac OS下它是’:’。使用os.sep而非直接使用字符，会使我们的程序具有移植性，可以在上述这些系统下工作。
`target = '/home/asia' + os.sep + 'log' + '.zip' #linux即为/home/asia/log.zip`
## **软件开发过程**
一般有以下环节： 
什么（分析）->如何（设计）->编写（实施）->测试（测试与调试）->使用（实施或开发）->维护（优化）
