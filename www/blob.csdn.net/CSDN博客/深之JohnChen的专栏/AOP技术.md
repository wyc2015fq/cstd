# AOP技术 - 深之JohnChen的专栏 - CSDN博客

2009年06月20日 01:32:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1075标签：[aop																[设计模式																[扩展																[jboss																[编程																[logging](https://so.csdn.net/so/search/s.do?q=logging&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=jboss&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=aop&t=blog)
个人分类：[JAVA](https://blog.csdn.net/byxdaz/article/category/628883)


AOP技术

1、引言

软件设计因为引入面向对象思想而逐渐变得丰富起来。“一切皆为对象”的精义，使得程序世界所要处理的逻辑简化，开发者可以用一组对象以及这些对象之间的关系将软件系统形象地表示出来。而从对象的定义，进而到模块，到组件的定义，利用面向对象思想的封装、继承、多态的思想，使得软件系统开发可以向搭建房屋那样，循序渐进，从砖石到楼层，进而到整幢大厦的建成。应用面向对象思想，在设计规模更大、逻辑更复杂的系统时，开发周期反而能变的更短。自然其中，需要应用到软件工程的开发定义、流程的过程控制，乃至于质量的缺陷管理。但从技术的细节来看，面向对象设计技术居功至伟。然而，面向对象设计的唯一问题是，它本质是静态的，封闭的，任何需求的细微变化都可能对开发进度造成重大影响。

可能解决该问题的方法是设计模式。GOF将面向对象软件的设计经验作为设计模式纪录下来，它使人们可以更加简单方便地复用成功的设计和体系结构，帮助开发人员做出有利于系统复用的选择。设计模式解决特定的设计问题，使面向对象设计更灵活、优雅，最终复用性更好。然而，设计模式虽然给了我们设计的典范与准则，通过最大程度的利用面向对象的特性，诸如利用继承、多态，对责任进行分离、对依赖进行倒置，面向抽象，面向接口，最终设计出灵活、可扩展、可重用的类库、组件，乃至于整个系统的架构。在设计的过程中，通过各种模式体现了对象的行为，暴露的接口，对象间关系，以及对象分别在不同层次中表现出来的形态。然而鉴于对象封装的特殊性，“设计模式”的触角始终在接口与抽象中大做文章，而对于对象内部则无能为力。

Aspect-Oriented Programming（面向方面编程，AOP）正好可以解决这一问题。它允许开发者动态地修改静态的OO模型，构造出一个能够不断增长以满足新增需求的系统，就象现实世界中的对象会在其生命周期中不断改变自身，应用程序也可以在发展中拥有新的功能。AOP利用一种称为“横切”的技术，剖解开封装的对象内部，并将那些影响了多个类的行为封装到一个可重用模块，并将其名为“Aspect”，即方面。所谓“方面”，简单地说，就是将那些与业务无关，却为业务模块所共同调用的逻辑或责任，例如事务处理、日志管理、权限控制等，封装起来，便于减少系统的重复代码，降低模块间的耦合度，并有利于未来的可操作性和可维护性。

面向方面编程（AOP）是施乐公司帕洛阿尔托研究中心（Xerox PARC）在上世纪90年代发明的一种编程范式。但真正的发展却兴起于近几年对软件设计方兴未艾的研究。由于软件系统越来越复杂，大型的企业级应用越来越需要人们将核心业务与公共业务分离。AOP技术正是通过编写横切关注点的代码，即“方面”，分离出通用的服务以形成统一的功能架构。它能够将应用程序中的商业逻辑同对其提供支持的通用服务进行分离，使得开发人员从重复解决通用服务的劳动中解脱出来，而仅专注于企业的核心商业逻辑。因此，AOP技术也就受到越来越多的关注，而应用于各种平台下的AOP技术也应运而生。但由于AOP技术相对于成熟的OOP技术而言，在性能、稳定性、适用性等方面还有待完善，同时AOP技术也没有形成一个统一的标准，这使得AOP技术的研究更具有前沿性的探索价值。

2、AOP技术起源
2.1 AOP技术起源

AOP技术的诞生并不算晚，早在1990年开始，来自Xerox Palo Alto Research Lab（即PARC）的研究人员就对面向对象思想的局限性进行了分析。他们研究出了一种新的编程思想，借助这一思想或许可以通过减少代码重复模块从而帮助开发人员提高工作效率。随着研究的逐渐深入，AOP也逐渐发展成一套完整的程序设计思想，各种应用AOP的技术也应运而生。

AOP技术在Java平台下是最先得到应用的。就在PARC对于面向方面编程进行研究的同时，美国Northeastern University的博士生Cristina Lopes和其同事也开始了类似的思考。最终，美国国防先进技术研究计划署（Defense Advanced Research Projects Agency即DARPA）注意到了这项工作，并提供了科研经费，鼓励将二者的工作成果结合起来。他们通过定义一套Java语言的扩展系统，使开发者可以方便的进行面向方面的开发，这套扩展系统被称为AspectJ。之后，AspectJ在2002年被转让给Eclipse Foundation，从而成为在开源社区中AOP技术的先锋，也是目前最为流行的AOP工具。

AspectWerkz则是基于Java的动态的、轻量级AOP框架。AspectWerkz仍然是开源社区中的产品，由BEA System提供赞助，开发者则是BEA的两名员工Jonas Bonér和Alexandre Vasseur。最近版本是AspectWerkz 2.0。2005年1月，AspectJ和AspectWerkz达成协议，同意将二者的成果综合到一起，取其精华创建一个单一的工具。他们合作的第一个发布版本为AspectJ 5，它扩展了AspectJ语言，以支持基于Annotation开发风格而又支持类似AspectJ代码风格。AspectJ 5也为Java 5的语言特性提供完全的AOP支持。

在Java阵营中，商用软件制造商JBoss在其2004年推出的JBoss 4.0中，引入了AOP框架和组件。在JBoss 4.0中，用户可以在JBoss应用服务器外部单独使用JBoss AOP，该版本为JBoss AOP 1.0，是在2004年10月发布的。在2005年，JBoss AOP框架又发布了1.3.0版本，新版本对加载期织入（Weev）和切点（point cut）匹配的性能做了很大的优化，使应用程序的启动时间大大缩短。

作为轻型的Framework，Spring在开发轻量级的J2EE时，应用是非常广泛的。它通过IoC模式（Inversion of Control，控制反转模式）来实现AOP，通常被称为Spring AOP。在2004年，被作为Spring框架的扩展而发布，目前版本已更新到1.1.3。Spring AOP作为一种非侵略性的，轻型的AOP框架，开发者无需使用预编译器或其他的元标签，在Java程序中应用AOP。目前，AOP的功能完全集成到了Spring事务管理、日志和其他各种特性的上下文中。

在.Net的阵营中，AOP技术的应用远不如Java阵营对AOP的关注。2005年1月，微软发布的Enterprise Library提供了7种不同的“应用程序块（application blocks）”。有个别专家认为，这些组件可以被认为是方面。但该观点并未得到一致的认同。事实上，在.Net平台下，推动AOP技术发展的原动力并非微软，而是开源社区。虽然，微软的技术专家们亦然听到了在.Net Framework中增加AOP技术的群众呼声，但作为如此巨大的软件公司，要让它灵活地转变战略方向，显然是不太现实的。正因为此，才赐予了开源社区在AOP技术的研究与探索上一个巨大的发展空间。

与Java阵营中的AOP技术不同，目前在.Net平台下的各种AOP工具，基本上还停留在实验室阶段。但一些在技术上领先且逐渐成熟的AOP产品，也在开源社区中渐露峥嵘。这其中主要包括Aspect#，AspectDNG，Eos AOP等。

Aspect#是基于Castle动态代理技术来实现的。Castle源于Apache Avalon项目，其目的在于实现一个轻量级的IoC容器。Aspect#于2005年6月被收录为Castle的其中一个子项目。它是针对CLI（.Net和Mono）实现的AOP框架，利用了反射、代理等机制。目前的Aspect#版本为2.1.1。

AspectDNG目前的版本为0.7，仍然处于beta版的阶段。它的实现技术是基于rail的静态织入。Rail属于IL级别下的代码织入，它自定义的一套xml格式的ILML语言，能够将原有的程序集拆散成ILML格式，以便于对静态程序集进行修改和扩展，从而达到静态织入的目的。因为AspectDNG是属于IL级别下的代码织入，因此在.Net平台下，并不受具体的编程语言限制。

Eos AOP与AspectDNG一样，仍然采用静态织入的方式，但从语法定义上，它更近似于AspectJ关于AOP的实现。它扩展了C#语法，引入了aspect、introduce、before、after等关键字，并且提供了专用的Eos编译器。Eos项目是于2004年9月开始启动，2005年6月推出的0.3.3版本为最新版本，主要的开发人员为Hridesh Rajan和Kevin Sullivan。前者为Virginia大学计算机系的研究生，Eos项目最初是由Hridesh Rajan提出的；而后者则为该计算机系的副教授（Associate Professor）。所以自Eos诞生之初，就带有浓厚的学院派特色。

从AOP技术的整体发展来看，高性能、稳定、可扩展、易用的AOP框架是其趋势与目标。从上述对各种AOP技术的分析来看，AOP技术无疑是具有共同特点的，而各种实现技术就是围绕着这些共性深入与延伸。接下来，我将概要地介绍AOP的本质，以及它的技术要素。

2.2 AOP技术本质

2.2.1 技术概览

AOP（Aspect-Oriented Programming，面向方面编程），可以说是OOP（Object-Oriented Programing，面向对象编程）的补充和完善。OOP引入封装、继承和多态性等概念来建立一种对象层次结构，用以模拟公共行为的一个集合。当我们需要为分散的对象引入公共行为的时候，OOP则显得无能为力。也就是说，OOP允许你定义从上到下的关系，但并不适合定义从左到右的关系。例如日志功能。日志代码往往水平地散布在所有对象层次中，而与它所散布到的对象的核心功能毫无关系。对于其他类型的代码，如安全性、异常处理和透明的持续性也是如此。这种散布在各处的无关的代码被称为横切（cross-cutting）代码，在OOP设计中，它导致了大量代码的重复，而不利于各个模块的重用。

而AOP技术则恰恰相反，它利用一种称为“横切”的技术，剖解开封装的对象内部，并将那些影响了多个类的公共行为封装到一个可重用模块，并将其名为“Aspect”，即方面。所谓“方面”，简单地说，就是将那些与业务无关，却为业务模块所共同调用的逻辑或责任封装起来，便于减少系统的重复代码，降低模块间的耦合度，并有利于未来的可操作性和可维护性。AOP代表的是一个横向的关系，如果说“对象”是一个空心的圆柱体，其中封装的是对象的属性和行为；那么面向方面编程的方法，就仿佛一把利刃，将这些空心圆柱体剖开，以获得其内部的消息。而剖开的切面，也就是所谓的“方面”了。然后它又以巧夺天功的妙手将这些剖开的切面复原，不留痕迹。

使用“横切”技术，AOP把软件系统分为两个部分：核心关注点和横切关注点。业务处理的主要流程是核心关注点，与之关系不大的部分是横切关注点。横切关注点的一个特点是，他们经常发生在核心关注点的多处，而各处都基本相似。比如权限认证、日志、事务处理。Aop 的作用在于分离系统中的各种关注点，将核心关注点和横切关注点分离开来。正如Avanade公司的高级方案构架师Adam Magee所说，AOP的核心思想就是“将应用程序中的商业逻辑同对其提供支持的通用服务进行分离。”

实现AOP的技术，主要分为两大类：一是采用动态代理技术，利用截取消息的方式，对该消息进行装饰，以取代原有对象行为的执行；二是采用静态织入的方式，引入特定的语法创建“方面”，从而使得编译器可以在编译期间织入有关“方面”的代码。然而殊途同归，实现AOP的技术特性却是相同的，分别为：

1、join point（连接点）：是程序执行中的一个精确执行点，例如类中的一个方法。它是一个抽象的概念，在实现AOP时，并不需要去定义一个join point。
2、point cut（切入点）：本质上是一个捕获连接点的结构。在AOP中，可以定义一个point cut，来捕获相关方法的调用。
3、advice（通知）：是point cut的执行代码，是执行“方面”的具体逻辑。
4、aspect（方面）：point cut和advice结合起来就是aspect，它类似于OOP中定义的一个类，但它代表的更多是对象间横向的关系。
5、introduce（引入）：为对象引入附加的方法或属性，从而达到修改对象结构的目的。有的AOP工具又将其称为mixin。

上述的技术特性组成了基本的AOP技术，大多数AOP工具均实现了这些技术。它们也可以是研究AOP技术的基本术语。

2.2.2 横切技术

“横切”是AOP的专有名词。它是一种蕴含强大力量的相对简单的设计和编程技术，尤其是用于建立松散耦合的、可扩展的企业系统时。横切技术可以使得AOP在一个给定的编程模型中穿越既定的职责部分（比如日志记录和性能优化）的操作。

如果不使用横切技术，软件开发是怎样的情形呢？在传统的程序中，由于横切行为的实现是分散的，开发人员很难对这些行为进行逻辑上的实现或更改。例如，用于日志记录的代码和主要用于其它职责的代码缠绕在一起。根据所解决的问题的复杂程度和作用域的不同，所引起的混乱可大可小。更改一个应用程序的日志记录策略可能涉及数百次编辑——即使可行，这也是个令人头疼的任务。

在AOP中，我们将这些具有公共逻辑的，与其他模块的核心逻辑纠缠在一起的行为称为“横切关注点（Crosscutting Concern）”，因为它跨越了给定编程模型中的典型职责界限。

2.2.2.1 横切关注点

一个关注点（concern）就是一个特定的目的，一块我们感兴趣的区域，一段我们需要的逻辑行为。从技术的角度来说，一个典型的软件系统包含一些核心的关注点和系统级的关注点。举个例子来说，一个信用卡处理系统的核心关注点是借贷/存入处理，而系统级的关注点则是日志、事务完整性、授权、安全及性能问题等，许多关注点——即横切关注点（crosscutting concerns）——会在多个模块中出现。如果使用现有的编程方法，横切关注点会横越多个模块，结果是使系统难以设计、理解、实现和演进。AOP能够比上述方法更好地分离系统关注点，从而提供模块化的横切关注点。

例如一个复杂的系统，它由许多关注点组合实现，如业务逻辑、性能，数据存储、日志和调度信息、授权、安全、线程、错误检查等，还有开发过程中的关注点，如易懂、易维护、易追查、易扩展等，图2.1演示了由不同模块实现的一批关注点组成一个系统。

![aop2.1.gif](http://images.cnblogs.com/cnblogs_com/wayfarer/aop/aop2.1.gif)
图2.1 把模块作为一批关注点来实现

通过对系统需求和实现的识别，我们可以将模块中的这些关注点分为：核心关注点和横切关注点。对于核心关注点而言，通常来说，实现这些关注点的模块是相互独立的，他们分别完成了系统需要的商业逻辑，这些逻辑与具体的业务需求有关。而对于日志、安全、持久化等关注点而言，他们却是商业逻辑模块所共同需要的，这些逻辑分布于核心关注点的各处。在AOP中，诸如这些模块，都称为横切关注点。应用AOP的横切技术，关键就是要实现对关注点的识别。

如果将整个模块比喻为一个圆柱体，那么关注点识别过程可以用三棱镜法则来形容，穿越三棱镜的光束（指需求），照射到圆柱体各处，获得不同颜色的光束，最后识别出不同的关注点。如图2.2所示：

![aop2.2.gif](http://images.cnblogs.com/cnblogs_com/wayfarer/aop/aop2.2.gif)
图2.2 关注点识别：三棱镜法则

上图识别出来的关注点中，Business Logic属于核心关注点，它会调用到Security，Logging，Persistence等横切关注点。

public class BusinessLogic
{
    public void SomeOperation()
    {
       //验证安全性；Securtity关注点；
       //执行前记录日志；Logging关注点；

       DoSomething();

       //保存逻辑运算后的数据；Persistence关注点；
       //执行结束记录日志；Logging关注点；
    }
}

AOP的目的，就是要将诸如Logging之类的横切关注点从BusinessLogic类中分离出来。利用AOP技术，可以对相关的横切关注点封装，形成单独的“aspect”。这就保证了横切关注点的复用。由于BusinessLogic类中不再包含横切关注点的逻辑代码，为达到调用横切关注点的目的，可以利用横切技术，截取BusinessLogic类中相关方法的消息，例如SomeOperation()方法，然后将这些“aspect”织入到该方法中。例如图2.3：

![aop2.3.gif](http://images.cnblogs.com/cnblogs_com/wayfarer/aop/aop2.3.gif)
图2.3 将横切关注点织入到核心关注点中

通过利用AOP技术，改变了整个系统的设计方式。在分析系统需求之初，利用AOP的思想，分离出核心关注点和横切关注点。在实现了诸如日志、事务管理、权限控制等横切关注点的通用逻辑后，开发人员就可以专注于核心关注点，将精力投入到解决企业的商业逻辑上来。同时，这些封装好了的横切关注点提供的功能，可以最大限度地复用于商业逻辑的各个部分，既不需要开发人员作特殊的编码，也不会因为修改横切关注点的功能而影响具体的业务功能。

为了建立松散耦合的、可扩展的企业系统，AOP应用到的横切技术，通常分为两种类型：动态横切和静态横切。

2.2.2.2 动态横切

动态横切是通过切入点和连接点在一个方面中创建行为的过程，连接点可以在执行时横向地应用于现有对象。动态横切通常用于帮助向对象层次中的各种方法添加日志记录或身份认证。在很多应用场景中，动态横切技术基本上代表了AOP。

动态横切技术的核心主要包括join point（连接点），point cut（切入点），advice（通知）和aspect（方面）。在前面，我已经概要地介绍了这些术语分别代表的含义。接下来，我将以一个具体的实例来进一步阐述它们在AOP动态横切中实现的意义。

考虑一个电子商务系统，需要对订单进行添加、删除等管理操作。毫无疑问，在实际的应用场景中，这些行为应与权限管理结合，只有获得授权的用户方能够实施这些行为。采用传统的设计方法，其伪代码如下：
public class OrderManager
{
    private ArrayList m_Orders;
    public OrderManager()
    {
       m_Orders = new ArrayList();
    }
    public void AddOrder(Order order)
    {
        if (permissions.Verify(Permission.ADMIN))
        {

            m_Orders.Add(order);
        }
    }

    public void RemoveOrder(Order order)
    {
        if (permissions.Verify(Permission.ADMIN))
        {
            m_Orders.Remove(order);
        }
    }
}

同样的，在该电子商务系统中，还需要对商品进行管理，它采用了同样的授权机制：
public class ProductManager
{
    private ArrayList m_Products;
    public ProductManager()
    {
        m_Products = new ArrayList();
    }
    public void AddProduct(Product product)
    {
        if (permissions.Verify(Permission.ADMIN))
        {
             m_Products.Add(product);
        }
    }
    public void RemoveProduct(Product product)
    {
        if (permissions.Verify(Permission.ADMIN))
        {
             m_Products.Remove(product);
        }
    }
}

如此以来，在整个电子商务系统中，核心业务包括订单管理和商品管理，它们都需要相同的权限管理，如图2.4所示：

![aop2.4.gif](http://images.cnblogs.com/cnblogs_com/wayfarer/aop/aop2.4.gif)
图2.4 电子商务系统的权限验证实现

毫无疑问，利用AOP技术，我们可以分离出系统的核心关注点和横切关注点，从横向的角度，截取业务管理行为的内部消息，以达到织入权限管理逻辑的目的。当执行AddOrder()等方法时，系统将验证用户的权限，调用横切关注点逻辑，因此该方法即为AOP的join point。对于电子商务系统而言，每个需要权限验证的方法都是一个单独的join point。由于权限验证将在每个方法执行前执行，所以对于这一系列join point，只需要定义一个point cut。当系统执行到join point处时，将根据定义去查找对应的point cut，然后执行这个横切关注点需要实现的逻辑，即advice。而point cut和advice，就组合成了一个权限管理aspect。

![aop2.5.gif](http://images.cnblogs.com/cnblogs_com/wayfarer/aop/aop2.5.gif)
图2.5 AOP动态横切的技术实现

由于aspect是一个封装的对象，我们可以定义这样一个aspect：
private static aspect AuthorizationAspect{……}

然后在这个aspect中定义point cut，在point cut中，定义了需要截取上下文消息的方法，例如：
private pointcut authorizationExecution():
execution(public void OrderManager.AddOrder(Order)) ||
execution(public void OrderManager.DeleteOrder(Order)) ||
execution(public void ProductManager.AddProduct(Product)) ||
execution(public void ProductManager.DeleteProduct(Product));

由于权限验证是在订单管理方法执行之前完成，因此在before advice中，定义权限检查：
before(): authorizationExecution()
{
    if !(permissions.Verify(Permission.ADMIN))
    {
        throw new UnauthorizedException();
    }
}

通过定义了这样一个完整的aspect，当系统调用OrderManager或ProductManager的相关方法时，就触发了point cut，然后调用相应的advice逻辑。如此以来，OrderManager和ProductManager模块就与权限管理模块完全解除了依赖关系，同时也消除了传统设计中不可避免的权限判断的重复代码。这对于建立一个松散耦合、可扩展的系统软件是非常有利的。

2.2.2.3 静态横切

静态横切和动态横切的区别在于它不修改一个给定对象的执行行为。相反，它允许通过引入附加的方法字段和属性来修改对象的结构。此外，静态横切可以把扩展和实现附加到对象的基本结构中。在AOP实现中，通常将静态横切称为introduce或者mixin。

静态横切在AOP技术中，受到的关注相对较少。事实上，这一技术蕴含的潜力是巨大的。使用静态横切，架构师和设计者能用一种真正面向对象的方法有效地建立复杂系统的模型。静态横切允许您不用创建很深的层次结构，以一种本质上更优雅、更逼真于现实结构的方式，插入跨越整个系统的公共行为。尤其是当开发应用系统时，如果需要在不修改原有代码的前提下，引入第三方产品和API库，则静态横切技术将发挥巨大的作用。

举例来说，当前已经实现了一个邮件收发系统，其中类Mail完成了收发邮件的功能。但在产品交付后，发现该系统存在缺陷，在收发邮件时，未曾实现邮件地址的验证功能。现在，第三方产品已经提供了验证功能的接口IValidatable：
public interface IValidatable
{
    bool ValidateAddress();
}

我们可以利用设计模式中的Adapter模式，来完成对第三方产品API的调用。我们可以定义一个新的类MailAdapter，该类实现了IValidatable接口，同时继承了Mail类：
public class MailAdapter:Mail,IValidatable
{
     public bool ValidateAddress()
     {
         if(this.getToAddress() != null)
         {
             return true;
         }
         else
         {
             return false;
         }
     }
}

通过引入MailAdapter类，原来Mail对象完成的操作，将全部被MailAdapter对象取代。然而，此种实现方式虽然能解决引入新接口的问题，但类似下面的代码，却是无法编译通过的：
Mail mail = new Mail();
IValidatable validate = ((IValidatable)mail).ValidateAddress();

必须将第一行代码作如下修改：
Mail mail = new MailAdapter();

利用AOP的静态横切技术，可以将IValidatable接口织入到原有的Mail类中，这是一种非常形象的introduce功能，其实现仍然是在aspect中完成：
import com.acme.validate.Validatable;

public aspect MailValidateAspect
{
    declare parents: Mail implements IValidatable;

    public boolean Mail.validateAddress()
    {
         if(this.getToAddress() != null)
         {
              return true;
         }
         else
         {
              return false;
         }
    }
}

静态横切的方法，并没有引入类似MailAdapter的新类，而是通过定义的MailValidateAspect方面，利用横切技术为Mail类introduce了新的方法ValidateAddress()，从而实现了Mail的扩展。因此如下的代码完全可行。
Mail mail = new Mail();
IValidatable validate = ((IValidatable)mail).ValidateAddress();

2.3 AOP技术的优势

AOP技术的优势是显而易见的。在面向对象的世界里，人们提出了各种方法和设计原则来保障系统的可复用性与可扩展性，以期建立一个松散耦合、便于扩展的软件系统。例如GOF提出的“设计模式”，为我们提供了设计的典范与准则。设计模式通过最大程度的利用面向对象的特性，诸如利用继承、多态，对责任进行分离、对依赖进行倒置，面向抽象，面向接口，最终设计出灵活、可扩展、可重用的类库、组件，乃至于整个系统的架构。在设计的过程中，通过各种模式体现对象的行为、暴露的接口、对象间关系、以及对象分别在不同层次中表现出来的形态。然而鉴于对象封装的特殊性，“设计模式”的触角始终在接口与抽象中大做文章，而对于对象内部则无能为力。

通过“横切”技术，AOP技术就能深入到对象内部翻云覆雨，截取方法之间传递的消息为我所用。由于将核心关注点与横切关注点完全隔离，使得我们能够独立的对“方面”编程。它允许开发者动态地修改静态的OO模型，构造出一个能够不断增长以满足新增需求的系统，就象现实世界中的对象会在其生命周期中不断改变自身，应用程序也可以在发展中拥有新的功能。

设计软件系统时应用AOP技术，其优势在于：

（一）在定义应用程序对某种服务（例如日志）的所有需求的时候。通过识别关注点，使得该服务能够被更好的定义，更好的被编写代码，并获得更多的功能。这种方式还能够处理在代码涉及到多个功能的时候所出现的问题，例如改变某一个功能可能会影响到其它的功能，在AOP中把这样的麻烦称之为“纠结（tangling）”。

（二）利用AOP技术对离散的方面进行的分析将有助于为开发团队指定一位精于该项工作的专家。负责这项工作的最佳人选将可以有效利用自己的相关技能和经验。

（三）持久性。标准的面向对象的项目开发中，不同的开发人员通常会为某项服务编写相同的代码，例如日志记录。随后他们会在自己的实施中分别对日志进行处理以满足不同单个对象的需求。而通过创建一段单独的代码片段，AOP提供了解决这一问题的持久简单的方案，这一方案强调了未来功能的重用性和易维护性：不需要在整个应用程序中一遍遍重新编写日志代码，AOP使得仅仅编写日志方面（logging aspect）成为可能，并且可以在这之上为整个应用程序提供新的功能。

总而言之，AOP技术的优势使得需要编写的代码量大大缩减，节省了时间，控制了开发成本。同时也使得开发人员可以集中关注于系统的核心商业逻辑。此外，它更利于创建松散耦合、可复用与可扩展的大型软件系统。



