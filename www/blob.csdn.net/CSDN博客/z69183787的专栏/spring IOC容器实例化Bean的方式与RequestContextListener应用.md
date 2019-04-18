# spring IOC容器实例化Bean的方式与RequestContextListener应用 - z69183787的专栏 - CSDN博客
2013年12月01日 09:56:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1171
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)

spring IOC容器实例化Bean的方式有：
singleton            在spring IOC容器中仅存在一个Bean实例,Bean以单实例的方式存在.
prototype            每次从容器中调用Bean时,都返回一个新的实例,即每次调用getBean()时,相当于执行new XxxBean()的操作.
request               每次HTTP请求都会创建一个新的Bean,该作用域仅适用于webApplicationContext环境.
session               同一个HTTP session共享一个Bean,不同HTTP session使用不同的Bean,该作用域仅适用于webApplicationContext环境.
globalSession   同一个全局session共享一个Bean,一般用于portlet应用环境,该作用域仅适用于webApplicationContext环境.
在低版本的spring中,由于只有两个Bean作用域,所以采用singleton="true|false"的配置方 式,spring2.0为了向后兼容,依旧支持这种配置方式.不过,spring2.0推荐采用新的配置方式:scope="<作用域类型>;"
-------------------------------------------------
singleton作用域
spring以容器的方式提供天然的单实例模式功能,任何POJO无须编写特殊的代码仅通过配置就可以了.
注意:spring将Bean的默认作用域定为singleton.
singleton例:
<bean id="car" class="com.baobaotao.scope.Car" scope="singleton"/>
<bean id="boss1" class="com.baobaotao.scope.Boss">
<property name="car" ref="car"/>
</bean>
Car Bean声明为singleton(因为默认是singleton,所以可以不显式指定).
在默认情况下,spring的ApplicationContext容器在启动时,自动实例化所有singleton的Bean并缓存于容器中.
虽然启动时会花费一些时间,但带来两个好处:首先对Bean提前的实例化操作会及早发现一些潜在的配置问题.
其次Bean以缓存的方式保存,当运行时使用到该Bean时就无须再实例化了,加快了运行效率.如果用户不希望在容
器启动时提前实例化singleton的Bean,可以通过lazy-init属性进行控制:
<bean id="boos1" class="com.baobaotao.scope.Boss" lazy-init="true">
<property name="car" ref="car"/>
</bean>
lazy-init="true"的Bean在某些情况下依旧会提前实例化:如果该Bean被其它需要提前实例化的Bean引用到,
spring也将忽略延迟实例化的设置.
-------------------------------------------------
prototype作用域
采用scope="prototype"指定非单实例作用域Bean,请看:
<bean id="car" class="com.baobaotao.scope.Car" scope="prototype"/>
<bean id="boss1" class="com.baobaotao.scope.Boss">
<property name="car" ref="car"/>
</bean>
<bean id="boss2" class="com.baobaotao.scope.Boss">
<property name="car" ref="car"/>
</bean>
boss1,boss2所引用的都是一个独立的Car实例.
在默认情况下,spring容器在启动时不实例化prototype的Bean.此外,spring容器将prototype的Bean交给调用
者后,就不再管理它的生命周期.
-------------------------------------------------
web应用环境相关的Bean作用域
如果用户使用spring的webApplicationContext,则可以使用另外3种Bean的作用域:request,session和globalSession.不过
在使用这些作用域之前,首先必须在web容器中进行一些额外的配置,在高版本的web容器中,则可以利用HTTP请求监听器进行配置:
<web-app>
...
<listener>
<listener-class>
org.springframework.web.context.request.RequestContextListener
</listener-class>
</listener>
...
</web-app>
细心的朋友可能有一个疑问:在介绍webApplicationContext初始化时,我们已经通过ContextLoaderListener将web容器与
spring容器整合,为什么这里又要引入一个额外的RequestContextListener以支持Bean的另外3个作用域呢?
在整合spring容器时使用ContextLoaderListener,它实现了ServletContextListener监听器接口,ServletContextListener
只负责监听web容器启动和关闭的事件.而RequestContextListener实现ServletRequestListener监听器接口,该监听器监听
HTTP请求事件,web服务器接收的每一次请求都会通知该监听器.
spring容器启动和关闭操作由web容器的启动和关闭事件触发,但如果spring容器中的Bean需要request,session,globalsession
作用域的支持,spring容器本身就必须获得web容器的HTTP请求事件,以HTTP请求的事件"驱动"Bean作用域的控制逻辑.
request作用域
顾名思义,request作用域的Bean对应一个HTTP请求和生命周期,考虑下面的配置:
<bean name="car" class="com.baobaotao.scope.Car" scope="request"/>
这样,每次HTTP请求调用到car Bean时,spring容器创建一个新的Car Bean,请求处理完毕后,销毁这个Bean.
session作用域
假设将以上car的作用域调整为session类型:
<bean name="car" class="com.baobaotao.scope.Car" scope="session"/>
这样配置后,car Bean的作用域横跨整个HTTP session,session中所有HTTP请求都共享同一个Car Bean,当HTTP Session结束后,实例
才被销毁.
globalSession作用域
下面的配置片断将car的作用域设置为了globalSession:
<bean name="loginController" class="com.baobaotao.scope.Car" scope="globalSession"/>
globalSession作用域类似于session作用域,不过仅在portlet的web应用中使用.Portlet规范定义了全局Session概念,它被组成portlet
web应用的所有子portlet共享.如果不在Portlet web应用环境下,globalSession自然等价于session作有域了
