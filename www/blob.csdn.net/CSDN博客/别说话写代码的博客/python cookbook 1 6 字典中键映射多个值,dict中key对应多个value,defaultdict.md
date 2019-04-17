# python cookbook 1.6 字典中键映射多个值,dict中key对应多个value,defaultdict - 别说话写代码的博客 - CSDN博客





2019年01月08日 10:30:18[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：47标签：[python dict一个key对应多个value																[字典中键映射多个值																[defaultdict](https://so.csdn.net/so/search/s.do?q=defaultdict&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=字典中键映射多个值&t=blog)




```python
#1.6字典中键映射多个值，也就是一个key对应多个value，即multidict
#dict就是一个key 对应一个value，你可以把value改成list或者set
d={
    'a':[1,2,3],
    'b':[4,5]
}
e={
    'a':{1,2,3},
    'b':{4,5}
}
#用list或set取决于你的需求，若要保证插入有序，使用list。若想去掉重复元素使用set，(并不关心元素顺序问题)
```

```python
#你可以使用collections中的defaultdict来构造这样的字典。defaultdict会自动初始化每个key对应的value，所以只需向其中添加元素
from collections import defaultdict
d = defaultdict(list)  #初始的每个key 对应的value为list
d['a'].append(1)
d['a'].append(2)
d['b'].append(3)
print(d)

s = defaultdict(set)  #初始的每个key对应的value为set
s['a'].add(1)
s['a'].add(1)
s['a'].add(2)
s['b'].add(3)
print(s)
```

![](https://img-blog.csdnimg.cn/20190108102756625.png)

```python
#注意，defaultdict会自动为要访问的key（哪怕当前不存在）创建映射实体,比如下面例子并没有d['c']，但它会创建一个空的'c':[]
print(d['c'])
print(d)

print(s['c'])
print(s)
```

![](https://img-blog.csdnimg.cn/20190108102823305.png)

```python
#如果你不需要这样的特性，你可以在普通的字典上使用setdefault()方法代替
d={}
d.setdefault('a',[]).append(1)
d.setdefault('a',[]).append(1)
d.setdefault('a',[]).append(2)
d.setdefault('b',[])  #对应的value为空
d.setdefault('c',[]).append(3)
print(d)
```

![](https://img-blog.csdnimg.cn/20190108102848879.png)

```python
#自己实现一个多值映射字典
d={}
pair=[(1,2),(3,4),(5,6)]
for k,v in pair:
    if k not in d:
        d[k]=[]
    d[k]=v
print(d)
```

![](https://img-blog.csdnimg.cn/20190108102917909.png)

```python
#若使用defaultdict就很简单了
d=defaultdict(list)
for k,v in pair:
    d[k].append(v)
print(d)
```

![](https://img-blog.csdnimg.cn/20190108102938366.png)](https://so.csdn.net/so/search/s.do?q=python dict一个key对应多个value&t=blog)




