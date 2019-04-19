# Swift3.0语法变化 - game3108的专栏 - CSDN博客
2016年10月25日 18:51:11[game3108](https://me.csdn.net/game3108)阅读数：394
# 最近在看swift3的官方文档，学习swift3，之前没怎么看过swift2，看了这篇文章，感觉swift的变化还是挺大的，也许还要再过两年才能稳定下来吧。
原文链接：http://www.jianshu.com/p/460b5424942a
# 写在前面
首先和大家分享一下学习新语法的技巧：
用Xcode8打开自己的Swift2.3的项目,选择Edit->Convert->To Current Swift Syntax… 让Xcode帮我们把Swift2.3的代码转换为Swift3.0。
![](http://upload-images.jianshu.io/upload_images/1244207-7f1a6fa46299e9a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
手动调出Xcode自动转换Swift2.3 到 Swift3.0
弹出语言版本选择界面，选择Covert to Swift3，Next：
![](http://upload-images.jianshu.io/upload_images/1244207-188a30ab077d4048.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
AAB5FC3D-3EF2-43D5-9328-C00E33B3109D.png
进入选择模块界面：
![](http://upload-images.jianshu.io/upload_images/1244207-436073fa1c19fa62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
选择模块界面
建议只选择自己创建的模块，第三方框架的模块最好不要使用Xcode来转换，等待第三方作者更新。
进入转换界面：
![](http://upload-images.jianshu.io/upload_images/1244207-4f726bbbcd506285.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
转换界面
不要着急Save，在这个界面中详细的列出了各个语法具体变化，我们可以利用这个界面来快速学习自己项目中遇到语法变化。
好了，下面给大家分享一下我的遇到的语法变化。
# 常用类及方法的Swfit风格化
## UIColor
将常用的标准颜色写成了只读属性，不再是方法，调用方法改变。
![](http://upload-images.jianshu.io/upload_images/1244207-d2a9577509789730.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 2.3 UIColor
![](http://upload-images.jianshu.io/upload_images/1244207-8c2b64058b974411.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 UIColor
![](http://upload-images.jianshu.io/upload_images/1244207-07d0313f6cd45673.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.0 写法对比
## Any和AnyObject
这两个类型都是Swift中很早就出现的类型，但是我们经常使用AnyObject,很少使用Any。
AnyObject类似于OC中的id类型，表示任意的class的实例对象，但是在Swift中，例如我们常见的String和Array都变为结构体了，而且在Swift3.0中，更多的类型或者枚举被写为结构体了，AnyObject的适用范围变相被削弱了，所以在Swift3.0的API中曾经许多AnyOjbect的类型被替换为Any了。
当然，这对于我们使用这些API没有影响，但是在我们自己定义方法时，如果需要用到AnyObject，就需要认真考虑一下该用AnyObject还是Any了。
![](http://upload-images.jianshu.io/upload_images/1244207-0e66496a372b2a14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.0 写法对比
## BOOL属性的命名规则
在OC中，官方建议我们将BOOL属性的getter方法命名为isXXX,Swift中由于只是将原有OCUIKit框架进行Swift转换，所以这个规则在之前是Swift中并没有体现。在Swift3.0中，这个规则被再次应用，所有的BOOL类型都重新命名为isXXX,所以以后我们的自定义类中BOOL属性的命名也应体现这个规则。
![](http://upload-images.jianshu.io/upload_images/1244207-10b20bc7d2550a3e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
布尔类型的属性get方法改变
## Foundation框架部分类名去掉NS前缀
包括：UserDefaults、URL、NotificationCenter、Bundle、Timer、Thread、RunLoop
![](http://upload-images.jianshu.io/upload_images/1244207-683a13e03f003fdb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.3 写法对比
## 常用系统提供单例类的获取方法Swift风格化
![](http://upload-images.jianshu.io/upload_images/1244207-61c64e6430700479.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.3 写法对比
## 常用结构体的构造方法改变
常用的结构体有：CGSize、CGPoint和CGRect。
![](http://upload-images.jianshu.io/upload_images/1244207-d1bd0a18ca0ec708.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.3 写法对比
Swift2.3中，使用构造方法和make函数都可以创建；
```cpp
```cpp
// Make函数创建
 let _ = CGSizeMake(10, 20)
// 构造方法创建
 let _ = CGSize(width: 10, height: 20)
```
```
Swift3.0中，废弃make函数，只能使用构造方法创建。
```cpp
```cpp
// 只能使用构造方法创建
let _ = CGSize(width: 10, height: 20)
```
```
## 转变为结构体的类
在之前的Swift版本中，苹果引入了String、Array和Dictionary这三个结构体来代替OC中的NSString、NSArray和NSDictionary这三个类，当然这三个OC类依然可以使用。但是在平时的开发使用中，Swift的这三个结构体使用起来更方便，大部分情况下效率更高。
在Swift3.0中，苹果又推出了以下新的结构体，原有OC类依然可以使用。并且可以相互转化。
![](http://upload-images.jianshu.io/upload_images/1244207-426759b360d8f442.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
新增结构体类型及对应的OC类型
## 通知的变化
![](http://upload-images.jianshu.io/upload_images/1244207-2ede8862f1ba4f41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 和 Swift 2.3 写法对比
Swift 3.0 中NSNotification和Notification创建时，通知的name参数类型都变为“Notification.Name”类型，该类型创建比较复杂。
```
// Swift3.0中的通知
let _ = NSNotification(name: NSNotification.Name(rawValue: "name"), object: nil)
let _ = Notification(name: NSNotification.Name(rawValue: "name"))
```
## UIViewController 返回是否显示状态栏的方法变化
![](http://upload-images.jianshu.io/upload_images/1244207-c3b762d5f6d2f5d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
控制器方法改为属性
## 获取string的字符串长度方法的改变
![](http://upload-images.jianshu.io/upload_images/1244207-d4b61e0c97b3bff5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
获取字符串长度参数改变
## 获取沙盒指定文件夹路径的方法变化
获取文件路径统一交给FileManager来管理
![](http://upload-images.jianshu.io/upload_images/1244207-70249ca8d9214e10.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
获取沙盒路径参数改变
# Swift3.0中GCD语法的改变
Swift3.0中GCD写起来更简洁了。
![](http://upload-images.jianshu.io/upload_images/1244207-160a53e9be38e4a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
GCD语法改变
延迟执行的代码转换的不够好。应该这样写：
```php
```php
// 延迟执行代码
DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + 5) {
    print("2324")
}
```
```
# Swfit的关键字的变化
## private和fileprivate
- private: 私有属性和方法，仅在当前类中可以访问，不包括分类；
- 
fileprivate: 文件内私有属性和方法，仅在当前文件中可以访问，包括同一个文件中不同的类。
```
/// 以下所有的类都在同一个文件中
class Test: NSObject {
  // 只能在当前大括号内访问
  private var value: Int = 0
  // 只能在当前文件内访问
  fileprivate var value1: Int = 0  
  // 只能在当前大括号内访问
  private func privatePractise() {
      value = 1
      value1 = 1
      fileprivatePractise()
      fileprivatePractise1()
      print("privatePractise方法被调用了")
  }
  // 只能在当前文件内访问
  fileprivate func fileprivatePractise() {
      privatePractise()
      fileprivatePractise()
      fileprivatePractise1()
      print("fileprivatePractise方法被调用了")
  }
}
extension Test {
  // 只能在当前大括号内访问
  private func privatePractise1() {
      value1 = 1
      fileprivatePractise()
      fileprivatePractise1()
      print("privatePractise方法被调用了")
  }  
  // 只能在当前文件内访问
  fileprivate func fileprivatePractise1() {
      privatePractise1()
      fileprivatePractise()
      print("fileprivatePractise方法被调用了")
  }
}  
class Test2: NSObject {
  func test() {
      let t = Test()
      t.value1 = 0
      t.fileprivatePractise()
      t.fileprivatePractise1()
  }
}
```
## public和open
### 在Swift2.3中，pubic有两层含义：
- 这个元素可以在其他作用域被访问
- 这个元素可以在其他作用域被继承或者override
继承是一件危险的事情。尤其对于一个framework或者module的设计者而言。在自身的module内，类或者属性对于作者而言是清晰的，能否被继承或者override都是可控的。但是对于使用它的人，作者有时会希望传达出这个类或者属性不应该被继承或者修改。这个对应的就是 final。
final的问题在于在标记之后，在任何地方都不能override。而对于lib的设计者而言，希望得到的是在module内可以被override，在被import到其他地方后其他用户使用的时候不能被override。
这就是 open产生的初衷。通过open和public标记区别一个元素在其他module中是只能被访问还是可以被override。
### 在Swift3.0中
- public表示当前类、属性或者方法只能在当前module内被继承或者override，在当前module意外只能被访问；
- open表示当前类、属性或者方法可以在任何地方被继承或者override；
- final是一个辅助修饰词，表示当前类、属性或者方法在任何地方都只能被访问，不能被继承或者override；
- internal表示默认级别。
```
/// ModuleA:
import UIKit                        
/// 这个类在ModuleA的范围外是不能被继承的，只能被访问
public class NonSubclassableParentClass: NSObject {
  // 这个方法在ModuleA的范围外只能被访问，不能被override
  public func test() {
      print("test")
  }
  //这是错误的写法，因为class已经不能被集成，所以她的方法的访问权限不能大于类的访问权限
  open func bar() {
      print("bar")
  }
  // 这个方法在任何地方都只能被访问，不能被override
  public final func baz() {
      print("baz")
  }
}
/// 在ModuleA的范围外可以被继承
open class SubclassableParentClass: NSObject {
  // 这个属性在ModuleA的范围外只能被访问，不能被override
  public var size: Int = 0
  // 这个方法在ModuleA的范围外只能被访问，不能被override
  public func foo() {
      print("foo")
  } 
 // 这个方法在任何地方都可以被override
  open func baz() {
      print("baz")
  }    
  // 这个方法在任何地方都只能被访问，不能被override
  public final func bar() {
      print("bar")
  }
}
/// 这个类在任何地方都不能被继承
public final class FinalClass {
}
```
## 总结
Swfit3.0中，访问控制权限由高到低依次为：open、public、internal（默认）、fileprivate，private。
## Swift3.0中if…where和guard…where的变化
Swift3.0中对where关键字的使用场景进行了一些调整，在Swift2.3中，我们常这样写：
```
// Swift2.3
var value: Int?
var num: Int?
if let v = value, n = num where v > n {
     print("value > num")
}
value = 1
num = 2
guard let v = value, n = num where v > n else {
     print("value < num")
     return
}
```
在Swift3.0中，应该这样实现：
```
// Swift3.0
var value: Int?
var num: Int?
if let v = value, let n = num, v > n {
    print("value > num")
}
value = 1
num = 2
guard let v = value, let n = num, v > n else {
    print("value < num")
    return
}
```
# Swift3.0中枚举的变化
在Swift2.3中，官方使用的枚举值首字母使用大写，在Swift3.0中，统一将官方使用的枚举值首字母改为了小写。虽然自定义的枚举中枚举值首字母依然可以使用大写，但是为了和官方风格保持一致，建议枚举值首字母使用小写。
```cpp
```cpp
/// 这种写法是正确的（与官方风格一致，推荐使用）
enum Direction: String {
    case east   = "east"
    case south  = "south"
    case west   = "west"
    case north  = "north"
}
/// 这种写法也是正确的（与官方风格不一致，不推荐使用）
enum Sex: Int {
    case Man    = 0
    case Woman  = 1
    case Other  = 2
}
```
```
# Swift3.0中方法名的Swift风格化
在Swift的方法命名规则中，参数有两个名称，一个内部名，一个外部名。当参数有外部名时，方法调用时只显示外部名，若无外部名，则默认外部名和内部名相同。
![](http://upload-images.jianshu.io/upload_images/1244207-71c5359bce48b5c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
外部名和内部名
在Swift2.3中，第一个参数若没有外部名，则调用时候常省略。对于常用的UIKit和Foundation框架来说，Swift2.3中的方法名称依然是OC语言的风格。
![](http://upload-images.jianshu.io/upload_images/1244207-518c03a532bc1de1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift2.3 方法名称风格
在Swift3.0中，第一个参数若没有外部名，则调用时显示内部名，不省略。同时将常用的UIKit和Foundation框架的方法名进行了Swift风格化，使方法调用时更简洁清晰。
![](http://upload-images.jianshu.io/upload_images/1244207-40684bb6125e2f57.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift3.0 方法名称风格
两种风格方法调用对比：
![](http://upload-images.jianshu.io/upload_images/1244207-141f891d0cc361e5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dismiss方法swift风格化
建议以后自定义方法时，风格尽量和Swift3.0保持一致。
在Swift3.0 编译器环境下两种风格对比：
![](http://upload-images.jianshu.io/upload_images/1244207-dfce7fdc2cb084e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自定义方法两种风格对比
# Swift3.0中selecter的Swift风格化
在Swift2.2中，当我们为一个按钮添加点击事件时常常这样写：
![](http://upload-images.jianshu.io/upload_images/1244207-22ae144519782adf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 2.3 中 Selector写法
在Swift2.2更新到Swift2.3后可以看到警告告诉我们这是一个OC风格的写法，建议改为Swift风格的写法。
在Swift3.0中两种写法依然都可以使用，但是建议统一写为Swift风格的，因为你不知道什么时候OC风格的就不被允许了。
![](http://upload-images.jianshu.io/upload_images/1244207-0e949c2c819fc0ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0 中 Selector写法
# 运算符的变化
- Swift3.0中运算符的左右两边必须不能为optional。
- ++和--是继承自C语言中的运算符，在Swift3.0中被移除，建议使用 x += 1来代替。
![](http://upload-images.jianshu.io/upload_images/1244207-dab6459e1d4e4c9b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自加自减运算符的变化
文／光无影（简书作者）
原文链接：http://www.jianshu.com/p/460b5424942a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
