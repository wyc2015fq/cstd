# include与jsp:include与s:action与s:include与iframe用法汇总 - z69183787的专栏 - CSDN博客
2012年11月14日 09:20:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：742
个人分类：[Jsp-Tag标签																[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)](https://blog.csdn.net/z69183787/article/category/2175879)
1.<%@ include file="../top/top.jsp"%> 
2.<jsp:include page="coupon.jsp"/> 
3.<iframe src="" width="100%" height="200" ></iframe> 
4.<s:include value="/CMS/headers/wrap/header.jsp"/> 
5.<s:action name="cmsPoint" namespace="/include"/> 
jsp开发时，这五种常用的页面引入标签，到底有什么区别，各自适应场合是什么，相信很多人都不陌生，但又感觉莫林两可的，本文将做一个全面的汇总。 
注：本文虽然各方面都涉及到了，但是很多细节还没列出来，后续在开发中还会不断总结不断完善，希望与广大it从业的朋友们多交流，共同进步。 
------------------------------------------------------------------------------ 
首先看include和jsp:include 
<%@ include file=” ”%> 
<jsp:include page=” ” flush=”true”/> 
前者是指令元素、后者是行为元素。具体它们将在何处用？如何用及它们有什么区别？这该是很多人看到它都会想到的问题。下面一起来看看吧。 
通常当应用程序中所有的页面的某些部分（例如标题、页脚和导航栏）都相同的时候，我们 
就可以考虑用include。具体在哪些时候用<%@ include file=” ”%>，哪些时候用 
<jsp:include page=” ” flush=”true”/>.这种形式。首先要明白的是它们之间的区别 
。只有了解了它们用法的不同才理解该在何时去用以及如何选择。 
<%@ include file=” ”%>,jsp的include指令元素读入指定页面的内容。并把这些内容和原来的页面融合到一起。(这个过程是在翻译阶段:也就是jsp被转化成servlet的阶段进行的 
。) 
这里对翻译阶段进行一下说明:我们知道，jsp页面不能原封不动地被传送给浏览器，所有的 
jsp元素都必须首先由服务器进行处理。这是通过将jsp页面转达化成servlet，然后执行这个servlet来完成的。服务器需要一个jsp容器来处理jsp页面。jsp容器通常以servlet的形式来实现，这个servlet经过配置，可以处理对jsp页面的所有请求。 
Jsp容器负责将jsp页面转化成servlet（称为jsp页面实现类?JSP Page implementation 
class）,并编译这个servlet。这两步就构成了翻译阶段. 
由此我们就会知道：jsp页面是把include指令元素所指定的页面的实际内容（也就是代码段 
）加入到引入它的jsp页面中,合成一个文件后被jsp容器将它转化成servlet。可以看到这时 
会产生一个临时class文件和一个java文件。下面举个例子。 
服务器用tomcat，引入页面的jsp文件叫test.jsp。被引入的页面叫date.jsp.这个jsp文件 
里存放的是一个有关时间的jsp代码,当前的上下文根设为test 
//======date.jsp的源文件=====// 
Html代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language=”java” c%>
- <%   
- java.util.Date date=new java.util.Date();   
- String date_cn ="";   
- String dateStr = "";   
- switch(date.getDay())   
- {   
- case 0:date_cn ="日"; break;   
- case 1:date_cn ="一"; break;   
- case 2:date_cn ="二"; break;   
- case 3:date_cn ="三"; break;   
- case 4:date_cn ="四"; break;   
- case 5:date_cn ="五"; break;   
- case 6:date_cn ="六"; break;   
- }   
- dateStr = (1900+date.getYear()) + "年" + (date.getMonth()+1) + "月" +   
- 
- date.getDate() + "日(星期" + date_cn + ")";   
- %>
- document.write("<%=dateStr%>");   
//======以下是test.jsp的源文件=============// 
Html代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language=”java” contentType=”text/html;charset=gb2312”%>
- <html>
- <head>
- <title>include的两种用法</title>
- <jsp:includepage=”date.jsp” flush=”true”/>
- <%--@ include file=”date.jsp” %-->
- //我们在这里用include的两种不同形式来引入date.jsp这个文件.   
- <head>
- <body>
- <table><tr><td>
- 有关jsp中include的两种用法.敬请关注。   
- </td></tr></table>
- </body>
- </html>
在test.jsp 文件中，我们只输出了一行文本“ 有关jsp中include的两种用法.敬请关注。 
”,现在让我们先用<%@ include file=”date.jsp” %>这种形式引入date.jsp这个文件。 
你想会出现什么问题了吗？此时出现了错误提示： 
HTTP Status 500 ? 
org.apache.jasper.JasperException: /date.jsp(0,0) Page directive: can't have 
multiple occurrences of contentType 
以下还有一堆错误，但我们只要看这里就知道问题的所在了。状态码为http 500服务器内部 
错误。再看下面的提示。在date.jsp页面中不能指定多个contentType. 原因就在这里了。 
是因为在翻译阶段,date.jsp文件的代码被原封不动地加入到了test.jsp页面从而合成一个 
文件。合成后的文件中就会相同的： 
<%@ page language=”java” contentType=”text/html;charset=gb2312”%> 
这句代码。解决的办法是把date.jsp文件中的这句删掉。刷新后再请求test.jsp页面 
请求test.jsp在页面显示如下 
2003年12月10日 13:12:40 
这时我们还不能发现什么。还是去查看tomcat下的临时文件吧。到那里去看看date.jsp文件 
的内容是否已被加入到了test.jsp文件中。 
在目录下会看到test_jsp.java和test_jsp.class两个文件. 
这里的java文件就是jsp容器将jsp转化成了servlet而得到的test_jsp.java这个文件。相对 
应的test_jsp.class这 个文件就是编译test_jsp.java这个servlet文件产生的类文件了。 
打开所产生的servlet文件（test_jsp.java）,此时 我们会发现，在test.jsp 文件被转化 
成servlet文件时，在输出的<haed>之间加入了一些不是test.jsp页面里面的代码,新加入的 
内容就是 date.jsp里面的代码: 新加入了哪些内容或是否真的加入了新的内容请自己测试 
去看一下就会一目了然了.在这里不再详述. 
以上就是我们用<%@ include file=”date.jsp”%>这种形式得到的结果. 
下面我们换用<jsp:include page=”dae.jsp” flush=”true”/>也就是将 
<%@ include file=”date.jsp”%>换成<jsp:include page=”dae.jsp” 
flush=”true”/>,然后请求test.jsp. 
2003? ê 12??10?? 13:30:13 
此时会在页面上看见.我们所引入date.jsp输出的日期中中文出现了乱码.什么原因?是因为 
include行为元素是在请求处理阶段执行的(此处要对 请求处理阶段进行说明一下,Jsp容器 
除了上面提到的负责将jsp页面转化成servlet外,还负责调用jsp页面实现类以处理每个请求 
并产生应答.这 个阶段我们就称为请求处理阶段.请求处理阶段仅执行类文件)。 
所以在我们作include行为元素引入页面时,实际只是引用了date.jsp这个文件被转化并被编 
译后产生的servlet类文件.既如此, date.jsp就是作为一个单独的文件在执行后才被 
test.jsp文件运行时调用.由于date.jsp文件中没有指定字符编码.所以出现了乱码.解 决办 
法是在date.jsp文件中重新把刚才去掉的 
<%@ page language=”java” contentType=”text/html;charset=gb2312”%> 
这行语句加入后刷新重新运行.此时页面显示正确,并跟用include指令正常运行时一样.再查 
看tomcat下的临时文件会发现.此时多出了一个 date_jsp.java文件和一个date_jsp.class 
文件.这两个文件得来的方式跟test_jsp.java和 test_jsp.class文件得来的方式一样.再查 
看一下此时test_jsp.java文件的代码会发现.此时只新增加了一句代码: 
JspRuntimeLibrary.include(request, response, "date.jsp", out, true); 
它并没有把date.jsp文件的代码加入到test.jsp.只是在运行时引入了date.jsp页面执行后 
所产生的应答.这意味着我们可以指定任何能 够产生应答的Web资源,(例如一个servlet或一 
个jsp页面),只要这些资源所产生的类型和jsp页面产生的内容类型相同.JSP容器将通过一个 
内部的函数调用来执行指定的资源.因此,这些被引入的资源可以帮助处理原来的请求,所以 
这些资源可以访问请求作用域内的所有对象.以及所有原来的请求参 数. 
由于在主页面被请求时,这些页面还没有被引入到主页面中,所以你可以对page属性使用一个 
请求时属性值,以便根据运行时的情况来决定要引入哪一个页面.还可以添加一些将被引入的 
页面读取的请求参数. 
<jsp:include page=”<%=pageSelectedAtRuntime%>” flush=”true” > 
<jsp:param name=”fitstParamer” value=”firstValue”> 
<jsp:param name=”lastParamer” value=”lastValue”> 
</jsp:include> 
如果修改了被引入的jsp页面,那么可以立刻使用该页面的最新版本,这是因为对待被引入的 
页面的方式与对待由浏览器直接调用的jsp页面的方式完全相同.即容器检测页面的变化,并 
自动进入翻译阶段,以得到页面的最新版本. 
(注意,include行为元素同jsp其它元素一样,没有行为体时要以”/”结束.就像下面这样. 
<jsp:include page=”<%=pageSelectedAtRuntime%>” flush=”true” />) 
以下是对include 两种用法的区别 
主要有两个方面的不同; 
一:执行时间上: 
<%@ include file=”relativeURI”%> 是在翻译阶段执行 
<jsp:include page=”relativeURI” flush=”true” /> 在请求处理阶段执行. 
二:引入内容的不同: 
<%@ include file=”relativeURI”%> 
引入静态文本(html,jsp),在JSP页面被转化成servlet之前和它融和到一起. 
<jsp:include page=”relativeURI” flush=”true” />引入执行页面或servlet所生成的 
应答文本. 
另外在两种用法中file和page属性都被解释为一个相对的URI.如果它以斜杠开头,那么它就 
是一个环境相关的路径.将根据赋给应用程序的URI的前缀进行解释,如果它不是以斜杠开头, 
那么就是页面相关的路径,就根据引入这个文件的页面所在的路径进行解释. 
------------------------------------------------------------------------- 
如果用了struts2框架，则还有以下的用法。 
<s:include value="/header.jsp"> 
<s:param name="mKeywords"> 
<s:property value="couponDetailVO.storeName"/>团购</s:param> 
</s:include> 
<s:action name="cmsPoint" namespace="/include" executeResult="true" 
ignoreContextParams="true"> 
<s:param name="type" value="'headers/new/include'"></s:param> 
<s:param name="match" value="'footer_950'"></s:param> 
<s:param name="query" value="'city='+oldCity"></s:param> 
</s:action> 
使用action标签,可以允许在jsp页面中直接调用Action,在调用Action时候,可以指定需要被 
调用的Action的name和namespace.如果指定了executeResult参数的属性值为true,该标签会 
把Action的处理结果(视图资源)包含到本页面中. 使用action标签指定属性有: 
id: 可选属性,作为该Action的引用ID    
name:必选属性,指定调用Action    
namespace:可选属性,指定该标签调用Action所属namespace    
executeResult:必选属性,指定是否将Action的处理结果包含到本页面中.默认值为false,不 
包含.    
ignoreContextParam:可选参数,指定该页面的请求参数是否需要传入调用的Action中,默认 
值是false,即传入参数.  
下面看一个完整的调用示例： 
Html代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language="java"contentType="text/html; charset=utf-8"
- pageEncoding="utf-8"%>
- <%@taglib prefix="s"uri="/struts-tags" %>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"   
- 
- "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=utf-8">
- <title>Insert title here</title>
- </head>
- <body>
- <s:actionname="head!toHead"executeResult="true">
- <s:paramname="headName">s.action测试</s:param>
- </s:action>
- </body>
- </html>
struts.xml 
Xml代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <!--测试一些页面引入标签 -->
- <actionname="head"class="com.SactionIncludeTest">
- <resultname="head">/head.jsp</result>
- </action>
- </package>
Java代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- package com;  
- 
- import javax.servlet.ServletContext;  
- import javax.servlet.http.HttpServletRequest;  
- 
- import org.apache.struts2.ServletActionContext;  
- 
- import com.opensymphony.xwork2.ActionSupport;  
- 
- publicclass SactionIncludeTest extends ActionSupport  {  
- private String headName;  
- 
- 
- public String getHeadName() {  
- return headName;  
-     }  
- 
- 
- publicvoid setHeadName(String headName) {  
- this.headName = headName;  
-     }  
- 
- 
- public String toHead(){  
-         HttpServletRequest request=ServletActionContext.getRequest();  
-         String attrHeadName=(String)request.getAttribute("headName");  
-         System.out.println("attrHeadName:"+attrHeadName);  
-         System.out.println("headName:"+this.getHeadName());  
- //可以通过以上两种方式获取到s.param的传值，而下面的方法是接收不
- 
- 到值的。  
- 
-         String paramHeadName=request.getParameter("headName");  
-         System.out.println("paramHeadName:"+paramHeadName);  
- return"head";  
- 
-     }  
- }  
head.jsp 
Html代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language="java"contentType="text/html; charset=utf-8"
- pageEncoding="utf-8"%>
- <%@ taglib prefix="s"uri="/struts-tags" %>
- this is head content!  
- this is head content!  
- this is head content!  
- this is head content!  
- this is head content!  
- this is head content!<br/>
- <s:propertyvalue="#request.headName"/>
----------------------------------- 
s:include的用法 
s:include的效果与jsp:include效果是一样的，一般我习惯用jsp:include 
下面给一个完整示例： 
s-include.jsp 
Html代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language="java"contentType="text/html; charset=utf-8"
- pageEncoding="utf-8"%>
- <%@ taglib prefix="s"uri="/struts-tags" %>
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=utf-8">
- <title>Insert title here</title>
- </head>
- <body>
- <s:includevalue="/foot.jsp">
- <s:paramname="footName">test footName!</s:param>
- </s:include>
- </body>
- </html>
foot.jsp 
Jsp代码  ![收藏代码](http://bingyingao.iteye.com/images/icon_star.png)
- <%@ page language="java" contentType="text/html; charset=utf-8"
-     pageEncoding="utf-8"%>  
- <%@ taglib prefix="s" uri="/struts-tags" %>  
- <html>  
- <head>  
- <meta http-equiv="Content-Type" content="text/html; charset=utf-8">  
- <title>Insert title here</title>  
- </head>  
- <body>  
- This is foot!<br/>  
- 可行的:<%=request.getParameter("footName") %>  
- 可行的：${param.footName }<br/>  
- //写面几种方式都是错误的<br/>  
- requset.footName:<s:property value="#requset.footName" /><br/>  
- s-param.type:<s:property value="param.footName"/> <br/>  
- parameters.type:<s:property value="#parameters.footName"/><br/>  
- </body>  
- </html>  
------------------------------------------------------------------- 
iframe用法 
iframe的src可以指向任何地址，它加载的是一个完整的dom模型。 
include与jsp:include的归纳参考了一个兄弟的博客，他总结的非常清晰： 
http://www.cnblogs.com/Ghost-Draw-Sign/articles/1835974.html 
总的来说： 
include与jsp:include用的比较多，基本可以满足要求了. 
s:action在与s:include用的相对少一点。 
iframe也很常用，不过据说它不利于搜索引擎。 
