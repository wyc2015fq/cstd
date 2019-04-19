# Web轻量级框架UIKit简介 - Big Smile - CSDN博客
2016年12月17日 01:06:46[王啸tr1912](https://me.csdn.net/tr1912)阅读数：6443标签：[框架																[uikit																[web](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=uikit&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
个人分类：[B/S																[HTML																[前端框架](https://blog.csdn.net/tr1912/article/category/6604376)](https://blog.csdn.net/tr1912/article/category/6162867)](https://blog.csdn.net/tr1912/article/category/6184179)
        之前从网上查找easyui的资料的时候，无意间在百度的右侧推荐里面看到了UIkit的介绍，作为和easyui一样的前端框架，相信他也有很多的不俗的地方，因此就好奇的看了看他的demo和应用场景，结果还真的是让我大吃一惊，因为里面的额效果真的是让人眼前一亮，效果真的是不简单。然而到网上搜索教程竟然不是很多，经过多次尝试终于有了一些眉目，特地分享给大家。
# 一、初识
![](https://img-blog.csdn.net/20161217002252040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        UIKit的中文官网奉上：[http://www.getuikit.net/](http://www.getuikit.net/)
        对于我们来说，前端的框架也是不陌生了，接触过了Esayui，MUI和bootstrap的同学一定对于框架有了很多的了解，我们的应用也是无非在于把DEMO中的代码copy过来在加上自己的修改就成了自己的东西了，关键就是要有自己的想法，因为框架提供的东西都是一些组件，我们需要发挥想象力把他们用起来才能成为一个产品。
        好了废话不多说了，来进入正题。
        说道UIkit，相信很多同学都没有听说过，因为他不是特别的出名，用到的地方也不是很多，而且他是仿照着bootstrap的基本结构来的，所以很少有人可以注意到它，但是他又有很多bootstrap也不是很擅长的东西，首先它对于触控操作很是在行，其次，他的界面要比bootstrap要华丽很多，符合一些现代元素的特征非常适合有探索精神和追求页面华丽的同志们，所以我当初决定要研究这个框架也是有理由的。
        其次他的轻量性和可定义性也是我们会选择他的一大理由。相信用过easyui和mui的同志们都有所体会，那就是一旦需要对于框架封装好的东西进行修改，那简直是一场噩梦，光是翻阅css和js就需要废好大的功夫。但是UIkit就提供了一个官方的在线的工具，可以直接定义你自己想要的核心css文件，里面包括几乎所有常用控件的修改，这也是很值得一些老牌框架学习的地方。
# 二、实战
        说起来，关于用UIkit这方面还真的是有些惭愧，因为光是搭这个基础的引用就耗费了我一天的时间，因为他的引用和自带的工具有些特殊的原因，也是因为网络上这个框架的资料太少的原因，所以才会有这样的情况发生（唯一的教学视频在B站上，还都是全英文的）所以初期的创造之路并不简单。但是经过多方查找还是找出了一些门道。
        首先来说说UIkit的核心文档，其实他的核心文档就分为三个部分（体现了轻量）其中css是全部的额css文件，JS就是包含了全部的js文件，其中里面又分为component和一般的css，js，一般的css和js都是核心
![](https://img-blog.csdn.net/20161217003242882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
的文件一般情况下需要全部引用，像uikit.css，uikit.js等等，其中JS下面又有一个core的文件夹，里面都是关于核心的组件功能扩展引用。具体意义看下面：
```java
/css
    <!-- UIKit的基本样式 -->
    uikit.css
    uikit.min.css
    <!-- UIKit的渐变样式 -->
    uikit.gradient.css
    uikit.gradient.min.css
    <!-- UIKit的扁平化样式 -->
    uikit.almost-flat.css
    uikit.almost-flat.min.css
    <!-- 高级组件 -->
    /components
/fonts
    <!-- FontAwesome网络字体 -->
    fontawesome-webfont.eot
    fontawesome-webfont.ttf
    fontawesome-webfont.woff
    FontAwesome.otf
/js
    <!-- JavaScript和压缩后的版本 -->
    uikit.js
    uikit.min.js
    <!-- 高级组件 -->
    /components
    <!-- 核心组件 -->
    /core
```
        然后有的朋友就会说了，有了这些组件，那例子在哪里呢，其实这个框架和其他的框架的不同之处就在于页面的构件上了，由于它的控件样式可变性，所以他的例子就没有明确的给出，只能在官方的网站寻得：
![](https://img-blog.csdn.net/20161217004300642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
   如图就是uikit的官方的帮助教程所在网页了，是不是看起来很漂亮呢，那就赶紧点进去学习吧。
        上面我们讲到了核心的组件就是一个js，一个css，所以我们在应用的时候只要把这两个引用上就可以解决大部分的问题了。（注意UIkit是需要jqurey支持的，而且需要2.0以上的版本，也正是如此，它不支持IE8以下的浏览器，这也是不受欢迎的原因之一）。
   如下所示，我们需要先引用这些东西在我们的html页面中：
```
<link rel="stylesheet" type="text/css" href="css/uikit.css">
<script src="http://libs.baidu.com/jquery/2.1.4/jquery.min.js"></script>		
<script type="text/javascript" src="js/uikit.min.js"></script>
```
值得注意的是，我们需要把jquery引用在uikit.js之前，如果相反的话，uikit就会出现uikit need jquery的错误，影响页面的显示。
然后我们就可以根据教程中核心组件中的教程来写控件了，下面是个例子：
```
<body>
   <div class="uk-container uk-container-center uk-margin-top">
	<div class="uk-panel uk-panel-box uk-text-center">
	    <h1>Hello Universe</h1>
	    <p>Lorgen ipsum dolor sit amet,consecletur elit,in antus libero debitis?</p>
	    <a href="" class="uk-button uk-button-primary uk-button-large">Buy the Universe Now</a>
	</div>
   </div>
</body>
```
这个里面用到了一个框和一个按钮加标题字体的显示设置，整个页面的代码如下：
```
<!DOCTYPE HTML>
<html>
	<head>
		<title>UIkit-Demo2</title>
		<meta charset="UTF-8">
		<link rel="stylesheet" type="text/css" href="css/uikit.gradient.min.css">
		<link rel="stylesheet" type="text/css" href="css/uikit.css">
		<script src="http://libs.baidu.com/jquery/2.1.4/jquery.min.js"></script>		
		<script type="text/javascript" src="js/uikit.min.js"></script>
	</head>
	<body>
		<div class="uk-container uk-container-center uk-margin-top">
			<div class="uk-panel uk-panel-box uk-text-center">
				<h1>Hello Universe</h1>
				<p>Lorgen ipsum dolor sit amet,consecletur elit,in antus libero debitis?</p>
				<a href="" class="uk-button uk-button-primary uk-button-large">Buy the Universe Now</a>
			</div>
		</div>
	</body>
</html>
```
仅供参考，另外如果想使用附加组件中的功能的话，需要注意里面所引用的高级样式和JS文件，才可以应用。
下面来看看可定制的工具，其实都是核心组件中的东西，只是你可以自己定制样式罢了：
![](https://img-blog.csdn.net/20161217010214149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    左面是有三个主题可以选择，右面是所有提供的控件，可以选中之后在左边的框中进行属性的修改。修改完成之后在左面可以下载相应的css文件中替换之前核心组件中的uikit.css为你下载的自定义css即可，也可以下载less文件（虽然不知道怎么用）进行整个主题的保存。
     以上就是关于UIkit的简短的介绍，希望对于您来说能有小小的帮助。

