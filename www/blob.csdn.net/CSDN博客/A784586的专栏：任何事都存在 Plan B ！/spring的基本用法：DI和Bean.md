# spring的基本用法：DI和Bean - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月09日 17:51:31[QuJack](https://me.csdn.net/A784586)阅读数：313标签：[spring基本用法																[spring中的DI																[依赖注入DI																[控制反转IOC																[Bean](https://so.csdn.net/so/search/s.do?q=Bean&t=blog)
个人分类：[spring](https://blog.csdn.net/A784586/article/category/6909325)





**spring的基本用法：DI和Bean**



spring的基本用法

关键词：spring介绍；依赖注入DI；设值注入和构造注入；




**1.通常**项目中各个组件的耦合方式：工厂模式，服务定位器模式；

**2.spring介绍**【官网：springsource.org】

spring从实际开发的项目中抽取，完成大量开发中的通用步骤；提供一个template的设计哲学。

企业应用开发的轻量级解决方案：基于依赖注入DI核心机制，基于AOP的申明式事务管理，可与多种持久层框架和Web
 MVC 框架整合。“一站式”选择，贯穿表现层，业务层，持久层；

2.1 低侵入设计，代码污染极低；

2.2 独立于各种应用服务器；

2.3 DI容器降低了业务对象之间的替换复杂性，提高了解耦；

2.4 AOP允许将一些通用任务如安全，事务，日志等进行集中处理，提供了更好的复用；

2.5 Spring的ORM和DAO提供了与第三方持久层的良好整合，简化数据访问；

2.6 高度开放性，可以选用部分；

**3.开始一个spring应用**

3.1 基于maven配置 ，自动下载spring框架到当前eclipse环境中；

3.2 从github上下载源码；

例子解析：配置框架依赖，将框架包导入开发环境；

**类1：**

public class PersonService{

private String name;

public void setName(String name){

this.name=name;

}

public void info(){

System.out.println("name:"+name);

}

}

**配置文件bean.xml：**

<bean id="personService",class="">

<property name="name" value="LiHua"/>

</bean>

**类2：**

public class SpringTest{

public static void main(){

ApplicationContext ctx=new ClassPathXmlApplicationContext("bean.xml");

PersonService p=ctx.getBean("personService",PersonService.class);

p.info();

}

}




解析：ApplicationContext 实例就是Spring容器，spring的核心，他是一个巨大的工厂。Spring容器根据配置文件实例化类PersonService，并为该类设置属性值。这种设置属性值的方式称为控制反转IOC。

**4.理解依赖注入：设值注入和构造注入**

0.理解依赖注入：三种实例化类方式

【a】.调用者自己创建被调用者，通过new关键字调用构造器创建一个对象；可扩展性差，各组件职责不清；

【b】.调用者定位到工厂，无需管理被调用者的具体实现，此时调用的代码面向接口编程，解耦调用者和被调用者，但是调用者和被调用者耦合在一起；

【c】.无需定位被调用者和工厂，实例之间的依赖关系由IOC容器管理；

依赖注入是指程序运行过程中，需要另外一个对象协作（调用它的方法，访问他的属性），无需再代码中创建被调用者，而是依赖外部容器注入。

设值注入：IOC容器使用属性的setter方法来注入被依赖的实例；

构造注入：IOC容易使用构造器来注入被依赖的实例；

4.1 设值注入：spring面向接口编程：提倡不管是ＤＡＯ组件，还是业务逻辑组件先定义接口在实现功能；

接口1：

public interface Person{

public void useAxe()；

}

接口2：

public interface Axe(){

public String chop();

}

实现类１：

public class StoneAxe implements Axe {

public String chop(){

return "石斧砍柴慢";}

}

实现类2：POJO即可，不需要一个标准的JavaBean

public class Chinese implement Person{

private Axe axe;

public void setAxe(Axe axe){

this axe=axe;}

}

public void useAxe(){

System.out.println(axe.chop());

}

}

配置文件：

<bean id="chinese" class="">

<property name="axe" ref="stoneAxe"/>

</bean>

<bean id="stoneAxe" class="">

</bean>

测试程序：

Main(String[] args){

ApplicationContext ctx=new ClassPathXmlApplicationContext("bean.xml");

Person p=ctx.getBean("chinese",Person.class);

p.useAxe();

}

解析：person实例无需了解Axe的具体实现，也不需要了解Axe创建过程；




4.2 构造注入：

