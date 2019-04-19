# javascript的几种运行函数写法 - 左直拳的马桶_日用桶 - CSDN博客
2018年08月22日 19:31:24[左直拳](https://me.csdn.net/leftfist)阅读数：116
个人分类：[JavaScript																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94500)
有意思，看代码：
1、写法一
```
var j1 = (function (){
	return {name:'zhang3'};
})();
```
2、写法二
```
var j2 = function (){
	return {name:'li4'};
}();
//注意直接这样写不会运行：
function (){
	return {name:'li4'};
}();
//所以一定要在赋值语句才会运行，但写法一即使没有赋值，单单右边也可以运行！
//但是在外层加个括号又可以，我快崩溃了！真是又爱又恨！
(function (){
	return {name:'li4'};
}());
```
3、写法三
```
var j3 = new function (){
	return {name:'wang5'};
};
alert(j1.name + "," + j2.name + "," + j3.name);//显示zhang3,li4,wang5
```
可见
```
1、(function(){})()
2、(function(){}())
3、new function(){}
```
基本是等价的，都是运行一个函数。可能new的功能更强一些，除了运行函数，还获得了这个函数实例。
