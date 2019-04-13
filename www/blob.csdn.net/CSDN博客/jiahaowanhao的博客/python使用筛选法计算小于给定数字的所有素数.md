
# python使用筛选法计算小于给定数字的所有素数 - jiahaowanhao的博客 - CSDN博客


2018年04月12日 13:09:50[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1478


**python使用筛选法计算小于给定数字的所有素数**
本文实例为大家分享了python计算小于给定数字的所有素数的具体代码，供大家参考，具体内容如下
代码思路：首先列出指定范围内所有候选数字，然后从前往后依次选择一个数字去除以后面所有数字，能够被整除的肯定不是素数，把这些数字过滤掉，然后重复这个过程，直到选择的除数大于最大数字的平方根为止。代码主要演示内置函数filter()和切片的用法，实际上这个算法的效率并不是很高。
def primes2(maxNumber):
'''筛选法获取小于maxNumber的所有素数'''
\#待判断整数
lst = list(range(3, maxNumber, 2))
\#最大整数的平方根
m = int(maxNumber**0.5)
for index in range(m):
current = lst[index]
\#如果当前数字已大于最大整数的平方根，结束判断
if current > m:
break
\#对该位置之后的元素进行过滤
lst[index+1:] = list(
filter(
lambda x: 0 if not x%current else x,
lst[index+1:]))
\#2也是素数
return [2] + lst
以上就是本文的全部内容，希望对大家的学习有所帮助

