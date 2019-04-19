# Scala中trait - LC900730的博客 - CSDN博客
2017年12月17日 17:07:06[lc900730](https://me.csdn.net/LC900730)阅读数：292
单基础多实现，但是接口只能包含抽象方法，不能包含字段。 
trait可以同时拥有抽象方法和具体方法，而类可以实现多个trait。
## 做接口使用的trait
```
trait Logger{
    def log(msg:String)   //抽象方法
}
```
#### 不需要将方法声明为abstract—trait中未被实现的方法默认就是抽象的。
#### 在重写trait的抽象方法时候不需要给出override关键字
```
class ConsoleLogger extends Logger{   //同extends，而不是implements
    def log(msg:String){
        println(msg)   //不需要写override
    }
}
```
#### 如果你需要的trait不止一个，可以用with关键字来添加额外trait
```
class ConsoleLogger extends Logger with Cloneable with Serializable
//所有Java接口都可以作为Scala的trait使用。
```
## 带有具体实现的trait
在Scala中，trait中的方法并不一定是抽象的。
```
trait ConsoleLogger{
    def log(msg:String){
        println(msg)
    }
}
```
//使用示例
```
class SavingsAccount extends Account with ConsoleLogger{
    def withdraw(amount:Double){
        if(amount>balance) log("Insufficient funds")
        else balance -= amount
    }
}
```
## #
标准Scala库中的Logged特质
```
trait Logged{
    def log(msg:String){}
}
```
#### 构造具体对象的时候混入
### 非抽象方法需要override
```
trait ConsoleLogger extends Logged{
    ovveride def log(msg:String){
        println(msg)
    }
} 
val acct= new SavingsAccount with ConsoleLogger
```
## 叠加在一起的trait
可以为类或者对象添加多个互相调用的trait，从最后一个开始。
```
trait TimestampLogger extends Logged{
    override def log(msg:String){
        super.log(new java.util.Date()+" "+msg)
    }
}
```
```
trait shortLogger extends Logged{
    val maxLenth=15  //trait中的字段
    override def log(msg:String){
        super.log(if (msg.lenth<=maxLength) msg else msg.substring(0,maxLenth-3)+"...")
    }
}
```
实际上super.log并不是像类那样拥有相同的含义，其调用的是特质层级中的下一个trait，具体是哪一个，要根据特质添加的顺序来决定。
