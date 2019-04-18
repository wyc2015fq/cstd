# IntrospectorCleanupListener 简介 - z69183787的专栏 - CSDN博客
2014年06月04日 11:13:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：909
org.springframework.web.util.IntrospectorCleanupListener监听器主要负责处理由JavaBean Introspector使用而引起的缓冲泄露,
它是一个在web应用关闭时清除JavaBean Introspector的监听器,在web.xml中注册这个listener可以保证在web应用关闭的时候释放掉与这个web应用相关的class loader和由它管理的类.
  如果你使用了JavaBeans Introspector来分析应用中的类.Introspector缓冲中会保留这些类的引用.结果在你的应用关闭的时候,这些类以及web应用相关的class loader没有被垃圾回收.
  不幸的是,清除Introsepctor的唯一方法是刷新整个缓冲,这是因为我们没有办法判断哪些是属于你的应用的引用,所以删除被缓冲的Introspection会导致把这台电脑上的所有应用的introspection都删掉.
  spring托管的bean不需要使用这个监听器.因为spring它自己的introspection所使用的缓冲在分析完一个类之后会被马上从javabeans introspector缓冲中清除掉.
应用程序中的类从来不直接使用JavaBeans Introspector,所以他们一般不会导致内部查看资源泄露,但是一些类库和框架往往会产生这个问题.如:Struts和Quartz.单个的内部查看泄露会导致整个的web应用的类加载器不能进行垃圾回收,在web应用关闭之后,你会看到此应用的所在静态资源如单例,这个错误不是由该类自身引起
 的.Java代码
package  org.springframework.web.util;  
import  java.beans.Introspector;  
import  javax.servlet.ServletContextEvent;  
import  javax.servlet.ServletContextListener;  
public   class  IntrospectorCleanupListener  implements  ServletContextListener   {  
     public   void  contextInitialized(ServletContextEvent event)   {  
    }   
      public   void  contextDestroyed(ServletContextEvent event)   {  
        Introspector.flushCaches();  
    }   
}  
package  org.springframework.web.util;
import  java.beans.Introspector;
import  javax.servlet.ServletContextEvent;
import  javax.servlet.ServletContextListener;
public   class  IntrospectorCleanupListener  implements  ServletContextListener   {
     public   void  contextInitialized(ServletContextEvent event)   {
    }
      public   void  contextDestroyed(ServletContextEvent event)   {
        Introspector.flushCaches();
    }
}
用法如下
Java代码
用法很简单，就是在web.xml中加入:   
<listener>   
<listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>   
</listener> 
用法很简单，就是在web.xml中加入:
<listener>
<listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
</listener>
就这么简单？？~~~
“Introspector.flushCaches();”就可以把缓存中的内容清楚掉了，那hibernate使用的资源呢，如何处理？
