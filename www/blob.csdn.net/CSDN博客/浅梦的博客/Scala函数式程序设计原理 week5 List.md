# Scala函数式程序设计原理 week5 List - 浅梦的博客 - CSDN博客





2017年09月19日 09:22:04[浅梦s](https://me.csdn.net/u012151283)阅读数：264








# List

`list`是函数式编程中得基础数据结构。 

lists和arrays的区别在于
- `list`是不可修改的，其元素值不可变
- `list`是递归的，而`array`是展平的

与array一样，list是同质的(homogeneous)，其元素必须为相同类型。 

一个含有元素类型T的列表写为`scala.List[T]`或简写为`List[T]`

## List构造器

所有的列表构造于
- 空列表`Nil`
- construction操作符`::`(读作cons)：`x::xs`产生一个新的列表，首元素是x，后面是xs的所有元素

## 右结合性

`::`是右结合的，通过`::`列表将从末端开始构建。
`A::B::C //解释为A::(B::C)`
## List上的操作

所有列表上的操作都可以表达成以下几种操作 

head,tail,isEmpty

## List模式

也可以用模式匹配解析列表
- Nil Nil常量
- p::ps 一个匹配列表头部和尾部的模式
- List(p1,…,pn) 和p1::…::pn::Nil 相同

## List方法
|方法|子列表和访问元素|
|----|----|
|xs.length|xs的元素个数|
|xs.last|获取最后一个元素，若不空|
|xs.init|获取除了最后一个元素的列表|
|xs take n|获取前n的元素的列表|
|xs drop n|移除前n个元素的列表|
|xs(n)|指定所以的元素|
|**方法**|**创建新列表**|
|xs ++ ys|创建包含xs和ys所有元素的列表|
|xs.reverse|创建逆序列表|
|xs updated (n,x)|更新索引n出的元素为x|
|**方法**|**查找元素**|
|xs indexOf x|返回指定元素索引|
|xs contains x|是否包含指定元素|

# 对偶和元组

## 对偶

包含元素x,y的对偶写作(x,y)
`val pair = ("answer",42) > pair:(String,Int)=(answer,42)`
以上对偶的类型为(String,Int) 

对偶可以用作模式：
`val (label,value) = pair`
## 元组

元组`(T1,...,Tn)`的是参数化类型`scala.Tuplen[T1,...Tn]`的缩写。

## 隐式参数

函数或方法可以带有一个标记为`implicit`的参数列表。这种情况下，编译器会查找缺省值，提供给该函数或方法。

```
case class Delimiters(left:String,right:String)
def quote(what:String)(implicit delims:Delimiters)=delims.left+what+delims.right
```

可以用一个显式的Delimiters对象来调用quote方法，就像这样：
`quote("hello")(Delimiters("<<",">>"))//将返回<<heelo>>`
注意这里有两个参数列表。这个函数是“柯里化”的。 

也可以略去隐式参数列表：
`quote("Hello")`
这种情况下，编译器会查找一个类型为`Delimiters`的隐式值。这必须是一个声明为`implicit`的值。编译器将会在如下两个地方查找这样的一个对象
- 在当前作用域所有可以用单个标识符指代的满足类型要求的val和def
- 在所要求类型相关联的类型的伴生对象。相关联的类型包括所要求类型本身，以及它的类型参数。

# List高阶函数（函数映射到集合）

## Map

当想要对集合中所有元素进行变化时，map方法可以将某个函数应用到集合中的每个元素并产出其结果的集合。

```
val names = List('a','b','c')
names.map(_.tpUpperCase)//List('A','B','C')
```

## Flatmap

如果函数产出一个集合而不是单个值得话，若想将所有得值串接在一起，使用`flatMap`。 

如果使用flatMap并传入返回Option得函数，最终返回得集合将包含所有的v值，前提是函数返回Some(v)。

## foreach

如果应用到函数各个元素仅仅是为了它的副作用而不关心函数值，使用`foreach`。

## Filter

另一个常用操作是筛选所有满足条件的元素。

```
val pair = List(1,2,3,4,5,6)
  pair.filter(x => x<3)//1,2
```

## Filter的变形

![这里写图片描述](https://img-blog.csdn.net/20170919104225369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 化简，折叠，扫描

## 化简

map方法将一元函数应用到集合的所有元素。下面介绍用二元函数组合集合中的元素。类似`c.reduceLeft(op)`这样的调用将op相继应用到元素。

```
List(1,7,2,9).reduceLeft(_-_)//((1-7)-2)-9=-17
List(1,7,2,9).reduceRight(_-_)//1-(7-(2-9))
```

## 折叠

以不同于集合首元素的初始元素开始计算，对`coll.foldLeft(init)(op)`的调用将会计算

```
List(1,7,2,9).foldLeft(0)(_-_)
将得到0-1-7-2-9 = -19
```

初始值和操作符是两个分开定义的”柯里化”参数，这样Scala就能用初始值的类型来推断出操作符的类型定义。 

也可以用`/:`操作符写foldLeft操作，
`(0/:List(1,7,2,9))(_-_)`
对`/:`操作符而言，初始值是第一个操作元。 

折叠有时候可以作为循环的替代。

```
val freq = scala.collection.mutable.Map[Char,Int]()
for (c<-"Hello") freq(c) = freq.getOrElse(c,0) + 1

(Map[Char,Int]() /: "Hello"){
(m,c) => m + (c->(m.getOrElse(c,0)+1))
}
```

## 扫描

`scanLeft`和`scanRight`方法将折叠和映射操作结合在一起。得到包含所有中间结果的集合

```
(1 to 10).scanLeft(0)(_+_)//产出所有中间结果和最后的和
Vector(0,1,3,6,10,...,45,55)
```



