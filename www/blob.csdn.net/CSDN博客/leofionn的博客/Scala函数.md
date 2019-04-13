
# Scala函数 - leofionn的博客 - CSDN博客


2018年03月05日 14:23:16[leofionn](https://me.csdn.net/qq_36142114)阅读数：39个人分类：[spark																](https://blog.csdn.net/qq_36142114/article/category/7640197)



scala中函数编写的方式如下：

1.规范化写法,scala 函数的返回值是最后一行代码；
def addInt(a:Int,b:Int) : Int = {
var total : Int = a + b
return total
}
//Unit，是Scala语言中数据类型的一种，表示无值,用作不返回任何结果的方法；
def returnUnit(): Unit = {
println("ZST loves basketball !")
}

2.不写明返回值的类型，程序会自行判断，最后一行代码的执行结果为返回值；
def addInt(a:Int,b:Int) = {
a+b
}

3。省略返回值类型和等于号，返回的是（）；
def addInt(a:Int,b:Int){
a+b
}

4.函数只有一行的写法；
def addInt (a:Int,b:Int) = x + y

5.最简单写法：def ,{ },返回值都可以省略，此方法在spark编程中经常使用。
val addInt = (x: Int,y: Int) => x + y
表示定义函数 addInt ，输入参数有两个分别为x,y，且均为Int类型，返回值为两者的和，类型为Int


【来自@若泽大数据】


