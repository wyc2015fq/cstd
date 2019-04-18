# 终于找到全annotation配置springMVC的方法了（事务不失效) - z69183787的专栏 - CSDN博客
2014年07月15日 16:39:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1873
原理参考：
[http://jinnianshilongnian.iteye.com/blog/1762632](http://jinnianshilongnian.iteye.com/blog/1762632)
如果带上事务，那么用annotation方式的事务注解和bean配置，事务会失效，要将service　bean配置到xml文件中才行 
这个问题是由于问答上有解决方案 
引用 
这个问题很经典了 
在主容器中（applicationContext.xml），将Controller的注解排除掉 
<context:component-scan base-package="com"> 
  <context:exclude-filter type="annotation" expression="org.springframework.stereotype.Controller" /> 
</context:component-scan> 
而在springMVC配置文件中将Service注解给去掉 
<context:component-scan base-package="com"> 
  <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller" /> 
  <context:exclude-filter type="annotation" expression="org.springframework.stereotype.Service" /> 
  </context:component-scan> 
因为spring的context是父子容器，所以会产生冲突，由ServletContextListener产生的是父容器，springMVC产生的是子容器，子容器Controller进行扫描装配时装配了@Service注解的实例，而该实例理应由父容器进行初始化以保证事务的增强处理，所以此时得到的将是原样的Service（没有经过事务加强处理，故而没有事务处理能力。 
还有一种方式是将service层改用xml配置,其实这样做也是变相的让springmvc无法扫描service，而只能依赖父窗口也就是ServletContextListener来进行初始化，这样同样被赋予了事务性。 
