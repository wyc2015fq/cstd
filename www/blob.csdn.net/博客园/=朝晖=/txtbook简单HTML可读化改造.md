# txtbook简单HTML可读化改造 - =朝晖= - 博客园
# [txtbook简单HTML可读化改造](https://www.cnblogs.com/dhcn/p/7115234.html)
       一般来讲下载的txtbook在notepad或者之类的文本编辑器下的阅读体验不是很好，PC上面专门装个txt阅读器好像有点杀鸡用牛刀，可以用HTML简单处理一下就可以放在浏览器下爽快的阅读了，这个操作主要用两个东西:一个pre元素，这个元素封装的文本可以保留txt的空格和换行，这下就爽了，那么长的文本书不用在里面做格式化处理了，剩下的居中、等排版处理我用了Bootstrap2版本中的[Container](http://lib.csdn.net/base/docker)元素，直接用一个class为container的div把pre元素包裹了，读起来就基本Ok了，剩下的再自定制处理可以在css link元素后面再对container元素做一下细调就Ok了。文本中的颜色方案来自于豆瓣阅读
来个简单模板吧：
```
<html>  
<head>  
<link href="http://cdn.bootcss.com/bootstrap/3.3.6/css/bootstrap.min.css" rel="stylesheet">  
<style>  
body{  
background: #e5e4db;   
}  
.container{  
width:80%;  
}  
pre{  
line-height: normal;  
font-family: "Helvetica Neue", Helvetica, "Lucida Grande", "Luxi Sans", Arial, "Hiragino Sans GB", STHeiti, "Microsoft YaHei", "Wenquanyi Micro Hei", "WenQuanYi Micro Hei Mono", "WenQuanYi Zen Hei", "WenQuanYi Zen Hei Mono", LiGothicMed;  
font-size:16px;  
background-color:#f6f4ec;  
white-space: pre-wrap;  
word-wrap: break-word;  
}  
</style>  
</head>  
<body>  
<div class="container">  
<pre >  
这个地方把txtbook文字Copy粘帖进来就行  
</pre>  
</div>  
</body>  
</html>
```
估计有人会说这个从技术上有点杀机用牛刀，不过从代码上讲.link一个成熟的东西要比自己写简单多了。

