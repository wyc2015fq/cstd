# python cookbook 1.4 找到最大或者最小的N个元素 heapq - 别说话写代码的博客 - CSDN博客





2019年01月07日 19:14:07[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：19标签：[找到最大或者最小的N个元素																[python cookbook																[heapq																[python heap](https://so.csdn.net/so/search/s.do?q=python heap&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=heapq&t=blog)




```python
#1.4找到最大或最小的N个元素，堆
#heapq模块中两个函数  nlargest()和nsmallest()
import heapq
nums = [5,12,6,2,7,3,-1,-6,66]
print(heapq.nlargest(3,nums))  #最大的3个元素
print(heapq.nsmallest(3,nums))   #最小的3个元素
```

![](https://img-blog.csdnimg.cn/20190107191225340.png)

```python
#这两个函数都可以接受参数key,从而可以工作在更复杂的数据结构上，按key排序
portfolio = [
    {'name':'IBM','shares':55,'price':91.1},
    {'name':'apple','shares':100,'price':543.1},
    {'name':'ms','shares':60,'price':31.1},
    {'name':'oracle','shares':200,'price':111.1},
    {'name':'google','shares':77,'price':61.1}
]
cheap = heapq.nsmallest(3,portfolio,key=lambda s:s['price'])  #按price升序排列
expensive = heapq.nlargest(3,portfolio,key=lambda s:s['price'])  #按price降序排列
print(cheap)
print(expensive)
```

![](https://img-blog.csdnimg.cn/20190107191250938.png)

```python
#堆最重要的特性heap[0]总是最小那个元素（小顶堆）
import heapq
nums = [5,12,6,2,7,3,-1,-6,66]
heap = list(nums)
heapq.heapify(heap)
print(heap)   #按照小顶堆的特性排列
print(heap[0])   #获取堆顶也就是最小值
```

![](https://img-blog.csdnimg.cn/20190107191318918.png)

```python
heapq.heappop(heap)
print(heap)
heapq.heappop(heap)
print(heap)
heapq.heappop(heap)
print(heap)
```

![](https://img-blog.csdnimg.cn/2019010719134147.png)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)](https://so.csdn.net/so/search/s.do?q=找到最大或者最小的N个元素&t=blog)




