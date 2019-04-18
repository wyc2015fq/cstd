# Spring mvc 视图解析器 ContentNegotiatingViewResolver 源码分析 - z69183787的专栏 - CSDN博客
2014年12月03日 14:19:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：32130
参考文章：http://blog.csdn.net/z69183787/article/details/41658489
http://blog.csdn.net/z69183787/article/details/41693015
http://blog.csdn.net/z69183787/article/details/41693035
http://blog.csdn.net/z69183787/article/details/41654603
参考了上面几篇文章后，发现还不是了解的很透彻，比如defaultViews、contentType与匹配规则的关系、视图匹配原则，InternalResourceViewResolver为何无法匹配等等，之后将spring-webmvc的java代码本地编译，并设了断点后一步步调试，才终于搞明白。
附上关键xml配置：
message-converter配置：
```
<mvc:annotation-driven content-negotiation-manager="cnManager">
        <mvc:message-converters register-defaults="false">
            <bean class="org.springframework.http.converter.StringHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
            <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <!--<property name="supportedMediaTypes">-->
                    <!--<list>-->
                        <!--<value>text/html;charset=UTF-8</value>-->
                        <!--<value>application/json;charset=UTF-8</value>-->
                        <!--<value>application/javascript;charset=UTF-8</value>-->
                        <!--<value>text/plain;charset=UTF-8</value>-->
                    <!--</list>-->
                <!--</property>-->
                <property name="prettyPrint" value="true"></property>
                <property name="objectMapper">
                    <bean class="com.fasterxml.jackson.databind.ObjectMapper">
                        <property name="dateFormat">
                            <bean class="java.text.SimpleDateFormat">
                                <constructor-arg type="java.lang.String" value="yyyy-MM-dd HH:mm:ss" />
                            </bean>
                        </property>
                    </bean>
                </property>
            </bean>
            <bean class="org.springframework.http.converter.xml.MarshallingHttpMessageConverter">
                <property name="marshaller" ref="jaxb2Marshaller"></property>
            </bean>
        </mvc:message-converters>
    </mvc:annotation-driven>
```
```
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
```
```
<bean id="viewResolver"
          class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="contentNegotiationManager" ref="cnManager"/>
        <property name="viewResolvers"><!-- 针对freemarker的视图配置 --> 
            <list>
                <bean class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>
                    <property name="viewNames" value=".ftl"/>
                    <property name="contentType" value="text/html; charset=utf-8"/>
                    <property name="cache" value="true" />
                    <property name="prefix" value="" />
                    <property name="suffix" value="" />
                    <property name="order" value="2"/>
                </bean>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="prefix" value="/WEB-INF/view/jsp/"></property><!-- 配置页面路径 -->
                    <property name="suffix" value=".jsp"></property><!-- 文件以value值结尾 -->
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
                <bean class="com.wonders.stpt.bid.controller.JXLExcelView"/>
            </list>
        </property>
    </bean>
```
步骤概要：
方法resolveViewName中根据方法getMediaTypes获取mediaType（由accept-header、请求参数或后缀名中获取，springmvc每个view的默认contenttype为text/html），根据访问的viewname与mediaType查找候选视图集合candidateViews，再根据方法getBestView获取最优视图并返回。
其中，ContentNegotiatingViewResolver 中包含2个属性：
ContentNegotiationManagerFactoryBean（**XML中viewResolver**）：使用ContentNegotiationStrateg解析策略构造ContentNegotiationManager（afterPropertiesSet方法中初始化）
ContentNegotiationManager（**XML中cnManager**）：包含List<ContentNegotiationStrategy> contentNegotiationStrategies属性记录匹配请求的解析策略，可能涉及的解析策略如下
1、org.springframework.web.accept.ServletPathExtensionContentNegotiationStrategy
后缀名，例：请求为 aaa.json
2、org.springframework.web.accept.ParameterContentNegotiationStrategy
参数，默认参数名称为format，例：请求为format=json
3、org.springframework.web.accept.HeaderContentNegotiationStrategy
通过accept-header匹配 content-type
4、org.springframework.web.accept.FixedContentNegotiationStrategy
通过设定defaultContentType content-type
```
<span style="font-size:12px;"> <property name="ignoreAcceptHeader" value="true"/>
        <property name="favorPathExtension" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="favorParameter" value="true"/>
        <property name="mediaTypes">
            <map>
                <entry key="xml" value="application/xml"/>
                <entry key="json" value="text/plain"/>
                <entry key="xls" value="application/vnd.ms-excel"/>
            </map>
        </property></span>
```
**favorPathExtension表示支持后缀匹配，favorParameter表示支持参数匹配，属性ignoreAcceptHeader默认为fasle，表示accept-header匹配，defaultContentType开启默认匹配。**
其中cnManager中的mediaTypes预设key对应后缀名及参数format的值， 与匹配策略1、2相关。
例如：请求aaa.xx，若设置<entry key="xx" value="application/xml"/> 也能匹配以xml返回。
根据以上条件进行一一匹配最终，得到相关并符合的策略初始化ContentNegotiationManager
1、getMediaTypes方法使用ContentNegotiationManager的resolveMediaTypes方法（使用上述匹配策略）得到需要的contenttype类型例：请求aaa.xx，若设置<entry key="xx" value="application/xml"/> ，返回application/xml的MediaType
2、getCandidateViews方法根据XML中预设的viewResolvers匹配1中的MediaType获取候选View，同时增加XML中defaultViews中的view。每个view都有对应的content-type，大部分的视图继承于AbstractView，默认值都是text/html;charset=ISO-8859-1。有些特定视图，比如MappingJackson2JsonView会重设contentType为application/json。每个view中的contentType在最后匹配中都会实际用到，所以很重要。
在此方法中，会调用每一个viewResolvers的resolveViewName方法创建view，需要传入的参数为viewname与locale，创建过程中，
会先从cache中查看是否已经匹配过，若存在，直接返回view；若不存在，继续创建view。
第一步：检测viewResolvers中的viewNames属性，<property name="viewNames" value=".ftl"/>（支持正则表达式
 如*.ftl,），
