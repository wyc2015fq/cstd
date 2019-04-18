# web.xml不认taglib标签的解决方法 - z69183787的专栏 - CSDN博客
2014年08月04日 13:14:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2235
在web.xml不认<taglib>解决办法：
如果头是这样的：
<!DOCTYPE web-app PUBLIC "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN" "http://java.sun.com/dtd/web-app_2_3.dtd">
直接在后面加   
<taglib>
<taglib-uri>/WEB-INF/struts-logic.tld</taglib-uri>
<taglib-location>/WEB-INF/struts-logic.tld</taglib-location>
</taglib>
如果头是这样的：
<web-appxmlns="http://java.sun.com/xml/ns/j2ee"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"version="2.4"xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee   http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
在后面加   
<jsp-config>
<taglib>
<taglib-uri>/WEB-INF/validator-user.tld</taglib-uri>
<taglib-location>/WEB-INF/validator-user.tld</taglib-location>
</taglib>
</jsp-config>
