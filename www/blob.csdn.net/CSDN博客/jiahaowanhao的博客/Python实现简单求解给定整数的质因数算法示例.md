
# Python实现简单求解给定整数的质因数算法示例 - jiahaowanhao的博客 - CSDN博客


2018年04月19日 21:19:50[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：87


[Python实现简单求解给定整数的质因数算法示例](http://cda.pinggu.org/view/25313.html)
本文实例讲述了Python实现简单求解给定整数的质因数算法。分享给大家供大家参考，具体如下：
接着做题遇到求解质因数分解的问题，思想很简单，就是需要遍历从1到该整数本身，并且判断当数字为质数时加入列表最后输出即可,求解这样的一个正整数的质因数分解，关键在于理解，每次得到一个质因数之后需要更新整数为：原始整数除以这个质因数的值，循环直至原始整数的值小于2终止，输出结果即可，实现如下：
\#!usr/bin/env python
\#encoding:utf-8
'''''
__Author__:沂水寒城
功能：求解整数的质因数分解
'''
num=int(raw_input())
def get_num_factors(num):
list0=[]
tmp=2
if num==tmp:
print num
else:
while (num>=tmp):
k=num%tmp
if( k == 0):
list0.append(str(tmp))
num=num/tmp \#更新
else:
tmp=tmp+1 \#同时更新除数值，不必每次都从头开始
print ' '.join(list0)+' '
结果如下：
90
2 3 3 5
180
2 2 3 3 5


