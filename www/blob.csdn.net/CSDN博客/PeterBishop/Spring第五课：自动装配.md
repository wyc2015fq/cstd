# Spring第五课：自动装配 - PeterBishop - CSDN博客





2019年02月19日 17:09:29[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：363
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这课我们来讲自动装配，或者叫自动注入，Autowired



  Spring容器可以自动装配协作bean之间的关系。不扯淡，直接看代码

![](https://img-blog.csdnimg.cn/20190219170915149.png)





autowire属性的几个可选值:

default: 就是no

byName: 根据属性名称使用getBean(String name)方法装配bean

byType: 根据属性类型使用getBean(Class reuiredType)方法装配bean

constructor: 根据该类的构造器自动装配bean

no: 默认值，不进行自动装配





byName
<bean id="father" class="com.ioc.bean.Father">

    <property name="name" value="父亲"></property>

</bean>



<bean id="mother" class="com.ioc.bean.Mother">

    <property name="name" value="母亲"></property>

</bean>



<bean id="child" class="com.ioc.bean.Daughtor">

    <property name="name" value="女儿"></property>

</bean>



<!--<bean id="child2" class="com.ioc.bean.Son">-->

    <!--<property name="name" value="儿子"></property>-->

<!--</bean>-->





<!--

public class Family {

    private Father father;

    private Mother mother;

    private Child child;

}

可以看到Family各个属性的名字和上面的bean的id一样，所以可以自动装配成功

当getBean(String name)无法匹配到合适的bean时就会返回null，这时Family的child属性就为null了



-->

<bean id="family" class="com.ioc.bean.Family" autowire="default">



</bean>







byType
<bean id="father" class="com.ioc.bean.Father">

    <property name="name" value="父亲"></property>

</bean>



<bean id="mother" class="com.ioc.bean.Mother">

    <property name="name" value="母亲"></property>

</bean>



<bean id="child1" class="com.ioc.bean.Daughtor">

    <property name="name" value="女儿"></property>

</bean>



<bean id="child2" class="com.ioc.bean.Son">

    <property name="name" value="儿子"></property>

</bean>




<!--

public class Family {

    private Father father;

    private Mother mother;

    private Child child;

}

byType则直接根据属性的类型进行匹配,但如果根据类型匹配到多个bean则会抛出异常



-->

<bean id="family" class="com.ioc.bean.Family" autowire="default">



</bean>
no和default
<bean id="father" class="com.ioc.bean.Father">

    <property name="name" value="父亲"></property>

</bean>



<bean id="mother" class="com.ioc.bean.Mother">

    <property name="name" value="母亲"></property>

</bean>



<bean id="child1" class="com.ioc.bean.Daughtor">

    <property name="name" value="女儿"></property>

</bean>



<bean id="child2" class="com.ioc.bean.Son">

    <property name="name" value="儿子"></property>

</bean>




<!--

public class Family {

    private Father father;

    private Mother mother;

    private Child child;

}no就是不进行自动装配

default也是no-->

<bean id="family" class="com.ioc.bean.Family" autowire="no">



</bean>


constructor一般不用



