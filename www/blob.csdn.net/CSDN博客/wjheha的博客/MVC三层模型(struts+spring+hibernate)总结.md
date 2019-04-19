# MVC三层模型(struts+spring+hibernate)总结 - wjheha的博客 - CSDN博客
2017年06月17日 23:14:36[wjheha](https://me.csdn.net/wjheha)阅读数：107标签：[jsp																[hibernate																[struts](https://so.csdn.net/so/search/s.do?q=struts&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
原文地址：http://blog.csdn.net/tender001/article/details/7706083
**简单的说：**
**struts 控制用的**
**[hibernate](http://lib.csdn.net/base/javaee) 操作[数据库](http://lib.csdn.net/base/mysql)的**
**[spring](http://lib.csdn.net/base/javaee) 用解耦的**
**详细的说：**
**STRUTS 在 SSH 框架中起控制的作用 , 其核心是 Controller, 即 ActionServlet, 而 ActionServlet 的核心就是 Struts-confi g.xml. 主要控制逻辑关系的处理 .**
**hibernate 是数据持久化层 , 是一种新的对象、关系的映射工具 , 提供了从 [Java](http://lib.csdn.net/base/java) 类到数据表的映射，也提供了数据查询和恢复等机制 , 大大减少数据访问的复杂度。把对数据库的直接操作 , 转换为对持久对象的操作 .**
**SPRING 是一个轻量级的控制反转 (IoC) 和面向切面 (AOP) 的容器框架 , 面向接口的编程 , 由容器控制程序之间的（依赖）关系，而非传统实现中，由程序代码直接操控。这也就是所谓 “ 控制反转 ” 的概念所在：（依赖）控制权由应用代码中转到了外部容器，控制权的转移，是所谓反转。依赖注入，即组件之间的依赖关系由容器在运行期决定，形象的来说，即由容器动态的将某种依赖关系注入到组件之中**
**起到的主要作用是解耦**
**Struts 、 spring 、 Hibernate 在各层的作用**
**1 ） struts 负责 web 层 .**
**ActionFormBean 接收网页中表单提交的数据，然后通过 Action 进行处理，再 Forward 到对应的网页。**
**在 struts-config.xml 中定义 <action-mapping>, ActionServlet 会加载。**
**2 ） spring 负责业务层管理，即 Service （或 Manager).**
**1 ． service 为 action 提供统计的调用接口，封装持久层的 DAO.**
**2 ．可以写一些自己的业务方法。**
**3 ．统一的 javabean 管理方法**
**4 ．声明式事务管理**
**5. 集成 Hiberante**
**3 ） Hiberante ，负责持久化层，完成数据库的 crud 操作**
**hibernate 为持久层，提供 OR/Mapping 。**
**它有一组 .hbm.xml 文件和 POJO, 是跟数据库中的表相对应的。然后定义 DAO ，这些是跟数据库打交道的类，它们会使用 PO 。**
**在 struts+spring+hibernate 的系统中，**
**对象的调用流程是： jsp-> Action － > Service ->DAO ->Hibernate 。**
**数据的流向是 ActionFormBean 接受用户的数据， Action 将数据从 ActionFromBean 中取出，封装成 VO 或 PO,**
**再调用业务层的 Bean 类，完成各种业务处理后再 forward 。而业务层 Bean 收到这个 PO 对象之后，会调用 DAO 接口方法，进行持久化操作。**
**spring:Aop管理事务控制,IoC管理各个组件的耦合,DaoTemplate作为常规持久层的快速开发模板!**
**struts:控制层Action,页面标签和Model数据,调用业务层**
**Hibernate:负责数据库和对象的映射,负责DAO层(Data Access Object:数据访问)**
**spring整合hibernate和struts，只要在配好了applicationContext.xml,在struts的action中直接调用就可以了。hibernate访问数据库的操作都在spring中实现了，spring的调用又在stuts的action中实现了。这个ssh框架就连到了一起……**
**1 SSH在开发中的位置**
**现在J2EE的开源框架多的数不清楚，目前（已经、正在）比较流行的常用框架大概有struts,spring,hibernate,jsf,webwork，而 struts+spring+hibernate(SSH)这种轻量级[架构](http://lib.csdn.net/base/architecture)被誉为“黄金组合”。spring和hibernate更是被许多人认为是未来五年内不会被淘汰的技术，犹如当年的struts，今天的开发中依然被广泛采用。**
**2 为什么使用SSH　　**
**其实，就算用Java建造一个不是很烦琐的web应用，也不是件轻松的事情。 在构架的一开始就有很多事情要考虑。从高处看，摆在开发者面前有很多问题：要考虑是怎样建立用户接口？在哪里处理业务逻辑？ 怎样持久化的数据。 而这三层构架中，每一层都有他们要仔细考虑的。 各个层该使用什么技术？怎样的设计能松散耦合还能灵活改变？ 怎样替换某个层而不影响整体构架？应用程序如何做各种级别的业务处理（比如事务处理）？**
**    构架一个Web应用需要弄明白好多问题。 幸运的是，已经有不少开发者已经遇到过这类问题，并且建立了处理这类问题的框架。 一个好框架具备以下几点：减轻开发者处理复杂的问题的负担（"不重复发明轮子"）；内部有良好的扩展； 并且有一个支持它的强大的用户团体。 好的构架一般有针对性的处理某一类问题，并且能将它做好（Do One Thing well）。 然而，你的程序中有几个层可能需要使用特定的框架，已经完成的UI(用户接口) 并不代表你也可以把你的业务逻辑和持久逻辑偶合到你的UI部分。 举个例子，你不该在一个Controller(控制器)里面写JDBC代码作为你的业务逻辑， 这不是控制器应该提供的。 一个UI 控制器应该委派给其它给在UI范围之外的轻量级组件。 好的框架应该能指导代码如何分布。 更重要的是，框架能把开发者从编码中解放出来，使他们能专心于应用程序的逻辑（这对客户来说很重要）。 **
**他们里面有很我优秀的设计理念及模式应用。比如， struts属于MVC框架，关键是要了解MVC的概念及大致原理，掌握就很容易了；而hibernate属于orm系统，属于持久层的解决方案，同样需要对ORM的概念及原理有一个总体的了解，必要时可以去查查EJB1及EJB2里面用于持久层的Entity Bean的使用。而spring属于应用程序框架，其核心是IOC容器以及AOP，把这两个核心概念(也可称为大模式)了解以后，再加上一定的内力修为，其它就都不难了。Spring中还集成了很多适用东西(不过这些东西80%的在某一个项目中可能一直用不上)，比如对JDBC的封装、自己的MVC、对动态语言的简洁访问等，这些你根据自己的项目情况来选择学习，用到的时候再看看他的文档，一个项目下来应该就能把握。**
**3 对于SSH的理解**
**在SSH框架中，struts用来解决MVC中显示、请求控制部分，spring主要负责访问数据库DAO类的事务控制以及它被人称誉的IOC思想在业务类中的恰当运用，hibernate主要是充当数据访问层组件。由于spring对hibernate的良好支持，在DAO类主要由spring来完成，hibernate更多关注的应是O/R影射文件上的配置，如级联关系，延迟加载等如何设置才能使效率更高。**
**4 收获和问题**
**4.1 actionform,PO,VO三对象的运用**
**讨论最多的是actionform,PO,VO三对象的运用，本人倾向的观点是：在SSH框架中，PO和VO可以不必区分，即业务层和持久层都可以使用hibernate产生的PO对象，我暂时把对象分成actionform和po两种来分析，action 应该是actionform和po的分界点，po不能穿透业务层，突破action到达页面显示层，同样actionform也不能突破action传到后台业务、持久层。（原因：po是持久对象，到达页面后就脱离了session成为无状态（暂理解为脱管态）的对象，而hibernate的持久对象是有状态（包含数据库主键）的，无状态的对象传到后台在调用hibernate的保存方法时会出错，一定要把无状态的对象先转化成持久态对象才能保存）在action中应该对两对象进行转化，转化的方法目前我还没发现有什么非常好的方法（欢迎高手不惜赐教），最普通的就是用get（），set（）方法，也可以使用struts提供的属性复制方法BeanUtils类，但这个好象只支持单个类的转化，对于集合对象不行，需要我们自己扩展。**
**4.2 spring事务管理**
**在配置spring的事务管理中，最好把事务控制配置在业务类上，而不要配置在DAO类（需要保证多个原子事务操作同时失败回滚时这是一种解决办法）；**
**4.3 action如何获取业务类**
**action中如何获取业务类：写一个父类action，在父类中通过spring的webapplicationcontent获得业务类的实例。struts中的具体action继承该父类，通过调用父类的getService（）直接获得业务类的实例。**
**4.4 理解AOP思想**
**深入理解AOP思想，我暂时感觉到的就是尽量面向接口编程，不管是域对象还是业务类或者是DAO类都设计出接口，在各方法中我们尽量传入对象的接口，这对我们重用这些方法，扩展是很有好处的。**
**4.5 分页处理 level**
[](http://blog.csdn.net/tender001/article/details/7706083#)[](http://blog.csdn.net/tender001/article/details/7706083#)[](http://blog.csdn.net/tender001/article/details/7706083#)[](http://blog.csdn.net/tender001/article/details/7706083#)[](http://blog.csdn.net/tender001/article/details/7706083#)[](http://blog.csdn.net/tender001/article/details/7706083#)

