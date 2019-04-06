# JSP 学习笔记

 

## JSP 基础语法

### 页码指令page

指令标签不会产生任何内容输出到网页中，它主要用于定义整个JSP页面的相关信息，例如使用的语言、导入的类包、指定错误处理页面等。其语法格式如下：

```
<%@ directive attribute="value" attributeN="valueN" ……%>
```

directive：指令名称，JSP中包含page、include、taglib共3个指令。 
 attribute：属性名称，不同的指令包含不同的属性。 
 value：属性值，为指定属性赋值的内容。 
 注意： 
 标签中的`<%@`和`%>`是完整的标记，不能再添加空格，但是标签中定义的各种属性之间以及与指令名之间可以有空格。 
 JSP指令有3种，分别是page、include和taglib。下面分别介绍这些指令的作用与用法。

page指令是JSP页面最常用的指令，它用于定义整个JSP页面的相关属性。这些属性在JSP被服务器解析成Servlet时会转换为相应的Java程序代码。page指令的语法格式如下：

```
<%@ page attr1="value1" attr2="value2" ……%>
```

page指令包含的属性有15个，下面对一些常用的属性进行介绍： 
 1．language属性 
 该属性用于设置JSP页面使用的语言，目前只支持Java语言，以后可能会支持其它语言，例如“C++”、“C#”等。该属性的默认值是Java。 
 例如：

```
<%@ page language="java" %>
```

2．extends属性 
 该属性用于设置JSP页面继承的Java类，所有JSP页面在执行之前都会被服务器解析成Servlet，而Servlet是由Java类定义的，所以JSP和Servlet都可以继承指定的父类。该属性并不常用，而且有可能影响服务器的性能优化。

3．import属性 
 该属性用于设置JSP导入的类包，JSP页面可以嵌入Java代码片段，这些Java代码在调用API时需要导入相应的类包。 
 例如：

```
<%@ page import="java.util.*" %>
```

4．pageEccoding属性 
 该属性用于定义JSP页面的编码格式，也就是指定文件编码，JSP页面中的所有代码都使用该属性指定的字符集，如果该属性值设置为“iso-8859-1”，那么这个JSP页面就不支持中文字符。通常我们设置编码格式为“GBK”，因为它可以显示简体中文和繁体中文。 
 例如：

```
<%@ page pageEncoding="GBK"%>
```

5．contentType属性 
 该属性用于设置JSP页面的MIME类型和字符编码，浏览器会根据该属性指定的类型和编码显示网页内容。 
 例如：

```
<%@ page contentType="text/html; charset=GBK"%>
```

注意： 
 Jsp页面默认的编码是ISO-8859-1，即“charset=iso8859-1”，该编码是不支持中文的，应该将其设置为（GBK、GB2312、GB18030），那么浏览器在显示该页面时，就不会产生中文乱码问题。

6．session属性 
 该属性指定JSP页面是否使用HTTP的session会话对象，其属性值是boolean类型，可选值为true和false，默认值是true，可以使用session会话对象，如果设置为false，当前JSP页面将无法使用session会话对象。 
 例如：

```
<%@ page session="false"%>
```

上述代码设置JSP页面不使用session对象，任何对session对象的引用都会发生错误。 
 说明： 
 session是JSP的内置对象之一。

7．buffer属性 
 该属性用于设置JSP的out输出对象使用的缓冲区大小，默认大小是8kb，切单位只能使用kb。建议程序开发人员使用8的倍数16、32、64、128等作为该属性的属性值。 
 例如：

```
<%@ page buffer="128kb"%>
```

说明： 
 out对象是JSP的内置对象之一。

8．autoFlush属性 
 该属性用于设置JSP页面缓存满的时候，是否自动刷新缓存，默认值为true，如果设置成false，那么在缓存被填满时，会抛出异常。 
 例如：

```
<%@ page autoFlush="false"%>
```

上述代码取消页面缓存的自动刷新。

9．info属性 
 该属性用于设置JSP页面的相关信息，这个信息可以在Servlet接口的getServletInfo()方法中获取。 
 例如：

```
<%@ page info="这是一个登录页面，是系统的入口"%>
```

10．isErrorPage属性 
 这个属性可以将当前JSP页面设置成错误处理页面，错误处理页面用于处理另一个JSP页面的错误，也就是异常处理。这意味着当前JSP页面业务的改变。 
 例如：

```
<%@ page isELIgnored="true"%>
```

11．errorPage属性 
 该属性用于指定处理当前JSP页面异常错误的另一个JSP页面，指定的JSP错误处理页面必须设置isErrorPage属性为true。errorPage属性的属性值是一个url字符串。 
 例如：

```
<%@ page errorPage="error/loginErrorPage.jsp"%>
```

注意： 
 如果设置该属性，那么在web.xml文件中定义的任何错误页面将被忽略，优先使用该属性定义的错误处理页面

12．isELIgnored属性 
 该属性用于定义JSP页面是否忽略EL表达式的使用，在Servlet2.4版本时默认值为false，即JSP支持EL表达式，而在Servlet2.3以前的版本中该属性的默认值为true，这里使用Tomcat6.0服务器支持Servlet2.4，所以默认值是false，可以直接使用EL表达式。  
 例如：

```
<%@ page isELIgnored="false"%>
```

### 文件包含指令include

include指令用于文件包含，它可以在JSP页面中包含另一个文件内容，但是该指令支持静态包含，也就是说被包含文件中所有内容原样包含到该JSP页面中，被包行的文件可以是一段Java代码、html代码或者是另一个JSP页面。 
 例如：

```
<%@include file="validate.jsp" %>
```

