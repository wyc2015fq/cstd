# python利用递归函数输出嵌套列表的每个元素 - lincappu - 博客园







# [python利用递归函数输出嵌套列表的每个元素](https://www.cnblogs.com/lincappu/p/8146055.html)





　　1.先用 for 循环取。

```
for item in l:
    if isinstance(item ,list):
        for newitem in item:
            print(newitem)
    else:
        print(item

输出：
1
2
3
4
5
6

#利用 for 循环取值，有几层嵌套就要写几层 for 循环，否则不识别。
l=[1,2,[3,4],[5,6,[7,8]]]
for item in l:
    if isinstance(item ,list):
        for newitem in item:
            print(newitem)
    else:
        print(item
输出：
1
2
3
4
5
6
[7, 8]
```
　　　　2.递归函数实现：
```
递归函数实现：
def getitem(l):
    for item in l:
        if isinstance(item,list):
            getitem(item)
        else:
            print(item)

getitem(l)

输出:
1
2
3
4
5
6
7
8
9
10
变式1：遇到类表就缩进一次：

def getitem(l,level=0):
    for item in l:
        if isinstance(item,list):
            getitem(item,level+1)
        else:
            for tab in range(level):
                print('\t',end='' #输出一个 制表符，并且将 print 后面的换行符去掉，这样就是了缩进
            print(item)

getitem(l)
输出：
![](https://images2017.cnblogs.com/blog/1272773/201712/1272773-20171229192004663-608673438.png)

 

变式2：加入开启机制，是否缩进
def getitem(l,level=0,count=False):
    for item in l:
        if isinstance(item,list):
            getitem(item,level+1,count)
        else:
            if count:
                for tab in range(level):
                    print('\t',end='' #输出一个 制表符，并且将 print 后面的换行符去掉，这样就是了缩进
                print(item)
        print(item)

getitem(l)

输出：

1
2
3
4
5
6
7
8
9
10
```














