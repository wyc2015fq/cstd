# SpringMVC之HandlerMethodArgumentResolver和 - z69183787的专栏 - CSDN博客
2017年01月24日 17:35:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1133
SpringMVC提供了一个HandlerMethodArgumentResolver接口可以让我们处理方法的参数，和注解结合提来，能有很强大的功能，例如SpringMVC提供的@ModelAttribute就是通过这种方法来处理的，它有对应的实现HandlerMethodArgumentResolver接口的类----ModelAttributeMethodProcessor，通过这种方法来处理参数
我们也可以使用一个类实现HandlerMethodArgumentResolver，在<mvc:annotation-driven>的子标签<mvc:argument-resolvers>中配置好后，SpringMVC就会调用我们写的类，然后我们在里面进行对参数的处理就好了，简单的示例代码如下，下面的功能是使用一个自定义的Java注解，来给参数设置一个初始值
Java注解如下：
```java
@Target(value=ElementType.PARAMETER)
@Retention(RetentionPolicy.RUNTIME)  
public @interface ValueAnnotation {
	public String value() default "";
}
```
Resolver如下：
```java
public class MyResolver implements HandlerMethodArgumentResolver {
	/*
	 * 返回false则不会进入到resolveArgument方法，返回true则会
	 */
	@Override
	public boolean supportsParameter(MethodParameter parameter) {
		return parameter.hasParameterAnnotation(ValueAnnotation.class);
	}
	@Override
	public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer, NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception {
		// 通过parameter获取参数上的Annotation
		ValueAnnotation annotation = parameter.getParameterAnnotation(ValueAnnotation.class);
		// 通过Annotation获取设置的值
		String value = annotation.value();
		// 将值返回给参数
		return value;
	}
}
```
Controller如下：
```java
@Controller
@RequestMapping("resolver")
public class AnnotationAndResolverController {
	
	@RequestMapping(value = "/test")
	public void test(@ValueAnnotation("123") String value) {
		System.out.println(value);
	}
}
```
会打印出123 
配置如下：
```
<mvc:annotation-driven>
        <mvc:argument-resolvers>
            <bean class="resolver.MyResolver"/>
            <bean class="resolver.MyModelAttributeResolver"/>
        </mvc:argument-resolvers>
    </mvc:annotation-driven>
```
只是简单的写了个例子来记录用法，在resolveArgument里面可以做很多事，例如在里面可以利用parameter获取参数的Class对象，然后里面反射机制来处理数据，或者在里面进行验证参数是否非空，等等。
可以看看SpringMVC提供的注解，很多利用HandlerMethodArgumentResolver处理数据，有兴趣可以看看里面的源码，有更多高级的用法，限于水平就不再展开
