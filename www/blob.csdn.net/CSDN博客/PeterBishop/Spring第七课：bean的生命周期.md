# Spring第七课：bean的生命周期 - PeterBishop - CSDN博客





2019年02月23日 18:02:22[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：93
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这课我们来谈bean的生命周期。



  所谓生命周期，指的是从对象的创建直到被销毁的过程。一般而言，bean的生命周期包含以下几个过程:

创建->初始化(通过执行初始化方法来完成初始化)->执行各种操作->销毁(销毁前执行销毁方法)



创建已经讲过了，通过constructor-args或者property来注入属性，执行各种操作不需要讲，这节课要讲的就是初始化和销毁，也即设置初始化方法和销毁方法。


<!--

    指定初始化方法: 使用init-method属性指定具有void无参数的方法的名称

             注意: 初始化方法必须是返回值类型为void且无参数的方法

-->

<!--

       指定销毁方法: 使用destroy-method属性指定具有void无参数的方法的名称

              注意: 销毁方法必须是返回值类型为void且无参数的方法

   -->

<bean id="student" class="com.ioc.bean.Student" init-method="init" destroy-method="destroy">

    <property name="id" value="1001"></property>

    <property name="age" value="20"></property>

    <property name="name" value="小叶"></property>

</bean>





<bean id="student2" class="com.ioc.bean.Student" init-method="init" scope="prototype" destroy-method="destroy">

    <property name="id" value="1001"></property>

    <property name="age" value="20"></property>

    <property name="name" value="小明"></property>

</bean>


测试:
package com.ioc.main;



import com.ioc.bean.Student;

import org.junit.Before;

import org.junit.Test;

import org.springframework.context.ApplicationContext;

import org.springframework.context.support.ClassPathXmlApplicationContext;



public class IoCTeachTest04 {



    ApplicationContext applicationContext;



    @Before

    public void init()

    {

        applicationContext = new ClassPathXmlApplicationContext("com/ioc/config/bean04.xml");

    }



    @Test

    public void test()

    {

        //我们就什么都不写，看会发生什么

        //我们什么也没做，只是初始化了IoC容器，然后初始化IoC容器的时候单例bean也会跟着创建，

        // 而创建完单例bean后紧接着就会立即调用它的初始化方法，因此也就输出了"这是Student类的初始化方法"

        //我们可以用多例bean来验证一下

        applicationContext.getBean("student2", Student.class);

        applicationContext.getBean("student2", Student.class);

        applicationContext.getBean("student2", Student.class);

        applicationContext.getBean("student2", Student.class);



        //接着来测销毁方法

        //现在有个问题，对象什么时候销毁？程序结束？不，只需要销毁容器即可, ClassPathXmlApplicationContext调用registerShutdownHook()方法

        ((ClassPathXmlApplicationContext)applicationContext).registerShutdownHook();

        //只输出了小叶的销毁方法

        //由此可见，Spring的IoC容器并不负责销毁多例bean

    }



}