上述代码将当前JSP文件相同位置的validate.jsp文件包含进来。 
 file属性用于指定被包含的文件，其值是当前JSP页面文件的相对URL路径。 
 例001  分别创建用于显示页头和页尾的top.jsp页面与down.jsp页面，创建index.jsp页面，在该页面中通过include指令包含top.jsp页面与down.jsp页面。 
 （1）top.jsp页面中显示整个网页的页头部分，具体代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="gbk"%>
<img src="images/top.gif">
```

（2）down.jsp页面中显示整个网页的页尾部分，具体代码如下：

```
<%@ page language="java" import="java.util.*,java.util.Date" pageEncoding="gbk"%>
<img src = "images/down.gif"/>
```

（3）在index.jsp页面中使用include指令包含top.jsp页面与down.jsp页面，具体代码如下：

```
<body>
      <%@ include file="top.jsp"%>                <!-- 在页面中包含top.jsp页 -->
      <table width="777" height="482" border="0"     
            background="images/center.gif">          <!-- 定义表格，表格的背景显示图片 -->
            <tr>
                  <td height="93">
                        &nbsp;
                  </td>
            </tr>
      </table>
      <%@ include file="down.jsp"%>             <!-- 在页面中包含down.jsp页 -->
</body>
```

### 引用标签库指令taglib

在JSP文件中，可以通过taglib指令标识声明该页面中所使用的标签库，同时引用标签库，并指定标签的前缀。在页面中，引用标签库后，就可以通过前缀来引用标签库中的标签。taglib指令的语法格式如下：

```
<%@ taglib prefix="tagPrefix" uri="tagURI" %>
```

prefix属性：用于指定标签的前缀。该前缀不能命名为jsp、jspx、java、javax、sun、servlet和sunw。 
 uri属性：用于指定标签库文件的存放位置。 
 例如，引用JSTL中的核心标签库，可以使用下面的代码。

```
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
```

##嵌入JAVA 代码

### 代码片段

代码片段就是在JSP页面中嵌入的Java代码，也有称为脚本段或脚本代码的。代码片段将在页面请求的处理期间被执行，脚本代码可以通过JSP内置对象在页面输出内容、访问session会话、编写流程控制语句等。其语法格式如下： 
 <% 编写Java代码 %> 
 Java代码片段被包含在“<%”和“%>”标记之间。可以编写单行或多行的Java代码，语句以“；”结尾，其编写格式与Java类代码格式相同。 
 例如：

```
<%
      Date now = new Date();
      String dateStr;
      dateStr = String.format("%tY年%tm月%td日", now, now, now);
%>
```

上述代码在代码片段中创建Date对象，并生成格式化的日期字符串。

### JSP 表达式

JSP表达式可以直接把Java的表达式结果输出到JSP页面中。表达式的最终运算结果将被转换为字符串类型，因为在网页中显示的文字都是字符串。JSP表达式的语法格式如下。 
 语法：

```
<%= 表达式 %>
```

表达式可以是任何Java语言的完整表达式。 
 例如，如果要在Jsp页面中将圆周率显示，可以在Jsp页面中添加如下代码。

```
圆周率是：<%=Math.PI %>
```

## 巧用注释

### HTML 注释

HTML语言的注释不会被显示在网页中，但是在浏览器中选择查看网页源代码时，还是能够看到注释信息的。 
 语法：

```
<!-- 注释文本 -->
例如：
<!-- 显示数据报表的表格 -->
<table>
      ……
</table>
```

上述代码为HTML的一个表格添加了注释信息，其它程序开发人员可以直接从注释中了解表格的用途，无需重新分析代码。在浏览器中查看网页代码时，上述代码将完整的被显示，包括注释信息

### JSP

程序注释通常用于帮助程序开发人员理解代码的用途，使用HTML注释可以为页面代码添加说明性的注释，但是在浏览器中查看网页源代码时将暴露这些注释信息，如果使用JSP注释就不用担心这种情况，因为JSP注释是被服务器编译执行的，不会发送到客户端，也就是在客户端通过查看源代码也不会被看到。  
 语法：

```
<%-- 注释文本 --%>
```

例如：

```
<%-- 显示用户信息开始 --%>
用户名：绿草<br>
部&nbsp;&nbsp;门：Java Web部门 <br>
权&nbsp;&nbsp;限：系统管理员
<%-- 显示用户信息结束 --%>
```

上述代码的注释信息不会被发送到客户端，那么在浏览器查看网页源码时也就看不到注释内容。

\###代码注释
 JSP页面支持嵌入的Java代码，这写Java代码的语法和注释方法都和Java类的代码格式相同，所以在JSP页面中嵌入的Java代码可以使用Java的代码注释格式。 
 例如：

```
<%//单行注释%>
<%/*
      多行注释
      */
%>
<%/**JavaDoc注释，用于成员注释*/%>
```

### 动态注释

由于HTML注释对JSP嵌入的代码不起作用，我们可以利用它们的组合构成动态的HTML注释文本。 
 例如：

```
<!-- <%=new Date()%> -->
```

上述代码将当前日期和时间作为HTML注释的注释文本。

## JSP 动作标识

### 包含外部文件的动作标识

在JSP2.0规范中提供了20个标准的使用XML语法写成的动作标签，这些标签可用来实现特殊的功能，例如转发用户请求、操作JavaBean、包含其他文件等。 
 动作标签是在请求处理阶段按照在页面中出现的顺序被执行的，JSP动作标签的优先级低于指令标签，在JSP页面被执行时首先进入翻译阶段，程序会先查找页面中的指令标签，将它们转换成Servlet，从而设置了整个的JSP页面。 
 动作标识遵循XML语法，使用开始标签和结束标签。其通用的语法格式如下：

```
<标签名 属性1="值1" 属性2="值2"…/>
```

或者

```
<标签名 属性1="值1" 属性2="值2"…>
      标签内容
