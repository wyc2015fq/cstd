# Spring第四课：懒加载 - PeterBishop - CSDN博客





2019年02月19日 17:08:43[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：23
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这课我们来学习懒加载，或者说叫懒初始化(lazy-initial)



默认情况下，ApplicationContext实现会在初始化的过程中立即创建和配置所有 [单例](https://docs.spring.io/spring/docs/5.1.5.RELEASE/spring-framework-reference/core.html#beans-factory-scopes-singleton) bean，通常，这种预先实例化是可取的，因为配置或周围环境中的错误是立即发现的，而不是几小时甚至几天后。当不希望出现这种情况时，可以通过将bean定义标记为延迟初始化来阻止单例bean的预实例化。延迟初始化的bean告诉IoC容器在第一次请求时创建bean实例，而不是在启动时。


<bean` `id`=`"lazy"` `class`=`"com.something.ExpensiveToCreateBean"` `lazy-init`=`"true"/>



<!-- 懒加载bean，讲lazy-init属性设置为true即可 -->

<!-- 懒加载的bean并不会在ioc容器初始化时就立刻被初始化好，而是要等到这个bean在第一次被调用getBean()方法获取时才创建并完成初始化 -->

<!-- 注意，lazy-init属性默认为false -->

<bean id="lazyStudent" class="com.ioc.bean.Student" lazy-init="true">

    <property name="id" value="1003"></property>

    <property name="age" value="20"></property>

    <property name="name" value="懒学生"></property>

</bean>




