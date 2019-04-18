# EL表达式-自定义函数 - z69183787的专栏 - CSDN博客
2015年03月10日 15:55:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1610
表达式语言除了可以使用基本的运算符外，还可以使用自定义函数。通过使用自定义函数，加强了表达式语言的功能。 
EL表达式函数，主要功能是完成对数据的修改，统一化格式； 
开发步骤 
    1.开发函数处理类，处理类就是普通的类；每个函数对应类中的一个静态方法； 
    2. 建立TLD文件，定义表达式函数
    3.在WEB.XML文件中配置；（可省略）
    4.在JSP页面内导入并且使用
案例说明
    1.开发函数处理类
Java代码  ![收藏代码](http://954151190.iteye.com/images/icon_star.png)
- package mytag;  
- /**
-  * EL表达式函数处理类
-  */
- publicclass ElTag {  
- 
- publicstatic String reverse(String name){  
- returnnew StringBuffer(name).reverse().toString();  
-     }  
- 
- publicstaticint countChar(String text){  
- return text.trim().length();  
-     }  
- }  
   2.创建TLD文件；
Java代码  ![收藏代码](http://954151190.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="GBK"?>  
- <taglib xmlns="http://java.sun.com/xml/ns/j2ee"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee 
-     http://java.sun.com/xml/ns/j2ee/web-jsptaglibrary_2_0.xsd"     
-     version="2.0">     
-     <!-- 定义函数版本 -->  
-     <tlib-version>1.0</tlib-version>  
-     <!-- 定义函数名称 -->  
-     <short-name>el</short-name>  
-     <!-- 定义第一个函数 -->  
-     <function>  
-         <!-- 定义第一个函数：reverse -->  
-         <name>reverse</name>  
-         <!-- 定义函数处理类 -->  
-         <function-class>mytag.ElTag</function-class>  
-         <!-- 定义函数的对应方法 -->  
-         <function-signature>  
-             java.lang.String reverse(java.lang.String)  
-         </function-signature>  
-     </function>  
- 
-     <function>  
-         <name>countChar</name>  
-         <function-class>mytag.ElTag</function-class>  
-         <function-signature>  
-             java.lang.Integer countChar(java.lang.String)  
-         </function-signature>  
-     </function>  
- 
- </taglib>  
 3.在WEB中配置
Java代码  ![收藏代码](http://954151190.iteye.com/images/icon_star.png)
- <?xml version="1.0" encoding="UTF-8"?>  
- <web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
-     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
-     <jsp-config>  
-         <taglib>  
-             <!-- 配置标签的引用地址 JSP页面中引用时使用-->  
-             <taglib-uri>/eltag</taglib-uri>  
-             <!-- 配置标签的TLD文件地址 -->  
-             <taglib-location>/WEB-INF/ElTag.tld</taglib-location>  
-         </taglib>  
-     </jsp-config>  
-     <welcome-file-list>  
-         <welcome-file>index.jsp</welcome-file>  
-     </welcome-file-list>  
- </web-app>  
4.JSP页面引入 并且使用
Java代码  ![收藏代码](http://954151190.iteye.com/images/icon_star.png)
- <%@ taglib uri="/eltag" prefix="el" %>   
- 
-   <body>  
-     ${el:reverse("ad") }  
-   </body>  
