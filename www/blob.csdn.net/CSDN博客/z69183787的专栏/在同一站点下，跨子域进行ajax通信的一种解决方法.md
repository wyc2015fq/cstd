# 在同一站点下，跨子域进行ajax通信的一种解决方法 - z69183787的专栏 - CSDN博客
2014年05月17日 06:54:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1015
个人分类：[Javascript-Ajax](https://blog.csdn.net/z69183787/article/category/2270715)
处于安全的考虑，浏览器对于ajax请求是有限定的，他要求发起和相应请求的双方处于相同的域名。
![](http://bbs.blueidea.com/data/attachment/album//201006/14/536324_1276475239xuen.gif)
一种比较容易的方式就是在当前的域名下设置一个代理页面，通过它来发送请求和转接回复的数据，从而实现ajax的跨域请求。这种方式的优势在于可以跨站点而不仅限于跨子域，不过这样做还有一个弊端就是客户端的session信息将会丢失，在一些需要session的情形(比如需要用户登录验证的情况)就不能适用。
![](http://bbs.blueidea.com/data/attachment/album//201006/14/536324_127647676810B3.gif)
这里介绍的是另一种方法。
解决办法其实和上面的代理页面方式有些类似，上面的方式我们实际上是借用代理页面来发送请求和接受数据，从而绕开浏览器的安全设定。而在这里我们将借用代理页面所处的环境，通过它来实现功能。
首先在想要进行通信的那个子域下设置一个"代理"页面，在这个页面中我们是能对这个子域下的数据进行ajax请求的。
接着就是需要建立一个通道来使用"代理"页面的这一权限，我们在需要使用跨子域的页面中通过iframe引入代理页面，然后通过javascript就能获得代理页面的javascritp环境(iframeEle.contentWindow)，不过这里有个前提就是代理页面需要和当前页面处于同一域中(document.domain需要一致)。由于对于document.domain的值设定时有一个规定就是，域只能提升，不能降低层级或者跳到同级中(比如
 home.blueidea.com 只能设为blueidea.com 而不能设为 bbs.blueidea.com或者 xx.home.blueidea.com)，所以这里可以在两个页面中同时把document.domain提升为站点的主域。
通过以上的实现，我们就可以在页面中调用想要进行通信的子域下的"代理"页面来进行ajax通讯了。
![](http://bbs.blueidea.com/data/attachment/album//201006/14/536324_12764752413O8z.gif)
以下是一个例子:
在yy.foo.com下设置一个页面，内容如下
yy.foo.com/proxy.html
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>proxy</title>
</head>
<body>
<script type="text/javascript">
document.domain = 'foo.com';
// 发送ajax请求的方法
function send(url, callback, method, data, encoding, async){
var xmlhttp = null;
var url = url || null;
var method = (method === 'POST') ? 'POST' : 'GET';
var callback = callback || function(){};
var data = data || null;
var encoding = encoding || 'utf-8';
var async = (async === false) ? false : true; 
if (url == null) return null;
// create instance of request
if (window.XMLHttpRequest){
 xmlhttp=new XMLHttpRequest();
} else {
 xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
}
if (!xmlhttp) return null;
xmlhttp.onreadystatechange=function(){
 if (xmlhttp.readyState !=4) return;  
 if (xmlhttp.status >= 200 && xmlhttp.status < 300){
callback({
status: 'success',
responseText: xmlhttp.responseText,
responseXML: xmlhttp.responseXML
});
 }
 else {
callback({
status: 'failure'
});
 }
}
xmlhttp.open(method, url, async);
xmlhttp.setRequestHeader("Content-type", "text/xml; charset="+encoding);
xmlhttp.send(data);
return xmlhttp;
}
function abort(xmlhttp){
if (!xmlhttp) return;
xmlhttp.abort();
}
</script>
</body>
</html>
在xx.foo.com页面中通过iframe引入proxy.html
<iframe id="proxy" src="yy.foo.com/proxy.html"></iframe>
设置xx.foo.com的domain
...
document.domain = 'foo.com';
...
发送请求 (foo.com中)
...
// 请求的回调函数
var callback = function(e){
if (e.status === 'success') {
// 成功
var responseText = e.responseText;
var responseXML = e.responseXML;
...
} else {
//失败
...
}
};
var proxy = document.getElementById('proxy').contentWindow;
// 调用代理页面中的方法发送请求
proxy.send('yy.foo.com/data', callback);