</标签名>
```

你问我答——什么是XML语法 
 XML是目前流行的一种技术，适用于不同应用程序间的数据交流，而且这种交流不以预先定义的一组数据结构为前提，可扩展性强。XML文档中的元素是以树形分层结构排列的，一个元素可以嵌套在另一个元素中，称为文档元素或根元素，类似于HTML页中的元素，其他所有元素都嵌套在根元素中。  
 动作标签可以将另外一个文件的内容包含到当前JSP页面中。被包含的文件内容可以是静态文本，也可以是动态代码。其语法格式如下。 
 语法：

```
<jsp:include page="url" flush="false|true" />
```

或者：

```
<jsp:include page="url" flush="false|true" >
      子标签
</jsp:include>
```

page：该属性用于指定被包含文件的相对路径。例如“validate.jsp”是将当前JSP文件相同文件夹中的validate.文件包含到当前JSP页面中。 
 flush：这是可选参数，它用于设置是否刷新缓冲区。默认值为false，如果设置为true，在当前页面输出使用了缓冲区的情况下，先刷新缓冲区，然后再执行包含工作。 
 例如：

```
<jsp:include page="validate.jsp"/>
```

上述代码将validate.jsp文件内容包含到当前页面中。 
 注意： 
 被包含的JSP页面中不要使用和标签，它们是HTML语言的结构标签，被包含进其它JSP页面会破坏页面格式。另外一点要注意源文件和被包含文件中的变量和方法的名称不要冲突，因为它们最终生成一个文件，重名会导致错误发生。 
 与include指令的区别 
 标签与include指令都拥有包含其它文件内容到当前JSP页面中的能力，但是它们存在一定的区别，具体体现在如下几点： 
 l          相对路径 
 include指令使用file属性指定被包含的文件，该属性值使用文件的相对路径指定被包含文件的位置，而标签以页面的相对路径来指定被包含的资源。 
 l          包含资源 
 include指令包含的资源为静态，例如HTML、TXT等，如果将JSP的动态内容用include指令包含的话，也会被当作静态资源包含到当前页面，被包含资源与当前JSP页面是一个整体，资源的相对路径在JSP页面转换为Servlet时发生。如图1所示。

![include指令工作流程](https://img-blog.csdn.net/20170413092637976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

指令包含JSP动态资源时，资源相对路径的解析在请求处理时发生。当前页面和被包含的资源是两个独立的实体，被包含的页面会对包含它的JSP页面中的请求对象进行处理，被包含的页面会对浏览器对当前JSP页面的请求进行处理，然后将处理结果作为当前JSP页面的包含内容，与当前页面内容一起发送到客户端。如图2所示。

![JSP:include标签工作流程](https://img-blog.csdn.net/20170413092707602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 请求转发的动作标签

是请求转发标签，该标签可以将当前页面的请求转发给其它Web资源，例如另一个JSP页面、HTML页面、Servlet等。而当前页面可以不对请求进行处理，或者做些验证性的和其它工作。其工作原理如图1所示。

![转发请求的工作原理](https://img-blog.csdn.net/20170413092902462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

例如在index.jsp页面中实现将要实现将地址转发至addUser.jsp中，可以在index.jsp页面中添加以下代码。

```
<jsp:forward page="addUser.jsp"/>
```

### 设置参数的自动作标识动作标识

该标签可以作为其它标签的子标签，可以为其它标签传递参数。其语法格式如下。 
 语法：

```
<jsp:param name="paramName" value="paramValue" />
```

name属性：用于指定参数名称。 
 value属性：用于设置对应的参数值。 
 例如：

```
<jsp:forward page="addUser.jsp">
      <jsp:param name="userName" value="mingri"/>
</jsp:forward>
```

上述代码在转发请求到adduser.jsp页面的同时，传递了参数userName，其参数值为“mingri”。

## JSP 内置对象

JSP中采用Java语言作为脚本编程语言，这样一来使系统具有了强大的对象处理能力，并且可以动态创建Web页面内容。但Java语法在使用一个对象之前都要先将这个对象进行实例化，比较繁琐。为了简化开发JSP提供了一些内置对象，这也是JSP语法结构中的独特语句变量，又被称为JSP预定义变量。它们都是由系统容器实现和管理的，在JSP页面中不需要定义，可以直接使用。  
 在JSP中一共预先定义了9个这样的对象，分别为request、response、session、application、out、pageContext、config、page、exception。

### 页面请求对象request

#### 获取请求参数

我们知道request对象用于处理HTTP请求中的各项参数。在这些参数中，最常用的就是获取访问请求参数。当我们通过超链接的形式发送请求时，可以为该请求传递参数，这可以通过在超链接的后面加上问号“?”来实现。注意这个问号为英文半角的符号。 
 例如，发送一个请求到delete.jsp页面，并传递一个名称为id的参数，可以通过以下超链接实现。

```
<a href="delete.jsp?id=1">删除</a>
```

注意： 
 在通过问号“?”来指定请求参数时，参数值不需要使用单引号或双引号括起来，包括字符型的参数。 
 技巧： 
 在通过问号“?”来指定请求参数时，可以同时指定多个参数，各参数间使用与符号“&”分隔即可。 
 在delete.jsp页面中，可以通过request对象的getParameter()方法获取传递的参数值，具体代码如下：

```
<%
request.getParameter("id");
%>
```

说明： 
 在使用request的getParameter()方法获取传递的参数值时，如果指定的参数不存在，将返回null，如果指定了参数名，但未指定参数值，将返回空的字符串”“。 
 例001  使用request对象获取请求参数值。 
 首先在Web项目中创建index.jsp页面，在其中加入一个超链接按钮用来请求show.jsp页面，并在请求后增加一个参数id。关键代码如下：

```
<body>
<a href="show.jsp?id=001">点击获取请求参数的值</a>
</body>
```

然后新建show.jsp页面，在其中通过getParameter()方法来获取id参数与name参数的值，并将其输出到页面中。关键代码如下：

```
<body>
id参数的值为：<%=request.getParameter("id") %><br>

