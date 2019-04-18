# SCALA中类的继承 - weixin_33985507的博客 - CSDN博客
2015年04月25日 21:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
慢慢的，回想起以前学习JAVA和C#当中的的类的特性了。
感觉大同小异吧。。。
package com.hengheng.scala
class OOPInScala {
}
class Student(name: String, age : Int, val major : String) extends Person(name, age) {
  println("This is the subclass of Person, major is :" + major)
  override def toString = "Overrided toString method....."
  override val University = "TsingHua"
}
class Person(val name : String, val age : Int){
  println("This is the primary constructor!")
  var gender : String = _
  
  def this(name :String, age : Int, gender : String){
    this(name, age)
    this.gender = gender
  }
  val University = "BJU"
}
//class Person {
 // var name : String = _
  //val age = 27
  //private[this] val  gender = "male"
//}
object OOPInScala{
  def main(args: Array[String]) {
    val p = new Person("Rocky", 27, "male")
    //val p = new Person
    //p.name = "Rocky"
    println(p.name + ":" + p.age + p.gender)
//    println(p.gender)
    val s = new Student("Rocky", 27, "Computer")
    println(s.toString)
    println(s.University)
  }
}
　　输出：
This is the primary constructor!
Rocky:27male
This is the primary constructor!
This is the subclass of Person, major is :Computer
Overrided toString method.....
TsingHua
