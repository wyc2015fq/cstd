# koa介绍 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月02日 14:07:02[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：379
[https://github.com/koajs/koa](https://github.com/koajs/koa)
[https://github.com/demopark/koa-docs-Zh-CN](https://github.com/demopark/koa-docs-Zh-CN)
ctx.response.type='json';
app.use()用来加载中间件
app.use(ctx
=> { ctx.body='Hello Koa';});
以"先进后出"（first-in-last-out）的顺序执行。
- 最外层的中间件首先执行。
- 调用next函数，把执行权交给下一个中间件。
- ...
- 最内层的中间件最后执行。
- 执行结束后，把执行权交回上一层的中间件。
- ...
- 最外层的中间件收回执行权之后，执行next函数后面的代码。
const one
=(ctx,
 next)=>{ console.log('>>
 one');next();
 console.log('<<
 one');}const
 two =(ctx,
 next)=>{
console.log('>>
 two');next();
 console.log('<<
 two');}const
 three =(ctx,
 next)=>{ console.log('>>
 three');next();
 console.log('<<
 three');}app.use(one);app.use(two);app.use(three);
有如下输出。
>> one>>
 two>> three<<
 three<< two<<
 one
上面都是同步的中间件，异步的需要加async
const main
= async
function(ctx,
 next){ ctx.response.type
='html';
 ctx.response.body
= await fs.readFile('./demos/template.html','utf8');};app.use(main);
上面代码中，fs.readFile是一个异步操作，必须写成await
 fs.readFile()，然后中间件必须写成 async 函数。
上下文
[https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/context.md](https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/context.md)
请求
[https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/request.md](https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/request.md)
响应
[https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/response.md](https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/response.md)
async function doWork() { // 使用 async/await 的方式依次获取两个 JSON 文件 var result1 = await fetch('data1.json'); var result2 = await fetch('data2.json'); return { result1, result2 };}
这个语法可以理解为：被 async 关键字标记的函数，可以对其使用 await 关键字来暂停函数的执行直到异步操作结束。
[https://chenshenhai.github.io/koa2-note/note/start/async.html](https://chenshenhai.github.io/koa2-note/note/start/async.html)
async
function indexStep1(ctx,next){//逻辑处理第一部分await
next();}async
function indexStep2(ctx,next){//逻辑处理第二部分await
next();}async
function indexStep3(ctx,next){//逻辑处理第三部分await
 ctx.render('index');}router.get('/index',
 indexStep1, indexStep2,
 indexStep3);
同时实现get和post
router.all
[https://wohugb.gitbooks.io/koajs/content/route/koa-router.html](https://wohugb.gitbooks.io/koajs/content/route/koa-router.html)
Create routes with multiple HTTP methods using router.register():
app.register('/',
 ['get',
'post'],
function *(next){
// ...});
Create route for all methods using router.all():
app.all('/',
function *(next){
// ...});
获取参数
eventCenter?data1=leijh
var data = ctx.request.query.data || null;
[https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/request.md](https://github.com/demopark/koa-docs-Zh-CN/blob/master/api/request.md)