name参数的值为：<%=request.getParameter("name") %>
</body>
```

#### 获取表单提交信息

除了获取请求参数中传递的值之外，我们还可以使用request对象获取从表单中提交过来的信息。在一个表单中会有不同的标签元素，对于文本元素、单选按钮、单选下拉列表框都可以使用getParameter()方法来获取其具体的值，但对于复选框以及多选列表框被选定的内容就要使用getParameterValues()方法来获取了，该方法会返回一个字符串数组，通过循环遍历这个数组就可以得到用户选定的所有内容。

例001  获取form表单提交的信息 
 创建index.jsp页面文件，在该页面中创建一个form表单，在表单中分别加入文本框、下拉列表框、单选按钮和复选框。关键代码如下：

```
<form action="show.jsp" method="post">
      <ul style="list-style: none; line-height: 30px">
            <li>输入用户姓名：<input type="text" name="name" /><br /></li>
            <li>选择性别：
                  <input name="sex" type="radio" value="男" />男
                  <input name="sex" type="radio" value="女" />女
            </li>
            <li>
                  选择密码提示问题：
                  <select name="question">
                        <option value="母亲生日">母亲生日</option>
                        <option value="宠物名称">宠物名称</option>
                        <option value="电脑配置">电脑配置</option>
                  </select>
            </li>
            <li>请输入问题答案：<input type="text" name="key" /></li>
            <li>
                  请选择个人爱好：
                  <div style="width: 400px">
                        <input name="like" type="checkbox" value="唱歌跳舞" />唱歌跳舞
                        <input name="like" type="checkbox" value="上网冲浪" />上网冲浪
                        <input name="like" type="checkbox" value="户外登山" />户外登山<br />
                        <input name="like" type="checkbox" value="体育运动" />体育运动
                        <input name="like" type="checkbox" value="读书看报" />读书看报
                        <input name="like" type="checkbox" value="欣赏电影" />欣赏电影
                  </div>
            </li>
            <li><input type="submit" value="提交" /></li>
      </ul>
</form>
```

接下来编写show.jsp页面文件，该页面是用来处理请求的，在其中分别使用getParameter()方法与getParameterValues()方法将用户提交的表单信息显示在页面中。关键代码如下：

```
<ul style="list-style:none; line-height:30px">
      <li>输入用户姓名：<%=new String(request.getParameter("name").getBytes("ISO8859_1"),"GBK") %></li>
      <li>选择性别：<%=new String(request.getParameter("sex").getBytes("ISO8859_1"),"GBK") %></li>
<li>选择密码提示问题：<%=new String(request.getParameter("question").getBytes("ISO8859_1"),"GBK") %>
</li>
<li>请输入问题答案：<%=new String(request.getParameter("key").getBytes("ISO8859_1"),"GBK") %></li>
<li>
            请选择个人爱好：
      <% 
            String[] like =request.getParameterValues("like");
            for(int i =0;i<like.length;i++){
      %>
      <%= new String(like[i].getBytes("ISO8859_1"),"GBK")+"&nbsp;&nbsp;" %>
      <% }
      %>
      </li>
</ul>
```

#### 解决中文乱码问题

在通过request对象获取请求参数时，如果遇到参数值为中文的情况，如果不进行处理，获取到的参数值将是乱码。在JSP中，解决获取到的请求参数中文乱码，可以分为以下两种情况。 
 1．获取访问请求参数时乱码 
 当访问请参数为中文时，通过request对象获取到的中文参数值为乱码，这是因为该请求参数采用的是ISO-8859-1编码，不支持中文。所以，只有将获取到的数据通过String的构造方法使用GB18030或GBK编码重新构造一个String对象，也可以正确的显示出中文。  
 例如，在获取包括中文信息的参数user时，可以使用下面的代码：

```
String user = new String(request.getParameter("user").getBytes("iso-8859-1"),"GB18030");
12
```

2．获取表单提交的信息乱码 
 当获取表单提交的信息时，通过request对象获取到的中文参数值为乱码。这可以通过在page指令的下方加上调用request对象的setCharacterEncoding()方法将编码设置为GB18030或是GBK解决。例如，在获取包括中文信息的用户名文本框（name属性为username）的值时，可以在获取全部表单信息前，加上下面的代码：

```
<%
      request.setCharacterEncoding("GB18030");
