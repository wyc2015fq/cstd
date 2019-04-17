# 浅析Scala单例对象与伴生类 - Machine Learning with Peppa - CSDN博客





2018年05月24日 20:45:13[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：100








## Scala的单例对象

Scala不能定义静态成员，而是代之定义单例对象(singleton object)。以object关键字定义。 
对象定义了某个类的单个实例，包含了你想要的特性：

```
object Accounts{
    private var lastNumber = 0
    def newUniqueNumber() = { lastNumber += 1; lastNumber}
}
```

当你在应用程序中需要一个新的唯一账号时，调用Account.newUniqueNumber()即可。 
对象的构造器在该对象第一次被使用时调用。

在下面几个场景下可以使用Scala单例对象： 
- 作为存放工具函数或常量的地方 
- 高效地共享单个不可变实例 
- 需要使用单个实例来协调某个服务时

类和单例对象间的差别是，单例对象不带参数，而类可以。因为单例对象不是用new关键字实例化的，所以没机会传递给它实例化参数。每个单例对象都被实现为虚拟类(synthetic class)的实例，并指向静态的变量，因为它们与Java静态类有相同的初始化语义。

## 独立对象(standalone object)

不与伴生类共享名称的单例对象称为独立对象。它可以用在很多地方，例如作为相关功能方法的工具类，或者定义Scala应用的入口点。

## 伴生对象(companion object)

当单例对象与某个类共享同一个名称时，它就被称为是这个类的伴生对象(companion object)。类和它的伴生对象必须定义在同一个源文件中。类被称为是这个单例对象的伴生类(companion class)。类和它的伴生对象可以互相访问其私有成员。

```
class Account {
    val id = Account.newUniqueNumber()
    private var balance = 0.0
    def deposit(amount: Double){ balance += amount }
    ...
}

object Account { //伴生对象
        private var lastNumber = 0
        def newUniqueNumber() = { lastNumber += 1; lastNumber}
    }
```

注意： 
- 类的伴生对象可以被访问，但并不在作用域当中。Account类必须通过Account.newUniqueNumber()来调用伴生对象的方法。 
- 在REPL中，要同时定义类和对象，必须用粘贴模式。键入`:paste`，然后键入或粘贴类和对象的定义，最后一Ctrl+D退出粘贴模式。

## 将伴生对象作为工厂使用

我们通常将伴生对象作为工厂使用。 
下面是一个简单的例子，可以不需要使用’new’来创建一个实例了。

```
class Bar(foo: String)

object Bar {
  def apply(foo: String) = new Bar(foo)
}
```

## 继承自类和特质的单例对象

一个object可以扩展类以及一个或多个特质，其结果是一个扩展了指定类以及特质的类的对象，同时拥有在对象定义中给出的所有特性。

### 继承自抽象类的例子

扩展类的一个有用的使用场景是给出可被共享的缺省对象。举例来说，考虑在程序中引入一个可撤销动作的类：

```
abstract class UndoableAction(val description: Sting) {
    def undo(): Unit
    def redo(): Unit
}

object DoNothingAction extends UndoableAction("Do nothing") {
    override def undo() {}
    override def redo() {}
}

//打开和保存功能尚未实现
val action = Map("open" -> DoNothingAction, "save" -> DoNothingAction, ...)
```

DoNothingAction对象可以被所有需要这个缺省行为的地方共用

### 混入特质的例子

有时，你可以混入像debugger或logging之类的特质来构建对象帮助调试对象，这样使得构建的对象实例具有log之类的方法：

```
trait Debugger {
    def log(message: String){
        //do something with message
    }
}

//no debugger
val child = new Child

//debugger added as the object is created
val problemChild = new ProblemChild with Debugger
```
- 



