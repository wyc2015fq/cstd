# split 过滤空的元素 - lincappu - 博客园







# [split 过滤空的元素](https://www.cnblogs.com/lincappu/p/8145689.html)







　命令形式：

　　split(str='',number=string.count(str))[n]

　　str 分隔符 number 切分几次，[n] 获取第几个值。

　　1.如果切分的可迭代对象中包含空元素的解决方法：
*　　split()  # 不加参数的时候，它会把空格 制表符  换行符都当做为分隔符。　　l = 'ni hao    ma wo  shi         shui    '　　print(l)　　print(l.split())*
```
>>> l='ni   hao ma wo shi  shui '
>>> l.split(' ')
['ni', '', '', 'hao', 'ma', 'wo', 'shi', '', 'shui', '']
>>> l.split() # 不加参数的时候，它会把空格 制表符  换行符都当做为分隔符。
['ni', 'hao', 'ma', 'wo', 'shi', 'shui']

# 第二种解决方法,利用 filter 函数
>>> l1=l.split(' ')
>>> l1
['ni', '', '', 'hao', 'ma', 'wo', 'shi', '', 'shui', '']
>>> newlist=filter(None,l1)
>>>
>>> newlist
<filter object at 0x10b1b27f0>
# filter 生成的是一个迭代器对象（python3中，python2生成的是个列表）

这时候有两种取值方法：样例

def is_odd(n):
    return n % 2 == 1

newlist = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
for  item  in  newlist:  #这取出来是一个一个的值
    print(item)


newlist = filter(is_odd, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
l1 = [item for item in newlist]
# 这样取出来的是字符串格式的，这里有问题问题要注意下，上面执行完后，下面取出来的是空值，因为迭代器已经取完了，*要想再取值，需要重新迭代*
```