%>
```

这样，再通过下面的代码获取表单的值时，就不会产生中文乱码了。

```
String user = request.getParameter("username");
```

注意： 
 调用request对象的setCharacterEncoding()方法的语句，一定要在页面中没有调用任何request对象的方法时，否则，该语句将不起作用。 
 说明： 
 在解决获取表单提交信息中文乱码时，也可以通过重新构造一个String对象实现，但是这两种方法不能同时使用。



通过request对象可以获取到客户端的相关信息。例如，HTTP报头信息、客户信息提交方式，客户端主机IP地址，端口号等。在客户端获取用户请求相关信息的request对象的方法如表1所示。

![JSP request获取客户端信息](https://img-blog.csdn.net/20170413094648039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

本实例通过上面介绍的方法演示如何使用request对象获取请求客户端信息。关键代码如下：

```
    <ul style="line-height:24px">
      <li>客户使用的协议：<%=request.getProtocol() %>
      <li>客户端发送请求的方法：<%=request.getMethod() %>
      <li>客户端请求路径：<%=request.getContextPath() %>
      <li>客户机IP地址：<%=request.getRemoteAddr() %>
      <li>客户机名称：<%=request.getRemoteHost() %>
      <li>客户机请求端口号：<%=request.getRemotePort() %>
      <li>接收客户信息的页面：<%=request.getServletPath() %>
      <li>获取报头中User-Agent值：<%=request.getHeader("user-agent") %>
      <li>获取报头中accept值：<%=request.getHeader("accept") %>
      <li>获取报头中Host值：<%=request.getHeader("host") %>
      <li>获取报头中accept-encoding值：<%=request.getHeader("accept-encoding") %>
      <li>获取URI：<%= request.getRequestURI() %>
      <li>获取URL：<%=request.getRequestURL() %>
</ul>
```

#### 在作用域中管理属性

通过使用setAttribute()方法可以在request对象的属性列表中添加一个属性，然后在request对象的作用域范围内通过使用getAttribute()方法将其属性取出；此外，还可使用removeAttribute()方法将一个属性删除掉。 
 例001  管理request对象属性。 
 本实例首先将date属性加入到request属性列表中，然后输出这个属性的值；接下来使用removeAttribute()方法将date属性删除，最后再次输出date属性。关键代码如下：

```
<% 
      request.setAttribute("date",new Date());                                          //添加一个属性
%>
<ul style="line-height: 24px;">
      <li>获取date属性：<%=request.getAttribute("date") %></li>
      <%request.removeAttribute("date"); %>                                    <!-- 将属性删除 -->
      <li>删除后再获取date属性：<%=request.getAttribute("date") %></li>
</ul>
```

注意： 
 request对象的作用域为一次请求，超出作用域后属性列表中的属性即会失效。 
 程序运行结果如图1所示，第一次正确输出了date的值；在将date属性删除以后，再次输出时date的值为null。

#### 获取cookie

cookie的中文意思是小甜饼，然而在互联网上的意思与这就完全不同了。它和食品完全没有关系。在互联网中，cookie是小段的文本信息，在网络服务器上生成，并发送给浏览器的。通过使用cookie可以标识用户身份，记录用户名和密码，跟踪重复用户等。浏览器将cookie以key/value的形式保存到客户机的某个指定目录中。  
 通过cookie的getCookies()方法即可获取到所有cookie对象的集合；通过cookie对象的getName()方法可以获取到指定名称的cookie；通过getValue()方法即可获取到cookie对象的值。另外将一个cookie对象发送到客户端使用了response对象的addCookie()方法。  
 例001  应用Cookie跟踪用户 
 首先创建index.jsp页面文件，在其中创建form表单，用于让用户输入信息；并且从request对象中获取cookie，判断是否含有此服务器发送过的cookie。如果没有，则说明该用户第一次访问本站；如果有，则直接将值读取出来，并赋给对应的表单。关键代码如下：

```
<% 
    request.setCharacterEncoding("GBK");  //设置请求编码
     request.setCharacterEncoding("GBK");              //设置相应编码
      String welcome = "第一次访问";
      String[] info = new String[]{"","",""};           //定义字符串数组
      Cookie[] cook = request.getCookies();      //获取所有的cookie对象
      if(cook!=null){
            for(int i=0;i<cook.length;i++){              //循环遍历cookie对象
                  if(cook[i].getName().equals("mrsoft")){                              //获取指定名称的cookie对象      
                String  cvalue = URLDecoder.decode(cook[i].getValue());      //将cookie对象进行编码转换，避免乱码问题
                  info =cvalue.split("#");      //将获取的cookie值进行拆分
                        welcome = "，欢迎回来！";     
                  }
            }
      }
%>
<%
      String name =info[0];                          //页面显示内容
%>
<%=name + welcome %>
<form action="show.jsp" method="post">
      <ul style="line-height: 23">
            <li>姓&nbsp;&nbsp;&nbsp;&nbsp;名：<input name="name" type="text" value="<%=info[0]%>">      
            <li>出生日期：<input name="birthday" type="text" value="<%=info[1] %>">
            <li>邮箱地址：<input name="mail" type="text" value="<%=info[2] %>">
            <li><input type="submit" value="提交">
      </ul>
</form>
```

（2）接下来创建show.jsp页面文件，在该页面中通过request对象将用户输入的表单信息提取出来；创建一个cookie对象，并通过response对象的addCookie()方法将其发送到客户端。关键代码如下：

```
<%       
    request.setCharacterEncoding("GBK");                //设置请求编码，避免乱码
    response.setCharacterEncoding("GBK");                     //设置相应编码，避免乱码  
      String name = request.getParameter("name");              //获取请求页面参数     
      String birthday = request.getParameter("birthday");
      String mail = request.getParameter("mail");                 
      Cookie myCook = new Cookie("mrsoft", URLEncoder.encode(name+ "#" + birthday
                  + "#" + mail)) ;                                          //创建cookie对象
      myCook.setMaxAge(60 * 60 * 24 * 365);                       //设置cookie有效期
      response.addCookie(myCook);                               //将cookie发送至客户端
%>
表单提交成功
<ul style="line-height: 24px">
      <li>
            姓名：<%=name%>                                          <%--在页面中显示内容 --%>
      <li>
            出生日期：<%=birthday%>
      <li>
            电子邮箱：<%=mail%>
      <li>
       <a href="index.jsp">返回</a>
