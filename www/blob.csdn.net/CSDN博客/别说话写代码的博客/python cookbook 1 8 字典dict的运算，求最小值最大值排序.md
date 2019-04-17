# python cookbook 1.8 字典dict的运算，求最小值最大值排序 - 别说话写代码的博客 - CSDN博客





2019年01月08日 11:18:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：32标签：[python cookbook																[python 字典运算 最大值最小值排序																[dict运算](https://so.csdn.net/so/search/s.do?q=dict运算&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python 字典运算 最大值最小值排序&t=blog)




```python
#1.8字典的运算   在数据字典中进行一些运算   求最大最小或者排序等
d = {   #假设数据为股票名称和价格的映射
    'ms':50,
    'apple':80,
    'google':25,
    'face':100,
    'twt':76
}
minprice=min(zip(d.values(),d.keys()))   #使用zip()将key和value翻转过来，zip的作用是将两对象中元素打包成一个元素
maxprice=max(zip(d.values(),d.keys()))   #min/max对元组求，先比较第一个元素，然后比较第二各元素
print(minprice,maxprice)
pricesort=sorted(zip(d.values(),d.keys()))   #排序
print(pricesort)
```

![](https://img-blog.csdnimg.cn/20190108111618401.png)

```python
#注意，zip创建的是一个只能访问一次的迭代器
keyvalue = zip(d.values(),d.keys())
print(min(keyvalue))   #只能访问一次
print(max(keyvalue))    #出错
```

![](https://img-blog.csdnimg.cn/20190108111642112.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```python
#通常对dict进行数学操作，一般只作用于它的key
print(min(d))
print(max(d))
print(sorted(d))  

print(min(d.values()))  #这样可以对value操作，但是若想得到它的key又得进行查找
print(max(d.values()))
print(max(d,key=lambda x:d[x] ))  #这样可以获得最大value对应key，使用内部的参数定义规则
print(sorted(d.values()))
```

![](https://img-blog.csdnimg.cn/20190108111710357.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```python
#这种情况一般使用zip()将key和value先反转，然后进行一个数学操作。zip()形成一个对应的元组
a=[1,2,3,4]
b=[5,6,7,8]  #将a,b合并成dict
c=dict(zip(a,b))
print(c)
#当多个实体有相同的value时，键key会决定返回结果。比如求max，value相同，返回key较大的。
a={
    'a':4,
    'b':4
}
print(min(zip(a.values(),a.keys())))   #value相同，key会变成最后结果
print(max(zip(a.values(),a.keys())))
```

![](https://img-blog.csdnimg.cn/20190108111731281.png)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)




