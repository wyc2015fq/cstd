# spring 使用注解来调度定时任务 - z69183787的专栏 - CSDN博客
2014年08月13日 10:39:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13437
1.在需要加载spring的配置文件里spring.xml / applicationContext.xml 添加
**[html]**[view
 plain](http://blog.csdn.net/hekang1011/article/details/17464411#)[copy](http://blog.csdn.net/hekang1011/article/details/17464411#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121998/fork)
- xmlns:task="http://www.springframework.org/schema/task"
- xsi:schemaLocation="  
- http://www.springframework.org/schema/task          
- http://www.springframework.org/schema/task/spring-task-3.0.xsd  
2.配置自动调度的包和定时开关
**[html]**[view
 plain](http://blog.csdn.net/hekang1011/article/details/17464411#)[copy](http://blog.csdn.net/hekang1011/article/details/17464411#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121998/fork)
- <!-- 自动调度需要扫描的包 -->
- <context:component-scanbase-package="*"/>
- <task:executorid="executor"pool-size="5"/>
- <task:schedulerid="scheduler"pool-size="10"/>
- <task:annotation-drivenexecutor="executor"scheduler="scheduler"/>
3.配置调度和注解调度
**[html]**[view
 plain](http://blog.csdn.net/hekang1011/article/details/17464411#)[copy](http://blog.csdn.net/hekang1011/article/details/17464411#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121998/fork)
- <!-- 配置调度 需要在类名前添加 @Service -->
-     <!--    
- <task:scheduled-tasks>
- <task:scheduledref="demoTask"method="myTestWork"cron="0/10 * * * * ?"/>
- </task:scheduled-tasks>
-     -->
- <!-- 不通过配置调度,需要在类名前 @Component/@Service,在方法名 前添加@Scheduled(cron="0/5 * * * * ? ")-->
4.在web.xml配置里添加启动时要扫描的配置文件和监听
**[html]**[view
 plain](http://blog.csdn.net/hekang1011/article/details/17464411#)[copy](http://blog.csdn.net/hekang1011/article/details/17464411#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121998/fork)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/applicationContext*.xml</param-value>
- </context-param>
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
5.添加调度的包
6.类测试
**[java]**[view
 plain](http://blog.csdn.net/hekang1011/article/details/17464411#)[copy](http://blog.csdn.net/hekang1011/article/details/17464411#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/121998/fork)
- @Service
- publicclass demo {  
- @Scheduled(cron="0/5 * * * * ? ")  
- publicvoid myTestWork(){  
- 
-         System.out.println("ssss");  
-     }  
- 
- }  
- 
