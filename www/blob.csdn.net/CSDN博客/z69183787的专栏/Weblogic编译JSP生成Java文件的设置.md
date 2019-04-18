# Weblogic编译JSP生成Java文件的设置 - z69183787的专栏 - CSDN博客
2014年01月06日 18:32:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3791
运行自己配置的web应用，往往只能看见weblogic编译之后的class文件。
而看不见编译前的java的文件。为了调试方便，我们有时候是想看编译前的java文件的。 
在weblogic.xml中加入
<jsp-descriptor>
<jsp-param>
    <param-name>keepgenerated</param-name> 
    <param-value>true</param-value>
</jsp-param>
</jsp-descriptor>
即可。
如果没有，在WEB-INF中建立一个weblogic.xml文件。
增加
<?xml version="1.0" encoding="UTF-8"?> 
<!DOCTYPE weblogic-web-app 
    PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" 
    "[http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd](http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd)" >
<weblogic-web-app>
    <jsp-descriptor>
        <jsp-param>
            <param-name>keepgenerated</param-name>
            <param-value>true</param-value>
        </jsp-param>
    </jsp-descriptor>
</weblogic-web-app>
看看web应用在weblogic中生成临时文件夹中，通常在(XXX代表您的项目名)
XXXdomain\myserver\.wlnotdelete\extract目录下，如果发现还是没有生成Java文件，那请将服务停止后，再将该目录下的myserver_XXX_XXX下的所有文件删除后，重启服务后就可以看到生成的Java文件，为程序查错就更方便了
