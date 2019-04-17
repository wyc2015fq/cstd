# python cookbook 1.1 python将序列分解为单独的变量 - 别说话写代码的博客 - CSDN博客





2019年01月07日 09:41:09[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.1将序列分解为单独的变量
p=(1,2)
x,y = p
print(x,y)

data = ['ABC',50,100.0,(2019,1,7)]
name,shares,price,data = data
print(name,shares,price,data)
```

![](https://img-blog.csdnimg.cn/2019010709393810.png)

不仅是元组或者列表，只要对象恰好是可迭代的，就可以执行分解操作。字符串，文件，迭代器以及生成器。

```python
s='hello'
a,b,c,d,e = s
print(a,b,c,d,e)
```

![](https://img-blog.csdnimg.cn/20190107094030938.png)



