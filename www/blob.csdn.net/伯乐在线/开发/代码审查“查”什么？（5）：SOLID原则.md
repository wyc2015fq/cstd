# 代码审查“查”什么？（5）：SOLID原则 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [cucr](http://www.jobbole.com/members/cucr) 翻译，[唐尤华](http://www.jobbole.com/members/tangyouhua) 校稿。未经许可，禁止转载！
英文出处：[Trisha Gee](http://blog.jetbrains.com/upsource/2015/08/31/what-to-look-for-in-a-code-review-solid-principles-2/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这是代码审查“查”什么系列第5篇。 [查看该系列以前的文章](http://blog.jobbole.com/?s=%E4%BB%A3%E7%A0%81%E5%AE%A1%E6%9F%A5%E2%80%9C%E6%9F%A5%E2%80%9D%E4%BB%80%E4%B9%88)。
在今天的文章中，我们将更多地关注代码本身的设计，专门检查代码是否遵循了良好的面向对象设计实践。与所有我们讨论的其他方面一样，并不是所有的团队将它作为最高准则来优先检查，但是如果你尝试遵循SOLID原则，或尝试将你的代码往这个方向上努力，这里的一些建议可能会对你有所帮助。
**什么是SOLID？**
[SOLID原则](https://en.wikipedia.org/wiki/SOLID_%28object-oriented_design%29) 是面向对象程序设计和编程的五个核心原则。这篇文章的目的并不是教会你这些原则是什么或者深入探讨为什么要遵循它们，而是指出那些代码评审中发现的代码异味，它们可能是没有遵循这些原则的结果。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1ereolg8hwxj20xc0dw0uq.jpg)
SOLID代表：
- **S** – [单一职责原则](http://www.objectmentor.com/resources/articles/srp.pdf)
- **O** – [开放封闭原则](http://www.objectmentor.com/resources/articles/ocp.pdf)
- **L** – [里氏替换原则](http://www.objectmentor.com/resources/articles/lsp.pdf)
- **I** – [接口分离原则](http://www.objectmentor.com/resources/articles/isp.pdf)
- **D** – [依赖倒置原则](http://www.objectmentor.com/resources/articles/dip.pdf)
**单一职责原则（SRP）**
不应该有多种情况需要修改某个类的对象。
仅仅通过代码评审有时会很难发现上述情况。根据定义，作者是（或应该是）由一个独立的要求去改变代码库——修复一个bug，添加一个新功能，或一次专门的重构。
你应该看看在一个类中哪些方法在同一时间会改变，以及哪些方法几乎是不可能受其他方法变化影响而被改变。例如：![](http://ww1.sinaimg.cn/mw690/6941baebjw1ewy5eh4oi9j21220gn7dz.jpg)
上面Upsource工具排展示的diff中，一个新功能被添加到TweetMonitor，能够基于user接口的某种排序画出Tweeter的十大排行榜。虽然这似乎是合理的， 因为它使用了onMessage方法收集的数据，但有迹象显示这违反了SRP。onMessage和 getTweetMessageFromFullTweet方法都是和接收或解析一个Twitter消息相关，而draw用来组织数据并在UI上显示。
代码审查者应该标记这两个职责，然后和作者研究出一个分离这些特性的更好方法：也许通过移动Twitter字符串解析方法到不同的类，或者创建一个不同的类负责来渲染排行榜。
**开放封闭原则（OCP）**
> 
软件实体应该对扩展开放，对修改封闭。
作为一个代码审查者，如果你看到一系列的if语句检查对象的特殊类型，可能这就是违反该原则的迹象：
![](http://ww4.sinaimg.cn/mw690/6941baebjw1ewy5egtnulj20ea06umyp.jpg)
如果你审查上面的代码应该很清楚，当一个新的Event类型添加到系统，新创建的Event类型可能要添加另一个else到这个方法中来处理它。
最好使用多态来移除这个if：
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ewy5gz36prj20y20fmgsf.jpg)
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ewy5ecykr2j20yf0ebafk.jpg)
像往常一样，这个问题的解决方案不止一个，但关键是把复杂的if/else和instanceof判断移除。
**里氏替换原则（LSP）**
函数使用基类的引用，必须能够在不知不觉的情况下使用派生类的对象。
发现违反这一原则的一个简单的方法是寻找显示的类型转换。如果你不得不把一个对象转换为某种类型，你就没有做到不知不觉地使用继承了基类的子类对象。
检查以下两条时可以找到更细微的冲突：
- [先决条件不能在子类型加强。](https://en.wikipedia.org/wiki/Liskov_substitution_principle)
- [后置条件不能在子类型减弱。](https://en.wikipedia.org/wiki/Liskov_substitution_principle)
例如，想象我们有一个抽象类Order和一些子类——BookOrder、ElectronicsOrder等等。placeOrder方法可能需要一个Warehouse，可以用这个方法来改变仓库中的实物产品的库存水平：
![](http://ww3.sinaimg.cn/mw690/6941baebjw1ewy5e9am66j20c704374y.jpg)
现在想象一下，我们引入电子礼品卡，只需简单地为钱包添加余额，但不需要物理盘点。如果实现为GiftCardOrder，placeOrder方法就不必使用warehouse参数：
![](http://ww2.sinaimg.cn/mw690/6941baebjw1ewy5e8zmexj20h006pwfu.jpg)
这可能在逻辑上看起来像继承，但事实上你可以说使用GiftCardOrder应该期望这个类和其他类有类似的行为，例如，你应该期望这个参数传递给所有子类：
![](http://ww2.sinaimg.cn/mw690/6941baebjw1ewy5e8sp6vj20ht06mtab.jpg)
但这不会传递，作为GiftCardOrders有不同类型的order行为。如果你审查这样的代码，应该质疑这里继承的使用，也许order行为可以[通过使用组合替代继承](https://en.wikipedia.org/wiki/Composition_over_inheritance)的方式插入。
**接口分离原则（ISP）**
多个客户端特定的接口比使用一个通用的接口要好。
可以很容易识别某些代码违反这一原则，因为接口上具有很多方法。这一原则与SRP呼应，正如你可能看到一个接口具有许多方法，实际上它是负责了多个功能。
但有时甚至一个接口只有两个方法也可以拆分成两个接口：
![](http://ww4.sinaimg.cn/mw690/6941baebjw1ewy5e8hxb0j20ky05yabf.jpg)
在这个例子中，有些时候可能不需要decode方法，并且一个编解码器根据使用要求可能被视为一个编码器或解码器，把SimpleCodec接口分割成一个编码器和解码器可能会更好。一些类可以选择两个都实现，但不必去重载不需要的方法，或者某些类只需要编码器，同时他们的编码器实例也实现解码。
**依赖倒置原则（DIP）**
依赖抽象。不要依赖于具体实现。
虽然想要在试图寻找违反该原则的简单例子时，类似到处使用new关键字（而不是使用[依赖注入](http://www.martinfowler.com/articles/injection.html)或[工厂模式](https://en.wikipedia.org/wiki/Abstract_factory_pattern)）和滥用集合类型（例如声明ArrayList变量或参数而不是List），作为一个代码审查者你在代码审查时，应该确保代码作者已经使用或创建了正确的抽象。
例如，服务级的代码直接连接到一个数据库读写数据：
![](http://ww2.sinaimg.cn/mw690/6941baebjw1ewy5gzc92oj20mf0ek0wp.jpg)
这段代码依赖于大量的具体的实现细节：连接到一个（关系）数据库的JDBC**，**基于具体数据库的SQL，需要知道数据库结构，等等。这段代码来自你的系统的某处，但是不应该出现在这里**。**这里还有其他方法不需要知道具体的数据库。更好的方式是，提取出一个[DAO](https://en.wikipedia.org/wiki/Data_access_object)或使用[Repository模式](http://martinfowler.com/eaaCatalog/repository.html)，将DAO或repository注入到这个服务中。
**总结**
一些代码异味可能意味着可能已经违反了一个或多个SOLID原则：
- 很长的if/else语句
- 强制转换成一个子类型
- 很多公共方法
- 实现的方法抛出UnsupportedOperationException异常
与所有的设计问题一样，在遵循这些原则和故意回避这些原则间需要找到一个平衡，这个平衡取决于你的团队的喜好。但是如果你在代码审查中看到复杂的代码，会发现应用这些原则将提供一个更简单、更容易理解的解决方案。
