# SpringMVC杂记(十七) HandlerMethodArgumentResolver接口应用example - z69183787的专栏 - CSDN博客
2016年07月11日 18:07:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：410
自从spring3.1 开始就有了这个接口，可以为@RequestMapping标注的方法扩展传入的参数。 
以shiro为例，扩展一个标注，@CurrentUser,只要有这个标注，就可以在shiro的安全上下文中取出适当的对象直接从参数传入，request响应函数。 
Java代码  ![收藏代码](http://yingzhuo.iteye.com/images/icon_star.png)
- import java.lang.annotation.Documented;  
- import java.lang.annotation.ElementType;  
- import java.lang.annotation.Retention;  
- import java.lang.annotation.RetentionPolicy;  
- import java.lang.annotation.Target;  
- 
- @Documented
- @Target({ElementType.PARAMETER})  
- @Retention(RetentionPolicy.RUNTIME)  
- public@interface CurrentUser {  
- }  
Java代码  ![收藏代码](http://yingzhuo.iteye.com/images/icon_star.png)
- @RequestMapping(value = "/test1")  
- public@ResponseBody String test1(@CurrentUser Long userId) {  
- return userId.toString();  
- }  
- 
- @RequestMapping(value = "/test2")  
- public@ResponseBody String test2(@CurrentUser UserDetails userDetails) {  
- return userDetails.toString();  
- }  
Java代码  ![收藏代码](http://yingzhuo.iteye.com/images/icon_star.png)
- import java.lang.annotation.Annotation;  
- 
- import org.springframework.core.MethodParameter;  
- import org.springframework.web.bind.support.WebDataBinderFactory;  
- import org.springframework.web.context.request.NativeWebRequest;  
- import org.springframework.web.method.support.HandlerMethodArgumentResolver;  
- import org.springframework.web.method.support.ModelAndViewContainer;  
- 
- import com.github.yingzhuo.mycar2.annotation.CurrentUser;  
- import com.github.yingzhuo.mycar2.security.SecurityUtils;  
- import com.github.yingzhuo.mycar2.security.UserDetails;  
- 
- publicclass CurrentUserHandlerMethodArgumentResolver implements HandlerMethodArgumentResolver {  
- 
- @Override
- publicboolean supportsParameter(MethodParameter parameter) {  
-         Class<?> klass = parameter.getParameterType();  
- if (klass.isAssignableFrom(UserDetails.class) || klass.isAssignableFrom(Long.class)) {  
-             Annotation[] as = parameter.getParameterAnnotations();  
- for (Annotation a : as) {  
- if (a.annotationType() == CurrentUser.class) {  
- returntrue;  
-                 }  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- @Override
- public Object resolveArgument(MethodParameter parameter, ModelAndViewContainer mavContainer, NativeWebRequest webRequest,  
-             WebDataBinderFactory binderFactory) throws Exception {  
- 
- if ((SecurityUtils.isAuthenticated() || SecurityUtils.isRemembered()) == false) {  
- returnnull;  
-         }  
- 
-         Class<?> klass = parameter.getParameterType();  
- 
-         UserDetails userDetails = SecurityUtils.getUserDetails();  
- 
- if (klass.isAssignableFrom(UserDetails.class)) {  
- return SecurityUtils.getUserDetails();  
-         }  
- 
- if (klass.isAssignableFrom(Long.class)) {  
- return userDetails != null ? userDetails.getId() : null;  
-         }  
- 
- returnnull;  
-     }  
- }  
最后，需要配置一下 
Xml代码  ![收藏代码](http://yingzhuo.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>
- <mvc:argument-resolvers>
- <beanclass="xxx.yyy.CurrentUserHandlerMethodArgumentResolver"/>
- </mvc:argument-resolvers>
- </mvc:annotation-driven>