表达式：return (viewNames == null || PatternMatchUtils.simpleMatch(viewNames, viewName));
若未设置viewNames属性，则默认为null，允许创建，进入第二步；若设置，会根据viewNames进行正则表达式的匹配，若匹配继续第二步，若不匹配，返回null view，跳出此viewResolver；继续与下一个viewresolvers进行匹配并获得view。
第二步：进行每个viewResolve都有的view的创建过程（初始化content-type,记录最终跳转页面prefix，postfix等），但诸如Freemarker，velocity 的 viewResolvers都会在view创建过程中的最后一步调用重写checkResource方法，查看最终跳转的ftl或vm文件是否存在？若不存在，则将创建的视图View重设成null并返回，故无法匹配并加入候选视图。
而InternalResourceView的checkResource没有重写，调用父类AbstractUrlBasedView的checkResource，默认返回true，为所有请求创建view，无论是否有最终文件。所以，InternalResourceView一般都作为springmvc最后的视图解析器，来处理一切请求。
```java
private List<View> getCandidateViews(String viewName, Locale locale, List<MediaType> requestedMediaTypes)
			throws Exception {
		List<View> candidateViews = new ArrayList<View>();
		for (ViewResolver viewResolver : this.viewResolvers) {
			View view = viewResolver.resolveViewName(viewName, locale);
			if (view != null) {
				candidateViews.add(view);
			}
			for (MediaType requestedMediaType : requestedMediaTypes) {
				List<String> extensions = this.contentNegotiationManager.resolveFileExtensions(requestedMediaType);
				for (String extension : extensions) {
					String viewNameWithExtension = viewName + "." + extension;
					view = viewResolver.resolveViewName(viewNameWithExtension, locale);
					if (view != null) {
						candidateViews.add(view);
					}
				}
			}
		}
		if (!CollectionUtils.isEmpty(this.defaultViews)) {
			candidateViews.addAll(this.defaultViews);
		}
		return candidateViews;
	}
```
3、getBestView方法获取最符合条件的匹配视图
首先，如果是smartView，即redirectView则return
否则：筛选getMediaTypes返回的content-type类型，与candidateViews中的候选视图匹配，其中：
下面的contentType重写了MappingJackson2JsonView的contentType，用于匹配cnManager中的mediaTypes，若该视图可以解析此contenttype，则返回
例子：若访问localhost/xxx/a.json，匹配策略为后缀匹配（favorExtension开启），由于后缀为json，则匹配cnManager中的text/plain筛选view后发现匹配视图为MappingJackson2JsonView，返回json格式的数据；若访问localhost/xxx/a,由于无后缀及参数匹配，则匹配策略为accept-header匹配，由于content-type为text/html，则匹配InternalResourceView中的视图，返回jsp页面。
你可能注意到，上述viewResolvers中配置了两个视图，为什么匹配InternalResourceView,而不是freeMarker？这是因为进入候选试图的只有InternalResourceView视图解析器，具体原因参见getCandidateViews。
```
<bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
                    <!-- <property name="extractValueFromSingleKeyModel" value="true"/> -->
                    <property name="prettyPrint" value="true"/>
                    <property name="contentType" value="text/plain"/>
                </bean>
```
```java
private View getBestView(List<View> candidateViews, List<MediaType> requestedMediaTypes, RequestAttributes attrs) {
		for (View candidateView : candidateViews) {
			if (candidateView instanceof SmartView) {
				SmartView smartView = (SmartView) candidateView;
				if (smartView.isRedirectView()) {
					if (logger.isDebugEnabled()) {
						logger.debug("Returning redirect view [" + candidateView + "]");
					}
					return candidateView;
				}
			}
		}
		for (MediaType mediaType : requestedMediaTypes) {
			for (View candidateView : candidateViews) {
				if (StringUtils.hasText(candidateView.getContentType())) {
					MediaType candidateContentType = MediaType.parseMediaType(candidateView.getContentType());
					if (mediaType.isCompatibleWith(candidateContentType)) {
						if (logger.isDebugEnabled()) {
							logger.debug("Returning [" + candidateView + "] based on requested media type '"
									+ mediaType + "'");
						}
						attrs.setAttribute(View.SELECTED_CONTENT_TYPE, mediaType, RequestAttributes.SCOPE_REQUEST);
						return candidateView;
					}
				}
			}
		}
		return null;
	}
```
