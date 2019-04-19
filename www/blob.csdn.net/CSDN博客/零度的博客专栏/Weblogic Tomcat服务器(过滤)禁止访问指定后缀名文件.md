# Weblogic/Tomcat服务器(过滤)禁止访问指定后缀名文件 - 零度的博客专栏 - CSDN博客
2016年05月12日 15:03:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：2254
在web.xml中添加如下代码：
          <security-constraint>
                 <web-resource-collection>
                         <web-resource-name>ABC</web-resource-name>
                         <url-pattern>*.list</url-pattern>
                         <url-pattern>*.conf</url-pattern>
                 </web-resource-collection>
                 <auth-constraint/>
         </security-constraint>
