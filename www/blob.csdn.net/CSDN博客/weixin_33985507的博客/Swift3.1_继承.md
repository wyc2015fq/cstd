# Swift3.1_继承 - weixin_33985507的博客 - CSDN博客
2018年02月22日 00:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# 简介
一个类可以继承另一个类的方法，属性和其它特性。当一个类继承其它类时，继承类叫子类，被继承类叫超类（或父类）。
在`Swift`中，类可以调用和访问超类的方法，属性和下标，并且可以重写这些方法，属性和下标来优化或修改它们的行为。`Swift`会检查你的重写定义在超类中是否有匹配的定义，以此确保你的重写行为是正确的。
### 定义一个基类
`Swift`中的类并不是从一个通用的基类继承而来。如果你不为你定义的类指定一个超类的话，这个类就自动成为基类。
```
class Vehicle {
    var currentSpeed = 0.0
    var description: String {
        return "traveling at \(currentSpeed) miles per hour"
    }
    func makeNoise() {
        // 什么也不做-因为车辆不一定会有噪音
    }
}
```
### 子类生成
子类生成指的是在一个已有类的基础上创建一个新的类。子类继承超类的特性，并且可以进一步完善。你还可以为子类添加新的特性。
为了指明某个类的超类，将超类名写在子类名的后面，用冒号分隔：
```
class Bicycle: Vehicle {
    var hasBasket = false
}
```
新的`Bicycle`类自动获得`Vehicle`类的所有特性，比如`currentSpeed`和`description`属性，还有它的`makeNoise()`方法。
```
let bicycle = Bicycle()
bicycle.hasBasket = true
bicycle.currentSpeed = 15.0
```
子类还可以继续被其它类继承，下面的示例为`Bicycle`创建了一个名为`Tandem`（双人自行车）的子类：
```
class Tandem: Bicycle {
    var currentNumberOfPassengers = 0
}
```
### 重写
子类可以为继承来的实例方法，类方法，实例属性，或下标提供自己定制的实现。我们把这种行为叫重写。
如果要重写某个特性，你需要在重写定义的前面加上`override`关键字。这么做，你就表明了你是想提供一个重写版本，而非错误地提供了一个相同的定义。意外的重写行为可能会导致不可预知的错误，任何缺少`override`关键字的重写都会在编译时被诊断为错误。
###### 访问超类的方法，属性及下标
你可以通过使用`super`前缀来访问超类版本的方法，属性或下标：
- 在方法`someMethod()`的重写实现中，可以通过`super.someMethod()`来调用超类版本的`someMethod()`方法。
- 在属性`someProperty`的`getter`或`setter`的重写实现中，可以通过`super.someProperty`来访问超类版本的`someProperty`属性。
- 在下标的重写实现中，可以通过`super[someIndex]`来访问超类版本中的相同下标。
###### 重写方法
在子类中，你可以重写继承来的实例方法或类方法，提供一个定制或替代的方法实现。
```
class Train: Vehicle {
    override func makeNoise() {
        print("Choo Choo")
    }
}
```
###### 重写属性
- 重写属性的 Getters 和 Setters
- 重写属性观察器
### 防止重写
你可以通过把方法，属性或下标标记为`final`来防止它们被重写。
如果你重写了带有`final`标记的方法，属性或下标，在编译时会报错。在类扩展中的方法，属性或下标也可以在扩展的定义里标记为 `final`的。
你可以通过在关键字`class`前添加`final`修饰符来将整个类标记为 `final`的。这样的类是不可被继承的，试图继承这样的类会导致编译报错。
