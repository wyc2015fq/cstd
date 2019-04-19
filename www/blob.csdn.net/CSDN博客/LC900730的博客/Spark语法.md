# Spark语法 - LC900730的博客 - CSDN博客
2017年12月11日 00:07:53[lc900730](https://me.csdn.net/LC900730)阅读数：413
### =>:scala中表示匿名函数
line=>line.size：表示以=>操作符左边的部分作为输入，对其执行一个函数，并以=>操作符合右边代码执行结果为输出。此处表示以line为输入，line.size执行结果作为输出。 
string=>int:将string对象映射为int。
### count：返回RDD中记录数目
### RDD缓存策略
Spark最强大的功能之一就是把数据缓存在集群的内存中。
### Saprk广播变量和累加器
#### 广播变量为只读变量，由运行在SparkContext的驱动程序创建后发送给会参与计算的节点。对于那些需要让各节点高效访问相同数据的应用场景非常有用。
```
val broadcastAList=sc.broadcast(List("a","b","c","d","e"))
sc.parallelize(List("1","2","3")).map(x=>broadcastAList.value++x).collect
```
#### 累加器也是一种被广播到工作节点的变量。累加器与广播变量关键不同是：后者只能读取而前者可以累加，有限制的累加。
### 匿名函数
```
//参数是x，返回值是x+1，即+1操作
scala> (x:Int)=>x+1
res2:(Int)=>Int=<function1>
```
```
scala>res2(1)
res3:Int=2
```
#### 传递匿名函数
```
val addOne=(x:Int)=>x+1
addOne:(Int)=>Int=<function1>
```
```
scala>addOne(1)
res4:Int=2
```
### 如果函数有很多表达式，使用{}格式化代码
```python
def timesTwo(i:Int):Int={
    println("hello world")
    i*2
}
```
### 柯里化函数
使用下划线_部分应用一个函数，结果将得到另一个函数 
如一个乘法函数，在一个场景需要选择乘数，而在另外一个场景需要选择被乘数
```python
scala> def multiply(m:Int)(n:Int):Int=m*n
```
##### 可以直接传递2个参数
`scala>multiply(2)(3)`
##### 可以填上第一个参数并且部分应用第二个参数
```
scala>val multiply(2)_
timesTwo:(Int)=>Int=<function1>
```
`scala>timesTwo(3)`
可以对任何多参数函数执行柯里化。
### 可变长度参数
可以向方法传入任意多个同类型的参数。
```
def capitalizeAll(args:String *)={
    args.map{
        arg=>arg.capitalize
    }
}
```
```
scala> capitalizeAll("rarity","applejack")
res2:Seq[String]=ArrayBuffer(Rarity,Applejack)
```
### filter
filter移除任何对传入函数计算结果为false的元素，返回一个布尔值的函数通常称为谓词函数[或判定函数]。
```
scala>numbers.filter((i:Int)=>i%2==0)
res0:List[Int]=List(2,4)
```
```python
scala>def isEven(i:Int):Boolean=i%2==0
isEven:(i:Int)Boolean
scala>numbers.filter(isEven_)
res2:List[Int]=List(2,4)
```
### zip
将两个列表的内容聚合到一个对偶列表中
```
scala>List(1,2,3).zip(List("a","b","c"))
res0:List[(Int,String)]=List(1,a)(2,b)(3,4)
```
### partition
partition将使用给定的谓词函数分割列表
```
scala> val numbers = List(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
scala> numbers.partition(_ % 2 == 0)
res0: (List[Int], List[Int]) = (List(2, 4, 6, 8, 10),List(1, 3, 5, 7, 9))
```
List(1,2,3)
