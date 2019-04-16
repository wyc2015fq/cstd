# dojo.provide：你到底“提供”了些什么？ - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月07日 13:15:00[慢游](https://me.csdn.net/eengel)阅读数：6174








dojo.provide/require机制作为JavaScript模块化管理工具，在用Dojo进行富客户端开发中确实起到了关键性的作用。具体可参考"[Understanding dojo.require](http://www.sitepen.com/blog/2010/03/29/understanding-dojo-require/)
"和"[Understanding dojo.declare, dojo.require, and dojo.provide](http://dojocampus.org/content/2008/06/03/understanding-dojodeclare-dojorequire-and-dojoprovide/)
"两文。

当在一个JavaScript文件的最开始声明

dojo.provide("my.c");

后，相当于通知了dojo两件事情：

1. 这儿有一个JS模块"my.c"，文件名叫做c.js，处在路径my下（关于路径的设置，见"[Understanding dojo.require](http://www.sitepen.com/blog/2010/03/29/understanding-dojo-require/)
"）；

2. 这儿创建了一个叫my.c的JavaScript object。

那么接下来就可以在c.js中名正言顺地干点事情了。既然my.c是一个object，那就可以包含自己的变量及函数。

非常自然地，我们会直接在c.js中定义如下变量：

var c1 = 1;

在前一篇"[Dojo对不同浏览器(IE, Firefox)下JavaScript变量作用范围的影响](http://blog.csdn.net/eengel/archive/2011/01/06/6121104.aspx)
"中，我们可以看到虽然在c.js中定义了变量c1，但在IE中c1既不能作为全局变量被访问，也和object my.c没有什么关系（也就是不能通过my.c.c1访问）。和我们设想的用dojo.provide进行模块化编程不一致。

由此可见，dojo.provide("my.c")有点像my.c = {}。虽然为我们生成了object my.c，但实际上并没有将所在的JS文件变成可直接定义变量的object内部环境（就如{}内部）。因此要在该JS文件中定义my.c的变量或函数，还得每次引用它。



下面提供几种定义变量的方法供参考。

1. my.c.c0 = 1;
my.c.c1 = 1;

2. my.c = {
		c0: 1,
		c1: 1
};

3.dojo.mixin(my.c, {
	c2: 1
});

4.dojo.mixin(my.c, new function(){
	this.c3 = 1;
}());







