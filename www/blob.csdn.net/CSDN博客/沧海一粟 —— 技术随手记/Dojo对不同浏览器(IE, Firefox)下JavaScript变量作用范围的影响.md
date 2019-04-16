# Dojo对不同浏览器(IE, Firefox)下JavaScript变量作用范围的影响 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月06日 20:08:00[慢游](https://me.csdn.net/eengel)阅读数：2448标签：[javascript																[firefox																[dojo																[ie																[浏览器																[firebug](https://so.csdn.net/so/search/s.do?q=firebug&t=blog)
个人分类：[Dojo](https://blog.csdn.net/eengel/article/category/773224)





关于JavaScript变量的作用范围，前面已经有很多文章谈论到。比如"[JavaScript tutorial: Variable scope and the var keyword](http://www.mredkj.com/tutorials/reference_js_intro_ex.html)

"区分了var在不同上下文中对变量作用范围的影响。

然而采用独立JS文件并引入dojo.provide/require机制后，情况就变得比较有趣了。

测试的server-side文件结构如下：

![](http://hi.csdn.net/attachment/201101/6/0_12943152296A8G.gif)

其中，a.js为未使用dojo.provide声明的文件，在test.html中通过script-src调用。
var a1 = 1;
a2 = 1;


c.js使用dojo.provide声明，在test.html中通过dojo.require调用。
dojo.provide("my.c");
var c1 = 1;
c2 = 1;


test.html具体如下：
<html>
<head>
<script type="text/javascript" src="/site_media/js/dojoroot/dojo/dojo.js.uncompressed.js""></script>



<script type="text/javascript" src="/site_media/js/my/a.js" ></script>



<script type="text/javascript">



	dojo.registerModulePath("my", "/site_media/js/my");



	dojo.require("my.c");



</script>



</head>



<body>



<script type="text/javascript">



	a1 += 1;



	a2 += 1;



	c2 += 1;



	c1 += 1;



</script>



</body>



</html>



1. Firefox



运行test.html后，在firebug console可以看到a1, a2, c1, c2的值都变成2了。符合变量作用范围的一般规则。



2. IE



运行test.html后，报错找不到c1。其他变量都变成了2。



由此看来，在dojo.provide声明的JS文件中，用var定义的变量在IE中并不被看作是全局变量；而在Firefox中却可以作为全局变量。](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=dojo&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)




