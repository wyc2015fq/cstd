# python cookbook 1.15根据字段将记录分组，将一系列字典或对象实例，根据某个特定字段分组迭代数据 - 别说话写代码的博客 - CSDN博客





2019年01月14日 22:56:47[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：109标签：[python cookbook																[python根据字段分组																[将字典分组](https://so.csdn.net/so/search/s.do?q=将字典分组&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python根据字段分组&t=blog)




```python
#1.15根据字段将记录分组，将一系列字典或对象实例，根据某个特定字段分组迭代数据
rows = [   #itertools.groupby()函数可以对数据进行分组
{'address': '5412 N CLARK', 'date': '07/01/2012'},
{'address': '5148 N CLARK', 'date': '07/04/2012'},
{'address': '5800 E 58TH', 'date': '07/02/2012'},
{'address': '2122 N CLARK', 'date': '07/03/2012'},
{'address': '5645 N RAVENSWOOD', 'date': '07/02/2012'},
{'address': '1060 W ADDISON', 'date': '07/02/2012'},
{'address': '4801 N BROADWAY', 'date': '07/01/2012'},
{'address': '1039 W GRANVILLE', 'date': '07/04/2012'},
]
#现在想根据日期以分组方式迭代数据，首先对目标字段排序，然后使用itertools.groupby()
from operator import itemgetter
from itertools import groupby

rows.sort(key=itemgetter('date'))
print(rows)
#使用groupby扫描序列找出拥有相同值的序列项，并将它们分组。返回一个值和一个子迭代器
#groupby只检查连续项，必须先对项排序
for data,items in groupby(rows,key=itemgetter('date')):
    print(data)
    for i in items:
        print(' ',i)
```

![](https://img-blog.csdnimg.cn/20190114225535706.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```python
#若只是想分组，按照日期相同的元素为一组，就利用defaultdict构建一个一键多值得字典
from collections import defaultdict
rows_by_date = defaultdict(list)
for row in rows:
    rows_by_date[row['date']].append(row)
print(rows_by_date)
for r in rows_by_date['07/01/2012']:
    print(r)    #输出这个时间的所有字典值
```

![](https://img-blog.csdnimg.cn/20190114225607121.png)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)




