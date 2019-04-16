# 【前端】向blog或网站中添加语法高亮显示代码方法总结 - ggabcda的博客 - CSDN博客





2017年08月26日 13:25:23[ggabcda](https://me.csdn.net/ggabcda)阅读数：231










**向blog或网站中添加语法高亮显示的代码方法总结**



> 
**文章目录**
- 预备知识
- 目标
- 第一类方法：嵌入
- 第二类方法：外部引用
- 第三类方法：忽略HTML和PHP




　　最近在写代码时遇到一个问题，就是如何让代码像在IDE或专业编译器一样能够高亮显示在网页或博客中（如下图显示），上网查了很多资料，下面是我对学习到的方法的归纳总结。



![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825114320449-2034998364.jpg)



下面的方法基本上都是利用第三方javascript插件实现的，因此不必担心方法有多难，只要拿过来使用就可以了。在讲述方法之前先介绍一下与之条件：




**预备知识**

- HTML和CSS的基本知识


**目标**

- 在博客或网页中让嵌入代码高亮显示.
- 在Wordpress中自动忽略HTML和PHP代码.

　　方法主要有两类，一种是引入第三方的JavaScript和高亮语法插件，让pre和code标签中的代码高亮显示，另一种方法是直接嵌入主流网站的语法代码。





**第一类方法：嵌入**



　　嵌入方式是最方便快捷的，很多网站都提供了代码导出的功能，它可以自动引入专业网站的高亮代码显示方式，美观大方。



**GitHub gist**

**使用方法：**


```
<!DOCTYPE html>
<html>
<head>
    <title>github代码嵌入</title>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="./assets/css/algorithm.css">
    <style type="text/css">
       div{
           margin: 8px;
       }
    </style>
</head>
<body>
<h1>github代码嵌入</h1>
<div>
<script src="https://gist.github.com/dragonir/b3b43d791c259b597907069020f4b754.js"></script>
</div>
</body>
</html>
```




**实现效果：**

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825113730339-1315555885.jpg)

**Codepen pen**

**使用方法：**


```
<!DOCTYPE html>
<html>
<head>
    <title>codepen代码嵌入</title>
    <meta charset="utf-8">
    <link rel="stylesheet" type="text/css" href="./assets/css/algorithm.css">
</head>
<body>
<h1>Codepen代码嵌入</h1>
<pre>
    <p data-height="265" data-theme- data-slug-hash="MogbxY" data-default-tab="result" data-user="dragonir" data-embed-version="2" data-pen-title="MogbxY" class="codepen">See the Pen <a href="https://codepen.io/dragonir/pen/MogbxY/">MogbxY</a> by dragonir (<a href="https://codepen.io/dragonir">@dragonir</a>) on <a href="https://codepen.io">CodePen</a>.</p>
</pre>
<script async src="https://production-assets.codepen.io/assets/embed/ei.js"></script>
</body>
</html>
```




**实现效果：**

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825113940605-1958020814.jpg)

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825113949089-1437889371.jpg)







**第二类方法：****JavaScript****高亮插件**



　　通过引入代码高亮显示插件，同样可以实现博客以及其他网站嵌入代码的高亮显示，以下是几种主流的方法。我只展示了基本的使用方法，想详细了解使用方法和接口，可以点击链接到官网。



**Highlight.js**

**使用方法：**


