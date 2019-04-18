# Javascript Promise入门 - weixin_33985507的博客 - CSDN博客
2016年08月21日 16:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
# 是什么？
https://www.promisejs.org/
> 
## What is a promise?
The core idea behind promises is that a promise represents the result of an asynchronous operation. A promise is in one of three different states:
- pending - The initial state of a promise.
- fulfilled - The state of a promise representing a successful operation.
- rejected - The state of a promise representing a failed operation.
Once a promise is fulfilled or rejected, it is immutable (i.e. it can never change again).
promise的核心思想是， promise可以表示异步操作的的结果。 包括三个状态， pending， fulfiled， rejected
promise 在英文中是诺言， fulfil promise 为实现诺言，  rejected promise 为 收回诺言。 分别表示成功和失败。
用promise 来描述异步操作结果， 是太形象了， 在当下定义了一个 promise， 诺言的结果， 需要在将来来验证， 不影响当下的运行计划，
例如， 你对你老婆说， 你将来一定要买个大房子给你老婆住。  在你说的时候， 不影响你当天或者明天的工作和生活计划， 该编码就编码， 该散步就散步， 
至于以后能不能买到大房子， 那是以后的事情了， 当然诺言要有个期限， 期限已到， 按照诺言， 需要买房， 如果能买到则 fulfill promise， 如果未实现，则reject promise。
如下面例子： 
定义了一个读取文件的 promise， 读取成功了调用  fulfill回调， 读取失败了 调用 reject回调。
```
function readFile(filename, enc){
  return new Promise(function (fulfill, reject){
    fs.readFile(filename, enc, function (err, res){
      if (err) reject(err);
      else fulfill(res);
    });
  });
}
```
# 为什么？
如果没有promise， 则例如上例中， 异步读取动作， 我们需要在我们的回调函数callback中， 处理各种错误情况， 和成功情况。
错误情况， 包括： 
1、 读取文件失败
2、 解析读取数据失败
成功情况：
1、 解析读取数据成功
使用promise后， 对于成功的情况， 定义成功的处理函数， 对于失败的情况， 定义失败的处理函数。  代码逻辑异常清晰。
```
function readJSON(filename, callback){
  fs.readFile(filename, 'utf8', function (err, res){
    if (err) return callback(err);
    try {
      res = JSON.parse(res);
    } catch (ex) {
      return callback(ex);
    }
    callback(null, res);
  });
}
```
# 怎么用？
除了第一节中，  构造一个promise的例子， 指出要指定 fulfill 和 reject 回调函数， 如果在异步操作完成后， 想做一些其他动作， 则可以调用如下接口，进行定义：
> 
use `.then` whenever you're going to do something with the result (even if that's just waiting for it to finish)
use `.done` whenever you aren't planning on doing anything with the result.
```
function readJSON(filename){
  return readFile(filename, 'utf8').then(function (res){
    return JSON.parse(res)
  })
}
```
结合 then 和 catch接口， 实现的例子：
https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Promise/catch
```
var p1 = new Promise(function(resolve, reject) {
  resolve("成功");
});
p1.then(function(value) {
  console.log(value); // "成功!"
  throw "哦，不!";
}).catch(function(e) {
  console.log(e); // "哦，不!"
});
```
# jquery ajax实现
https://api.jquery.com/jQuery.ajax/#jqXHR
```
$.ajax({
  url: "http://fiddle.jshell.net/favicon.png",
  beforeSend: function( xhr ) {
    xhr.overrideMimeType( "text/plain; charset=x-user-defined" );
  }
})
  .done(function( data ) {
    if ( console && console.log ) {
      console.log( "Sample of data:", data.slice( 0, 100 ) );
    }
  });
```
API
> - **jqXHR.done(function( data, textStatus, jqXHR ) {});**
An alternative construct to the success callback option, refer to `[deferred.done()](https://api.jquery.com/deferred.done/)` for implementation details.
- **jqXHR.fail(function( jqXHR, textStatus, errorThrown ) {});**
An alternative construct to the error callback option, the `.fail()` method replaces the deprecated `.error()` method. Refer to `[deferred.fail()](https://api.jquery.com/deferred.fail/)` for implementation details.
- **jqXHR.always(function( data|jqXHR, textStatus, jqXHR|errorThrown ) { });**
An alternative construct to the complete callback option, the `.always()` method replaces the deprecated `.complete()` method.
In response to a successful request, the function's arguments are the same as those of `.done()`: data, textStatus, and the jqXHR object. For failed requests the arguments are the same as those of `.fail()`: the jqXHR object, textStatus, and errorThrown. Refer to `[deferred.always()](https://api.jquery.com/deferred.always/)` for implementation details.
- **jqXHR.then(function( data, textStatus, jqXHR ) {}, function( jqXHR, textStatus, errorThrown ) {});**
Incorporates the functionality of the `.done()` and `.fail()` methods, allowing (as of jQuery 1.8) the underlying Promise to be manipulated. Refer to [`deferred.then()`](https://api.jquery.com/deferred.then/) for implementation details.
# Promise API标准
https://www.promisejs.org/api/
每个API都有简单的使用介绍， 很是浅显易懂， i like it。
```
var p1 = new Promise(function(resolve, reject) {
  resolve("Success");
});
p1.then(function(value) {
  console.log(value); // "Success!"
  throw "oh, no!";
}).catch(function(e) {
  console.log(e); // "oh, no!"
});
setTimeout(function(){
p1.then(function(value) {
  console.log(value); // "Success!"
  throw "oh, no!";
}).then(undefined, function(e) {
  console.log(e); // "oh, no!"
});
}, 2000)
```
