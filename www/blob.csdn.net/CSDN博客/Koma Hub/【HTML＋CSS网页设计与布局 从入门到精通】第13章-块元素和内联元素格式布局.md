# 【HTML＋CSS网页设计与布局 从入门到精通】第13章-块元素和内联元素格式布局 - Koma Hub - CSDN博客
2019年01月07日 22:23:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：51
个人分类：[CSS																[HTML](https://blog.csdn.net/Rong_Toa/article/category/7651718)](https://blog.csdn.net/Rong_Toa/article/category/8590132)
**目录**
[块元素背景，边框，颜色等布局](#%E5%9D%97%E5%85%83%E7%B4%A0%E8%83%8C%E6%99%AF%EF%BC%8C%E8%BE%B9%E6%A1%86%EF%BC%8C%E9%A2%9C%E8%89%B2%E7%AD%89%E5%B8%83%E5%B1%80)
[块元素ID选择器](#%E5%9D%97%E5%85%83%E7%B4%A0ID%E9%80%89%E6%8B%A9%E5%99%A8)
[另一个例子](#%E5%8F%A6%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90)
[块元素中的图片](#%E5%9D%97%E5%85%83%E7%B4%A0%E4%B8%AD%E7%9A%84%E5%9B%BE%E7%89%87)
[盒子模型的演示](#%E7%9B%92%E5%AD%90%E6%A8%A1%E5%9E%8B%E7%9A%84%E6%BC%94%E7%A4%BA)
[块级元素中的文本格式](#%E5%9D%97%E7%BA%A7%E5%85%83%E7%B4%A0%E4%B8%AD%E7%9A%84%E6%96%87%E6%9C%AC%E6%A0%BC%E5%BC%8F)
[块级元素div与内联元素span的对比](#%E5%9D%97%E7%BA%A7%E5%85%83%E7%B4%A0div%E4%B8%8E%E5%86%85%E8%81%94%E5%85%83%E7%B4%A0span%E7%9A%84%E5%AF%B9%E6%AF%94)
[两个行内元素的margin](#%E4%B8%A4%E4%B8%AA%E8%A1%8C%E5%86%85%E5%85%83%E7%B4%A0%E7%9A%84margin)
[两个块级元素的margin](#%E4%B8%A4%E4%B8%AA%E5%9D%97%E7%BA%A7%E5%85%83%E7%B4%A0%E7%9A%84margin)
[子div](#%E5%AD%90div)
[span的margin设置为负数](#span%E7%9A%84margin%E8%AE%BE%E7%BD%AE%E4%B8%BA%E8%B4%9F%E6%95%B0)
[盒子模型的演示](#%E7%9B%92%E5%AD%90%E6%A8%A1%E5%9E%8B%E7%9A%84%E6%BC%94%E7%A4%BA)
# 块元素背景，边框，颜色等布局
```
<html>
<head>
<title>border-style</title>
<style type="text/css">
div{
	border-width:6px;
	border-color:#000000;
	margin:20px; padding:5px;
	background-color:#FFFFCC;
}
</style>
</head>
<body>
	<div style="border-style:dashed">The border-style of dashed.</div>	
	<div style="border-style:dotted">The border-style of dotted.</div>
	<div style="border-style:double">The border-style of double.</div>
	<div style="border-style:groove">The border-style of groove.</div>
	<div style="border-style:inset">The border-style of inset.</div>
	<div style="border-style:outset">The border-style of outset.</div>
	<div style="border-style:ridge">The border-style of ridge.</div>
	<div style="border-style:solid">The border-style of solid.</div>
</body>
</html>
```
# 块元素ID选择器
```
<html>
<head>
<style type="text/css">
#outerBox{
	width:200px;
	height:100px;
	border:2px black solid;
	border-left:4px green dashed;
	border-color:red gray orange blue;  /*上  右  下  左*/
	border-right-color:purple; 
}
</style>
</head>
<body>
	<div id="outerBox">
	</div>
</body>
```
## 另一个例子
```
<html>
<head>
<style type="text/css">
#outerBox{
	width:128px;
	height:128px;
	border:10px black dashed;
	background:silver;
}
</style>
</head>
<body>
	<div id="outerBox">
	</div>
</body>
```
# 块元素中的图片
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>演示</title>
<style type="text/css">
#box{
	width:128px;
	height:128px;
	padding:0 20px 10px;    /*上     左右     下*/
	padding-left:10px;
	border:10px gray dashed;
}
#box img{
border:1px blue solid;
} 
</style>
</head>
<body>
	<div id="box"><img src="cup.gif"></img></div>
</body>
</html>
```
# 盒子模型的演示
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>盒子模型的演示</title>
    <style type="text/css">
	body{
	margin:0 0 0 0;
	font-family:宋体;
	font-size:12px;
	
	}
      ul { 
        background: #ddd; 
        margin: 15px 15px 15px 15px;
        padding: 5px 5px 5px 5px;
                                     /* 没有设置边框 */
      }
      li { 
        color: black;                /* 黑色文本 */ 
        background: #aaa;            /* 浅灰色背景 */
        margin: 20px 20px 20px 20px; /* 左侧外边距为0，其余为20像素*/
        padding: 10px 0px 10px 10px; /* 右侧内边距为0，其余10像素 */
        list-style: none             /* 取消项目符号 */
                                     /* 未设置边框 */
      }
      li.withborder {
        border-style: dashed;
        border-width: 5px;       	/* 设置边框为2像素 */
        border-color: black;
        margin-top:20px； 
      }
    </style>
  </head>
  <body>
    <ul>
      <li>第1个列表的第1个项目内容</li>      
      <li class="withborder">第1个列表的第2个项目内容，内容更长一些，目的是演示自动折行的效果。</li>
    </ul>
    <ul>
      <li>第2个列表的第1个项目内容</li>
      <li class="withborder">第2个列表的第2个项目内容，内容更长一些，目的是演示自动折行的效果。</li>
    </ul>
  </body>
</html>
```
# 块级元素中的文本格式
```
<html>
<head>
<title>div 标记范例</title>
<style type="text/css">
div{
	font-size:18px;						/* 字号大小 */
	font-weight:bold;					/* 字体粗细 */
	font-family:Arial;					/* 字体 */
	color:#FFFF00;						/* 颜色 */
	background-color:#0000FF;			/* 背景颜色 */
	text-align:center;					/* 对齐方式 */
	width:300px;						/* 块宽度 */
	height:100px;						/* 块高度 */
}
</style>
</head>
<body>
	<div>
	这是一个div标记
	</div>
</body>
</html>
```
# 块级元素div与内联元素span的对比
```
<html>
<head>
<title>div与span的区别</title>
</head>
<body>
	<p>div标记不同行：</p>
	<div><img src="cup.gif" border="0"></div>
	<div><img src="cup.gif" border="0"></div>
	<div><img src="cup.gif" border="0"></div>
	<p>span标记同一行：</p>
	<span><img src="cup.gif" border="0"></span>
	<span><img src="cup.gif" border="0"></span>
	<span><img src="cup.gif" border="0"></span>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190107221920227.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 两个行内元素的margin
```
<html>
<head>
<title>两个行内元素的margin</title>
<style type="text/css">
span{
	background-color:#a2d2ff;
	text-align:center;
	font-family:Arial, Helvetica, sans-serif;
	font-size:12px;
	padding:10px;
}
span.left{
	margin-right:30px;
	background-color:#a9d6ff;
}
span.right{
	margin-left:40px;
	background-color:#eeb0b0;
}
</style>
</head>
<body>
	<span class="left">行内元素1</span><span class="right">行内元素2</span>
</body>
</html>
```
# 两个块级元素的margin
```
<html>
<head>
<title>两个块级元素的margin</title>
<style type="text/css">
<!--
div{
	background-color:#a2d2ff;
	text-align:center;
	font-family:Arial, Helvetica, sans-serif;
	font-size:12px;
	padding:10px;
}
-->
</style>
</head>
<body>
	<div style="margin-bottom:50px;">块元素1</div>
	<div style="margin-top:30px;">块元素2</div>
</body>
</html>
```
# 子div
```
<head>
<title>父子块的margin</title>
<style type="text/css">
<!--
div.father{						/* 父div */
	background-color:#fffebb;
	text-align:center;
	font-family:Arial, Helvetica, sans-serif;
	font-size:12px;
	padding:10px;
	border:1px solid #000000;
	height:40px;				/* 设置父div的高度 */
}
div.son{						/* 子div */
	background-color:#a2d2ff;
	margin-top:30px;
	margin-bottom:0px;
	padding:15px;
	border:1px dashed #004993;
}
-->
</style>
</head>
<body>
	<div class="father">
		<div class="son">子div</div>
	</div>
</body>
```
# span的margin设置为负数
```
<head>
<title>margin设置为负数</title>
<style type="text/css">
span{
	text-align:center;
	font-family:Arial, Helvetica, sans-serif;
	font-size:12px;
	padding:10px;
	border:1px dashed #000000;
}
span.left{
	margin-right:30px;
	background-color:#a9d6ff;
}
span.right{
	margin-left:-53px;			/* 设置为负数 */
	background-color:#eeb0b0;
}
</style>
</head>
<body>
	<span class="left">行内元素1</span><span class="right">行内元素2</span>
</body>
```
# 盒子模型的演示
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>盒子模型的演示</title>
<style type="text/css">
body{
	margin:0 0 0 0;
	font-family:宋体;
}
ul { 
    background: #ddd; 
    margin: 15px;
    padding: 10px;
	font-size:12px;
	line-height:14px;
}
h1 { 
    background: #ddd; 
    margin: 15px;
    padding: 10px;
    height:30px;
    font-size:25px;
}
p,li { 
    color: black;                /* 黑色文本 */ 
    background: #aaa;            /* 浅灰色背景 */
    margin: 20px 20px 20px 20px; /* 左侧外边距为0，其余为20像素*/
    padding: 10px 0px 10px 10px; /* 右侧内边距为0，其余10像素 */
    list-style: none             /* 取消项目符号 */
}
.withborder {
    border-style: dashed;
    border-width: 5px;       	/* 设置边框为2像素 */
    border-color: black;
    margin-top:20px; 
}
</style>
</head>
  <body>
   <h1>标准流中的盒子模型演示</h1>
    <ul>
      <li>第1个项目内容</li>      
      <li class="withborder">第2个项目内容，第2个项目内容，第2个项目内容，第2个项目内容，第2个项目内容，第2个项目内容。</li>
    </ul>
  </body>
</html>
```
