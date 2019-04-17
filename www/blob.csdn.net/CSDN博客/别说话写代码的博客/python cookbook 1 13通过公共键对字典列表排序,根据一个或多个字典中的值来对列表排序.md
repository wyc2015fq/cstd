# python cookbook 1.13通过公共键对字典列表排序,根据一个或多个字典中的值来对列表排序 - 别说话写代码的博客 - CSDN博客





2019年01月12日 15:15:34[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：37标签：[根据一个或多个字典中的值来对列表排序																[通过公共键对字典列表排序																[python cookbook](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=通过公共键对字典列表排序&t=blog)




```python
#1.13通过公共键对字典列表排序,根据一个或多个字典中的值来对列表排序
#利用operator中的itemgetter函数对这类结构排序很简单
rows = [
{'fname': 'Brian', 'lname': 'Jones', 'uid': 1003},
{'fname': 'David', 'lname': 'Beazley', 'uid': 1002},
{'fname': 'John', 'lname': 'Cleese', 'uid': 1001},
{'fname': 'Big', 'lname': 'Jones', 'uid': 1004}
]

from operator import itemgetter
rows_by_name = sorted(rows,key=itemgetter('fname'))   #按照fname的大小对字典进行排序
rows_by_uid = sorted(rows,key=itemgetter('uid'))    #按照uid 对字典进行排序
print(rows_by_name)
print(rows_by_uid)

#itemgetter()函数可以接受多个键
rows_by_lfname = sorted(rows,key=itemgetter('lname','fname'))   #按照lname,fname同时排序
print(rows_by_lfname)
```

![](https://img-blog.csdnimg.cn/20190112151415760.png)

```python
#也可以自定义，用lambda函数取代itemgetter()函数的功能
rows_by_name = sorted(rows,key=lambda x:x['fname'])   #按照fname的值排序
print(rows_by_name)
rows_by_lfname = sorted(rows,key=lambda x:(x['lname'],x['fname']))
print(rows_by_lfname)
```

![](https://img-blog.csdnimg.cn/20190112151437964.png)

```python
#itemgetter()方法同样适用于min(),max()这样的方法
print(min(rows,key=itemgetter('uid')))
print(max(rows,key=itemgetter('uid')))
```

![](https://img-blog.csdnimg.cn/20190112151506889.png)](https://so.csdn.net/so/search/s.do?q=根据一个或多个字典中的值来对列表排序&t=blog)




