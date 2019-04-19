# Scala中继承 - LC900730的博客 - CSDN博客
2017年12月17日 16:32:29[lc900730](https://me.csdn.net/LC900730)阅读数：139
## 扩展类
Scala扩展类的方式与Java一样—使用extends关键字
```php
class Employee extends Person{
    var salary=0.0
    ...
}
```
与Java中一样，你可以将类声明为final，这样它就不能被扩展。还可以将单个方法或字段声明为final，以确保它们不能被重写。
## 重写方法
在Scala中重写一个非抽象方法必须使用override修饰符。
```
public class Person{
    ...
    override def toString=getClass.getName+"[name="+name+"]"
}
```
override修饰符可以在多个常见情况下给出有用的错误提示 
在Scala中调用超类的方法和Java完全一样，使用super关键字。
### isInstanceOf:测试某个对象是否属于给定的类
```
class  Counter{
  private[this] var value=0
  def increment(){value+=1;println("hello world")}
  def current()=value
}
class MyCounter extends Counter{
}
var myCounter=new Counter()
var n=new Counter
var n1=new MyCounter
println(n1.isInstanceOf[Counter])       //true
println(n1.isInstanceOf[MyCounter])     //true
println(n.isInstanceOf[MyCounter])      //false
println(n1.asInstanceOf[Counter])       //Mycounter
println(n.asInstanceOf[Counter])       //Counter
```
#### 类以及子类的对象，isInstanceOf[类]以及isInstanceOf[子类]均返回true
但是调用asInstanceOf[]的是类对象，则返回的是对象；调用的是子类对象，则返回的是子类对象。
如果测试返回的是类对象但不是子类对象，可以用 
p.getClass==classOf[Person]
## 模式匹配
与类型检查相比，模式匹配通常是更好的选择
```
p match {
    case s:Employee => ...   //将s作为Employee处理
    case _=>                 //p不是Employee
}
```
## 超类的构造器
类有一个主构造器和任意数量的辅助构造器，而每个辅助构造器都必须以对先前定义的辅助构造器或主构造器的调用开始 
这样后果是：辅助构造器不可能之间调用超类的构造器 
子类的辅助构造器最终会调用主构造器，只有主构造器可以调用超类的构造器
```
class Employee(name:String,age:Int,val salary:Double) extends Person(name,age)
//类的3个参数name、age、salary，其中2个被传递到了超类。
```
### scala扩展java类
Scala可以扩展Java类。这种情况下，它的主构造器必须调用Java超类的某一个构造方法
`class Square(x:Int,y:Int,width:Int) extends java.awt.Rectangle(x,y,width,width)`
