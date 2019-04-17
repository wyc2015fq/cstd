# Scala函数式程序设计原理 week6 collections - 浅梦的博客 - CSDN博客





2017年09月19日 13:12:17[浅梦s](https://me.csdn.net/u012151283)阅读数：242








# 集合特质

![这里写图片描述](https://img-blog.csdn.net/20170919122037974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Seq是一个有先后次序的值的序列，比如数组或列表。IndexSeq允许我们通过整型的下标快速访问任意元素。 

Set是一组没有先后次序的值。在SortedSet中，元素以某种排过序的顺序被访问。 

Map是一组（键，值）对偶。SortedMap按照键的排序访问其中的实体。
## 集合类型定义的增删操作符

![这里写图片描述](https://img-blog.csdn.net/20170920102216291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Iterable特质的重要方法

![这里写图片描述](https://img-blog.csdn.net/20170920100530034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170920100550752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 可变和不可变集合

Scala同时支持可变和不可变的集合。不可变的集合从不改变，因此你可以安全地共享其引用，甚至在一个多线程的应用程序中也没问题。 

Scala优先采用不可变集合

# 序列

![这里写图片描述](https://img-blog.csdn.net/20170919123535881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Vector

**Vector**是ArrayBuffer的不可变版本，一个带下标的序列，支持快速的随机访问。向量以树形结构的形式实现，每个节点可以有不超过32个子结点，对于一个有100万个元素的向量而言，只需4层结点。

## Range

**Range**表示一个整数序列，Range对象并不存储所有值而是只有起始值，结束值和增值。 

可以用`to`(包含末尾)和`until`（不含末尾）和`by`(步长)方法构造Range对象。

```
val r:Range = 1 until 5//1,2,34
val s:Range = 1 to 5//1,2,3,4,5
1 to 10 by 3 //1,4,7,10
```

## Array和String

**Array**和**String**支持和**Seq**相同的操作，可以隐式地转换成序列。（他们不能成为序列的子类，因为他们来组与Java）

```
val xs:Array[Int] = Array(1,2,3)
xs map (x => 2*x)
val ys: String = "Hello"
ys filter (_.isUpper)
```

![这里写图片描述](https://img-blog.csdn.net/20170919124748609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 可变列表

可变的`LinkedList`和不可变的`List`相似，可以通过对elem引用赋值来修改其头部，对next引用赋值来修改其尾部。

## for循环表达式

![这里写图片描述](https://img-blog.csdn.net/20170919125152388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 集Set

集是不重复元素的集合。集是无序的。

# 映射Map

另一个基础集合类型是map.一个类型为`Map[Key,Value]`的数据结构。

## 查询映射

map.getOrElse(key,default_value)

## 更新映射

在可变映射中，可以更新映射的值，或者添加一个新的映射关系 

scores(“Bob”) = 10

## 迭代映射

for ((k,v) <- 映射) 处理k和v

## 默认值

至今为止，映射是偏函数，一个并非对所有输入值都有定义的函数。`withDefaultValue`操作可以将一个映射转换成完全函数。

```
val cap1 = capitalOfCounry withDefaultValue "<unknown>"
cap1("abc")//"unkonwn"
```

![这里写图片描述](https://img-blog.csdn.net/20170919131203846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





