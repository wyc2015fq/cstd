# 使用Python实现质因式分解算法 - 小灰笔记 - CSDN博客





2017年05月07日 21:39:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：7535








      本来想实现一个其它的基本数学算法问题，但是发现在实现之前必须得先完成分解质因式的算法。

      没有去网上寻找什么比较好的求解算法，纯粹是按照自己的理解写了一个试探性求解算法。也就是在MIT公开课中教授提到的guess-try方法。代码实现如下：

#!/usr/bin/python



def PrimeNum(num):

      r_value =[]

      for i inrange(2,num+1):

            for jin range(2,i):

                  ifi % j == 0:

                        break

            else:

                  r_value.append(i)

      returnr_value



def PrimeFactorSolve(num,prime_list):

      for n inprime_list:

            ifnum % n == 0:

                  return[n,num / n]





def PrimeDivisor(num):

      prime_range= PrimeNum(num)

      ret_vale =[]

      while numnot in prime_range:

            factor_list= PrimeFactorSolve(num,prime_range)

            ret_vale.append(factor_list[0])

            num =factor_list[1]

      else:

            ret_vale.append(num)

      printret_vale



PrimeDivisor(120)



      程序执行结果如下：

E:\WorkSpace\01_编程语言\03_Python\python_exp07>pythonpython_exp07.py

[2, 2, 2, 3, 5]

      抽取测试了其他的数据，从简单的例子计算看，结果还是准确的。



