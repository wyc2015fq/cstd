# javascript模块化编程：CommonJS和AMD规范 - 左直拳的马桶_日用桶 - CSDN博客
2017年02月10日 17:36:17[左直拳](https://me.csdn.net/leftfist)阅读数：579
AMD规范，异步模块定义。与CommonJS规范齐名并列。 作用都是利于JavaScript的模块化编程。
模块化编程的好处就是： 
1、可重用 
2、独立 
3、能解决加载的依赖性问题 
4、能解决重复加载问题
CommonJS是Mozilla 的工程师 Kevin Dangoor 在2009年1月创建的，当时的名字是 ServerJS，后来改名为 CommonJS，以显示其 API 的更广泛实用性。CommonJS是一套规范，创建和核准是开放的，已经有很多版本和具体实现。CommonJS 并不属于 ECMAScript TC39 小组的工作，但 TC39 中的一些成员参与了制定。
比较而言，CommonJS针对的是服务器和桌面等浏览器之外的环境，而AMD则是for浏览器。或者说，**CommonJS同步，是服务器端的规范，而AMD异步，是客户端的规范**。
因为对服务器来说，所有的模块都存放在本地硬盘，可以同步加载完成，等待时间就是硬盘的读取时间。但是，对于浏览器，这却是一个大问题，因为模块都放在服务器端，等待时间取决于网速的快慢，可能要等很长时间，浏览器处于”假死”状态。
因此，AMD规范规定异步方式加载模块，模块的加载不影响它后面语句的运行。所有依赖这个模块的语句，都定义在一个回调函数中，等到加载完成之后，这个回调函数才会运行。
不过，其实CommonJS也有浏览器端的实现，其原理是现将所有模块都定义好并通过 id 索引，这样就可以方便的在浏览器环境中解析了。
**【CommonJS】**
CommonJS规范规定，每个模块内部，module变量代表当前模块。这个变量是一个对象，它的exports属性（即module.exports）是对外的接口。加载某个模块，其实是加载该模块的module.exports属性。require方法用于加载模块。
```
//example.js
var x = 5;
var addX = function (value) {
  return value + x;
};
module.exports.x = x;
module.exports.addX = addX;
```
```
var example = require('./example.js');
console.log(example.x); // 5
console.log(example.addX(1)); // 6
```
实现CommonJS的代表是node.js。
**【AMD规范】**
本规范只定义了一个函数 “define”，它是全局变量。函数的描述为：
```
define(id?, dependencies?, factory);
id:模块名称，字符串，可选
dependencies:依赖模块，数组，可选
factory:回调函数。此函数的参数要与依赖模块一一对应
```
示例。定义：
```java
//math.js
define([], function ($) {
    function _add(a,b){
        return a + b;
    }
    return {
        add: _add
    };
});
```
调用
```
require(['math'], function (math) {
　　　　math.add(2, 3);
　　});
```
本质上，这里的define返回的是一个json，元素为一个个函数，跟我们平时所用的
```java
var f = (function(){
    function _add(){}
    return {
        add:_add
    }
})();
```
极其相像。
目前，主要有两个Javascript库实现了AMD规范：require.js和curl.js。至于CommonJS，主要是node.js吧。然而，2013年5月，Node.js 的包管理器 NPM 的作者 Isaac Z. Schlueter 说 CommonJS 已经过时，Node.js 的内核开发者已经废弃了该规范。
参考文章： 
[CommonJS 规范](http://zhaoda.net/webpack-handbook/commonjs.html)
[AMD 规范](http://zhaoda.net/webpack-handbook/amd.html)
[Javascript模块化编程（二）：AMD规范](http://www.ruanyifeng.com/blog/2012/10/asynchronous_module_definition.html)
[AMD (中文版)](https://github.com/amdjs/amdjs-api/wiki/AMD-%28%E4%B8%AD%E6%96%87%E7%89%88%29)
