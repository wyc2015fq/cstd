# weblogic的热部署 - z69183787的专栏 - CSDN博客
2012年10月25日 14:11:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：20398
```
<jsp-descriptor>
02
    <!-- 在 JSP 编译过程中保存作为中间步骤生成的 Java 文件。除非此参数设置为 true，否则编译完成后将删除所有中间 Java 文件。 -->
03
    <keepgenerated>true</keepgenerated>
04
    <!-- 检查JSP文件的时间间隔（秒），以便查看 JSP 文件是否已更改以及是否需要重新编译。如果已发生更改，还会检查依赖关系并递归重新加载。 -->
05
    <!-- -1表示永不检查页面。该值为生产环境中的默认值。0表示总是检查页面。1表示每秒检查一次页面。该值为开发环境中的默认值。 -->
06
    <!-- <page-check-seconds>1</page-check-seconds> -->
07
    <!-- 如果设置为 true，当部署或重新部署 Web应用程序时，或启动 WebLogic时，WebLogic Server会自动预编译所有已修改的JSP。 -->
08
    <!-- <precompile>true</precompile> -->
09
    <!-- 如果设置为 true，即使编译期间其中某些 JSP 失败，WebLogic Server也会继续预编译所有已修改的 JSP。仅当 precompile设置为true时才生效。 -->
10
    <!-- <precompile-continue>true</precompile-continue> -->
11
    <debug>true</debug>
12
</jsp-descriptor>
```
**11g 示例：**
```
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
<context-root>/上下文根</context-root>    -->
</weblogic-web-app>
```
大家在进行web开发的时候，都希望自己修改的内容在不手动编译、不重启服务的时候立即生效，这样可以极大的方便开发和调试。这个就是我今天要讲的weblogic的热部署。
      Weblogic能实现热部署功能，而且有自己的热部署机制，一般的应用在Weblogic的控制台中有如下三个配置的页面：
            servlet-reload-check-secs
             resource-reload-check-secs
            JSPpage-check-seconds
    其中weblogic.xml对于这三个设置选项的参数介绍：
** servlet-reload-check-secs**
<servlet-reload-check-secs>元素定义 WebLogic Server是否执行 servlet检查以查看 servlet是否已更改，如果已更改，是否重新加载。
§       值 -1 表示永不检查 servlet。该值为生产环境中的默认值。
§       值 0 表示总是检查 servlet。
§       值 1 表示每秒检查一次 servlet。该值为开发环境中的默认值。
**resource-reload-check-secs**
使用 <resource-reload-check-secs>元素针对 Web应用程序范围内资源路径中发现的缓存资源执行元数据缓存。该参数标识 WebLogic
 Server检查资源是否发生修改的频率，如果已修改，则重新加载。
§       值 -1 表示元数据进行缓存，但从不对磁盘进行检查以便找出所做的更改。建议在生产环境中使用该值，以提升性能。
§       值 0 表示不执行元数据缓存。持续更改文件的客户必须将该参数设置为大于或等于 0的一个值。
§       值 1 表示每秒重新加载一次。该值为开发环境中的默认值。
**JSPpage-check-seconds**
设置 WebLogic Server检查 JSP文件的时间间隔（秒），以便查看 JSP文件是否已更改以及是否需要重新编译。如果已发生更改，还会检查依赖关系并递归重新加载。
§       值 -1 表示永不检查页面。该值为生产环境中的默认值。
§       值 0 表示总是检查页面。
§       值 1 表示每秒检查一次页面。该值为开发环境中的默认值。
所以分别配置Servlet、Resource和Jsp的来控制对应三类所控制的内容的热部署时间间隔，如果设置为-1的话那么这一项就不支持热部署。当值配置为1的话就可以支持热部署。
为了实现程序的热部署，我们需要以下几个步骤：
**1：修改weblogic的启动参数**
使用domain中的startWebLogic.cmd来启动weblogic的话，则在domain/bin下的setDomainEnv.cmd中将该参数配置上，在该文件最后添加设置参数命令:set JAVA_OPTIONS=%JAVA_OPTIONS% -Dorg.apache.tapestry.disable-caching=true -Dorg.apache.tapestry.enable-reset-service=true
 如图:
[](http://photo.blog.sina.com.cn/showpic.html#blogid=695278770100kkd1&url=http://s14.sinaimg.cn/orignal/69527877g8f304db047cd)
![](https://img-my.csdn.net/uploads/201204/18/1334758441_4057.jpg)
**2. 检查weblogic.xml配置文件，其中如果有：**
<container-descriptor>
  <servlet-reload-check-secs>0</servlet-reload-check-secs>
  <resource-reload-check-secs>0</resource-reload-check-secs>
 </container-descriptor>
<jsp-descriptor>
<jsp-param>
   <param-name>pageCheckSeconds</param-name>
   <param-value>0</param-value>
  </jsp-param>
 </jsp-descriptor>
请将上述数值都修改为0。
**3.配置Eclipse中的工程自动编译**
       检查Eclipse中工程配置的默认编译输出路径是否为projectHome/web/WEB-INF/classes。
       修改步骤为：在Package Explorer视图中的工程上右键-->属性(properties)-->Java构件路径(Java Build Path)-->源代码(Source)-->默认输出路径(Default output folder)。这里修改为web/WEB-INF/classes即可。可以通过右边的浏览（Browse）按钮来选择。确保
 project->build automatically   已经被选上。
[](http://photo.blog.sina.com.cn/showpic.html#blogid=695278770100kkd1&url=http://s4.sinaimg.cn/orignal/69527877g8f3041c1f293)
[](http://photo.blog.sina.com.cn/showpic.html#blogid=695278770100kkd1&url=http://s4.sinaimg.cn/orignal/69527877g8f3041c1f293)
**4. 检查ie选项中是否设置了每次访问本页时检查最新。**
设置方法：工具-->Internet选项-->常规-->浏览历史记录中的设置，在弹出框中设置（针对ie7，8）。
![](https://img-my.csdn.net/uploads/201204/18/1334758465_3406.jpg)[](http://photo.blog.sina.com.cn/showpic.html#blogid=695278770100kkd1&url=http://s12.sinaimg.cn/orignal/69527877g8f305e3394eb)
