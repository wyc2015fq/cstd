# python cookbook 1.18将名称映射到序列的元素中 - 别说话写代码的博客 - CSDN博客





2019年01月15日 12:50:12[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.18将名称映射到序列的元素中
#collections.namedtuple()命名元组，返回标准元组类型的子类。我们提供类型名称以及相应字段，它返回一个可实例化的类，为你已定义好字段传入值
from collections import namedtuple
subscrber = namedtuple('Subscrber',['addr','joined'])
sub = subscrber('rara@123.com','2013-10-19')
print(sub)
print(sub.addr,sub.joined)
print(len(sub),sub[0])   #支持普通元组的操作，索引分解等
a,b = sub
print(a,b)
```

![](https://img-blog.csdnimg.cn/20190115124851690.png)

```python
#使用命名元组使得程序有更好的扩展性
def computecost(records):
    total =0.0
    for r in records:
        total  += r[1]*r[2]   #这样虽然程序没问题但是可扩展性不强
    return total

from collections import namedtuple
stock = namedtuple('Stack',['name','shares','price'])   #这样可读性可扩展性都很好
def computecost(records):
    total = 0.0
    for r in records:
        r=stock(*r)     #将r可变参数赋给stock
        total += r.shares * r.price    #这样可读性好很多
    return total
```

```python
#namedtuple可作为字典的替代，其比字典高效，但namedtuple是不可变的
s= stock('aaa',10,11)
print(s)
#s.shares = 20   #不可变，报错

#若要修改任何属性，可通过namedtuple实例的_replace()方法实现,该方法会创建一个全新的命名元组，并对相应的值做替换
s = s._replace(shares=75)   #就可以改了
print(s)
```

![](https://img-blog.csdnimg.cn/20190115124929868.png)

```python
from  collections import namedtuple
stock = namedtuple('Stock',['name','shares','price','date','time'])
stock_prototype = stock('',0,0,None,None)
def dict2stock(s):    #自定义一个新建功能
    return stock_prototype._replace(**s)   #*a是将0个或多个参数以list传进来，而**s是将0个或多个参数以dict传进来，接受x=0,y=6这种

a ={'name':'nnn','shares':20,'price':10.1}
print(dict2stock(a))
b = {'name':'eee','shares':10,'price':111,'date':33}
print(dict2stock(b))
print(stock_prototype)
```

![](https://img-blog.csdnimg.cn/20190115124948530.png)





