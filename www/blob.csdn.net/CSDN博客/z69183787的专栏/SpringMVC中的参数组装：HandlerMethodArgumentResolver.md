# SpringMVC中的参数组装：HandlerMethodArgumentResolver - z69183787的专栏 - CSDN博客
2016年07月11日 18:06:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：640
SpringMVC3.1引入了HandlerMethodArgumentResolver接口，Spring调用该接口实现Controller的参数装配。HandlerMethodArgumentResolver实现类中会调用DataBinder，Converter等。
常用的该接口实现类有:
ServletModelAttributeMethodProcessor:实体类的组装用它实现。
RequestParamMethodArgumentResolver：基本数据类型如String用它实现。
在我学习过程中，发现对List类型的参数SpringMVC没有提供默认实现。我参照Spring的[示例](https://github.com/spring-projects/spring-mvc-showcase) 通过实现 HandlerMethodArgumentResolver接口，实现对List参数的组装。
示例中用到的Model类：
**[java]**[view
 plain](http://blog.csdn.net/fytain/article/details/43918609#)[copy](http://blog.csdn.net/fytain/article/details/43918609#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/607126/fork)
- package com.weishubin.springmvc.model;  
- 
- publicclass User {  
- private String name;  
- privateint age;  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- publicint getAge() {  
- return age;  
-     }  
- publicvoid setAge(int age) {  
- this.age = age;  
-     }  
- 
- public String toString() {  
-         StringBuilder sb = new StringBuilder();  
-         sb.append(name);  
-         sb.append("-");  
-         sb.append(age);  
- return sb.toString();  
-     }  
- }  
注解类：
**[java]**[view
 plain](http://blog.csdn.net/fytain/article/details/43918609#)[copy](http://blog.csdn.net/fytain/article/details/43918609#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/607126/fork)
- package com.weishubin.springmvc.listargument;  
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
- public@interface ListAttribute {  
- 
- }  
实现了HandlerMethodArgumentResolver接口的参数解析器：
**[java]**[view
 plain](http://blog.csdn.net/fytain/article/details/43918609#)[copy](http://blog.csdn.net/fytain/article/details/43918609#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/607126/fork)
- package com.weishubin.springmvc.listargument;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.springframework.beans.BeanWrapper;  
- import org.springframework.beans.PropertyAccessorFactory;  
- import org.springframework.core.MethodParameter;  
- import org.springframework.web.bind.support.WebDataBinderFactory;  
- import org.springframework.web.context.request.NativeWebRequest;  
- import org.springframework.web.method.support.HandlerMethodArgumentResolver;  
- import org.springframework.web.method.support.ModelAndViewContainer;  
- 
- import com.weishubin.springmvc.model.User;  
- 
- publicclass ListArgumentResolver implements HandlerMethodArgumentResolver {  
- 
- publicboolean supportsParameter(MethodParameter parameter) {  
- //仅作用于添加了注解ListAttribute的参数
- return parameter.getParameterAnnotation(ListAttribute.class) != null;  
-     }  
- 
- public Object resolveArgument(MethodParameter parameter,  
-             ModelAndViewContainer mavContainer, NativeWebRequest webRequest,  
-             WebDataBinderFactory binderFactory) throws Exception {  
-         List<User> users = new ArrayList<User>();  
-         String[] names = webRequest.getParameterValues("name");  
-         String[] ages = webRequest.getParameterValues("age");  
- 
- 
- for (int i = 0; i < names.length; i++) {  
-             User user = new User();  
-             BeanWrapper beanWrapper = PropertyAccessorFactory.forBeanPropertyAccess(user);  
- 
-             String name = names[i];  
-             String age = ages[i];  
-             beanWrapper.setPropertyValue("name", name);  
-             beanWrapper.setPropertyValue("age", age);  
- 
-             users.add(user);  
-         }  
- return users;  
-     }  
- 
- }  
Controller类：
**[java]**[view
 plain](http://blog.csdn.net/fytain/article/details/43918609#)[copy](http://blog.csdn.net/fytain/article/details/43918609#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/607126/fork)
- package com.weishubin.springmvc.listargument;  
- 
- import java.util.List;  
- 
- import org.springframework.stereotype.Controller;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.ResponseBody;  
- 
- import com.weishubin.springmvc.model.User;  
- 
- @Controller
- publicclass ListArgumentController {  
- 
- @ResponseBody
- @RequestMapping("/list")  
- public String argumentResolver(@ListAttribute List<User> list) {  
-         StringBuilder b = new StringBuilder();  
- for (User u : list) {  
-             b.append(u);  
-             b.append("<br/>");  
-         }  
- return b.toString();  
-     }  
- }  
配置文件：
**[html]**[view
 plain](http://blog.csdn.net/fytain/article/details/43918609#)[copy](http://blog.csdn.net/fytain/article/details/43918609#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/607126/fork)
- <mvc:annotation-driven>
- <mvc:argument-resolvers>
- <beanclass="com.weishubin.springmvc.listargument.ListArgumentResolver"/>
- </mvc:argument-resolvers>
- </mvc:annotation-driven>
- 
