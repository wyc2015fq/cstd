# 单元测试系列之3：测试整合之王Unitils - z69183787的专栏 - CSDN博客
2015年02月05日 20:19:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1193
个人分类：[测试-单元测试/压测工具](https://blog.csdn.net/z69183787/article/category/2881107)
**引述**：程序测试对保障应用程序正确性而言，其重要性怎么样强调都不为过。JUnit是必须事先掌握的测试框架，大多数测试框架和测试工具都在此基础上扩展而来，Spring对测试所提供的帮助类也是在JUnit的基础上进行演化的。直接使用JUnit测试基于Spring的应用存在诸多不便，不可避免地需要将大量的精力用于应付测试夹具准备、测试现场恢复、访问测试数据操作结果等边缘性的工作中。Mockito、Unitils、Dbunit等框架的出现，这些问题有了很好的解决方案，特别是Unitils结合Dbunit对测试DAO层提供了强大的支持，大大提高了编写测试用例的效率和质量。 
Unitils海纳百川（Junit,dbunit,mockito
 spring hibernate and so on..），以打造一个在实际应用开发中真正实战的测试框架，是致力于应用实战的开发者不得不学习的开源框架。
Unitils概述
   Unitils测试框架目的是让单元测试变得更加容易和可维护。Unitils构建在DbUnit与EasyMock项目之上并与JUnit和TestNG相结合。支持数据库测试，支持利用Mock对象进行测试并提供与Spring和Hibernate相集成。Unitils设计成以一种高度可配置和松散耦合的方式来添加这些服务到单元测试中，目前其最新版本是3.1。 
Unitils功能特点
-  自动维护和强制关闭单元测试数据库（支持Oracle、Hsqldb、MySQL、DB2）。
-  简化单元测试数据库连接的设置。
-  简化利用DbUnit测试数据的插入。
-  简化Hibernate session管理。
-  自动测试与数据库相映射的Hibernate映射对象。
-  易于把Spring管理的Bean注入到单元测试中，支持在单元测试中使用Spring容器中的Hibernate SessionFactory。
-  简化EasyMock Mock对象创建。
-  简化Mock对象注入，利用反射等式匹配EasyMock参数。
Unitils模块组件
   Unitils通过模块化的方式来组织各个功能模块，采用类似于Spring的模块划分方式，如unitils-core、unitils-database、unitils-mock等。比以前整合在一个工程里面显得更加清晰，目前所有模块如下所示： 
-   unitils-core：核心内核包。
-   unitils-database：维护测试数据库及连接池。
-   unitils-DbUnit：使用DbUnit来管理测试数据。
-   unitils-easymock：支持创建Mock和宽松的反射参数匹配。
-   unitils-inject：支持在一个对象中注入另一个对象。
-   unitils-mock：整合各种Mock，在Mock的使用语法上进行了简化。
-   unitils-orm：支持Hibernate、JPA的配置和自动数据库映射检查。
-   unitils-spring：支持加载Spring的上下文配置，并检索和Spring Bean注入。
   Unitils的核心架构中包含Moudule和TestListener两个概念，类似Spring中黏连其他开源软件中的FactoryBean概念。可以看成第三方测试工具的一个黏合剂。整体框架如图16-4所示： 
![](http://dl.iteye.com/upload/attachment/0066/2317/4c6e08a4-3fab-3304-a6d1-c7e6b7dea9c3.jpg)
   通过TestListener可以在测试运行的不同阶段注入某些功能。同时某一个TestListener又被一个对应的Module所持有。Unitils也可以看成一个插件体系结构，TestListener在整个Unitils中又充当了插件中扩展点的角色，从TestListener这个接口中我们可以看到，它可以在crateTestObject、before(after)Class、before(after)TestMethod、beforeSetup、afterTeardown的不同切入点添加不同的动作。 
Unitils配置文件
-   unitils-defaults.properties：默认配置文件，开启所有功能。
-   unitils.properties：项目级配置文件，用于项目通用属性配置。
-   unitils-local.properties：用户级配置文件，用于个人特殊属性配置。
    Unitils的配置定义了一般配置文件的名字unitils.properties和用户自定义配置文件unitils-local.properties，并给出了默认的模块及模块对应的className，便于Unitils加载对应的模块module。但是如果用户分别在unitils.properties文件及unitils
 -local.properties文件中对相同属性配置不同值时，将会以unitils-local.properties 的配置内容为主。 
Unitils断言
   典型的单元测试一般都包含一个重要的组成部分：对比实际产生的结果和希望的结果是否一致的方法，即断言方法（assertEquals）。Unitils 为我们提供了一个非常实用的断言方法，我们以第2章中编写的用户领域对象User为蓝本，比较两个User对象的实例来开始认识Unitils的断言之旅。 
assertReflectionEquals：反射断言
    在Java世界中，要比较现有两个对象实例是否相等，如果类没有重写equals()方法，用两个对象的引用是否一致作为判断依据。有时候，我们并不需要关注两个对象是否引用同一个对象，只要两个对象的属性值一样就可以了。在JUnit单元测试中，有两种测试方式进行这样的场景测试：一是在比较实体类中重写equals()方法，然后进行对象比较；二是把对象实例的属性一个一个进行比较。不管采用哪种方法，都比较烦锁，Unitils为我们提供了一种非常简单的方法，即使用ReflectionAssert.assertReflectionEquals方法，
 如代码清单16-11所示： 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- package com.baobaotao.test;  
- import java.util.*;  
- import org.junit.Test;  
- importstatic org.unitils.reflectionassert.ReflectionAssert.*;  
- importstatic org.unitils.reflectionassert.ReflectionComparatorMode.*;  
- import com.baobaotao.domain.User;  
- 
- publicclass AssertReflectionEqualsTest {  
- @Test
- publicvoid testReflection(){  
-         User user1 = new User("tom","1234");  
-         User user2 = new User("tom","1234");  
-         ReflectionAssert.assertReflectionEquals(user1, user2);  
-     }  
- 
- }  
    ReflectionAssert. AssertReflectionEquals（期望值，实际值，比较级别）方法为我们提供了各种级别的比较断言。下面我们依次介绍这些级别的比较断言。 
    ReflectionComparatorMode.LENIENT_ORDER：忽略要断言集合collection 或者array 中元素的顺序。 
    ReflectionComparatorMode.IGNORE_DEFAULTS：忽略Java类型默认值，如引用类型为null，整型类型为0，或者布尔类型为false时，那么断言忽略这些值的比较。 
    ReflectionComparatorMode.LENIENT_DATES：比较两个实例的Date是不是都被设置了值或者都为null，而忽略Date的值是否相等。 
assertLenientEquals：断言
   ReflectionAssert 类为我们提供了两种比较断言：既忽略顺序又忽略默认值的断言assertLenientEquals，使用这种断言就可以进行简单比较。下面通过实例学习其具体的用法，如代码清单16-12所示。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- package com.baobaotao.test;  
- import java.util.*;  
- …  
- publicclass AssertReflectionEqualsTest {  
-       Integer orderList1[] = new Integer[]{1,2,3};  
-       Integer orderList2[] = new Integer[]{3,2,1};  
- 
- 
- //① 测试两个数组的值是否相等，忽略顺序
- //assertReflectionEquals(orderList1, orderList2,LENIENT_ORDER);
- assertLenientEquals(orderList1, orderList2);  
- 
- 
- //② 测试两个对象的值是否相等，忽略时间值是否相等
-         User user1 = new User("tom","1234");  
-         Calendar cal1 = Calendar.getInstance();  
-         user1.setLastVisit(cal1.getTime());  
-         User user2 = new User("tom","1234");  
-         Calendar cal2 = Calendar.getInstance();  
-         cal2.set(Calendar.DATE, 15);  
-         user2.setLastVisit(cal2.getTime());  
- //assertReflectionEquals(user1, user2,LENIENT_DATES);
-         assertLenientEquals(user1, user2);  
- }  
assertPropertyXxxEquals：属性断言
   assertLenientEquals 和assertReflectionEquals 这两个方法是把对象作为整体进行比较，ReflectionAssert 类还给我们提供了只比较对象特定属性的方法：assertPropertyReflection   Equals()和assertPropertyLenientEquals()。下面通过实例学习其具体的用法，如代码清单16-13所示。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- package com.baobaotao.test;  
- import java.util.*;  
- …  
- publicclass AssertReflectionEqualsTest {  
-       User user = new User("tom","1234");  
-       assertPropertyReflectionEquals("userName", "tom", user);  
-       assertPropertyLenientEquals("lastVisit", null, user);  
- }  
   assertPropertyReflectionEquals()断言是默认严格比较模式但是可以手动设置比较级别的断言，assertPropertyLenientEquals()断言是具有忽略顺序和忽略默认值的断言。 
集成Spring
   Unitils 提供了一些在Spring 框架下进行单元测试的特性。Spring 的一个基本特性就是，类要设计成为没有Spring 容器或者在其他容器下仍然易于进行单元测试。但是很多时候在Spring 容器下进行测试还是非常有用的。 
   Unitils 提供了以下支持 Spring 的特性： 
-   ApplicationContext 配置的管理；
-  在单元测试代码中注入Spring 的Beans；
-  使用定义在Spring 配置文件里的Hibernate SessionFactory；
-  引用在Spring 配置中Unitils 数据源。
ApplicationContext 配置
   可以简单地在一个类、方法或者属性上加上@SpringApplicationContext 注解，并用Spring的配置文件作为参数，来加载Spring应用程序上下文。下面我们通过实例来介绍一下如何创建ApplicationContext。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- import org.junit.Test;  
- import org.springframework.context.ApplicationContext;  
- import org.unitils.UnitilsJUnit4;  
- import org.unitils.spring.annotation.SpringApplicationContext;  
- import org.unitils.spring.annotation.SpringBean;  
- import com.baobaotao.service.UserService;  
- importstatic org.junit.Assert.*;  
- //①用户服务测试
- publicclass UserServiceTest extends UnitilsJUnit4 {  
- 
- //①-1 加载Spring配置文件
- @SpringApplicationContext({"baobaotao-service.xml", "baobaotao-dao.xml"})  
- private ApplicationContext applicationContext;  
- 
- //①-1 加载Spring容器中的Bean
- @SpringBean("userService")  
- private UserService userService;  
- 
- //①-3 测试Spring容器中的用户服务Bean
- @Test
- publicvoid testUserService (){  
-         assertNotNull(applicationContext);  
-         assertNotNull(userService.findUserByUserName("tom"));  
-     }  
- }  
- …  
   在①-1处，通过@SpringApplicationContext 注解加载baobaotao-service.xml和baobaotao- dao.xml两个配置文件，生成一个Spring应用上下文，我们就可以在注解的范围内引用applicationContext这个上下文。在①-2处，通过@SpringBean注解注入当前Spring容器中相应的Bean，如实例中加载ID为“userService”的Bean到当前测试范围。在①-3处，通过JUnit断言验证是否成功加载applicationContext和userService。Unitils加载Spring上下文的过程是：首先扫描父类的@SpringApplicationContext注解，如果找到了就在加载子类的配置文件之前加载父类的配置文件，这样就可以让子类重写配置文件和加载特定配置文件。 
   细心的读者可能会发现，采用这种方式加载Spring应用上下文，每次执行测试时，都会重复加载Spring应用上下文。Unitils为我们提供在类上加载Spring应用上下文的能力，以避免重复加载的问题。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- …  
- @SpringApplicationContext({"baobaotao-service.xml", "baobaotao-dao.xml"})  
- publicclass BaseServiceTest extends UnitilsJUnit4 {  
- 
- //加载Spring上下文
- @SpringApplicationContext
- public ApplicationContext applicationContext;  
- 
- }  
   在父类BaseServiceTest里指定了Spring配置文件，Spring应用上下文只会创建一次，然后在子类SimpleUserServiceTest 里会重用这个应用程序上下文。加载Spring应用上下文是一个非常繁重的操作，如果重用这个Spring应用上下文就会大大提升测试的性能。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- …  
- publicclass SimpleUserServiceTest extends BaseServiceTest {  
- 
- //① Spring容器中加载Id为"userService"的Bean
- @SpringBean("userService")  
- private UserService userService1;  
- 
- //② 从Spring容器中加载与UserService相同类型的Bean
- @SpringBeanByType
- private UserService userService2;  
- 
- //③ 从Spring容器中加载与userService相同名称的Bean
- @SpringBeanByName
- private  UserService userService;  
- 
- //④ 使用父类的Spring上下文
- @Test
- publicvoid testApplicationContext(){  
-         assertNotNull(applicationContext);  
-     }  
- 
- @Test
- publicvoid testUserService(){  
-         assertNotNull(userService.findUserByUserName("tom"));  
-         assertNotNull(userService1.findUserByUserName("tom"));  
-         assertNotNull(userService2.findUserByUserName("tom"));  
-     }  
- }  
- …  
   在①处，使用@SpringBean 注解从Spring容器中加载一个ID为userService的Bean。在②处，使用@ SpringBeanByType注解从Spring容器中加载一个与UserService相同类型的Bean，如果找不到相同类型的Bean，就会抛出异常。在③处，使用@SpringBeanByName
 注解从Spring容器中加载一个与当前属性名称相同的Bean。 
集成Hibernate
   Hibernate是一个优秀的O / R开源框架，它极大地简化了应用程序的数据访问层开发。虽然我们在使用一个优秀的O/R框架，但并不意味我们无须对数据访问层进行单元测试。单元测试仍然非常重要。它不仅可以确保Hibernate映射类的映射正确性，也可以很便捷地测试HQL查询等语句。Unitils为方便测试 Hibernate，提供了许多实用的工具类，如HibernateUnitils就是其中一个，使用assertMappingWithDatabaseConsistent()方法，就可以方便测试映射文件的正确性。 
SessionFactory 配置
   可以简单地在一个类、方法或者属性上加上@ HibernateSessionFactory 注解，并用Hibernate的配置文件作为参数，来加载Hibernate上下文。下面我们通过实例来介绍一下如何创建SessionFactory。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- …  
- @HibernateSessionFactory("hibernate.cfg.xml")  
- publicclass BaseDaoTest extends UnitilsJUnit4 {  
- @HibernateSessionFactory
- public SessionFactory sessionFactory;  
- 
- 
- @Test
- publicvoid testSessionFactory(){  
-         assertNotNull(sessionFactory);  
-     }  
- }  
    在父类BaseDaoTest里指定了Hibernate配置文件，Hibernate应用上下文只会创建一次，然后在子类SimpleUserDaoTest里会重用这个应用程序上下文。加载Hibernate应用上下文是一个非常繁重的操作，如果重用这个Hibernate应用上下文就会大大提升测试的性能。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- …  
- publicclass SimpleUserDaoTest extends BaseDaoTest {  
- private UserDao userDao;  
- 
- //① 初始化UserDao
- @Before
- publicvoid init(){  
-          userDao = new WithoutSpringUserDaoImpl();  
-         userDao.setSessionFactory(sessionFactory); //使用父类的SessionFactory
-     }  
- 
- //② Hibernate映射测试
- @Test
- publicvoid testMappingToDatabase() {  
-         HibernateUnitils.assertMappingWithDatabaseConsistent();  
- }  
- 
- //③ 测试UserDao
- @Test
- publicvoid testUserDao(){  
-         assertNotNull(userDao);  
-         assertNotNull(userDao.findUserByUserName("tom"));  
-         assertEquals("tom", userDao.findUserByUserName("tom").getUserName());  
-     }  
- }  
- …  
   为了更好演示如何应用Unitils测试基于Hibernate数据访问层，在这个实例中不使用Spring框架。所以在执行测试时，需要先创建相应的数据访问层实例，如实例中的userDao。其创建过程如①处所示，先手工实例化一个UserDao，然后获取父类中创建的SessionFactory，并设置到UserDao中。在②处，使用Unitils提供的工具类HibernateUnitils中的方法测试我们的Hibernate映射文件。在③处，通过JUnit的断言验证
 UserDao相关方法，看是否与我们预期的结果一致。 
集成Dbunit
   Dbunit是一个基于JUnit扩展的数据库测试框架。它提供了大量的类，对数据库相关的操作进行了抽象和封装。Dbunit通过使用用户自定义的数据集以及相关操作使数据库处于一种可知的状态，从而使得测试自动化、可重复和相对独立。虽然不用Dbunit也可以达到这种目的，但是我们必须为此付出代价（编写大量代码、测试及维护）。既然有了这么优秀的开源框架，我们又何必再造轮子。目前其最新的版本是2.4.8。 
    随着Unitils的出现，将Spring、Hibernate、DbUnit等整合在一起，使得DAO层的单元测试变得非常容易。Unitils采用模块化方式来整合第三方框架，通过实现扩展模块接口org.unitils.core.Module来实现扩展功能。在Unitils中已经实现一个DbUnitModule，很好整合了DbUnit。通过这个扩展模块，就可以在Unitils中使用Dbunit强大的数据集功能，如用于准备数据的@DataSet注解、用于验证数据的@ExpectedDataSet注解。Unitils集成DbUnit流程图如图16-5所示。 
![](http://dl.iteye.com/upload/attachment/0066/2324/282cc613-3a3d-31de-8f1e-18cc865a7ad7.jpg)
16.4.5  自定义扩展模块
   Unitils通过模块化的方式来组织各个功能模块，对外提供一个统一的扩展模块接口org.unitils.core.Module来实现与第三方框架的集成及自定义扩展。在Unitils中已经实现目前一些主流框架的模块扩展，如Spring、Hibernate、DbUnit、Testng等。如果这些内置的扩展模块无法满足需求，我们可以实现自己的一些扩展模块。扩展Unitils模块很简单，如代码清单16-19所示。 
Java代码  ![收藏代码](http://stamen.iteye.com/images/icon_star.png)
- package sample.unitils.module;  
- import java.lang.reflect.Method;  
- import org.unitils.core.TestListener;  
- import org.unitils.core. Module;  
- //① 实现Module接口
- publicclass CustomExtModule implements Module {  
- //② 实现获取测试监听的方法
- public TestListener getTestListener() {  
- returnnew CustomExtListener();  
-     }  
- 
- //② 新建监听模块 
- protectedclass CustomExtListener extends TestListener {  
- //③ 重写 TestListener里的相关方法，完成相关扩展的功能
- @Override
- publicvoid afterTestMethod(Object testObject, Method testMethod,  
-                 Throwable testThrowable) {  
- …  
-         }  
- 
- @Override
- publicvoid beforeTestMethod(Object testObject, Method testMethod) {  
- …  
-         }  
-     }  
- …  
- }  
在①处新建自定义扩展模块CustomExtModule，实现Module接口。在②处新建自定义监听模块，继承TestListener。在③处重写（@Override）TestListener里的相关方法，完成相关扩展的功能。实现自定义扩展模块之后，剩下的工作就是在Unitils配置文件unitils.properties中注册这个自定义扩展的模块： 
引用
unitils.modules=…,custom 
unitils.module. custom.className= sample.unitils.module.CustomExtModule
   这些文章摘自于我的《Spring 3.x企业应用开发实战》的第16章，我将通过连载的方式，陆续在此发出。欢迎大家讨论。
