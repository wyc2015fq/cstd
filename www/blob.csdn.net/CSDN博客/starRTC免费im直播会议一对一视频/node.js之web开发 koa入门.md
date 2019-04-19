# node.js之web开发 koa入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月30日 19:19:02[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：215
用Node.js开发Web服务器端，有几个显著的优势：
速度快，非常快！这得益于Node.js天生是异步的。
常见的Web框架包括：[Express](http://expressjs.com/)，[Sails.js](http://sailsjs.org/)，[koa](http://koajs.com/)，[Meteor](https://www.meteor.com/)，[DerbyJS](http://derbyjs.com/)，[Total.js](https://www.totaljs.com/)，[restify](http://restify.com/)……
构建工具有：[Grunt](http://gruntjs.com/)，[Gulp](http://gulpjs.com/)，[Webpack](http://webpack.github.io/)……
好消息是这个教程已经帮你选好了，你只需要跟着教程一条道走到黑就可以了。
koa是Express的下一代基于Node.js的web框架，目前有1.x和2.0两个版本。建议用2.0
Express是第一代最流行的web框架，它对Node.js的http进行了封装
**var**
 express = **require**('express');**var**
 app = express();app.get('/',
**function** (req,
 res) { res.send('Hello World!');});app.listen(3000,
**function** ()
 { console.log('Example app listening on port 3000!');});
koa2的代码看上去像这样：
app.**use**(async
 (ctx, next) => { await next(); **var**
 data = await doReadFile(); ctx.response.type = 'text/plain';
 ctx.response.body = data;});
koa入门
如何安装koa2
可以用npm命令直接安装koa。先打开命令提示符，务必把当前目录切换到文件所在的那个目录，然后执行命令：
npm install koa@2.0.0
任何时候都可以直接删除整个node_modules目录，因为用npm
 install命令可以完整地重新下载所有依赖。并且，这个目录不应该被放入版本控制中。
或者通过下面方式安装：
创建一个package.json，这个文件描述了我们的hello-koa工程会用到哪些包
{ "name":
"hello-koa2", "version":
"1.0.0", "description":
"Hello Koa 2 example with async",
 "main":
"app.js", "scripts":
 { "start":
"node app.js" }, "keywords":
 [ "koa",
"async" ], "author":
"Michael Liao", "license":
"Apache-2.0", "repository":
 { "type":
"git", "url":
"https://github.com/michaelliao/learn-javascript.git"
 }, "dependencies": { "koa":
"2.0.0" }}
其中，dependencies描述了我们的工程依赖的包以及版本号。其他字段均用来描述项目信息，可任意填写。
然后，我们在hello-koa目录下执行npm
 install就可以把所需包以及依赖包一次性全部装好：
npm start命令会让npm执行定义在package.json文件中的start对应命令

*// 导入koa，和koa 1.x不同，在koa2中，我们导入的是一个class，因此用大写的Koa表示:***const**
 Koa = **require**('koa');*// 创建一个Koa对象表示web app本身:***const**
 app = **new**
 Koa();*// 对于任何请求，app将调用该**异步函数**处理请求：*app.**use**(async
 (ctx, next) => { await next(); ctx.response.type = 'text/html';
 ctx.response.body = '<h1>Hello, koa2!</h1>';});*// 在端口3000监听:*app.listen(3000);console.log('app
 started at port 3000...');
其中，参数ctx是由koa传入的封装了request和response的变量，我们可以通过它访问request和response，next是koa传入的将要处理的下一个异步函数。
async标记的函数称为异步函数，在异步函数中，可以用await调用另一个异步函数，这两个关键字将在ES7中引入。
每收到一个http请求，koa就会调用通过app.use()注册的async函数，并传入ctx和next参数。
为什么要调用await
 next()？
原因是koa把很多async函数组成一个处理链，每个async函数都可以做一些自己的事情，然后用await
 next()来调用下一个async函数。我们把每个async函数称为middleware，这些middleware可以组合起来，完成很多有用的功能。
console.log(`${ctx.request.method} ${ctx.request.url}`);
*// 打印URL*
**const**
 ms = **new**
 Date().getTime() - start; *// 耗费时间*
 console.log(`Time: ${ms}ms`); *// 打印耗费时间*
如果一个middleware没有调用await
 next()，会怎么办？答案是后续的middleware将不再执行了。
例如，一个检测用户权限的middleware可以决定是否继续处理请求，还是直接返回403错误：
app.use(async (ctx,
**next**) => {
**if** (await
 checkUserPermission(ctx)) { await **next**();
 } **else** {
 ctx.response.status = 403;
 }});
处理url路由
"koa-router": "7.0.0"
**const**
 Koa = require('koa');*// 注意require('koa-router')返回的是函数:***const**
 router = require('koa-router')();**const**
 app = **new**
 Koa();*// log request URL:*app.use(async
 (ctx, next) => { console.log(`Process ${ctx.request.method} ${ctx.request.url}...`); await next();});*// add url-route:*router.get('/hello/:name',
 async (ctx, next) => { **var**
 name = ctx.params.name; ctx.response.body
 = `<h1>Hello, ${name}!</h1>`;});router.get('/',
 async (ctx, next) => { ctx.response.body = '<h1>Index</h1>';});//
 add router middleware:app.use(router.routes());app.listen(3000);console.log('app started at port 3000...');
