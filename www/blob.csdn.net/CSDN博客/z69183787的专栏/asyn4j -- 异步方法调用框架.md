# asyn4j -- 异步方法调用框架 - z69183787的专栏 - CSDN博客
2014年06月23日 10:58:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7561
asyn4j 是一个java异步方法调用框架，基于消费者与生产者模式。包括了异步方法执行，异步回调执行，异步工作缓存模块.支持Spring. 
让我们写异步方法不再写很多的相关多线程代码。用asyn4j轻松搞定异步方法调用.提高程序的响应能力. 
设计图 
![](http://dl.iteye.com/upload/picture/pic/70272/2688411d-a56d-35c3-9183-83cfc6a87659.jpg)
1.调用普通方法
主方法 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {   
- // 初始化异步工作服务 
-         AsynService asynService =  AsynServiceImpl.getService(300, 3000L, 3, 2);   
- // 启动服务 
-         asynService.init();   
- // 异步回调对象 
-         AsynCallBack back = new TargetBack();   
- for (int i = 0; i < 1000; i++) {   
- // 添加加异步工作- TargetService 的 test 方法 ，方法参数 asynej+ i 
-                 asynService.addWork(new Object[] { "asyn4j" + i },   
-                                 TargetService.class, "test", new TargetBack());   
- //实例化目标对象再调用 
- // TargetService targetService = new TargetService (); 
- //asynService.addWork(new Object[] { "asyn4j" + i }, 
- //              targetService , "test", new TargetBack()); 
- 
-         }   
- }  
异步调用对象 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicclass TargetService {   
- public String test(String name){   
-                 System.out.println(name +" test is execute!");   
- return name;   
-         }   
- 
- }  
回调方法 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- //回调需继承AsynCallBack抽象类 
- publicclass TargetBack extends AsynCallBack {   
- 
- @Override
- publicvoid doNotify() {   
- //输出异步方法调用返回结果 
-                 System.out.println(this.methodResult);   
- 
-         }   
- 
- }  
2.调用Spring Bean的异步方法
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- 调用 Spring testBean 的 myName 方法   
-    applicationContext.xml 加入   
-     <bean id="springBeanUtil"class="com.googlecode.asyn4j.spring.AsynSpringUtil">   
-         </bean>   
- 
-   <bean id="asynService"class="com.googlecode.asyn4j.spring.AsynServiceFactoryBean">   
- 
-    </bean>   
- 
- 
- publicclass TestMain {   
- 
- public AsynService asynService;   
- 
- publicvoid setAsynService(AsynService asynService) {   
- this.asynService = asynService;   
-         }   
- 
- publicvoid maintest(){   
- for(int i=0;i<10000;i++){   
-                         asynService.addWorkWithSpring(new Object[] { "panxiuyan" + i }, "testBean", "myName");   
-                 }   
-         }   
- 
- }  
3.使用异步工作缓冲器
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- AsynService anycService = AsynServiceImpl.getService(300, 3000L, 100,   
- 100);   
-                 anycService.setWorkQueueFullHandler(new CacheAsynWorkHandler(100));   
-                 anycService.init();  
当工作队列中的工作超过300个时，异步工作将由CacheAsynWorkHandler?处理; 
4.异步工作优级
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- 默认优先级为5  ,相关API   
- //目标对象为指定实例 
- publicvoid addWork(Object[] params, Object tagerObject, String method,AsynCallBack asynCallBack, int weight);   
- 
- //目标对象为class 
- publicvoid addWork(Object[] params,Class clzss,String method,AsynCallBack asynCallBack,int weight);   
- 
- //Spring对应 
- publicvoid addWorkWithSpring(Object[] params,String target,String method,AsynCallBack asynCallBack,int weight);  
看到这里感兴趣了吗?想了解更多查看如下资料或站内信联络. 
项目地址:[http://asyn4j.googlecode.com](http://asyn4j.googlecode.com/)
源码SVN : http://asyn4j.googlecode.com/svn/branches/asyn4j 
WIKI: [http://code.google.com/p/asyn4j/wiki/user_guide](http://code.google.com/p/asyn4j/wiki/user_guide)
