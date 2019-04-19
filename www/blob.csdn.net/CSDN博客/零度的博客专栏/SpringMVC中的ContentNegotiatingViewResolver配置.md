# SpringMVC中的ContentNegotiatingViewResolver配置 - 零度的博客专栏 - CSDN博客
2016年05月31日 16:31:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：6325
       RESTful服务中很重要的一个特性即是同一资源,多种表述.如 get put post delete head方式提交的请求，或者根据accept，参数，后缀等方式。**get、put、post、delete、head**等方式不介绍。
       Accept方式：
                 chrome:  Accept:application/xml,application/xhtml+xml,textml;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5 
                 firefox:  Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8 
                 IE8:  Accept:image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/x-silverlight, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, */*
        根据请求的accpet，来返回信息。由于浏览器的差异,发送上来的Accept Header头将是不一样的. 将导致服务器不知要返回什么格式的数据给你. @ResponseBody String返回类型，google chrome下会有问题就是这个原因。firefox IE6-7由于第一个accept是text/html，StringHttpMessageConverter处理的还是text/html。
        使用扩展名：
                /user/123.xml  将返回xml格式数据  
                /user/123.json 将返回json格式数据  
                /user/123.html 将返回html格式数据  
        丧失了同一url多种展现的方式。在rest架构中，user/123应该具有不同的展示。而/user/123.xml和/user/123.json则已经是不同的url了。
        使用参数：
                现在很多open API是使用这种方式,但可能由于要编写的字符较多（占用更多带宽）,所以较少使用.
        ContentNegotiatingViewResolver：这个类它实现了ViewResolver。但它并不直接解析视图，而是委托给别人。默认情况，它是从spring 上下文，查找视图解析器，并调用这些解析器。也可以在初始化这个bean的时候，设置它的解析器属性（viewResolvers），这是个list类型的属性。
请注意，要让这个视图解析器正常工作，需要设置比别人更高的优先级（默认为Ordered.HIGHEST_PRECEDENCE）。
配置的例子
Xml代码  ![收藏代码](http://liyixing1.iteye.com/images/icon_star.png)
- <!--  
-     根据客户端的不同的请求决定不同的view进行响应, 如 /rest/1.json /rest/1.xml /rest?format=json
-     /rest?format=xml
- -->
- <bean
- class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
- <!-- 设置为true以忽略对Accept Header的支持-->
- <propertyname="ignoreAcceptHeader"value="true"/>
- <!-- true，开启扩展名支持，false关闭支持 -->
- <propertyname="favorPathExtension"value="false"/>
- <!-- 用于开启 /userinfo/123?format=json的支持 -->
- <propertyname="favorParameter"value="true"/>
- <!--
-         在没有扩展名和参数时即: "/user/1" 时的默认展现形式
-     -->
- <propertyname="defaultContentType"value="text/html"/>
- 
-     <!--  
-         参数值至mimeType的映射,即 /rest?format=json json是key，application/json就是value  
-         暂时只支持json和xml  
-     -->
- <propertyname="mediaTypes">
- <map>
- <entrykey="json"value="application/json"/>
- <entrykey="xml"value="application/xml"/>
- </map>
- </property>
- 
- <propertyname="viewResolvers">
- <!-- 关闭所有的解析器，防止它在查找候选视图时多个解析器都运行 -->
- <list></list>
- </property>
- 
- <propertyname="defaultViews">
- <list>
- <!-- for application/json -->
- <bean
- class="org.springframework.web.servlet.view.json.MappingJacksonJsonView">
- <!--                 <property name="renderedAttributes">-->
- <!--                     <set>-->
- <!--                         <value>result</value>-->
- <!--                         <value>user</value>-->
- <!--                         <value>page</value>-->
- <!--                     </set>-->
- <!--                 </property>-->
- <!-- 重新设置 objectMapper-->
- <propertyname="objectMapper">
- <beanclass="org.codehaus.jackson.map.ObjectMapper">
- <!--
-                             设置objectMapper的serializationConfig的serializationInclusion属性，以忽略null对象
-                         -->
- <propertyname="serializationConfig.serializationInclusion">
- <value
- type="org.codehaus.jackson.map.annotate.JsonSerialize.Inclusion">NON_NULL</value>
- </property>
- </bean>
- </property>
- </bean>
- <!-- for application/xml -->
- <beanclass="org.springframework.web.servlet.view.xml.MarshallingView">
- <propertyname="marshaller">
- <beanclass="org.springframework.oxm.castor.CastorMarshaller">
- <propertyname="validating"value="false"></property>
- </bean>
- </property>
- </bean>
- </list>
- </property>
- </bean>
这个视图解析器根据请求类型来返回视图。就是上面说的三种方式。
1.如果设置了setFavorPathExtension(boolean)为true，会根据后缀来使用不同的数据格式。
2.如果设置了setFavorParameter(boolean)为true，会根据请求参数来设置不同的数据格式。参数名是通过属性parameterName设置的，默认private String parameterName = "format";
3.如果ignoreAcceptHeader未设置为false（未关闭），那么会通过accpet 来获得相应的信息。
在其内部中，是通过
protected List<MediaType> getMediaTypes(HttpServletRequest request) {
方法来根据请求过来的信息，返回对应的需要的回应的context-type的。首先判断的是扩展名是否开启，
if (this.favorPathExtension) {
如果开启，先获取扩展名，如果扩展名不为null，
查看是否有可以处理的扩展，这个是将扩展名作为key，来获取的
MediaType mediaType = this.mediaTypes.get(extension);
这里的mediaTypes就是刚才的配置
Xml代码  ![收藏代码](http://liyixing1.iteye.com/images/icon_star.png)
- <propertyname="mediaTypes">
- <map>
- <entrykey="json"value="application/json"/>
- <entrykey="xml"value="application/xml"/>
- </map>
- </property>
设置的值。
如果我们访问的是/127.0.0.1/daowole/rest.xml
扩展名就是xml，这个key对应的value就是application/xml。
有则返回媒体类型。扩展名得方式还会判断useJaf是否开启。开启的话会使用jaf（Java Activation Framework）来获取，也就是通过   
String mediaType = fileTypeMap.getContentType(fileName);来获取媒体类型。这里的fileTypeMap是一个静态常量。它里面建立了一张还算完整的根据文件名字的后缀，映射的媒体表格。
如(其中的一部分）
c++=MIMETypeEntry: text/plain, 
bcpio=MIMETypeEntry: application/x-bcpio,
xwd=MIMETypeEntry: image/x-xwindowdump,
如果是从jaf取出来的，会自动往mediaTypes添加内容。
if (mediaType != null) {
this.mediaTypes.putIfAbsent(extension, mediaType);
}
否则进入下一个处理方式，
下一个处理方式是参数，判断favorParameter是否开启。处理类似
通过参数。只是将参数值，作为key从mediaTypes获取。
然后然后是accept方式，判断ignoreAcceptHeader是否开启。处理类似，但不需要从mediaTypes获取信息。
如果没有，查看defaultContentType是否为空，返回的是defaultContentType
否则返回return Collections.emptyList();一个空的内容。
其次根据返回的
List<MediaType> requestedMediaTypes，controller返回的viewname，以及locale，来生成多个候选的View（List<View>）
List<View> candidateViews = getCandidateViews(viewName, locale, requestedMediaTypes);
/**发现候选的方式是遍历它的viewResolvers属性（这个类在构造的时候，会从spring上下文把所有的解析器取出来，当然我们也可以通过bean初始化的property元素设置它的viewResolvers属性），将每个viewResolver拿出来，调用它的resolveViewName，来获取view，如果view有值，表示这个视图解析器可以处理（可以处理不表示只有这一个是候选的，候选可能是多个的）。这个时候还没结束*/
Java代码  ![收藏代码](http://liyixing1.iteye.com/images/icon_star.png)
- for (ViewResolver viewResolver : this.viewResolvers) {  
-             View view = viewResolver.resolveViewName(viewName, locale);  
- if (view != null) {  
- //不为null，添加到候选视图
-                 candidateViews.add(view);  
-             }  
- 
- //这里还需要遍历需要返回给客户端的媒体类型
- for (MediaType requestedMediaType : requestedMediaTypes) {  
- //根据媒体类型，返回扩展名，List<String> extensions = getExtensionsForMediaType(requestedMediaType);这部做的是和通过key获取value方式反了一下，而是通过value获取key了，也就是通过媒体，获取这个媒体在mediaType中的key，这里返回的key可能是多个，也就是说一种媒体可以对应各种文件格式，如json可以对应application/json text/json。
- 
-                 List<String> extensions = getExtensionsForMediaType(requestedMediaType);  
- 
- //遍历这个媒体，通过String viewNameWithExtension = viewName + "." + extension;操作后，生成新的逻辑视图，并查看这个逻辑是否也可以在当前的视图解析器处理出一个物理视图。如我们的action是/daowole/test.json 本来我们controller返回的逻辑视图是 order/addsuccess，它对应的物理视图是 order/addsuccess.ftl，而这个时候会增加逻辑视图， order/addsuccess.json 那么对应的物理视图就是 order/addsuccess.json.ftl
- for (String extension : extensions) {  
-                     String viewNameWithExtension = viewName + "." + extension;  
-                     view = viewResolver.resolveViewName(viewNameWithExtension, locale);  
- //可以处理，将它也加入到候选视图。
- if (view != null) {  
-                         candidateViews.add(view);  
-                     }  
-                 }  
- 
-             }  
-         }  
- 
- 
- if (!CollectionUtils.isEmpty(this.defaultViews)) {  
- //这里会把我们在上面设置的默认视图（视图不是视图解析器）也加入进去。我们这里设置的默认视图只有一个json
-             candidateViews.addAll(this.defaultViews);  
-         }  
- 当然这里的视图比较特殊，它不需要逻辑视图名，就可以直接返回responsebody了。  
- 
- 随后需要从候选视图查找一个唯一可以处理的视图，也就是  
- View bestView = getBestView(candidateViews, requestedMediaTypes);  
- 
- 代码如下  
- MediaType bestRequestedMediaType = null;  
-         View bestView = null;  
- //首先是遍历需要回应给客户端的媒体信息
- for (MediaType requestedMediaType : requestedMediaTypes) {  
- //这里是遍历每个候选视图
- for (View candidateView : candidateViews) {  
- if (StringUtils.hasText(candidateView.getContentType())) {  
- //获取当前候选时候的媒体信息
-                     MediaType candidateContentType = MediaType.parseMediaType(candidateView.getContentType());  
- //判断这个候选视图媒体是否在位于需要回应的媒体中，至于怎么判断是否位于这个媒体中我们不做详细介绍，但候选视图的媒体刚好包含于回应视图中的，那么
- if (requestedMediaType.includes(candidateContentType)) {  
-                         bestRequestedMediaType = requestedMediaType;  
- //baseView就确定了。
-                         bestView = candidateView;  
- break;  
-                     }  
-                 }  
-             }  
- if (bestView != null) {  
- if (logger.isDebugEnabled()) {  
-                     logger.debug(  
- "Returning [" + bestView + "] based on requested media type '" + bestRequestedMediaType +  
- "'");  
-                 }  
- break;  
-             }  
-         }  
- return bestView;  
- 
- 这里返回后，也就是  
- View bestView = getBestView(candidateViews, requestedMediaTypes);执行后  
- 
- if (bestView != null) {  
- return bestView;  
-         }  
- 
- else {  
- //这里生成是一个404
- if (this.useNotAcceptableStatusCode) {  
- if (logger.isDebugEnabled()) {  
-                     logger.debug("No acceptable view found; returning 406 (Not Acceptable) status code");  
-                 }  
- return NOT_ACCEPTABLE_VIEW;  
-             }  
- else {  
- if (logger.isDebugEnabled()) {  
-                     logger.debug("No acceptable view found; returning null");  
-                 }  
- //任何都不返回，这个useNotAcceptableStatusCode设置的时候要注意了，如果为true，而且没有找到任何可以处理的view，那么就会404，否则，它就return null。当spring mvc 的servlet接受到的view为null，那么就会交给下一个视图解析器处理，那么原来的解析器就依然可以起到效果了。这种情况是我们大多数应用所希望的。所以spring mvc在设计这个类得时候把它的默认值设置为false。
- returnnull;  
-             }  
-         }  
