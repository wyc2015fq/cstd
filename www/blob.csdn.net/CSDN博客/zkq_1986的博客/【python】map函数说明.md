# 【python】map函数说明 - zkq_1986的博客 - CSDN博客





2018年12月11日 17:58:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：55








map()是 Python 内置的高阶函数

例如：

```
def f(x):
    return x*x
print(*map(f, [1, 2, 3, 4, 5, 6, 7, 8, 9]))
```

输出结果：
1 4 9 16 25 36 49 64 81




