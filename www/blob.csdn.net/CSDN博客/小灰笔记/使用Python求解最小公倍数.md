# 使用Python求解最小公倍数 - 小灰笔记 - CSDN博客





2017年05月08日 19:45:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3837
个人分类：[Python																[数学](https://blog.csdn.net/grey_csdn/article/category/6890098)](https://blog.csdn.net/grey_csdn/article/category/6639459)








      简单分析了一下，最大公约数的求解方法跟最小公倍数的求解方法类似，只需要改一个简单的条件，然后做一点简单的其他计算。问题的解决也是基于分解质因式的程序。

      程序实现以及测试case代码如下：

#!/usr/bin/python



from collections import Counter



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

      num_temp =num

      prime_range= PrimeNum(num)

      ret_value =[]

      while numnot in prime_range:

            factor_list= PrimeFactorSolve(num,prime_range)

            ret_value.append(factor_list[0])

            num =factor_list[1]

      else:

            ret_value.append(num)

      returnCounter(ret_value)



def LeastCommonMultiple(num1,num2):

      dict1 =PrimeDivisor(num1)

      dict2 =PrimeDivisor(num2)

      least_common_multiple= 1

      for key indict1:

            ifkey in dict2:

                  ifdict1[key] > dict2[key]:

                        least_common_multiple*= (key ** dict1[key])

                  else:

                        least_common_multiple*= (key ** dict2[key])

      for key indict1:

            ifkey not in dict2:

                  least_common_multiple*= (key ** dict1[key])

      for key indict2:

            ifkey not in dict1:

                  least_common_multiple*= (key ** dict2[key])



      returnleast_common_multiple



print(LeastCommonMultiple(12,18))

print(LeastCommonMultiple(7,2))

print(LeastCommonMultiple(7,13))

print(LeastCommonMultiple(24,56))

print(LeastCommonMultiple(63,81))

      程序执行结果：

E:\WorkSpace\01_编程语言\03_Python\math>pythonleast_common_multiple.py

36

14

91

168

567

      通过验证，计算结果准确。




