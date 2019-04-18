# el表达式无效 - z69183787的专栏 - CSDN博客
2014年04月17日 12:54:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1210
第一步： 
首先确定你是否导入的JSTL包。 
在项目上右键，Myeclipse--》Add JSTL Librarys 
第二步： 
确定用的Servlet / JSP的版本。 
查看web.xml的开头部分，如果有以下内容，表示是Servlet 2.4 / JSP 2.0。 
<web-app version="2.4" 
xmlns="http://java.sun.com/xml/ns/j2ee" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd"> 
如果有以下内容，表示是Servlet 2.3 / JSP 1.2。 
<!--CTYPE web-app PUBLIC </sp--> 
  "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN" 
  "http://java.sun.com/dtd/web-app_2_3.dtd"> 
在默认情况下，Servlet 2.3 / JSP 1.2是不支持EL表达式的，而Servlet 2.4 / JSP 2.0支持。
如果是在Servlet 2.3 / JSP 1.2下，将JSP页面的开头添加如下一句代码即可。 
<%@ page isELIgnored="false" %> 
或者，在WEB.XML中加入 
<jsp-config> 
    <el-ignored>false </el-ignored> 
</jsp-config> 
如果是在Servlet 2.4 / JSP 2.0下，检查上面的这句代码里是否把isELIgnored设为了true，还有在web.xml中，是否将jsp-config标签里的jsp-property-group标签里的el-ignored设为了true。