</ul>
```

#### 显示国际化信息

浏览器可以通过accept-language的HTTP报头向Web服务器指明它所使用的本地语言。request对象中的getLocale()和getLocales()方法允许JSP开发人员获取这一信息，获取的信息属于java.util.Local类型。java.util.Local类型的对象封装了一个国家和一种国家所使用的语言。使用这一信息，JSP开发者就可以使用语言所特有的信息作出响应。使用这个报头的代码如下：

```
<%
java.util.Locale locale=request.getLocale();
String str="";
if(locale.equals(java.util.Locale.US)){
      str="Hello, welcome to access our company's web!";
}
if(locale.equals(java.util.Locale.CHINA)){
      str="您好，欢迎访问我们公司网站！";
}
%>
<%=str %>
```

上面的代码，如果所在区域为中国，将显示“您好，欢迎访问我们公司网站！”，而所在区域为英国，则显示“Hello, welcome to access our company’s web!”。

### 客户端响应对象response

#### 重定向网页

使用response对象提供的sendRedirect()方法可以将网页重定向到另一个页面。重定向操作支持将地址重定向到不同的主机上，这一点与转发不同。在客户端浏览器上将会得到跳转的地址，并重新发送请求链接。用户可以从浏览器的地址栏中看到跳转后的地址。进行重定向操作后，request中的属性全部失效，并且开始一个新的request对象。  
 sendRedirect()方法的语法格式为：response.sendRedirect(String path); 
 path：用于指定目标路径，可以是相对路径，也可以是不同主机的其他URL地址。 
 例如，使用该方法重定向到百度网.

```
response.sendRedirect("www.baidu.com");1
```

注意： 
 在JSP页面中使用该方法的时候前面不要有HTML代码，并且在重定向操作之后紧跟一个return，因为重定向之后下面的代码已经没有意义了，并且还可能产生错误。

#### 设置输出缓冲

通常情况下，服务器要输出到客户端的内容不会直接写到客户端，而是先写到一个输出缓冲区，当满足以下三种情况之一，就会把缓冲区的内容写到客户端。 
 l         JSP页面的输出信息已经全部写入到了缓冲区。 
 l         缓冲区已满。 
 l         在JSP页面中，调用了response对象的flushbuffer()方法或out对象的flush()方法。 
 你问我答——什么是缓冲区 
 在计算机术语中，缓冲区被定义为暂时放置输入或输出资料的内存。实际上，缓冲区也可以这样理解：在一个粮库中，由于装缷车队的速度要快于传送带的传输速度，为了不造成装缷车队的浪费，粮库设计了一个站台，装缷车队可以先将运送的粮食缷到这个平台上，然后让传送机慢慢传送。粮库的这个站台就起到了缓冲的作用。  
 response对象提供了如表1所示的对缓冲区进行配置的方法。

![对缓冲区进行配置的方法](https://img-blog.csdn.net/20170413095344956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

例如，设置缓冲区的大小为32KB，可以使用以下代码。

```
response.setBufferSize(32);
```

说明： 
 如果将缓冲区的大小设置为0KB，则表示不缓冲。

#### 处理HTTP 文件头

通过response对象可以设置HTTP响应报头，其中，最常用的是设置响应的内容类型、禁用缓存、设置页面自动刷新和定时跳转网页。下面分别进行介绍。 
 1．设置响应的内容类型 
 通过response对象的setContentType()方法可以设置响应的内容类型，默认情况下，采用的内容类型是text/html。通过指定响应的内容类型可以让网页内容以不同的格式输出到浏览器中。 
 setContentType()方法的语法格式如下：response.setContentType(String type); 
 说明： 
 type：用于指定响应的内容类型，可选值为text/html、text/plain、application/x_msexcel和application/msword等。 
 setHeader()方法通过两个参数——头名称与参数值的方式来设置HTTP文件头。 
 2．禁用缓存 
 在默认的情况下，浏览器将会对显示的网页内容进行缓存。这样，当用户再次访问相的的网页时，浏览器会判断网页是否有变化，如果没有变化则直接显示缓存中的内容，这样可以提高网页的显示速度。对于一些安全性要求较高的网站，通常需要禁用缓存。通过设置HTTP头的方法实现禁用缓存，可以通过以下代码实现。

```
<%
response.setHeader("Cache-Control","no-store");
response.setDateHeader("Expires",0);
%>
```

3．设置页面自动刷新 
 通过设置HTTP头还可以实现页面的自动刷新。例如，让网页每隔10秒自动刷新一次，可以使用下面的代码。

```
<%
response.setHeader("refresh","10");
%>
```

4．定时跳转网页 
 通过设置HTTP头还可以实现定时跳转网页的功能。例如，让网页5秒钟后自动跳转到指定的页面，可以使用下面的代码。

```
<%
response.setHeader("refresh","5;URL=login.jsp");
%>
```

### session会话对象

#### 创建及获取session

session对象是由服务器自动创建的与用户请求相关的对象。服务器为每个用户都生成一个session对象，用于保存该用户的信息，跟踪用户的操作状态。session对象内部使用Map类来保存数据，因此保存数据的格式为“key/value”。session对象的value可以是复杂的对象类型，而不仅仅局限于字符串类型。session中的常用方法如表1所示。

![session](https://img-blog.csdn.net/20170413151807607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过session对象可以存储或读取客户相关的信息。例如，用户名或购物信息等。这可以通过session对象的setAttribute()方法和getAttribute()方法实现。

setAttribute()方法 
 用于将信息保存在session范围内，其语法格式为：session.setAttribute(String name,Object obj) 
 name：用于指定作用域在session范围内的变量名；obj：保存在session范围内的对象。 
 例如，将用户名“绿草”保存到session范围内的username变量中，可以使用下面的代码：

```
session.setAttribute("username","绿草");
```

l         getAttribute()方法 
 用于获取保存在session范围内的信息，其语法格式为：getAtttibute(String name) 
 name：指定保存在session范围内的关键字。 
 例如，读取保存到session范围内的username变量的值，可以使用下面的代码：

```
session.getAttribute("username");
```

getAttribute()方法的返回值是Object类型，如果需要将获取到的信息赋值给String类型的变量，需要进行强制类型转或是调用其toString()方法，例如，下面的两行代码都是正确的。

```
String user=(String)session.getAttribute("username");        //强制类型转
String user1=session.getAttribute("username").toString();          //调用toString()方法
```

#### 从会话中移除指定的对象

对于存储在session会话中的对象，如果想将其从session会话中移除，可以使用session对象的removeAttribute()方法。 
 语法格式为：removeAttribute(String key) 
 说明：key：保存在session范围内的关键字。 
 例如，将保存在session会话中的对象移除。

```
session.removeAttribute("message");
```

#### 设置会话的有效时间

在应用session对象时应该注意session的生命周期。一般来说，session的生命周期在20～30分钟之间。当用户首次访问时将产生一个新的会话，以后服务器就可以记住这个会话状态，当会话生命周期超时时，或者服务器端强制使会话失效时，这个session就不能使用了。在开发程序时应该考虑到用户访问网站时可能发生的各种情况，比如用户登录网站后在session的有效期外进行相应操作，用户会看到一张错误页面。这样的现象是不允许发生的。为了避免这种情况的发生，在开发系统时应该对session的有效性进行判断。  
 在session对象中提供了设置会话生命周期的方法，分别介绍如下。 
 l       getLastAccessedTime()：返回客户端最后一次与会话相关联的请求时间。 
 l       getMaxInactiveInterval()：以秒为单位返回一个会话内两个请求最大时间间隔。 
 l       setMaxInactiveInterval()：以秒为单位设置session的有效时间。 
 例如，通过setMaxInactiveInterval()方法设置session的有效期为10000秒，超出这个范围session将失效。

```
session.setMaxInactiveInterval(10000);
```

#### 销毁session

当调用session对象的invalidate()方法后，表示session对象被删除，即不可以再使用session对象。 
 语法格式如下：session.invalidate(); 
 如果调用了session对象的invalidate()方法，之后在调用session对象的任何其他方法时，都将报出Session already invalidated异常。

#### session 对象的应用

session是比较常用的内置对象之一，与request对象相比session对象其作用范围更大。下面通过实例为大家介绍session对象的应用。 
 例001   在index.jsp页面中，提供用户输入用户名文本框；在session.jsp页面中，将用户输入的用户名保存在session对象中，用户在该页面中可以添加最喜欢去的地方；在result.jsp页面中，将用户输入的用户名与最想去的地方在页面中显示。  
 （1）index.jsp页面的关键代码如下：

```
<form id="form1" name="form1" method="post" action="session.jsp">
    <div align="center">
  <table width="23%" border="0">
    <tr>
      <td width="36%"><div align="center">您的名字是：</div></td>
      <td width="64%">
        <label>
        <div align="center">
          <input type="text" name="name" />
        </div>
        </label>
        </td>
    </tr>
   …//省略了在页面中添加其他表单元素的代码
  </table>
