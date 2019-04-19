# Jsonp跨域获取json数据原理 - 零度的博客专栏 - CSDN博客
2016年04月25日 10:46:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：1722
其实 jsonp 是个很简单的一个东西。
主要是利用了 <script/> 标签对 javascript 文档的动态解析来实现。（其实也可以用eval函数）
来个超简单的例子：
首先是准备客户端的代码，
**[html]**[view plain](http://blog.csdn.net/z69183787/article/details/8110985#)[copy](http://blog.csdn.net/z69183787/article/details/8110985#)[print](http://blog.csdn.net/z69183787/article/details/8110985#)[?](http://blog.csdn.net/z69183787/article/details/8110985#)
- <spanstyle="font-size: 18px;"><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <title>Test Jsonp</title>
- <scripttype="text/javascript">
-             function jsonpCallback(result)  
-             {  
-             alert(result.msg);  
-             }  
- </script>
- <scripttype="text/javascript"src="http://crossdomain.com/jsonServerResponse?jsonp=jsonpCallback"></script>
- </head>
- <body>
- </body>
- </html></span>
```
<span style="font-size:18px;"><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title>Test Jsonp</title>
	<script type="text/javascript">
        	function jsonpCallback(result)
        	{
			alert(result.msg);
        	}
    	</script>
	<script type="text/javascript" src="http://crossdomain.com/jsonServerResponse?jsonp=jsonpCallback"></script>
</head>
<body>
</body>
</html> </span>
```
其中 jsonCallback 是客户端注册的，获取跨域服务器上的json数据后，回调的函数。
http://crossdomain.com/jsonServerResponse?jsonp=jsonpCallback
这个 url 是跨域服务器取 json 数据的接口，参数为回调函数的名字，返回的格式为
**[javascript]**[view plain](http://blog.csdn.net/z69183787/article/details/8110985#)[copy](http://blog.csdn.net/z69183787/article/details/8110985#)[print](http://blog.csdn.net/z69183787/article/details/8110985#)[?](http://blog.csdn.net/z69183787/article/details/8110985#)
- jsonpCallback({msg:'this is json data'})
```java
<span style="font-size:18px;">jsonpCallback({msg:'this is json data'})</span>
```
Jsonp原理：
首先在客户端注册一个callback, 然后把callback的名字传给服务器。
此时，服务器先生成 json 数据。 
然后以 javascript 语法的方式，生成一个function , function 名字就是传递上来的参数 jsonp.
最后将 json 数据直接以入参的方式，放置到 function 中，这样就生成了一段 js 语法的文档，返回给客户端。
客户端浏览器，解析script标签，并执行返回的 javascript 文档，此时数据作为参数，传入到了客户端预先定义好的 callback 函数里.（动态执行回调函数）
