# 一个Filter配置多个url-pattern - z69183787的专栏 - CSDN博客
2015年08月07日 17:35:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21169
最近做项目遇到一个Filter需要配置多个url-pattern,上网查了下资料，经测试，现总结下 
一、完全错误的方式 
Java代码
- <filter>  
-     <filter-name>authority</filter-name>  
-     <filter-**class**>com.util.AuthorityFilter</filter-**class**>  
- </filter>  
- <filter-mapping>  
-     <filter-name>authority</filter-name>  
-     <url-pattern>/pages/cmm/*;/pages/genbill/*</url-pattern>  
- </filter-mapping>  
二、有一定作用，但不能实现需要的效果，只会过滤最下面配置的url-pattern。 
Java代码 
- <filter>  
-     <filter-name>authority</filter-name>  
-     <filter-**class**>com.util.AuthorityFilter</filter-**class**>  
- </filter>  
- <filter-mapping>  
-     <filter-name>authority</filter-name>  
-     <url-pattern>/pages/cmm/*</url-pattern>  
-        <url-pattern>/pages/genbill/*</url-pattern>  
- </filter-mapping>  
三、现在给出正确的配置方式 
Java代码
- <filter>  
-     <filter-name>authority</filter-name>  
-     <filter-**class**>com.util.AuthorityFilter</filter-**class**>  
- </filter>  
- <filter-mapping>  
-     <filter-name>authority</filter-name>  
-        <url-pattern>/pages/genbill/*</url-pattern>  
- </filter-mapping>  
- <filter-mapping>  
-     <filter-name>authority</filter-name>  
-     <url-pattern>/pages/cmm/*</url-pattern>  
- </filter-mapping>  