//将setter改为构造函数

public class Chinese implement Person{

private Axe axe;

public Chinese
(Axe axe){//将set改为构造函数

this axe=axe;}

}

public void useAxe(){

System.out.println(axe.chop());

}

}

//修改配置文件：

<bean id="chinese" class="">

<constructor-arg ref="stoneAxe">

</bean>

<bean id="stoneAxe" class="">

</bean>

与设值注入的**区别**：

设置注入先通过无参构造器创建一个Bean实例，然后调用对应的setter方法注入依赖；而构造函数注入则直接调用有参构造函器，当Bean实例创建完成后，已经完成了依赖关注的注入；

两者的适用场景：

设置注入：

1. 直观，自然；

2. 复杂依赖导致构造注入过于臃肿，性能下降；

3. 某些属性可选时，多参数的构造器更加笨重；

构造注入：

1. 代码中清晰的决定注入的顺序；

2.对于依赖关系无需变化的Bean，构造注入更有用处，不用担心后面的代码对已有依赖关系破坏；

3.只有组件的创建者才能改变组件，组件的调用者组件内部的依赖关系完全透明，更符号高内聚的原则；




**5.使用spring：**

1.两个核心基本的接口：BeanFactory和它的子接口ApplicationContext(也称为spring上下文)，都
 代表spring容器；

spring管理所有组件当做Bean管理，实例化Bean和管理Bean的生命周期；java面向接口编程，不关系具体的实现，所以spring的配置文件需要明确接口对应的实现类；

spring容器负责配置，创建，管理Bean以及Bean之间的依赖关系；

接口ApplicationContext的实现类：FileSystemXmlApplicationContext 和ClassPathXmlApplicationContext

配置文件采用XML Schema或者DTD来定义；

ApplicationContext增强了BeanFactory,一般都用ApplicationContext，除非**内存**很紧张才考虑BeanFactory；

相比BeanFactory，ApplicationContext多的功能：

继承MessageSource，提供国际化支持；

资源访问，URL和文件；

事件机制；

载入多个配置文件；

以声明的方式启动并创建Spring容器；

利用如ContextLoader的支持类，在web应用的启动时候自动创建。也可以编程方式创建。

5.1 ApplicationContext的国际化支持：

继承MessageSource接口，该接口包含三个getMessage()方法;

配置xml;调用getMessage()方法；

5.2 ApplicationContext的事件机制：观察者设计模式

定义个Event 继承 ApplicationEvent;

NotFilter 实现ApplicationListener;

配置NotFilter到xml；

可以主动触发容器事件；ctx.publishEvent(event);

5.3 让bean获取spring容器：实现ApplicationContextAware接口的setApplication()方法;

**6.Spring中的Bean**

开发者：开发Bean，配置Bean；

spring框架：根据配置文件创建Bean，调用Bean的实例方法完成依赖注入；

通常情况下spring会直接使用new根据class实例化Bean；spring使用工厂方法实例化就不需要class属性；

6.1 Bean的配置

基本定义：

id class value type

lazy-init="true"

<alias name="person" alias="jack">

Bean的作用域：5种。

scope=singleton,protoype,request,session,globalSession;

**6.2 配置依赖**：调用关系==依赖关系 底层是反射

设置注入：<property... />

构造注入：<constructor-arg... />

+ value：配置基本类型，字符串，日期；

+ ref：注入另一个bean的引用；

+ bean：

+ list,set,map,props;

配置合作者Bean：ref=

自动装配bean：<beans/>中的default-autowire,<bean/>中的autowire;

1） 注入嵌套bean：某个bean所依赖的bean不想被spring容器直接访问，可以使用嵌套bean；嵌套bean只对嵌套他的外部bean有效；

<bean id="chinese" class="...">

<property name="axe">

<bean class="...">

</property>

</bean>

2） 注入集合值：

<bean id="chinese" class="...">

<!--
map属性 -->

<property name="score">

<map>

<entry
key="语文" value="90">

<entry key="shuxue" value="95">

</map>

</property>

</bean>

List和数组属性：

<list>

<value>value1</value>

</list>

Properties属性：

<props>

<prop key="血压">正常</prop>

</props>

Set集合：

<set>

<value>value1</value>

</set>

集合元素可以是基本类型或者引用类型。

容器的继承：集合合并，子类覆盖父类相同的。

<bean id="child"
parent="parent">

<props>

<prop key="血压">正常</prop>

</props>

