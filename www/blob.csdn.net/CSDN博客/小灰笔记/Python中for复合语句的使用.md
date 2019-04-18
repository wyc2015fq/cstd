# Python中for复合语句的使用 - 小灰笔记 - CSDN博客





2017年06月05日 23:16:25[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1943








       当Python中用到双重for循环设计的时候我一般会使用循环的嵌套，但是在Python中其实还存在另一种技巧——for复合语句。

       简单写一个小程序，用于延时循环嵌套功能如下：

#!/usr/bin/python



defFunc1(ten_num,one_num):

       for i in range(ten_num):

              for j in range(one_num):

                     print(10 * i + j)

Func1(2,5)的执行结果如下：

0

1

2

3

4

10

11

12

13

14

       以上是一个数字组合运算的结果清单。接下来使用for的复合语句实现类似功能，增加扩充代码如下：

#!/usr/bin/python



defFunc1(ten_num,one_num):

       for i in range(ten_num):

              for j in range(one_num):

                     print(10 * i + j)



defFunc2(ten_num,one_num):

       print([(10 * i + j) 

              for i in range(ten_num) 

                     for j in range(one_num)])



#Func1(2,5)

Func2(2,5)

       程序的执行结果如下：

[0, 1, 2, 3, 4,10, 11, 12, 13, 14]

       这里以列表的形式存储了生成的各种数字组合结果并进行了打印。程序实现的功能类似，但是从上面的代码以及结果来看，for复合语句还是有其自己的特点的。具体如下：

       1，从代码上看，复合语句的形式代码更为凝练；

       2，从阅读上看，复合语句其实在表述上更加贴近于英语语法；

       3，在实现结果的矩阵化上，复合语句更有优势。



