# Scala学习（二）---入门基础知识 - BlueSky - CSDN博客
2015年11月28日 16:09:25[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：405
## 元组使用：
元组List一样而是不能改变的结构，但是元组内每个元素的类型可以不同，List也可以但是List是推断为any，而Tuple推断为各个元素的类型。元组的作用一般为在方法里返回对个对象，元组对其内元素的操作为._来取元素（其是基于1开始索引而不是0）。
```
<span style="font-size:18px;"> val pair = (1,"Hi")
scala> print(pair._1)
1
</span>
```
## 集(set)和映射(map)
set和map都可分为可变和不可变两种特质，set可以理解为集合有很多种方法可以设置，可以参考其api，map主要是映射，常见的为<key,value>对，比如其map初始化可以为：
```
<span style="font-size:18px;">val treasureMap = Map[Int,String]()
treasureMap += (1 -> "Hello")
treasureMap += （2，“world”） //这种方式它内部会隐义转换为2 -> "world" 这种形式</span>
```
## 文件操作
文件操作主要是引用scala.io包中的Source类，比如以下脚本，实现的是对另一个文件的长度统计、格式化输入等。保存为myFile.scala
```
<span style="font-size:18px;">import scala.io.Source
def widthOfLength(s : String) = s.length.toString.length
def lengthOfLine(s:String) = s.toString.length
if (args.length>0){
  val lines = Source.fromFile(args(0)).getLines.toList  //把文件转化list并存在内存中
  val longestLine = lines.reduceLeft((a,b) => if(a.length>b.length) a else b)
  val maxWidth = widthOfLength(longestLine)
  val maxLength = lengthOfLine(longestLine)
  val sparkWidth = maxLength+maxWidth+2
  println("*"*(sparkWidth))
  for(line <- lines){
    val numSpaces = maxWidth-widthOfLength(line)
    val padding = " " * numSpaces
    val paddingLast = " "*(sparkWidth-maxWidth-line.length-2)
    println(padding+line.length + "|"+line+paddingLast+"*")
  }
  println("*"*(sparkWidth))
}
</span>
```
运行 scala myFile.scala asia
![](https://img-blog.csdn.net/20151128160618853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
