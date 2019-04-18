# 依赖注入DI的代替，服务定位器模式 - weixin_33985507的博客 - CSDN博客
2019年02月19日 17:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
> 
以下内容是我用来温习的，从以下两个博客总结而来，请关注原作者
ref:
[Service Locator 模式](https://www.cnblogs.com/gaochundong/archive/2013/04/12/service_locator_pattern.html)
[设计模式【服务器定位模式Service Locator Pattern】](https://blog.csdn.net/qiaoquan3/article/details/78630951)
服务定位模式（Service Locator Pattern）是一种软件开发中的设计模式，可对涉及尝试获取一个服务的过程进行封装。该模式使用一个称为 "Service Locator" 的中心注册表来处理请求并返回处理特定任务所需的必要信息。
在首次请求某个服务时，服务定位器会查找服务，没有已经存在的服务，会新建并缓存该服务对象。当再次请求相同的服务时，服务定位器会在它的缓存中查找，这样可以在很大程度上提高应用程序的性能。
## 优点
- 
把类与依赖项解耦，从而使这些依赖项可被替换或者更新。
- 
类在编译时并不知道依赖项的具体实现。
- 
类的隔离性和可测试性非常好。
- 
类无需负责依赖项的创建、定位和管理逻辑。
- 
通过将应用程序分解为松耦合的模块，达成模块间的无依赖开发、测试、版本控制和部署。
## 解决方案
创建一个 Service Locator，其包含各服务的引用，并且封装了定位服务的逻辑。在类中使用 Service Locator 来获取所需服务的实例。
![1632095-1b626cf46e2fb453.png](https://upload-images.jianshu.io/upload_images/1632095-1b626cf46e2fb453.png)
image
Service Locator 模式**并不描述如何实例化服务**，**其描述了一种注册和定位服务的方式**。通常情况下，Service Locator 模式与工厂模式（Factory Pattern）和依赖注入模式（Dependency Injection Pattern）等结合使用。
服务定位器应该能够在不知道抽象类的具体类型的情况下定位到服务。例如，它可能会使用**字符串**或**服务接口类型**来映射服务，这允许在无需修改类的条件下替换依赖项的具体实现。
## **实现细节**
我们将创建 ServiceLocator、InitialContext、Cache、Service 作为表示实体的各种对象。Service1 和 Service2 表示实体服务。
ServiceLocatorPatternDemo，我们的演示类在这里是作为一个客户端，将使用 ServiceLocator 来演示服务定位器设计模式。
![1632095-b83f8915b777b13f.png](https://upload-images.jianshu.io/upload_images/1632095-b83f8915b777b13f.png)
image
## 注意事项
在使用 Service Locator 模式之前，请考虑以下几点：
- 
有很多程序中的元素需要管理。
- 
在使用之前必须编写额外的代码将服务的引用添加到服务定位器。
- 
类将对服务定位器有依赖关系。
- 
源代码变的更加复杂和难以理解。
- 
可以使用配置数据来定义运行时的关系。
- 
必须提供服务的实现。因为服务定位器模式将服务消费者与服务提供者解耦，它可能需要提供额外的逻辑。这种逻辑将保证在服务消费者尝试定位服务之前，服务提供者已被安装和注册。
## **相关模式**
- 
依赖注入（Dependency Injection）。这种模式解决了与 Service Locator 模式相同的问题，但它使用不同的方法。
- 
控制反转（Inversion of Control）。Service Locator 模式是这种模式的特殊版本。它将应用程序的传统控制流程反转。它用被调用对象来代替控制过程的调用方。
