# Scala 内建控制对象（if、while、for、异常、match、beak） - shine302的博客 - CSDN博客
2018年05月03日 15:14:09[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：83
**if表达式：**与其他语言无区别
**while表达式：**
var line = ""
while((line = readLine()) != ""){
}
本质为（unit ！= String）因此循环永远无法执行。
使用var和while是指令式的编程方式。可以将while转换为递归的形式。
  // 函数式编程while（用递归代替）
   def gcd(x : Long , y : Long) : Long =
    if ( y ==0) x else gcd(y,x%y)
**for 表达式：**
    for(1 <- 1 to 4){// <- 右侧必须是list，不能是数字
    }
    for(1 <- 1 until 4){//这种方式不包含上界
    }
    //制造新集合
    def testyield = //不需要定义类型
    for(arrayitem <- testArray if(arrayitem == "hello1"))yield arrayitem
    testyield.foreach(println)
**try表达式处理异常：**
//抛出异常
    val n = 91
    val half =
      if ( n % 2 == 0 )
        n/2
      else
        throw  new RuntimeException("nihao")
    try {
      val f = new FileReader("input.txt")
    }catch{
      case ex: FileNotFoundException => println("meiyou zhaoda")
      case ex: IOException => println("IO")
    }finally {
      println("a")
    }
    // 神奇与java不一样
    def g(): Int = try {1} finally {2}
    println(g())
**match 表达式：**
 val matchtest = "test"
    matchtest match {
      case "nihao" => println();
      case _ => println(matchtest)//表达未知值的通配符
    }
    //生成值
    val matchtest2 = matchtest match {
      case "nihao" => "nihao"
    }
**不再使用break和continue**
continue用if代替，break用布尔类型代替
例如找到一个以 .scala结尾的文件，和以不以A开头的文件
**变量范围（允许嵌套范围内定义同名变量）**
 val a = 1;
    {
      val a = 2
      println(a)
    }
    println(a)

