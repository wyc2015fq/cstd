# python对字典及列表递归排序 - _天枢 - 博客园
## [python对字典及列表递归排序](https://www.cnblogs.com/yhleng/p/9407895.html)
2018-08-02 16:24 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9407895)
对字典内所有内容进行排升序排序，包括，子数组，子字典
需要注意：
1.字典因为是在一定程序上无序的，所以这里采用了内置包，来变成有序字典
```
from collections import OrderedDict
```
实现代码：
 代码中，清除了值为[]的列表，与值为None的项
```
def sortDict(pyload):
    item = 'p.items()'
    if type(pyload).__name__=='list':
        p = sorted(pyload)
        item = 'enumerate(p)'
    if type(pyload).__name__ == 'dict':
        p=OrderedDict(sorted(pyload.items(),key=lambda a:a[0]))
    for k,v in eval(item):
        if type(v).__name__=='list':
            if not v or (v is None):
                p.pop(k)
            else:
                p[k]=list(sortDict(sorted(v)))
        elif type(v).__name__=='dict':
            if not v or (v is None):
                p.pop(k)
            else:
                p[k] =dict(sortDict(v))
            return p
        else:
            if v is None:
                p.pop(k)
    return p
listk = {'d':['a',1,4,3,8,2,'b',[4,3,2,1,[9,8,7,6]]],'b':1}
print sortDict(listk)
```
运行结果：
```
D:\Python27\python.exe D:/untitled4/tmp6.py
OrderedDict([('b', 1), ('d', [1, 2, 3, 4, 8, [1, 2, 3, 4, [6, 7, 8, 9]], 'a', 'b'])])
Process finished with exit code 0
```
注意：看着输出里边又是列表，又是元组的有点晕。其实这个你不用管的。直接当字典使用就好。
如，我要取key为b的值
```
OrderedDict('b')
```
