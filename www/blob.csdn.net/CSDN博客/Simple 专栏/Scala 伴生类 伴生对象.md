# Scala 伴生类 伴生对象 - Simple 专栏 - CSDN博客
2018年11月16日 18:50:48[Simple_Zz](https://me.csdn.net/love284969214)阅读数：49
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
```
object AccompanyObject {
  def main(args: Array[String]): Unit = {
    println(ScalaPerson.sex)  //true 在底层等价于 ScalaPerson$.MODULE$.sex()
    ScalaPerson.sayHi()       //在底层等价于 ScalaPerson$.MODULE$.sayHi()
  }
}
//伴生类
class ScalaPerson { //
  var name : String = _
}
//伴生对象
object ScalaPerson { //
  var sex : Boolean = true
  def sayHi(): Unit = {
    println("object ScalaPerson sayHI~~")
  }
}
```
- 当在同一个文件中，有 calss 和 object
- class 称为伴生类，将非静态的内容写到该类中
- object 称为半生对象，将静态的内容写入到该对象（类）
- class 编译后底层生成 ScalaPerson 类 ScalaPerson.class
- object 编译后底层生成 ScalaPerson$类 ScalaPerson$.class
- 对于半生对象的内容，我们可以直接通过 ScalaPerson . (属性 或者 方法) 访问。
![](https://img-blog.csdnimg.cn/20181116184536509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=,size_16,color_FFFFFF,t_70)
```java
// 伴生类源码
public class ScalaPerson 
{
  private String name;
  public static void sayHi()
  {
    ScalaPerson..MODULE$.sayHi();
  }
  public static void sex_$eq(boolean paramBoolean)
  {
    ScalaPerson..MODULE$.sex_$eq(paramBoolean);
  }
  public static boolean sex()
  {
    return ScalaPerson..MODULE$.sex();
  }
  public String name()
  {
    return this.name; 
  } 
  public void name_$eq(String x$1) { this.name = x$1; }
}
```
```java
// 伴生对象源码
public final class ScalaPerson$
{
  public static final  MODULE$; // 就是 ScalaPerson$实例
  private boolean sex;
  static
  {
    new ();
  }
  public boolean sex()
  {
    return this.sex; 
  } 
  public void sex_$eq(boolean x$1) { this.sex = x$1; } 
  public void sayHi() {
    Predef..MODULE$.println("object ScalaPerson sayHI~~");
  }
  private ScalaPerson$() {
    MODULE$ = this;
    this.sex = true;
  }
}
```
### 伴生对象的小结
- Scala 中伴生对象采用 object 关键字声明，伴生对象中声明的全是 **“静态” **内容，可以通过** 伴生对象名称 **直接调用。
- 伴生对象对应的类称之为伴生类，伴生对象的名称应该和伴生类名一致。
- 伴生对象中的属性和方法都可以通过 **伴生对象名(类名)直接调用** 访问。
- 从语法角度来讲，所谓的伴生对象其实就是类的静态方法和成员的集合。
- 从技术角度来讲，scala还是没有生成静态的内容，只不过是将伴生对象生成了一个新的类，实现属性和方法的调用。
- 从底层原理开开，伴生对象实现静态特性是依赖于 public static final MODULE**$** 实现的。
- 伴生对象的生命应该和伴生类的生命在同一个源码文件中(如果不在同一个文件中会 ！！！运行错误 ！！！)，但是如果没有伴生类，也就是没有所谓的伴生对象，所以放在那里都无所谓。
- 如果 class A 独立存在，那么A就是一个类，如果 object A 独立存在，那么 A 就是一个 "静态" 性质的对象 【即类对象】，在 object A 中声明的属性和方法可以通过 A.属性 和 A.方法 来实现调用。
- 
当一个文件中，存在伴生类和伴生对象时，idea 文件的图标会发生变化。
![](https://img-blog.csdnimg.cn/2018111618364344.png)
