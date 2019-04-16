# scala 简要：数组与映射 - 我相信...... - CSDN博客





2014年12月27日 15:50:15[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1857








Scala中的Array是定长数组，ArrayBuffer是变长数组，对应于Java中的ArrayList，C++中的Vector，可以用相同的代码处理这两种数据结构，用 for （i<-区间 ）来遍历，

用for（...) yield  创建一个类型与原始集合相同的新集合，还可以通过if 在进行条件过滤。Scala中的内建函数sum，sorted，max，min，quicksork提供了常用算法。由于Scala数组是用java数组实现的，可以在java和scala之间传递，只需引入scala.collection.JavaConversions里的隐式转换方法。

scala中，映射是对偶的集合,可以看做将键映射到值的函数，区别在于函数一般用于计算，而映射只做查询。用=可以直接增加映射，也可用+=添加多个关系，用for((k,v)<-映射） 来遍历映射，使用scala.collection.JavaConversions.mapAsScalaMap将Java中的map转换为scala中的映射。

scala中，元组是不同类型的值的聚集，（）构成元组，用方法1,2...访问其组元，而通常使用模式匹配来获取元组的组元。使用元组的原因之一是把多个值绑在一起，以便它们能够被一起处理，通常用zip方法开完成，使用toMap方法将对偶的集合转换成映射



