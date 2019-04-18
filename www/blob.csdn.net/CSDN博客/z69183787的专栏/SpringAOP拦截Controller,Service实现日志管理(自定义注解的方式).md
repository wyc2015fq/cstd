# SpringAOP拦截Controller,Service实现日志管理(自定义注解的方式) - z69183787的专栏 - CSDN博客
2016年07月01日 13:19:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：831
   首先我们为什么需要做日志管理，在现实的上线中我们经常会遇到系统出现异常或者问题。这个时候就马上打开CRT或者SSH连上服务器拿日子来分析。受网络的各种限制。于是我们就想为什么不能直接在管理后台查看报错的信息呢。于是日志管理就出现了。
         其次个人觉得做日志管理最好的是Aop，有的人也喜欢用拦截器。都可以，在此我重点介绍我的实现方式。
         Aop有的人说拦截不到Controller。有的人说想拦AnnotationMethodHandlerAdapter截到Controller必须得拦截org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter。
首先Aop可以拦截到Controller的，这个是毋容置疑的其次须拦截AnnotationMethodHandlerAdapter也不是必须的。最起码我没有验证成功过这个。我的Spring版本是4.0.3。
         Aop之所以有的人说拦截不到Controller是因为Controller被jdk代理了。我们只要把它交给cglib代理就可以了。
