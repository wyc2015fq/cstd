# 【HTML＋CSS网页设计与布局 从入门到精通】第4章 - Koma Hub - CSDN博客
2019年01月06日 20:17:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：76
**目录**
[超链接](#%E8%B6%85%E9%93%BE%E6%8E%A5)
[h1,h2,h3...](#%3Ch1%3E%3C%2Fh1%3E)
[图片超链接](#%E5%9B%BE%E7%89%87%E8%B6%85%E9%93%BE%E6%8E%A5)
[邮件超链接](#%E9%82%AE%E4%BB%B6%E8%B6%85%E9%93%BE%E6%8E%A5)
[图像热区超链接](#%E5%9B%BE%E5%83%8F%E7%83%AD%E5%8C%BA%E8%B6%85%E9%93%BE%E6%8E%A5)
[图像热区超链接-自己作图](#%E5%9B%BE%E5%83%8F%E7%83%AD%E5%8C%BA%E8%B6%85%E9%93%BE%E6%8E%A5-%E8%87%AA%E5%B7%B1%E4%BD%9C%E5%9B%BE)
[图像热区超链接-多热区](#%E5%9B%BE%E5%83%8F%E7%83%AD%E5%8C%BA%E8%B6%85%E9%93%BE%E6%8E%A5-%E5%A4%9A%E7%83%AD%E5%8C%BA)
[插入一个框架-frame（将一个html文件当做一个frame）](#%E6%8F%92%E5%85%A5%E4%B8%80%E4%B8%AA%E6%A1%86%E6%9E%B6-frame%EF%BC%88%E5%B0%86%E4%B8%80%E4%B8%AAhtml%E6%96%87%E4%BB%B6%E5%BD%93%E5%81%9A%E4%B8%80%E4%B8%AAframe%EF%BC%89)
[插入一个框架-frame（同样可以是一个网址）](#%E6%8F%92%E5%85%A5%E4%B8%80%E4%B8%AA%E6%A1%86%E6%9E%B6-frame%EF%BC%88%E5%90%8C%E6%A0%B7%E5%8F%AF%E4%BB%A5%E6%98%AF%E4%B8%80%E4%B8%AA%E7%BD%91%E5%9D%80%EF%BC%89)
[插入一个框架-iframe（网址）](#%E6%8F%92%E5%85%A5%E4%B8%80%E4%B8%AA%E6%A1%86%E6%9E%B6-iframe%EF%BC%88%E7%BD%91%E5%9D%80%EF%BC%89)
# 超链接
```
<html>
   <head>
      <title>超链接</title>
   </head>
   <body>
      <p>点击<a href= "http://www.artech.cn/guest-book.html">链接01</a>链接到第1个网页。</p>
      <p>点击<a href= "/02.html">链接02</a>链接到第2个网页。</p>
      <p>点击<a href= "../sub/03.html">链接03</a>链接到第3个网页。</p>
   </body>
</html>
```
# h1,h2,h3...
```
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
        <title>Title</title>
    </head>
    <body>
        <h1>Web</h1>
        <h2>在这里向您推荐最好的Web设计图书</h2>
        <h2>在这里向您推荐最好的Web设计图书</h2>
        <p><a href="#first">第1本书</a> <a href="#second">第2本书</a> <a href="#third">第3本书</a></p>
        <h3><a name="first"></a>精通CSS+DIV网页样式与布局</h3>
        <p>本书系统地讲解了CSS层叠样式表的基础理论和实际运用技术</p>
        <p>详细介绍</p>
        <h3><a name="second"></a>CSS设计彻底研究</h3>
        <p>本书是一本深入研究和揭示CSS设计技术的书籍。<br>详细介绍</p>
        <h3><a name="third"></a>CSS设计禅意花园</h3>
        <p>本书作者是世界著名的网站设计师<br>详细介绍</p>
        <h4>技术趋势</h4>
        <p>今年是社会化网络年。</p>
        <p>RSS继续向主流应用渗透。</p>
        <p>Web 2.0这个词被大众认为是一个市场营销的用语。</p>
        <h4>关于本站</h4>
        <p>本站作者是一名Web设计和开发的爱好者。</p>
    </body>
</html>
```
# 图片超链接
```
<html>
   <head>
      <title>图片的超链接</title>
   </head>
   <body>
      <a href=1.html><img src=cup.gif></a><br>
   </body>
</html>
```
# 邮件超链接
```
<html>
   <head>
      <title>邮件的链接</title>
   </head>
   <body>
      联系我们：<a href="mailto:support@artech.cn">给我们发送邮件</a>。
   </body>
</html>
```
# 图像热区超链接
```
<html>
   <head>
      <title>图像热区</title>
   </head>
   <body>
		<img src="stars.jpg" border="0" usemap="#Map">
<map name="Map">
  <area shape="poly" coords="26,130,62,113,144,119,161,203,69,228,31,221,14,202,8,155" href="http://baike.baidu.com/view/6023.htm">
</map>
</body>
</html>
```
# 图像热区超链接-自己作图
```
<html>
<head>
<title>图像热区</title>
</head>
<body>
<img src="hotmap.jpg" border="0" usemap="#Map">
<map name="Map">
  <area shape="rect" coords="16,37,127,105" href="01.htm">
  <area shape="circle" coords="204,69,41" href="02.htm">
  <area shape="poly" coords="284,15,344,4,386,59,330,111,268,56" href="03.htm">
</map>
</body>
</html>
```
# 图像热区超链接-多热区
```
<html>
<head>
<title>图像热区</title>
</head>
<body style="margin:0">
<img src="navi.jpg" width="400" height="200" border="0" usemap="#Map">
<map name="Map">
  <area shape="rect" coords="303,22,377,52" href="04-08-01.htm" target="main">
  <area shape="rect" coords="242,76,321,105" href="04-08-02.htm" target="main">
  <area shape="rect" coords="141,126,216,157" href="04-08-03.htm" target="main">
  <area shape="rect" coords="15,152,92,183" href="04-08-04.htm" target="main">
</map>
</body>
</html>
```
# 插入一个框架-frame（将一个html文件当做一个frame）
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>
<frameset rows="210,*" cols="*">
  <frame src="04-07.htm">
  <frame src="04-08-01.htm" name="main" >
</frameset>
<noframes></noframes>
</html>
```
# 插入一个框架-frame（同样可以是一个网址）
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>
<frameset rows="210,*" cols="*">
  <frame src="04-07.htm">
  <frame src="https://www.baidu.com/" name="main" >
</frameset>
<noframes></noframes>
</html>
```
# 插入一个框架-iframe（网址）
```
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>
<body>
<p>这是<strong>公司介绍</strong>页面 </p>
<iframe width=700 height=200 src=http://www.baidu.com > </iframe>
</body>
</html>
```