注意导入koa-router的语句最后的()是函数调用：
处理post请求
可以用router.post('/path',
 async ...
注意：需要引入另一个middleware来解析原始request请求，然后，把解析后的参数，绑定到ctx.request.body中。
"koa-bodyparser": "3.2.0"
**const**
 bodyParser = **require**('koa-bodyparser');
在合适的位置加上：
app.**use**(bodyParser());
由于middleware的顺序很重要，这个koa-bodyparser必须在router之前被注册到app对象上。
name = ctx.request.body.name || '', //
默认值
把URL处理函数集中到某几个js文件中
比如 在controllers目录下编写index.js：
var fn_index = async (ctx, next) => { ctx.response.body = `<h1>Index</h1><form
action="/signin"method="post"><p>Name:
<input
name="name"value="koa"></p><p>Password:
<input
name="password"type="password"></p><p><input
type="submit"value="Submit"></p></form>`;};
...
module.exports = { 'GET /': fn_index, 'POST /signin': fn_signin};
另一个
**var**
 fn_hello = async (ctx, next) => { **var**
 name = ctx.params.name; ctx.response.body = `<h1>Hello, ${name}!</h1>`;};module.exports
 = { 'GET /hello/:name': fn_hello};
修改app.js，让它自动扫描controllers目录，找到所有js文件，导入，然后注册每个URL：
[http://es6.ruanyifeng.com/#docs/let](http://es6.ruanyifeng.com/#docs/let)
let命令，用来声明变量。它的用法类似于var，但是所声明的变量，只在let命令所在的代码块内有效。for循环的计数器，就很合适使用let命令。
**const**
 fs = **require**('fs');
**function**addMapping(router, mapping)
 { **for** (**var**
 url **in** mapping)
 { **if** (url.startsWith('GET
 ')) {
**var** path =
 url.substring(4); router.get(path,
 mapping[url]); console.log(`register URL mapping: GET ${path}`); } **else****if** (url.startsWith('POST
 ')) {
**var** path =
 url.substring(5); router.post(path,
 mapping[url]); console.log(`register URL mapping: POST ${path}`); } **else**
 { console.log(`invalid URL: ${url}`); } }}**function**addControllers(router) {
**var** files
 = fs.readdirSync(__dirname + '/controllers');
**var** js_files
 = files.filter((f) => { **return**
 f.endsWith('.js'); });
**for** (**var**
 f of js_files) { console.log(`process controller: ${f}...`); **let**
 mapping = require(__dirname + '/controllers/'
 + f); addMapping(router, mapping); }}//addControllers(router);
module.exports =
**function** (dir)
 { let controllers_dir = dir || 'controllers',
*// 如果不传参数，扫描目录默认为'controllers'*
 router = **require**('koa-router')();
 addControllers(router, controllers_dir);
**return** router.routes();};
主文件中
*// 导入controller middleware:***const**
 controller = **require**('./controller');...*// 使用middleware:*app.**use**(controller());
其它：
Nunjucks模板引擎

[https://www.nodebeginner.org/index-zh-cn.html](https://www.nodebeginner.org/index-zh-cn.html)
