# 【HTML＋CSS网页设计与布局 从入门到精通】第6章-标题h1,h1字体格式的设置方式 - Koma Hub - CSDN博客
2019年01月06日 20:36:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：119
**目录**
[方式1](#%E6%96%B9%E5%BC%8F1)
[方式2](#%E6%96%B9%E5%BC%8F2)
# 方式1
```
<html>
<head>
	<title>演示</title>
	<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
</head>
<body>
	<h2><font color="#0000FF" face="幼圆">这是标题文本</font></h2>
	<p>这里是正文内容</p>
	<h2><font color="#0000FF" face="幼圆">这是标题文本</font></h2>
	<p>这里是正文内容</p>
	<h2><font color="#0000FF" face="幼圆">这是标题文本</font></h2>
	<p>这里是正文内容</p>
</body>
</html>
```
# 方式2
```
<html>
<head>
	<title>演示</title>
	<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<style>
h2{
	font-family:幼圆;
	color:blue;
}
</style>
</head>
<body>
	<h2>这是标题文本</h2>
	<p>这里是正文内容</p>
	<h2>这是标题文本</h2>
	<p>这里是正文内容</p>
	<h2>这是标题文</h2>
	<p>这里是正文内容</p>
</body>
</html>
```
```
<html>
<head>
	<title>演示</title>
	<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<style>
h2{
	font-family:黑体;
	color:red;
}
</style>
</head>
<body>
	<h2>这是标题文本</h2>
	<p>这里是正文内容</p>
	<h2>这是标题文本</h2>
	<p>这里是正文内容</p>
	<h2>这是标题文</h2>
	<p>这里是正文内容</p>
</body>
</html>
```