</div>
</form>
```

（2）在session.jsp页面中，将用户在index.jsp页面中输入的用户名保存在session对象中，并为用户提供用于添加最想去的地址的文本框。代码如下：  

```
     <%
     String name = request.getParameter("name");                     //获取用户填写的用户名
     session.setAttribute("name",name);                             //将用户名保存在session对象中
   %>
    <div align="center">
  <form id="form1" name="form1" method="post" action="result.jsp">
    <table width="28%" border="0">
      <tr>
        <td>您的名字是：</td>
        <td><%=name%></td>                             
      </tr>
      <tr>
        <td>您最喜欢去的地方是：</td>
        <td><label>
          <input type="text" name="address" />
        </label></td>
      </tr>
      <tr>
        <td colspan="2"><label>
          <div align="center">
            <input type="submit" name="Submit" value="提交" />
            </div>
        </label></td>
      </tr>
    </table>
  </form>
```

（3）在result.jsp页面中，实现将用户输入的用户名、最喜欢去的地方在页面中显示。代码如下：

```
<%
     String name = (String)session.getAttribute("name");            //获取保存在session范围内的对象
     String solution = request.getParameter("address");              //获取用户输入的最喜欢去的地方
   %>
<form id="form1" name="form1" method="post" action="">
  <table width="28%" border="0">
    <tr>
      <td colspan="2"><div align="center"><strong>显示答案</strong></div></td>
    </tr>
    <tr>
      <td width="49%"><div align="left">您的名字是：</div></td>
      <td width="51%"><label>
        <div align="left"><%=name%></div>                        <!-- 将用户输入的用户名在页面中显示 -->
      </label></td>
    </tr>
    <tr>
      <td><label>
        <div align="left">您最喜欢去的地方是：</div>
      </label></td>
      <td><div align="left"><%=solution%></div></td>  <!-- 将用户输入的最喜欢去的地方在页面中显示 -->
    </tr>
  </table>
</form>
```

### application 对象

#### 应用程序初始化参数

application对象用于保存所有应用程序中的公有数据。它在服务器启动时自动创建，在服务器停止时销毁。当application对象没有被销毁时，所有用户都可以共享该application对象。与session对象相比，application对象的生命周期更长，类似于系统的“全局变量”。  
 application对象提供了对应用程序初始化参数进行访问的方法。应用程序初始化参数在web.xml文件中进行设置，web.xml文件位于Web应用所在目录下的WEB-INF子目录中。在web.xml文件中通过标记配置应用程序初始化参数。例如，在web.xml文件中配置连接MySQL数据库所需的url参数，可以使用下面的代码：

```
   <context-param>
 <param-name>url</param-name>
  <param-value>jdbc:mysql://127.0.0.1:3306/db_database</param-value>
 </context-param>
 </web-app>
