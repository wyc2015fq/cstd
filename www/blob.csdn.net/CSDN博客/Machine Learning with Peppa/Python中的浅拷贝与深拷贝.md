# Python中的浅拷贝与深拷贝 - Machine Learning with Peppa - CSDN博客





2018年03月08日 16:39:32[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：88标签：[Python																[内存																[编程																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[编程之美：Python																[***算法与编程艺术***																[系统：编译与底层](https://blog.csdn.net/qq_39521554/article/category/7484528)




（一）2个为什么

          先通过2个为什么来了解一下python内存中变量的存储情况。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> name = [1,2,3,["alex","rain"]]
>>> name2 = name.copy()     # 将原列表copy一份赋值给name2
>>> print(name)
[1, 2, 3, ['alex', 'rain']]
>>> print(name2)
[1, 2, 3, ['alex', 'rain']]
>>> 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

name与name2相同

第一个为什么：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> name[1] = -2
>>> print(name)
[1, -2, 3, ['alex', 'rain']]
>>> print(name2)
[1, 2, 3, ['alex', 'rain']]
>>> 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

name[1]改变后，name改变了而name2没有改变，为什么？

第二个为什么：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
>>> name[3][0] = "ALEX"
>>> print(name)
[1, -2, 3, ['ALEX', 'rain']]
>>> print(name2)
[1, 2, 3, ['ALEX', 'rain']]
>>> 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

 将name[3][0]的值改后，name改变了，name2也改变了，为什么？

    第一个为什么和第二个为什么都对列表进行了更改，而结果为什么不一样尼？

    首先我们要清楚，列表name的元素是存在于多块内存空间中的，不是在同一块；每个元素的内存地址都是独立的。

    变量name等于[1,2,3,["alex","rain"]]这个含有这些元素的列表的时候，内存中发生了2件事：一是变量name是个列表，开辟了一块内存空间；二是列表里的每一个元素各自开辟了属于自己的内存空间。name列表开辟的内存空间里存的不是元素，而是每个元素的内存地址。每一个元素比作是家的话，name列表的内存空间里存的就是邮寄到你家的包裹上的地址，是一个指向。

     name2 copy name，只是copy了name中的元素（一级元素）的内存地址，即将id(1)，id(2)，id(3)等这些元素的内存地址复制到了name2的内存空间里了。

解答第一个为什么：

            将name中的2改为-2，name内的2的内存地址会被擦除，然后将新开辟的-2的内存地址占位到此处。由于2的内存地址还被name2引用，所以2的内存不会被释放，依然存在。

    对于第一个为什么，name的第一个元素为-2，name2的第一个元素为2，因为这2个的内存地址不同，所以地址所指向的数据（在内存里以十六进制数表示）就不同。内存地址不同，数据不同。

    解答第二个为什么：

要区分清楚列表的内存地址和列表中元素的内存地址是不一样的，不要混淆。

  name2 copy name的时候，copy了每个元素（一级元素）的内存地址。name中元素['alex', 'rain']这个小列表的内存地址也被复制，即name中的小列表的内存地址与name2中的小列表的内存地址是一样的，也就是说name2中的小列表没有开辟新的空间，而是引用了name中小列表的空间。id(name[3]) = id(name2[3]).

    第二个为什么中，对name内的小列表中的元素"alex"进行了全大写更改（过程："ALEX"开辟了一块新的空间，将这块新空间的地址放到小列表中的索引为0的位置），即小列表（嵌套列表）内的元素的发生改变，而小列表的内存地址没有发生变化，name2中的小列表的内存地址与name中的小列表的内存地址一样，所以name中小列表的值发生变化，name2中的小列表的值也会变化。内存地址相同，指向的数据也相同。

（二）赋值、浅拷贝、深拷贝

1、赋值：传递对象的引用而已,原始列表name改变，被赋值的n也会做相同的改变。(见下图，图画的不好)

![](https://images2017.cnblogs.com/blog/1207051/201709/1207051-20170927005746122-1619083740.png)



2.浅拷贝：拷贝父对象，不会拷贝对象的内部的子对象。即拷贝列表name里面的一级元素的内存地址，不拷贝name里的小列表里的元素的内存地址。

![](https://images2017.cnblogs.com/blog/1207051/201709/1207051-20170927010725934-920290026.png)



![](https://images2017.cnblogs.com/blog/1207051/201709/1207051-20170927011127887-603893289.png)





![](https://images2017.cnblogs.com/blog/1207051/201709/1207051-20170927011618340-1454797700.png)



3.深拷贝：copy 模块的 deepcopy 方法，完全拷贝了父对象及其子对象。即name2不仅拷贝了name中一级元素（1，2，3，["alex","rain"]）的的内存地址，也拷贝了嵌套列表，["alex","rain"]里面的"alex"和"rain"的内存地址。



![](https://images2017.cnblogs.com/blog/1207051/201709/1207051-20170927012253840-876581808.png)



　　name比作是一个容器的话，我们把name里的每样东西复制了一份放到另一个容器name2里。name里有东西丢失的话，name2里的还在，而name2这个容器是新开辟的空间。在第一个容器里的东西还未变化之前，2个不同容器装了同样的东西。

　　非容器类型的没有拷贝这一说。](https://blog.csdn.net/qq_39521554/article/category/7361353)](https://blog.csdn.net/qq_39521554/article/category/7392111)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




