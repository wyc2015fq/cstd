# scala 关于Array,List,Tuple的区别 - zkq_1986的博客 - CSDN博客





2017年01月14日 11:21:35[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：149
个人分类：[java](https://blog.csdn.net/zkq_1986/article/category/6317924)










- 
在Scala 2.7中，Array、List都不能混合类型，只有Tuple可以；而在Scala以上版本中，3者的元素都可以混合不同的类型（转化为Any类型），只不过是当使用混合类型时，Array和List会将元素类型转化为Any类型,而Tuple则保留每一个元素的初始类型；

- 
关于Array，List，Tuple

![](http://static.oschina.net/uploads/space/2015/0929/141005_J5Zb_1034176.jpg)

- 
关于初始化

1) val array= new Array[String](3) // Array(null, null, null)相当于声明了3个null值的空元素






val array= Array("a","b","c","d") // 相当于Array.apply("a","b","c","d")


   定义一个类型为Any的Array：

    val aa = Array[Any](1, 2)或：val aa: Array[Any] = Array(1, 2)或：val aa: Array[_] = Array(1, 2)

    2) val list:List[Int] = List(1,3,4,5,6) // 或者val list = List(1,3,4,5,6)

(：：：)实现叠加List,(::)cons:将新元素组合到列表的最前端。元素合并使用：：，集合合并使用：：：，示例如下：其中Nil代表空元素


val list2 = "a"::"b"::"c"::Nil // Nil

    val list4 = list2:::list3

    3) 元组也是不可变的，但是元组可以是不同类型的数据，实例化：var a = （，）,可以通过点号，下划线，-N（N从1开始）的索引访问元素

![](http://static.oschina.net/uploads/space/2015/0929/143827_4lxw_1034176.jpg)

转载自：https://my.oschina.net/u/1034176/blog/512314










