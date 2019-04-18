# 信息: TLD skipped. URI: http://java.sun.com/jstl/* is already defined解决方法 - z69183787的专栏 - CSDN博客
2014年07月23日 16:18:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：29253
[导读]整合Spring MVC由于用到jstl，所以假如jstl便签用的jar包，启动tomcat时控制台出现了如下的输出：2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body信息: TLD skipped. URI: http://java.sun.com/jstl/core_rt is already defined201...
整合Spring MVC由于用到jstl，所以假如jstl便签用的jar包，启动tomcat时控制台出现了如下的输出：
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/core_rt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/core
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jsp/jstl/core
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/fmt_rt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/fmt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jsp/jstl/fmt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jsp/jstl/functions
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//jakarta
```
```bash
.apache.org
```
```bash
/taglibs/standard/permittedTaglibs
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//jakarta
```
```bash
.apache.org
```
```bash
/taglibs/standard/scriptfree
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/sql_rt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/sql
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jsp/jstl/sql
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/xml_rt
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jstl/xml
```
```bash
is already defined
```
```bash
2014-3-25 23:54:49 org.apache.catalina.startup.TaglibUriRule body
```
```bash
信息: TLD skipped. URI: http:
```
```bash
//java
```
```bash
.sun.com
```
```bash
/jsp/jstl/xml
```
```bash
is already defined
```
上网查询资料时各种说法，后来我查询standard.jar的作用时，知道这个jar包与jstl.jar一起使用，但是jstl 1.2版本的就不需要这个jar包了，而本人使用的正是jstl 1.2.jar这个jar包，去掉standard.jar文件后重启tomcat就不会再出现上述信息。
