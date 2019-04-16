# Spring第二课：ioc简介和初步使用 - PeterBishop - CSDN博客





2019年01月30日 09:22:10[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：41
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课开始来学习Spring核心技术的第一个特性，依赖注入(或者说控制反转)



  先来说下缩写，控制反转的缩写叫IoC,依赖注入的缩写叫DI



  然后，Spring官网看IoC



  可以看到核心技术篇的第一章就是IoC，全称叫The IoC Container，也就是IoC容器



 IoC也称为依赖注入（DI）。这是一个过程，通过这个过程，对象只能通过构造函数参数，工厂方法的参数或在构造或从工厂方法返回后在对象实例上设置的属性来定义它们的依赖关系（即，它们使用的其他对象）。 。然后容器在创建bean时注入这些依赖项。此过程基本上是bean本身的逆（因此名称，控制反转），通过使用类的直接构造或诸如服务定位器模式的机制来控制其依赖关系的实例化或位置。



  上面是官方的解释，我来说人话。



  控制反转的意思就是

从

我什么时候需要我就什么时候主动new一个对象(Spring里称之为bean)

反转为

我什么时候需要我就直接找IoC容器要一个。



 因此，使用IoC容器时必须预先定义好我们以后可能需要用到的bean，并且将这些bean放入IoC容器中交由IoC容器来管理。



接下来，开始学习如何使用IoC。



与IoC有关的jar包不多，就下面这几个



全部添加进来就行。



下面开始教怎么用
- 如何获取IoC容器

该org.springframework.context.ApplicationContext接口代表Spring IoC容器，负责实例化，配置和组装bean。

因此我们要用ApplicationContext接口来获取IoC容器。


- 如何配置bean

看官方文档可以看到可以用xml文件来做

定义bean要用<bean />标签，而所有的<bean/>标签必须被包含在<beans/>标签中



一下是官方文档给出的配置文件样例:

用图片没法复制，我还是直接粘贴文本


<?xml version="1.0" encoding="UTF-8"?><beans` `xmlns`=`"http://www.springframework.org/schema/beans"`    `xmlns:xsi`=`"http://www.w3.org/2001/XMLSchema-instance"`    `xsi:schemaLocation`=`"http://www.springframework.org/schema/beanshttp://www.springframework.org/schema/beans/spring-beans.xsd">`    `<bean` `id`=`"..."` `class`=`"...">`   ``        `*<!-- collaborators and configuration for this bean go here -->*`    `</bean>`    `<bean` `id`=`"..."` `class`=`"...">`        `*<!-- collaborators and configuration for this bean go here -->*`    `</bean>`    `*<!-- more bean definitions go here -->*</beans>


该`id`属性是一个标识单个bean定义的字符串。

该`class`属性定义bean的类型并使用完全限定的类名。


- 实例化容器
`ApplicationContext context = `**new**` ClassPathXmlApplicationContext(`"services.xml"`, `"daos.xml"`);`- 获取bean
*// create and configure beans*`ApplicationContext context = `**new**` ClassPathXmlApplicationContext(`"services.xml"`, `"daos.xml"`);`*// retrieve configured instance*`PetStoreService service = context.getBean(`"petStore"`, PetStoreService.class);`
显然是使用getBean()方法



Demo



Student.java
package com.ioc.bean;



//学生类

public class Student {

    private int id;

    private String name;

    private int age;



    public Student() {

    }



    public Student(int id, String name, int age) {

        this.id = id;

        this.name = name;

        this.age = age;

    }



    public int getId() {

        return id;

    }



    public void setId(int id) {

        this.id = id;

    }



    public String getName() {

        return name;

    }



    public void setName(String name) {

        this.name = name;

    }



    public int getAge() {

        return age;

    }



    public void setAge(int age) {

        this.age = age;

    }



    @Override

    public String toString() {

        return "Student{" +

                "id=" + id +

                ", name='" + name + '\'' +

                ", age=" + age +

                '}';

    }

}


bean01.xml
<?xml version="1.0" encoding="UTF-8" ?>

<!--

    定义bean要用<bean />标签，而所有的<bean/>标签必须被包含在<beans/>标签中

-->

<beans xmlns="http://www.springframework.org/schema/beans"

       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

       xsi:schemaLocation="http://www.springframework.org/schema/beans

        http://www.springframework.org/schema/beans/spring-beans.xsd">

    <!--

        bean的名字可以用id或者name属性来设置，但我们一般用id

        注意:

        1. bean的id属性必须唯一，不允许多个bean的id相同

        2. id属性并不是必须的

    -->

    <!--

        class属性定义bean的类型并使用完全限定的类名。

    -->

    <bean id="student" class="com.ioc.bean.Student"></bean>

</beans>




IoCTeach01.java
package com.ioc.main;



import com.ioc.bean.Student;

import org.junit.Test;

import org.springframework.context.ApplicationContext;

import org.springframework.context.support.ClassPathXmlApplicationContext;



public class IoCTeach01 {





    @Test

    public void testApplicationContext()

    {

        //路径规则就是以类路径为相对路径

        //注意，类路径是out/production/SpringTeach，不是src,只不过二者目录结构相同而已

        ApplicationContext context = new ClassPathXmlApplicationContext("com/ioc/config/bean01.xml");

        //OK，这样我们就获取到IoC容器了



        //可以看到getBean()有四个重载方法

        */**         * Object getBean(String name)         * **根据bean的name获取bean，这个name其实就是xml里配置的id属性，返回值类型是Object，因此使用的时候要转为具体类还需要强转一下         *         * T getBean(Class**<T> **requiredType)         * **根据bean的类型获取bean，返回值类型是传入的类型         *         * T getBean(String name, Class**<T> **requiredType)         * **根据bean的name和type获取bean，返回值同样是传入的类型         *         * Object getBean(String name, Object... args)         * 返回指定bean的实例，该实例可以是共享的，也可以是独立的。         *允许指定显式构造函数参数/工厂方法参数，重写bean定义中指定的默认参数（如果有）。         * 基本不会用这个         *         * T getBean(Class**<T>**requiredType, Object... args)         * **基本同上，也基本不会用这个         */        *//根据name

        Student student = (Student) context.getBean("student");

        System.*out*.println(student.toString());



        //根据class

        student = context.getBean(Student.class);

        System.*out*.println(student.toString());



        //根据name和class

        student = context.getBean("student",Student.class);

        System.*out*.println(student);

    }

}




