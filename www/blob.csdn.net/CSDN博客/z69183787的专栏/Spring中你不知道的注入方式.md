# Spring中你不知道的注入方式 - z69183787的专栏 - CSDN博客
2016年10月25日 18:43:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：354
个人分类：[Spring-注入&注解&扫描](https://blog.csdn.net/z69183787/article/category/7066579)
摘要: 在实际的应用中，某个实例的属性值可以是某个方法的返回值，或者类的Field值，或者属性值，Spring同样支持这种非常规的注入方式。本文将逐一介绍。部分内容来自书籍。
前言
    在Spring配置文件中使用XML文件进行配置，实际上是让Spring执行了相应的代码，例如：
- 
使用<bean>元素，实际上是让Spring执行无参或有参构造器
- 
使用<property>元素，实际上是让Spring执行一次setter方法
    但Java程序还可能有其他类型的语句：调用getter方法、调用普通方法、访问类或对象的Field等，而Spring也为这种语句提供了对应的配置语法：
- 
调用getter方法：使用PropertyPathFactoryBean
- 
调用类或对象的Filed值：使用FiledRetrievingFactoryBean
- 
调用普通方法：使用MethodInvokingFactoryBean
注入其他Bean的属性值
    PropertyPathFactoryBean用来获得目标Bean的属性值（实际上就是调用getter方法返回的值），获得的值可以注入给其他的Bean，也可以直接定义新的Bean。看如下的配置文件：
```xml
<bean id="person" class="com.abc.Person">
    <property name="age" value="30" />
    <property name="son">
        <!-- 使用嵌套Bean定义属性值 -->
        <bean class="com.abc.service.Son">
            <property name="age" value="11" />
        </bean>
    </property>
</bean>
<bean id="son2" class="com.abc.service.Son">
    <!-- age属性不是直接注入，而是将person中的son的age属性赋值给son2的age属性 -->
    <property name="age">
        <!-- 注意这里使用的是PropertyPathFactoryBean -->
        <bean id="person.son.age" 
            class="org.springframework.beans.factory.config.PropertyPathFactoryBean" />
    </property>
</bean>
```
    其中Person类和Son类的属性可以从配置文件中看出，这不再给出。主程序如下：
```
public class Test {
    public static void main(String args[]) {
        ApplicationContext ac = 
            new ClassPathXmlApplicationContext("applicationContext.xml");
        System.out.println("age=" + ac.getBean("son2", Son.class).getAge());
    }
}
```
    输出结果：
`age=11`
    Bean实例的属性值，不仅可以注入另一个Bean，还可将Bean实例的属性值直接定义成Bean实例，这也是通过PropertyPathFactoryBean完成的。对上面的配置文件增加这样一段：
```xml
<bean id="son1" 
    class="org.springframework.beans.factory.config.PropertyPathFactoryBean">
    <!-- 确定目标Bean，表明son1来自哪个Bean的组件 -->
    <property name="targetBeanName" value="person" />
    <!-- 确定属性，表明son1来自目标Bean的哪个属性 -->
    <property name="propertyPath" value="son" />
</bean>
```
    执行上面的Test类，把son2换成son1，结果一样。
注入其他Bean的Field值
    通过FieldRetrievingFactoryBean类，可以将其他Bean的Field值注入给其他Bean，或者直接定义新的Bean。下面是配置片段：
```java
```java
<bean id="son" class="com.abc.service.Son">
    <property name="age">
        <bean id="java.sql.Connection.TRANSACTION_SERIALIZABLE"
            class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean" />
    </property>
</bean>
```
```
    测试主程序与上文定义的类似，这里不再提供，执行结果如下：
`age=8`
    在这个配置中，son对象的age的值，等于java.sql.Connection.TRANSACTION_SERIALIZABLE的值。在上面的定义中，定义FieldRetrievingFactoryBean工厂Bean时，指定的id并不是该Bean实例的唯一标识，而是指定Field的表达式（即将要被取出来的值）。
    注意：Field既可以是静态的，也可以是非静态的。上面的配置片段指定的Field表达式是静态Field值，因此可以通过类名直接访问。如果Field值是非静态的，则应该通过容器中已经存在的Bean来访问——即Field表达式的第一个短语应该是容器中已经存在的Bean。
    Field值也可以定义成Bean实例，例如，在配置文件中增加下面一段：
```xml
<bean id="age" 
    class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean">
    <!-- targetClass指定Field所在的目标类 -->
    <property name="targetClass" value="java.sql.Connection" />
    <!-- targetField指定Field名 -->
    <property name="targetField" value="TRANSACTION_SERIALIZABLE" />
</bean>
```
    在主程序中增加如下输出：
`System.out.println("age=" + ac.getBean("age"));`
    执行结果和上文一样。
    使用FieldRetrievingFactoryBean获取Field值时，必须指定如下两个属性：
- 
targetClass或targetObject：分别用于指定Field值所在的目标类或目标对象。如果需要获得的Field是静态的，则使用targetClass指定目标类；如果Field是非静态的，则使用targetObject指定目标对象
- 
targetField：指定目标类或目标对象的Field名
    如果Field是个静态Field，则有一种更加简洁的写法：
```java
```java
<bean id="age" 
    class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean">
    <!-- value指定哪个类的哪个静态域值 -->
    <property name="staticField" value="java.sql.Connection.TRANSACTION_SERIALIZABLE" />
</bean>
```
```
注入其他Bean的方法返回值
    通过MethodInvokingFactoryBean工厂Bean，可将目标方法的返回值注入为Bean的属性值。这个工厂Bean用来获取指定方法的返回值，该方法既可以是静态方法，也可以是实例方法；这个值既可以被注入到指定Bean实例的指定属性，也可以直接定义成Bean实例。看例子：
```xml
<bean id="valueGenerator" class="com.abc.util.ValueGenerator" />
<bean id="son1" class="com.abc.service.Son">
    <property name="age">
        <!-- 获取方法返回值：调用valueGenerator的getValue方法 -->
        <bean 
            class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
            <property name="targetObject" ref="valueGenerator" />
            <property name="targetMethod" value="getValue" />
        </bean>
    </property>
</bean>
```
    下面是ValueGenerator：
```java
```java
public class ValueGenerator {
    public int getValue() { return 2; }
    public static int getStaticValue () { return 3;}
}
```
```
    测试程序依旧打印son1中age的值，代码略，结果如下：
`age=2`
    如果要调用静态方法，则把配置修改为：
```java
```java
<bean id="son1" class="com.abc.service.Son">
    <property name="age">
        <!-- 获取方法返回值：调用valueGenerator的getStaticValue方法 -->
        <bean 
            class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
            <property name="targetClass" value="com.abc.util.ValueGenerator" />
            <property name="targetMethod" value="getStaticValue" />
        </bean>
    </property>
</bean>
```
```
    测试结果为：
`age=3`
    由于Java是支持重载的，只给定方法名，还不足以能够确定调用哪个方法，通过上面的配置能调用成功是因为ValueGenerator中的两个方法都没有参数。如果方法中有参数，该如何配置呢？在配置文件中加入以下内容：
```xml
<bean id="sysProps" 
    class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
    <property name="targetClass" value="java.lang.System" />
    <property name="targetMethod" value="getProperties" />
<bean>
<bean id="javaVersion" class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
    <!-- 指向上面的sysProps Bean -->
    <property name="targetObject" value="sysProps" />
    <property name="targetMethod" value="getProperty" />
    <!-- 这里配置参数 -->
    <property name="arguments">
        <!-- 使用list元素列出调用方法的多个参数 -->
        <list>
            <value>java.version</value>
        </list>
    </property>
<bean>
```
    上例中相当于用"java.version"作为参数调用了java.lang.System的getProperty方法，返回值将创建一个名为javaVersion的Bean。即相当于：
`javaVersion = java.lang.System.getProperty("java.version");`
    和前文中的Field一样，如果要调用的方法为静态方法，也有一种更加简洁的方法：
```java
```java
<bean id="myBean"
    class="org.springframework.beans.factory.config.MethodInvokingFactoryBean">
    <!-- 使用staticMethod属性，直接指定目标类的目标方法 -->
    <property name="staticMethod" value="com.abc.util.ValueGenerator.getStaticValue" />
</bean>
```
```
**枚举：**
**[java]**[view
 plain](http://blog.csdn.net/yingkongshi99/article/details/22991931#)[copy](http://blog.csdn.net/yingkongshi99/article/details/22991931#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/276713/fork)
- //定义枚举类型
- publicenum ReportType {  
-     MONTH,WEEK,DAY  
- }  
- 
- //使用枚举类型
- publicclass ReportJob {  
- private ReportType reportType;  
- }  
- 
- //spring配置文件注入
- <bean id="DAY"class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean">    
-         <property name="staticField" value="com.test.ReportType.DAY" />    
-     </bean>  
- <bean id="dayReportJob"class="com.test.ReportJob">  
-         <property name="reportType" ref="DAY" />  
-     </bean>  
注意：
枚举类型要想注入到类中，一定要先使用org.springframework.beans.factory.config.FieldRetrievingFactoryBean类将枚举类型进行转换，即
**[java]**[view
 plain](http://blog.csdn.net/yingkongshi99/article/details/22991931#)[copy](http://blog.csdn.net/yingkongshi99/article/details/22991931#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/276713/fork)
- <bean id="DAY"class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean">    
-         <property name="staticField" value="com.test.ReportType.DAY" />    
-     </bean>  
将ReportType.Day转换为DAY这个bean，然后在要注入的bean中使用<property name="reportType" ref="DAY" />引用即可。
