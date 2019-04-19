# 基于OSGi和Spring开发Web应用 - 零度的博客专栏 - CSDN博客
2015年08月11日 11:28:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：10013标签：[OSGI																[JavaWeb																[Spring](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)](https://so.csdn.net/so/search/s.do?q=JavaWeb&t=blog)](https://so.csdn.net/so/search/s.do?q=OSGI&t=blog)
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
        作为一个新的事实上的工业标准，OSGi 已经受到了广泛的关注， 其面向服务（接口）的基本思想和动态模块部署的能力， 是企业级应用长期以来一直追求的目标。Spring 是一个著名的轻量级J2EE 开发框架，其特点是面向接口编程和非侵入式的依赖注入。将 OSGi 和 Spring 结合能充分发挥二者各自的特长，更好地满足企业级应用开发的需求。Spring 开发组织在 2008 年发布了将 OSGi 和 Spring 结合的第一个版本：Spring-DM。本文通过一个简单实例，介绍如何利用 Spring-DM
 开发基于 OSGi 和 Spring 架构的 Web 应用，同时探讨其中用到的关键技术及其基本思想。
## 开发一个简单的OSGi Web 应用实例——一个简单的 Web 应用
我们写一个简单的 Web 应用 compute.html ：计算两个数字的和或乘积。如下图所示：
##### 图 1. 一个简单例子
![一个简单例子](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image001.jpg)
为了体现 OSGi bundle 的动态部署能力，我们写两个 service bundle，其中一个计算两个数字的和（称为 add bundle），另外一个计算两个数字的积（称为 multiply bundle）。 当我们点击“Compute”按钮的时候，如果此时 add bundle 被部署，则页面将返回两个数字的和，否则如果此时 multiply bundle 被部署，则页面将返回两个数字的积。
### 开发环境准备
- 下载 Eclipse 3.4
- 获取所有 OSGi, Equinox 和 Spring 的插件, 如下图所示: 
##### 图 2. 相关插件列表
![相关插件列表](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image002.jpg)
- 打开 Eclipse, 设置 target platform 为上述插件集合
### 基本模块设计
该应用主要包含两个层次: 服务层和 Web 层。Web 层基于 Spring-MVC 实现，包含处理 Web访问相关的 bundle（本例中只有一个）。服务层包含处理数字计算的 bundle，本例中包含一个声明服务接口的 compute interface bundle 和两个实现该服务接口的 bundle ：add bundle 和 multiply bundle。基本模块结构如下图所示：
##### 图 3. 基本框架
![基本框架](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image003.gif)
### 模块程序实现
**Step 1 ：实现 Service Layer**
服务层的三个 OSGi bundle 实现完毕之后如下图所示 ：
##### 图 4. 服务层
![服务层](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image004.jpg)
**服务层.bmp**
其中 com.zxn.example.service.compute 是声明服务接口的 bundle。com.zxn.example.service.compute.add和com.zxn.example.service.compute.multiply 是实现了服务接口的两个 bundle。
- com.zxn.example.service.compute
声明一个 Compute 接口，其中包含一个接口方法 computeNums（），如下图所示 ：
##### 图 5. 服务层接口 bundle
![服务层接口 bundle](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image005.jpg)
- com.zxn.example.service.compute.add
bundle com.zxn.example.service.compute.add 的基本程序结构如下图所示：
##### 图 6. 接口实现 bundle ：add
![接口实现 bundle ：add](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image006.jpg)
在该 add bundle 中，添加一个 Add 类，实现 Compute 接口，如下图所示：
##### 图 7. 接口实现代码 ：Add 类
![接口实现代码 ：Add 类](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image007.jpg)
注意到我们在 META-INF 下建了一个 spring 目录，并且添加了一个 computeAdd-context.xml 文件。系统启动时，Spring 将利用该 xml 文件创建一个 bean 实例，并把该 bean 输出为一个 OSGi service，如下图所示 ：
##### 图 8. Spring 声明文件 ：computeAdd-context.xml
![Spring 声明文件 ：computeAdd-context.xml](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image008.jpg)
该xml文件中，**osgi : service**是 Spring-DM 输出 OSGi service 的标记，其中的 interface属性标明了该 service 实现的服务接口。
- com.zxn.example.service.compute.multiply
按照与 add bundle 同样的方法，实现 multiply bundle，如下图所示：
##### 图 9. 接口实现代码：Multiply 类
![接口实现代码：Multiply 类](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image009.jpg)
**接口实现代码-Multiply类.bmp**
类似的，添加一个 computeMultiply-context.xml 输出 OSGi service，如下图所示 ：
##### 图 10. Spring 声明文件：computeMultiply-context.xml
![Spring 声明文件：computeMultiply-context.xml](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image010.jpg)
**Step 2 ：实现 Web Layer**
Web 层只包含一个 bundle：com.zxn.example.web，采用 Spring-MVC 和 OSGi 构建，基本程序结构如下图所示：
##### 图 11. Web Layer 程序结构
![Web Layer 程序结构](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image011.jpg)
- ComputeControler.java
该JAVA类实现了 `org.springframework.web.servlet.mvc.Controller``，`是本 web应用中核心的 servlet，负责接受并处理 web 请求。该类调用 ComputeServiceRef 的方法实现业务逻辑。该类关键的方法是 handleRequest(…), 如下图所示 ：
##### 图 12. 核心 servlet 类
![核心 servlet 类](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image012.jpg)
- ComputeServiceRef.java
该 JAVA 类负责引用部署的 service bundle 完成最终计算，其中的 computeService 由 Spring 根据 OSGi 中实际部署的 service 进行注入。本例中，实际部署的 service 可能是 add bundle 或者 multiply bundle。
需要特别注意的是，此处体现了 Spring-DM 的动态特性。OSGi 的动态部署能力使得 Spring 的动态服务注入成为可能。
##### 图 13. 服务消费类
![服务消费类](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image013.jpg)
- HTTPContextResgistry.java
该 JAVA 类负责在 OSGi 环境中配置和注册 HTTP 服务，其关键方法为 bean 初始化时调用的 init( ) 方法。
##### 图 14. 在 OSGi 环境中注册 HTTP 服务
![在 OSGi 环境中注册 HTTP 服务](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image014.jpg)
该 init 方法中，第六行的 getHTTPService(…) 调用 OSGi 的 ServiceTracker 来获取 OSGi环境中注册的 HTTP 服务的引用，如下图所示：
##### 图 15. 使用 ServiceTracker 获取 HTTP 服务
![使用 ServiceTracker 获取 HTTP 服务](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image015.jpg)
- computeWeb-context.xml
该 xml 文件主要用于配置 HTTPContextResgistry bean 类，以及导入对 Compute 服务接口的引用。标记 **osgi : reference**用于声明要导入的服务接口，其 interface 属性标明了该接口的定义，本例中为 com.zxn.example.service.compute.Compute 接口。
##### 图 16. Spring 声明文件：导入服务接口
![Spring 声明文件：导入服务接口](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image016.jpg)
- computeWeb-Dispatcher.xml
该 xml 文件用于配置 ComputeControler bean类。
##### 图 17. Spring 声明文件：配置核心 servlet 类
![Spring 声明文件：配置核心 servlet 类](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image017.jpg)
[回页首](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/#ibm-pcon)
## 运行程序
以往开发 J2EE 应用通常需要将应用服务器的 runtime 集成到开发环境中才能进行程序调试，非常麻烦。基于 OSGi 的应用完全可以脱离应用服务器运行，这使得程序开发和调试变得非常容易，直接在 Eclipse 中调试运行就可以。我们在 Eclipse 中将程序运行起来，如下图所示：
##### 图18. 运行 OSGi 程序
![图 18. 运行 OSGi 程序](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image018.jpg)
从上图中看到，我们同时选择部署了 add bundle 和 multiply bundle，利用 OSGi console 察看如下：
##### 图 19. 察看部署的 OSGi bundle
![察看部署的 OSGi bundle](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image019.jpg)
当 OSGi 环境中同时部署有多个服务接口的实现 bundle 时，OSGi 会选择一个默认的 bundle提供服务。本例中，Spring 会默认注入 add bundle。我们通过 web 访问 compute.html 页面：
##### 图 20. 访问页面
![访问页面](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image020.jpg)
点 Compute 按钮之后，结果页面如下：
##### 图 21. 访问结果
![图 21. 访问结果](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image021.jpg)
可以看出，是 add bundle 提供了计算服务。下面我们通过命令 ＜stop 76＞ 来停止 add bundle的服务：
##### 图 22. 停止 add bundle
![停止 add bundle](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image022.jpg)
##### 图 23. add bundle 状态变为 RESOLVED
![add bundle 状态变为 RESOLVED](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image023.jpg)
重新访问 compute.html 页面，结果得到的是两个数字的乘积。可以看出，是 multiply bundl 
提供了计算服务。如下图所示：
##### 图 24. 再次访问页面
![图 24. 再次访问页面](http://www.ibm.com/developerworks/cn/opensource/os-cn-osgi-spring/images/image024.jpg)
**小结**
作为当前颇具生命力的两个标准和框架，OSGi 和 Spring 已经初步融合在一起。二者的结合，为开发企业级的 Web 应用同时提供了巨大的灵活性和动态部署能力。本文通过一个简单的例子，描述如何开发一个基于 OSGi 和 Spring 的 Web 应用，并说明了开发过程中涉及到的技术关键点。
 对应的源代码下载地址：[http://download.csdn.net/detail/zmx729618/8989709](http://download.csdn.net/detail/zmx729618/8989709)
