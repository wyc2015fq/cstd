# spring abstract = “true” 抽象bean及继承（模板） - z69183787的专栏 - CSDN博客
2017年08月03日 17:00:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2436

## 问题描述
抽象类不能在java中实例化。但是春天说的是像`abstract="true"`这样的豆类创作。如果抽象类的状态仅由其子类实例初始化(我猜我是对的)，那么如果我需要在抽象类中定义的方法中使用该属性，那么…有可能它？我有一组代码如下：
```java
class abstract A { 
    private Something somethingObj; 
    // getters and setters are present.
    public void logSomething() { 
        try{ 
           //some code which throws exception 
        }
        catch(Exception e){ 
            somethingObj.logIt(e);// I have some logic inlogIt method. 
        } 
    }
}
```
## 最佳解决方法
Spring中的抽象bean与抽象类有些不同。事实上，Spring中的抽象bean甚至不必映射到任何类。以此为例：
```xml
<bean id="dao" abstract="true">
    <property name="dataSource" ref="dataSource"/>
    <property name="someHelper" ref="someHelper"/>
</bean>
<bean id="fooDao" class="FooDao" parent="dao">
    <property name="fooHelper" ref="fooHelper"/>
</bean>
<bean id="barDao" class="BarDao" parent="dao">
    <property name="barHelper" ref="barHelper"/>
</bean>
```
和班：
```php
public class FooDao {
    private DataSource dataSource;
    private SomeHelper someHelper;
    private FooHelper fooHelper;
    //setters
}
public class BarDao {
    private DataSource dataSource;
    private SomeHelper someHelper;
    private BarHelper barHelper;
    //setters
}
```
请注意，`FooDao`和`BarDao`没有任何父类(抽象或不抽象)基类。父抽象bean定义仅用于分组公共属性，因此避免了XML中的重复。
另一方面，引入`FooDao`和`BarDao`都能继承的抽象`Dao`类是一个好主意：
```php
public abstract Dao {
    protected DataSource dataSource;
    protected SomeHelper someHelper;
    //setters
}
public class FooDao extends Dao {
    private FooHelper fooHelper;
    //setters
}
public class BarDao extends Dao {
    private BarHelper barHelper;
    //setters
}
```
但是还是`dao` bean不必定义一个类。在多个具体的bean具有相同的依赖关系时，将抽象bean视为减少XML中的重复的一种方式。
## 参考文献
- [What is
 meant by abstract=“true” in spring?](https://gxnotes.com/link.php?target=https%3A//stackoverflow.com/questions/9397532/what-is-meant-by-abstract-true-in-spring%3Fanswertab%3Dvotes)
- 

我们在应用Spring时，在一般的设计时，肯定要用的抽象类。那在Spring中怎么样配置这些抽象Bean呢。请看下面：
如果两个bean 之间的配置信息非常相似，可利用继承来减少重复配置工作。
继承是指子bean 定义可从父bean 定义继承部分配置信息，也可覆盖特定的配置信
息，或者添加一些配置。使用继承配置可以节省很多的配置工作。在实际应用中，通用
配置会被配置成模板，可供子bean 继承。
**使用abstract 属性**
正如前面所介绍的，通用的配置会被配置成模板，而模板不需要实例化，仅仅作为子bean 定义的模板使用。而ApplicationContext 默认预初始化所有的singleton bean 。使用abstract 属性，可以阻止模板bean 被预初始化。
abstract 属性为true 的bean 称为抽象bean ，容器会忽略所有的抽象bean 定义，预初始化时不初始化抽象bean。
如果没有定义abstract 属性，该属性默认为false 。如下配置文件定义了一个抽象bean ，该抽象bean 作为模板使用:
public class SteelAxe implements Axe
{
> 
//count 是个状态值，每次执行chop 方法该值增加1
private int count = 0;
public SteelAxe(){
> System.out.println("Spring实例化依赖bean: SteelAxe 实例.. .");
}
//测试用方法
public String chop(){
> return "钢斧砍柴真快" + ++count;
}
}
public class Chinese implements Person
> 
//面向Axe 接口编程，而不是具体的实现类
private Axe axe;
//默认的构造器
public Chinese(){
> System.out.println("Spring实例化主调bean: Chinese 实例... ");
}
//设值注入所需的setter 方法
public void setAxe( Axe axe){
> System.out.pr工ntln (" Spring 执行依赖关系注入...");
this.axe = axe;
}
//实现Person 接口的useAxe 方法
public void useAxe(){
> System.out.println(axe.chop());
}
}
<?xml version="1.0" encoding="gb2312"?>
<!一指定Spring 配置文件的dtd>
<lDOCTYPE beans PUBL工C "-//SPRING//DTD BEAN//EN"
''http://www.springframework.org/dtd/spring-beans.dtd''>
<!一Spring 配置文件的根元素一〉
<beans>
<bean id="steelAxe" class="lee.SteelAxe"/>
<!… 通过abstract 属性定义该bean 是抽象bean-->
<bean id="chineseTemplate" class="lee.Chinese" abstract="true">
<!一定义依赖注入的属性一〉
<property name="axe">
<ref local="steelAxe"/>
</property>
</bean>
</beans>
从配置文件中可以看出，抽象bean 的定义与普通bean 的定义几乎没有区别，仅仅增加abstract 属性为true ，
但主程序执行结果却有显著的差别。下面的主程序采用AppliactionContext 作为Spring 容器，
. AppliationContext 默认预初始化所有的singleton bean。其主程序部分如下:
public class BeanTest
{
> 
public static void main(String[] args)throws Exception{
> ApplicationContext ctx = new FileSysternXmlApplicationContext("bean.xml");
}
}
//主程序部分仅仅实例化了ApplicationContext，在实例化ApplicationContext时，默认实例化singleton bean。
程序执行结果如下:
Spring 实例化依赖bean: SteelAxe 实例.. .
容器并没有实例化chineseTemplate bean ，而忽略了所有声明为abstract 的beano 如果取消abstract 属性定义，
则程序执行结果如下:
Spring 实例化依赖bean: SteelAxe 实~J...
Spring 实例化主调bean: Chinese 实例.. .
Spring 执行依赖关系注入...
可以看出，抽象bean 是一个bean 模板，容器会忽略抽象bean 定义，因而不会实例化抽象bean。
但抽象bean 无须实例化，因此可以没有class 属性。如下的配置文件也有效:
<?xml version="1.0" e口coding="gb2312"?>
<!一指定Spring 配置文件的dtd>
<!DOCTYPE beans PUBLIC "-/!SPRING//DTD BEAN//EN"
"http://www.springframework.org/dtd/spring-beans.dtd''>
<! -- Spring 配置文件的根元素-->
<beans>
<bean id="steelAxe" class="lee.SteelAxe"/>
<!一通过abstract 属性定义该bean 是抽象bean，抽象bean 没有指定class 属性一〉
<bean id="chineseTemplate" abstract="true">
<!… 定义依赖注入的属性一〉
<property name="axe">
<ref local="steelAxe"/>
</property>
</bean〉
</beans>
**注意:抽象bean 不能实例化，既不能通过getBean 获得抽象bean，也不能让其他bean 的ref 属性值指向抽象bean，因而只要企图实例化抽象bean，都将导致错误**。
**定义子bean**
我们把指定了parent 属性值的bean 称为子bean; parent 指向子bean 的模板，称为父bean 。
子bean 可以从父bean 继承实现类、构造器参数及属性值，也可以增加新的值。如果指定了init-method ， 
destroy-method 和factory-method 的属性，则它们会覆盖父bean的定义。子bean 无法从父bean 
继承如下属性: depends-on, autowire, dependency-check,singleton, lazy-init。这些属性将从子bean 定义中获得，
或采用默认值。通过设置parent 属性来定义子bean ， parent 属性值为父bean id。修改上面的配置文
件如下，增加了子bean 定义:
<?xml version="1.0" encoding="gb2312"?>
<!一指定Spring 配置文件的dtd>
<lDOCTYPE beans PUBL工C "-//SPRING//DTD BEAN//EN"
''http://www.springframework.org/dtd/spring-beans.dtd''>
<!-- Spring 配置文件的根元素一〉
<beans>
<bean id="steelAxe" class="lee.SteelAxe"/>
<!一通过abstract 属性定义该bean 是抽象bean-->
<bean id="chineseTemplate" class="lee.Chinese" abstract="true">
<!-- 定义依赖注入的属性一〉
<property name="axe">
<ref local="steelAxe"/>
</property>
</bean>
<!一通过parent 属性定义子bean ?
<bean id="chinese" parent="chineseTemplate"/>
</beans>
子bean 与普通bean 的定义并没有太大区别，仅仅增加了parent 属性。子bean 可以没有class 属性，
若父bean 定义中有class 属性，则子bean 定义中可省略其class 属性，但子bean 将采用与父bean 相同的实现类。
测试程序修改如下：
public class BeanTest
{
> 
public static void main(String[] args)throws Exception{
> ApplicationContext ctx = new FileSysternXmlApplicationContext("bean.xml");
Person p = (Person)ctx.getBean("chinese");
p.useAxe();
}
}
程序执行结果如下:
Spring 实例化依赖bean: Stee1Axe 实例.. .
Spring实例化主调bean: Chinese 实例.. .
spring 执行依赖关系注入...
钢斧砍柴真快
另外，子bean 从父bean 定义继承了实现类并依赖bean 。但子bean 也可覆盖父bean的定义，看如下的配置文件:
//Axe 的实现类StoneAxe如下:
public class StoneAxe implements Axe
> 
//默认构造器
public StoneAxe(){
> System.out.println("Spring实例化依赖bean: StoneAxe实例.. .");
}
//实现Axe 接口的chop 方法
public String chop(){
> return "石斧砍柴好慢";
}
}
Chinese子类如下：
public class Shanghai extends Chinese {
   public void show() {
                System.out.println("子Bean ,中国的上海");
    }
}
<?xm1 version="1.0" encoding="gb2312"?>
<! 指定Spring 配置文件的dtd>
<lDOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN"
''http://www.springframework.org/dtd/spring-beans.dtd''>
<! -- Spring 配置文件的根元素一〉
<beans>
<bean id="steelAxe" class="lee.SteelAxe"/>
<bean id="stoneAxe" class="lee.StoneAxe"/>
<!一通过abstract 属性定义该bean 是抽象bean-->
<bean id="chineseTemplate" class="lee.Chinese" abstract="true">
<property name="axe">
<ref local="steelAxe"/>
</property>
</bean>
<!一通过parent 属性定义子bean-->
<bean id="shanghai" parent="chineseTemplate">
<!一覆盖父bean 的依赖定义…〉
<property name="axe">
<ref local="stoneAxe"/>
</property>
</bean>
</beans>
此时，子bean 的依赖不再是父bean 定义的依赖了。**注意，这个时候的父类lee.Chinese 不能是抽象类，（说明下：有abstract="true"）不一定说明这个类一定是个抽象类，不是抽象类同样可以在Spring里定义为抽象Bean，如果你的Class是抽象类，那这个时候就不能用父Bean的Class，一定要在子Bean中定义Class来初始化这个子Bean）**
测试程序修改如下：
public class BeanTest
{
> 
public static void main(String[] args)throws Exception{
> ApplicationContext ctx = new FileSysternXmlApplicationContext("bean.xml");
Person p = (Person)ctx.getBean("shanghai");
p.useAxe();
}
}
按上面的测试程序执行结果如下:
Spring 实例化依赖bean: SteelAxe 实例.. .
spring 实例化依赖bean: StoneAxe 实例.. .
Spring 实例化主调bean: Chinese 实例.. .
Spring 执行依赖关系注入...
石斧砍柴好慢
**注意:上例中的子bean 定义都没有class 属性，因为父bean 定义中已有class 属性，子bean 的class 属性可从父bean 定义中继承，但需要注意的是从父Bean继承Class时，父Bean一定不能是抽象类，因为抽象类不能创建实例;如果父bean 定义中也没有指定class 属性，则子bean 定义中必须指定class 属性，否则会出错;如果父bean 定义指定了class 属性，子bean 定义也指定了class 属性，则子bean 将定义的class 属性覆盖父bean 定义的class属性。**
**-------------------------------------------------------------------------------------**
**---------------------------------------------------**
**Spring 中bean的继承和Java中的继承截然不同，前者是实例与实例之间的参数的延续，后者是一般到特殊的细化，前者是对象和对象之间的关系，后者是类和类之间的关系。**
  a.Spring中的子bean和父bean可以是不同的类型，但是Java中的继承，子类是一种特殊的父类；
  b.Spring中的bean的继承是实例之间的关系，主要表现在参数的延续，而Java中的继承是类与类之间的关系，主要体现在方法和属性的延续。
  c.Spring中子bean不可以作为父bean使用，不具备多态性，Java中的子类实例完全可以当作父类实例使用。
