# NSObject：类与协议(NSObject: the Class and the Protocol) - game3108的专栏 - CSDN博客
2016年04月19日 10:52:13[game3108](https://me.csdn.net/game3108)阅读数：261
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
原文链接：https://www.mikeash.com/pyblog/friday-qa-2013-10-25-nsobject-the-class-and-the-protocol.html
译文链接：http://www.cocoachina.com/ios/20131203/7463.html
**读者 Tomas Bouda asks问：NSObject协议是什么的？**
在Cocoa里有两个NSObject，一个是类，一个是协议。为什么有两个？用来做什么？我将通过这篇文章来解答这些问题。
**命名空间**
首先， 我们来看看这两个有着相同命名的部分是怎么共存的。NSObject的类和协议在Objective-c中存在于不同的命名空间。你可以同时拥有相同命名的类和协议，但是他们在编程语言上是不相关联的。
这就是NSObject。
如果你光看语句写法，它们没有什么不同。类命名可以用来发送消息，可以用在@interface的中做声明，还可以看作类型名。而协议也可以用在同样的地方，只是用法不同，所以很容易混淆。
**根类**
NSObject类属于根类。根类在层级结构中处于最高级，也就是说除此以外没有更高层级。而且Objective-c中还有其他根类，不像Java里只有一个java.lang.Object根类，其他所有的类都直接或间接的继承于它。因此，Java代码可以依据任何对象来实现它的基本方法。
Cocoa有多个根类，除了NSObject还有NSProxy等其他等级的根类。这只是部分原因，NSObject协议定义了一套所有的根类都可以实现的基础方法，这样在编码时就容易找到了。
NSObject类遵循NSObject协议，这就是说，NSObject类实现了下面这些基础方法：
- @interface NSObject  
NSProxy同样遵循NSObject协议：
- @interface NSProxy  
NSObject协议包含了hash，isEqual:，description等方法。事实上，NSProxy遵循NSObject协议意味着你可以依靠实现NSProxy来实现NSObject方法。
**顺便说说Proxy**
既然我们说到这里，为什么这里有一个NSProxy作为根类？
很多情况下你持有一个类，但可以不实现你不需要的方法。NSProxy顾名思义，proxy(代理、转发)就是很有用的。NSObject类实现了比NSObject协议更多的东西，比如键值编码，可能你根本不需要用它。
建立proxy对象的目是为了预留大多数未实现的方法，使用forwardInvocation:方法来转发。如果把NSObject作为子类则可能会引入更多包，那样容易导致冲突。而NSProxy通过提供了一个不含多余内容的简洁超类来避免此类发生。
**协议**
实际上，鉴于我们并不经常使用其他的根类，所以NSObject协议虽然很有用，但是在大部分Objective-C的编程中也没有太多的用处。然而，它在我们创建自定义的协议时就派上用场了。
比如，我们可以这样写：
-  @protocol MyProtocol 
- - (void)foo; 
- 
- @end 
同时，你定义一个类指针：
- id obj; 
你可以这样调用foo:
- [obj foo]; 
当然，你不能这样使用description：
- [obj description]; // no such method in the protocol
同样，你也不能这样比较两者是否相等：
- [obj isEqual: obj2]; // no such method in the protocol
总的来说，你不可能要求它来完成一般对象做的任何任务，仅在有需要的时候用，这就是为什么要引进NSObject的协议。协议是可以继承于其他协议的，你可以这样写继承于NSObject的协议：
- @protocol MyProtocol  
- - (void)foo; 
- @end 
这就是说，不仅仅对象遵循MyProtocol协议并响应-foo方法，而且可以响应所有在NSObject协议中类似的消息。正因为你应用中所有的对象都继承于NSObject类并且遵循NSObject协议，这样在实现MyProtocol的实例时不需要更多条件就可以使用NSObject的一般方法。
**总结**
Frameworks框架中有两个不同的NSObject确实比较奇怪，理解透了还是很有用的。一个NSObject协议允许多个根类拥有相同的基础方法，因此我们能很方便的声明一个包含了相同基础功能的协议 。正因为NSObject类遵循了NSObject协议，把所有的东西融合到一起。  
