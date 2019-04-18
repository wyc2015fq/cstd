# 使用Spring MVC统一异常处理实战 - z69183787的专栏 - CSDN博客
2015年03月09日 10:38:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：931
个人分类：[SpringMvc-异常处理](https://blog.csdn.net/z69183787/article/category/6689566)
**1 描述**
在J2EE项目的开发中，不管是对底层的数据库操作过程，还是业务层的处理过程，还是控制层的处理过程，都不可避免会遇到各种可预知的、不可预知的异常需要处理。每个过程都单独处理异常，系统的代码耦合度高，工作量大且不好统一，维护的工作量也很大。 
那么，能不能将所有类型的异常处理从各处理过程解耦出来，这样既保证了相关处理过程的功能较单一，也实现了异常信息的统一处理和维护？答案是肯定的。下面将介绍使用Spring MVC统一处理异常的解决和实现过程。 
**2 分析**
Spring MVC处理异常有3种方式： 
（1）使用Spring MVC提供的简单异常处理器SimpleMappingExceptionResolver； 
（2）实现Spring的异常处理接口HandlerExceptionResolver 自定义自己的异常处理器； 
（3）使用@ExceptionHandler注解实现异常处理； 
**3 实战**
**3.1 引言**
为了验证Spring MVC的3种异常处理方式的实际效果，我们需要开发一个测试项目，从Dao层、Service层、Controller层分别抛出不同的异常，然后分别集成3种方式进行异常处理，从而比较3种方式的优缺点。 
**3.2 实战项目**
**3.2.1 项目结构**
![](http://dl.iteye.com/upload/attachment/0069/0213/02bd63f4-d3cc-3e6c-a4e2-6d1cd68f4932.png)![](http://dl.iteye.com/upload/attachment/0069/0215/d74ab215-abb2-3bd7-8fb4-9e11910a1337.png)
**3.2.2 Dao层代码**
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- @Repository("testDao")  
- publicclass TestDao {  
- publicvoid exception(Integer id) throws Exception {  
- switch(id) {  
- case1:  
- thrownew BusinessException("12", "dao12");  
- case2:  
- thrownew BusinessException("22", "dao22");  
- case3:  
- thrownew BusinessException("32", "dao32");  
- case4:  
- thrownew BusinessException("42", "dao42");  
- case5:  
- thrownew BusinessException("52", "dao52");  
- default:  
- thrownew ParameterException("Dao Parameter Error");  
-         }  
-     }  
- }  
**3.2.3 Service层代码**
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- publicinterface TestService {  
- publicvoid exception(Integer id) throws Exception;  
- 
- publicvoid dao(Integer id) throws Exception;  
- }  
- 
- @Service("testService")  
- publicclass TestServiceImpl implements TestService {  
- @Resource
- private TestDao testDao;  
- 
- publicvoid exception(Integer id) throws Exception {  
- switch(id) {  
- case1:  
- thrownew BusinessException("11", "service11");  
- case2:  
- thrownew BusinessException("21", "service21");  
- case3:  
- thrownew BusinessException("31", "service31");  
- case4:  
- thrownew BusinessException("41", "service41");  
- case5:  
- thrownew BusinessException("51", "service51");  
- default:  
- thrownew ParameterException("Service Parameter Error");  
-         }  
-     }  
- 
- @Override
- publicvoid dao(Integer id) throws Exception {  
-         testDao.exception(id);  
-     }  
- }  
**3.2.4 Controller层代码**
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- @Controller
- publicclass TestController {  
- @Resource
- private TestService testService;  
- 
- @RequestMapping(value = "/controller.do", method = RequestMethod.GET)  
- publicvoid controller(HttpServletResponse response, Integer id) throws Exception {  
- switch(id) {  
- case1:  
- thrownew BusinessException("10", "controller10");  
- case2:  
- thrownew BusinessException("20", "controller20");  
- case3:  
- thrownew BusinessException("30", "controller30");  
- case4:  
- thrownew BusinessException("40", "controller40");  
- case5:  
- thrownew BusinessException("50", "controller50");  
- default:  
- thrownew ParameterException("Controller Parameter Error");  
-         }  
-     }  
- 
- @RequestMapping(value = "/service.do", method = RequestMethod.GET)  
- publicvoid service(HttpServletResponse response, Integer id) throws Exception {  
-         testService.exception(id);  
-     }  
- 
- @RequestMapping(value = "/dao.do", method = RequestMethod.GET)  
- publicvoid dao(HttpServletResponse response, Integer id) throws Exception {  
-         testService.dao(id);  
-     }  
- }  
**3.2.5 JSP页面代码**
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- <%@ page contentType="text/html; charset=UTF-8"%>  
- <html>  
- <head>  
- <title>Maven Demo</title>  
- </head>  
- <body>  
- <h1>所有的演示例子</h1>  
- <h3>[url=./dao.do?id=1]Dao正常错误[/url]</h3>  
- <h3>[url=./dao.do?id=10]Dao参数错误[/url]</h3>  
- <h3>[url=./dao.do?id=]Dao未知错误[/url]</h3>  
- 
- 
- <h3>[url=./service.do?id=1]Service正常错误[/url]</h3>  
- <h3>[url=./service.do?id=10]Service参数错误[/url]</h3>  
- <h3>[url=./service.do?id=]Service未知错误[/url]</h3>  
- 
- 
- <h3>[url=./controller.do?id=1]Controller正常错误[/url]</h3>  
- <h3>[url=./controller.do?id=10]Controller参数错误[/url]</h3>  
- <h3>[url=./controller.do?id=]Controller未知错误[/url]</h3>  
- 
- 
- <h3>[url=./404.do?id=1]404错误[/url]</h3>  
- </body>  
- </html>  
**3.3 集成异常处理**
**3.3.1 使用SimpleMappingExceptionResolver实现异常处理**
1、在Spring的配置文件applicationContext.xml中增加以下内容： 
Xml代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- <beanclass="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
- <!-- 定义默认的异常处理页面，当该异常类型的注册时使用 -->
- <propertyname="defaultErrorView"value="error"></property>
- <!-- 定义异常处理页面用来获取异常信息的变量名，默认名为exception -->
- <propertyname="exceptionAttribute"value="ex"></property>
- <!-- 定义需要特殊处理的异常，用类名或完全路径名作为key，异常也页名作为值 -->
- <propertyname="exceptionMappings">
- <props>
- <propkey="cn.basttg.core.exception.BusinessException">error-business</prop>
- <propkey="cn.basttg.core.exception.ParameterException">error-parameter</prop>
- 
- <!-- 这里还可以继续扩展对不同异常类型的处理 -->
- </props>
- </property>
- </bean>
2、启动测试项目，经验证，Dao层、Service层、Controller层抛出的异常（业务异常BusinessException、参数异常ParameterException和其它的异常Exception）都能准确显示定义的异常处理页面，达到了统一异常处理的目标。 
3、从上面的集成过程可知，使用SimpleMappingExceptionResolver进行异常处理，具有集成简单、有良好的扩展性、对已有代码没有入侵性等优点，但该方法仅能获取到异常信息，若在出现异常时，对需要获取除异常以外的数据的情况不适用。 
**3.3.2 实现HandlerExceptionResolver 接口自定义异常处理器**
1、增加HandlerExceptionResolver 接口的实现类MyExceptionHandler，代码如下： 
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- publicclass MyExceptionHandler implements HandlerExceptionResolver {  
- 
- public ModelAndView resolveException(HttpServletRequest request, HttpServletResponse response, Object handler,  
-             Exception ex) {  
-         Map<String, Object> model = new HashMap<String, Object>();  
-         model.put("ex", ex);  
- 
- // 根据不同错误转向不同页面
- if(ex instanceof BusinessException) {  
- returnnew ModelAndView("error-business", model);  
-         }elseif(ex instanceof ParameterException) {  
- returnnew ModelAndView("error-parameter", model);  
-         } else {  
- returnnew ModelAndView("error", model);  
-         }  
-     }  
- }  
2、在Spring的配置文件applicationContext.xml中增加以下内容： 
Xml代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- <beanid="exceptionHandler"class="cn.basttg.core.exception.MyExceptionHandler"/>
3、启动测试项目，经验证，Dao层、Service层、Controller层抛出的异常（业务异常BusinessException、参数异常ParameterException和其它的异常Exception）都能准确显示定义的异常处理页面，达到了统一异常处理的目标。 
4、从上面的集成过程可知，使用实现HandlerExceptionResolver接口的异常处理器进行异常处理，具有集成简单、有良好的扩展性、对已有代码没有入侵性等优点，同时，在异常处理时能获取导致出现异常的对象，有利于提供更详细的异常处理信息。 
**3.3.3 使用@ExceptionHandler注解实现异常处理**
1、增加BaseController类，并在类中使用@ExceptionHandler注解声明异常处理，代码如下： 
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- publicclass BaseController {  
- /** 基于@ExceptionHandler异常处理 */
- @ExceptionHandler
- public String exp(HttpServletRequest request, Exception ex) {  
- 
-         request.setAttribute("ex", ex);  
- 
- // 根据不同错误转向不同页面
- if(ex instanceof BusinessException) {  
- return"error-business";  
-         }elseif(ex instanceof ParameterException) {  
- return"error-parameter";  
-         } else {  
- return"error";  
-         }  
-     }  
- }  
2、修改代码，使所有需要异常处理的Controller都继承该类，如下所示，修改后的TestController类继承于BaseController： 
Java代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- publicclass TestController extends BaseController  
3、启动测试项目，经验证，Dao层、Service层、Controller层抛出的异常（业务异常BusinessException、参数异常ParameterException和其它的异常Exception）都能准确显示定义的异常处理页面，达到了统一异常处理的目标。 
4、从上面的集成过程可知，使用@ExceptionHandler注解实现异常处理，具有集成简单、有扩展性好（只需要将要异常处理的Controller类继承于BaseController即可）、不需要附加Spring配置等优点，但该方法对已有代码存在入侵性（需要修改已有代码，使相关类继承于BaseController），在异常处理时不能获取除异常以外的数据。 
**3.4 未捕获异常的处理**
对于Unchecked Exception而言，由于代码不强制捕获，往往被忽略，如果运行期产生了Unchecked Exception，而代码中又没有进行相应的捕获和处理，则我们可能不得不面对尴尬的404、500……等服务器内部错误提示页面。 
我们需要一个全面而有效的异常处理机制。目前大多数服务器也都支持在Web.xml中通过<error-page>(Websphere/Weblogic)或者<error-code>(Tomcat)节点配置特定异常情况的显示页面。修改web.xml文件，增加以下内容： 
Xml代码  ![收藏代码](http://cgs1999.iteye.com/images/icon_star.png)
- <!-- 出错页面定义 -->
- <error-page>
- <exception-type>java.lang.Throwable</exception-type>
- <location>/500.jsp</location>
- </error-page>
- <error-page>
- <error-code>500</error-code>
- <location>/500.jsp</location>
- </error-page>
- <error-page>
- <error-code>404</error-code>
- <location>/404.jsp</location>
- </error-page>
- 
- <!-- 这里可继续增加服务器错误号的处理及对应显示的页面 -->
**4 解决结果**
1、运行测试项目显示的首页，如下图所示： 
![](http://dl.iteye.com/upload/attachment/0069/0219/ca03eacc-fbdb-3813-b531-703c6076f329.png)
2、业务错误显示的页面，如下图所示： 
![](http://dl.iteye.com/upload/attachment/0069/0221/ac4b5d4c-a12d-3edf-ae7c-335612c129a9.png)
3、参数错误显示的页面，如下图所示： 
![](http://dl.iteye.com/upload/attachment/0069/0223/e9835731-292a-3bc0-ab5f-e14fd5305b14.png)
4、未知错误显示的页面，如下图所示： 
![](http://dl.iteye.com/upload/attachment/0069/0225/f3f05730-d337-308b-bd7e-2c7628e0bb86.png)
5、服务器内部错误页面，如下图所示： 
![](http://dl.iteye.com/upload/attachment/0069/0227/f5a5ce30-3deb-384b-ac53-1dbcb8f09aa9.png)
**5 总结**
综合上述可知，Spring MVC集成异常处理3种方式都可以达到统一异常处理的目标。从3种方式的优缺点比较，若只需要简单的集成异常处理，推荐使用SimpleMappingExceptionResolver即可；若需要集成的异常处理能够更具个性化，提供给用户更详细的异常信息，推荐自定义实现HandlerExceptionResolver接口的方式；若不喜欢Spring配置文件或要实现“零配置”，且能接受对原有代码的适当入侵，则建议使用@ExceptionHandler注解方式。 
**6 源代码**
源代码项目如下所示，为Maven项目，若需运行，请自行获取相关的依赖包。 
[点击这里获取源代码](http://dl.iteye.com/topics/download/6685e62d-b02d-3217-8c45-c4e1e8959928)
**7 参考资料**
[1] Spring MVC统一处理异常的方法 
http://hi.baidu.com/99999999hao/blog/item/25da70174bfbf642f919b8c3.html 
[2] SpringMVC 异常处理初探 
http://exceptioneye.iteye.com/blog/1306150 
[3] Spring3 MVC 深入研究 
http://elf8848.iteye.com/blog/875830 
[4] Spring MVC异常处理 
http://blog.csdn.net/rj042/article/details/7380442
