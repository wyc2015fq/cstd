# python中set集合如何决定是否重复？ - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 16:16:20[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1719
http://heipark.iteye.com/blog/1743819
看下面代码，两个值相同的Item对象，添加到set中被认为是两个对象。
**[python]**[view
 plain](http://blog.csdn.net/sunmenggmail/article/details/8546804#)[copy](http://blog.csdn.net/sunmenggmail/article/details/8546804#)
- class Item(object):  
- 
- def __init__(self, foo, bar):  
- self.foo = foo  
- self.bar = bar  
- 
- def __repr__(self):  
- return"Item(%s, %s)" % (self.foo, self.bar)  
- 
- print set([Item('1', '2'), Item('1', '2')])  
- 
- # 输出： set([Item(1, 2), Item(1, 2)])
数据结构中HASH表的设计思路是：
- 被插入元素根据hash值决定插入那个桶（通常一个数组）中
- 新插入元素根据的equals方法依次比较桶内元素，以确定是否已经存在
在python中如果我们自定义class，则需要添加__hash__和__eq__两个方法，前者用于决定当前元素在哪个桶，后者决定当前元素是否在桶中已经存在。
修改后的code如下：
**[python]**[view
 plain](http://blog.csdn.net/sunmenggmail/article/details/8546804#)[copy](http://blog.csdn.net/sunmenggmail/article/details/8546804#)
- class Item(object):  
- def __init__(self, foo, bar):  
- self.foo = foo  
- self.bar = bar  
- 
- def __repr__(self):  
- return"Item(%s, %s)" % (self.foo, self.bar)  
- 
- def __eq__(self, other):  
- if isinstance(other, Item):  
- return ((self.foo == other.foo) and (self.bar == other.bar))  
- else:  
- returnFalse
- 
- def __hash__(self):  
- return hash(self.foo + " " + self.bar)  
- 
- print set([Item('1', '2'), Item('1', '2')])  
- 
- # 输出：set([Item(1, 2)])
- 
