# Stripes视图框架与Struts2之间的比较 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月06日 16:54:58[boonya](https://me.csdn.net/boonya)阅读数：2867








     Stripes视图框架与Struts2的行为很相似，所以在这里做一个简要的对比，发现它们的优缺点，以便在实际项目中合理的选择。

    原文来自：[http://www.stripesframework.org/display/stripes/Stripes+vs.+Struts2](http://www.stripesframework.org/display/stripes/Stripes+vs.+Struts2)

这里有一些Stripes和Struts2快速进行比对的一些指标：




||Stripes|Struts2|
|----|----|----|
|参照版本|1.5|2.0.12|
|配置文件|web.xml,|web.xml, struts.xml, optionally struts.properties  ...|
|主要核心|类需实现 ActionBean接口|类中需有execute()方法，选择实现Action类或继承ActionSupport类|
|响应机制|Resolution的实例|一个在struts.xml 或在注解中对应的一个String标识|
|视图技术|JSP，FreeMarker|JSP,，FreeMarker,，Velocity|
|布局机制|内置三种布局标签，它们可以被那些喜欢分层和表格的人很好的使用|提供分层和表格|
|绑定机制|内置|OGNL|
|输入验证|使用@Validate 和@ValidateNestedProperties实现|需要XML配置文件或注解来实现|
|快速验证|内置可配置的ValidationState.ALWAYS 和Validation.InvokeValidateWhenErrorsExist|在 <field-validator>标签上设置属性： short-circuit="true"|
|自定义验证|用 @ValidationMethod注解方法|扩展要么ValidatorSupport或FieldValidatorSupport，需配置validators.xml|
|模型视图传输|使用${actionBean} 获取属性|使用值栈（ValueStack）|
|类型转换|实现TypeConverter<T>（泛型）|实现OGNL，类型转换通常是扩展StrutsTypeConverter（非泛型）|
|格式化|实现 Formatter<T> （泛型）|实现OGNL，类型转换通常是扩展StrutsTypeConverter（非泛型）|
|自定义模块配置|自动加载Extension.Packages初始化参数|在struts.xml中配置|
|拦截器|实现Interceptor接口或者在方法前加@Before/@After注释|实现Interceptor接口，并在struts.xml中配置|
|本地化|为错误、命名和JSTL提供资源包|提供资源包查询机制|

 注：从上面的配置中可以看出，Stripes减少了很多配置工作，并且内置了验证注解而不再去编写新的Java 类来实现验证功能。

**行为(Actions)**

     Stripes的Action定义由类实现ActionBean的接口，并自动加载包或子包的ActionResolver.Packages初始化参数中列出的包之一。

     Struts2的Action可以是普通的公共字符串的execute（）方法，或者实现了Action接口的类的类。他们必须在struts.xml声明，或者复制来自Stripes的自动加载机制。


**事件处理程序（Event handlers）**

     在Stripes中，用public Resolution methodName() {}来定义一个事件处理程序。它使用“name=”来表示一个submit的提交，"event="来表示事件的触发指向（target）的事件方法的调用。在一个表单中可以拥有多个event handler事件处理方法和多个submit的按钮，一个简单的mapper（接口）可以对应多个事件处理程序，并且可以将之放入submit标签对应值的按钮。

   Struts2是面向一个单一的事件处理程序方法：execute（）。你可以有其他的事件处理方法，具有任意名称的，但您必须在struts.xml中包含URL映射到一个方法的名称的配置策略。Struts2令人惊讶的是它很困难去实现有一个以上的提交按钮的形式。虽然它支持多个submit按钮，但并不像Stripes那么简单明了。

**决议与结果（Resolutions and results）**

   Stripes 的事件处理程序返回的是一个实现了Resolution的实例。Stripes 内置实现转发或重定向，数据流，返回JavaScript对象，或者是HTTP请求的错误码。它很简单的就实现了自定义要求的Resolution 接口（只需要一个方法）。

Struts2's execute() 方法返回一个必须被映射到具体东西的String，无论是在的struts.xml中或与注释的符号结果。可以说，返回一个象征性的结果，不必去别的地方连接字符串的结果避免给开发人员带来不必要的负担。

**自定义类型转换（Custom Type Converters）**

   Stripes中实现自定义转换器需要实现TypeConverter<T> （T是目标对象）。然后，您可以将T对象的每个属性放入自定义类型转换器中，并将该类型转换器放入扩展包（Extension.Packages
 package）里。或者，您也可以使用@Validate(converter=YourTypeConverter.class).来注解你需要特殊转化处理的属性字段。


 
     Struts2的，你写一个自定义类型转换器通过实施ognl.TypeConverter接口，通常由扩展StrutsTypeConverter类。Stripes相反，Struts2的接口没有泛型化，所以你的方法将返回一个Object。使用类型转换器类型T的每个属性，添加一行xwork的conversion.properties的属性完全合格的名称和值类型转换器类的完全限定名称的属性表示T。对于一个特定的属性，你添加的属性的名称和类型转换器类的完全限定名称ActionName
 conversion.properties ActionName是类名的动作，该文件是在同一个目录层次结构包动作类。

**视图技术（View Technology）**

 
      Stripes框架支持任何支持JSP标签库的视图技术。这意味着你可以使用JSP和Freemarker的，并且可以实现为一个servlet的映射。从J2EE，JSP，FreeMarker有详细FreeMarker的Stripes配置。使用Velocity与Stripes是可能使用工具项目VelocityView，但你缺少标签库(taglib)的支持。Velocity1.5不支持JSP标记库
 - 这是一个自2006年以来2.0愿望清单功能的特性（feature）。


 
     Struts2的，JSP的支持，就像Stripes。但Struts2的也有插件来处理Freemarker和Velocity，从而使这两个视图技术的功能变得可能。话虽这么说，使用Freemarker的（或者JSP）是比Velocity更自然一些。例如，对于此标记：



```
<s:form action="Login">
  <s:textfield name="username" label="Username"/>
  <s:submit value="Submit"/>
</s:form>
```
**拦截器(Interceptors)**


Stripes拦截器类实现的是Interceptor接口，并指定它与@Intercepts注解的生命周期阶段（次）被截获。Stripes会自动加载类通过Extension.Packages机制。您也可以在web.xml中配置的拦截器，拦截器执行的顺序是重要的。

 
      Struts2的拦截器也是实现Interceptor接口。然后，您必须在struts.xml定义拦截器类，定义一个新的拦截器栈，使用默认的堆栈，并增加你的拦截器。最后，你必须配置Action将使用这个新的拦截器栈。



