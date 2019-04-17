# Python中的join函数用法 - 小灰笔记 - CSDN博客





2017年08月12日 15:32:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3233








       Python中的join跟Perl中的join在功能上有一点相似。Python中，join用于实现序列或者集合的合并，而且可以指定合并时候元素之间的间隔符号。此外，还能够将字符串作为一个对象对每个可切片元素进行合并。

       先看一下对字符串的合并。

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python

Python 2.7.6(default, Oct 26 2016, 20:30:19)

[GCC 4.8.4] onlinux2

Type"help", "copyright", "credits" or"license" for more information.

>>> str1= "life is short,you need python"

>>> a ='_'.join(str1)

>>> a

'l_i_f_e_ _i_s__s_h_o_r_t_,_y_o_u_ _n_e_e_d_ _p_y_t_h_o_n'

>>>type(a)

<type 'str'>

       从上面的交互记录可以看出，字符串进行相应的处理之后能够时间分隔符的插入。同时，看到一个比较有意思的东西，那就是运行环境。我是在Windows10中的bash下执行的，软件的版本是py2。但是，我自己Windows系统中安装的软件其实是py3。从这里看，其实Windows10中的bash安装其实是给安装了一个比较完整的Linux子系统的。有了这个东西的话，以后的Windows使用体验应该会更好一些了！

       再来看看对元组的操作，这次直接写代码文件吧！代码如下：

cell_data =('I','Love','Coding')



var =','.join(cell_data)

print(var)

print(type(var))

       程序执行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python join.py

I,Love,Coding

<type 'str'>

       从上面的结果可以看出，合并成功。合并后的对象是一个字符串。但是，元组的元素却不一定都是字符串，为了能够更充分试探出这个函数的功能，代码修改如下：

cell_data =('I','Love','Coding',1,2,3)



var =','.join(cell_data)

print(var)

print(type(var))

       程序的执行结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python join.py

Traceback (mostrecent call last):

  File "join.py", line 3, in<module>

    var = ','.join(cell_data)

TypeError:sequence item 3: expected string, int found

       这里果真出错了，期待的是一个字符串但是参数却是整型。不用说，换用列表之后也肯定是类似的效果。修改代码测试：

list_data =['I','Love','Coding',1,2,3]



var =','.join(list_data)

print(var)

print(type(var))

       程序运行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python join.py

Traceback (mostrecent call last):

  File "join.py", line 10, in<module>

    var = ','.join(list_data)

TypeError:sequence item 3: expected string, int found

       继续修改如下：

list_data =['I','Love','Coding']



var =','.join(list_data)

print(var)

print(type(var))

       程序运行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python join.py

I,Love,Coding

<type 'str'>

       进一步修改如下：

list_data =[1,2,3,4]



var =','.join(list_data)

print(var)

print(type(var))

       程序运行结果：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/OOP/2017/08/12$python join.py

Traceback (mostrecent call last):

  File "join.py", line 10, in<module>

    var = ','.join(list_data)

TypeError:sequence item 0: expected string, int found

       由此，基本上可以确定join的功能就是面向字符串处理的一个功能。其实，使用一般的Python基础功能也能够实现一个类似的功能函数，或许还能够进行一些其他功能特性的定制。不过，既然已经有了这个现成的函数，能够直接使用也是很好的。



