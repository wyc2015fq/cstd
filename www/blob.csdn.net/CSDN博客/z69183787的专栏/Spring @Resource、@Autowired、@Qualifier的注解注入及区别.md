# Spring @Resource、@Autowired、@Qualifier的注解注入及区别 - z69183787的专栏 - CSDN博客
2017年01月11日 21:58:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4064
spring2.5提供了基于注解（Annotation-based）的配置，我们可以通过注解的方式来完成注入依赖。在[Java](http://lib.csdn.net/base/javase)代码中可以使用
 @Resource或者@Autowired注解方式来经行注入。虽然@Resource和@Autowired都可以来完成注入依赖，但它们之间是有区 别的。首先来看一下：
 a。@Resource默认是按照名称来装配注入的，只有当找不到与名称匹配的bean才会按照类型来装配注入；
 b。@Autowired默认是按照类型装配注入的，如果想按照名称来转配注入，则需要结合@Qualifier一起使用；
 c。@Resource注解是又J2EE提供，而@Autowired是由[spring](http://lib.csdn.net/base/javaee)提供，故减少系统对spring的依赖建议使用  
       @Resource的方式；
d。 @Resource和@Autowired都可以书写标注在字段或者该字段的setter方法之上
2、使用注解的方式，我们需要修改spring配置文件的头信息，修改部分红色标注，如下
   <?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
*xmlns:context="http://www.springframework.org/schema/context"*
       xsi:schemaLocation="http://www.springframework.org/schema/beans 
http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-2.5.xsd">
*<context:annotation-config/>*
</beans>
3、修改以上配置文件的头信息后，我们就可以在Java代码通过注解方式来注入bean，看下面代码
（1）@Resource
public class StudentService3 implements IStudentService {
//@Resource(name="studentDao")放在此处也是可行的
    private IStudentDao studentDao;
private String id;
public void setId(String id) {
this.id = id;
}
 @Resource(name="studentDao") // 通过此注解完成从spring配置文件中查找名称为studentDao的bean来装配字段studentDao，如果spring配置文件中不存在 studentDao名称的bean则转向按照bean类型经行查找
 public void setStudentDao(IStudentDao studentDao) {
this.studentDao = studentDao;
}
public void saveStudent() {
studentDao.saveStudent();
System.out.print(",ID 为："+id);
}
}
配置文件添加如下信息
  <bean id="studentDao" class="com.wch.dao.impl.StudentDao"></bean>
<bean id="studentService3" class="com.wch.service.impl.StudentService3" />
(2)@Autowired
public class StudentService3 implements IStudentService {
//@Autowired放在此处也是可行的
    private IStudentDao studentDao;
private String id;
public void setId(String id) {
this.id = id;
}
 @Autowired//通过此注解完成从spring配置文件中 查找满足studentDao类型的bean
//@Qualifier("studentDao")则按照名称经行来查找转配的
 public void setStudentDao(IStudentDao studentDao) {
this.studentDao = studentDao;
}
public void saveStudent() {
studentDao.saveStudent();
System.out.print(",ID 为："+id);
}
}
配置文件添加如下信息
  <bean id="studentDao" class="com.wch.dao.impl.StudentDao"></bean>
<bean id="studentService3" class="com.wch.service.impl.StudentService3" />
在java代码中可以使用@Autowire或者@Resource注解方式进行装配，这两个注解的区别是：
@Autowire 默认按照类型装配，默认情况下它要求依赖对象必须存在如果允许为null，可以设置它required属性为false，如果我们想使用按照名称装配，可 以结合@Qualifier注解一起使用;
@Resource默认按照名称装配，当找不到与名称匹配的bean才会按照类型装配，可以通过name属性指定，如果没有指定name属 性，当注解标注在字段上，即默认取字段的名称**（如A aaaa，将取aaaa为bean name）**作为bean名称寻找依赖对象，当注解标注在属性的setter方法上，即默认取属性名作为bean名称寻找 依赖对象.
注意：如果没有指定name属性，并且按照默认的名称仍然找不到依赖的对象时候，会回退到按照类型装配，但一旦指定了name属性，就只能按照名称 装配了.
-------------------------------------------
在java代码中使用@Autowired或@Resource注解方式进行装配，这两个注解的区别是：@Autowired 默认按类型装配，@Resource默认按名称装配，当找不到与名称匹配的bean才会按类型装配。
    @Autowired
    private PersonDao  personDao;//用于字段上
    @Autowired
    public void setOrderDao(OrderDao orderDao) {//用于属性的setter方法上
        this.orderDao = orderDao;
    }
@Autowired注解是按类型装配依赖对象，默认情况下它要求依赖对象必须存在，如果允许null值，可以设置它required属性为false。如果我们想使用按名称装配，可以结合@Qualifier注解一起使用。如下：
    @Autowired  @Qualifier("personDaoBean")
    private PersonDao  personDao;
@Resource注解和@Autowired一样，也可以标注在字段或属性的setter方法上，但它默认按名称装配。名称可以通过@Resource的name属性指定，如果没有指定name属性，当注解标注在字段上，即默认取字段的名称作为bean名称寻找依赖对象，当注解标注在属性的setter方法上，即默认取属性名作为bean名称寻找依赖对象。
    @Resource(name=“personDaoBean”)
    private PersonDao  personDao;//用于字段上
注意：如果没有指定name属性，并且按照默认的名称仍然找不到依赖对象时， @Resource注解会回退到按类型装配。但一旦指定了name属性，就只能按名称装配了。
步骤:
(1).修改beans.xml
Java代码 
在java代码中使用@Autowired或@Resource注解方式进行装配。但我们需要在xml配置文件中配置以下信息：  
<beans xmlns="http://www.springframework.org/schema/beans"  
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
       xmlns:context="http://www.springframework.org/schema/context"  
       xsi:schemaLocation="http://www.springframework.org/schema/beans  
           http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
           http://www.springframework.org/schema/context  
           http://www.springframework.org/schema/context/spring-context-2.5.xsd">  
          <context:annotation-config/>  
</beans>  
 这个配置隐式注册了多个对注释进行解析处理的处理器:AutowiredAnnotationBeanPostProcessor，CommonAnnotationBeanPostProcessor，PersistenceAnnotationBeanPostProcessor，RequiredAnnotationBeanPostProcessor
      注： @Resource注解在spring安装目录的lib\j2ee\common-annotations.jar
   <context:annotation-config/>  必不可少
