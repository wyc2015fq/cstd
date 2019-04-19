# swift语言实现单例模式 - 月若无涯的专栏 - CSDN博客
2014年06月06日 09:48:45[月若无涯](https://me.csdn.net/u010124617)阅读数：4285
#### Swift实现单例模式
单例模式在各个语言中都有实现，swift语言推出已经几天了，通过这几天的看文档，特奉上写的Swift的单例实现，供大家学习交流，欢迎指正。
---若转载请注明出处，本人Github博客新地址- YueRuo's Blog - [http://yueruo.github.io](http://yueruo.github.io/) ---
由于Swift语言弱化了struct和class之间的界限，这里我分别给出自己写的两种的单例实现
###### class版本：
```
class SwiftSingleton{
    class func shareInstance()->SwiftSingleton{
        struct YRSingleton{
            static var predicate:dispatch_once_t = 0
            static var instance:SwiftSingleton? = nil
        }
        dispatch_once(&YRSingleton.predicate,{
                YRSingleton.instance=SwiftSingleton()
            }
        )
        return YRSingleton.instance!
    }
}
```
对于单例类，需要一个唯一的对外输出实例的shareInstance方法，而通过官方文档的查阅，发现对于class来说，静态方法可以用`class func` 来标示，静态变量使用`class var`来处理，但这里我借助了内部struct的静态变量来对唯一的instance进行存储。 调用时，可以使用
```
var swiftInstance1=SwiftSingleton.shareInstance()
var swiftInstance2=SwiftSingleton.shareInstance()
if swiftInstance1===swiftInstance2{//“===”判别是否是同一个实例对象
    println("they are the same instance!")
}
```
另外，上面使用到了dispatch_once，有过GCD编程经验的应该会非常熟悉，可以保证线程安全，以及只会被调用一次。
###### struct版本
struct版本与class版本几乎一致，唯一区别在于对于func使用的关键字由`class func`变为 `static func`
```
struct StructSingleton{
    static func shareInstance()->StructSingleton{
        struct YRSingleton{
            static var predicate:dispatch_once_t = 0
            static var instance:StructSingleton? = nil
        }
        dispatch_once(&YRSingleton.predicate,{
                YRSingleton.instance=StructSingleton()
            }
        )
        return YRSingleton.instance!
    }
}
```
