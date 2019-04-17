# JS代码简洁之道 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月26日 10:14:46[boonya](https://me.csdn.net/boonya)阅读数：1504








因为很久没有关注JS代码了，正巧看到表弟写的JS感觉挺好的，所以拿来与大家分享一下（代码量很少而且可读性强）。

### 1.JSP代码



```
<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'index.jsp' starting page</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">

  </head>
   
  <body>
    <script src="${pageContext.request.contextPath}/js/jquery.js"></script>
	<script src="${pageContext.request.contextPath}/js/bootstrap.min.js"></script>
	<script type="text/javascript" src="${pageContext.request.contextPath}/js/testform.js"></script>
	
    用户信息管理 <br>
    
     <form id="testform" method="post"><br/><br/>
     	<input type="hidden" name="method" value="addUser"/>
     	姓名: <input type="text" name="studentName"/><br/><br/>
     	性别: <input type="text" name="studentSex" /><br/><br/>
     	地址: <input type="text" name="studentDizhi"/><br/><br/>
     	<input id="save" type="button" value="保存"/>
     </form>
   

  </body>
</html>
```



### 2.JS代码



```java
$(function() {
	
	function test(){
		var options = {
			url : "/demo/testAction.do",
			type : "post",
			data : $("form#testform").serialize(),
			cache : false,
			async : true,
			success : function(data) {
					alert("操作成功");
					window.location.reload();
			}
		};
		$.ajax(options);
	}	
	
	function init() {
		$("input#save").click(test);
	}
	
	$(init);	
});
```



### 3.代码分析

1.功能代码分离：各方法各司其职，如test()仅仅干一件事，就是保存。

2.事件统一管理及初始化：这样很清晰哪些方法需要哪些操作，如init()，在这里面初始化绑定事件。


3.尽量使用jQuery所提供的功能：比如form表单的序列化传参$("form#testform").serialize()。

注：很多情况下我们并不注重代码的复用，如果稍加改进可以大大减少代码量，这也是平时开发需要注意和改进的地方。




