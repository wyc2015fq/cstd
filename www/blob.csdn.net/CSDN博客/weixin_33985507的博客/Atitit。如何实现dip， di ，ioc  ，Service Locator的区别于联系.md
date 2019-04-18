# Atitit。如何实现dip， di ，ioc  ，Service Locator的区别于联系 - weixin_33985507的博客 - CSDN博客
2016年09月16日 16:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
Atitit。如何实现dip，
di ，ioc  ，Service Locator的区别于联系

[1.
Dip原则又来自于松耦合思想方向1](#_Toc2630%20)
[2.
要实现dip原则，有以下俩个模式1](#_Toc10036%20)
[3.
Ioc和di的区别1](#_Toc10346%20)
[4.
Service Locator模式1](#_Toc694%20)
[5.
Service Locator vs. Dependency Injection2](#_Toc26682%20)
[6.
参考2](#_Toc28648%20)
# 1. **Dip原则又来自于松耦合思想方向**
松耦合系统的好处有两点：
一点是它适应变化的灵活性；
另一点是当某个服务的内部结构和实现逐渐发生改变时，不影响其他服务
# 2. **要实现dip原则，有以下俩个模式**
Ioc模式
*控制反转”（Inversion of Control，IoC）*
依赖注入”（Dependency
 Injection）。模式
服务定位器模式(Service Locator Pattern
IOC,控制反转的意思,DI是依赖注入所谓控制反转,即依赖关系的获取方式(controll)反过来了
作者::  ★(attilax)>>> 绰号:老哇的爪子（全名：：Attilax Akbar Al Rapanui 阿提拉克斯阿克巴阿尔拉帕努伊）汉字名：艾龙，  EMAIL:1466519819@qq.com
转载请注明来源： http://blog.csdn.net/attilax
# 3. **Ioc和di的区别**
IOC控制反转，侧重于原理。
DI依赖注入：说的是创建对象实例时，为这个对象注入属性值或其它对象实例，侧重于实现
# 4. **Service Locator模式**
**背后的基本思想是：有一个对象（即服务定位器）知道如何获得一个应用程序所需的所有服务**
# 5. **Service Locator vs. Dependency** Injection
首先，我们面临Service Locator 和Dependency
 Injection 之间的选
**实际上这两个模式都提供了基本的解耦合能力——无论使用哪个模式，应用程序代码都不依赖于服务接口的具体实现。两者之间最重要的区别在于：这个“具体实现”以什么方式提供给应用程序代码。使用Service Locator 模式时，应用程序代码直接向服务定位器发送一个消息，明确要求服务的实现；使用Dependency Injection 模式时，应用程序代码不发出显式的请求，服务的实现自然会出现在应用程序代码中，这也就是所谓“控制反转”**
控制反转是框架的共同特征，但它也要求你付出一定的代价：它会增加理解的难度，并且给调试带来一定的困难。所以，整体来说，除非必要，否则我会尽量避免使用它。这并不意味着控制反转不好，只是我认为在很多时候使用一个更为直观的方案（例如Service
 Locator 模式）会比较合适。
一个关键的区别在于：使用Service Locator 模式时，服务的使用者必须依赖于服务定位器。定位器可以隐藏使用者对服务具体实现的依赖，但你必须首先看到定位器本身。所以，问题的答案就很明朗了：选择Service
 Locator 还是Dependency Injection，取决于“对定位器的依赖”是否会给你带来麻烦。
# 6. **参考**
依赖注入控制反转服务定位器模式 Dependency Injection Inversion of Control Service Locator Patterns _
超级经典 - Yiraner的日志
- 网易博客.html
