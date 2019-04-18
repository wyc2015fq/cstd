# 解析mediaTypes+viewResolvers+viewResolvers - z69183787的专栏 - CSDN博客
2014年12月01日 22:39:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3248
spring在解析视图的时候有两个重要的接口：ViewResolver 和 View
ViewResolver 中只有一个方法 resolveViewName ，提供 view name 和 实际 view的映射；
View 中两个方法 getContentType 和 render ，解析请求中的参数并把这个请求处理成某一种 View.
说白了，就是ViewResolver 负责怎么去解析， 而View只代表一种 视图层的技术。
对于一个请求，应该返回什么样的视图是 ViewResolver 来决定的，spring3.0提供的 ViewResolver 包括 AbstractCachingViewResolver，
XmlViewResolver，ResourceBundleViewResolver，UrlBasedViewResolver，InternalResourceViewResolver，VelocityViewResolver/FreeMarkerViewResolver，
ContentNegotiatingViewResolver等。从字面意思我们大致就可以猜出起用途。
我们平时使用ResourceBundleViewResolver或者InternalResourceViewResolver来返回JSP页面，他们就是其中的两个 ViewResolver 
下面我主要说说ContentNegotiatingViewResolver 
根据官方文档：The ContentNegotiatingViewResolver does not resolve views itself but rather delegates to other view resolvers，就是说ContentNegotiatingViewResolver 本身并不自己去解析，他只是分配给其他的ViewResolver 去解析。并选择一个看起来像是客户端请求需要返回的一种  View  返回。
下面来看看我们想要返回的JSON格式的数据，spring3.0中提供了一种View 来支持 JSON，MappingJacksonJsonView  ，在这个View中我们可以封装数据，
属性等等，但是怎么让spring返回这个view呢，还是要通过 ViewResolver 来处理。
我们来看官方文档里的一份关于ContentNegotiatingViewResolver  的典型配置：
Java代码  ![收藏代码](http://image60.360doc.com/DownloadImg/2013/04/2814/31959471_1.png)
- <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">  
- <!-- 设置为true以忽略对Accept Header的支持-->
- 
 <property name="ignoreAcceptHeader" value="true"/>
<!-- 在没有扩展名时即: "http://www.sxrczx.com/rest" 时的默认展现形式 -->
 <property name="defaultContentType" value="text/html"/>
<!-- 扩展名至mimeType的映射,即 http://www.sxrczx.com/rest.json 映射为 application/json -->
-   <property name="*mediaTypes*">  
-     <map>  
-       <entry key="atom" value="application/atom+xml"/>  
-       <entry key="html" value="text/html"/>  
-       <entry key="json" value="application/json"/>  
-     </map>  
-   </property>
- <!-- 用于开启 http://www.sxrczx.com/rest?format=json 的支持 --><property name="favorParameter" value="false"/>  
-   <property name="*viewResolvers*">  
-     <list>  
-       <bean class="org.springframework.web.servlet.view.BeanNameViewResolver"/>  
-       <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">  
- <!--
-         <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
- -->
-         <property name="prefix" value="/WEB-INF/jsp/"/>  
-         <property name="suffix" value=".jsp"/>  
-       </bean>  
-     </list>  
-   </property>  
- 
-   <property name="*defaultViews*">  
-     <list>  
- <!-- for application/json -->
-       <bean class="org.springframework.web.servlet.view.json.MappingJacksonJsonView" />  
-       <!-- for application/xml --><!--  <bean class="org.springframework.web.servlet.view.xml.MarshallingView" >  <property name="marshaller">  <bean class="org.springframework.oxm.xstream.XStreamMarshaller"/>  </property>        </bean>-->
-     </list>  
-   </property>  
- </bean>  
- 
- <bean id="content"class="com.springsource.samples.rest.SampleContentAtomView"/>  
关于 mediaTypes 这个属性我稍后分析，先看viewResolvers和defaultViews这两个属性，viewResolvers中定义了两个 ViewResolver ，defaultViews定义了一个默认的返回视图。
但是ContentNegotiatingViewResolver  是怎么决定使用哪个ViewResolver 以及 返回什么样的 View呢？ 通过跟踪源码和查看API文档可以很容易发现。
API中写道：
This view resolver uses the requested media type to select a suitable View for a request. This media type is determined by using the following criteria:
1. If the requested path has a file extension and if the setFavorPathExtension(boolean) property is true, the mediaTypes property is inspected for a matching media type.
2. If the request contains a parameter defining the extension and if the setFavorParameter(boolean) property is true, the mediaTypes property is inspected for a matching media type. 
The default name of the parameter is format and it can be configured using the parameterName property.
3. If there is no match in the mediaTypes property and if the Java Activation Framework (JAF) is both enabled and present on the class path, FileTypeMap.getContentType(String) is used instead.
4. If the previous steps did not result in a media type, and ignoreAcceptHeader is false, the request Accept header is used.
Once the requested media type has been determined, this resolver queries each delegate view resolver for a View and determines if the requested media type is compatible with the view's content type). The most compatible view is returned.
1. spring检查`setFavorPathExtension(boolean) `，如果这个属性为true（默认为true），它检查请求的后缀名，来返回一种 mediaType ，
而后缀名和mediaType是通过ContentNegotiatingViewResolver  配置中的mediaTypes指定的，这个我开始也不确定，后来跟踪源码发现确实是这样映射的。
2.spring检查`setFavorParameter(boolean) ``这个属性是否为true（默认为false），而如果你打开这个属性，那么默认的参数名应为 format ，`
`spring通过你传过去的参数决定返回哪种mediaType。`
3.如果前两步没有找到合适的mediaType，则启动**机制去找，这个看不懂，也不用管了。
4.如果前三步都没有找到合适的mediaType，并且 `ignoreAcceptHeader ``这个属性为false（默认为false），spring则根据  你请求头里面设置的  ContentType 来找适合的 mediaType。`
那么现在我们明白了 ContentNegotiatingViewResolver   resolves a view based on the request file name or `Accept` header.  
 就是ContentNegotiatingViewResolver  根据文件名和请求头类型来决定返回什么样的View。而mediaTypes这个属性存储了 你请求后缀名 或者 参数 所对应 的mediaType。
所以要想返回JSON数据所代表的MappingJacksonJsonView   ，我们要么在请求头中设置contentType为application/json，要么使用 **.json   或者  **?format=json 
（这是我的猜测，我猜spring接收到format中的参数后也会去那个map中找）这种请求，其中json这个名字你可以任意换，只要在配置文件中统一就可以了。
下面是我项目中具体的使用：
XML中的配置：
Xml文件代码  ![收藏代码](http://image60.360doc.com/DownloadImg/2013/04/2814/31959471_1.png)
- <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">    
-     <property name="mediaTypes">    
-       <map>    
-         <entry key="html" value="text/html"/>    
-         <entry key="spring" value="text/html"/>  
-         <entry key="json" value="application/json"/>    
-       </map>    
-     </property>  
-     <property name="viewResolvers">    
-       <list>  
-         <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">    
-           <property name="prefix" value="/"/>  
-           <property name="suffix" value=".jsp"/>  
-         </bean>  
-       </list>  
-     </property>  
-     <property name="defaultViews">  
-         <list>  
-             <bean class="org.springframework.web.servlet.view.json.MappingJacksonJsonView"/>  
-         </list>  
-     </property>  
- </bean>  
前台调用：
Js代码  ![收藏代码](http://image60.360doc.com/DownloadImg/2013/04/2814/31959471_1.png)
- <script type="text/javascript">  
- $(function() {  
-     jQuery.ajax({  
-         url : 'index.json',  
-         contentType : "application/json",//application/xml
-         processData : true,//contentType为xml时，些值为false
-         dataType : "json",//json--返回json数据类型；xml--返回xml
-         data : {  
-             tag : 'tag123'
-         },  
-         success : function(data) {  
-             document.write(data.applyList.length);  
-         },  
-         error : function(e) {  
-             document.write('error');  
-         }  
-     });  
- });  
- </script>  
后台Controller：
Java代码  ![收藏代码](http://image60.360doc.com/DownloadImg/2013/04/2814/31959471_1.png)
- @RequestMapping(value = "/index.json")  
- public ModelAndView queryAppliesForJson() {  
-        ModelAndView mav = new ModelAndView("query_list_paginition");  
-     List<ChangeApply> applyList = changeApplyService.findAllApplies();  
-     mav.addObject("applyList", applyList);  
- return mav;  
- }  
这么前台JavaScript会接收到JSON字符串。 而且这样设计也符合spring提倡的 RESTful 风格。我们在任何地方只要发出对应的请求，服务器就会给我们返回需要的数据。
....陆续增加中，下次我可能会写从源码角度去分析。
