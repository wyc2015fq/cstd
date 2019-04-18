# C# 8新提案让泛型Attribute成为现实 - weixin_33985507的博客 - CSDN博客
2019年01月28日 10:07:40[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
从一开始， Attribute就是.NET的一部分。因此，它们在引入泛型之前就已经被创建好，这意味着如果要引用属性中的类型，必须暴露一个Type参数或属性。例如：
`[TypeConverter(typeof(X))]`
这种模式存在一些限制。在上面的示例中，X必须有一个无参数的构造函数，并继承TypeConverter。编译器不会强制这些，所以开发人员只能在运行时发现错误。
相反，如果我们使用泛型类型参数，那么就可以在编译器强制执行这些规则，而且语法会稍微清晰一些。
`[TypeConverter\u0026lt;X\u0026gt;]`
理论上，“[Allow Generic Attributes](%5C)”提案是一个非常简单的变更。CLR已经支持这个概念，问题是如何在.NET语言中使用它。但有时候理论和现实是两条平行线，而且即使CLR可以支持，并不代表就一定会支持。Jan Kotas解释说：
> 
我认为这个问题的本质与局部引用和return类似。局部引用和return不受任何API的限制，理论上它们在现有的运行时、工具和库上都能正常运行。但在实际当中，它们可能不能正常运行，因为它们暴露了预先存在的错误和限制，可能是因为它们的处理路径从未被执行过，或者它们明显不在预期之内。与引用return相关的错误和问题刚刚开始出现，因为人们开始使用这项功能，并发现了一些不奏效的地方。我认为这在预期之内。我们应该承认这些事实，并且不会采取之前我们撤回默认值类型构造函数功能那样的行动。
这个功能非常类似。ECMA并没有针对禁止使用泛型Attribute的内容，在理论上应该可以正常运行，但实际上却没有。如果要正常运行，必须更新整个系统中的很多部件：
- 存在多个运行时（Mono、CoreRT、整个框架）；
- IL的一些工具也可能会受到影响（编译器、基于Cecil或基于CCI2的工具）；
- 可能需要用于访问泛型Attribute的API（例如，现有的反射API不允许你访问泛型Attribute——你必须进行枚举）。
该提案的一个限制是在编译时必须“关闭”Attribute功能。下面是提案中提供的一个启用泛型Attribute的例子。
`[SomeAttribute\u0026lt;T\u0026gt;]public void DoSomething\u0026lt;T\u0026gt;(T input) { }`
因为T不是在编译时定义的，因此反射机制就不知道这个表达式会返回什么。
`typeof(GenericClass\u0026lt;\u0026gt;).GetMethod(\u0026quot;DoSomething\u0026quot;).GetCustomAttributes()`
**状态**
目前，泛型Attribute功能列在[C# 8路线图](%5C)中，状态为“进行中”。
**查看英文原文**：[https://www.infoq.com/news/2019/01/Generic-Attributes](%5C)
