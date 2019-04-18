# Spring mvc 标签对应处理Java类 - z69183787的专栏 - CSDN博客
2018年04月03日 16:11:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：136
spring-webmvc-4.3.9.RELEASE.jar
![](https://img-blog.csdn.net/20180403160736248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
public class MvcNamespaceHandler extends NamespaceHandlerSupport {
	@Override
	public void init() {
		registerBeanDefinitionParser("annotation-driven", new AnnotationDrivenBeanDefinitionParser());
		registerBeanDefinitionParser("default-servlet-handler", new DefaultServletHandlerBeanDefinitionParser());
		registerBeanDefinitionParser("interceptors", new InterceptorsBeanDefinitionParser());
		registerBeanDefinitionParser("resources", new ResourcesBeanDefinitionParser());
		registerBeanDefinitionParser("view-controller", new ViewControllerBeanDefinitionParser());
		registerBeanDefinitionParser("redirect-view-controller", new ViewControllerBeanDefinitionParser());
		registerBeanDefinitionParser("status-controller", new ViewControllerBeanDefinitionParser());
		registerBeanDefinitionParser("view-resolvers", new ViewResolversBeanDefinitionParser());
		registerBeanDefinitionParser("tiles-configurer", new TilesConfigurerBeanDefinitionParser());
		registerBeanDefinitionParser("freemarker-configurer", new FreeMarkerConfigurerBeanDefinitionParser());
		registerBeanDefinitionParser("velocity-configurer", new VelocityConfigurerBeanDefinitionParser());
		registerBeanDefinitionParser("groovy-configurer", new GroovyMarkupConfigurerBeanDefinitionParser());
		registerBeanDefinitionParser("script-template-configurer", new ScriptTemplateConfigurerBeanDefinitionParser());
		registerBeanDefinitionParser("cors", new CorsBeanDefinitionParser());
	}
}
```
常用的注解 annotation-driven 在 AnnotationDrivenBeanDefinitionParser 中处理
包含一些 在 入参与返回值的数据处理 ，还有一些 messageConverters 的注册
分别是RequestMappingHandlerAdapter和RequestMappingHandlerMapping两个类，前者是注册注解处理器的后者是进行注解映射的，里面都封装了Spring默认的一些注解处理器和返回值处理器
![](https://img-blog.csdn.net/20180403161011299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面的代码是RequestMappingHandlerAdapter中Spring默认的注解处理器（resolvers）和返回值处理器（handlers）集合，RequestMappingHandlerAdapter和RequestMappingHandlerMapping都在org.springframework.web.servlet.mvc.method.annotation包下面， RequestMappingHandlerAdapter不仅仅是定义注解处理器和返回值处理器，还可以定义消息转换器（messageconverters）和视图模型转换器（modelAndViewResolvers）等等
![](https://img-blog.csdn.net/20180403161338489?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180403161445356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
