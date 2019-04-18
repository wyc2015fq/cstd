# Spring MVC 文件下载过程中 出现下载文件乱码 - z69183787的专栏 - CSDN博客
2018年06月30日 20:56:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：154

Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @RequestMapping("download")  
- publicvoid download(HttpServletResponse res) throws IOException {  
-     OutputStream os = res.getOutputStream();  
- try {  
-         res.reset();  
-         res.setHeader("Content-Disposition", "attachment; filename=dict.txt");  
-         res.setContentType("application/octet-stream; charset=utf-8");  
-         os.write(FileUtils.readFileToByteArray(getDictionaryFile()));  
-         os.flush();  
-     } finally {  
- if (os != null) {  
-             os.close();  
-         }  
-     }  
- }  
因为鄙人强烈的精神洁癖，心中大骂 
“这样的狗屁代码也贴在网上？” 
既然使用了mvc，怎么还能暴露HttpServletResponse这样的j2ee接口出来！ 
我相信spring提供了更好的方式，于是开始翻阅文档，得出如下代码 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @RequestMapping("download")  
- public ResponseEntity<byte[]> download() throws IOException {  
-     HttpHeaders headers = new HttpHeaders();  
-     headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);  
-     headers.setContentDispositionFormData("attachment", "dict.txt");  
- returnnew ResponseEntity<byte[]>(FileUtils.readFileToByteArray(getDictionaryFile()),  
-                                       headers, HttpStatus.CREATED);  
- }  
理论上没问题，实现上很优雅 
但是文件下载后内容如下 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- "YWEJMQ0KdnYJMg0KaGgJMw=="
正确内容为 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- aa  1
- vv  2
- hh  3
我把代码改为 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- ResponseEntity<String>  
输出如下 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- "aa    1\n\tvv     2\n\thh  3"
相信很多人看到这已经知道了发生了什么 
但是因为本人狗屎一样的基础知识，又浪费了几小时 
先去看了ByteArrayHttpMessageConverter的源码 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public ByteArrayHttpMessageConverter() {  
- super(new MediaType("application", "octet-stream"), MediaType.ALL);  
- }  
-        ...  
- protectedvoid writeInternal(byte[] bytes, HttpOutputMessage outputMessage) throws IOException {  
-     FileCopyUtils.copy(bytes, outputMessage.getBody());  
- }  
没感觉到有任何问题 
捉耳挠腮了一阵子，又去看AnnotationMethodHandlerAdapter 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public AnnotationMethodHandlerAdapter() {  
- // no restriction of HTTP methods by default
- super(false);  
- 
- // See SPR-7316
- StringHttpMessageConverter stringHttpMessageConverter = new StringHttpMessageConverter();  
- stringHttpMessageConverter.setWriteAcceptCharset(false);  
- //这里有默认值
- this.messageConverters = new HttpMessageConverter[]{new ByteArrayHttpMessageConverter(), stringHttpMessageConverter,  
- new SourceHttpMessageConverter(), new XmlAwareFormHttpMessageConverter()};  
- 
- 
- //set时默认值全被取消了
- publicvoid setMessageConverters(HttpMessageConverter<?>[] messageConverters) {  
- this.messageConverters = messageConverters;  
再去看MappingJacksonHttpMessageConverter 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- extends AbstractHttpMessageConverter<Object>//Object是万用的，就想Exception类型，优先级别应该最低
突然有一种自己是个傻 逼的感觉，浪费了大概3、4个小时 
修改xml 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">  
-     <property name="messageConverters">  
-         <list>  
- //把ByteArray加在Json前面
-             <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>  
-             <bean id="jsonHttpMessageConverter"class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter" >  
-                 <property name = "supportedMediaTypes">  
-                     <list>  
-                         <value>text/plain;charset=UTF-8</value>  
-                     </list>  
-                 </property>  
-             </bean>  
-         </list>  
-     </property>  
- </bean>  
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @RequestMapping("download")  
- publicvoid download(HttpServletResponse res) throws IOException {  
-     OutputStream os = res.getOutputStream();  
- try {  
-         res.reset();  
-         res.setHeader("Content-Disposition", "attachment; filename=dict.txt");  
-         res.setContentType("application/octet-stream; charset=utf-8");  
-         os.write(FileUtils.readFileToByteArray(getDictionaryFile()));  
-         os.flush();  
-     } finally {  
- if (os != null) {  
-             os.close();  
-         }  
-     }  
- }  
因为鄙人强烈的精神洁癖，心中大骂 
“这样的狗屁代码也贴在网上？” 
既然使用了mvc，怎么还能暴露HttpServletResponse这样的j2ee接口出来！ 
我相信spring提供了更好的方式，于是开始翻阅文档，得出如下代码 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @RequestMapping("download")  
- public ResponseEntity<byte[]> download() throws IOException {  
-     HttpHeaders headers = new HttpHeaders();  
-     headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);  
-     headers.setContentDispositionFormData("attachment", "dict.txt");  
- returnnew ResponseEntity<byte[]>(FileUtils.readFileToByteArray(getDictionaryFile()),  
-                                       headers, HttpStatus.CREATED);  
- }  
理论上没问题，实现上很优雅 
但是文件下载后内容如下 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- "YWEJMQ0KdnYJMg0KaGgJMw=="
正确内容为 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- aa  1
- vv  2
- hh  3
我把代码改为 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- ResponseEntity<String>  
输出如下 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- "aa    1\n\tvv     2\n\thh  3"
相信很多人看到这已经知道了发生了什么 
但是因为本人狗屎一样的基础知识，又浪费了几小时 
先去看了ByteArrayHttpMessageConverter的源码 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public ByteArrayHttpMessageConverter() {  
- super(new MediaType("application", "octet-stream"), MediaType.ALL);  
- }  
-        ...  
- protectedvoid writeInternal(byte[] bytes, HttpOutputMessage outputMessage) throws IOException {  
-     FileCopyUtils.copy(bytes, outputMessage.getBody());  
- }  
没感觉到有任何问题 
捉耳挠腮了一阵子，又去看AnnotationMethodHandlerAdapter 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- public AnnotationMethodHandlerAdapter() {  
- // no restriction of HTTP methods by default
- super(false);  
- 
- // See SPR-7316
- StringHttpMessageConverter stringHttpMessageConverter = new StringHttpMessageConverter();  
- stringHttpMessageConverter.setWriteAcceptCharset(false);  
- //这里有默认值
- this.messageConverters = new HttpMessageConverter[]{new ByteArrayHttpMessageConverter(), stringHttpMessageConverter,  
- new SourceHttpMessageConverter(), new XmlAwareFormHttpMessageConverter()};  
- 
- 
- //set时默认值全被取消了
- publicvoid setMessageConverters(HttpMessageConverter<?>[] messageConverters) {  
- this.messageConverters = messageConverters;  
再去看MappingJacksonHttpMessageConverter 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- extends AbstractHttpMessageConverter<Object>//Object是万用的，就想Exception类型，优先级别应该最低
突然有一种自己是个傻 逼的感觉，浪费了大概3、4个小时 
修改xml 
Java代码    ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">  
-     <property name="messageConverters">  
-         <list>  
- //把ByteArray加在Json前面
-             <bean class="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>  
-             <bean id="jsonHttpMessageConverter"class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter" >  
-                 <property name = "supportedMediaTypes">  
-                     <list>  
-                         <value>text/plain;charset=UTF-8</value>  
-                     </list>  
-                 </property>  
-             </bean>  
-         </list>  
-     </property>  
- </bean>  