</bean>

组合属性名称：

<bean id="" class="">

<property name="foo.bar.x.y" value="yyy" />

</bean>

相当于：a.getFoo().getBar().getX().setY("yyy");




7.spring提供xml配置，也提供java配置管理；

@Configuration

@Bean

@Value

8.Bean实例的创建方法：

8.1 构造函数；

大多数情况，BeanFactory直接通过new关键字根据class指定的实现类，调用构造函数创建Bean实例；而使用实例工厂方法创建或者子Bean方法时，不需要指定class属性；

8.2 静态工厂；

<bean id="dog"
class="Factory" factory-method="getBeing">

<constructor-arg value="dog"/>

<property name="msg" value="狗" />

</bean>

<bean id="cat" class="Factory" factory-method="getBeing">

<constructor-arg value="cat"/>

<property name="msg" value="猫" />

</bean>

8.3 实例工厂方法；

<bean id="personFactory"
class="personFactory">

</bean>

<bean id="chinese" factory-bean="personFactory"> //personFactory为实例工厂

<constructor-arg value="chin"/>

</bean>

9.深入理解容器中的Bean：抽象Bean，子Bean，工厂Bean；

抽象bean：abstract="true",不能被实例化，价值在于继承；Bean模板；

子bean：有一批bean配置的大量配置信息相同。子bean从父bean继承实现类，构造参数，属性值等；

工厂bean：实现FactoryBean接口的一种特殊的bean，对该bean ID请求时候，返回的是FactoryBean的产品，&id返回本身。

-----------------------------------

bean继承和java类继承的区别：

1.bean继承是实例与实例之间的参数的延续；java类继承是一般到特殊的细化，是方法，属性的延续；

2.Spring中子bean和父类bean可以是不同类型，java中继承子类是一种特许的父类；

3.spring中子bean不能作为父bean使用，不具备多态；

---------------------------------------

获得bean本身的id:获得bean后如何获得部署该bean时注定的ID。

实现接口BeanNameAware.Spring框架在bean初始化后，回调setBeanName().

强制初始化bean：

<bean id="beanOne" class="..."
depends-on="manager">

<property name="manager" ref="manager"/>

</bean>

<bean id="manager" class="..."/>

10.Bean的生命周期

singleton作用域：spring管理其整个生命周期：创建，初始化，销毁；

prototype作用域：仅仅负责创建，创建后交给客户端代码管理；

管理Bean生命周期的两个时机：注入依赖关系之后，销毁之前；

注入依赖之后：

1.实现一个接口：implement InitializingBean,处理代码写在afterPropertiesSet()方法中；

2.写一个任意方法int(),配置文件配置init-method="init"；

销毁之前：

1.实现一个接口：implement DisposableBean,处理代码写在destroy()方法中；

2.写一个任意方法close(),配置文件配置init-method="close"；

协调作用域不同的bean：

singleton依赖singleton,prototype依赖singleton；通过属性定义依赖关系即可；

singleton依赖prototype，会产生依赖得不到及时的更新，解决办法：

部分放弃依赖注入或者利用方法注入。

方法注入：单例A中依赖多例B，A设置成抽象类，其中包含一个抽象方法，public abstrack axe getAxe();

配置文件：

<bean id="B" class="..." scope="prototype" />

<bean id="A" class="..." >

<lookup-method name="getAxe" bean="steelAxe" />

</bean>

11.深入理解依赖关系配置；

spring在xml中的配置相当于让spring执行相应的代码：

11.1 <bean.../> ------------ 执行无参构造器或者有参构造器；

11.2 <property.../> --------- 执行一次setter方法；

11.3 调用getter方法：PropertyPathFactoryBean;------------注入其他bean的属性值；

11.4 访问类或者对象的Field值：MethodInvokingFactoryBean;-----------注入其他bean的Field值；

11.5 调用普通方法：MethodInvokingFactoryBean;--------------------注入其他方法的返回值；




12.基于xml Schema的简化配置;

基于DTD配置文件；

基于xml schema配置；

简化配置。

13.SpEL：

简化配置，扩展容器功能；](https://so.csdn.net/so/search/s.do?q=控制反转IOC&t=blog)](https://so.csdn.net/so/search/s.do?q=依赖注入DI&t=blog)](https://so.csdn.net/so/search/s.do?q=spring中的DI&t=blog)](https://so.csdn.net/so/search/s.do?q=spring基本用法&t=blog)




