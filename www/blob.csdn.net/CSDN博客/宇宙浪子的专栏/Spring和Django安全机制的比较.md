# Spring和Django安全机制的比较 - 宇宙浪子的专栏 - CSDN博客
2014年09月21日 23:26:35[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：2257
# 介绍
Spring是Java语言开发的一站式Web框架。包括：SpringMVC，Spring,SpringSecurity,SpringAOP等子框架。Spring在数据库访问层可以整合Hibernate,iBatis等第三方框架。构成了一个完整的Web应用程序框架。
Spring大量使用了策略模式、模板方法模式，提供了钩子回调第三方的API，因而可以整合大量第三方框架。
Django是Python语言开发的一站式Web应用程序框架。其独立开发了从Web层到数据库访问层在内的所有框架。
Spring和Django在功能上基本对等，都是Web应用程序开发的基础平台。
Django利用了Python语言自身的优势，优雅地实现了一整套Web应用框架。
Spring为人广为诟病的一点就是，其大量使用了xml格式的配置文件。配置工作量相当大。在Java5引入annotation之后，虽然可以在Java源码中直接加上配置。但是每次修改配置必须重新编译程序，殊为不便！
Python是动态语言，因而Django直接使用Python源代码作为配置文件。Python程序可以即时编译，不需要额外的编译过程。
本文将对Spring和Django的安全机制做一系统比较。
# 传统的Web安全解决方案
## HttpRequest拦截方法
大部分Web应用程序在Action层实现安全控制。Action层负责接收请求和发出响应消息的这一层次。
通常的做法是，在数据库中为用户定义权限。每一个权限使用一个URL标识。
用户登录后发给用户浏览器一个cookie。服务器端也保存这个cookie，并把这个cookie和一个用户关联起来。
用户再次发出请求之后，根据用户发来的cookie到数据库中查询对应的用户，取得User对象和相应的权限集合。保存在HttpRequest或者HttpSession中，或者ThreadLocal中。
编写一个Filter，对Http请求进行预过滤。比对User的权限中是否有这个URL。如果没有，那么就直接返回错误消息，不会把这个request发送到URL对应的Action方法中处理。
这一方案可以在Action层实现安全控制，有效拦截非法访问。
## EJB安全机制
Java的EJB框架也有自己的基于角色的一套安全控制机制。它可以对EJB对象而不是Action层实现对服务的访问控制，粒度更低。
但是使用EJB安全机制很麻烦。必须按照EJB的要求定义角色和安全模型，必须编写大段的xml配置文件指定访问控制策略。
## 小结：
EJB在Business层实现了安全控制，这对于EJB架构的程序是有意义的。因为EJB架构中，EJB是独立部署的服务组件。客户端使用RMI远程协议访问它。
EJB的客户端可以是Web服务器，也可以是富客户端程序。
但是，EJB这样的架构是否必须呢？这个在业界有很长时间的争论。很多人包括笔者本人都认为EJB这种架构已经过时了。
富客户端程序同样可以通过Http协议与Web服务器程序通信。Web服务器可以同时支持B/S和C/S双架构。
Web服务器程序同样也可以提供TCP/UDP接口供富客户端程序访问。
最后一个问题，如EJB这样把Business组件单独部署是否有必要？EJB集群 VS
 Web服务器集群谁优谁劣？
Web服务器同时包括http接口和Business逻辑。Web服务器可以和EJB一样实现集群部署。EJB服务器使用RMI对外接口通讯。Web服务器使用Http对外接口通讯。应该说EJB集群没有提供比Web服务器集群更多的优势。
因此，我认为EJB安全机制并不比HttpRequest拦截安全机制更优秀。
# **Spring****和Django安全机制**
Spring和Django都使用了AOP（面向方面编程）技术来实现安全控制。
SpringAOP是Spring开发的一个AOP框架。利用了Java动态运行的特性，使用反射技术实现了面向方面(AOP)编程。
Spring框架负责安全的子系统是Spring Security框架。Spring
 Security就是使用Spring AOP实现安全控制的。
Python与生俱来就支持AOP。Python的适配器函数就可以轻松实现AOP。
`Python``的装饰者函数在语法上和``Java``的``Annotation``很相似，但实际实现完全不同。`
`     ``Java``的``Annotation``是运行时可以通过发射得到的描述型数据。`
`      ``Annotation``在``Python``中的对应物是``Python``中的``Doc``。``Doc``也是在运行时可以得到的描述型数据，用于生成``JavaDoc``这样的文档，或者是运行时通过``help(``模块名``/``类名``/``函数名``)``得到帮助信息。``Python``的``Doc``一般没有像``Java``的``Annotation``这样使用的。`
`     ``Python``的装饰者函数不是运行时可以得到的元数据。``Python``的装饰者函数就是一个普通的``Python``函数。它的第一个参数是被修饰的函数。因此可以直接实现``AOP``中的``round``。我们知道``AOP``包括``3``种拦截机制：``before,after``和``round``。``Round``是同时``before``和``after``。`
`         ``因此``Python``的装饰者函数直接等价于``Java``的``AOP``。`
## Spring Security安全机制
下面内容摘自Spring3.0指南：
Spring Security可以用在多种不同的验证环境下。 我们推荐人们使用Spring
 Security进行验证，而不是与现存的容器管理验证相结合， 然而这种方式也是被支持的 - 作为与你自己的 验证系统相整合的一种方式。
什么是Spring Security的验证呢？
让我们考虑一种标准的验证场景，每个人都很熟悉的那种。
- 
一个用户想使用一个账号和密码进行登陆。
- 
系统（成功的）验证了密码对于这个用户名 是正确的。
- 
这个用户对应的信息呗获取 （他们的角色列表以及等等）。
- 
为用户建立一个安全环境。
- 
用户会执行一些操作，这些都是潜在被 权限控制机制所保护的，通过对操作的授权， 使用当前的安全环境信息。【函数需要校验用户的权限是否满足自身的要求。因此函数必须知道从哪里获得用户的授权信息】
前三个项目执行了验证过程，所以我们可以看一下 Spring Security的作用。
- 
用户名和密码被获得，并进行比对， 在一个`UsernamePasswordAuthenticationToken`的实例中
 （它是`Authentication`接口的一个实例， 我们在之前已经见过了）。
- 
这个标志被发送给一个`AuthenticationManager`的实例进行校验。
- 
`AuthenticationManager`返回一个完全的 `Authentication`实例，
 在成功校验后。
- 
安全环境被建立，通过调用 `SecurityContextHolder.getContext().setAuthentication(...)`，
 传递到返回的验证对象中。
`SecurityContext``跨请求存储：`
In Spring Security, the responsibility for storing the
SecurityContext between requests falls to the SecurityContextPersistenceFilter,
which by default stores the context as an HttpSession attribute between HTTP requests.
`It restores the context to the SecurityContextHolder for each request and, crucially, clears the SecurityContextHolder when the request completes.`
### `技术说明：`
`       ``SecurityContext``是`
```
Spring
 Security
```
`框架保存用户授权信息的对象。在用户登录时创建。`
每一次请求开始时，Spring Security使用Filter把HttpSession中的SecurityContext恢复到
          SecurityContextHolder中。SecurityContextHolder是一个Java类，包含多个静态函数。
用SecurityContextHolder的方法：public
 static void **setContext**([SecurityContext](http://www.acegisecurity.org/acegi-security/apidocs/org/acegisecurity/context/SecurityContext.html) context)方法把SecurityContext对象保存到ThreadLocal中。
方法原型：
`setContext`
public static void
**setContext**([SecurityContext](http://www.acegisecurity.org/acegi-security/apidocs/org/acegisecurity/context/SecurityContext.html) context)
Associates a new `SecurityContext` with the current thread of execution.
**Parameters:**
`context` - the new `SecurityContext` (may
 not be `null`)
`SecurityContextHolder.getContext()``方法返回保存在``ThreadLocal``中的``SecurityContext``对象。`
`     ``因为``Action``层和``Service``层都在同一个``Thread``下执行。因此``Action``层``AOP``存放下``SecurityContext``可以被``Service``层重用。`
用户不应该直接操作HttpSession中的SecurityContext对象。总是应该用 `SecurityContextHolder``提供的方法获取``SecurityContext``对象。`
`        ``每一次请求结束时，``Filter``都会把当前线程中的``SecurityContext``对象清除。因为线程可能会被重用。不清除可能会引发安全问题。`
`        ``Spring Security``可以使用``xml``配置文件或者``java annotation``在业务层对方法声明安全限制。`
```
Spring
 Security
```
`使用``Spring AOP``技术在业务层方法执行时对其拦截，用``SecurityContextHolder.getContext()``对象内的用户授权和函数上声明的授权进行比对。如果不符合，就抛出异常返回。从而实现了对业务层方法的安全控制。`
## `Django``安全机制`
`     ``Django``使用``App``的概念实现各个子框架。``Django``负责安全的子框架是``Auth``应用。`
`     ``Django``用户登录后，会在``HttpSession``中保存``User``对象。在``Aciton``层（``Django``的术语是``View``，我在本文中为了和``Java``术语相同，使用``Action``代替``View``）可以得到``User``对象及其``Perm``授权集合。程序员可以在``Action``层中手工对用户的权限和``Action``要求的权限进行比对，实现访问控制。`
`     ``这和前文``HttpRequest``拦截方法类似。`
`     ``但是，``Django``的能力不止与此。使用``Python``装饰者函数，``Django``可以使用类似于`
```
Spring
 Security
```
`的``anontation``的语法对``Aciton``函数实现声明式的安全控制！`
`如：`
`from django.contrib.auth.decorators import login_required`
@login_required
def my_view(request):
# ...
`这里的``@login_required``声明表示必须是登录用户才可以调用这个``Action``（``View``）函数。``@login_required``就是``Python``的装饰者函数。`
`更进一步的访问限制：`
`from django.contrib.auth.decorators import permission_required`
@permission_required('polls.can_vote', login_url="/login/")
def vote(request):
# ...
注意, permission_required() 也有一个可选的login_url 参数, 这个参数默认为'/accounts/login/' 。
```这里可以根据用户的权限对``Action``进行限制。如果用户通不过授权，就返回给用户一个登录页面。`
`这些都不需要程序员编写一行代码！神奇吧？！`
## `比较``：`
`    ``Python Django``框架只能在``Action``层进行``ACL``控制。因为它没有使用``Filter``在``ThreadLocal``中保存``User``信息。因此在``Service``层中无法获得``User``信息。`
`    ``Python``自身支持``AOP``，也支持``ThreadLocal``。因此``Django``也可以像`
```
Spring
 Security
```
`一样提供对业务层函数的安全控制。只是``Django``没有这样做而已。`
`    ``也许是``Django``作为一个相对新生的社区，没有精力做这件事。或者更可能的是，``Django``社区的人认为在业务层实现安全控制没有必要。``Action``层直接控制不就完了吗？`
``
`本文转自：[http://blog.csdn.net/shendl/article/details/6049586](http://blog.csdn.net/shendl/article/details/6049586)`
