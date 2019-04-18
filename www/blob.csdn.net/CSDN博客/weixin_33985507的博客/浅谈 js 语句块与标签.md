# 浅谈 js 语句块与标签 - weixin_33985507的博客 - CSDN博客
2014年05月28日 23:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
语句块是什么？
其实就是用 {} 包裹的一些js代码而已，当然语句块不能独立作用域。
可以详细参见这里《[MDN block](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/block)》 
也许很多人第一印象 {} 不是对象字面量么？怎么成了语句块了？
如果在赋值语句或者表达式里用的时候，确实是对象字面量，如：
```java
var a = {};
({toString:function(){return "hehe"}}) + "...";
```
![](https://images0.cnblogs.com/i/477954/201405/282333443538548.jpg)
是不是很有意思。。
但是直接使用如：
```java
{toString: function(){return "hehe"}}
```
却不能正常执行
![](https://images0.cnblogs.com/i/477954/201405/282334071031131.jpg)
其实这里的 {} 只是起到语句块的作用，而不是对象字面量。
怎么回事呢，明明就是对象啊。。
其实这里要说到标签了。
我简单描述下标签是什么，如果想详细了解，请移步 《[MSN label](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/label)》 
```java
我是标签: var a = 1;
```
![](https://images0.cnblogs.com/i/477954/201405/282334407137564.jpg)
一切正常，从 MSN label 里可以看到，标签往往是结合  `continue, break ` 一起使用的。
知道标签是什么了，就可以回到刚才的问题上了。
`{toString: function(){return "hehe"}}` 其实就是
```java
{ // 语句块
    标签: function(){return "hehe"}
}
```
由于  `function(){return "hehe"}`  既不是函数声明，也不是函数表达式，所以就报错了。
我们只要简单修改下，修改成函数声明或者函数表达式即可。
```java
{ // 语句块
    标签1: function test1(){return "hehe"};
    标签2: var test2 = function(){return "hehe"};
}
```
![](https://images0.cnblogs.com/i/477954/201405/282335572756906.jpg)
可以看到一切正常，这个就是语句块和标签
它可以构成非常像对象字面量的语句，但其实它仍然是 语句块 + 标签 而已。
还记不记得你用 eval 解析 json 的时候，不加 () 就报错么。
其实现在非常容易解释了，因为当直接执行 {} 的时候，只把内部代码当作语句块执行，而不是对象字面量。
加上 () 就是让它当作表达式执行，所以才能正常解析成对象字面量。
这也是上面这个  `({toString:function(){return "hehe"}}) + "...";`  必须加 () 的原因。
其实你完全可以这样写你的代码：
```java
程序猿A写的功能块: {
    // 功能块代码...
}
程序猿B写的功能块: {
    // 功能块代码...
}
```
这样是不是很清晰？
但是不推荐这样写，因为语句块起不到独立作用域的功能，所以很容易全局污染。
依然推荐
```java
(function () {
    // 功能块代码...
})();
```
自调用函数，不仅可以独立作用域，还可以在 UglifyJS，Closure Compressor 等工具编译的时候更加优化。
好了，我们来个小小的测试吧。
1. 执行  `a: b: c: d: e: 1, 2, 3, 4, 5`  结果是什么？ 为什么？ 那  `a:{b:{c:1}}`  呢？
2. 执行  `1 + {valueOf:function(){return 1}}`  结果是什么？ 为什么？
好了，今天的分享就这些了，如有不对之处，请跟帖指出，小生先谢过了。
![](https://images0.cnblogs.com/i/477954/201405/282340451662822.jpg)
