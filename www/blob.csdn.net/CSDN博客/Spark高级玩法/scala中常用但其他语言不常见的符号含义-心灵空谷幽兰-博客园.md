# scala中常用但其他语言不常见的符号含义 - 心灵空谷幽兰 - 博客园 - Spark高级玩法 - CSDN博客
2018年01月22日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：235

本文旨在介绍Scala在其他语言中不太常见的符号含义，帮助理解Scala Code。
随着我对Scala学习的深入，我会不断增加该篇博文的内容。
**修改记录**
----2016.11.23  新增scala中最神秘强大的下划线(_)用处
下面介绍Scala中的符号：
- 
:::三个冒号运算符：表示list的连接操作
```scala;gutter
```
val one = List(1,2,3)val two = List(4,5,6)val three = one:::two
　　输出结果为：three: List[Int] = List(1, 2, 3, 4, 5, 6)
- 
:: 两个冒号运算符：表示普通元素与list的连接操作
- 
_N下划线数字运算符：用于访问元组的第N个元素，N的取值从1开始。元组的元素访问方法与数组不同是因为元组的元素类型可以不同。
- 
-> 返回一个二元元组
　　返回：(List[Int], List[Int]) = (List(1, 2, 3),List(4, 5, 6))
- 
<-  用于遍历集合对象 
- 
=>  把左边的东西改成右边的东西；可以看做创建函数实例的语法糖，这个运算符常见的有下面三种：
- 
=>：Example：Int => String表示函数输入为Int型，返回String型，同Function(Int,String)
- 
()=>：Example：() => T表示函数没有输入参数，但返回T
- 
Unit=>：Unit相当于无值的值，相当于C++中的Void
- 
 _   有很多含义
　　　　　　1、通配符  
import org.apache.spark.SparkContext._
　　　　　   2、匿名函数
                 3、指代集合中的每一个元素。例如筛选列表中大于某个值的元素
```scala;gutter
```
val lst = List(1,2,3,4,5)val lstFilter = lst.filter(_ > 3)
　　           4、使用模式匹配可以用来获取元组的组员。
```scala;gutter
```
val m = Map(1 -> 2,2 -> 4)for ((k,_) <- m) println(k)   //如果不需要所有部件，则在不需要的部件使用_；本例只取key,因此在value处用_
　　         5、代表某一类型的默认值
                      对于Int类型来说，它是0
                      对于Double来说，它是0.0
                      对于引用类型来说，它是null
- 
:_* 作为一个整体，告诉编译器你希望将某个参数当作数序列处理。
val s = sum(1 to 5:_*)      //把1 to 5当作一个序列处理
- 
+=：为map类型变量添加元素
- 
-=：为map类型变量移除元素及其对应的值
参考文献
　1 http://stackoverflow.com/questions/6951895/what-does-and-mean-in-scala
   2 http://docs.scala-lang.org/tutorials/FAQ/finding-symbols.html
   3 https://www.zhihu.com/question/21622725/answer/21588672
    摘自 ：https://www.cnblogs.com/xinlingyoulan/p/6031157.html
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
