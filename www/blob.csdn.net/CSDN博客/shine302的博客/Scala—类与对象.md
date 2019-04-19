# Scala—类与对象 - shine302的博客 - CSDN博客
2018年04月22日 22:26:38[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：18
类的定义：
class DiDi {
  val parameter1 = 0
}
类的初始化：
val didi1 = new DiDi
val didi2 = new DiDi
didi1.parameter1 = 1
![](https://img-blog.csdn.net/20180422181221249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
/**
 * Created by jiaoshuhai on 2018/4/22.
 */
class DiDi {
  var parameter1 = 0 //不写权限的就是public
  private var parameter2 = 1 //任何类从外部访问参数2都会被拒绝。
  def add1(b:Byte) : Unit = { //方法参数都是val ，不能重命名b
    parameter2 += b
  }
  def add2(b:Byte) {parameter2 += b}//没有等号，返回值为unit
  def add3(b:Byte): Int = ~(parameter2&0xFF) + 1
  def g(){"jiaoshuhai"}
  def j()={"jiaoshuhai"}
}
//scala中没有静态成员（类的所有对象共用一个静态成员），却而代之的是伴生对象和单例对象。可以互相访问私有成员。
object DiDi{
}
application特性
**object **ScalaTest {//extends App
