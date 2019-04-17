# Spring第三课：依赖注入 - PeterBishop - CSDN博客





2019年02月07日 18:46:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：46
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来学依赖注入



依赖注入（DI）是一个过程，通过这个过程，对象只能通过构造函数参数，工厂方法的参数或在构造对象实例后在对象实例上设置的属性来定义它们的依赖关系（即，它们使用的其他对象）。从工厂方法返回。然后容器在创建bean时注入这些依赖项。这个过程基本上是bean本身的反向（因此名称，控制反转），它通过使用类的直接构造或服务定位器模式来控制其依赖项的实例化或位置。



说白了就是通过xml文件为bean对象的属性赋值



依赖注入有两种方式: 构造器注入和setter注入



构造器注入:
<bean id="student" class="com.ioc.bean.Student">

    <!--基于构造器的注入方式，通过constructor-arg标签来注入-->

    <!--public Student(int id, String name, int age)-->

    <!--在使用构造器注入方式时要注意配置的参数的顺序必须符合构造器里参数的声明顺序-->

    <constructor-arg value="1001"></constructor-arg>

    <constructor-arg value="xiaoye"></constructor-arg>

    <constructor-arg value="20"></constructor-arg>

</bean>



<!--<!–下面演示如果不按构造器声明顺序正确的配置会出现的情况一–>-->

<!--<bean id="student_error" class="com.ioc.bean.Student">-->

    <!--<constructor-arg value="xiaoye"></constructor-arg>-->

    <!--<constructor-arg value="1001"></constructor-arg>-->

    <!--<constructor-arg value="20"></constructor-arg>-->

<!--</bean>-->

<!--<!–因为无法将String转为int所以会直接报错–>-->



<!--<!–下面演示如果不按构造器声明顺序正确的配置会出现的情况二–>-->

<!--<bean id="student_error" class="com.ioc.bean.Student">-->

    <!--<constructor-arg value="20"></constructor-arg>-->

    <!--<constructor-arg value="xiaoye"></constructor-arg>-->

    <!--<constructor-arg value="1001"></constructor-arg>-->

<!--</bean>-->

<!--<!–此种情况虽然可以正常运行，但id为20且age为1001,这显然比上一个更无法接受，因为这玩意不叫error，叫bug–>-->


setter注入
<!--看官方文档可以看到setter注入使用到的标签是property-->

<bean id="studentBySetter" class="com.ioc.bean.Student">

    <property name="id" value="1001"></property>

    <property name="age" value="20"></property>

    <property name="name" value="xiaoye"></property>

</bean>

<!--使用property需要知道name，注意，这个name不是属性名，而是setXxx()方法的Xxx的首字母小写-->


下面还有一个问题，如果这个属性是一个对象怎么办?
<bean id="xiaohong" class="com.ioc.bean.Student">

    <property name="name" value="xiaohong"></property>

    <property name="lover" ref="xiaoming"></property>

    <!--使用ref标签可以注入对象，ref的属性值是配置的一个bean的id-->

</bean>




