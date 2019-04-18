# weblogic中优先加载应用中的jar包 prefer-application-packages - z69183787的专栏 - CSDN博客
2014年05月14日 15:13:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16754
其他帮助连接：http://shuwen.iteye.com/blog/1124220
仅针对10.3及以上版本。 
在WEB-INF下面添加weblogic.xml文件。
|`01`|`<?``xml``version``=``"1.0"``encoding``=``"UTF-8"``?>`|
|`02`|`<``weblogic-web-app`|
|`03`|`        ``xmlns``=``"http://xmlns.oracle.com/weblogic/weblogic-web-app"`|
|`04`|`        ``xmlns:xsi``=``"http://www.w3.org/2001/XMLSchema-instance"`|
|`05`|`        ``xsi:schemaLocation="http://java.sun.com/xml/ns/javaee`|
|`06`|`http://java.sun.com/xml/ns/javaee/ejb-jar_3_0.xsd`|
|`07`|`http://xmlns.oracle.com/weblogic/weblogic-web-app`|
|`08`|`http://xmlns.oracle.com/weblogic/weblogic-web-app/1.2/weblogic-web-app.xsd">`|
|`09`|`    ``<``jsp-descriptor``>`|
|`10`|`        ``<``working-dir``>app_workingDir</``working-dir``>`|
|`11`|`    ``</``jsp-descriptor``>`|
|`12`|`    ``<``container-descriptor``>`|
|`13`|`        ``<!--<prefer-web-inf-classes>true</prefer-web-inf-classes>-->`|
|`14`|`        ``<``prefer-application-packages``>`|
|`15`|`            ``<``package-name``>org.apache.commons.lang.*</``package-name``>`|
|`16`|`            ``<``package-name``>antlr.*</``package-name``>`|
|`17`|`            ``<``package-name``>org.hibernate.*</``package-name``>`|
|`18`|`            ``<``package-name``>javax.persistence.*</``package-name``>`|
|`19`|`        ``</``prefer-application-packages``>`|
|`20`|`    ``</``container-descriptor``>`|
|`21`|`    ``<``context-root``>/app</``context-root``>`|
|`22`|`</``weblogic-web-app``>`|
其中prefer-web-inf-classes和prefer-application-packages只能二选一。 
使用此方法对hibernate jpa2.0加载时可不用修改weblogic启动脚本的CLASSPATH。 
注意xml文件的xsd文件声明必须正确。 
我用此方法解决了在weblogic10.3.6和hibernate3.6.10的jpa jar包冲突。 
Invocation of init method failed; nested exception is java.lang.ArrayStoreException: sun.reflect.annotation.EnumConstantNotPresentExceptionProxy 
如只设定prefer-web-inf-classes为true 也不能解决以上问题。 
