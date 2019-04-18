# weblogic中如果使用80端口和根目录作为应用的默认目录 - z69183787的专栏 - CSDN博客
2013年02月20日 06:35:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3228
个人分类：[服务器-Weblogic](https://blog.csdn.net/z69183787/article/category/2175431)
使用根目录作为默认目录，需要新建一个工程可以叫做index，然后在在WEB-INF目录下面新建一个weblogic.xml，内容是
<!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd">
<weblogic-web-app>
<context-root>/</context-root>
</weblogic-web-app>
之后，在web.xml的welcome-file中指定一个跳转页面比如是index.jsp，之后想默认跳转到任何应用都可以通过这个跳转页进行。 
监听80端口，需要在相关域下面的config目录中修改config.xml，例如：
<server>
<name>demo</name>
<listen-port>80</listen-port>
<listen-address>192.168.1.2</listen-address>
</server>
默认端口不是80，只需要将listen-port改成80即可。 