```

application对象提供了两种访问应用程序初始化参数的方法，下面分别进行介绍。

```
      getInitParameter()方法
```

该方法用于返回一下已命名的参数值，其语法格式如下：

```
  application.getInitParameter(String name);
```

name：用于指定参数名。 
 例如，获取上面web.xml文件中配置的url参数的值，可以使用下面的代码：

```
   application.getInitParameter("url");
```

getAttributeNames()方法 
 该方法用于返回所有已定义的应用程序初始化参数名的枚举。

```
application.getAttributeNames();
```

例如，应用getAttributeNames()方法获取web.xml中定义的全部应用程序初始化参数名，并通过循环输出，可以使用下面的代码：

```
<%@ page import="java.util.*" %>
<%
Enumeration enema=application.getInitParameterNames();  //获取全部初始化参数
while(enema.hasMoreElements()){
      String name=(String)enema.nextElement();                        //获取参数名
      String value=application.getInitParameter(name);         //获取参数值
      out.println(name+"：");                                      //输出参数名
      out.println(value);                                              //输出参数值
}
%>
```

如果在web.xml文件中，只包括一个上面添加的url参数，执行上面的代码将显示以下内容：

url： jdbc:mysql://127.0.0.1:3306/db_database

#### 管理应用程序环境属性

与session对象相同，也可以在application对象中设置属性。与session对象不同的是，session只是在当前客户的会话范围内有效，当超过保存时间，session对象就被收回；而application对象在整个应用区域中都有效。application对象管理应用程序环境属性的方法分别介绍如下。

l         getAttributeNames()：获得所有application对象使用的属性名。

l         getAttribute(String name)：从application对象中获取指定对象名。

l         setAttribute(String key,Object obj)：使用指定名称和指定对象在application对象中进行关联。

l         removeAttribute(String name)：从application对象中去掉指定名称的属性。

### 其它内置对象

#### 获取会话范围的pageContext对象

pageContext对象的作用是取得任何范围的参数，通过它可以获取JSP页面的out、request、response、session、application等对象。pageContext对象的创建和初始化都是由容器来完成的，在JSP页面中可以直接使用pageContext对象。pageContext对象的常用方法如表1所示。

![pageContext对象](https://img-blog.csdn.net/20170414123519097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

pageContext对象在实际JSP开发过程中很少使用，因为request和response等对象均为内置对象都可以直接调用其相关方法实现具体的功能，如果通过pageContext来调用这些对象比较麻烦。

#### 读取web.xml 配置新的config 对象

config对象的主要作用是取得服务器的配置信息。通过pageContext对象的getServletConfig()方法可以获取一个config对象。当一个Servlet初始化时，容器把某些信息通过config对象传递给这个Servlet。开发者可以在web.xml文件中为应用程序环境中的Servlet程序和JSP页面提供初始化参数。config对象的常用方法如表1所示。

![config对象](https://img-blog.csdn.net/20170414123718426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 应答或请求的page 对象

page对象代表JSP本身，只在JSP页面内才是合法的。page对象本质上是包含当前Servlet接口引用的变量，可以看作是this关键字的别名。page对象提供的常用方法如表1所示。

![page对象](https://img-blog.csdn.net/20170414123921130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgzMzQwNDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

创建index.jsp文件，在该文件中，调用page对象的各方法，并显示返回结果。

```
<body>
<%!      Object object;    //声明一个Object型的变量         %>
<ul>
<li>getClass()方法的返回值:<%=page.getClass()%></li>
<li>hashCode()方法的返回值:<%=page.hashCode()%></li>
<li>toString()方法的返回值:<%=page.toString()%></li>
<li>与Object对象比较的返回值:<%=page.equals(object)%></li>
<li>与this对象比较的返回值:<%=page.equals(this)%></li>
</ul>
</body>
```

#### 获取异常信息的exception 对象

exception对象的作用是显示异常信息，只有在包含isErrorPage=”true”的页面中才可以被使用，在一般的JSP页面中使用该对象将无法编译JSP文件。exception对象和Java的所有对象一样，都具有系统的继承结构。exception对象几乎定义了所有异常情况。在Java程序中，可以使用try/catch关键字来处理异常情况；如果在JSP页面中出现没有捕捉到的异常，就会生成exception对象，并把exception对象传送到在page指令中设定的错误页面中，然后在错误页面中处理相应的exception对象。exception对象的常用方法如表1所示。

使用exception对象获取错误信息。 
 （1）新建web项目，在该项目的index.jsp页面中，定义页面指令“errorPage=”error.jsp””，这样如果在index.jsp页面中出现异常，系统会转发至error.jsp页面中，关键代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="gbk" errorPage="error.jsp"%>
  <body>
   <%
      int apple = Integer.parseInt("ad");         //定义表示苹果价格的int对象
      out.println("苹果每斤"+apple+"元");            //页面输出信息
      %>
  </body>
```

（2）在项目中定义error.jsp页，用于接收传递过来的异常信息。关键代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="gbk" isErrorPage="true"%>
<body>
  错误提示为：<%=exception.getMessage() %>
</body>
```

由于将字符串“ad”转换为int型变量会发生异常，因此系统将转发至error.jsp页面，如图2所示。如果将index.jsp页面中的代码int  apple = Integer.parseInt(“ad”);替换为int apple = Integer.  parseInt(“3”)，则不会有异常发生，不会转发至error.jsp页面，