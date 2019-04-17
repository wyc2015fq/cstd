# python cookbook 1.10从序列中移除重复项且保持元素间顺序不变 - 别说话写代码的博客 - CSDN博客





2019年01月12日 11:40:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：62标签：[python cookbook																[python set																[python 元素去重且相对位置不变](https://so.csdn.net/so/search/s.do?q=python 元素去重且相对位置不变&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python set&t=blog)




```python
#1.10从序列中一处重复项且保持元素间顺序不变
def dedupe(items):      
    seen = set()    #若序列中的值是可哈希的（不可变，整数浮点数字符串元组），那么这是可通过集合和生成器解决
    for item in items:
        if item not in seen:
            yield item   #每次返回一个不再seen中的元素，其实就相当于返回给了调用外面的list，list是有插入顺序关系的`       
            seen.add(item)
        
a = [1,2,6,4,1,2,9,4,9]
print(list(dedupe(a)))
```

![](https://img-blog.csdnimg.cn/20190112113838523.png)

```python
#在不可哈希对象中去除重复项，稍作修改即可
def dedupe(items,key=None):   #key的作用是指定一个函数用来将序列中元素转换为可哈希类型，这么做目的是为了检测重读对象
    seen = set()
    for item in items:
        val = item if item is None else key(item)
        if val not in seen:
            yield item
            seen.add(val)
            
a = [{'x':1,'y':2},{'x':1,'y':3},{'x':1,'y':2},{'x':2,'y':4}]
print(list(dedupe(a,key=lambda d:(d['x'],d['y']))))   #d['x']相等且d['y']相等
print(list(dedupe(a,key=lambda d:d['x'])))    #只按d['x']进行筛选
```

![](https://img-blog.csdnimg.cn/20190112113905325.png)

```python
#若只是为了去除重复项，不考虑元素顺序的话，可以直接用set()
a = [11,1,2,6,4,1,2,9,4,9,7,8,3,3,2,1]
b="231431432564362434262"
print(set(a))
print(set(b))   #输出无序，不能保证元素间的顺序不变，得到的结果会被打乱。前面方法可避免这个问题
```

![](https://img-blog.csdnimg.cn/20190112113931707.png)

```python
#集合的特点就是集合中的元素都是唯一的，但不保证他们之间的顺序
a=set()
a.add(10)
a.add(1)
a.add(5)
print(a)
```

![](https://img-blog.csdnimg.cn/20190112113953977.png)

```python
#若想读一个文件，去除其中重复文本行，可这样
with open(somefile,'r') as f:
    for line in dedupe:
        ...
```](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)




