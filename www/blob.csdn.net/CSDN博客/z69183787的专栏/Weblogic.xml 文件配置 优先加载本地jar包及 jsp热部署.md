# Weblogic.xml 文件配置 优先加载本地jar包及 jsp热部署 - z69183787的专栏 - CSDN博客
2013年12月25日 14:25:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6193
具体配置文件 各标签属性，可参考
[http://edocs.weblogicfans.net/wls/docs92/webapp/weblogic_xml.html](http://edocs.weblogicfans.net/wls/docs92/webapp/weblogic_xml.html) 或官网
[http://docs.oracle.com/cd/E23943_01/web.1111/e13712/weblogic_xml.htm](http://docs.oracle.com/cd/E23943_01/web.1111/e13712/weblogic_xml.htm)
<?xml version="1.0" encoding="UTF-8"?>
<weblogic-web-app xmlns="http://www.bea.com/ns/weblogic/90" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<context-root>/schedule</context-root>
<jsp-descriptor>
<keepgenerated>true</keepgenerated>
<page-check-seconds>0</page-check-seconds>
</jsp-descriptor>
<container-descriptor>
<servlet-reload-check-secs>0</servlet-reload-check-secs>
<resource-reload-check-secs>0</resource-reload-check-secs>
<prefer-web-inf-classes>true</prefer-web-inf-classes>
</container-descriptor>
<session-descriptor>
<timeout-secs>3600</timeout-secs>
<cookie-name>JSESSIONIDSCHEDULE</cookie-name>
       <!-- <persistent-store-type>replicated</persistent-store-type>
        -->
</session-descriptor>    
<!-- 
当<session-timeout>设置为－2，表示将使用在weblogic.xml中设置的 
TimeoutSecs这个属性值。 
当<session-timeout>设置为－1，表示Session将永不过期，而忽略在 
weblogic.xml中设置的TimeoutSecs属性值。 
-->
<!--  
<context-root>/上下文根</context-root>    -->
</weblogic-web-app>
