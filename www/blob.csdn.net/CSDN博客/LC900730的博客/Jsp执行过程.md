# Jsp执行过程 - LC900730的博客 - CSDN博客
2017年07月26日 11:23:23[lc900730](https://me.csdn.net/LC900730)阅读数：199
1.访问jsp页面，tomcat扫描到jsp文件，在%tomcat%/work把jsp文件翻译成java源文件 
01.hello.jsp—>01_hello_jsp.java 
2.tomcat服务器把java源文件编译成class字节码文件 
01_hello_jsp.java—>01_hello_jsp.class 
3.tomcat服务器构造01_hello_jsp类对象 
4，调用01_hello_jsp类里面的方法(调用JspService()方法)，返回内容显示到浏览器 
第一次访问走1，2，3，4，之后只需走4
## Jsp语法
#### jsp模板
```
jsp页面中的html代码就是jsp的模板
```
#### jsp表达式
```
语法：
    <%=变量或者表达式%>：向浏览器输出
    表达式的原理就是翻译成out.print("变量")，不能加分号 
    <%i %>:声明jsp的变量或方法
    注意：变量翻译成成员变量，方法翻译成成员方法
    <jsp:forward page="/">
    <%-- --%>:jsp里面的注释
```
#### jsp指令
```
include：
    用于包含其他的页面
    <%@ include file=" " %>
    tomcat读到这个代码，就把被包含的页面翻译到包含页面中，合并翻译成一个java源文件，在编译运行。这种包含叫做静态包含(源码包含)
page指令：
    告诉tomcat服务器如何翻译jsp文件
    <%@ page 
        language="java" 
        import="java.util.*"
        contentType="text/html; charset=UTF-8"
        pageEncoding="UTF-8"
        isErrorPage="false"
        buffer="8kb"
        session="true"
        isELIgnored="false"    
    %>
全局配置错误页面：
在web.xml中
    <error-page>
        <error-code>500</error-code>
        <location>/common/500.jsp</location>
    </error-page>
    <error-page>
        <error-code>404</error-code>
        <location>/common/404.html</location>
    </error-page>
```
