# python cookbook 1.17从字典中提取子集，创建一个字典是另一个字典的子集 - 别说话写代码的博客 - CSDN博客





2019年01月15日 12:48:24[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：52
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.17从字典中提取子集，创建一个字典是另一个字典的子集
prices = {
'ACME': 45.23,
'AAPL': 612.78,
'IBM': 205.55,
'HPQ': 37.20,
'FB': 10.75
}  
p1 = {k:v for k,v in prices.items() if v > 200}    #选出符合条件的字典子集
print(p1)    
tech_name = {'AAPL','IBM','HPQ','MSFT'}
p2 = {k:v for k,v in prices.items()  if k in tech_name}   #选出符合条件的子集
print(p2)
```

![](https://img-blog.csdnimg.cn/20190115124735980.png)

```python
#可以用字典推导式解决的问题也可通过创建元组序列然后将他们传给dict()函数来完成
p1 = dict((k,v) for k,v in prices.items() if v > 200)   #这样快很多
tech_name = {'AAPL','IBM','HPQ','MSFT'}
p2 = {k:prices[k] for k in prices.keys()  &  tech_name}   #同时完成两件事
print(p1)
print(p2)
```

![](https://img-blog.csdnimg.cn/20190115124754831.png)





