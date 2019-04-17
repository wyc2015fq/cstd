# python if for 多种写法 - 别说话写代码的博客 - CSDN博客





2019年01月03日 14:53:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：115
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









**对于if来说**

1.常规写法

```python
if a>b:
    c=a
else :
    c=b
```

2.表达式，类似于c/c++中的c=a>b?a:b

```python
c=a if a>b else b
```

3.列表

```python
c = [b,a][a>b]   #不满足取b,满足取a
```

**对于for来说**

1.常规写法

```python
for i in range(10):
    print(i)
```

2.加上判断或其他操作

[对(x)的操作 for x in 集合 if 条件]

[对(x,y)的操作 for x in 集合1 for y in 集合2 if 条件]

举个例子：

x=[1,2,3,4]

y=[5,6,7,8]

使两个list中的偶数分别相加，应该结果是2+6,4+6,2+8,4+8

```python
[a + b for a in x for b in y if a%2 == 0 and b%2 ==0]
```

参考：[https://blog.csdn.net/zl87758539/article/details/51675628](https://blog.csdn.net/zl87758539/article/details/51675628)



