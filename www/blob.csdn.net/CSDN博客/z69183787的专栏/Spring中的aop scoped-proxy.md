# Spring中的aop:scoped-proxy - z69183787的专栏 - CSDN博客
2014年06月04日 11:20:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1066
     首先看一下Spring文档上的两个例子对比： 
<bean id="userPreferences" class="com.foo.UserPreferences" scope="session"/>
<bean id="userManager" class="com.foo.UserManager">
<property name="userPreferences" ref="userPreferences"/>
</bean>
<bean id="userPreferences" class="com.foo.UserPreferences" scope="session">
<aop:scoped-proxy/>
</bean>
<bean id="userManager" class="com.foo.UserManager">
<property name="userPreferences" ref="userPreferences"/>
</bean>
       前一个例子没有使用<aop:scoped-proxy/>，并且userManager是singleton，所有userManager仅被初始化一次，并且其属性userPreferences也仅被注射一次。当session失效后，userManager仍将保留userPreferences实例。但后一个例子则不一样，userManager的属性userPreferences指向的是com.foo.UserPreferences实例的代理，当session过期后，userManager的属性userPreferences自然也不能再使用。网上有人解释说，这个代理只随着session的创建而创建，销毁而销毁。 
