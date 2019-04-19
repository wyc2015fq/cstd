# Scala Match - Simple 专栏 - CSDN博客
2018年11月30日 11:34:50[Simple_Zz](https://me.csdn.net/love284969214)阅读数：34标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
### Scala中提供了比java，c更加强大的switch语句——match，而且不用担心因为忘记写break语句而发生case穿透。
**1.更好的 switch 与if，类似match也是表达式**
```
// 匹配字符的函数
def charMatch(c: Char) = c match {
  case '+' => 1
  case '-' => -1
  case _ => 0
}
// 可以使用任何类型，而不仅仅是数字匹配
def colorMatch(c:Color) = c match {
  case Color.RED => 0xff0000
  case Color.BLACK => 0x000000
  case Color.YELLOW => 0xffff00
  case _ => 0
}
```
**2.守卫**
```
// 修改一下例一,可以添加if这样的守卫，来匹配所有数字，守卫可以是任何Boolean条件
def charMatch(c: Char) = c match {
  case '+' => 1
  case '-' => -1
  case ch if Character.isDigit(ch) => Character.digit(ch, 10)
  case _ => 0
}
```
**3.模式中的变量 **
- 如果case后跟着一个变量名，那么匹配的表达式会被复制给那个变量,你可以将case _ 
- 看做这个特性的一个特殊情况，变量名为_可以通配。
```
case **ch** if Character.isDigit(**ch**) => Character.digit(**ch**, 10)
// 变量模式和常量模式如果有冲突，可以用反引号` `
def dMatch(d: String) = d match {
  case `Pi` => 3.14
  case n => Integer.parseInt(n)
  case _ => 0
}
```
**4.类型匹配 **
- 这样就不用使用isInstanceOf的操作符判断类型，在用asInstanceOf转换 
- 但是要注意Map和其他有泛型的集合，模式匹配发生在运行期，泛型会被擦掉 
- Map[Int,Int] 要用Map[_ , _]代替
```
def anyMatch(obj: Any) = obj match {
  case i: Int => i
  case s: String => Integer.parseInt(s)
  case _: BigInt => Int.MaxValue
  case m: Map[_, _] => 1 //       模式匹配发生在运行期  
  case _ => 0
}
```
**5.匹配数组列表和元组**
```
//  匹配数组，列表和元组
def arrMatch(a: Array[Int]) = a match {
  case Array(0) => "0"
  case Array(x, y) => x + " " + y
  case Array(0, _*) => "0 ..."
  case _ => "something else"
}
//  case class ::[E] (head :E,tail:List[E]) extends List[E]
def listMatch(l: List[Int]) = l match {
  case 0 :: Nil => "0"
  // 多个中置表达式更易读
  case x :: y :: Nil => x + " " + y
  case h :: t => h + " " + t.mkString(" ")
  // 上面的一种写法,将调用：：.unapply(lst)
  case ::(h, t) => h
  case 0 :: tail => "0 ..."
  case _ => "something else"
}
def pairMatch(p: (Int, Int)) = p match {
  case (0, _) => "0 ..."
  case (y, 0) => y + " 0"
  case _ => "neither is 0"
}
```
**6.提取器extractor **
- 提取unapply或unapplySeq方法
- 这里来注意一下了一个正则表达式的场景，正则中的分组匹配，可以用提取器来获取每个分组
```
//  提取器extractor
val pattern = "([0-9]+) ([a-z]+)".r
def patternMatch(s: String) = s match {
  // 这里的提取器不是伴生对象而是提取器对象
  case pattern(num, item) => num
  case _ => 0
}
```
**7.变量声明中的模式**
```
val (x, y) = (1, 2)
// /%获取商和余数
val (q, r) = BigInt(10) /% 3
   
val arr = Array(1, 2, 3)
// 提取数组中的第一第二个元素
val Array(f, s, _*) = arr
```
**8.for表达式中的模式**
```
import collection.JavaConversions._
//  for((k,v) <- System.getProperties) println(k -> v)
//  匹配为空的值
for ((k, v) <- System.getProperties if v == "") println(k -> v)
for ((k, "") <- System.getProperties) println(k -> "")
```
**9.样例类 **
- 这是一种经过优化而被用于模式匹配的类
```
abstract class Amount
case class Dollar(value: Double) extends Amount
case class Currency(value: Double, unit: String) extends Amount
case object Nothing extends Amount
//单例对象不用加括号
//  样例类的实例使用()，样例对象不用使用()
def amountMatch(a: Amount) = a match {
        
  case Dollar(v) => "$" + v
  //      case Currency(_,u) => "Oh noes , I got "+u
  //      如果两个参数是对偶，则可以写成中置表示法
  case a Currency u => a + " Oh noes , I got " + u
  case Nothing => ""
}
```
**10.copy方法和带名参数 **
- 通过带名参数直接修改这些对象的属性
```
//  copy方法和带名参数
val amt = Currency(29.95, "EUR")
val price = amt.copy()
val price1 = amt.copy(value = 20.0)
val price2 = amt.cop
```
**11.case 中置表示法**
```
//  中置表示法
case object +: {
  def unapply[T](input: List[T]) = if (input.isEmpty) None else Some((input.head, input.tail))
}
1 +: 7 +: 9 +: Nil match {
  //可以用+:来析构列表
  case f +: s +: r => f + s + r.length
  case _ => None
}
```
**12.匹配嵌套结构 **
- 场景售卖某个商品，会将商品捆绑销售：
```
abstract class Item
case class Article(description: String, price: Double) extends Item
case class Bundle(description: String, price: Double, items: Item*) extends Item
    Bundle("Book1", 20, Article("Book2", 39), Bundle("Book3", 25, Article("Book4", 31), Article("Book5", 11)))
def matchItem(i: Item) = i match {
  //  匹配到了被绑定到Bundle第一个Article的描述desc
  case Bundle(_, _, Article(_, p), _*) => println(p)  
  //      匹配到Bundle只含有一个Article的第一个Article
  //      @可以将变量绑定到嵌套的值上
  case Bundle(_, _, art@Article(_, _), rest) => println(art.price)  
  //      可以匹配到Bundle含有多个Article的第一个Article
  case Bundle(_, _, art@Article(_, _), rest@_*) => println(art.price)  
  case _ => println(None)
}
//  可以用于计算商品的价格
def price(it: Item): Double = it match {
  //      计算单本书的价格
  case Article(_, p) => p
  //      递归计算一捆书的价格
  case Bundle(_, disc, its@_*) => its.map(price _).sum - disc
}
```
**13.密封类sealed **
- 当你用样例类来做匹配是，你可能想让编译器确保你已经列出了所有的可能选择。 
- 要达到这个目的，需要将其超类声明为sealed：
```
sealed abstract class Amount
case class Dollar(value: Double) extends Amount
case class Currency(value: Double, unit: String) extends Amount
case object Nothing extends Amount //单例对象不用加括号
// 密封类何所有子类必须在同一个源文件中定义。 如果要加上一个欧元类
case class Euro(value: Double) extends Amount
```
**14.模拟枚举**
```
//  模拟枚举,样例类可以模拟枚举类，scala不自带枚举
sealed abstract class TrafficLightColor
case object Red extends TrafficLightColor
case object Yellow extends TrafficLightColor
case object Green extends TrafficLightColor
def trafficLightMatch(light: TrafficLightColor) = light match {
  case Red   => "stop"
  case Yellow   => "hurry up"
  case Green   => "go"
}
```
**15.Option类**
- option可能存在有可能不存在的值。其样例子类Some包装了某个值如：Some(“Fred”),而样例对象None表示没有值
```
//  Option支持泛型 Some("Fred") => Some[String]
//  Map类的get返回一个Option，用模式匹配来获取值
val scores = Map("Bob" -> 75, "Alice" -> 90) scores
.get("Alice") match {
  case Some(s) => println(s)
  case None => println("no score")
}
//  不过这很烦
val score = scores.get("Alice")
   
if (score.isEmpty) println("no score")
else println(score.get)
//  这也很烦 使用getOrElse   
println(score.getOrElse("no socre"))
```
**16.偏函数 **
- 偏函数必须位于编译器可以推断返回类型的上下文中。当你把它复制个一个PartialFunction[Char,Int]有类型声明的变量，和当做参数传递是符合这个要求。 
- 被包含在{}一组case语句是一个偏函数—不是对所有输入值都有定义的函数。他是partailFunction[A,B]类的一个实例。A是参数类型，B是返回值类型。 
- 该类有两个方法：apply方法从匹配到的模式计算函数值，而isDefinedAt只要有一个匹配返回true。
```
val func: PartialFunction[Char, Int] = {
  case '+' => 1;
  case '-' => -1
}
func('-') //调用了f.apply('-'),-1
func.isDefinedAt('0') //false
func('0') //MacthError
//  collect方法将偏函数作为参数
"-3+4".collect(func) //Vector(-1,1)
```
