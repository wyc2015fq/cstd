# Spring基础：快速入门spring



# （1）：基础概念

2016年11月25日 07:51:37

 

liumiaocn

 

阅读数：7491

 

标签： [spring](http://so.csdn.net/so/search/s.do?q=spring&t=blog)[java](http://so.csdn.net/so/search/s.do?q=java&t=blog)[框架](http://so.csdn.net/so/search/s.do?q=%E6%A1%86%E6%9E%B6&t=blog)[基础](http://so.csdn.net/so/search/s.do?q=%E5%9F%BA%E7%A1%80&t=blog) 更多

个人分类： [spring](https://blog.csdn.net/liumiaocn/article/category/6497113)

所属专栏： [spring快速入门](https://blog.csdn.net/column/details/13907.html)



版权声明：本文为博主原创文章，未经博主允许欢迎转载，但请注明出处。	https://blog.csdn.net/liumiaocn/article/details/53330379

![这里写图片描述](https://img-blog.csdn.net/20161124173739872)

> 作为流行了10年以上的老将，spring依然精神矍铄，影响不减。本文将对spring很基础的概念进行介绍以及为学习spring最核心和基础的知识作环境搭建的准备。

# Spring官网

> <http://docs.spring.io/>

# 简介

> Spring为JAVA企业级应用提供了一个较为复杂框架流行框架。spring到底能做什么，或者说spring现在能做什么？

![这里写图片描述](https://img-blog.csdn.net/20161124173436603)

> 除了spring framework之外还有spring boot/spring data/spring cloud/… 快接近无所不包了。已经非常复杂和活跃。

![这里写图片描述](https://img-blog.csdn.net/20161124173607730)

# Spring framework

> 就像上面提到的那样，Spring有很多project（<http://spring.io/projects>）， 
> spring framework只是其中的一个（<http://projects.spring.io/spring-framework/>）。

# API & Reference

> 目前Current GA 是4.3.4，对应的API和Reference如下。作为第一参考资料，碰到问题的时候Reference -〉API的顺序进行查询。

| 项目      | 详细地址                                                     |
| --------- | ------------------------------------------------------------ |
| API       | <http://docs.spring.io/spring/docs/current/javadoc-api/>     |
| Reference | <http://docs.spring.io/spring/docs/current/spring-framework-reference/htmlsingle/> |

# 工具

> 工欲善其事，必先利其器。可以使用STS/NeatBean/IDEA直接下载来使用即可。

## STS

> spring官方推荐，对工具没有特别的喜好，可以使用STS。其本身就是spring在Eclipse上简单组装的。

| 项目 | 详细地址                         |
| ---- | -------------------------------- |
| STS  | <http://spring.io/tools/sts/all> |

## IDEA

IDEA是收费的软件，但是可以使用其社区版，基础功能都已经包含，是很多java程序员的最爱。

## Eclipse

> 很多人已经对Eclipse有了很多自定义的设定，对他们来说，spring只是很小的一部分。而实际项目中的开发更为如此，所以本文使用Eclipse中更新STS插件的方式进行安装。

### Eclipse 4.6 Neon

| 项目 | 详细地址                                                     |
| ---- | ------------------------------------------------------------ |
| Neon | <https://www.eclipse.org/downloads/download.php?file=/technology/epp/downloads/release/neon/1a/eclipse-jee-neon-1a-win32-x86_64.zip> |

### 更新方式

| 项目    | 详细地址                                                  |
| ------- | --------------------------------------------------------- |
| sts插件 | <http://dist.springsource.com/release/TOOLS/update/e4.6/> |

# Maven/Gradle

使用Maven或者Gradle管理依赖关系非常的方便，如果你还是习惯于使用ant，那也可以，但是ant不能直接提供依向Maven或者Gradle那样的依赖关系的自动管理的功能。比如4.3.4的简单地spring的依赖关系可以写成如下所示：

## Maven dependency

```
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>4.3.4.RELEASE</version>
    </dependency>
</dependencies>1234567
```

## Gradle Dependency

```
dependencies {
    compile 'org.springframework:spring-context:4.3.4.RELEASE'
}123
```

> 这些在Spring的Reference中都可轻松查询到（<https://projects.spring.io/spring-framework/>）

# IOC & DI

> IOC和DI基本上是一个意思，在spring最初引入的时候，把这种控制的方式称为IoC（Inversion of Control）。但是后来Martin Fowler等认为其实没有反转任何事情，其做的事情只是将注入了依赖关系所以称之为Dependency Injection，个人认为文字游戏的成份稍多。 
> A类的对象通过调用B类对象的方法来实现某一功能，在spring中是可以通过Injection的方式来实现的。作为入门者需要理解的是如果没有IOC应该怎么做呢，在Spring中被称为Dependency的可能就是一个简单的属性或者方法参数等等等，简单地来说，通过调用setter方法或者构造函数等等，这个就是IOC/DI方式。而通过只在Meta数据中描述实际的类的依赖关系，spring负责把这些拧在一起，同时负责管理其lifecycle，解放了不少的工作，提高了开发的效率。

# 环境准备

> 为了更加清楚，使用Eclipse直接倒入spring的库的方式。熟悉的可以直接跳过。

## 下载spring库

| 项目         | 详细地址                                                     |
| ------------ | ------------------------------------------------------------ |
| lib          | <http://repo.spring.io/release/org/springframework/spring/>  |
| 目前最新版本 | <http://repo.spring.io/release/org/springframework/spring/4.3.4.RELEASE/> |

## 准备步骤

Step 1：使用Eclipse创建一个Java项目 
Step 2：创建一个lib目录 
Step 3：将上面下载下来的spring的库文件全部拷贝到lib目录中 
Step 4：在项目上右键->属性后将lib文件夹下的文件全部加入







# （2）：理解Ioc

2016年11月25日 23:18:31

 

liumiaocn

 

阅读数：4004

 

标签： [spring](http://so.csdn.net/so/search/s.do?q=spring&t=blog)[java](http://so.csdn.net/so/search/s.do?q=java&t=blog)[实例](http://so.csdn.net/so/search/s.do?q=%E5%AE%9E%E4%BE%8B&t=blog)[Ioc](http://so.csdn.net/so/search/s.do?q=Ioc&t=blog) 更多

个人分类： [spring](https://liumiaocn.blog.csdn.net/article/category/6497113)

所属专栏： [spring快速入门](https://blog.csdn.net/column/details/13907.html)



 版权声明：本文为博主原创文章，未经博主允许欢迎转载，但请注明出处。	https://liumiaocn.blog.csdn.net/article/details/53343652

![这里写图片描述](https://img-blog.csdn.net/20161124173739872)

> 本文将使用简单的例子来说明spring的Ioc是如何实施的。

# 传统方式

> 首先我们要看一下不使用Ioc的时候得一个简单的例子。创建一个Teacher的类，此类有一个sayhello的函数， 
> 在另外一个类里面创建实例并调用它。

## 创建Package

> 在src目录上右击，创建Java package(com.liumiao.demo.spring)

## 创建一个名为Teacher的Class

```
package com.liumiao.demo.spring;

public class Teacher {

    public String sayhello(){
        return "Hello, I am a teacher";
    }
}
123456789
```

## 创建用于演示的TestDemo的Class

```
package com.liumiao.demo.spring;

public class TestDemo {

  public static void main(String[] args){
    Teacher person = new Teacher();

    System.out.println(person.sayhello());
  }
}12345678910
```

## 执行TestDemo

> 结果如下

```
Hello, I am a teacher1
```

# 传统方式的改进

> 一般的code使用最佳实践对代码进行优化，可能改成下面的样子

## 引入Person这个Interface

```
package com.liumiao.demo.spring;

public interface Person {
  public String sayhello();
}12345
```

## 改写Teacher类

```
package com.liumiao.demo.spring;

public class Teacher implements Person {
  @Override
  public String sayhello(){
    return "Hello, I am a teacher";
  }
}12345678
```

## 修改TestDemo

```
package com.liumiao.demo.spring;

public class TestDemo {

  public static void main(String[] args){
    Person person = new Teacher();

    System.out.println(person.sayhello());
  }
}12345678910
```

## 执行TestDemo

> 结果如下

```
Hello, I am a teacher1
```

> 结果没有变化。为什么要这样作呢，看起来只是绕圈实现而已，多做无用功的感觉。但其实把这个放大或者放到特定的上下文之中，其作用立即就会显现。举个简单的例子，我们想增加更多的子类的实现，比如实现一个Student的类，扩展或者替换，和其他设计模式进行结合都会非常方便，接下来来看看增加一个Student的实现需要做哪些事情。

## 增加一个Student的类

```
package com.liumiao.demo.spring;

public class Student implements Person {
  @Override
  public String sayhello(){
    return "Hello, I am a student.";
  }
}12345678
```

## 修改TestDemo

```
package com.liumiao.demo.spring;

public class TestDemo {

  public static void main(String[] args){
    Person person = new Student();

    System.out.println(person.sayhello());
  }
}12345678910
```

## 执行TestDemo

> 结果如下

```
Hello, I am a student.1
```

> 修改很少，OCP原则，开闭方便。source过于简单不便多说，可自行在项目中体会。虽然依然很简单有效，但是如果改变的话依然需要修改source，毕竟这里new Student这个操作被hardcoding了，接下来我们在这个基础之上来看看spring是如何做的。

# Spring方式

## 创建spring的配置文件

> 我们在com.liumiao.demo.spring下面创建一个spring的配置文件spring-cfg.xml,内容如下

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

</beans>123456
```

## 配置spring bean

> 接下来需要把一些bean设定到spring的配置文件中，我们先把Teacher这个bean设定进取，设定后的配置文件内容如下

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

  <bean id="thePerson" class="com.liumiao.demo.spring.Teacher">
  </bean>

</beans>123456789
```

## 使用

> 配置好了之后，spring中使用方式一般按照如下四步

| 步骤   | 内容                                   | 参考                           |
| ------ | -------------------------------------- | ------------------------------ |
| Step 1 | 装载spring配置文件                     | ClassPathXmlApplicationContext |
| Step 2 | 从返回的context中检索相应的spring bean | context.getBean                |
| Step 3 | 执行spring bean                        |                                |
| Step 4 | 关闭context                            | context.close                  |

## 改写TestDemo类

```
package com.liumiao.demo.spring;

import com.liumiao.demo.spring.Person;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class TestDemo {
    public TestDemo() {
    }

    public static void main(String[] args) {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("classpath:com/liumiao/demo/spring/spring-cfg.xml");
        Person person = (Person)context.getBean("thePerson", Person.class);
        System.out.println(person.sayhello());
        context.close();
    }
}12345678910111213141516
```

## 执行结果

```
Nov 24, 2016 8:41:31 PM org.springframework.context.support.ClassPathXmlApplicationContext prepareRefresh
情報: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@722c41f4: startup date [Fri Nov 24 20:41:30 CST 2016]; root of context hierarchy
Nov 24, 2016 8:41:31 PM org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
情報: Loading XML bean definitions from class path resource [com/liumiao/demo/spring/spring-cfg.xml]
Hello, I am a teacher
Nov 24, 2016 8:41:31 PM org.springframework.context.support.ClassPathXmlApplicationContext doClose
情報: Closing org.springframework.context.support.ClassPathXmlApplicationContext@722c41f4: startup date [Fri Nov 24 20:41:30 CST 2016]; root of context hierarchy1234567
```

## 使用Student类

> 从使用Teacher类变成使用Student类，在改善过了的传统方式需要修改少量代码，而使用Spring的话，则只需要向如下这样修改spring设定文件即可

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

  <bean id="thePerson" class="com.liumiao.demo.spring.Student">
  </bean>

</beans>123456789
```

## 执行结果

```
Nov 24, 2016 8:44:53 PM org.springframework.context.support.ClassPathXmlApplicationContext prepareRefresh
情報: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@722c41f4: startup date [Fri Nov 24 20:44:53 CST 2016]; root of context hierarchy
Nov 24, 2016 8:44:54 PM org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
情報: Loading XML bean definitions from class path resource [com/liumiao/demo/spring/spring-cfg.xml]
Nov 24, 2016 8:44:54 PM org.springframework.context.support.ClassPathXmlApplicationContext doClose
情報: Closing org.springframework.context.support.ClassPathXmlApplicationContext@722c41f4: startup date [Fri Nov 24 20:44:53 CST 2016]; root of context hierarchy
Hello, I am a student.1234567
```

> 这样就可以看到spring使用这种方式非常简单的通过修改设定文件来实现所需的功能的过程

```
备注：过程中如需用到commons-logging，下载地址为https://commons.apache.org/proper/commons-logging/download_logging.c
```







# （3）：构造注入

2016年11月26日 18:41:07

 

liumiaocn

 

阅读数：3465

更多

所属专栏： [spring快速入门](https://blog.csdn.net/column/details/13907.html)



 版权声明：本文为博主原创文章，未经博主允许欢迎转载，但请注明出处。	https://liumiaocn.blog.csdn.net/article/details/53353764

> spring可以使用构造注入/setter方法/java注解等，本文通过实例来学习一下构造注入的方法。

![这里写图片描述](https://img-blog.csdn.net/20161124173739872)

# 定义注入依赖的interface

```
package com.liumiao.demo.spring;

public interface TeachingService {
    public String provideTeachingService();
}12345
```

# 定义注入依赖的类

```
package com.liumiao.demo.spring;

public class SwimmingTeachingService implements TeachingService {
    @Override
    public String provideTeachingService(){
        return "I provide service of teaching swimming.";
        }
}12345678
```

# Person interface

```
package com.liumiao.demo.spring;

public interface Person {
    public String sayhello();
    public String provideTeachingService();
}123456
```

# 修改构造函数

```
package com.liumiao.demo.spring;

public class Teacher implements Person{
    private TeachingService service;
    public Teacher(TeachingService theService){
        this.service=theService;
    }
    public String sayhello(){
        return "Hello, I am a teacher";
    }
    @Override
    public String provideTeachingService(){
        return service.provideTeachingService();
    }
}123456789101112131415
```

# 配置spring设定文件

> 如下修改配置文件,唯一需要注意的是ref和id的一致

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="thePerson" class="com.liumiao.demo.spring.Teacher">
        <constructor-arg ref="theService"></constructor-arg>
    </bean>

    <bean id="theService" class="com.liumiao.demo.spring.SwimmingTeachingService">
    </bean>

</beans>12345678910111213
```

# 修改TestDemo

```
package com.liumiao.demo.spring;

import com.liumiao.demo.spring.Person;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class TestDemo {
    public TestDemo() {
    }

    public static void main(String[] args) {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("classpath:com/liumiao/demo/spring/spring-cfg.xml");
        Person person = (Person)context.getBean("thePerson", Person.class);
        System.out.println(person.sayhello());
        System.out.print(person.provideTeachingService());
        context.close();
    }
}
```

# 执行结果

```
十一月 26, 2016 6:39:09 下午 org.springframework.context.support.ClassPathXmlApplicationContext prepareRefresh
信息: Refreshing org.springframework.context.support.ClassPathXmlApplicationContext@25f38edc: startup date [Sat Nov 26 18:39:09 CST 2016]; root of context hierarchy
十一月 26, 2016 6:39:09 下午 org.springframework.beans.factory.xml.XmlBeanDefinitionReader loadBeanDefinitions
信息: Loading XML bean definitions from class path resource [com/liumiao/demo/spring/spring-cfg.xml]
十一月 26, 2016 6:39:09 下午 org.springframework.context.support.ClassPathXmlApplicationContext doClose
信息: Closing org.springframework.context.support.ClassPathXmlApplicationContext@25f38edc: startup date [Sat Nov 26 18:39:09 CST 2016]; root of context hierarchy
Hello, I am a teacher
I provide service of teaching swimming.
```



























