# Ajax实现跨域请求 - 零度的博客专栏 - CSDN博客
2015年08月27日 17:45:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：1667
1、Get方式请求--使用jsonp
         什么是jsonp格式呢？如果获取的数据文件存放在远程服务器上（域名不同，也就是跨域获取数据），则需要使用jsonp类型。使用这种类型的话，会创建一个查询字符串参数 callback=? ，这个参数会加在请求的URL后面。服务器端应当在JSON数据前加上回调函数名，以便完成一个有效的JSONP请求。意思就是远程服务端需要对返回的数据做下处理，根据客户端提交的callback的参数，返回一个callback(json)的数据，而客户端将会用script的方式处理返回数据，来对json数据做处理。JQuery.getJSON也同样支持jsonp的数据方式调用。
客户端JS：
```
var xhrurl = 'http://localhost:8001/Ajax/ticketNotify.ashx?cu=kefu1';
  $.ajax({
          type : "get",
          async : false,
          url :xhrurl, 
          cache : false,
          dataType : "jsonp",
          jsonp: "callbackparam",
          jsonpCallback:"jsonpCallback1",
          success : function(json){
             alert(json[0].name);
          },
          error:function(e){
             alert("error");
         }
       });
```
服务端代码：
```
String callbackFunName = context.Request["callbackparam"];
             context.Response.Write(callbackFunName + "([ { \"name\":\"John\"}])");
```
注意：客户端的jsonp参数是用来通过url传参，传递jsonpCallback参数的参数名，比较拗口，通俗点讲：
jsonp: "callbackparam"
jsonpCallback:"jsonpCallback1"这两个参数最终会拼接在请求的url后面，变成 http://www.xxx.com/ajax/xxx.ashx?callbackparam=jsonCallback1
服务端要获取这个参数值："jsonCallback1"  ,拼接在要输出的JSON数据最前面。2、Post方式请求--使用CORS   CORS定义一种跨域访问的机制，可以让AJAX实现跨域访问。CORS 允许一个域上的网络应用向另一个域提交跨域 AJAX 请求。实现此功能非常简单，只需由服务器发送一个响应标头即可。   CORS浏览器支持情况如下图：　　假设我们页面或者应用已在http://www.test1.com 上了，而我们打算从http://www.test2.com 请求提取数据。一般情况下，如果我们直接使用 AJAX 来请求将会失败，浏览器也会返回“源不匹配”的错误，“跨域”也就以此由来。　　利用 CORS，http://www.test2.com 只需添加一个标头，就可以允许来自 http://www.test1.com 的请求，设置，**“*”号表示允许任何域向我们的服务端提交请求**：**也可以设置指定的域名，如域名http://www.test2.com ，那么就允许来自这个域名的请求：**　　当前我设置的header为“*”，任意一个请求过来之后服务端我们都可以进行处理&响应，那么在调试工具中可以看到其头信息设置，其中见红框中有一项信息是“Access-Control-Allow-Origin: *"表示我们已经启用CORS，如下图。
 　　简单的一个header设置，一个支持跨域POST请求的server就完成了
