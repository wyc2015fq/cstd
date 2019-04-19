# python函数默认参数作用域 - _天枢 - 博客园
## [python函数默认参数作用域](https://www.cnblogs.com/yhleng/p/9561207.html)
2018-08-30 16:55 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9561207)
当def函数参数默认值为对象时，例如列表[],字典{}
示例1：猜测一下，会输出什么？？？  
```
def ddd(a,b=[]):
    b.append(a)
    return b
print(ddd(1))
print(ddd(2,['a','b','c']))
print(ddd(3))
```
[1]  ['a','b','c','2'] [3]   ?????你是否认为是这样？ 输出一下看看
输出结果：
```
D:\Python27\python.exe D:/untitled1/temp5.py
[1]
['a', 'b', 'c', 2]
[1, 3]
```
看到以上结果有什么想法，为什么呢？？？[1, 3]而不是[3],那么修改一下代码，输出b变量的内存地址看一下。我想，可以找到我要的答案
```
def ddd(a,b=[]):
    b.append(a)
    print(id(b)) #查看内存地址
    return b
print(ddd(1))
print(ddd(2,['a','b','c']))
print(ddd(3))
```
输出结果：
```
D:\Python27\python.exe D:/untitled1/temp5.py
170580872
[1]
171586568
['a', 'b', 'c', 2]
170580872
[1, 3]
```
从输出中，可以看出，除了，第二个print，其它两个内存是一们的。那么考虑一下为什么第二个为被改变。结果显而易见，因为第二个print改变了，b对象，重新创建了一个对象。
那么如果不想出现，每次调用，默认值对象不变的问题，可以这样改一下：
```
def ddd(a,b=None):
    if type(b).__name__!='list':
        b = []
        
    b.append(a)
    print(id(b)) #查看内存地址
    return b
print(ddd(1))
print(ddd(2,['a','b','c']))
print(ddd(3))
```
初始值设置为None，通过类型判断来，进行内处理
小结：
       1.python定义def的时候，如果参数做为对象，那么，如果没有改变这个对象的前提下，那么即使，多次调用该函数也，不会创建新的对象。
          那么就会出现多次调用函数，有可能都进行了处理。
　　2.如果，没有特殊需求，函数参数，尽量不采用对象，如果一有需要，对象进行内处理。传默认值None
