# Weblogic Tomcat服务器 （过滤）禁止访问指定后缀名文件 - z69183787的专栏 - CSDN博客
2014年09月25日 16:45:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1885
在web.xml中添加如下代码：
           <security-constraint>
                 <web-resource-collection>
                         <web-resource-name>ABC</web-resource-name>
                         <url-pattern>*.list</url-pattern>
                         <url-pattern>*.conf</url-pattern>
                 </web-resource-collection>
                 <auth-constraint/>
         </security-constraint>
