# express的路由 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月09日 20:58:05[左直拳](https://me.csdn.net/leftfist)阅读数：2983
就是 app.get()、app.post()、app.all()啦，啦啦啦
（其中，var express = require("express"); var app = express();）
之前曾经遇到做一个页面，get没问题，提交就报404错误，后来发现症结在于路由设置，只写了个app.get，意味着只允许GET请求，而不允许POST或其他。法无授权则禁止，法治社会，呵呵呵呵呵呵呵。
说到底，它们都是app.use的别名，呵呵呵呵呵呵。当然，使用get,post等比较专业、合适一点。
这几个函数，除了名字有别，格式是一样的。都是类似
app.get(路由规则,函数);
```java
var express = require('express');
var app = express();
app.get("/",function(req,res){
	res.send("Hello World!");
});
app.get("/hi/leftfist",function(req,res){
	res.send("hello, 左直拳 !");
});
//模式匹配；？号代表可以忽略
app.get("/hi/:who?",function(req,res){
	var who = req.params.who || 'anonymous';
	res.send("hello, " + who + " !");
});
// 或/commits/71dbb9c..4c084f9这样的git格式的网址
app.get(/^\/commits\/(\w+)(?:\.\.(\w+))?$/, function(req, res){
  var from = req.params[0];
  var to = req.params[1] || 'HEAD';
  res.send('commit range ' + from + '..' + to);
});
//404
app.get("*",function(req,res){
	res.send("404!");
});
var server = app.listen(3000,function(){
	console.log("http server is ready on 3000");
});
```
这就是路由的设置。所谓“路由”，就是指为不同的访问路径，指定不同的处理方法。以前，说路由，都想到路由器；后来接触了ASP.NET MVC后，里面才有所谓路由规则。
以往，埋头只知开发WEB FORM，然后在IIS下跑，每个URI，其实都是有一张张实实在在的页面（Page）与之对应，否则就报404错误，Page not found。这种氛围之下，做梦也想不到还有路由规则这种东西。说白了，HTTP服务器就是提供服务，对于浏览器发过来的请求，如何解释，如何响应，完全取决于HTTP服务器如何工作。或者说，你浏览器发过来请求，我http服务器让你看什么，你就看什么。在IIS + WEB FORM中，浏览器请求某一个页面，然后真的有一个页面返回，那是因为IIS真的是将物理页面的内容读取，加上对动态内容的解释，然后返回；假如这个IIS它不去读取物理页面，只返回一句Hello World，那浏览器就只能看到一句Hello World！当然，IIS我们比较难改动，但我们也可以写一个Http Module 或 Http Handler来拦截请求；然而在IIS6或更低版本，这种机制，只对请求.aspx页面有效。并且在IIS那种方便的傻瓜式操作中，一般程序员，怎么会想到有这种所谓的拦截？我以前搞微软的ASP.NET，一直搞，一直搞，从来不明白什么叫Http Module，Http Handler，只在用asp.net 的atlas（微软为了方便程序员使用jquery，而提供的一种控件）的时候，需要配置web.config里有见过，但感到很复杂，令人畏惧，没有做什么了解。IIS的操作对象，就是文件夹，物理页面，所以微软系程序员也就傻逼地认为所谓的路由，就是一定要有真实的页面与之对应！哪里像node.js，路由由自己编写，允许的请求方法也由自己决定，写一次代码下来，原理就能（被迫）有个大致的了解。
微软的问题在于没有区分业余和专业两个概念，也许是windows做得太成功了，让电脑走进了千家万户，傻逼都会使用，于是也把这套思想放到开发工具上来，意图把程序开发也搞成像windows、office一样，人人能用，至少入门门槛非常的低。简单易用的图形界面背后，掩盖了最需要被理解的原理，甚至是为了方便使用，断章取义地扭曲了原理，像上述那个IIS和Page就是活生生的例子。这样子的做法，入门是容易了，但开发人员对原理不理解，造成进阶举步维艰，或者只能做出一些玩具级的应用。
2013年我有几个月去当了老师，发现老师这份职业极其无聊。因为老师的重点在于将知识讲授明白，造成一些简单的东西也要大费口舌，主要精力不在做学问上，所以老师的水平只能是科普水平。同样，也许正是由于微软的精力主要在于打造易于入门、易于使用的开发工具，因此微软的产品，大师级技术水平的东西不多，许多开创性的概念都不出自微软；微软只能亦步亦趋地跟在业界后面，拾人牙慧，然后继续将一些成熟理论包装，搞成易于使用的傻瓜产品，比如asp.net mvc。与此同时，又不得不不断抛弃一些自己封装出来的弱智工具，比如visual foxpro,silverlight，WPF，visual j++ 之类的狗屁。跟着微软走，只能吃屎了。
