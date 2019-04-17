# python cookbook 1.16筛选序列中的元素，列表推导式和生成器表达式 - 别说话写代码的博客 - CSDN博客





2019年01月14日 23:14:38[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24标签：[python cookbook																[筛选序列中的元素																[列表推导式和生成器表达式](https://so.csdn.net/so/search/s.do?q=列表推导式和生成器表达式&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=筛选序列中的元素&t=blog)




```python
#1.16筛选序列中的元素，列表推导式和生成器表达式
mylist = [1,2,4,-5,-5,-2,-7,9,1]
print([n for n in mylist if n>0])   #输出所有大于0的元素
pos = (n for n in mylist if n <0)   #若原始输入非常大，可以用生成器表达式通过迭代筛选结果
for x in pos:
    print(x)
```

![](https://img-blog.csdnimg.cn/20190114231303583.png)

```python
#若有的数据不规则，没法直接用到生成器或者列表推导式中，或涉及异常处理。基于此可将处理逻辑代码放到单独函数中，使用内建函数filter()处理
values = ['1','2','-4','-','6',5,'N/A','9']
def is_int(val):
    try:
        x = int(val)
        return True
    except ValueError:
        return False
    
ivals = list(filter(is_int,values))   #filter函数就是讲第二个参数每个值执行第一个函数，filter返回迭代器，要用列表形式需要加list()
print(ivals)
```

![](https://img-blog.csdnimg.cn/20190114231326359.png)

```python
mylist = [1,2,4,-5,-5,-2,-7,9,1]
import math
pt=[math.sqrt(n) for n in mylist if n >0]  
print(pt)
pr=[n if n>0 else 0 for n in mylist]   #将mylist中大于0的输出，小于0的置为0
print(pr)
```

![](https://img-blog.csdnimg.cn/20190114231345144.png)

```python
#itertools.compress() 筛选工具，它接受一个可迭代对象及一个布尔选择器作为输入
addresses = [
'5412 N CLARK',
'5148 N CLARK',
'5800 E 58TH',
'2122 N CLARK',
'5645 N RAVENSWOOD',
'1060 W ADDISON',
'4801 N BROADWAY',
'1039 W GRANVILLE',
]
counts = [ 0, 3, 10, 4, 1, 7, 6, 1]
#现在想构建一个地址列表，其中count值大于5

from itertools import compress
more5 = [n>5 for n in counts]
print(more5)
print(list(compress(addresses,more5)))   #输出count大于5对应的元素
```

![](https://img-blog.csdnimg.cn/20190114231405914.png)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)




