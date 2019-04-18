# ajax 设置Access-Control-Allow-Origin实现跨域访问 - z69183787的专栏 - CSDN博客
2016年12月07日 15:29:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3179
ajax跨域访问是一个老问题了，解决方法很多，比较常用的是JSONP方法，JSONP方法是一种非官方方法，而且这种方法只支持GET方式，不如POST方式安全。
即使使用[jQuery](http://lib.csdn.net/base/jquery)的jsonp方法，type设为POST，也会自动变为GET。
官方问题说明：
“script”: Evaluates the response as [JavaScript](http://lib.csdn.net/base/javascript) and returns it as
 plain text. Disables caching by appending a query string parameter, “_=[TIMESTAMP]“, to the URL unless the cache option is set to true.Note: This will turn POSTs into GETs for remote-domain requests.
如果跨域使用POST方式，可以使用创建一个隐藏的iframe来实现，与ajax上传图片原理一样，但这样会比较麻烦。
**因此，通过设置Access-Control-Allow-Origin来实现跨域访问比较简单。**
例如：客户端的域名是**www.client.com**,而请求的域名是**www.server.com**
如果直接使用ajax访问，会有以下错误
XMLHttpRequest cannot load http://www.server.com/server.[PHP](http://lib.csdn.net/base/php). No 'Access-Control-Allow-Origin'
 header is present on the requested resource.Origin 'http://www.client.com' is therefore not allowed access.
**在被请求的Response header中加入**
**[php]**[view
 plain](http://blog.csdn.net/fdipzone/article/details/46390573/#)[copy](http://blog.csdn.net/fdipzone/article/details/46390573/#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/686401/fork)
- // 指定允许其他域名访问
- header('Access-Control-Allow-Origin:*');  
- // 响应类型
- header('Access-Control-Allow-Methods:POST');  
- // 响应头设置
- header('Access-Control-Allow-Headers:x-requested-with,content-type');  
就可以实现ajax POST跨域访问了。
**代码如下：**
**client.html** 路径：http://www.client.com/client.html
**[html]**[view
 plain](http://blog.csdn.net/fdipzone/article/details/46390573/#)[copy](http://blog.csdn.net/fdipzone/article/details/46390573/#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/686401/fork)
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
- <html>
- <head>
- <metahttp-equiv="content-type"content="text/html;charset=utf-8">
- <title> 跨域测试 </title>
- <scriptsrc="//code.jquery.com/jquery-1.11.3.min.js"></script>
- </head>
- 
- <body>
- <divid="show"></div>
- <scripttype="text/javascript">
-     $.post("http://www.server.com/server.php",{name:"fdipzone",gender:"male"})  
-       .done(function(data){  
-         document.getElementById("show").innerHTML = data.name + ' ' + data.gender;  
-       });  
- </script>
- </body>
- </html>
**server.php** 路径：http://www.server.com/server.php
**[php]**[view
 plain](http://blog.csdn.net/fdipzone/article/details/46390573/#)[copy](http://blog.csdn.net/fdipzone/article/details/46390573/#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/686401/fork)
- <?php  
- $ret = array(  
- 'name' => isset($_POST['name'])? $_POST['name'] : '',  
- 'gender' => isset($_POST['gender'])? $_POST['gender'] : ''
- );  
- 
- header('content-type:application:json;charset=utf8');  
- header('Access-Control-Allow-Origin:*');  
- header('Access-Control-Allow-Methods:POST');  
- header('Access-Control-Allow-Headers:x-requested-with,content-type');  
- 
- echo json_encode($ret);  
- ?>  
**Access-Control-Allow-Origin:*** 表示允许任何域名跨域访问
如果需要指定某域名才允许跨域访问，只需把Access-Control-Allow-Origin:*改为Access-Control-Allow-Origin:允许的域名
例如：header('Access-Control-Allow-Origin:http://www.client.com');
如果需要设置多个域名允许访问，这里需要用php处理一下
例如允许 www.client.com 与 www.client2.com 可以跨域访问
**server.php 修改为**
**[php]**[view
 plain](http://blog.csdn.net/fdipzone/article/details/46390573/#)[copy](http://blog.csdn.net/fdipzone/article/details/46390573/#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/686401/fork)
- <?php  
- $ret = array(  
- 'name' => isset($_POST['name'])? $_POST['name'] : '',  
- 'gender' => isset($_POST['gender'])? $_POST['gender'] : ''
- );  
- 
- header('content-type:application:json;charset=utf8');  
- 
- $origin = isset($_SERVER['HTTP_ORIGIN'])? $_SERVER['HTTP_ORIGIN'] : '';  
- 
- $allow_origin = array(  
- 'http://www.client.com',  
- 'http://www.client2.com'
- );  
- 
- if(in_array($origin, $allow_origin)){  
-     header('Access-Control-Allow-Origin:'.$origin);  
-     header('Access-Control-Allow-Methods:POST');  
-     header('Access-Control-Allow-Headers:x-requested-with,content-type');  
- }  
- 
- echo json_encode($ret);  
- ?>  
- 
