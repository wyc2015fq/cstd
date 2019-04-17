# Scala常用List列表操作方法 - Machine Learning with Peppa - CSDN博客





2018年07月03日 16:18:21[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2300








把Scala List的几种常见方法梳理汇总如下，日常开发场景基本上够用了。

创建列表

```
scala> val days = List("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday")
days: List[String] = List(Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday)
```

创建空列表

```
scala> val l = Nil
l: scala.collection.immutable.Nil.type = List()

scala> val l = List()
l: List[Nothing] = List()
```

用字符串创建列表

```
scala> val l = "Hello" :: "Hi" :: "Hah" :: "WOW" :: "WOOW" :: Nil
l: List[String] = List(Hello, Hi, Hah, WOW, WOOW)
```

用“:::”叠加创建新列表

```
scala> val wow = l ::: List("WOOOW", "WOOOOW")
wow: List[String] = List(Hello, Hi, Hah, WOW, WOOW, WOOOW, WOOOOW)
```

通过索引获取列表值

```
scala> l(3)
res0: String = WOW
```

获取值长度为3的元素数目

```
scala> l.count(s => s.length == 3)
res1: Int = 2
```

返回去掉l头两个元素的新列表

```
scala> l.drop(2)
res2: List[String] = List(Hah, WOW, WOOW)

scala> l
res3: List[String] = List(Hello, Hi, Hah, WOW, WOOW)
```

返回去掉l后两个元素的新列表

```
scala> l.dropRight(2)
res5: List[String] = List(Hello, Hi, Hah)

scala> l
res6: List[String] = List(Hello, Hi, Hah, WOW, WOOW)
```

判断l是否存在某个元素

```
scala> l.exists(s => s == "Hah")
res7: Boolean = true
```

滤出长度为3的元素

```
scala> l.filter(s => s.length == 3)
res8: List[String] = List(Hah, WOW)
```

判断所有元素是否以“H”打头

```
scala> l.forall(s => s.startsWith("H"))
res10: Boolean = false
```

判断所有元素是否以“H”结尾

```
scala> l.forall(s => s.endsWith("W"))
res11: Boolean = false
```

打印每个元素

```
scala> l.foreach(s => print(s + ' '))
Hello Hi Hah WOW WOOW
```

取出第一个元素

```
scala> l.head
res17: String = Hello
```

取出最后一个元素

```
scala> l.last
res20: String = WOOW
```

剔除最后一个元素，生成新列表

```
scala> l.init
res18: List[String] = List(Hello, Hi, Hah, WOW)
```

剔除第一个元素，生成新列表

```
scala> l.tail
res49: List[String] = List(Hi, Hah, WOW, WOOW)
```

判断列表是否为空

```
scala> l.isEmpty
res19: Boolean = false
```

获得列表长度

```
scala> l.length
res21: Int = 5
```

修改每个元素，再反转每个元素形成新列表

```
scala> l.map(s => {val s1 = s + " - 01"; s1.reverse})
res29: List[String] = List(10 - olleH, 10 - iH, 10 - haH, 10 - WOW, 10 - WOOW)
```

生成用逗号隔开的字符串

```
scala> l.mkString(", ")
res30: String = Hello, Hi, Hah, WOW, WOOW
```

反序生成新列表

```
scala> l.reverse
res41: List[String] = List(WOOW, WOW, Hah, Hi, Hello)
```

按字母递增排序

```
scala> l.sortWith(_.compareTo(_) < 0)
res48: List[String] = List(Hah, Hello, Hi, WOOW, WOW)
```



