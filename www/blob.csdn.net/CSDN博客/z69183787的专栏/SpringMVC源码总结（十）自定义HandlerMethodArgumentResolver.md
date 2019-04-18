# SpringMVC源码总结（十）自定义HandlerMethodArgumentResolver - z69183787的专栏 - CSDN博客
2016年10月14日 17:11:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：859
上一篇文章介绍了HandlerMethodArgumentResolver的来龙去脉，这篇就要说说自定义HandlerMethodArgumentResolver来解决我们的需求，本文提供了四种解决方案。 
需求，有一个Teacher类和Student类，他们都有属性name和age： 
前端form表单为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <form action="/test/two" method="post" >  
-             <input type="text" name="teacher.name" value="张三">  
-             <input type="text" name="teacher.age" value=88>  
-             <input type="text" name="student.name" value="李四">  
-             <input type="text" name="student.age" value=89>  
-             <input type="submit" value="提交">  
-         </form>  
希望后台能这样接收这样的参数： 
解决方案有很多： 
方案一：
新建一个类，融合这两个类。如
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass Father {  
- 
- private Teacher teacher;  
- private Student student;  
- public Teacher getTeacher() {  
- return teacher;  
-     }  
- publicvoid setTeacher(Teacher teacher) {  
- this.teacher = teacher;  
-     }  
- public Student getStudent() {  
- return student;  
-     }  
- publicvoid setStudent(Student student) {  
- this.student = student;  
-     }  
- }  
在后台这样接收参数：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/father",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testFather(@RequestBody Father f){  
- //略
-         }  
即使用@RequestBody来接受这样的参数。下面还要说说这样做的两个问题，你或许可以试猜一下： 
使用form表单来进行提交，运行： 
问题一： 
首先会遇到415 Unsupported Media Type，如下： 
![](http://dl2.iteye.com/upload/attachment/0100/7616/9241fce5-daa4-3e83-9d4b-4e179424ed6f.png)
我们的form表单默认是以application/x-www-form-urlencoded方式提交的，而@RequestBody又采用的是RequestResponseBodyMethodProcessor这个HandlerMethodArgumentResolver，RequestResponseBodyMethodProcessor内部的处理原理就是用一系列的HttpMessageConverter来进行数据的转换的。这时候就需要找到支持MediaType类型为application/x-www-form-urlencoded和数据的类型为Father的HttpMessageConverter，当然就找不到了。我们本意是想让MappingJackson2HttpMessageConverter来处理的，但是它仅仅支持的MediaType类型为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- public MappingJackson2HttpMessageConverter() {  
- super(new MediaType("application", "json", DEFAULT_CHARSET),  
- new MediaType("application", "*+json", DEFAULT_CHARSET));  
-     }  
即application/json或者application/*+json。所以此时就需要我们更改提交的content-type。然而form表单目前的仅仅支持三种content-type即application/x-www-form-urlencoded、multipart/form-data、text/plain。所以我们需要更换成ajax提交，如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- function postFather1(){  
-         var url='/test/father';  
-         var data={  
- 'teacher.name':'张三' ,  
- 'teacher.age':88    ,  
- 'student.name':'李四' ,  
- 'student.age':89    ,  
-         };  
-         $.ajax({  
-             url:url,  
-             type:'POST',  
-             data:JSON.stringify(data),  
-             dataType:'json',  
-             contentType:"application/json;charset=utf-8",  
-             success:function(result){  
- 
-             }  
-         });  
-     }  
此时又有一个问题，teacher.name这样的形式并不能正确解析成Father。仍然需要变换格式：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- var data={  
- 'teacher':{  
- 'name':'张三',  
- 'age':88
-             },  
- 'student':{  
- 'name':'李四',  
- 'age':89
-             }  
-         };  
这样的json形式才能够被正确解析出来。 
所以说方案一有很多的地方要修改，并不是那么优雅。 
方案二：
我们仍然使用form表单提交：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <form action="/test/two" method="post">  
-             <input type="text" name="teacher.name" value="张三">  
-             <input type="text" name="teacher.age" value=88>  
-             <input type="text" name="student.name" value="李四">  
-             <input type="text" name="student.age" value=89>  
-             <input type="submit" value="提交">  
-         </form>  
服务器端的变化为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @InitBinder("teacher")  
- publicvoid initBinder1(WebDataBinder binder) throws Exception {    
-         binder.setFieldDefaultPrefix("teacher.");     
-     }  
- 
- @InitBinder("student")  
- publicvoid initBinder2(WebDataBinder binder) throws Exception {    
-         binder.setFieldDefaultPrefix("student.");     
-     }  
- @RequestMapping(value="/test/two",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testrequestHeader(Teacher a,Student b){  
-         Map<String,Object> map=new HashMap<String,Object>();  
-         map.put("name","lg");  
-         map.put("age",23);  
-         map.put("date",new Date());  
- return map;  
-     }  
大体上来说就是在解析每个参数时加上前缀限制。下面就要看看这个过程的源码分析： 
到底选择哪个HandlerMethodArgumentResolver来解析我们的参数呢？它最终会选择ServletModelAttributeMethodProcessor，看下它的判断条件：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- /**
-      * @return true if the parameter is annotated with {@link ModelAttribute}
-      * or in default resolution mode also if it is not a simple type.
-      */
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- if (parameter.hasParameterAnnotation(ModelAttribute.class)) {  
- returntrue;  
-         }  
- elseif (this.annotationNotRequired) {  
- return !BeanUtils.isSimpleProperty(parameter.getParameterType());  
-         }  
- else {  
- returnfalse;  
-         }  
-     }  
这里说明了它可以支持两种情况，一种情况为含有@ModelAttribute注解的参数，另一种情况就是虽然不含@ModelAttribute注解，但它并不是简单类型，如常用的String、Date等。你会发现spring会注册两个ServletModelAttributeMethodProcessor，一个annotationNotRequired为false，另一个为true。这主要是因为调用HandlerMethodArgumentResolver的解析顺序的原因，如果只有一个ServletModelAttributeMethodProcessor，当它判断参数不含@ModelAttribute注解，那它就把参数作为非简单类型来处理，这样的话，后面很多的HandlerMethodArgumentResolver将无法发挥作用。所以annotationNotRequired=true的ServletModelAttributeMethodProcessor是在最后才调用的。 
然后再具体看看ServletModelAttributeMethodProcessor的处理过程：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal Object resolveArgument(  
-             MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest request, WebDataBinderFactory binderFactory)  
- throws Exception {  
- 
-         String name = ModelFactory.getNameForParameter(parameter);  
-         Object attribute = (mavContainer.containsAttribute(name)) ?  
-                 mavContainer.getModel().get(name) : createAttribute(name, parameter, binderFactory, request);  
- 
-         WebDataBinder binder = binderFactory.createBinder(request, attribute, name);  
- if (binder.getTarget() != null) {  
-             bindRequestParameters(binder, request);  
-             validateIfApplicable(binder, parameter);  
- if (binder.getBindingResult().hasErrors()) {  
- if (isBindExceptionRequired(binder, parameter)) {  
- thrownew BindException(binder.getBindingResult());  
-                 }  
-             }  
-         }  
- 
- // Add resolved attribute and BindingResult at the end of the model
- 
-         Map<String, Object> bindingResultModel = binder.getBindingResult().getModel();  
-         mavContainer.removeAttributes(bindingResultModel);  
-         mavContainer.addAllAttributes(bindingResultModel);  
- 
- return binder.getTarget();  
-     }  
首先就是获取参数名的过程，String name = ModelFactory.getNameForParameter(parameter);具体内容如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicstatic String getNameForParameter(MethodParameter parameter) {  
-         ModelAttribute annot = parameter.getParameterAnnotation(ModelAttribute.class);  
-         String attrName = (annot != null) ? annot.value() : null;  
- return StringUtils.hasText(attrName) ? attrName :  Conventions.getVariableNameForParameter(parameter);  
-     }  
这里先尝试从@ModelAttribute注解中获取参数名，若没有则根据参数类型来获取参数名
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicstatic String getVariableNameForParameter(MethodParameter parameter) {  
-         Assert.notNull(parameter, "MethodParameter must not be null");  
-         Class<?> valueClass;  
- boolean pluralize = false;  
- 
- if (parameter.getParameterType().isArray()) {  
-             valueClass = parameter.getParameterType().getComponentType();  
-             pluralize = true;  
-         }  
- elseif (Collection.class.isAssignableFrom(parameter.getParameterType())) {  
-             valueClass = GenericCollectionTypeResolver.getCollectionParameterType(parameter);  
- if (valueClass == null) {  
- thrownew IllegalArgumentException(  
- "Cannot generate variable name for non-typed Collection parameter type");  
-             }  
-             pluralize = true;  
-         }  
- else {  
-             valueClass = parameter.getParameterType();  
-         }  
- 
-         String name = ClassUtils.getShortNameAsProperty(valueClass);  
- return (pluralize ? pluralize(name) : name);  
-     }  
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicstatic String getShortNameAsProperty(Class<?> clazz) {  
-         String shortName = ClassUtils.getShortName(clazz);  
- int dotIndex = shortName.lastIndexOf('.');  
-         shortName = (dotIndex != -1 ? shortName.substring(dotIndex + 1) : shortName);  
- return Introspector.decapitalize(shortName);  
-     }  
获取类的简单名称如Teacher，然后再进行处理
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicstatic String decapitalize(String name) {  
- if (name == null || name.length() == 0) {  
- return name;  
-        }  
- if (name.length() > 1 && Character.isUpperCase(name.charAt(1)) &&  
-                        Character.isUpperCase(name.charAt(0))){  
- return name;  
-        }  
- char chars[] = name.toCharArray();  
-        chars[0] = Character.toLowerCase(chars[0]);  
- returnnew String(chars);  
-    }  
有了类的简单名称，如果类的简单名称第一个和第二个字母都大写则不进行处理直接返回类的简单名称，否则仅仅将类的第一个大写变成小写。就此获取到了参数名为teacher。 
然后就是获取或者创建我们要绑定的Teacher对象。它首先尝试从要返回的model中能否找到属性名为teacher的model，如找不到，就需要去创建一个：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- protectedfinal Object createAttribute(String attributeName,  
-                                            MethodParameter parameter,  
-                                            WebDataBinderFactory binderFactory,  
-                                            NativeWebRequest request) throws Exception {  
- 
-         String value = getRequestValueForAttribute(attributeName, request);  
- if (value != null) {  
-             Object attribute = createAttributeFromRequestValue(value, attributeName, parameter, binderFactory, request);  
- if (attribute != null) {  
- return attribute;  
-             }  
-         }  
- 
- returnsuper.createAttribute(attributeName, parameter, binderFactory, request);  
-     }  
先尝试从request参数中能否找到teacher这一个参数，找到了就进行绑定和转换。未找到，就需要自己来实例化一个Teacher对象，此时并没有绑定相应的参数值。 
有个返回的目标，然后就是创建WebDataBinder实现绑定的过程： 
WebDataBinder binder = binderFactory.createBinder(request, attribute, name);
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal WebDataBinder createBinder(NativeWebRequest webRequest, Object target, String objectName)  
- throws Exception {  
-         WebDataBinder dataBinder = createBinderInstance(target, objectName, webRequest);  
- if (this.initializer != null) {  
- this.initializer.initBinder(dataBinder, webRequest);  
-         }  
-         initBinder(dataBinder, webRequest);  
- return dataBinder;  
-     }  
这一个过程，我们之前已经分析过。就是调度执行一些@InitBinder方法注册一些PropertyEditor。我们继续要来看看initBinder(dataBinder, webRequest);执行了那些@InitBinder方法：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicvoid initBinder(WebDataBinder binder, NativeWebRequest request) throws Exception {  
- for (InvocableHandlerMethod binderMethod : this.binderMethods) {  
- if (isBinderMethodApplicable(binderMethod, binder)) {  
-                 Object returnValue = binderMethod.invokeForRequest(request, null, binder);  
- if (returnValue != null) {  
- thrownew IllegalStateException("@InitBinder methods should return void: " + binderMethod);  
-                 }  
-             }  
-         }  
-     }  
- 
- protectedboolean isBinderMethodApplicable(HandlerMethod initBinderMethod, WebDataBinder binder) {  
-         InitBinder annot = initBinderMethod.getMethodAnnotation(InitBinder.class);  
-         Collection<String> names = Arrays.asList(annot.value());  
- return (names.size() == 0 || names.contains(binder.getObjectName()));  
-     }  
当@InitBinder指定了value值的时候，只有那些value值含有binder.getObjectName()的才会执行，而此时的binder.getObjectName()就是我们辛辛苦苦找出来的参数名teacher。所以本例中@InitBinder("teacher")会执行，而@InitBinder("student")则不会执行。 
之后对四个参数 teacher.name='张三'、teacher.age=88、student.name='李四'、student.age=89 通过前缀进行过滤等其他操作实现了参数绑定。此过程不再分析，有兴趣的可以继续研究。 
方案三：
使用自定义的HandlerMethodArgumentResolver： 
表单提交的内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <form action="/test/two" method="post">  
-             <input type="text" name="teacher.name" value="张三">  
-             <input type="text" name="teacher.age" value=88>  
-             <input type="text" name="teacher.date" value="2014---09---04 05:23:00">  
-             <input type="text" name="teacher.love" value="乒乓球,篮球">  
-             <input type="text" name="student.name" value="李四">  
-             <input type="text" name="student.age" value=89>  
-             <input type="text" name="student.date" value="2014---09---05 05:23:00">  
-             <input type="text" name="student.love" value="羽毛球,台球">  
-             <input type="submit" value="提交">  
-         </form>  
其中Teacher和Student做了相应的修改，加大了数据的复杂性。如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicclass Teacher {  
- 
- private String name;  
- privateint age;  
- private Date date;  
- private List<String> love;  
- 
- public Teacher() {  
- super();  
-     }  
- 
- public Teacher(String name, int age) {  
- super();  
- this.name = name;  
- this.age = age;  
-     }  
- //get set 省略 
- }  
请求的的处理函数为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @InitBinder
- publicvoid initBinder(WebDataBinder binder) throws Exception {    
-         DateFormat df = new SimpleDateFormat("yyyy---MM---dd HH:mm:ss");    
-         CustomDateEditor dateEditor = new CustomDateEditor(df, true);    
-         binder.registerCustomEditor(Date.class, dateEditor);        
-     }  
- 
- @RequestMapping(value="/test/two",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testrequestHeader(@MyForm Teacher a,@MyForm Student b){  
-         Map<String,Object> map=new HashMap<String,Object>();  
-         map.put("name","lg");  
-         map.put("age",23);  
-         map.put("date",new Date());  
- return map;  
-     }  
经过测试，通过。 
自定义了两个东西，一个就是标签MyForm，另一个就是MyHandlerMethodArgumentResolver，并且我们从上一篇文章中知道如何将自定义HandlerMethodArgumentResolver加入HandlerMethodArgumentResolver大军中。如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven >  
-         <!--其他省略 -->  
-         <mvc:argument-resolvers>  
-             <bean class="com.lg.mvc.MyHandlerMethodArgumentResolver"/>  
-         </mvc:argument-resolvers>  
-     </mvc:annotation-driven>    
我们来具体分析下这个过程，首先是注解 MyForm：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- package com.lg.annotation;  
- 
- import java.lang.annotation.Documented;  
- import java.lang.annotation.ElementType;  
- import java.lang.annotation.Retention;  
- import java.lang.annotation.RetentionPolicy;  
- import java.lang.annotation.Target;  
- 
- @Target(ElementType.PARAMETER)  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- public@interface MyForm {  
- 
-     String value() default"";  
- }  
只有有一个value属性，用来指定from表单的中字段的前缀，若不指定，我将采取类名首字母小写的规则来默认前缀。如@MyForm Teacher a，默认前缀是teacher。 
然后就是MyHandlerMethodArgumentResolver，专门用来解析@MyForm注解的：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- package com.lg.mvc;  
- 
- import java.lang.reflect.Field;  
- 
- import org.springframework.core.MethodParameter;  
- import org.springframework.util.ClassUtils;  
- import org.springframework.web.bind.WebDataBinder;  
- import org.springframework.web.bind.support.WebDataBinderFactory;  
- import org.springframework.web.context.request.NativeWebRequest;  
- import org.springframework.web.method.support.HandlerMethodArgumentResolver;  
- import org.springframework.web.method.support.ModelAndViewContainer;  
- 
- import com.lg.annotation.MyForm;  
- 
- publicclass MyHandlerMethodArgumentResolver implements HandlerMethodArgumentResolver{  
- 
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
- if (parameter.hasParameterAnnotation(MyForm.class)) {  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- @Override
- public Object resolveArgument(MethodParameter parameter,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest,  
-             WebDataBinderFactory binderFactory) throws Exception {  
- if (binderFactory==null) {  
- returnnull;  
-         }  
-         Class<?> targetType=parameter.getParameterType();  
-         MyForm myForm=parameter.getParameterAnnotation(MyForm.class);  
-         String prefix=getprefix(myForm,targetType);  
-         Object arg=null;  
-         Field[] fields=targetType.getDeclaredFields();  
-         Object target=targetType.newInstance();  
-         WebDataBinder binder = binderFactory.createBinder(webRequest, null,prefix);  
- for(Field field:fields){  
-             field.setAccessible(true);  
-             String fieldName=field.getName();  
-             Class<?> fieldType=field.getType();  
-             arg = binder.convertIfNecessary(webRequest.getParameter(prefix+"."+fieldName),fieldType, parameter);  
-             field.set(target,arg);  
-         }  
- return target;  
-     }  
- 
- private String getprefix(MyForm myForm,Class<?> targetType) {  
-         String prefix=myForm.value();  
- if(prefix.equals("")){  
-             prefix=getDefaultClassName(targetType);  
-         }  
- return prefix;  
-     }  
- 
- private String getDefaultClassName(Class<?> targetType) {  
- return ClassUtils.getShortNameAsProperty(targetType);  
-     }  
- 
- }  
其实也挺简单的。对于supportsParameter方法就是看看有没有MyForm注解，若有则处理。 
重点就在resolveArgument方法上：targetType就是MyForm所修饰的Teacher类或Student类，这里以Teacher为例。首先就是调用Teacher的无参的构造函数创建一个Teacher对象。然后由绑定工厂创建出绑定类，WebDataBinder binder = binderFactory.createBinder(webRequest, null,prefix);这一过程已在方案二中分析过了，就是执行那些符合的@InitBinder方法，这里我们传的值为prefix，即MyForm的value，若没指定就是类名的首字母小写，在这里就是teacher。也就是说那些@InitBinder的value值中含有teacher或者@InitBinder没有指定value值的方法才会被执行。因此我们这里注册的日期转换CustomDateEditor会被注册进去。然后就是执行绑定的过程。这个过程就是利用已注册的PropertyEditor和Converter来进行Field类型的转换。如下分析 
遍历它的Field，如String name，fieldType为String。binder.convertIfNecessary(webRequest.getParameter(prefix+"."+fieldName),fieldType, parameter);这里就是把teacher.name参数值转换成fieldType,都是String，所以就不需要转换器。对于Date date,就是把teacher.date参数的字符串值转换成Date类型，然后就用到了我们注册的CustomDateEditor，成功的进行了转换。对于
 List<String> love,就是把teacher.love参数的字符串值转换成List集合，使用的是Spring已经注册的StringToArrayConverter，字符串默认是以','分割。 
该方案只能进行简单类型的转换（Teacher中field都是些简单类型），还不支持Teacher中包含复杂类型如包含其他属性类。其实也可以做成支持的，就是再稍加改造些，对于Field的处理先判断是否是简单类型，如Address类，若不是则递归调用上面的处理过程即对Address再次遍历Field来实现Address中简单类型的绑定。关键就是执行个递归调用，其他也没什么，有兴趣的可以自行研究。本例中的自定义文件可在后面下载。 
方案四：
根据方案二我们其实就可以想到更改下方案二所用到的ServletModelAttributeMethodProcessor，就可以达到我们想要的结果。即如下：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicfinal Object resolveArgument(  
-             MethodParameter parameter, ModelAndViewContainer mavContainer,  
-             NativeWebRequest request, WebDataBinderFactory binderFactory)  
- throws Exception {  
- 
-         String name = ModelFactory.getNameForParameter(parameter);  
-         Object attribute = (mavContainer.containsAttribute(name)) ?  
-                 mavContainer.getModel().get(name) : createAttribute(name, parameter, binderFactory, request);  
- //重点在这里在这里在这里在这里在这里在这里在这里
-         WebDataBinder binder = binderFactory.createBinder(request, attribute, name);  
- if (binder.getTarget() != null) {  
-             bindRequestParameters(binder, request);  
-             validateIfApplicable(binder, parameter);  
- if (binder.getBindingResult().hasErrors()) {  
- if (isBindExceptionRequired(binder, parameter)) {  
- thrownew BindException(binder.getBindingResult());  
-                 }  
-             }  
-         }  
- 
- // Add resolved attribute and BindingResult at the end of the model
- 
-         Map<String, Object> bindingResultModel = binder.getBindingResult().getModel();  
-         mavContainer.removeAttributes(bindingResultModel);  
-         mavContainer.addAllAttributes(bindingResultModel);  
- 
- return binder.getTarget();  
-     }  
WebDataBinder binder = binderFactory.createBinder(request, attribute, name);在创建出WebDataBinder后，调用下binder.setFieldDefaultPrefix(prefix);就可以大功告成了。然而，我们会看到该方法是final，不可覆盖的，我就复制粘贴了一份，出来，新建了一个自定义的MyServletModelAttributeMethodProcessor以及它对应的注解标签MyServletModelForm，代码如下：
MyServletModelForm内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @Target(ElementType.PARAMETER)  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- public@interface MyServletModelForm {  
- 
-     String value() default"";  
- 
- }  
MyServletModelAttributeMethodProcessor的主要内容为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- publicboolean supportsParameter(MethodParameter parameter) {  
- if (parameter.hasParameterAnnotation(MyServletModelForm.class)) {  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- @Override
- public Object resolveArgument(MethodParameter parameter,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest,  
-             WebDataBinderFactory binderFactory) throws Exception {  
-         String name = ModelFactory.getNameForParameter(parameter);  
-         Object attribute = (mavContainer.containsAttribute(name)) ?  
-                 mavContainer.getModel().get(name) : createAttribute(name, parameter, binderFactory, webRequest);  
- 
-         WebDataBinder binder = binderFactory.createBinder(webRequest, attribute, name);  
-         String prefix=getFieldDefaultPrefix(parameter);  
- if(!prefix.equals("")){  
-             binder.setFieldDefaultPrefix(prefix+".");  
-         }  
- if (binder.getTarget() != null) {  
-             bindRequestParameters(binder, webRequest);  
-             validateIfApplicable(binder, parameter);  
- if (binder.getBindingResult().hasErrors()) {  
- if (isBindExceptionRequired(binder, parameter)) {  
- thrownew BindException(binder.getBindingResult());  
-                 }  
-             }  
-         }  
- 
- 
-         Map<String, Object> bindingResultModel = binder.getBindingResult().getModel();  
-         mavContainer.removeAttributes(bindingResultModel);  
-         mavContainer.addAllAttributes(bindingResultModel);  
- 
- return binder.getTarget();  
-     }  
处理函数代码为：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- @RequestMapping(value="/test/two",method=RequestMethod.POST)  
- @ResponseBody
- public Map<String,Object> testrequestHeader(@MyServletModelForm Teacher a,@MyServletModelForm Student b){  
-         Map<String,Object> map=new HashMap<String,Object>();  
-         map.put("name","lg");  
-         map.put("age",23);  
-         map.put("date",new Date());  
- return map;  
-     }  
并把MyServletModelAttributeMethodProcessor在xml文件中进行配置：
Java代码  ![收藏代码](http://lgbolgger.iteye.com/images/icon_star.png)
- <mvc:annotation-driven >  
- 
-         <mvc:message-converters register-defaults="true">  
-             <bean class="org.springframework.http.converter.StringHttpMessageConverter">    
-                 <constructor-arg value="UTF-8"/>    
-             </bean>    
-         </mvc:message-converters>  
-         <mvc:argument-resolvers>  
-             <bean class="com.lg.mvc.MyHandlerMethodArgumentResolver"/>  
-             <bean class="com.lg.mvc.MyServletModelAttributeMethodProcessor"/>  
-         </mvc:argument-resolvers>  
-     </mvc:annotation-driven>    
经测试，可以。附件中含有这两个类供下载。
- [自定义.zip](http://dl.iteye.com/topics/download/a9a4d415-8ceb-3957-b969-4cd16c1ef8b4) (3.5 KB)
- 下载次数: 22
- [查看图片附件](http://lgbolgger.iteye.com/blog/2111897#)


