# Scala  映射  Map - Simple 专栏 - CSDN博客
2018年09月16日 00:49:59[Simple_Zz](https://me.csdn.net/love284969214)阅读数：259标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala映射(`Map`)是一组键/值对的对象。 任何值都可以根据键来进行检索。键在映射中是唯一的，但值不一定是唯一的。映射也称为哈希表。映射有两种，不可变的和可变的。可变对象和不可变对象之间的区别在于，当对象不可变时，对象本身无法更改。
默认情况下，Scala使用不可变映射(`Map`)。如果要使用可变集合(`Set`)，则必须明确导入`scala.collection.mutable.Map`类。如果想同时使用可变的和不可变映射(`Map`)，那么可以继续引用不可变映射(`Map`)，但是可以将`mutable`集合引用`mutable.Map`。
以下是声明不可变映射(`Map`)的示例声明 -
```
// Empty hash table whose keys are strings and values are integers:
var A:Map[Char,Int] = Map()
// A map with keys and values.
val colors = Map("red" -> "#FF0000", "azure" -> "#F0FFFF")
```
在定义空的映射(`Map`)时，类型注释是必需的，因为系统需要将具体的类型分配给变量。 如果我们要向映射(`Map`)添加一个键值对，可以使用运算符`+`，如下所示 -
```
A + = ('I' -> 1)
A + = ('J' -> 5)
A + = ('K' -> 10)
A + = ('L' -> 100)
```
## 集合基本操作
映射(`Map`)的所有操作都可以用以下三种方法来表示：
|序号|方法|描述|
|----|----|----|
|1|keys|此方法返回包含映射中每个键的迭代。|
|2|values|此方法返回一个包含映射中每个值的迭代。|
|3|isEmpty|如果列表为空，则此方法返回`true`，否则返回`false`。|
尝试以下示例程序显示Map方法的用法。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      val colors = Map("red" -> "#FF0000", "azure" -> "#F0FFFF", "peru" -> "#CD853F")
      val nums: Map[Int, Int] = Map()
      println( "Keys in colors : " + colors.keys )
      println( "Values in colors : " + colors.values )
      println( "Check if colors is empty : " + colors.isEmpty )
      println( "Check if nums is empty : " + nums.isEmpty )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Keys in colors : Set(red, azure, peru)
Values in colors : MapLike(#FF0000, #F0FFFF, #CD853F)
Check if colors is empty : false
Check if nums is empty : true
```
## 连接映射
可以使用`++`运算符或`Map.++()`方法连接两个或多个映射，但在添加映射时，它将删除重复的键。
尝试以下示例程序连接两个映射。
以下是连接两个映射的例子 -
```
object Demo {
   def main(args: Array[String]) {
      val colors1 = Map("red" -> "#FF0000", "azure" -> "#F0FFFF", "peru" -> "#CD853F")
      val colors2 = Map("blue" -> "#0033FF", "yellow" -> "#FFFF00", "red" -> "#FF0000")
      // use two or more Maps with ++ as operator
      var colors = colors1 ++ colors2
      println( "colors1 ++ colors2 : " + colors )
      // use two maps with ++ as method
      colors = colors1.++(colors2)
      println( "colors1.++(colors2)) : " + colors )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
colors1 ++ colors2 : Map(blue -> #0033FF, azure -> #F0FFFF, 
   peru -> #CD853F, yellow -> #FFFF00, red -> #FF0000)
colors1.++(colors2)) : Map(blue -> #0033FF, azure -> #F0FFFF, 
   peru -> #CD853F, yellow -> #FFFF00, red -> #FF0000)
```
## 打印映射的键和值
可以使用`foreach`循环迭代映射的键和值。在这里，使用与迭代器相关联的方法`foreach`来遍历键。 以下是示例程序。
```
object Demo {
   def main(args: Array[String]) {
      val colors = Map("red" -> "#FF0000", "azure" -> "#F0FFFF","peru" -> "#CD853F")
      colors.keys.foreach{ i =>  
         print( "Key = " + i )
         println(" Value = " + colors(i) )}
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Key = red Value = #FF0000
Key = azure Value = #F0FFFF
Key = peru Value = #CD853F
```
## 查找检查映射中的键
可以使用`Map.contains`方法来测试映射中给定的键是否存在。尝试以下示例程序进行键检查。
```
object Demo {
   def main(args: Array[String]) {
      val colors = Map("red" -> "#FF0000", "azure" -> "#F0FFFF", "peru" -> "#CD853F")
      if( colors.contains( "red" )) {
         println("Red key exists with value :"  + colors("red"))
      } else {
           println("Red key does not exist")
      }
      if( colors.contains( "maroon" )) {
         println("Maroon key exists with value :"  + colors("maroon"))
      } else {
         println("Maroon key does not exist")
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Red key exists with value :#FF0000
Maroon key does not exist
```
