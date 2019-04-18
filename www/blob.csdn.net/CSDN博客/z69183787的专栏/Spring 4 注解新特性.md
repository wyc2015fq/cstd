# Spring 4 注解新特性 - z69183787的专栏 - CSDN博客
2017年01月18日 17:20:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：540
## 一、注解方面的改进
spring4对注解API和ApplicationContext获取注解Bean做了一点改进。
获取注解的注解，如@Service是被@Compent注解的注解，可以通过如下方式获取@Componet注解实例：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Annotation service = AnnotationUtils.findAnnotation(ABService.class, org.springframework.stereotype.Service.class);  
- Annotation component = AnnotationUtils.getAnnotation(service, org.springframework.stereotype.Component.class);  
获取重复注解：
比如在使用hibernate validation时，我们想在一个方法上加相同的注解多个，需要使用如下方式：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Length.List(  
-         value = {  
- @Length(min = 1, max = 2, groups = A.class),  
- @Length(min = 3, max = 4, groups = B.class)  
-         }  
- )  
- publicvoid test() {  
可以通过如下方式获取@Length：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Method method = ClassUtils.getMethod(AnnotationUtilsTest.class, "test");  
- Set<Length> set = AnnotationUtils.getRepeatableAnnotation(method, Length.List.class, Length.class);  
当然，如果你使用Java8，那么本身就支持重复注解，比如spring的任务调度注解，
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- @Repeatable(Schedules.class)  
- public@interface Scheduled {   
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Target({ElementType.METHOD, ElementType.ANNOTATION_TYPE})  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- public@interface Schedules {  
- 
-     Scheduled[] value();  
- 
- }  
这样的话，我们可以直接同时注解相同的多个注解：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Scheduled(cron = "123")  
- @Scheduled(cron = "234")  
- publicvoid test     
但是获取的时候还是需要使用如下方式：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- AnnotationUtils.getRepeatableAnnotation(ClassUtils.getMethod(TimeTest.class, "test"), Schedules.class, Scheduled.class)  
ApplicationContext和BeanFactory提供了直接通过注解获取Bean的方法：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Test
- publicvoid test() {  
-     AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext();  
-     ctx.register(GenericConfig.class);  
-     ctx.refresh();  
- 
-     Map<String, Object> beans = ctx.getBeansWithAnnotation(org.springframework.stereotype.Service.class);  
-     System.out.println(beans);  
- }  
这样可以实现一些特殊功能。
另外和提供了一个AnnotatedElementUtils用于简化java.lang.reflect.AnnotatedElement的操作，具体请参考其javadoc。   
