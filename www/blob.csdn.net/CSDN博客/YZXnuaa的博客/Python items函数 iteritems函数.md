# Python items函数 iteritems函数 - YZXnuaa的博客 - CSDN博客
2018年01月30日 17:44:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：919
items函数，将一个字典以列表的形式返回，因为字典是无序的，所以返回的列表也是无序的。
```
a = {'a':1,'b':3}
a.items()
返回a = [('a',1),('b',3)]
```
iteritems（)返回一个迭代器函数
![复制代码](https://common.cnblogs.com/images/copycode.gif)
b = a.iteritems()
list(b) =[('a',1),('b',3)]
for k,v in b:     
    print k,v
返回a 1
      b 3
