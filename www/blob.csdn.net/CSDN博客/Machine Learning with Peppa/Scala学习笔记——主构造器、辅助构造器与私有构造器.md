# Scala学习笔记——主构造器、辅助构造器与私有构造器 - Machine Learning with Peppa - CSDN博客





2018年07月14日 17:41:30[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：183








## 辅助构造器(Auxiliary Constructor)

```
1. 辅助构造器的名称为this
2. 每个辅助构造器都必须以一个对先前已定义的其他辅助构造器或主构造器的调用开始
```

```
class Student {
    private var name = " "
    private var age = 0

    def this(name: String){     //辅助构造器1
        this()                  //调用主构造器
        this.name = name
    }
    def this(name: String,age: Int){        //辅助构造器2 
        this(name)                          //调用前一个辅助构造器
        this.age = age
    }
}
```
- 

`现在有以下三种方式实例化对象:`
```python
object Test1 {
  def main(args: Array[String]): Unit = {
    val s1 = new Student                  //主构造器
    val s2 = new Student("KK")            //辅助构造器1
    val s3 = new Student("Captian72",18)  //辅助构造器2
  }
}
```

## 主构造器(Primary Constructor)
`Scala中，每个类都有主构造器，但并不以this定义。`
1. 主构造器的参数直接放在类名之后

```
class Student(val name: String,val age: Int){   // 括号中的就是主构造器的参数
    ......
}
```
`只有主构造器的参数才会被编译成字段，其初始化的值为构造时传入的参数。`
2. 主构造器会执行类定义中的所有语句

```
class Student (val name: String,val age: Int){
    println("Primary constructor and Auxiliary constructor!")
    def fun = name + " is " + age +" years old!"
}

object Test2 {
  def main(args: Array[String]): Unit = {
    val s1 = new Student("Captian72",18)
    val s2 = new Student("KK",20)
    println(s1.fun)
    println(s2.fun)
  }
}
```
`输出为：`
```
Primary constructor and Auxiliary constructor!
Primary constructor and Auxiliary constructor!
Captian72 is 18 years old!
KK is 20 years old!
```

## 说明：

```
因为println语句是主构造器的一部分，所以在每次实例化的时候，会执行一次println。
然后fun函数被调用2次，就执行两次fun函数的内容，即输出：
Captian72 is 18 years old!
KK is 20 years old!
```

3. 在主构造器中使用默认参数，可防止辅助构造器使用过多
`class Student(val name:String="",val age: Int = 0)`
4. 如果不带val或var的参数至少被一个方法使用，它将升格为字段

```
class Dog(name:String, color:String){
    def fun = name + " is " + color + " Dog!"
}
```

上述代码声明并初始化了不可变字段name和color，并且这两个字段都是对象私有的。也就是说，类的方法，只能访问到当前对象的字段。

## 私有构造器(Private Constructor)

想要让主构造器变成私有构造器，只需要加上private关键字即可。例如：
`class Dog private(val age: Int){...}`
这样做之后，就必须使用辅助构造器来构造Dog对象了。



