# app.use - 左直拳的马桶_日用桶 - CSDN博客
2014年12月09日 10:41:18[左直拳](https://me.csdn.net/leftfist)阅读数：4806
这里说的app，是指express对象
var express = require('express');
var app = express();
其中，*app.use是express“调用中间件的方法”。所谓“中间件（middleware），就是处理HTTP请求的函数，用来完成各种特定的任务，比如检查用户是否登录、分析数据、以及其他在需要最终将数据发送给用户之前完成的任务。”*。这是阮一峰文章的原话。
简而言之，app.use() 里面使用的参数，主要是函数。但这个使用，并不是函数调用，而是使能的意思。当用户在浏览器发出请求的时候，这部分函数才会启用，进行过滤、处理。
express的路由，众所周知，是app.get(),app.post(),app.all()，。。。，但其实，它们都是app.use的别名，呵呵。怪不得，我说为什么看上去，app.use 跟它们那么像：app.use的调用方式，除了类似 app.use(匿名函数或函数名)，也可以是类似 app.use("/",匿名函数或函数名)。
有代码有J8：
```java
var express = require('express');
var app = express();
app.use("/about",function(req,res){
	console.log("关于");
	res.send("wroted by leftfist");
});
app.use(function(req,res,next){
	console.log("步骤一");
	res.writeHead(200, { "Content-Type": "text/plain" });
	next();
});
app.use(function(req,res){
	console.log("步骤二");
	res.end("Hello World!");
});
var server = app.listen(3000,function(){
	console.log("http server is ready on 3000")
})
```
参考文章：
[http://javascript.ruanyifeng.com/nodejs/express.html#toc5](http://javascript.ruanyifeng.com/nodejs/express.html#toc5)
