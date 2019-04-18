# 依赖注入 javax.inject中@Inject、@Named、@Qualifier和@Provider用法 - z69183787的专栏 - CSDN博客
2014年08月19日 19:25:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：34121
**这个是 Java EE 6 规范 JSR 330 -- Dependency Injection for Java 中的东西，也就是 Java EE 的依赖注入。根据 API document 上的说明，被 @Inject 标注的构造、成员字段和方法是可注入的。其包可以在 jcp.org 上找到，并可以在这里下载：[https://cds.sun.com/is-bin/INTERSHOP.enfinity/WFS/CDS-CDS_JCP-Site/en_US/-/USD/ViewProductDetail-Start?ProductRef=dependency_injection-1.0-final-oth-JSpec@CDS-CDS_JCP](https://cds.sun.com/is-bin/INTERSHOP.enfinity/WFS/CDS-CDS_JCP-Site/en_US/-/USD/ViewProductDetail-Start?ProductRef=dependency_injection-1.0-final-oth-JSpec@CDS-CDS_JCP)**
Spring自带的@Autowired的缺省情况等价于JSR-330的@Inject注解；
Spring自带的@Qualifier的缺省的根据Bean名字注入情况等价于JSR-330的@Named注解；
Spring自带的@Qualifier的扩展@Qualifier限定描述符注解情况等价于JSR-330的@Qualifier注解。
用过Spring框架的我们都知道，每当生成依赖注入的时候，我们都必须生成相应类的set方法，而且要在set方法上面写上@Autowired，才能实现依赖注入，如下：
Java代码  ![收藏代码](http://meigesir.iteye.com/images/icon_star.png)
- **package** com.kaishengit.web;  
- 
- **import** com.kaishengit.service.ProjectService;  
- **import** org.springframework.beans.factory.annotation.Autowired;  
- **import** org.springframework.stereotype.Controller;  
- 
- @Controller
- **public****class** FolderController {  
- **private** ProjectService projectService;  
- 
- //set
- @Autowired
- **public****void** setProjectService(ProjectService projectService) {  
- **this**.projectService = projectService;  
-     }  
- }  
 每次都要生成相应的set方法感觉好麻烦，现在如果我们使用javax.inject.jar，只需要在相应类的属性上面加上@Inject，如下代码：
Java代码  ![收藏代码](http://meigesir.iteye.com/images/icon_star.png)
- **package** com.kaishengit.web;  
- 
- **import** com.kaishengit.service.ProjectService;  
- **import** org.springframework.stereotype.Controller;  
- 
- **import** javax.inject.Inject;  
- 
- @Controller
- **public****class** FolderController {  
- @Inject
- **private** ProjectService projectService;  
- 
- 
- }  
 javax.inject.jar下载地址：[https://code.google.com/p/dependency-shot/downloads/detail?name=javax.inject.jar&can=2&q=](https://code.google.com/p/dependency-shot/downloads/detail?name=javax.inject.jar&can=2&q=)
**@Inject**
    @Inject支持构造函数、方法和字段注解，也可能使用于静态实例成员。可注解成员可以是任意修饰符（private,package-private,protected,public）。注入顺序：构造函数、字段，然后是方法。父类的字段和方法注入优先于子类的字段和方法，同一类中的字段和方法是没有顺序的。
    @Inject注解的构造函数可以是无参或多个参数的构造函数。@Inject每个类中最多注解一个构造函数。
    在字段注解：
- 用@Inject注解
- 字段不能是final的
- 拥有一个合法的名称
    在方法上注解：
- 用@Inject注解
- 不能是抽象方法
- 不能声明自身参数类型
- 可以有返回结果
- 拥有一个合法的名称
- 可以有0个或多个参数
        @Inject *MethodModirers ResultType Identifier(FormalParameterList ) Throws MethodBody*
    [上述翻译：inject的doc文档，翻译不好敬请谅解]
    构造函数注解：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Inject
- public House(Person owner) {  
-     System.out.println("---这是房屋构造函数---");  
- this.owner = owner;  
- }  
    字段注解：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Injectprivate Person owner;  
    方法注解：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Inject
- publicvoid setOwner(Person owner) {  
- this.owner = owner;  
- }  
    @Inject注解和Spring的@Autoware注解都是根据类型对其进行自动装配。
    SpringUtil类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- publicclass SpringUtil {  
- privatestatic ApplicationContext context = null;  
- publicstatic ApplicationContext getApplicationContext() {  
- if (context == null) {  
-             context = new ClassPathXmlApplicationContext("spring.xml");  
-         }  
- return context;  
-     }  
- 
- publicstatic ApplicationContext getApplicationContext(String path) {  
- returnnew ClassPathXmlApplicationContext(path);  
-     }  
- 
- publicstatic ApplicationContext getAnnotationConfigApplicationContext(String basePackages) {  
- returnnew AnnotationConfigApplicationContext(basePackages);  
-     }  
- }  
Person类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- import javax.inject.Named;  
- 
- @Named
- publicclass Person {  
- private String name;  
- 
- public Person() {  
-         System.out.println("---这是人的构造函数---");  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- }  
House类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Named
- publicclass House {  
- @Injectprivate Person owner;  
- public House() {  
-         System.out.println("---这是房屋构造函数---");  
-     }  
- 
- public Person getOwner() {  
- return owner;  
-     }  
- 
- publicvoid setOwner(Person owner) {  
- this.owner = owner;  
-     }  
- }  
测试类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- publicclass Test {  
- publicstaticvoid main(String[] args) {  
-         ApplicationContext context = SpringUtil.getApplicationContext(  
- "test/spring/inject/bean-inject.xml");  
-         House house = (House)context.getBean("house");  
-         Person p = house.getOwner();  
-         p.setName("张三");  
-         System.out.println(house.getOwner().getName());  
-     }  
- }  
输出结果：
---这是房屋构造函数---
---这是人的构造函数---
张三
    上述例子在Spring3.1下测试成功，在Spring3.1下，每个构造函数只初始化一次及默认的单例形式，个人感觉如果脱离Spring环境应该每次用都会实例化新的对象，当然根据实现的jar包不同而不同，要不javax.inject下的@Singleton注解就没有什么用途了。
**@Named**
    @Named和Spring的@Component功能相同。@Named可以有值，如果没有值生成的Bean名称默认和类名相同。
    例如：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Namedpublicclass Person  
    该bean的名称就是person。
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Named("p") publicclass Person  
    如果指定名称，那么就是指定的名称喽。
**@Qualifier**
    任何人都可以定义一个新的修饰语，一个qualifier注解应该满足如下条件：
- 定义的注解类有@Qualifier，@Retention(RUNTIME)和@Documented。
- 可以有属性
- 可以是公共API的一部分
- 可以用@Target注解限定使用范围
    下面是Qualifier的例子：
Genre注解类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Documented
- @Retention(RetentionPolicy.RUNTIME)  
- @Qualifier
- @Target(value = {ElementType.FIELD, ElementType.PARAMETER, ElementType.TYPE})  
- public@interface Genre {  
-     User user() default User.STUDENT;  
- publicenum User {STUDENT, TEACHER}  
- }  
用户接口：（对个数进行统计）
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- publicinterface IUserDAO {  
- int count();  
- }  
StudentDAO：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Named
- @Genre(user = User.STUDENT)  
- publicclass StudentDAO implements IUserDAO{  
- @Override
- publicint count() {  
-         System.out.println("----StudentDAO----");  
- return0;  
-     }  
- 
- }  
TeacherDAO：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Named
- @Genre(user = User.TEACHER)  
- publicclass TeacherDAO implements IUserDAO {  
- 
- @Override
- publicint count() {  
-         System.out.println("--TeacherDAO--");  
- return0;  
-     }  
- }  
UserDAOProcessor：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- @Named
- publicclass UserDAOProcessor {  
- /*对TeacherDAO类的注入，如果对StudentDAO类注入应该是：@Genre(user = User.STUDENT)或@Genre，因为@Genre默认的是STUDENT*/
- @Inject
- private@Genre(user = User.TEACHER) IUserDAO userDAO;   
- 
- publicint count() {  
- return userDAO.count();  
-     }  
- 
- public IUserDAO getUserDAO() {  
- return userDAO;  
-     }  
- 
- publicvoid setUserDAO(IUserDAO userDAO) {  
- this.userDAO = userDAO;  
-     }  
- }  
测试类：
**[java]**[view
 plain](http://blog.csdn.net/binxigogo/article/details/7851924#)[copy](http://blog.csdn.net/binxigogo/article/details/7851924#)
- publicclass Test {  
- publicstaticvoid main(String[] args) {  
-         ApplicationContext context = SpringUtil.getApplicationContext(  
- "test/spring/inject/bean-inject.xml");  
-         UserDAOProcessor processor = (UserDAOProcessor)context.getBean("userDAOProcessor");  
-         System.out.println(processor.count());  
-     }  
- }  
输出结果：
--TeacherDAO--
0
    个人对@Qualifier的理解：
- 和Spring的@Qualifier大致相同
- 单独用@Inject无法满足对接口的注入，无法找到哪个具体类，所以用@Qualifier来确定注入的具体类
- 用到@Qualifier的注解中可以有值、无值和用枚举类型
**@Singleton**
    使用该注解标记该类只创建一次，不能被继承。一般在类上用该注解。
Spring自带的@Autowired的缺省情况等价于JSR-330的@Inject注解；
Spring自带的@Qualifier的缺省的根据Bean名字注入情况等价于JSR-330的@Named注解；
Spring自带的@Qualifier的扩展@Qualifier限定描述符注解情况等价于JSR-330的@Qualifier注解。
