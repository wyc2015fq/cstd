# Scala两行代码实现回文数的生成与判断 - Machine Learning with Peppa - CSDN博客





2018年08月01日 23:06:18[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：235








今天练习Scala发现一个好玩的东西。之前用C自己写过回文数的判断，大概用了20多行。在Scala里可以通过视图高效的截取任意范围内的回文数，只需要1-3行代码即可。

```java
val palindrom = (1 to 100000).view.map(
 		x => x * x).filter(x => x.toString == x.toString.
 		reverse && x > 10)       
         
//> palindrom  : scala.collection.SeqView[Int,Seq[_]] = SeqViewMF(...)
 		
 	palindrom.take(10).mkString(",")          

//> res5: String = 121,484,676,10201,12321,14641,40804,44944,69696,94249
```

如果要从输入的数判断是否为回文数或者找出回文数，则可以将第一句改为

```java
val palindrom =  Input_number.view.filter(x => x.toString == x.toString.
 		reverse && x > 10)
```

如果是回文字符串的话更简单：

```java
def palind_str(x:String):Unit ={ if (x == x.reverse) println("yes")  else println("no")}
```





