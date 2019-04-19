# Scala中类private - LC900730的博客 - CSDN博客
2017年12月17日 15:13:53[lc900730](https://me.csdn.net/LC900730)阅读数：717
类中属性没有修饰符，默认public
```
class  Counter{
  var value=0
  def increment(){value+=1;println("hello world")}
  def isLess(other:Counter)=value<other.value
  //def current=value
}
val n=new Counter
val n1=new Counter
n1.increment()
println(n1.value)
println(n.isLess(n1))
```
如果是 
private val value=0; 
那么n1.value就会报错
Scala对每个字端都提供geter与setter方法。
```php
class Person{
      var age=0 
}
```
![这里写图片描述](https://img-blog.csdn.net/20171217123557815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编译器创建了age和age_$eq方法(=号被翻译成$eq,是因为JVM不允许方法名中出现=) 
在Scala中，getter与setter方法并不是被命名为getXXX，和setXXX。
任何时候都可以自己重新定义getter和setter方法。
```
class Person{
    private var privateAge=0
    def age=privateAge
  def age_=(newValue:Int): Unit ={
    if(newValue>privateAge) privateAge=newValue
  }
}
```
scala为每个字段生成getter与setter方法。
- 如果字段是私有的，那么setter与getter也是私有的
- 如果字段是val，那么只有getter方法被生成
- 如果不需要任何getter与setter，那么将字段声明为private
![这里写图片描述](https://img-blog.csdn.net/20171217124434350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 只带getter的属性
如果需要只读属性，那么声明为val，Scala生成一个私有的final字段和一个getter方法，但是没有setter。
