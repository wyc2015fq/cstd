# python字典合并 - 别说话写代码的博客 - CSDN博客





2019年01月03日 14:45:13[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：47
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









方法一：

```python
a={'a':1,'b':2}
b={'a':3,'c':4}
a.update(b)
```

![](https://img-blog.csdnimg.cn/20190103144416580.png)

方法二：

```python
a={'a':1,'b':2}
b={'a':3,'c':4}
dict(a,**b)
```

![](https://img-blog.csdnimg.cn/2019010314434487.png)



