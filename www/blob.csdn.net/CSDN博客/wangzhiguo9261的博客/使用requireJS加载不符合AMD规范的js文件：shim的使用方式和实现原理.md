
# 使用requireJS加载不符合AMD规范的js文件：shim的使用方式和实现原理 - wangzhiguo9261的博客 - CSDN博客


2018年04月13日 15:06:31[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：95个人分类：[js](https://blog.csdn.net/wangzhiguo9261/article/category/7508784)


我们知道在javascript中定义全局变量有2种方式，本质上是等价的，都是向window对象注入属性或者方法。
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
// global.js
varg_name ="aty";
window.g_age = 25;
当global.js加载的时候，浏览器的全局对象window就会多出2个属性:g_name和g_age。
我们编写一个js工具类或者是js框架，通常有2种方式：
方式1：dateUtil.js
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
(function(window) {
varDateUtils = {};
DateUtils.toString =function(){
alert("toString");
};
// 全局变量
window.DateUtils = DateUtils;
})(window);
这种方式是最常用的，比如JQuery、Underscore等框架都是采用这种结构编写的。
方式2：stringUtil.js
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
// 全局变量
varStringUtils = {};

StringUtils.toUpperCase =function(input){
alert("toUpperCase");
}

很显然stringUtil.js和dateUtil.js都不符合AMD规范，现在我们看看如何通过requireJS进行加载。工程目录结构如下：**[plain]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
index.html
main.js
libs
--dateUtil.js
--stringUtil.js

index.html和main.js的实现代码如下：
**[html]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
<!doctype html>
<html>
<head>
<title>shim</title>
<metacharset="utf-8">
<scriptdata-main="main.js"src="./../requirejs-2.1.15.js"></script>
</head>
<body>
<divid="div1"style="width:200px;height:200px;background-color:\#465ae0;"></div>
</body>
</html>

**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
requirejs.config({
baseUrl:'libs'
});

require(["dateUtil","stringUtil"],function(dateUtil,stringUtil) {
alert(dateUtil===undefined);//true
});

运行index.html，通过F12观察:![](https://img-blog.csdn.net/20150312194932158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWl0YW5neW9uZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
很明显dateUtil.js和stringUtil.js能够被requireJS正常加载，但是不能获取到这2个模块的返回值。我们修改下index.html，给div注册事件处理函数，并在事件处理函数中调用stringUtil.js提供的方法:
**[html]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
<!doctype html>
<html>
<head>
<title>shim</title>
<metacharset="utf-8">
<scriptdata-main="main.js"src="./../requirejs-2.1.15.js"></script>
<script>
function test()
{
StringUtils.toUpperCase();
}
</script>
</head>
<body>
<divid="div1"style="width:200px;height:200px;background-color:\#465ae0;"onclick="test();"></div>
</body>
</html>
点击div1，可以发现test()函数不会报错。也就是说，requireJS加载不符合AMD规范的js文件，跟我们直接在html通过<script>标签加载，没有太大的差别。js文件中引入的全局变量，依然会存在，依然能够正常使用。
下面我们看下shim参数的使用方式，我们将main.js修改如下:
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
requirejs.config({
baseUrl:'libs',
shim:{
dateUtil:{
deps:[],
exports:'DateUtils'
},
stringUtil:{
deps:[],
exports:'StringUtils'
}
}
});

require(["dateUtil","stringUtil"],function(dateUtil,stringUtil) {
stringUtil.toUpperCase();
dateUtil.toString();
});
这段代码可以正常运行，可以看到：shim参数能够帮助我们以AMD模块的方式，使用那些不符合AMD规范的模块。下面接介绍下：deps和exports的含义。exports很好理解，就是模块的返回值。main.js中exports的值，一定要与dateUtil.js和stringUtil.js中暴露出的全局变量名称一致。很显然dateUtil.js和stringUtil.js这2个模块的返回值，就是暴露出的全局变量window.DateUtils和window.StringUtils，requireJS框架就是将这些全局变量的值返回，作为模块的返回结果。如果dateUtil.js或stringUtil.js中暴露了多个全局变量，那么exports可以指定其中任何的一个，作为模块的返回结果。不过一般的框架，都只会使用1个全局变量，这样冲突的可能性会减少，毕竟全局变量越少越好。
上面我们编写的dateUtil.js和stringUtil.js，都不依赖于其他js模块，所以指定的deps是空数组。下面我们编写的aplugin.js和bplugin.js都依赖于模块util.js。
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
//aplugin.js
(function(window,util) {
vara = {};
a.toString =function(){
alert("a="+util.add(1,2));
};
// 全局变量
window.a = a;
})(window,util);
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
//bplugin.js
varb = {};
b.toString =function(){
alert("b="+util.add(1,2));
}
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
//util.js
varutil = {};
util.add =function(v1,v2){
returnv1+v2;
};
main.js代码如下，只有设置正确的依赖顺序，使用的时候才不会出问题。
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
requirejs.config({
baseUrl:'libs',
shim:{
dateUtil:{
deps:[],
exports:'DateUtils'
},
stringUtil:{
deps:[],
exports:'StringUtils'
},
aplugin:{
deps:["util"],
exports:'a'
},
bplugin:{
deps:["util"],
exports:'b'
}
}
});

require(["stringUtil","dateUtil","aplugin","bplugin"],function(string,date) {
//string.toString();
//date.toString();
varaPl = require("aplugin");
varbPl = require("bplugin");
aPl.toString();
bPl.toString();
});
很显然util.js也不符合AMD规范，如果A模块依赖于B模块，A模块不符合AMD规范(使用的是全局变量)，那么B模块也必须是使用全局变量，否则会报错。即如果将util.js改成符合AMD规范的写法，那么aplugin.js和bplugin.js都会因找不到util对象而报错。
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
// 符合AMD规范的util.js
define(function(){
functionadd(v1,v2)
{
returnv1+v2;
}
return{"add":add};
});

最后我们看下shim配置参数中init的作用。init可以指定一个函数主要就是用来避免类库之间的冲突。由于不符合AMD规范的js文件，会使用全局变量。所以当加载多个模块的时候存在名字冲突的可能。比如JQuery、UnderScore等框架都会提供一个noConflict()函数来避免名字冲突，noConflict()的实现原理可以[参考这篇文章](http://blog.csdn.net/aitangyong/article/details/44200751)。我们编写一个不符合AMD规范的模块conflict.js，使用了全局变量$E，并提供noConflict方法。
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
(function(window) {
// 保存之前数据
var_$E = window.$E;
varmyplugin = {"name":"aty"};
myplugin.noConflict =function(){
window.$E = _$E;
returnmyplugin;
};
// 向全局对象注册$E
window.$E = myplugin;
})(window);

将index.html修改如下，在requireJS加载之前，先定义一个全局变量$E。
**[html]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
<!doctype html>
<html>
<head>
<title>shim</title>
<metacharset="utf-8">
<script>
var $E="before";
</script>
<scriptdata-main="main.js"src="./../requirejs-2.1.15.js"></script>
</head>
<body>
<divid="div1"style="width:200px;height:200px;background-color:\#465ae0;"onclick="test();"></div>
</body>
</html>

main.js中代码如下：
**[javascript]**[view plain](https://blog.csdn.net/aitangyong/article/details/44225859#)[copy](https://blog.csdn.net/aitangyong/article/details/44225859#)
requirejs.config({
baseUrl:'libs',
shim:{
conflict:{
deps:[],
exports:'$E',
init:function(){
return$E.noConflict();
}
}
}
});

require(["conflict"],function(mayConflict) {
alert(mayConflict.name);
alert(window.$E);//before
});
运行index.html，可以发现conflict.js能够与之前定义的全局变量$E共存，避免了冲突，这就是通过init实现的。如果没有定义init，可以看到alert(window.$E)打印的值是undefined。

版权声明：本文为博主原创文章，未经博主允许不得转载。            https://blog.csdn.net/aitangyong/article/details/44225859


