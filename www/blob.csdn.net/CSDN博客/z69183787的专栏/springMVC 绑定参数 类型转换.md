# springMVC 绑定参数 类型转换 - z69183787的专栏 - CSDN博客
2015年01月06日 21:01:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6991
一、使用注解式控制器注册PropertyEditor(针对具体的controller类处理)
1、使用WebDataBinder进行控制器级别的注册PropertyEditor(控制器独享)
Java代码  ![收藏代码](http://hbiao68.iteye.com/images/icon_star.png)
- @InitBinder
- // 此处的参数也可以是ServletRequestDataBinder类型
- publicvoid initBinder(WebDataBinder binder) throws Exception {  
- // 注册自定义的属性编辑器
- // 1、日期
-     DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
- //CustomDateEditor类是系统内部自带的类
-     CustomDateEditor dateEditor = new CustomDateEditor(df, true);  
- // 表示如果命令对象有Date类型的属性，将使用该属性编辑器进行类型转换
-     binder.registerCustomEditor(Date.class, dateEditor);  
- // 自定义的电话号码编辑器(和【4.16.1、数据类型转换】一样)
-     binder.registerCustomEditor(PhoneNumberModel.class, new PhoneNumberEditor());  
- }  
 备注：转换对象必须要实现PropertyEditor接口，例如CustomDateEditor类
Java代码  ![收藏代码](http://hbiao68.iteye.com/images/icon_star.png)
- package org.springframework.beans.propertyeditors;  
- 
- import java.beans.PropertyEditorSupport;  
- import java.text.DateFormat;  
- import java.text.ParseException;  
- import java.util.Date;  
- import org.springframework.util.StringUtils;  
- 
- publicclass CustomDateEditor extends PropertyEditorSupport {  
- 
- privatefinal DateFormat dateFormat;  
- privatefinalboolean allowEmpty;  
- privatefinalint exactDateLength;  
- 
- public CustomDateEditor(DateFormat dateFormat, boolean allowEmpty) {  
- this.dateFormat = dateFormat;  
- this.allowEmpty = allowEmpty;  
-         exactDateLength = -1;  
-     }  
- 
- public CustomDateEditor(DateFormat dateFormat, boolean allowEmpty, int exactDateLength) {  
- this.dateFormat = dateFormat;  
- this.allowEmpty = allowEmpty;  
- this.exactDateLength = exactDateLength;  
-     }  
- 
- publicvoid setAsText(String text) throws IllegalArgumentException {  
- if (allowEmpty && !StringUtils.hasText(text)) {  
-             setValue(null);  
-         } else {  
- if (text != null && exactDateLength >= 0 && text.length() != exactDateLength)  
- thrownew IllegalArgumentException((new StringBuilder("Could not parse date: it is not exactly")).append(exactDateLength).append("characters long").toString());  
- try {  
-                 setValue(dateFormat.parse(text));  
-             } catch (ParseException ex) {  
- thrownew IllegalArgumentException((new StringBuilder("Could not parse date: ")).append(ex.getMessage()).toString(), ex);  
-             }  
-         }  
-     }  
- 
- public String getAsText() {  
-         Date value = (Date) getValue();  
- return value == null ? "" : dateFormat.format(value);  
-     }  
- 
- }  
2、使用xml配置实现类型转换(系统全局转换器)
（1、注册ConversionService实现和自定义的类型转换器 
Xml代码  ![收藏代码](http://hbiao68.iteye.com/images/icon_star.png)
- <!-- ①注册ConversionService -->
- <beanid="conversionService"class="org.springframework.format.support.FormattingConversionServiceFactoryBean">
- <propertyname="converters">
- <list>
- <beanclass="hb.base.convert.DateConverter">
- <constructor-argvalue="yyyy-MM-dd"/>
- </bean>
- </list>
- </property>
-     <!-- 格式化显示的配置  
- <propertyname="formatters">
- <list>
- <beanclass="hb.base.convert.DateFormatter">
- <constructor-argvalue="yyyy-MM-dd"/>
- </bean>
- </list>
- </property>
- -->
- </bean>
（2、使用 ConfigurableWebBindingInitializer 注册conversionService
Xml代码  ![收藏代码](http://hbiao68.iteye.com/images/icon_star.png)
- <!-- ②使用 ConfigurableWebBindingInitializer 注册conversionService -->
- <beanid="webBindingInitializer"
- class="org.springframework.web.bind.support.ConfigurableWebBindingInitializer">
- <propertyname="conversionService"ref="conversionService"/>
- <propertyname="validator"ref="validator"/>
- </bean>
（3、注册ConfigurableWebBindingInitializer 到RequestMappingHandlerAdapter
Xml代码  ![收藏代码](http://hbiao68.iteye.com/images/icon_star.png)
- <!--Spring3.1开始的注解 HandlerAdapter -->
- <bean
- class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
- <!--线程安全的访问session -->
- <propertyname="synchronizeOnSession"value="true"/>
- <propertyname="webBindingInitializer"ref="webBindingInitializer"/>
- </bean>
此时可能有人会问，如果我同时使用 PropertyEditor 和 ConversionService，执行顺序是什么呢？内部首先查找PropertyEditor 进行类型转换，如果没有找到相应的 PropertyEditor 再通过 ConversionService进行转换。 
