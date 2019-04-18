# spring mvc json及各类视图解析 源码分析 - z69183787的专栏 - CSDN博客
2016年08月24日 07:54:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1888
附上自己项目的配置文件：
```
<?xml version="1.0" encoding="UTF-8" ?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.2.xsd">
    <mvc:annotation-driven content-negotiation-manager="cnManager">
        <mvc:message-converters register-defaults="false">
            <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <property name="supportedMediaTypes">
                    <list>
                        <value>text/html</value>
                        <value>application/json</value>
                        <value>application/javascript</value>
                        <value>text/plain</value>
                    </list>
                </property>
                <property name="prettyPrint" value="true"></property>
            </bean>
            <bean class="org.springframework.http.converter.xml.MarshallingHttpMessageConverter">
                <property name="marshaller" ref="jaxb2Marshaller"></property>
            </bean>
        </mvc:message-converters>
    </mvc:annotation-driven>
    <!-- 当在web.xml 中   DispatcherServlet使用 <url-pattern>/</url-pattern> 映射时，能映射静态资源 -->
    <mvc:default-servlet-handler/>
    <context:component-scan base-package="com.dianping.emidas.merchant.**.controller">
        <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller"/>
        <context:include-filter type="annotation" expression="org.springframework.web.bind.annotation.ControllerAdvice"/>
    </context:component-scan>
    <!-- 静态资源映射 -->
    <mvc:resources mapping="/image/**" location="/WEB-INF/image/"/>
    <mvc:resources mapping="/css/**" location="/WEB-INF/css/"/>
    <mvc:resources mapping="/js/**" location="/WEB-INF/js/"/>
    <!-- 支持上传文件 -->
    <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver"/>
    <bean id="cnManager" class="org.springframework.web.accept.ContentNegotiationManagerFactoryBean">
        <property name="ignoreAcceptHeader" value="true"/>
        <property name="favorPathExtension" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="favorParameter" value="true"/>
        <property name="mediaTypes">
            <map>
                <entry key="xml" value="application/xml"/>
                <entry key="json" value="text/plain"/>
                <entry key="xls" value="application/vnd.ms-excel"/>
            </map>
        </property>
    </bean>
    <!-- freemarker config -->
    <bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
        <property name="templateLoaderPath" value="/WEB-INF/view/ftl/"/>
        <property name="defaultEncoding" value="UTF-8"/>
        <property name="freemarkerVariables">
            <map>
                <entry key="xml_escape" value-ref="fmXmlEscape"/>
            </map>
        </property>
        <property name="freemarkerSettings">
            <props>
                <!--<prop key="template_update_delay">3600</prop>-->
                <prop key="tag_syntax">auto_detect</prop>
                <!-- 设置标签类型 两种：[] 和 <> 。[] 这种标记解析要快些 -->
                <prop key="default_encoding">UTF-8</prop>
                <prop key="output_encoding">UTF-8</prop>
                <prop key="locale">zh_CN</prop>
                <prop key="date_format">yyyy-MM-dd</prop>
                <prop key="time_format">HH:mm:ss</prop>
                <prop key="auto_import">/spring.ftl as spring</prop>
                <prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>
                <prop key="number_format">#</prop>
                <!-- 设置数字格式 以免出现 000.00 -->
                <prop key="classic_compatible">true</prop>
                <!-- 可以满足一般需要。默认情况变量为null则替换为空字符串，如果需要自定义，写上${empty!"EmptyValue of fbysss"}的形式即可  -->
                <prop key="template_exception_handler">html_debug</prop>
                <!-- ignore,debug,html_debug,rethrow -->
            </props>
        </property>
    </bean>
    <bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape"/>
    <!--Basic application beans. -->
    <bean id="viewResolver"
          class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="contentNegotiationManager" ref="cnManager"/>
        <property name="viewResolvers">
            <list>
                <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>
                    <property name="contentType" value="text/html; charset=utf-8"/>
                    <property name="requestContextAttribute" value="request"/>
                    <property name="suffix" value=".ftl"/>
                    <property name="cache" value="true"/>
                    <property name="order" value="0"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView" />
                    <property name="prefix" value="/WEB-INF/view/jsp/"></property>
                    <property name="suffix" value=".jsp"></property>
                    <property name="order" value="1"/>
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
                    <!-- <property name="extractValueFromSingleKeyModel" value="true"/> -->
                    <property name="prettyPrint" value="true"/>
                    <property name="contentType" value="text/plain"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <constructor-arg ref="jaxb2Marshaller">
                    </constructor-arg>
                </bean>
                <!--<bean class="com.wonders.stpt.match.controller.ExcelView"/>-->
            </list>
        </property>
    </bean>
    <bean id="jaxb2Marshaller" class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
        <property name="packagesToScan">
            <list>
                <value>com.dianping.emidas.merchant.**.domain</value>
            </list>
        </property>
        <!--<property name="classesToBeBound">-->
        <!--<list>-->
        <!--<value>com.wonders.stpt.match.domain.MatchTheme</value>-->
        <!--</list>-->
        <!--</property>-->
    </bean>
    <!--  控制器异常处理 -->
    <!--<bean id="exceptionHandlerExceptionResolver" class="org.springframework.web.servlet.mvc.method.annotation.ExceptionHandlerExceptionResolver">-->
    <!--</bean>-->
    <!--<bean class="com.dianping.emidas.merchant.base.exception.DefaultExceptionHandler"/>-->
    <!-- 异常处理 -->
   <bean id="exceptionResolver"
         class="com.dianping.emidas.merchant.base.exception.CustomSimpleMappingExceptionResolver">
       <!-- 定义默认的异常处理页面，当该异常类型的注册时使用  -->
        <property name="defaultErrorView" value="error/error-page"></property>
        <!-- 定义异常处理页面用来获取异常信息的变量名，默认名为exception -->
        <property name="exceptionAttribute" value="ex"></property>
        <!-- 定义需要特殊处理的异常，用类名或完全路径名作为key，异常也页名作为值  -->
        <property name="exceptionMappings">
            <props>
                <prop key="org.springframework.security.access.AccessDeniedException">error/error-page</prop>
                <prop key="com.dianping.emidas.merchant.base.exception.SystemException">error/error-page</prop>
                <prop key="com.dianping.emidas.merchant.base.exception.BusinessException">error/error-page</prop>
                <prop key="java.lang.exception">error/error-page</prop>
            </props>
        </property>
        <!-- 相关状态码对应的错误页面 -->
        <property name="statusCodes">
            <props>
                <prop key="error/error-page">500</prop>
                <!--<prop key="error/error-page">404</prop>-->
            </props>
        </property>
        <!-- 设置日志输出级别，不定义则默认不输出警告等错误日志信息 -->
        <property name="warnLogCategory" value="以下为警告信息"/>
        <!-- 默认HTTP状态码 -->
        <property name="defaultStatusCode" value="500"/>
    </bean>
</beans>
```
首先谈一下，mvc中对各种请求及返回类型的一些认识。（以下内容皆基于springmvc 4.0之后的版本）
项目中基本的使用情况，有以下几类
请求：
1、path、parameter，key/value形式绑定数据
2、requestBody，json格式字符串映射controller参数对象，只支持post请求，并且contentType需设置为contentType: "application/json"
返回：
1、freemarker、velocity基础请求返回基于模板引擎渲染的html页面
2、rest形式，返回json/jsonp/xml形式的对象，具体返回类型由请求头 accept与提供的messageconverter决定 accept与contentType概念参考
[http://blog.csdn.net/z69183787/article/details/52313101](http://blog.csdn.net/z69183787/article/details/52313101)
3、excel或pdf类文件流以供下载
## requestbody、responsebody（messageConverter处理）：
3.X版本后spring 使用 RequestMappingHandlerMapping和RequestMappingHandlerAdapter 处理requestbody及responsebody及一些注解
同样也可以使用默认配置 <mvc:annotation-driven/>，相当于注册了上述两个处理器及对应的messageConverter
messageConverter用于对请求及响应的对象及消息进行对应的处理转换，有如下几个实现：
StringHttpMessageConverter：将请求信息转换为字符串，T为String，可读取所有媒体类型(*/*),可通过supportedMediaTypes属性指定媒体类型
ByteArrayHttpMessageConverter：读写二进制数据，T为byte[]类型，可读取*/*，可通过supportedMediaTypes属性指定媒体类型，响应信息媒体类型为application/octer-stream
MarshallingHttpMessageConverter：T为Object，可读取text/xml和application/xml媒体类型请求，响应信息的媒体类型为text/xml或application/xml
Jaxb2RootElementHttpMessageConverter：通过JAXB2读写XML信息，将请求消息转换到标注XmlRootElement和XmlType注解的类中，T为Object，可读取text/xml和application/xml媒体类型请求，响应信息的媒体类型为text/xml或application/xml
MappingJackson2HttpMessageConverter：利用jackson的ObjectMapper读写Json数据，T为Object，可读取application/json，响应媒体类型为application/json
FormHttpMessageConverter/XmlAwareFormHttpMessageConverter/ResourceHttpConverter
通常使用的比较多的就是MappingJackson2HttpMessageConverter：
```
<bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
<property name="supportedMediaTypes">
    <list>
<value>text/html</value>
<value>application/json</value>
<value>application/javascript</value>
<value>text/plain</value>
    </list>
</property>
<property name="prettyPrint" value="true"></property>
</bean>
```
简单描述一下此转换器的属性及工作方式
springmvc初始化时，会根据
```
<bean id="cnManager" class="org.springframework.web.accept.ContentNegotiationManagerFactoryBean">
        <!--<property name="ignoreAcceptHeader" value="true"/>-->
        <property name="favorPathExtension" value="true"/>
        <!--<property name="defaultContentType" value="text/html"/>-->
        <property name="favorParameter" value="true"/>
        <property name="mediaTypes">
            <map>
                <entry key="xml" value="application/xml"/>
                <entry key="json" value="application/json"/>
                <entry key="xls" value="application/vnd.ms-excel"/>
            </map>
        </property>
    </bean>
```
中的配置项生成ContentNegotiationManager，并将对应策略mediaType的解析strategies注入
其中配置项ignoreAcceptHeader true：忽视请求头中的accept中的contentType，去除HeaderContentNegotiationStrategy策略；false为默认项
favorPathExtension标示启动后缀匹配mediaType，比如a.xml匹配excel
defaultContentType标识默认contentType，启动FixedContentNegotiationStrategy策略
当defaultContentType生效时，会忽略header中的accept，chrome中浏览器访问接口的 accept为
- 
Accept:
text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
可以看到最后又 */*，该mediaType可以兼容任何响应mediaType。
完。
接上，其中supportedMediaTypes指定了该转换器可以支持的响应contentType，当请求中的contentType与该转换器的支持类型匹配时，springmvc会选择该消息转换器进行消息响应及格式处理。
在项目中最常见的情况就是，浏览器直接访问请求，返回http406，就是因为当浏览器直接访问同时（ignoreAcceptHeader 为true）时，
当设置defaultContentType时即返回 默认contentType，若恰好设置的值无法与转换器中的匹配，比如设置为text/html ，返回requestedMediaTypes 为 text/html，
当json转换器中没有配置该mediaType时，即会返回406（无法处理该类请求）。
当未设置defaultContentType时 由于所有策略均无法解析，mediaType为空 ，返回requestedMediaTypes 为 */*，即可匹配所有messageConverter
当如下请求进入时，并有@ResponseBody注解时，
```java
@ResponseBody
    @RequestMapping(method = RequestMethod.GET, value = "/test")
    public Test index(Test t) throws Exception {
        System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(t.getA()));
        return new Test();
    }
```
由AbstractMessageConverterMethodProcessor的writeWithMessageConverters进行处理，其中有如下关键方法
```java
Class<?> returnValueClass = getReturnValueType(returnValue, returnType);
HttpServletRequest servletRequest = inputMessage.getServletRequest();
List<MediaType> requestedMediaTypes = getAcceptableMediaTypes(servletRequest);
List<MediaType> producibleMediaTypes = getProducibleMediaTypes(servletRequest, returnValueClass);
```
其中List<MediaType> requestedMediaTypes = getAcceptableMediaTypes(servletRequest);将根据之前ContentNegotiationManager注入的strategies返回该请求所需要响应的contentType的mediaType。
List<MediaType> producibleMediaTypes = getProducibleMediaTypes(servletRequest, returnValueClass);根据returnValueClass的类型，对xml中的配置messageConverter进行匹配，若满足序列化或消息处理条件（canWrite），就将此messageConverter的supportedMediaTypes加入到结果集中。每个messageConverter都有默认的mediaType，比如MappingJackson2HttpMessageConverter为application/json;charset=UTF-8与application/*+json;charset=UTF-8；这里的messageConverter为之前<mvc:annotation-driven/>
 <mvc:message-converters register-defaults="false">注解注入的 各种消息处理器的实现，其中register-defaults为取消默认messageConverter。
关于messageConverter的其他内容可以参见：
[http://blog.csdn.net/z69183787/article/details/52261274](http://blog.csdn.net/z69183787/article/details/52261274)
[http://blog.csdn.net/z69183787/article/details/52261238](http://blog.csdn.net/z69183787/article/details/52261238)
[http://blog.csdn.net/z69183787/article/details/52302172](http://blog.csdn.net/z69183787/article/details/52302172)
之后对请求需要的与mvc提供的进行一个匹配，然后得到真实的messageConverter，并调用write方法进行json渲染。
以上是对响应消息时mvc的messageConverter处理模式。下面来看一下请求时的处理方式，这里使用@RequestBody直接接收json来调试，
RequestResponseBodyMethodProcessor负责处理
当如下请求进入时，@RequestBody请求必须为Post
```java
@ResponseBody
    @RequestMapping(method = RequestMethod.POST, value = "/test")
    public Test index(@RequestBody Test t) throws Exception {
        System.out.println(t);
        System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(t.getA()));
        return new Test();
    }
```
进入resolveArgument方法 对requestbody中的数据与参数的json对象进行绑定
```java
public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer,
			NativeWebRequest webRequest, WebDataBinderFactory binderFactory) throws Exception
```
有一点需要注意，如果ajax传入的json字符串中的参数比实际接收java对象的参数多，那么会抛出转换失败的异常org.springframework.http.converter.HttpMessageNotReadableException: Could not read JSON
解决方案：[http://blog.csdn.net/z69183787/article/details/52302323](http://blog.csdn.net/z69183787/article/details/52302323)
[http://blog.csdn.net/z69183787/article/details/52302392](http://blog.csdn.net/z69183787/article/details/52302392)
# 视图解析器（viewResolver处理）：
ContentNegotiatingViewResolver在我的理解里是对 同一资源请求进行不同形式的返回，参考[http://blog.csdn.net/z69183787/article/details/41654603?locationNum=1](http://blog.csdn.net/z69183787/article/details/41654603?locationNum=1)
流程：，未完待续。[http://blog.csdn.net/z69183787/article/details/41696709?locationNum=2](http://blog.csdn.net/z69183787/article/details/41696709?locationNum=2)
# jsonp支持
[http://blog.csdn.net/z69183787/article/details/52290014](http://blog.csdn.net/z69183787/article/details/52290014)
[http://blog.csdn.net/z69183787/article/details/52290032](http://blog.csdn.net/z69183787/article/details/52290032)
