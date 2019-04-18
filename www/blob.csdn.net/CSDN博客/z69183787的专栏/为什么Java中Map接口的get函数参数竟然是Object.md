# 为什么Java中Map接口的get函数参数竟然是Object - z69183787的专栏 - CSDN博客
2018年07月16日 18:07:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：445
[https://www.jianshu.com/p/7889e4b32362](https://www.jianshu.com/p/7889e4b32362)
文章来源于自己实际开发项目中遭遇到的一个问题。在一开始，我定义了一个
```
Map<Integer, String> map;
```
这并没有说明问题，然后开始往下写了一堆函数，在写到第N个函数的时候，我发现这个`map`需要改成
```
Map<String, String> map;
```
改完之后我天真的以为ide能帮我检查出所有需要修改的地方，我只需要按照列出的红线一个一个的把传入参数的类型修改好就可以了，于是毫无疑问的，我的程序最后败在了这个地方
```
map.get(someIntegerParam);
```
改正程序中的问题只是一方面，更重要的是，为什么这个`get`函数就要这么特别，为什么不和`put`一样用范型作为参数呢！？google了一把，在stackoverflow上也有不少人问过这个问题，其中[这一篇](https://link.jianshu.com?t=http://stackoverflow.com/questions/857420/what-are-the-reasons-why-map-getobject-key-is-not-fully-generic)说的算比较好的，大概的意思就是说如果采用范型作为`get`的参数并不能很好的体现出`map`的精神，因为key是否相等，取决于`key1.equals(key2)`是否成立，这里并不应该限制类型。在sof上的[另外一篇文章](https://link.jianshu.com?t=http://stackoverflow.com/questions/104799/why-arent-java-collections-remove-methods-generic)也阐述了这个问题，并引用了[Josh Bloch](https://link.jianshu.com?t=http://baike.baidu.com/link?url=cYBAkZ1WL7noKPWMNFqmncZluJxtlm178VfroPlbAJmjgEwz1M_FYu9g6zBLNzgeKSt1JWwZ_8ykTjyqCA9WGa)的一段话：
> 
Josh Bloch says (6:41) that they attempted to generify the get method of Map, remove method and some other, but "it simply didn't work". There are too many reasonable programs that could not be generified if you only allow the generic type of the collection as parameter type. The example given by him is an intersection of a List of Numbers and a List of Longs.
大概意思就是说我们曾经尝试过把`get`方法范型化，但是它挂了，有很多很多原因使他不能被范型化，比如你用`List<Number>`做key，但却想用`List<Long>`来`get`。
