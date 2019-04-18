# jsonp调用及后台java返回 - z69183787的专栏 - CSDN博客
2013年11月13日 09:41:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：31609
```java
<script>
				  $.ajax({
				    type: 'GET',
					url: '<%=urls%>',
					dataType:'jsonp',
					data:{
							"token" : '<%=token%>',	
							"method" : '<%=method%>',				
							"appName" : '<%=appName%>',	
							"secret" : '<%=secret%>',	
							"dataType" : 'json',	
							"dataParams" : '<%=dataParams%>',	
							"sign" : '<%=sign%>'
						},
						jsonp:'jsonpcallback',
					error: function(XmlHttpRequest,textStatus,errorThrown){
							alert("部门切换失败，请联系管理员!");
							top.location.href = '/portal/mainFrame/frame.html';
						},
					success: function(msg){		
							if(msg.code=="100"){
								top.location.href = "clearToken.jsp?appName=<%=caAppName%>";
							}else{
								alert("部门切换失败，请联系管理员!\n错误代码："+msg.code);
								top.location.href = '/portal/mainFrame/frame.html';
							}
									
						}		
				  });
			</script>
```
java：
```java
String jsonp=request.getParameter("jsonpcallback");
    	return jsonp+"("+s+")";
```
jsonp
类型：String
在一个 jsonp 请求中重写回调函数的名字。这个值用来替代在 "callback=?" 这种 GET 或 POST 请求中 URL 参数里的 "callback" 部分，比如 {jsonp:'onJsonPLoad'} 会导致将 "onJsonPLoad=?" 传给服务器。
jsonpCallback
类型：String
为 jsonp 请求指定一个回调函数名。这个值将用来取代 jQuery 自动生成的随机函数名。这主要用来让 jQuery 生成度独特的函数名，这样管理请求更容易，也能方便地提供回调函数和错误处理。你也可以在想让浏览器缓存 GET 请求的时候，指定这个回调函数名。
