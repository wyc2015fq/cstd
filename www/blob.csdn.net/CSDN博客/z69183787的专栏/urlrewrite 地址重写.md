# urlrewrite 地址重写 - z69183787的专栏 - CSDN博客
2015年07月19日 20:12:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：613
环境：
```
Maven 3.0.4
Urlrewrite 2.5.2
Myeclipse 8.6.1
```
借此机会顺便提一下 Maven Project 的创建，会了的朋友或还不想了解 Maven 的朋友，可以略过创建过程。
创建一个 Maven Project：New --> Other --> Maven Project
在弹出的对话框里，如果你想修改你项目的存放路径你可以改一下，其他的默认，我的所有项目都是存放在默认的工作区下，所以直接点NEXT
接下来找到并选择webapp，其他的默认，NEXT
![](http://pic002.cnblogs.com/images/2012/417214/2012061004435543.jpg)
填写你的 groupId、artifactId、version 和 package 信息，然后 Finish
(Finish 之后myeclipse在创建maven的时候可能会弹出错误框提示，没事，不用管它，直接按两次回车或手动关闭提示框)
![](http://pic002.cnblogs.com/images/2012/417214/2012061004453771.jpg)
下面我简单解释一下 groupId、artifactId、version 和 package：
groupId：group 翻译成中文就是组、集团的意识，groupId就是这样一个组或集团的ID标识，就拿 Apache 来说好了，Apache 大家都不陌生吧，
             Apache 官网上有非常多的项目，每个项目里面肯定能找得到 org.apache.xxxx，而 org.apache 就是这里说的 groupId
artifactId：还是拿 Apache 来说，Apache 官网上有非常多的项目，而每个项目的名字就是这里的 artifactId，换句话说，artifactId 就是你的项目名
version：这个就没有什么好解释的了吧，是项目版本的意思
package：这个也没什么好说的，就是 groupId.artifactId，创建的时候会自动帮你补全，如果没有自动帮你补全就自己手动敲上去
好，下面来看创建出来的urlrewrite-maven-example的目录结构：
![](http://pic002.cnblogs.com/images/2012/417214/2012061005083752.jpg)
src/main/resources：用来存放资源文件(默认会帮你创建这个资源文件夹)
src/main/java：用来存放 java 源码文件(需要自己手动创建)
src/main/test：用来存放测试代码(需要自己手动创建，也有人习惯命名为：src/test)
target：是用来存放 Maven 编译好的字节码的地方
Maven 的创建就讲到这，接着讲 urlrewrite，另外为了调试方便我引进了 Ant，即上面的 build.xml，这里我不讲它，没有影响。
pom.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" 
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 
  http://maven.apache.org/maven-v4_0_0.xsd">
  
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.fancy</groupId>
  <artifactId>urlrewrite-maven-example</artifactId>
  <packaging>war</packaging>
  <version>1.0</version>
  <name>urlrewrite-maven-example Maven Webapp</name>
  <url>http://maven.apache.org</url>
  <dependencies>
    <!-- urlrewrite jar -->
    <dependency>
      <groupId>org.tuckey</groupId>
      <artifactId>urlrewrite</artifactId>
      <version>2.5.2</version>
    </dependency>
  </dependencies>
  <build>
    <finalName>urlrewrite-maven-example</finalName>
  </build>
  
</project>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
web.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
  xsi:schemaLocation="http://java.sun.com/xml/ns/javaee [http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"](http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd%22%3E)>
 
  <!-- 开启URLREWRITE监听 -->
  <filter>
    <filter-name>UrlRewriteFilter</filter-name>
    <filter-class>org.tuckey.web.filters.urlrewrite.UrlRewriteFilter</filter-class>
    <init-param>
      <param-name>logLevel</param-name>
      <param-value>WARN</param-value>
    </init-param>
  </filter>
  <filter-mapping>
    <filter-name>UrlRewriteFilter</filter-name>
    <url-pattern>/*</url-pattern>
    <dispatcher>REQUEST</dispatcher>
    <dispatcher>FORWARD</dispatcher>
  </filter-mapping>
  
  <!-- 404错误 -->
  <error-page>
    <error-code>404</error-code>
    <location>/404.jsp</location>
  </error-page>
  
  <!-- 500错误 -->
  <error-page>
    <error-code>500</error-code>
    <location>/500.jsp</location>
  </error-page>
  
  <welcome-file-list>
    <welcome-file>welcome.jsp</welcome-file>
  </welcome-file-list>
  
</web-app>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
urlrewrite.xml
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE urlrewrite PUBLIC "-//tuckey.org//DTD UrlRewrite 3.2//EN" "\\urlrewrite3.2.dtd">
  
<urlrewrite>
  
  <rule>
    <note>重写index.jsp成index.html</note>
    <note>example：/index.html</note>
    <from>/index.html</from>
    <to type="forward">/index.jsp</to>
  </rule>
  
  <rule>
    <note>将view根目录下所有jsp资源重写成/xxx.action</note>
    <note>example：/index.action</note>
    <from>/([A-Za-z0-9]+).action</from>
    <to type="forward">/view/$1.jsp</to>
  </rule>
  
  <rule>
    <note>forward(转发模式)传参</note>
    <note>example：/user/param/fancy/8080.do</note>
    <from>/user/([A-Za-z0-9]+)/([A-Za-z0-9]+)/([A-Za-z0-9]+).do</from>
    <to type="forward">/view/parameter/$1.jsp?username=$2&password=$3</to>
  </rule>
  
  <rule>
    <note>redirect(重定向模式)传参,to中写绝对地址</note>
    <note>example：/admin/param/fancy/8080.do</note>
    <from>/admin/([A-Za-z0-9]+)/([A-Za-z0-9]+)/([A-Za-z0-9]+).do</from>
    <to type="redirect">/urlrewrite-maven-example/view/parameter/$1.jsp?username=$2&password=$3</to>
  </rule>
  
</urlrewrite>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
urlrewrite.xml 这个配置文件一定只能放在 WEB-INF 下，放别的地方，无论你在web.xml中怎么配置都是不行的，至少现在是不行的。
所谓重写，就一定会有规则的匹配，这里用到了正则表达式，方便起见，我只用了[A-Za-z0-9]+，简单的解释一下：
A-Z：匹配 A 到 Z 中的任意一个字符，说一句废话，同理可得，
a-z：匹配 a 到 z 中的任意一个字符
0-9：匹配 0 到 9 的任意一个字符
+：至少出现一次，即，一次或一次以上
合起来 [A-Za-z0-9]+ 的意思就是：匹配一个任意的英文字母和数字组合的字符串
<rule>：自定义匹配规则
<note>：注释，解释标签
<from>：定义具体的匹配规则
<to>：匹配成功后的目标地址
<to type="">：type的值有两个，一个是 forward (转发,参数不丢失)，一个是 redirect (重定向,地址栏显示的地址就是目标真实地址)
$1：匹配中的第一个正则表达式的字符串的值，$2,$3,$4....也是如此
& ：是 & 的实体名，代表的就是 &，这个可以在 w3school 中找到：[http://www.w3school.com.cn/tags/html_ref_entities.html](http://www.w3school.com.cn/tags/html_ref_entities.html)
第一个rule：访问 /index.html，实际上是访问了 /index.jsp
第二个rule：访问 /xx.action ， 实际上是访问了 view 目录下的 xx.jsp
第三个rule：访问 /user/xx/yy/zz.do , 实际上是访问了 view 目录下的 parameter 目录下的 xx.jsp?username=yy&password=zz
第四个rule：跟第三个其实是一样的，但是由于是重定向模式，所以地址栏就会显示真的是地址
下面是 测试的 JSP ：
/welcome.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
  <title>Urlrewrite Simple Demo</title>
 </head>
 <body>
  <h3>/welcome.jsp</h3>
  <HR/>
  <p>
   <a href="index.html" target="_blank">/index.html</a>
  </p>
  <p>
   <a href="index.action" target="_blank">/index.action</a>
  </p>
  <p>
   <a href="user/param/fancy/8080.do" target="_blank">/user/param/fancy/8080.do</a>(forward[转发模式])
  </p>
  <p>
   <a href="admin/param/fancy/8080.do" target="_blank">/user/param/fancy/8080.do</a>(redirect[重定向模式])
  </p>
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
/index.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
  <title>Urlrewrite Simple Demo</title>
 </head>
 <body>
  <h3>/index.jsp</h3>
  <h1>Hello Urlrewrite</h1>
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
/view/index.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
  <title>Urlrewrite Simple Demo</title>
 </head>
 <body>
  <h3>/view/index.jsp</h3>
  <h1>Hello Urlrewrite</h1>
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
/view/parameter/param.jsp
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
  <title>Urlrewrite Simple Demo</title>
 </head>
 <body>
  <h3>/view/parameter/param.jsp</h3>
  <h1>${param}</h1>
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Run it :
在 pom.xml 中右键，Run as --> Maven install
下面是生成的 target 资源：
![](http://pic002.cnblogs.com/images/2012/417214/2012061005574868.jpg)
将 war 文件拷贝到你的 服务器中，我用的是tomcat，然后启动tomcat：
如果你是第一次使用 urlrewrite 可能会抛一个异常：
![](http://pic002.cnblogs.com/images/2012/417214/2012061007111431.jpg)
这个异常信息是说系统找不到 tomcat bin 下的 urlrewrite3.2.dtd 文件，解决办法很简单：
方法一：将 urlrewrite3.2.dtd 放到 urlrewrite.xml 所在的目录下，即 WEB-INFO 的目录下
方法二：将 urlrewrite3.2.dtd 放到 tomcat 的 bin 目录下，这个方法一劳永逸，所以我选的是这个
urlrewrite3.2.dtd 这个文件可以在文章的最下面下载，源码包里我没有放这个文件，没问题之后，
访问：[http://localhost:8080/urlrewrite-maven-example/](http://localhost:8080/urlrewrite-maven-example/) 留意地址栏的地址，你会发现，真实的地址没有被显示出来，你看到的是伪地址：
![](http://pic002.cnblogs.com/images/2012/417214/2012061006393292.jpg)
/index.html
![](http://pic002.cnblogs.com/images/2012/417214/2012061006402051.jpg)
/index.action
![](http://pic002.cnblogs.com/images/2012/417214/2012061006410993.jpg)
/user/param/fancy/8080.do
![](http://pic002.cnblogs.com/images/2012/417214/2012061006415851.jpg)
/user/param/fancy/8080.do , 这次是重定向了，注意留意地址栏地址，这回不再是伪地址了，而是真实的地址
![](http://pic002.cnblogs.com/images/2012/417214/2012061006432690.jpg)
源码：[http://files.cnblogs.com/fancyzero/urlrewrite-maven-example.zip](http://files.cnblogs.com/fancyzero/urlrewrite-maven-example.zip)
DTD: [http://files.cnblogs.com/fancyzero/urlrewrite3.2.dtd.zip](http://files.cnblogs.com/fancyzero/urlrewrite3.2.dtd.zip)
