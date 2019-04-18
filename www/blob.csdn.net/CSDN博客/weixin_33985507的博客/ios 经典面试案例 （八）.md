# ios 经典面试案例 （八） - weixin_33985507的博客 - CSDN博客
2018年06月05日 14:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
## Swift mutating关键字的使用？
在Swift中，包含三种类型(type): **structure,enumeration,class**
其中**structure**和**enumeration**是值类型(value type),**class**是引用类型(reference type)
但是与Objective-C不同的是，structure和enumeration也可以拥有方法(method)，其中方法可以为实例方法(instance method)，也可以为类方法(type method)，实例方法是和类型的一个实例绑定的。
##### 在swift官方教程中有这样一句话：
“Structures and enumerations are value types. By default, the properties of a value type cannot be modified from within its instance methods.”
大致意思就是说，虽然结构体和枚举可以定义自己的方法，但是默认情况下，实例方法中是不可以修改值类型的属性。
##### 1. 在结构体的实例方法里面修改属性
```
struct Persion {
    var name = ""
    mutating func modify(name:String) {
        self.name = name
    }
}
```
##### 2. 在协议里面， 如何继承的结构体或枚举类型，想要改遍属性值， 必须添加**mutating**
```
protocol Persionprotocol {
    var name : String {get}
    mutating func modify(name:String)
}
struct Persion : Persionprotocol {
    var name = ""
    mutating func modify(name:String) {
        self.name = name
    }
}
```
##### 3. 在枚举中直接修改self属性
```
enum Switch {
    case On, Off
    
    mutating func operatorTion() {
        switch self {
        case .On:
            self = .Off
        default:
            self = .On
        }
    }
}
var a = Switch.On
a.operatorTion()
print(a)
```
### 题的搬运，不成敬意！
