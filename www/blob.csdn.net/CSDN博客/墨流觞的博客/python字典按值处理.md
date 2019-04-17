# python字典按值处理 - 墨流觞的博客 - CSDN博客





2017年03月31日 21:50:08[墨氲](https://me.csdn.net/dss_dssssd)阅读数：337








# 在字典上进行按值处理

在字典数据上按字典值进行各种计算，最大最小值，排序等 

1.使用zip()函数取出值和键，组成元组的zip对象（迭代器），在进行相应计算 

2.使用插入关键字参数(key=)的min()，max(), sorted()函数
```python
prices = {
    'ACME': 45.32,
    'AAPL': 612.78,
    'IBM': 205.55,
    'HPQ': 37.20,
    'FB': 10.75,
    'DSS': 10.75
}

sorted_prices = sorted(zip(prices.values(), prices.keys()))
 # print(sorted_prices) #注意先按元组第一个值排序，在按第二个值排序
min_prices = zip(prices.values(), prices.keys())


tu = zip(prices.values(), prices.keys())
print(tu)
for i in tu:
    print(i)
# min_prices = min(tu) # error：min() arg is an empty sequence zip()   函数返回的迭代器只能使用一次

# 直接对字典使用min() max() sorted() 是对字典的键排序

min(prices)  # AAPL
print(sorted(prices))  # ['AAPL', 'ACME', 'FB', 'HPQ', 'IBM']

# 传入关键字参数，返回值最小的键
min_k = min(prices, key=lambda k: prices[k])
print(min_k, prices[min_k])
```




