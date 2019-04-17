# python字典(dict)相加，相同key相加，不同key保留 - 别说话写代码的博客 - CSDN博客





2019年01月03日 00:40:48[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：480标签：[python dict相加																[python字典相加](https://so.csdn.net/so/search/s.do?q=python字典相加&t=blog)](https://so.csdn.net/so/search/s.do?q=python dict相加&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








**字典 相加：**

假如 两个字典dict1={'a':1,'b':2,'c':3},dict2={'c':4,'d':5}，若两个dict1和dict2有相同的key则对应的value相加，若没有则直接添加过来。结果为dict3={'a':1,'b':2,**'c':7,'d':5**}

```python
def merge_dict(x,y):
    for k,v in x.items():
                if k in y.keys():
                    y[k] += v
                else:
                    y[k] = v
```

底下这个是所有元素相加

```
x={'a':1,'b':2,'c':3}
y={'c':4,'d':5}
from collections import Counter
X,Y=Counter(x),Counter(y)
z=dict(X+Y)
print(z)
```

![](https://img-blog.csdnimg.cn/20190325142536606.png)




