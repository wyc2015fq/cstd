
# PHP网站使用JavaScript和Iframe简单实现部分刷新效果 - 杨秀璋的专栏 - CSDN博客

2015年02月16日 05:59:45[Eastmount](https://me.csdn.net/Eastmount)阅读数：8707所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)



本文主要是记录自己寒假作业PHP网站实现加载界面的文章，运行效果如下图所示。主要记录php+html+Apache开发网站的3个功能：(方便以后阅读和其他人学习)
1.如何实现简单页面布局
2.使用jsp如何实现隐藏与显示效果
3.通过iframe实现局部动态更新页面内容
**一.运行效果**
运行apache访问本地页面http://localhost:8080/CourseStudy/index.php，效果如下所示：([lamp/wamp配置php网站](http://blog.csdn.net/eastmount/article/details/11823549))
![](https://img-blog.csdn.net/20150216044609736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以发现该界面布局主要由3部分组成,顶部head、中间左边菜单栏和中间右边显示界面，点击左边菜单栏会通过JavaScript实现隐藏缩放功能；同时点击不同菜单栏可以在右边显示不同界面。如下图所示：
![](https://img-blog.csdn.net/20150216045633116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150216045603005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**二.实现方法介绍**
**1.界面布局**
界面布局主要采用的是include加载php文件实现，采用div和table实现布局，其中index.php文件代码如下：
```python
<?php
include("head.php");
?>
<br />
<!-- 布局中部 -->
<div id="middle">
<!-- 布局中部右边 否则总是显示在左边之下 why? -->	
<div id="index_right">
<iframe height="100%" width="100%" border="0" frameborder="0" 
	src="main_right.php" name="rightFrame" id="rightFrame" title="rightFrame">
</iframe>
</div>
<!-- 布局中部左边 -->
<div id="index_left">
<?php
	include('main_left.php');
?>
</div>
</div>
```
主要是通过head.php布局该界面的头部，main_right.php实现加载界面的中间右边部分，main_left.php加载界面的中间左边菜单栏，而<iframe>后面实现局部加载会讲述。
![](https://img-blog.csdn.net/20150216051243014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中head.php代码如下图所示，就是Html+CSS简单的布局：
```python
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>《分布式系统》精品课程学习</title>
<link  href="css/mycss.css" type="text/css" rel="stylesheet"/>
</head>
<body>
<div id="main">
<TABLE cellSpacing=0 cellPadding=0 width="100%" 
		background="images/header_bg.gif" border=0>
	<!--头部图片-->
  <TR height=80>
    <TD width=260>
    	<IMG height=80 src="images/logo.gif" width=260>
    </TD>
    <TD style="FONT-SIZE: 12px; FONT-WEIGHT: bold; COLOR: #000;
    	 PADDING-TOP: 20px; PADDING-RIGHT: 20px" align=right>
    	 您还未登录! |
      <A style="COLOR: #000" href="" 
      	target=main>登录</A>|
      <A style="COLOR: #000" href="" 
      	target=main>注册</A>|
      <A style="COLOR: #000" href=""
      	onclick="if (confirm('确定要退出吗？')) return true; else return false;" 
      	target=main>退出系统</A> 
    </TD>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
  <TR bgColor=#1c5db6 height=4>
    <TD></TD></TR>
</TABLE>
```
**2.JavaScript实现隐藏缩放功能**
main_left.php中采用table布局并调用该SCRIPT函数实现该功能，其中核心代码如下所示：
```python
<SCRIPT language=javascript>
	function expand(el)
	{
		childObj = document.getElementById("child" + el);
		if (childObj.style.display == 'none')
		{
			childObj.style.display = 'block';
		}
		else
		{
			childObj.style.display = 'none';
		}
		return;
	}
</SCRIPT>
```
其中第一个菜单调用代码如下，通过onclick=expend(1)调用，而且子菜单DISPLAY初值为NONE，则调用该函数后初值为block显示。
```python
<!-- 第一选项 -->
<TABLE cellSpacing=0 cellPadding=0 width=150 border=0>  
  <TR height=30>
    <TD style="PADDING-LEFT: 20px; FONT-SIZE: 15px" background=images/menu_bt.jpg><A 
      class=menuParent onclick=expand(1) 
      href="javascript:void(0);">课程首页</A></TD></TR>
  <TR height=4>
    <TD></TD></TR>
</TABLE>
<TABLE id=child1 style="DISPLAY: none" cellSpacing=0 cellPadding=0 
width=150 border=0>
  <TR height=20>
    <TD align=middle width=30><IMG height=9 
      src="images/menu_icon.gif" width=9></TD>
    <TD><A href="main_right_yk1.php" 
    	target="rightFrame">首页介绍</A></TD></TR>
  <TR height=4>
    <TD colSpan=2></TD></TR>
</TABLE>
<!-- 第二选项 -->
```
**3.Iframe实现局部加载效果**
通过iframe创建包含另外一个文档的内联框架(即行内框架)并实现局部加载功能，也就是点击左边不同的菜单右边显示不同的内容而整个界面布局并没有发生改变。
在index.php布局中首先采用<iframe></frame>布局，同时src中引用加载的php。代码如下：
```python
<!-- 布局中部右边 -->	
<div id="index_right">
<iframe height="100%" width="100%" border="0" frameborder="0" 
	src="main_right.php" name="rightFrame" id="rightFrame" title="rightFrame">
</iframe>
</div>
```
上面代码中其中src=""中加入要嵌入的页面，name=""要嵌入页面中traget。
然后在子菜单中添加：
<A href="main_right_yk2-2.php" target="rightFrame">教师团队</A>
href中添加要加载的php界面，target中添加框架中的name。
其中第二栏“课程概括”代码如下：(可参考：[百度文库](http://wenku.baidu.com/link?url=J0Zj4ebpeZR-oej7B8F8_sY49gFGaYrVzGlX2bVCYVtsfb0C56TqZiymnSL0hZ_BBEszCATeI8KdcC3CUCU7qlu5IfSNaQLKxlXvr1e1wXi))
```python
<TABLE id=child2 style="DISPLAY: none" cellSpacing=0 cellPadding=0 
width=150 border=0>
  <TR height=20>
    <TD align=middle width=30><IMG height=9 
      src="images/menu_icon.gif" width=9></TD>
    <TD><A href="main_right_yk2-1.php" class=menuChild 
    	target="rightFrame">课程简介</A></TD></TR>
  <TR height=20>
    <TD align=middle width=30><IMG height=9 
      src="images/menu_icon.gif" width=9></TD>
    <TD><A href="main_right_yk2-2.php" 
    	target="rightFrame">教师团队</A></TD></TR>
  <TR height=20>
    <TD align=middle width=30><IMG height=9 
      src="images/menu_icon.gif" width=9></TD>
    <TD><A href="main_right_yk2-3.php" 
    	target="rightFrame">教学条件</A></TD></TR>
  <TR height=4>
    <TD colSpan=2></TD></TR>
</TABLE>
```
最后讲讲接下来需要解决的问题：
1.iframe布局后，刷新总是重新加载index.php界面，而当前显示的内容会消失；
2.php如何通过Post+Session进行登陆及表单访问MySQL数据库；
3.如何实现网页index.php显示不同内容时index.php/xxxx加载些内容.
希望文章对你有所帮助，后天就要过年了！提前祝自己和大家新年快了。
下面地址是该部分界面代码：[http://pan.baidu.com/s/1740Cu](http://pan.baidu.com/s/1740Cu)
(BY:Eastmount 2015-2-16 清晨6点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))




