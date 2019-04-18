# 在Spring MVC 中使用 flash attributes - z69183787的专栏 - CSDN博客
2014年12月03日 09:53:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1897
## http://www.yulezhandian.com/?p=150
## [Spring MVC 学习（六） Using flash attributes](http://www.yulezhandian.com/?p=150)
By [yulezhandian](http://www.yulezhandian.com/?author=1)[2013 年 1 月 14 日](http://www.yulezhandian.com/?p=150)[Spring
 MVC](http://www.yulezhandian.com/?cat=6)[No Comments](http://www.yulezhandian.com/?p=150#respond)
# 17.6 在Spring MVC 中使用 flash attributes
Spring MVC Flash attributes 提供了一个请求存储属性可供另外请求使用一种方式。 在使用重定向时候非常必要 — 例如， *Post/Redirect/Get* 模式。 Flash attributes 在重定向之前暂存 (就像存在 session中) 以便重定向之后还能使用，并立即删除。
Spring MVC 有两个主要的抽象来支持 flash attributes。 `FlashMap` 用于保持 flash attributes 而 `FlashMapManager`用于存储，检索，管理`FlashMap` 实例。
Flash attribute 支持默认开启（ “on”）并不需要显式启用，它永远不会导致HTTP Session的创建。 每一个请求都有一个 “input”`FlashMap` 具有从上一个请求（如果有的话）传过来的属性和一个 “output” `FlashMap` 具有将要在后续请求中保存的属性。 这两个 `FlashMap` 实例都可以通过静态方法RequestContextUtils从Spring MVC的任何位置访问。
使用注解的控制器通常不需要直接与 `FlashMap` 一起工作。 相反， `@RequestMapping` 方法可以接受一个 `RedirectAttributes` 类型的参数，用它来在重定向场景中增加 flash attributes 。通过`RedirectAttributes` 加入的 Flash attributes 将自动传播到 “output” FlashMap。同样的，重定向之后，从 “input” `FlashMap` 来的属性自动添加到控制器的 `Model` 
 ，为目标URL服务。
**Matching requests to flash attributes**
闪存属性（flash attributes）的概念存在于许多其他的 Web 框架，事实证明，有时受到并发问题。  这是因为根据定义 flash 属性是要被存储直到下一个请求。但是恰好 "next" 请求并不是接受者，而是另外的异步请求 (例如 轮询或资源请求) 这种情况下，flash attributes 被过早移除。
为了减少这种问题的可能性， `RedirectView` 自动以重定向URL的目标path和query parameters "stamps" `FlashMap` 实例。  反过来，当查找"input" `FlashMap`时， 默认的`FlashMapManager` 匹配该信息到传入的请求。
这并不能完全排除一个并发问题的可能性但尽管如此已在重定向 URL 中可用信息中大大降低。  因此使用闪存属性主要用于重定向场景。
