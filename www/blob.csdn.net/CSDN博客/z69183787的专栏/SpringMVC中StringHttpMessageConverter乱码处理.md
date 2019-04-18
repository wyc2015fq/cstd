# SpringMVC中StringHttpMessageConverter乱码处理 - z69183787的专栏 - CSDN博客
2018年01月26日 11:16:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：226
个人分类：[SpringMvc-原理分析](https://blog.csdn.net/z69183787/article/category/6456963)
[http://blog.csdn.net/wangyangbto/article/details/48804155](http://blog.csdn.net/wangyangbto/article/details/48804155)
很多人都碰到过，SpringMVC使用 @ResponseBody 注解，返加字符串不做任何处理时，有可能会出现乱码问题。
这是由于 StringHttpMessageConverter 类中，默认采用的字符集是 ISO-8859-1。
public class StringHttpMessageConverter extends AbstractHttpMessageConverter<String> {
   public static final Charset *DEFAULT_CHARSET *= Charset.forName("ISO-8859-1");
   private final Charset defaultCharset;
   private final List<Charset> availableCharsets;
   private boolean writeAcceptCharset = true;
*/****    * A default constructor that uses {****@code ****"ISO-8859-1"} as the default charset.**    * ****@see ****#StringHttpMessageConverter(Charset)**    */*public StringHttpMessageConverter() {
      this(*DEFAULT_CHARSET*);
}
那么要解决乱码问题，需要改变 StringHttpMessageConverter
 中的默认字符集，本文中以UTF-8为例。
修改默认字付集：
<bean id="stringHttpMessageConverter" class="org.springframework.http.converter.StringHttpMessageConverter">
    <constructor-arg value="UTF-8" index="0"/>
    <property name="supportedMediaTypes">
        <list>
            <value>text/plain;charset=UTF-8</value>
        </list>
    </property>
</bean>配置起效
方式一：
<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping"/>
<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
    <property name="messageConverters">
        <list>
            <ref bean="stringHttpMessageConverter"/>
            <ref bean="jackson2HttpMessageConverter"/>
        </list>
    </property>
</bean>
<mvc:annotation-driven />注：配置必须在 <mvc:annotation-driven /> 之前，否则将不会启效；<mvc:annotation-driven
 /> 会自动注册DefaultAnnotationHandlerMapping 与AnnotationMethodHandlerAdapter。
方式二：
<mvc:annotation-driven>
    <mvc:message-converters>
        <ref bean="stringHttpMessageConverter"/>
        <ref bean="jackson2HttpMessageConverter"/>
    </mvc:message-converters>
</mvc:annotation-driven>
