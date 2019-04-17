# JavaWeb —— JSP 指令 - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:43:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
所属专栏：[JavaWeb 学习笔记](https://blog.csdn.net/column/details/34361.html)









# 【概述】

JSP 指令主要用于设定整个 JSP 页面范围内都有效的相关信息，其是被服务器解释并执行的，只是告诉 JSP 引擎如何处理 JSP 页面中的其他部分，不会产生任何内容输出到网页中。

其语法格式为：**<%@ 指令名 属性1="属性值1" 属性2="属性值2" ... %>**

JSP 中，有 page、include、taglib 三条指令，不同指令包含不同的属性，一条指令中可设置多个属性。

# 【page 指令】

page 指令是 JSP 页面最常用的指令，用于定义整个 JSP 页面的相关属性，这些属性在 JSP 中被服务器解析成 Servlet 时会转换成相应的 Java 代码。

该指令可以放在 JSP 页面中的任何位置，但无论放置在何位置，其作用的都是整个 JSP 页面，为保证程序的可读性和规范性，常放置在 JSP 页面的起始位置。

语法格式：**<%@ page attr1="value1" attr2="value2" ... %>**

常用的属性有：
|language|用于设置 JSP 页面使用的语言，目前只支持 Java|
|----|----|
|extends|设置 JSP 页面继承的 Java 类，该属性不常用，且有可能会影响服务器的性能|
|import|设置 JSP 导入的类包，在嵌入 Java 代码片段时，这些 Java 代码在调用 API 时需要导入相应的包|
|contentType|用于设置 JSP 页面的响应类型，指定返回页面的类型与编码类型，实际调用的是 JSP 页面的 response 对象内置的 response.setContextType() 方法，而其属性值可在 Tomcat 服务器 conf 目录下的 web.xml 中找到，最常用的为 "text/html; charset=UTF-8"|
|pageEncoding|设置 JSP 页面编码格式，JSP 页面中的所有代码都使用该属性指定的字符集，其值通常与 contentType 属性中的 charse 一致|
|session|指定 JSP 页面能否使用 session 内置对象，其值有 "true"、"false"|
|isErrorPage|指定当前 JSP 页面是否设置为错误处理页面(用于以处理另一 JSP 页面的错误)，其值有 "true"、"false"|
|errorPage|用于指定处理当前 JSP 页面异常错误的另一 JSP 页面，其属性值是设为错误处理页面的 url 字符串，且指定的 JSP 错误处理页面必须设 isErrorPage 属性为 "true"|
|isELIgnored|指定当前 JSP 页面是否可以使用 EL 表达式，通常取值为 "true"|
|buffer|设置JSP的out输出对象使用的缓冲区大小，默认为8KB，且单位只能使用KB，建议使用8的倍数16、32、64、128等|
|autoFlush|设置JSP页面缓存满时，是否自动刷新缓存，默认值为 "true"，若设为"false"，则当缓存满时将抛出异常|

**注：一般情况下，不建议通过输入 URL 的方式来直接访问错误处理页面，因此，在 Tomcat 服务器中，一般将错误处理页面放置在 WEB-INF 目录下，因为在该目录下的文件，无法通过输入 URL 的方式来进行访问，只能通过转发来进行访问**

除 isErrorPage 与 errorPage 配合使用外，还可以通过配置当前 WEB 应用的 web.xml 文件来配置错误页面

例如：

```
<error-page>
    <error-code>500</error-code><!--  指定出错的代码 -->
    <location>/WEB-INF/error.JSP</location><!-- 指定响应页面的位置 -->
</error-page>

<error-page>
    <exception-type>java.lang.ArithmeicException</exception-type><!-- 指定异常类型 -->
    <location>/WEB-INF/error.JSP</location><!-- 指定响应页面的位置 -->
</error-page>
```

page 指令实例

```
<!-- 设置JSP页面使用Java语言 -->
<%@ page language="java" %>    

 <!-- 导入java.util包 -->
<%@ page import="java.util.*" %>   

<!-- 编码格式默认为ISO-8859-1,该编码格式不支持中文,若想支持中文，设置为GBK或UTF-8均可 -->
<%@ page pageEncoding="GBK" %>    

<!-- 设置JSP页面的MIME类型与字符编码 -->
<%@ page contentType="text/html;charset=UTF-8" %>    

<!-- 允许JSP页面使用HTTP的会话对象 -->
<%@ page session="true" %>

<!-- 设置out输出对象缓冲区大小为32KB -->
<%@ page buffer="32KB" %>

<!-- 设置JSP页面不自动刷新缓存 -->
<%@ page autoFlush="false" %>

<!-- 将当前JSP页面设为错误处理页面 -->
<%@ page isErrorPage="true" %>

<!-- 设置处理JSP页面异常错误的页面 -->
<%@ page ErrorPage="error/loginErrorPage.jsp" %>
```

# 【include 指令】

include 指令用于通知 JSP 引擎在翻译当前 JSP 页面时，将其他文件合并进当前 JSP 页面转换的 Servlet 源文件中，即将当前 JSP 界面与静态引入的界面进行结合最终成为一个 Servlet，因此在包含和被包含的文件中不能有相同名称的变量。

用 include 指令将文件插入 JSP 页面的方式称为静态引入。

在应用 include 指令进行文件包含时，为使整个页面的层次结构不发生冲突，建议在**被包含的页面**中将 <html>、<body> 等标记删除，因为在**包含该页面的文件**中已指定了这些标记。

语法格式：**<%@ include file="path" %>**

该指令只有一个 file 数学，用于指定要包含文件的相对路径，且不能是通过 <%= %> 表达式所代表的文件。

```
<%@ include file="top.jsp" %>
<%@ include file="copyright.jsp" %>
```

# 【taglib 指令】

taglib 指令标识声明该页面中所使用的标签库，同时引用该标签库并指定标签的前缀，在页面中引用标签库后，就可通过前缀来引用标签库中的标签。

语法格式：**<%@ taglib prefix="tagPrefix" uri="tagURI" %>**

prefix 用于指定标签的前缀，该前缀不能命名为 jsp、jspx、java、javax、sun、sunw、servlet；uri 用于指定标签库文件的存放位置

例如：

```
<%@ taglib prefix="c" uri="http://java.sum.com/jsp/jstl/core" %>

<c:forEach>
    ...
</c:forEach>
```





