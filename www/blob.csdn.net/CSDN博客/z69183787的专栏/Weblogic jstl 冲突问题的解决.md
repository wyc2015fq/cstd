# Weblogic jstl 冲突问题的解决 - z69183787的专栏 - CSDN博客
2014年07月18日 16:46:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3542
**1、将web.xml 中的 servlet版本更改为 2.5**
**2、jstl包版本更改为 1.2**
或 进行如下配置：
```
<?xml version="1.0" encoding="UTF-8"?>
<weblogic-web-app xmlns="http://www.bea.com/ns/weblogic/90" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
	<context-root>/</context-root>
	<jsp-descriptor>
	 	<keepgenerated>true</keepgenerated>
    	<page-check-seconds>0</page-check-seconds>
	 </jsp-descriptor>
	<container-descriptor>
		<servlet-reload-check-secs>0</servlet-reload-check-secs>
  		<resource-reload-check-secs>0</resource-reload-check-secs>
    <prefer-application-packages>
        <package-name>antlr.*</package-name>
        <package-name>org.hibernate.*</package-name>
        <package-name>javax.persistence.*</package-name>
    </prefer-application-packages>		</strong></span>
	</container-descriptor>
	<session-descriptor>
		<timeout-secs>3600</timeout-secs>
       	<cookie-name>JSESSIONIDSstpt</cookie-name>
       <!-- 	<persistent-store-type>replicated</persistent-store-type>
        -->
	</session-descriptor>    
	<!-- 
		当<session-timeout>设置为－2，表示将使用在weblogic.xml中设置的 
		TimeoutSecs这个属性值。 
		当<session-timeout>设置为－1，表示Session将永不过期，而忽略在 
		weblogic.xml中设置的TimeoutSecs属性值。 
	 -->
	<!--  
	<context-root>/上下文根</context-root>    -->
</weblogic-web-app>
```
