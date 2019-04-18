# Scala——Traits - Soul Joy Hub - CSDN博客

2016年12月06日 13:30:33[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：738


类似于Java中的接口，trait被用于通过所支持的方法特例化来定义对象。如Java 8中一样，Scala允许trait被部分实现。和class相比，trait可以没有构造函数。如：

```
trait Similarity {
  def isSimilar(x: Any): Boolean
  def isNotSimilar(x: Any): Boolean = !isSimilar(x)
}
```

这个trait有两个方法 isSimilar and isNotSimilar. isSimila没实现,而 method isNotSimilar实现了. 因此, classes extends 这个trait，只用提供对isSimilar的实现:

```
class Point(xc: Int, yc: Int) extends Similarity {
  var x: Int = xc
  var y: Int = yc
  def isSimilar(obj: Any) =
    obj.isInstanceOf[Point] &&
    obj.asInstanceOf[Point].x == x
}
object TraitsTest extends App {
  val p1 = new Point(2, 3)
  val p2 = new Point(2, 4)
  val p3 = new Point(3, 3)
  println(p1.isNotSimilar(p2))
  println(p1.isNotSimilar(p3))
  println(p1.isNotSimilar(2))
}
```

输出：

```bash
false
true
true
```

