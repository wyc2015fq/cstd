# Spring第六课：bean的作用域 - PeterBishop - CSDN博客





2019年02月23日 18:01:41[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这课我们来讲Spring的IoC容器里bean的作用域，或者说bean的类型



  官方文档里的不太好理解，我来介绍。

  简单来说，就是我们在xml文件里配置bean的时候可以设置bean标签的scope属性来设置bean的作用域或者说是bean的类型，scope取值大体有以下几种


|值|说明|
|----|----|
|singleton|表示这个bean是单例|
|prototype|表示这个bean是多例|
|request|这个bean是多例，且每个request对象带有一个这个bean的实例|
|session|这个bean是多例，且每个session对象带有一个这个bean的实例|
|application|这个bean是单例，因为application对象(也就是ServletContext对象，java web基础里讲过)是单例|



我们日常开发用到的只有singleton和prototype，所以只详细讲解这两个


<!--

    singleton: 单例bean，这样的bean在IoC对象里只会被创建一次，且是在IoC容器初始化时就会被创建好，在之后的getBean()方法调用时获取的都是同一个对象

               如果不设置scope属性，默认bean是单例bean

-->

<bean id="singletonStudent" class="com.ioc.bean.Student" scope="singleton">

    <property name="id" value="1001"></property>

    <property name="age" value="20"></property>

    <property name="name" value="xiaoye"></property>

</bean>



<!--

    prototype: 多例bean,这样的bean并不会在IoC容器初始化时就被创建，而是每调用一次getBean()方法就会创建一个新的bean实例并返回

               所以每次getBean()获取到的多例bean都不是同一个对象

-->

<bean id="prototypeStudent" class="com.ioc.bean.Student" scope="prototype">

    <property name="id" value="1002"></property>

    <property name="age" value="20"></property>

    <property name="name" value="xiaoming"></property>

</bean>








测试代码:
package com.ioc.main;



import com.ioc.bean.Student;

import org.junit.Before;

import org.junit.Test;

import org.springframework.context.ApplicationContext;

import org.springframework.context.support.ClassPathXmlApplicationContext;



public class IoCTeachTest03 {



    ApplicationContext applicationContext;



    @Before

    public void init()

    {

        applicationContext = new ClassPathXmlApplicationContext("com/ioc/config/bean03.xml");

    }



    @Test

    public void testScopeSingleton()

    {

        Student singletonStudent1 = applicationContext.getBean("singletonStudent", Student.class);

        Student singletonStudent2 = applicationContext.getBean("singletonStudent", Student.class);



        System.*out*.println(singletonStudent1 == singletonStudent2);

        //由此可见singletonStudent1和singletonStudent2引用的同一个对象

    }



    @Test

    public void testScopePrototype()

    {

        Student prototypeStudent1 = applicationContext.getBean("prototypeStudent", Student.class);

        Student prototypeStudent2 = applicationContext.getBean("prototypeStudent", Student.class);



        System.*out*.println(prototypeStudent1 == prototypeStudent2);

        //由此可见prototypeStudent1和prototypeStudent2引用的是不同的对象

    }

}




