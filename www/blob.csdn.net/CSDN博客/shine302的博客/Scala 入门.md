# Scala 入门 - shine302的博客 - CSDN博客
2018年04月22日 17:38:12[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：31
1）Scala简介与特性
Scala是一种把面向对象和函数式编程加入静态类型语言的混合体。
Scala是函数式的：
函数是头等值：与整数、字符串处于同一个地位。函数里可以接着定义函数，就像定义整数一样。
函数不应该有副作用/应该把输入值映射为输出值而不是就地更改数据。
鼓励使用不可变的数据结构和指称透明的方法
2）scala入门
2.1变量定义 val 和var。val里面的java的final变量。一旦初始化就不能更改。
2.2函数定义：def factorial (x:BigInt): BigInt =
                       if(x == 0) 1 else x*factorial(x-1)
2.3scala脚本：
2.4for 和foreach的用法：
=> 函数字面量语法，captial是参数也是函数字面量
<- 循环操作
 captial.foreach(println)
 captial.foreach(captial => println(captial))
 for(a <- captial)
      print(a)
2.5 scala没有操作符重载
2.6参数化实例对象
//使用参数实例化
    val big = new BigInteger("45455544457562")
2.7数组使用
    //使用数组,
    val testArray = new Array[String](3)
    testArray(0) = "hello"//注意取数组元素用（）
    testArray(1) = "hello"//注意取数组元素用（）
    testArray(2) = "hello"//注意取数组元素用（）
    //推荐的数组构建方式
    val testBArray = Array("A","B",5)
    //数组是可变的
    for(i <- 0 to 2)
      testBArray(i)=1
2.8 使用List
    //使用list
    val testList1 = List("1",1,"2")
    //list是不可变的
    for(i <- 0 to 2)
      println(testList1(i))
    val testList2 = List("1",1,"2")
    val testList3 = testList1:::testList2//+号不行
    val testList4 = 111 :: testList1 :: Nil //空表的缩写
2.9 使用set
![](https://img-blog.csdn.net/20180422163258303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  //set使用 不可变集合返回的是新的集合，可变集合是加入本身
    //不可变集合
    var setTest1 = Set("1","2")
    setTest1 += "lll"
    println(setTest1.contains("j"))
    //可变集合
    import scala.collection.mutable.Set//不能加在前面
    val setTest2 = Set("a","b")
    setTest2 += "c"
    println(setTest2)
set下面有Hashset和TreeSet
Hashset顾名思义里面是哈希表结构
TreeSet就是树结构
1、TreeSet 是二差树实现的,Treeset中的数据是自动排好序的，不允许放入null值。 
2、HashSet 是哈希表实现的,HashSet中的数据是无序的，可以放入null，但只能放入一个null，两者中的值都不能重复，就如数据库中唯一约束。 
3、HashSet要求放入的对象必须实现HashCode()方法，放入的对象，是以hashcode码作为标识的，而具有相同内容的 String对象，hashcode是一样，所以放入的内容不能重复。但是同一个类的对象可以放入不同的实例 。
2.10 map使用
  //map使用 map也分为不变和不可变map ，可变要用var。
    var mapTest = Map[Int,String]()
      mapTest += (1 -> "jiao")
2.11 函数式风格
尽量不要用var；不要打印到标准输出流；
2.12 从文件中读取文本行
