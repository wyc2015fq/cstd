
# Python实现求解一元二次方程的方法示例 - jiahaowanhao的博客 - CSDN博客


2018年07月09日 21:53:15[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：724标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python实现求解一元二次方程的方法示例](http://cda.pinggu.org/view/26045.html)
本文实例讲述了Python实现求解一元二次方程的方法。分享给大家供大家参考，具体如下：
1. 引入math包
2. 定义返回的对象
3. 判断b*b-4ac的大小
具体计算代码如下：
\# -*- coding:utf-8 -*-
\#! python2
import math
class Result:
result1 = 0
result2 = 0
def __init__(self, r1, r2):
self.result1 = r1
self.result2 = r2
def __return__(self):
return Result(self.result1, self.result2)
def main(a, b, c):
num = b*b-4*a*c
if num < 0:
return 'no result'
elif num == 0:
return Result((-b+math.sqrt(num))/(2*a), (-b+math.sqrt(num))/(2*a))
else:
return Result((-b+math.sqrt(num))/(2*a), (-b-math.sqrt(num))/(2*a))
if __name__ == '__main__':
result = main(1, 2, 1)
print result.result1, result.result2
运行结果：
-1.0 -1.0

