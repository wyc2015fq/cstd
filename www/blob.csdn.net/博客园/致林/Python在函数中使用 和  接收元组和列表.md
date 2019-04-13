
# Python在函数中使用-和--接收元组和列表 - 致林 - 博客园






# [Python在函数中使用*和**接收元组和列表](https://www.cnblogs.com/bincoding/p/7944860.html)
当要使函数**接收元组或字典形式的参数**的时候，有一种特殊的方法，它**分别使用*和**前缀**。这种方法在函数需要**获取可变数量的参数**的时候特别有用。
**[注意]**
[1] 由于在args变量前有***前缀**，所有多余的函数参数都会**作为一个元组存储在args中**。如果使用的是****前缀**，多余的参数则会被认为是**一个字典的健/值对**。
[2] 对于def func(*args):，*args表示把传进来的位置参数存储在tuple（元组）args里面。例如，调用**func(1, 2, 3)**，**args就表示(1, 2, 3)这个元组**。
[3] 对于def func(**args):，**args表示把参数作为字典的健-值对存储在dict（字典）args里面。例如，调用**func(a='I', b='am', c='wcdj')**，**args就表示{'a':'I', 'b':'am', 'c':'wcdj'}这个字典**。
[4] 注意普通参数与*和**参数公用的情况，一般将*和**参数放在参数列表最后。
**[元组的情形]**
\#! /usr/bin/python\#Filename: tuple_function.py\#2010-7-19 wcdjdefpowersum(power, *args):'''Return the sum of each argument raised
to specified power.'''total=0foriinargs:
        total+=pow(i,power)returntotalprint'powersum(2, 3, 4)==', powersum(2, 3, 4)print'powersum(2, 10)==', powersum(2, 10)\#\#\#\#\#\#\#\#\#output\#\#\#\#\#\#\#\#powersum(2, 3, 4)==25powersum(2, 10)==100


**[字典的情形]**
\#! /usr/bin/python\#Filename: dict_function.py\#2010-7-19 wcdjdeffindad(username, **args):'''find address by dictionary'''print'Hello:', usernameforname, addressinargs.items():print'Contact %s at %s'%(name, address)
findad('wcdj', gerry='gerry@byteofpython.info', /wcdj='wcdj@126.com', yj='yj@gmail.com'


在gvim中的输出结果：
![gvim中的输出结果](http://hi.csdn.net/attachment/201007/19/45214_1279524186pXo8.jpg)
http://blog.csdn.net/delphiwcdj/article/details/5746560





