# python cookbook 1.19同时对数据做转换和换算 - 别说话写代码的博客 - CSDN博客





2019年01月15日 12:51:25[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28标签：[同时对数据做转换和换算																[python cookbook](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)](https://so.csdn.net/so/search/s.do?q=同时对数据做转换和换算&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```python
#1.19同时对数据做转换和换算
nums = [1,2,43,4,56]
s = sum(x*x for x in nums)
print(s)

# import os
# files = os.listdir('dirname')  #当前目录下
# if any(name.endswith('.py') for name in files):    #any返回是否为空对象，空返回true
#     print('have python')
# else:
#     print('no python')
    
s = ('ABC',50,123.1)
print(','.join(str(x) for x in s))

portfolio = [
{'name':'GOOG', 'shares': 50},
{'name':'YHOO', 'shares': 75},
{'name':'AOL', 'shares': 20},
{'name':'SCOX', 'shares': 65}
]
min_shares = min(s['shares'] for s in portfolio)   #拿出来shares最小的
print(min_shares)
```

![](https://img-blog.csdnimg.cn/20190115125048783.png)

```python
print(sum((x*x for x in nums)))
print(sum(x*x for x in nums))   #和上面那种一样，生成器表达式
print(sum([x*x for x in nums]))   #这样只是适用于销量数据

print(min(s['shares'] for s in portfolio))   #这样返回的是一个值
print(min(portfolio , key = lambda s:s['shares']))    #这样使用key参数返回的值和原来类型相同
```

![](https://img-blog.csdnimg.cn/20190115125106916.png)