第一步定义两个注解：
**[java]**[view
 plain](http://blog.csdn.net/czmchen/article/details/42392985#)[copy](http://blog.csdn.net/czmchen/article/details/42392985#)
- package com.annotation;    
- 
- import java.lang.annotation.*;    
- 
- /** 
-  *自定义注解 拦截Controller 
-  */
- 
- @Target({ElementType.PARAMETER, ElementType.METHOD})    
- @Retention(RetentionPolicy.RUNTIME)    
- @Documented
- public@interface SystemControllerLog {    
- 
-     String description()  default"";    
- 
- 
- }    
- 
- package com.annotation;    
- 
- import java.lang.annotation.*;    
- 
- /** 
-  *自定义注解 拦截service 
-  */
- 
- @Target({ElementType.PARAMETER, ElementType.METHOD})    
- @Retention(RetentionPolicy.RUNTIME)    
- @Documented
- public@interface SystemServiceLog {    
- 
-     String description()  default"";    
- 
- 
- }    

第二步创建一个切点类：
**[java]**[view
 plain](http://blog.csdn.net/czmchen/article/details/42392985#)[copy](http://blog.csdn.net/czmchen/article/details/42392985#)
- package com.annotation;    
- 
- import com.model.Log;    
- import com.model.User;    
- import com.service.LogService;    
- import com.util.DateUtil;    
- import com.util.JSONUtil;    
- import com.util.SpringContextHolder;    
- import com.util.WebConstants;    
- import org.aspectj.lang.JoinPoint;    
- import org.aspectj.lang.annotation.*;    
- import org.slf4j.Logger;    
- import org.slf4j.LoggerFactory;    
- import org.springframework.stereotype.Component;    
- import org.springframework.web.context.request.RequestContextHolder;    
- import org.springframework.web.context.request.ServletRequestAttributes;    
- import javax.annotation.Resource;    
- import javax.servlet.http.HttpServletRequest;    
- import javax.servlet.http.HttpSession;    
- import java.lang.reflect.Method;    
- 
- /** 
-  * 切点类 
-  * @author tiangai 
-  * @since 2014-08-05 Pm 20:35 
-  * @version 1.0 
-  */
- @Aspect
- @Component
- publicclass SystemLogAspect {    
- //注入Service用于把日志保存数据库  
- @Resource
- private LogService logService;    
- //本地异常日志记录对象  
- privatestaticfinal Logger logger = LoggerFactory.getLogger(SystemLogAspect. class);    
- 
- //Service层切点  
- @Pointcut("@annotation(com.annotation.SystemServiceLog)")    
- publicvoid serviceAspect() {    
-     }    
- 
- //Controller层切点  
- @Pointcut("@annotation(com.annotation.SystemControllerLog)")    
- publicvoid controllerAspect() {    
-     }    
- 
- /** 
-      * 前置通知 用于拦截Controller层记录用户的操作 
-      * 
-      * @param joinPoint 切点 
-      */
- @Before("controllerAspect()")    
- publicvoid doBefore(JoinPoint joinPoint) {    
- 
-         HttpServletRequest request = ((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getRequest();    
-         HttpSession session = request.getSession();    
- //读取session中的用户  
-         User user = (User) session.getAttribute(WebConstants.CURRENT_USER);    
- //请求的IP  
-         String ip = request.getRemoteAddr();    
- try {    
- //*========控制台输出=========*//  
-             System.out.println("=====前置通知开始=====");    
-             System.out.println("请求方法:" + (joinPoint.getTarget().getClass().getName() + "." + joinPoint.getSignature().getName() + "()"));    
-             System.out.println("方法描述:" + getControllerMethodDescription(joinPoint));    
-             System.out.println("请求人:" + user.getName());    
-             System.out.println("请求IP:" + ip);    
- //*========数据库日志=========*//  
-             Log log = SpringContextHolder.getBean("logxx");    
-             log.setDescription(getControllerMethodDescription(joinPoint));    
-             log.setMethod((joinPoint.getTarget().getClass().getName() + "." + joinPoint.getSignature().getName() + "()"));    
-             log.setType("0");    
-             log.setRequestIp(ip);    
-             log.setExceptionCode( null);    
-             log.setExceptionDetail( null);    
-             log.setParams( null);    
-             log.setCreateBy(user);    
-             log.setCreateDate(DateUtil.getCurrentDate());    
- //保存数据库  
-             logService.add(log);    
-             System.out.println("=====前置通知结束=====");    
-         }  catch (Exception e) {    
- //记录本地异常日志  
-             logger.error("==前置通知异常==");    
-             logger.error("异常信息:{}", e.getMessage());    
-         }    
-     }    
- 
- /** 
-      * 异常通知 用于拦截service层记录异常日志 
-      * 
-      * @param joinPoint 
-      * @param e 
-      */
- @AfterThrowing(pointcut = "serviceAspect()", throwing = "e")    
- publicvoid doAfterThrowing(JoinPoint joinPoint, Throwable e) {    
-         HttpServletRequest request = ((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getRequest();    
-         HttpSession session = request.getSession();    
- //读取session中的用户  
-         User user = (User) session.getAttribute(WebConstants.CURRENT_USER);    
- //获取请求ip  
-         String ip = request.getRemoteAddr();    
- //获取用户请求方法的参数并序列化为JSON格式字符串  
-         String params = "";    
- if (joinPoint.getArgs() !=  null && joinPoint.getArgs().length > 0) {    
- for ( int i = 0; i < joinPoint.getArgs().length; i++) {    
-                 params += JSONUtil.toJsonString(joinPoint.getArgs()[i]) + ";";    
-             }    
-         }    
- try {    
- /*========控制台输出=========*/
-             System.out.println("=====异常通知开始=====");    
-             System.out.println("异常代码:" + e.getClass().getName());    
-             System.out.println("异常信息:" + e.getMessage());    
-             System.out.println("异常方法:" + (joinPoint.getTarget().getClass().getName() + "." + joinPoint.getSignature().getName() + "()"));    
-             System.out.println("方法描述:" + getServiceMthodDescription(joinPoint));    
-             System.out.println("请求人:" + user.getName());    
-             System.out.println("请求IP:" + ip);    
-             System.out.println("请求参数:" + params);    
- /*==========数据库日志=========*/
-             Log log = SpringContextHolder.getBean("logxx");    
-             log.setDescription(getServiceMthodDescription(joinPoint));    
-             log.setExceptionCode(e.getClass().getName());    
-             log.setType("1");    
-             log.setExceptionDetail(e.getMessage());    
-             log.setMethod((joinPoint.getTarget().getClass().getName() + "." + joinPoint.getSignature().getName() + "()"));    
-             log.setParams(params);    
-             log.setCreateBy(user);    
-             log.setCreateDate(DateUtil.getCurrentDate());    
-             log.setRequestIp(ip);    
- //保存数据库  
-             logService.add(log);    
-             System.out.println("=====异常通知结束=====");    
-         }  catch (Exception ex) {    
- //记录本地异常日志  
-             logger.error("==异常通知异常==");    
-             logger.error("异常信息:{}", ex.getMessage());    
-         }    
- /*==========记录本地异常日志==========*/
-         logger.error("异常方法:{}异常代码:{}异常信息:{}参数:{}", joinPoint.getTarget().getClass().getName() + joinPoint.getSignature().getName(), e.getClass().getName(), e.getMessage(), params);    
- 
-     }    
- 
- 
- /** 
-      * 获取注解中对方法的描述信息 用于service层注解 
-      * 
-      * @param joinPoint 切点 
-      * @return 方法描述 
-      * @throws Exception 
-      */
- publicstatic String getServiceMthodDescription(JoinPoint joinPoint)    
- throws Exception {    
-         String targetName = joinPoint.getTarget().getClass().getName();    
-         String methodName = joinPoint.getSignature().getName();    
-         Object[] arguments = joinPoint.getArgs();    
-         Class targetClass = Class.forName(targetName);    
-         Method[] methods = targetClass.getMethods();    
-         String description = "";    
- for (Method method : methods) {    
- if (method.getName().equals(methodName)) {    
-                 Class[] clazzs = method.getParameterTypes();    
- if (clazzs.length == arguments.length) {    
-                     description = method.getAnnotation(SystemServiceLog. class).description();    
- break;    
-                 }    
-             }    
-         }    
- return description;    
-     }    
- 
- /** 
-      * 获取注解中对方法的描述信息 用于Controller层注解 
-      * 
-      * @param joinPoint 切点 
-      * @return 方法描述 
-      * @throws Exception 
-      */
- publicstatic String getControllerMethodDescription(JoinPoint joinPoint)  throws Exception {    
-         String targetName = joinPoint.getTarget().getClass().getName();    
-         String methodName = joinPoint.getSignature().getName();    
-         Object[] arguments = joinPoint.getArgs();    
-         Class targetClass = Class.forName(targetName);    
-         Method[] methods = targetClass.getMethods();    
-         String description = "";    
- for (Method method : methods) {    
- if (method.getName().equals(methodName)) {    
-                 Class[] clazzs = method.getParameterTypes();    
- if (clazzs.length == arguments.length) {    
-                     description = method.getAnnotation(SystemControllerLog. class).description();    
- break;    
-                 }    
-             }    
-         }    
- return description;    
-     }    
- }    
 第三步把Controller的代理权交给cglib
在实例化ApplicationContext的时候需要加上
Xml代码 
- <!-- 启动对@AspectJ注解的支持 -->  
- **<aop:aspectj-autoproxy/>**
 在调用Controller的时候AOP发挥作用所以在SpringMVC的配置文件里加上
Xml代码 
- <!--通知spring使用cglib而不是jdk的来生成代理方法 AOP可以拦截到Controller-**>**
- **<aop:aspectj-autoproxy** proxy-target-class="true" **/>**
 第四步使用
Controller层的使用
Java代码
**[java]**[view
 plain](http://blog.csdn.net/czmchen/article/details/42392985#)[copy](http://blog.csdn.net/czmchen/article/details/42392985#)
- /** 
-     * 删除用户 
-     * 
-     * @param criteria 条件 
-     * @param id       id 
-     * @param model    模型 
-     * @return 数据列表 
-     */
- @RequestMapping(value = "/delete")    
- //此处为记录AOP拦截Controller记录用户操作  
- @SystemControllerLog(description = "删除用户")    
- public String del(Criteria criteria, String id, Model model, HttpSession session) {    
- try {    
-            User user = (User) session.getAttribute(WebConstants.CURRENT_USER);    
- if ( null != user) {    
- if (user.getId().equals(id)) {    
-                    msg = "您不可以删除自己!";    
-                    criteria = userService.selectByCriteriaPagination(criteria);    
-                }  else {    
- //删除数据并查询出数据  
-                    criteria = userService.delete(id, criteria);    
-                    msg = "删除成功!";    
-                }    
-            }    
-        }  catch (Exception e) {    
-            msg = "删除失败!";    
-        }  finally {    
-            model.addAttribute("msg", msg);    
-            model.addAttribute("criteria", criteria);    
-        }    
- //跳转列表页  
- return"user/list";    
-    }    
 Service层的使用

**[java]**[view
 plain](http://blog.csdn.net/czmchen/article/details/42392985#)[copy](http://blog.csdn.net/czmchen/article/details/42392985#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/571056/fork)
- /** 
-     * 按照分页查询 
-     * @param criteria 
-     * @return 
-     */
- //此处为AOP拦截Service记录异常信息。方法不需要加try-catch  
- @SystemServiceLog(description = "查询用户")    
- public Criteria<User> selectByCriteriaPagination(Criteria<User> criteria)    
-    {    
-        criteria.getList().get(0).getAccount();    
- //查询总数  
- long total=userMapper.countByCriteria(criteria);    
- //设置总数  
-        criteria.setRowCount(total);    
-        criteria.setList(userMapper.selectByCriteriaPagination(criteria));    
- return  criteria;    
-    }    

效果图
用户操作：
![](http://dl2.iteye.com/upload/attachment/0100/1600/79f6694c-385e-36ca-b644-e47de25ff4a7.jpg)
 异常
![](http://dl2.iteye.com/upload/attachment/0100/1602/fdd7ebec-bfe5-3595-89a3-f9d81307626f.jpg)
文章来源http://itindex.net/detail/50710-springaop-controller-service
