# Python入门--13--递归 - 默槑 - 博客园







# [Python入门--13--递归](https://www.cnblogs.com/modaidai/p/6877491.html)





什么是递归：

有调用函数自身的行为

有一个正确的返回条件



设置递归的深度：

import sys

sys.setrecursionlimit(10000)　　　　#可以递归一万次



用普通的方法也就是非递归版本编写一个阶乘的程序：

def jiecheng(n):
    result = n
    for i in range(1,n):
        result *= i
    return result

number = int(input("请输入一个整数："))
result=jiecheng(10)

print ("%d 的阶乘是 : %d" %(number,result))

用递归的方法编写阶乘的程序：

![](https://images2015.cnblogs.com/blog/1038110/201705/1038110-20170522165232851-823096072.png)

把程序写出来就是：

def qiujiecheng(n):

　　if n==1:

　　　　return 1

　　else:

　　　　return n * qiujiecheng(n-1)

number = int(input="请输入一个数字：")

result = qiujiecheng(number)

print ("%d 的阶乘是 %d"%(number,result))


















