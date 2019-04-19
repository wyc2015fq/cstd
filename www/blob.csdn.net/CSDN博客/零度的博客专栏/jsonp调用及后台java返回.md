# jsonp调用及后台java返回 - 零度的博客专栏 - CSDN博客
2016年05月04日 13:30:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：3104标签：[jsonp																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jsonp&t=blog)
个人分类：[JSON/JSONP](https://blog.csdn.net/zmx729618/article/category/6197661)
**[javascript]**[view plain](http://blog.csdn.net/z69183787/article/details/15809167#)[copy](http://blog.csdn.net/z69183787/article/details/15809167#)[print](http://blog.csdn.net/z69183787/article/details/15809167#)[?](http://blog.csdn.net/z69183787/article/details/15809167#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/318086/fork)
- <script>  
-                   $.ajax({  
-                     type: 'GET',  
-                     url: '<%=urls%>',  
-                     dataType:'jsonp',  
-                     data:{  
- "token" : '<%=token%>',     
- "method" : '<%=method%>',               
- "appName" : '<%=appName%>',     
- "secret" : '<%=secret%>',   
- "dataType" : 'json',      
- "dataParams" : '<%=dataParams%>',   
- "sign" : '<%=sign%>'
-                         },  
-                         jsonp:'jsonpcallback',  
-                     error: function(XmlHttpRequest,textStatus,errorThrown){  
-                             alert("部门切换失败，请联系管理员!");  
-                             top.location.href = '/portal/mainFrame/frame.html';  
-                         },  
-                     success: function(msg){       
- if(msg.code=="100"){  
-                                 top.location.href = "clearToken.jsp?appName=<%=caAppName%>";  
-                             }else{  
-                                 alert("部门切换失败，请联系管理员!\n错误代码："+msg.code);  
-                                 top.location.href = '/portal/mainFrame/frame.html';  
-                             }  
- 
-                         }         
-                   });  
-             </script>  
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
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/15809167#)[copy](http://blog.csdn.net/z69183787/article/details/15809167#)[print](http://blog.csdn.net/z69183787/article/details/15809167#)[?](http://blog.csdn.net/z69183787/article/details/15809167#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/318086/fork)
- String jsonp=request.getParameter("jsonpcallback");  
- return jsonp+"("+s+")";  
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
