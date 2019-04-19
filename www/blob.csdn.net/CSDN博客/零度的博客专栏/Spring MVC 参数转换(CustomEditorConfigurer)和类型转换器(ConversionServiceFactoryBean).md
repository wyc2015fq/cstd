# Spring MVC 参数转换(CustomEditorConfigurer)和类型转换器(ConversionServiceFactoryBean) - 零度的博客专栏 - CSDN博客
2017年02月27日 15:06:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：574
                
问题:
现在这样一种需求, 有一个bean它的属性是java.util.Date类型,我们要在spring的xml配置初始化它,怎么做呢
## 解决方案:
可以说spring的属性编辑器和类型转换器都是做类型转换的，但属性编辑器多为string转其它类型,
### 方法1：
添加属性编辑器:
Servlet中的输入参数为都是string类型，而spring mvc通过data bind机制将这些string 类型的输入参数转换为相应的command object（根据view和controller之间传输数据的具体逻辑，也可称为model
 attributes, domain model objects）。在这个转换过程中，spring实际是先利用java.beans.PropertyEditor中的 setAdText方法来把string格式的输入转换为bean属性， 
亦可通过继承java.beans.PropertyEditorSupport来实现自定义的PropertyEditors,具体实现方式可参考spring reference 3.0.5 第 5.4节中的 Registering additional custom
 PropertyEditors部分。 
自定义完毕propertyEditor后，有以下几种方式来注册自定义的customer propertyEditor. 
1:直接将自定义的propertyEditor放到需要处理的java bean相同的目录下
名称和java Bean相同但后面带Editor后缀。 
例如需要转换的java bean 名为User,则在相同的包中存在UserEditor类可实现customer propertyEditor的自动注册。 
2:利用@InitBinder来注册customer propertyEditor
这个在之前的笔记中已经介绍过了，即在controller类中增加一个使用@InitBinder标注的方法，在其中注册customer Editor 
Java代码  [](http://starscream.iteye.com/blog/1072179)
- @InitBinder
- publicvoid initBinder(WebDataBinder binder) {  
- 
    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
- 
    dateFormat.setLenient(false);  
- 
    binder.registerCustomEditor(Date.class, new CustomDateEditor(  
- 
            dateFormat, false));  
- 
}  
3:继承 WebBindingInitializer 接口来实现全局注册
使用@InitBinder只能对特定的controller类生效，为注册一个全局的customer Editor，可以实现接口WebBindingInitializer 。 
Java代码  [](http://starscream.iteye.com/blog/1072179)
- publicclass CustomerBinding implements WebBindingInitializer {  
- @Override
- publicvoid initBinder(WebDataBinder binder, WebRequest request) {  
- 
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
- 
        dateFormat.setLenient(false);  
- 
        binder.registerCustomEditor(Date.class, new CustomDateEditor(  
- 
                dateFormat, false));  
- 
- 
    }  
并修改 servlet context xml配置文件 
Xml代码  [](http://starscream.iteye.com/blog/1072179)
- **<****bean**
- class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter"**>**
- **<****property**name="webBindingInitializer"**>**
- **<****bean**
- class="net.zhepu.web.customerBinding.CustomerBinding"**/>**
- **</****property****>**
- **</****bean****>**
但这样一来就无法使用mvc:annotation-driven  了。 
### 方法二:
注册类型转换器
###  Configuring a ConversionService
使用conversion-service来注册自定义的converter
DataBinder实现了PropertyEditorRegistry, TypeConverter这两个interface，而在spring mvc实际处理时，返回值都是return binder.convertIfNecessary(见HandlerMethodInvoker中的具体处理逻辑)。因此可以使用customer
 conversionService来实现自定义的类型转换。 
Xml代码  [](http://starscream.iteye.com/blog/1072179)
- **<****bean**id="conversionService"
- class="org.springframework.format.support.FormattingConversionServiceFactoryBean"**>**
- 
- **<****property**name="converters"**>**
- **<****list****>**
- **<****bean**class="net.zhepu.web.customerBinding.CustomerConverter"**/>**
- **</****list****>**
- **</****property****>**
- 
- 
lt;/bean**>**
需要修改spring service context xml配置文件中的annotation-driven,增加属性conversion-service指向新增的conversionService bean。 
Xml代码  [](http://starscream.iteye.com/blog/1072179)
- **<****mvc:annotation-driven**validator="validator"
- conversion-service="conversionService"**/>**
实际自定义的converter如下。 
Java代码  [](http://starscream.iteye.com/blog/1072179)
- publicclass CustomerConverter implements Converter<String, Date> {  
- @Override
- public Date convert(String source) {  
- 
    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
- 
    dateFormat.setLenient(false);  
- try {  
- return dateFormat.parse(source);  
- 
    } catch (ParseException e) {  
- 
        // TODO Auto-generated catch block  
- 
        e.printStackTrace();  
- 
    }         
- returnnull;  
- 
}  
对于requestBody或httpEntity中数据的类型转换
Spring MVC中对于requestBody中发送的数据转换不是通过databind来实现，而是使用HttpMessageConverter来实现具体的类型转换。 
例如，之前提到的json格式的输入，在将json格式的输入转换为具体的model的过程中，spring mvc首先找出request header中的contenttype，再遍历当前所注册的所有的HttpMessageConverter子类， 根据子类中的canRead()方法来决定调用哪个具体的子类来实现对requestBody中的数据的解析。如果当前所注册的httpMessageConverter中都无法解析对应contexttype类型，则抛出HttpMediaTypeNotSupportedException 
 （http 415错误）。 
那么需要如何注册自定义的messageConverter呢，很不幸，在spring 3.0.5中如果使用annotation-driven的配置方式的话，无法实现自定义的messageConverter的配置，必须老老实实的自己定义AnnotationMethodHandlerAdapter的bean定义，再设置其messageConverters以注册自定义的messageConverter。 
在3.1版本中，将增加annotation-driven对自定义的messageConverter的支持 (SPR-7504)，具体格式如下 
Xml代码  [](http://starscream.iteye.com/blog/1072179)
- **<****mvc:annotation-driven****>**
- **<****mvc:message-converters****>**
- **<****bean**class="org.springframework.http.converter.StringHttpMessageConverter"**/>**
- **<****bean**class="org.springframework.http.converter.ResourceHttpMessageConverter"**/>**
- **<****bean**class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter"**/>**
- **</****mvc:message-converters****>**
- **</****mvc:annotation-driven****>**
