# python cookbook 1.7 字典dict排序 OrderedDict - 别说话写代码的博客 - CSDN博客





2019年01月08日 11:15:04[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：28标签：[python dict排序																[OrderedDict](https://so.csdn.net/so/search/s.do?q=OrderedDict&t=blog)](https://so.csdn.net/so/search/s.do?q=python dict排序&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```python
#1.7字典排序.想创建一个字典，在迭代或者序列化时能够控制元素的顺序.可以使用collections模块中的OrderedDict()
#在迭代操作时它会保持元素被插入的顺序
from collections import OrderedDict
d=OrderedDict()
d['hello']=1
d['world']=2
d['lei']=3
d['da']=4
d['da']=5
print(d)
#当你想构建其他格式编码时，OrderedDict通常非常有用
```

![](https://img-blog.csdnimg.cn/20190108111346266.png)

#OrderedDict内部维护着一个按插入顺序**有序的双向链表**。插入元素时，在链表尾部添加一个节点。对一个已经存在的key重复赋值不会改变其顺序

#需要注意其**空间大小一般是dict大小的2倍**，因为它内部维护着另外一个链表。所以处理大数据时，考虑内存开销和时间开销的平衡




