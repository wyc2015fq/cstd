# SSH网上商城初步——一些概念的理解 - Big Smile - CSDN博客
2017年02月16日 16:42:19[王啸tr1912](https://me.csdn.net/tr1912)阅读数：536
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        刚刚结束了SSH网上商城的学习，感觉里面有很多之前没有见过的名词，在这里总结一下，也为没有进入SSH的同学做一个铺垫。
# 一、SSH理解
        刚开始SSH的时候，郭嘉老师就一直在说我们已经学习过SSH的内容了，下面开始做项目，配置这个配置文件，配置那个配置文件，拷贝Jar包等等的内容。其中频率最高的三个词莫过Spring，Struts2，Hibernate了，这个里面的东西很多啊，因为是三个框架的组合成的一个框架，所以我们想要理解这个框架，还要从这三个来探索。
        SSH在J2EE项目中表示了3种框架，即 Spring + Struts +Hibernate。 Struts对Model，View和Controller都提供了对应的组件。Spring是一个轻量级的控制反转(IoC)和面向切面(AOP)的容器框架，它由Rod Johnson创建。它是为了解决企业应用开发的复杂性而创建的。Spring使用基本的JavaBean来完成以前只可能由EJB完成的事情。 Hibernate是一个开放源代码的对象关系映射框架，它对JDBC进行了非常轻量级的对象封装，可以应用在任何使用JDBC的场合，可以在Servlet/JSP的Web应用中使用，也可以在应用EJB的J2EE架构中取代CMP，完成数据持久化的重任。以上是百度百科对于SSH的介绍，下面附上SSH中三种框架的作用方式的图片：
![](https://img-blog.csdn.net/20170216162805704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**①、Spring**
          其实spring这个东西我们都听说过，因为我们只要参加过ITOO的同学们都知道我们在搭建框架的时候需要在nuget上去下载一个叫Spring.core的组件，然后需要在配置文件中各种写BLL层的注解，DAL层的注解等一系列的东西，现在回想起来我们在SSH中也做过类似的事情，比如在各个action和service文件中的注入，还有在applicationContext这个配置文件中写的三个层的配置文件就是实现了这个注入的过程，所以说我们的spring在IOC方面是主力，在这个里面其实Spring还有一些我们没有用过的东西：
**1.面向切面**　Spring提供了面向切面编程的丰富支持，允许通过分离应用的业务逻辑与系统级服务（例如审计（auditing）和事务（transaction）管理）进行内聚性的开发。应用对象只实现它们应该做的——完成业务逻辑——仅此而已。它们并不负责（甚至是意识）其它的系统级关注点，例如日志或事务支持。
**2.容器**　Spring包含并管理应用对象的配置和生命周期，在这个意义上它是一种容器，你可以配置你的每个bean如何被创建——基于一个可配置原型（prototype），你的bean可以创建一个单独的实例或者每次需要时都生成一个新的实例——以及它们是如何相互关联的。然而，Spring不应该被混同于传统的重量级的EJB容器，它们经常是庞大与笨重的，难以使用。
**3.框架**　Spring可以将简单的组件配置、组合成为复杂的应用。在Spring中，应用对象被声明式地组合，典型地是在一个XML文件里。Spring也提供了很多基础功能（事务管理、持久化框架集成等等），将应用逻辑的开发留给了你。
**②、Struts2**
        刚接触这个词我们可能会有点陌生，但是里面的一些原理我们也是使用过的，其实它就是一个被封装了并且增加了很多别的功能的MVC的框架，对于MVC我们肯定很是熟悉，因为基本上我们除了三层之外接触过最多的项目就是MVC的了，MVC的一个核心就是要把页面的显示和后台的处理分离，这里Struts2所起到的作用也是如此，我们在配置Action和Jsp页面的交互的时候用的就是Struts的配置文件来进行配置的：
```
<!-- 购物车的Action -->
<action name="cart_*" class="cartAction" method="{1}">
	<result name="addCart">/WEB-INF/jsp/cart.jsp</result>
	<result name="clearCart">/WEB-INF/jsp/cart.jsp</result>
	<result name="removeCart">/WEB-INF/jsp/cart.jsp</result>
	<result name="myCart">/WEB-INF/jsp/cart.jsp</result>
</action>
```
        如上代码中的配置，其中name就是写的JSP想要访问的action的标志，在jsp页面中写的xxx.action的访问方式就是通过这个name来抓取的，然后放到class下面对应的action中去就可以调用了，而action的返回页面则也是由下面的result来定义的。
        其实在Struts2中Struts文件最核心也是最新的作用就是拦截器功能，他的作用就是通过拦截访问地址来跳转到相应的页面，为用户和代码的构建提供了方便的使用方法，在形式上有些类似于.net中MVC的路由功能。
**③、Hibernate**
        这个陌生的框架其实我们也不陌生了，他在SSH中的作用就是对于数据库的访问，和ITOO项目中用的EF映射层的作用是一样的，hibernate对于JDBC的数据访问做了轻量级的面向对象的封装，所以用的数据查询语句也是面向对象的，和sql语句不一样，这个里面用的语句叫做HQL语句，里面用的所有的查询条件和查询内容都是在vo层定义过的对象，所以才能成为是面向对象的一种查询方式，其中的CURD逻辑控制语句和sql中的是一样的。hibernate还有一些接口是我们需要注意的：
       1.Session接口　Session接口负责执行被持久化对象的CRUD操作(CRUD的任务是完成与数据库的交流，包含了很多常见的SQL语句。)。但需要注意的是Session对象是非线程安全的。同时，Hibernate的session不同于JSP应用中的HttpSession。这里当使用session这个术语时，其实指的是Hibernate中的session，而以后会将HttpSesion对象称为用户session。
        2.SessionFactory接口　SessionFactory接口负责初始化Hibernate。它充当数据存储源的代理，并负责创建Session对象。这里用到了工厂模式。需要注意的是SessionFactory并不是轻量级的，因为一般情况下，一个项目通常只需要一个SessionFactory就够，当需要操作多个数据库时，可以为每个数据库指定一个SessionFactory。
       3.Configuration接口　Configuration接口负责配置并启动Hibernate，创建SessionFactory对象。在Hibernate的启动的过程中，Configuration类的实例首先定位映射文档位置、读取配置，然后创建SessionFactory对象。
       4.Transaction接口　Transaction接口负责事务相关的操作。它是可选的，开发人员也可以设计编写自己的底层事务处理代码。
       5.Query和Criteria接口　Query和Criteria接口负责执行各种数据库查询。它可以使用HQL语言或SQL语句两种表达方式。 
# 二、值栈
        我们在看SSH视频的时候肯定听到过值栈这个词，并且老师还对于值栈的数据位置进行了很多的思考才写的代码，那么值栈到底是什么呢？
        值栈是Struts2中的一个新的特性，说白了就是一个请求对象的一个小型轻量级的内存数据中心，在这个里面统一管理Action的提交的一些数据，每次提交数据都在栈顶，如果栈顶数据不是当前要找的数据，就会从栈顶向下去寻找数据。我们在后台代码中存储数据到值栈之后，在前台的JSP页面就可以通过Struts的一个标签来访问到值栈中的内容。
# 三、模型驱动
        其实最开始接触这个词就有点感觉，毕竟这个名字意义还是比较明显的，所谓模型驱动就是用模型作为整个数据流动的推动对象，在ITOO框架中ViewModel其实有类似的功能，我们在每个View页面的上面都可以看到他用@的语句绑定了一个ViewModel在页面上面，然后可以随时调用后台传到ViewModel中的值，所以这个里面的模型驱动中的模型也是同理了。
        模型驱动：把数据跟处理分开，用单独的JavaBean封装数据（不管是请求的，还是返回的，可以在一起封装），把处理类完全孤立成一个框架，用这个模型作为灵魂在驱动整个骨架。
        特性：血肉分开，使结构更清晰，职能划分更清楚，免得局长管权力又管财务的，弄不好贪污。
        核心：模型（model）就是值对象Vo（value object）
        对比：属性驱动--你把一大堆属性搞到你的处理类里面，完全可以。方便而快捷，不过结构清晰度当然就不好了。
        开发：模型驱动开发（Model-driven development，MDD）是软件开发的一种样式，其中主要的软件工件是模型，根据最佳实践，可以从这些模型生成代码和其他工件。模型是从特定角度对系统进行的描述，它省略了相关的细节，因此可以更清楚地看到感兴趣的特性。例如，结构工程师会创建适合于确定建筑物承载特性的模型。
