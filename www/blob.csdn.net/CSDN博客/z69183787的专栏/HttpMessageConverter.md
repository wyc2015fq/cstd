# HttpMessageConverter - z69183787的专栏 - CSDN博客
2016年08月20日 14:22:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2294
使用HttpMessageConverter<T>
HttpMessageConverter<T>接口负责将请求信息转换成T类型对象，将对象输出为响应信息。
DispatcherServlet默认安装了AnnotationMethodHandlerAdapter做为HandlerAdapter的组件实现类，HttpMessageConverter即由AnnotationMethodHandlerAdapter使用，将请求信息转换成对象，或将对象转换成响应信息。
HttpMessageConverter接口方法
Boolean canRead(Class<?> clazz,MediaTypemediaType)指定转换器可以读取的对象类型，同时指定支持MIME类型。
Boolean canWriter(Class<?> clazz,MediaTypemediaType)指定转换器可以将clazz类型的对象写到响应流中，响应流支持类型在mediaType中定义
List<MediaType> getSupportMediaTypes()该转换器支持的媒体类型
T read(class<? extends T>clazz,HttpInputMessageinputMessage)将请求信息流转换成T类型的对象
void write(T t,MediaType contentType,HttpOutputMessageoutputMessage)将T类型的对象写到响应流中，同时指定响应媒体类型为contentType
HttpMessageConverter实现类
StringHttpMessageConverter:将请求信息转换为字符串，T为String，可读取所有媒体类型(*/*),可通过supportedMediaTypes属性指定媒体类型
ByteArrayHttpMessageConverter：读写二进制数据，T为byte[]类型，可读取*/*，可通过supportedMediaTypes属性指定媒体类型，响应信息媒体类型为application/octer-stream
MarshallingHttpMessageConverter：T为Object，可读取text/xml和application/xml媒体类型请求，响应信息的媒体类型为text/xml或application/xml
Jaxb2RootElementHttpMessageConverter：通过JAXB2读写XML信息，将请求消息转换到标注XmlRootElement和XmlType注解的类中，T为Object，可读取text/xml和application/xml媒体类型请求，响应信息的媒体类型为text/xml或application/xml
MappingJacksonHttpMessageConverter：利用jackson的ObjectMapper读写Json数据，T为Object，可读取application/json，响应媒体类型为application/json
FormHttpMessageConverter/XmlAwareFormHttpMessageConverter/ResourceHttpConverterMessage/BufferedImageHttpMessageConverter/SourceHttpMessageConverter/RssChannelHttpMessageConverter/AtomFeedHttpMessageConverter
AnnotationMethodHandlerAdapter默认已安装一下HttpMessageConverter
StringHttpMessageConverter、ByteArrayHttpMessageConverter、sourceHttpMessageConverter、XmlAwareFormHttpMessageConverter，如需要装配其它类型HttpMessageConverter，可在spring的web容器上下文中自定义AnnotationMethodHandlerAdapter
如：
<beanclass="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter"”>
<propertyname="messageConverters" ref="messageConverters"/>
</bean>
<util:list>
<beanclass="org.springframework.http.converter.BufferedImageHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.StringHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.xml.XmlAwareFormHttpMessageConverter"/>
</util:list>
如果在sring中显示定义了一个AnnotationMethodHandlerAdapter，则spring mvc将使用它覆盖默认的AnnotationMethodHandlerAdapter
使用@RequestBody/@ResponseBody对方法进行标注
使用HttpEntity<T>/ResponseEntity<T>作为处理方法的入参或返回值
实例
@RequestMapping(“/handle41”)
public String handler41(@RequestBody StringrequestBody){
         System.out.pringln(requestBody);
         returnSUCCESS;
}
@ResponseBody
@RequestMapping(“handle42/{imageId}”)
public byte[] handle42(@PathVariable(“imageId”)StringimageId)throw IOException{
         Resourcerec = new ClassPathResource(“/image.jpg”);
         byte[]fileData = FileCopyUtils.copyToByteArray(rec.getInputStream);
         returnfileData;
}
说明：
通过自定义AnnotationMethodHandlerAdapter注册若干个HttpMessageConverter，handler41方法的requestBody标注了@RequestBody，spring mvc 将根据requestBody的类型查找匹配的HttpMessageConverter，由于StringHttpMessageConverter的泛型支持String，StringHttpMessageConverter会被spring mvc选中，用它将请求体信息进行转换并将结果绑定到requestBody入参上
handle42()方法拥有@ResponseBody 注解，方法的返回值类型为byte[]，spring mvc会选中ByteArrayHttpMessageConverter 对返回值进行处理，即将图片数据流输出到客户端
和@RequestBody/@ResponseBody类似，spring mvc根据HttpEntity的泛型类型查找对应的HttpMessageConverter
@RequestMapping(“/handle43”)
pubic String handle43(HttpEntity<String> httpEntity){
         long contentLen = httpEntity.getHeaders().getContentLength();
         return SUCCESS;
}
当控制器处理方法使用到@RequestBody/@ResponseBody或HttpEntity<T>/ReponseEntity时，spring mvc使用注册的HttpMessageConverter对请求/响应信息进行处理，spring会根据请求头或响应头的Accepter属性匹配HttpMessageConverter，进而根据参数类型或泛型类型过滤得到匹配的HttpMessageConverter，如找不到则报错
处理XML和Json
Spring mvc提供
MarshallingHttpMessageConverter、
Jaxb2RootElementHttpMessageConverter(JAXB)、
MappingJacksonHttpMessageConverter(Jackson)来处理XML/JSON格式
实例：
<beanclass="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter"”>
<propertyname="messageConverters" ref="messageConverters"/>
</bean>
<util:list>
<beanclass="org.springframework.http.converter.BufferedImageHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.ByteArrayHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.StringHttpMessageConverter"/>
<bean
class="org.springframework.http.converter.xml.XmlAwareFormHttpMessageConverter"/>
<beanclass="org.springframework.http.converter.xml.MarshallingHttpMessageConverter">
         p:marshaller-ref=”xmlMarshaller”
         p:unmarshaller-ref=”xmlMarshaller”
</bean>
<beanclass="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"/>
</util:list>
<bean class="xmlMarshaller"class="org.springframework.oxm.xstream.XStreamMarshaller">
<propertyname="streamDriver">
<beanclass="com.thoughtworks.xstream.io.xml.StaxDriver"/>
</property>
<property name="annotatedClasses">
<list>com.xxx.User</list>
</property>
</bean>
装配好处理XML和JSON的HttpMessageConverter后，在控制器
@RequestMapping(value=”handle51”)
pubic ResponseEntity<User> handle51(HttpEntity<User> requestEntity){
         User user =requestEntity.getBody();
         user.setUserId(1000);
         return newResponseEntity<User>(user,HttpStatus.OK);
}
客户端通过设置Content-Type及Accept属性，服务端会自动转换
