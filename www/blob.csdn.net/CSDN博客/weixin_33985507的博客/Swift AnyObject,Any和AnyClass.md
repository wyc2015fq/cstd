# Swift:AnyObject,Any和AnyClass - weixin_33985507的博客 - CSDN博客
2017年12月23日 16:11:10[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

Swift 3.0 和 Swift 2.0 写法对比
Any和AnyObject
这两个类型都是Swift中很早就出现的类型，但是我们经常使用AnyObject,很少使用Any。
AnyObject类似于OC中的id类型，表示任意的class的实例对象，但是在Swift中，例如我们常见的String和Array都变为结构体了，而且在Swift3.0中，更多的类型或者枚举被写为结构体了，AnyObject的适用范围变相被削弱了，所以在Swift3.0的API中曾经许多AnyOjbect的类型被替换为Any了。
当然，这对于我们使用这些API没有影响，但是在我们自己定义方法时，如果需要用到AnyObject，就需要认真考虑一下该用AnyObject还是Any了。
AnyObject
AnyObject是个什么玩意呢? 有什么用呢?
我们点进其头文件看一下, 发现@objc public protocol AnyObject { }, 这个单词protocol大家都认识吧, 对AnyObject是一个协议, 它的注释第一行有这么一句话:The protocol to which all classes implicitly conform.意思是所有的类都隐式遵守了这个协议.
AnyObject有什么用呢?有过Objective-C开发经验的人肯定知道id, 它可以表示任意类的实例, 编译器不会对向声明为 id 的变量进行类型检查.而 Swift为了与 Cocoa 架构进行协作开发，就将原来的id用 AnyObject 来进行替代。
AnyObject的自动类型转换
Objective-C的NSArray里面只能保存对象类型, 不能添加基本数据类型, 枚举,结构体等.但是如果我们在Swfit里面将Array的元素类型声明成AnyObject会发生什么呢? 我们定义一个数组:let array:[Any] = ["123", 123], 在其下面插入一个断点, 如下图所示
截图.png
没错, 它将基本数据类型`int`转换成了Objective-C中的`NSNumber`对象,
所以在Swift中, 我们将基本数据类型或者enum和struct通过AnyObject来保存也不会像Objective-C那样报错, 因为Swift中很多数据类型可以和Objective-C中的数据类型进行自动转换
Any
Any是什么? 同上, 我们进入Any所在的头文件:public typealias Any = protocol<>, 关于type alias, 大家可以进入喵神的[TYPEALIAS 和泛型接口](https://link.juejin.im?target=http%3A%2F%2Fswifter.tips%2Ftypealias%2F)这篇文章进行详细了解. 我们直接看Any的注释:/// The protocol to which all types implicitly conform., (小tip:///在Swift中就是文档注释), 意思是所有的类型, 都隐式的遵守这个协议.
说得更直白一点, 就是所有的类型都可以用Any表示, 包括基本数据类型, enum, struct, func(方法)等等.
此时, 我们和AnyObject, 我们也定义一个数组, 用Any类型保存:let array: [Any] = ["123", 123], 同样插入一个断点, 如图:
截图.png
显而易见, `Any `并不会如同`AnyObject `一样进行数据类型的转换
AnyClass
AnyClass: 表示任意类的元类型.它所在的头文件这样写道:public typealias AnyClass = AnyObject.Type, 它的注释第一句就指明了:/// The protocol to which all class types implicitly conform., 任意类的类型都隐式遵守这个协议.AnyObject.Type中的.Type就是获取元类型, 辟如你有一个Student类,Student.Type就是获取Student的元类型.
如果一个变量/常量的类型是AnyClass,AnyClass并不知道其具体的类型, 所以需要将他转换为已知的类型, 然后再调用init()方法创建对象
