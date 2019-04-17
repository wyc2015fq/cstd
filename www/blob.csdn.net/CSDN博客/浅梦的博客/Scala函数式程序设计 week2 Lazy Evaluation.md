# Scala函数式程序设计 week2 Lazy Evaluation - 浅梦的博客 - CSDN博客





2017年09月20日 19:25:01[浅梦s](https://me.csdn.net/u012151283)阅读数：105
个人分类：[Scala](https://blog.csdn.net/u012151283/article/category/7178055)









# 迭代器

可以用`iterator`方法从集合获得一个迭代器。 

对于那些完整构造需要很大开销的集合而言，迭代器很有用。 

有了迭代器，可以用`next`和`hasNext`来遍历集合中的元素
```
while(iter.hasNext)
对 iter.next()执行某种操作
//---------------------
for (elem <- iter)
对elem执行某种操作
```

上述两种循环都会将迭代器移动到集合的末尾，在此之后就不能再使用了。 
`Iterator`类定义了与集合方法使用起来完全相同的方法。除了head,headOption,last,lastOption,tail,init,takeRight,dropRight外。 

在调用了诸如map,filter,count,sum甚至length方法后,迭代器将位于集合的尾端。 

对于其他方法，比如find或take，迭代器位于已找到元素或已取得元素之后。 

可以用toArray,toIterable,toSeq,toSet或toMap将值拷贝到新的集合中。
# 流

迭代器具有一个缺点，每次对next的调用都会改变迭代器的指向。`stream`提供的是一个不可变的替代品。流是一个**尾部被懒计算的不可变列表**，只有当需要时才会被计算。

```python
def numsFrom(n:Int):Stream[Int] = n #:: numsFrom(n+1)
```

`#::`操作符类似`::`操作符，种子不过构建出来的是一个流。 

调用

```
调用 val tenOrMore = numsFrom(10)时
得到的是一个被显示为
Stream(10,?)
的流对象
```

其尾部是未被求值的。 

流的方法是懒执行的

```
val squares = numsFrom(1).map(x => x*x)//将产出
Stream(1,?)
```

如果想要得到多个答案，可以调用`take`，然后用`force`，这将强制对所有值求值。 

如

```
squares.take(5).forece
squares.force //不能这样写，这个调用尝试对一个无穷流的所有成员进行求值，引发内存错误
```

可以从迭代器构造一个流。流将缓存访问过的所有行，允许重新访问。





