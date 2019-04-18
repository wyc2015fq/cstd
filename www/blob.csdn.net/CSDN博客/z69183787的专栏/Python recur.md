# Python recur - z69183787的专栏 - CSDN博客
2016年08月11日 10:32:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：334
个人分类：[其他语言-Python](https://blog.csdn.net/z69183787/article/category/6359977)
```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 利用递归函数计算阶乘
# N! = 1 * 2 * 3 * ... * N
def fact(n):
    if n == 1:
        return 1
    return n * fact(n-1)
print('fact(1) =', fact(1))
print('fact(5) =', fact(5))
print('fact(10) =', fact(10))
# 利用递归函数移动汉诺塔:
def move(n, a, b, c):
    if n == 1:
        print('move', a, '-->', c)
        return
    move(n-1, a, c, b)
    print('move', a, '-->', c)
    move(n-1, b, a, c)
move(4, 'A', 'B', 'C')
```
