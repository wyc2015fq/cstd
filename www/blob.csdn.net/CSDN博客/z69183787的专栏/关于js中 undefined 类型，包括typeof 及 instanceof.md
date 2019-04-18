# 关于js中 undefined 类型，包括typeof 及 instanceof - z69183787的专栏 - CSDN博客
2012年10月22日 09:53:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1806
```java
var a;
if(typeof(a)=="undefined"){
	alert(1);
}
if(a==null){
	alert(2);
}
```
typeof 是一个一元运算，放在一个运算数之前，运算数可以是任意类型。
它返回值是一个字符串，该字符串说明运算数的类型。typeof 一般只能返回如下几个结果：
number,boolean,string,function,object,undefined。我们可以使用 typeof 来获取一个变量是否存在，如 if(typeof a!="undefined"){alert("ok")}，而不要去使用 if(a) 因为如果 a 不存在（未声明）则会出错，对于 Array,Null 等特殊对象使用 typeof 一律返回 object，这正是 typeof 的局限性。
instanceof 用于判断一个变量是否某个对象的实例，如 var a=new Array();alert(a instanceof Array); 会返回 true，同时 alert(a instanceof Object) 也会返回 true;这是因为 Array 是 object 的子类。再如：function test(){};var a=new test();alert(a instanceof test) 会返回
谈到 instanceof 我们要多插入一个问题，就是 function 的 arguments，我们大家也许都认为 arguments 是一个 Array，但如果使用 instaceof 去测试会发现 arguments 不是一个 Array 对象，尽管看起来很像。
另外：
测试 var a=new Array();if (a instanceof Object) alert('Y');else alert('N');
得'Y’
但 if (window instanceof Object) alert('Y');else alert('N');
得'N'
所以，这里的 instanceof 测试的 object 是指 js 语法中的 object，不是指 dom 模型对象。
使用 typeof 会有些区别
alert(typeof(window)) 会得 object