```
<!DOCTYPE html>
<html>
<head>
	<title>网页嵌入代码语法高亮</title>
	<meta charset="utf-8">

	<!-- 
		<link rel="stylesheet" href="/path/to/styles/default.css"> 
		<script src="/path/to/highlight.pack.js"></script>
		<script>hljs.initHighlightingOnLoad();</script>
	-->
	
	<link rel="stylesheet" type="text/css" href="./highlight/styles/railscasts.css">
	<link rel="stylesheet" type="text/css" href="./assets/css/algorithm.css">
	<script src="./highlight/highlight.pack.js"></script>
	<script>hljs.initHighlightingOnLoad();</script>
</head>
<body>
<h1>Highlight.js</h1>
<pre>
	<code class="javascript">
		function selectionSort(arr){
			var minIndex, temp, len = arr.length;
			for(var i=0; i < len; i++){
				minIndex = i;
				for(var j=i+1; j < len; j++){
					if(arr[j] < arr[minIndex]){
						minIndex = j;
					}
				}
				temp = arr[i];
				arr[i] = arr[minIndex];
				arr[minIndex] = temp;
			}
			return arr;
		}
		var num = new Array;
		num = [1,5,2,8,4,9,3,0,4];
		console.log(selectionSort(num));
	</code>
</pre>


</body>
</html>
```




**实现效果：**

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825114228136-1712245386.jpg)



**Prism.js**. 

**使用方法：**


```
<!DOCTYPE html>
<html>
<head>
	<title>prism.js</title>
	<meta charset="utf-8">
	
	<link rel="stylesheet" type="text/css" href="./assets/css/algorithm.css">
	<link rel="stylesheet" type="text/css" href="./prism/prism.css">
	<script src="./prism/prism.js"></script>
</head>
<body>
<h1>prism.js</h1>
<pre>
	<code class="language-javascript">
		function selectionSort(arr){
			var minIndex, temp, len = arr.length;
			for(var i=0; i < len; i++){
				minIndex = i;
				for(var j=i+1; j < len; j++){
					if(arr[j] < arr[minIndex]){
						minIndex = j;
					}
				}
				temp = arr[i];
				arr[i] = arr[minIndex];
				arr[minIndex] = temp;
			}
			return arr;
		}
		var num = new Array;
		num = [1,5,2,8,4,9,3,0,4];
		console.log(selectionSort(num));
	</code>
</pre>


</body>
</html>
```




**实现效果：**

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825114247668-885706121.jpg)



**Google Prettify**

**使用方法：**


```
<!DOCTYPE html>
<html>
<head>
	<title>prettify.js</title>
	<meta charset="utf-8">

	<!-- 
	Include the script tag below in your document:
	<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
	See Getting Started to configure that URL with options you need.
	Look at the skin gallery and pick styles that suit you. 
	-->

	<!-- 
	Put code snippets in <pre class="prettyprint">...</pre> or <code class="prettyprint">...</code> and it will automatically be pretty-printed. 
	-->
	
	<link rel="stylesheet" type="text/css" href="./assets/css/algorithm.css">
	<script src="./prettify/loader/run_prettify.js"></script>
</head>
<body>
<h1>prettify.js</h1>
<pre>
	<code class="prettyprint">
		function selectionSort(arr){
			var minIndex, temp, len = arr.length;
			for(var i=0; i < len; i++){
				minIndex = i;
				for(var j=i+1; j < len; j++){
					if(arr[j] < arr[minIndex]){
						minIndex = j;
					}
				}
				temp = arr[i];
				arr[i] = arr[minIndex];
				arr[minIndex] = temp;
			}
			return arr;
		}
		var num = new Array;
		num = [1,5,2,8,4,9,3,0,4];
		console.log(selectionSort(num));
	</code>
</pre>
</body>
</html>
```




**实现效果：**

![](http://images2017.cnblogs.com/blog/772544/201708/772544-20170825114255308-1551492736.jpg)





**第三类方法：忽略****HTML****和****PHP**



　　为了显示HTML和PHP代码，浏览器必须要将显示的代码自动忽略，你可以使用在线转换工具 **Free Online HTML Escape Tool**来转换你需要显示的HTML代码。如果你是用的博客是wordPress的博客，**WordPress plugin** 可以实现此功能，但是它无法和Prism.js同时使用。





**总结**

　　现在就在你的博客或网站中嵌入好看的代码吧，如果你想了解更多有用的WordPress的功能，推荐访问**this is the resource for you.**



> 
转载请标明出处：http://www.cnblogs.com/dragonir/p/7426965.html，作者：Dragonir ，欢迎转载。




