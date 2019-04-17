# python cookbook 1.20将多个映射和并为单个映射，将多个字典合并为一个字典 - 别说话写代码的博客 - CSDN博客





2019年01月15日 12:53:01[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：24
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```python
#1.20将多个映射和并为单个映射，将多个字典合并为一个字典
a = {'x':1,'z':3}
b = {'y':2,'z':4}

from collections import ChainMap
c = ChainMap(a,b)    #只是简单维护一个记录底层映射关系的列表，若有重复键，以第一个映射中所对应的值，修改操作都在第一个dict上执行
print(c) 
print(c['x'],c['y'],c['z']) #c['z']是a中的元素值
c['z'] = 10   #修改第一个dict中的值
c['w'] = 40   #给第一个dict中添加
del c['x']
print(c)
#del c['y']   #不能在第二个元素上执行
```

![](https://img-blog.csdnimg.cn/20190115125157670.png)

```python
a = {'x':1,'z':3}
b = {'y':2,'z':4}
merge = dict(b)
merge.update(a)   #merge是一个新字典，重复值为更新的值
print(merge)
```

![](https://img-blog.csdnimg.cn/20190115125232879.png)



