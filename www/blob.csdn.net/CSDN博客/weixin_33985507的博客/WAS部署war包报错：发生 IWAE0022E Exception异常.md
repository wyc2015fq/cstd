# WAS部署war包报错：发生 IWAE0022E Exception异常 - weixin_33985507的博客 - CSDN博客
2012年03月21日 16:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
在用WAS部署应用程序的时候，通常先会打一个空的war包部署上，然后把程序覆盖到应用目录下就ok。要不然，包含各种程序的war包在部署的时候老是会出现war包异常，因为war包在部署的时候，WAS会对war包的内容格式进行严格校验。
之前，我一直也是这么做的，可是今天，又遇到了问题。
**问题描述：**部署war包是出现异常
发生 IWAE0022E Exception occurred loading deployment descriptor for module “demo.war” in EAR file “/home/was/was6/profiles/appsrv01/wstemp/714564614/upload/demo_war.ear” 异常。查看日志以获取详细信息。
查看SystemErr.log
Wrapped exception
**org.xml.sax.SAXParseException: Attribute “version” must be declared for element type “web-app”.**at org.apache.xerces.util.ErrorHandlerWrapper.createSAXParseException(Unknown Source)
at org.apache.xerces.util.ErrorHandlerWrapper.error(Unknown Source)
**问题解决：**需修改web.xml文件
修改前
<?xml version=”1.0″ encoding=”UTF-8″?>
<web-app version=”2.5″
xmlns=”http://java.sun.com/xml/ns/javaee”
xmlns:xsi=”http://www.w3.org/2001/XMLSchema-instance”
xsi:schemaLocation=”http://java.sun.com/xml/ns/javaee
http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd”>
修改后
<?xml version=”1.0″ encoding=”UTF-8″?>
<web-app id=”WebApp_ID” version=”2.4″
xmlns=”http://java.sun.com/xml/ns/j2ee”
xmlns:xsi=”http://www.w3.org/2001/XMLSchema-instance”
xsi:schemaLocation=”http://java.sun.com/xml/ns/j2ee
http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd”>
修改完web.xml之后，重新打war包，重新部署问题得到解决。
