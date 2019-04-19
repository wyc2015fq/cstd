# 【HTML＋CSS网页设计与布局 从入门到精通】第3章 - Koma Hub - CSDN博客
2019年01月06日 18:09:28[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：304
**目录**
[段落首行缩进](#%E6%AE%B5%E8%90%BD%E9%A6%96%E8%A1%8C%E7%BC%A9%E8%BF%9B)
[无序列表](#%E6%97%A0%E5%BA%8F%E5%88%97%E8%A1%A8)
[段落右对齐](#%E6%AE%B5%E8%90%BD%E5%8F%B3%E5%AF%B9%E9%BD%90)
[页面背景颜色](#%E9%A1%B5%E9%9D%A2%E8%83%8C%E6%99%AF%E9%A2%9C%E8%89%B2)
[段落内个别字体加粗](#%E6%AE%B5%E8%90%BD%E5%86%85%E4%B8%AA%E5%88%AB%E5%AD%97%E4%BD%93%E5%8A%A0%E7%B2%97)
[字体字号设置](#%E5%AD%97%E4%BD%93%E5%AD%97%E5%8F%B7%E8%AE%BE%E7%BD%AE)
[特殊字符](#%E7%89%B9%E6%AE%8A%E5%AD%97%E7%AC%A6)
[数学公式](#%E6%95%B0%E5%AD%A6%E5%85%AC%E5%BC%8F)
[显示一张图片](#%E6%98%BE%E7%A4%BA%E4%B8%80%E5%BC%A0%E5%9B%BE%E7%89%87)
[显示图片，并规定大小](#%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87%EF%BC%8C%E5%B9%B6%E8%A7%84%E5%AE%9A%E5%A4%A7%E5%B0%8F)
[父子div](#%E7%88%B6%E5%AD%90div)
[父子+孙子div](#%E7%88%B6%E5%AD%90%2B%E5%AD%99%E5%AD%90div)
```
<html>
   <head>
      <title>段落与换行</title>
   </head>
   <body>
   
互联网发展的起源
	1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。
20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。
   </body>
</html>
```
```
<html>
   <head>
      <title>段落与换行</title>
   </head>
   <body>
   
    <p>互联网发展的起源</p>
    <p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
    <p>20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。</p>
   </body>
</html>
```
```
<html>
   <head>
      <title>段落与换行</title>
   </head>
   <body>
   
<h1>互联网发展的起源</h1>
<h2>第1阶段</h2>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<h2>第2阶段</h2>
    <p>20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。</p>
   </body>
</html>
```
```
<html>
   <head>
      <title>文本排版</title>
   </head>
   <body>
   
<center><h1>互联网发展的起源</h1></center>
<h2>第1阶段</h2>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<h2>第2阶段</h2>
    <p>20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。</p>
   </body>
</html>
```
# 段落首行缩进
```
<html>
   <head>
      <title>文本排版</title>
   </head>
   <body>
   
<center><h1>互联网发展的起源</h1></center>
<h2>第1阶段</h2>
<blockquote>
 1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。
</blockquote>
<h2>第2阶段</h2>
    <blockquote>
     20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。
    </blockquote>
   </body>
</html>
```
# 无序列表
```
<html>
	<head>
		<title>无序列表</title>
	</head>
	<body>
		这是一个无序列表：
		<P>
			<ul>
　				<li>绘制切片并导出
　　		    <li>编辑首页
　　		    <li>插入图像内容
　　			<li>设置自由延伸表格
　　			<li>编辑二级页面并把它另存为模板
	        </ul>
	</body>
</html>
```
# 段落右对齐
```
<html>
   <head>
      <title>文本排版</title>
   </head>
   <body>
   
<h1 align="center">互联网发展的起源</h1>
<h2 align="right">第1阶段</h2>
<p>
 1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。
</p>
<h2 align="right">第2阶段</h2>
    <p>
     20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。
    </p>
   </body>
</html>
```
# 页面背景颜色
```
<html>
   <head>
      <title>文本排版</title>
   </head>
   <body text="blue" bgcolor="#CCCCFF">
   
<h1 align="center">互联网发展的起源</h1>
<h2 align="right">第1阶段</h2>
<p>
 1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。
</p>
<h2 align="right">第2阶段</h2>
    <p>
     20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。
    </p>
   </body>
</html>
```
# 段落内个别字体加粗
```
<html>
   <head>
      <title>文本排版</title>
   </head>
   <body >
   
<h1 align="center">互联网发展的<i>起源</i></h1>
<h2 align="right">第1阶段</h2>
<p>
 1969年，为了<b>保障通信</b>联络，美国国防部高级研究计划署<em>ARPA</em>资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<h2 align="right">第2阶段</h2>
    <p>
     20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。
    </p>
   </body>
</html>
```
# 字体字号设置
```
<html>
    <head>
 	   <title>文本排版</title>
    </head>
    <body >
        <h1 align= "center"><font color="green"  face="楷体_GB2312" size="7"> 互联网发展的</font><i>起源</i></h1>
        <h2 align="right">第1阶段</h2>
        <p align=""> 1969年，为了<b>保障通信</b>联络，美国国防部高级研究计划署<em>ARPA</em>资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
        <h2 align="right">第2阶段</h2>
        <p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模 和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
    </body>
</html>
```
# 特殊字符
```
<html>
<head>
<title>使用符号</title>
</head>
<body>
<p>   假设有如下4个变量,并满足如下不等式:</p>
<p align="center">
 x > y <br>
 m < n </p>
 
<p align="right"> 版权所有©前沿教室</p>
</body>
</html>
```
# 数学公式
```
<html>
   <head>
      <title>运算式</title>
   </head>
   <body>
       [(6 <sup>3</sup> + 3 <sup>6</sup>) ÷ 2] ± 1 = ?<br>
      结果以 ‰ 表示。<p>
      H <sub>2</sub> + O <sub>2</sub> ⇔ H <sub>2</sub> O
   </body>
</html>
```
# 显示一张图片
```
<html>
   <head>
      <title>图像</title>
   </head>
   <body>
      <img src="cup.gif">
   </body>
</html>
```
# 显示图片，并规定大小
```
<html>
<head>
<title>图像</title>
</head>
<body>
   <img src="cup.gif">
   <img src="cup.gif"  width="64">
   <img src="cup.gif"  width="64" height="128" >
</body>
</html>
```
# 父子div
```
<head>
<title>设置父块的高度</title>
<style type="text/css">
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
    margin-top:30px; margin-bottom:0px;
    padding:15px;
    border:1px dashed #004993;
}
</style>
</head>
<body>
    <div class="father">
        <div class="son">子div</div>
    </div>
</body>
```
# 父子+孙子div
```
<head>
<title>设置父块的高度</title>
<style type="text/css">
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
    margin-top:30px; margin-bottom:0px;
    padding:15px;
    border:1px dashed #004993;
}
div.son2{						/* 子div */
    background-color:#a2d2ff;
    margin-top:30px; margin-bottom:0px;
    padding:15px;
    border:1px dashed #004993;
}
div.grandson{						/* 子div */
    background-color:#a2d2ff;
    margin-top:30px; margin-bottom:0px;
    padding:15px;
    border:1px dashed #004993;
}
</style>
</head>
<body>
    <div class="father">
        <div class="son">大儿子div</div>
        <div class="son2">二儿子div
            <div class="grandson">大孙子div</div>
        </div>
    </div>
</body>
```
