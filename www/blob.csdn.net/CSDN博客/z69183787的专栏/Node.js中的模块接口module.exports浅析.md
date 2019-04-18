# Node.js中的模块接口module.exports浅析 - z69183787的专栏 - CSDN博客
2017年02月08日 14:22:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2765
在写node.js代码时，我们经常需要自己写模块(module)。同时还需要在模块最后写好模块接口，声明这个模块对外暴露什么内容。实际上，node.js的模块接口有多种不同写法。这里作者对此做了个简单的总结。
返回一个JSON Object
如下代码是一个简单的示例。
1 var exp = { 
2   "version": "1.0.0", 
3   "function1": null, 
4   "module1": null,
5 };
6 module.exports = exp;
这种方式可以用于返回一些全局共享的常量或者变量，例如
1 // MATH.js
2 var MATH = {
3   "pi": 3.14,
4   "e": 2.72,
5 };
6 
7 module.exports = MATH;
调用方式为
1 var MATH = require("./MATH")
2 console.log(MATH.pi);
这种方式还可以用于返回几个require的其他模块，可以实现一次require多个模块
1 // module_collection.js
2 var module_collection = {
3   "module1": require("./module1"),
4   "module2": require("./module2"),
5 };
6 
7 module.exports = module_collection;
调用方式为
1 var module_collection = require("./module_collection");
2 var module1 = module_collection.module1;
3 var module2 = module_collection.module2;
4 // Do something with module1 and module2
其实这种方式还有个变种，如下，通常可以返回几个函数
 1 // functions.js
 2 var func1 = function() {
 3   console.log("func1");
 4 };
 5 
 6 var func2 = function() {
 7   console.log("func2");
 8 };
 9 
10 exports.function1 = func1;
11 exports.function2 = func2;
调用方式为
1 var functions = require("./functions");
2 functions.function1();
3 functions.function2();
返回一个构造函数，也就是一个类
如下是一个简单的示例。
 1 // CLASS.js
 2 var CLASS = function(args) {
 3   this.args = args;
 4 }
 5 
 6 CLASS.prototype.func = function() {
 7   console.log("CLASS.func");
 8   console.log(this.args);
 9 };
10 
11 module.exports = CLASS;
调用方法为
1 var CLASS = require("./CLASS")
2 var c = new CLASS("arguments");
返回一个普通函数
如下是一个简单的示例
复制代码
1 // func.js
2 var func = function() {
3   console.log("this is a testing function");
4 };
5 
6 module.exports = func;
调用方法为
1 var func = require("./func");
2 func();
返回一个对象object
如下是一个简单的示例
 1 // CLASS.js
 2 var CLASS = function() {
 3   this.say_hello = "hello";
 4 };
 5 
 6 CLASS.prototype.func = function() {
 7   console.log("I say " + this.say_hello);
 8 };
 9 
10 module.exports = new CLASS();
调用方法为
1 var obj = require("./CLASS");
2 obj.func();
有时候我们需要模块返回一个单例 singleton. 可以利用上面的方式1和方式4来实现。也就是如下两种形式
1 // MATH.js
2 var MATH = {
3   "pi": 3.14,
4   "e": 2.72,
5 };
6 
7 module.exports = MATH;
以及
 1 // CLASS.js
 2 var CLASS = function() {
 3   this.say_hello = "hello";
 4 };
 5 
 6 CLASS.prototype.func = function() {
 7   console.log("I say " + this.say_hello);
 8 };
 9 
10 module.exports = new CLASS();





