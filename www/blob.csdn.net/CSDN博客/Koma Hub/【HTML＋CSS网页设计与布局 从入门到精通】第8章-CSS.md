# 【HTML＋CSS网页设计与布局 从入门到精通】第8章-CSS - Koma Hub - CSDN博客
2019年01月06日 21:18:09[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：81
**目录**
[无格式](#%E6%97%A0%E6%A0%BC%E5%BC%8F)
[图文并茂](#%E5%9B%BE%E6%96%87%E5%B9%B6%E8%8C%82)
[标题h1格式](#%E6%A0%87%E9%A2%98h1%E6%A0%BC%E5%BC%8F)
[图片img格式](#%E5%9B%BE%E7%89%87img%E6%A0%BC%E5%BC%8F)
[正文p格式](#%E6%AD%A3%E6%96%87p%E6%A0%BC%E5%BC%8F)
[body格式](#body%E6%A0%BC%E5%BC%8F)
[ID选择器的优先级低于标记选择器](#ID%E9%80%89%E6%8B%A9%E5%99%A8%E7%9A%84%E4%BC%98%E5%85%88%E7%BA%A7%E4%BD%8E%E4%BA%8ECSS)
# 无格式
```
<html>
<head>
<title>体验CSS</title>
</head>
<body>
<h1>互联网发展的起源</h1>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# 图文并茂
```
<html>
<head>
<title>体验CSS</title>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# 标题h1格式
```
<html>
<head>
<title>体验CSS</title>
<style>
h1{
	color:white;					/* 文字颜色*/
	background-color:#0000FF;		/* 背景色 */
	text-align:center;				/* 居中 */
	padding:15px;					/* 边距 */
}
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# 图片img格式
```
<html>
<head>
<title>体验CSS</title>
<style>
h1{
	color:white;					/* 文字颜色*/
	background-color:#0000FF;		/* 背景色 */
	font-size:25px;					/* 字号 */
	font-weight:bold;				/* 粗体 */
	text-align:center;				/* 居中 */
	padding:15px;					/* 间距 */
}
img{float:left;
border:1px #9999CC dashed;
margin:5px;
}
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/>
<p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# 正文p格式
```
<html>
<head>
<title>体验CSS</title>
<style>
h1{
	color:white;					/* 文字颜色*/
	background-color:#0000FF;		/* 背景色 */
	font-size:25px;					/* 字号 */
	font-weight:bold;				/* 粗体 */
	text-align:center;				/* 居中 */
	padding:15px;					/* 间距 */
}
img{float:left;
border:1px #9999CC dashed;
margin:5px;
}
p{
font-size:12px;
text-indent:2em;
line-height:1.5;
padding:5px;
}
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/><p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# body格式
```
<html>
<head>
<title>体验CSS</title>
<style>
body{margin:0px;
background-color:#CCCCFF;
}
h1{
	color:white;					/* 文字颜色*/
	background-color:#0000FF;		/* 背景色 */
	font-size:25px;					/* 字号 */
	font-weight:bold;				/* 粗体 */
	text-align:center;				/* 居中 */
	padding:15px;					/* 间距 */
}
img{float:left;
border:1px #9999CC dashed;
margin:5px;
}
p{
font-size:12px;
text-indent:2em;
line-height:1.5;
padding:5px;
}
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/><p>1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
# ID选择器的优先级低于标记选择器
```
<html>
<head>
<title>体验CSS</title>
<style>
body{margin:0px;
background-color:#CCCCFF;
}
h1{
	color:white;					/* 文字颜色*/
	background-color:#0000FF;		/* 背景色 */
	font-size:25px;					/* 字号 */
	font-weight:bold;				/* 粗体 */
	text-align:center;				/* 居中 */
	padding:15px;					/* 间距 */
}
img{float:left;
border:1px #9999CC dashed;
margin:5px;
}
p{
font-size:14px;
text-indent:2em;
line-height:1.5;
padding:5px;
}
#p1{
border-right:4px red double ;
}
#p2{
border-right:4px orange double ;
}
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<img src="cup.gif" width="128" height="128"/><p id="p1">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
<p id="p2"> 20世纪70年代末到80年代初，计算机网络蓬勃发展，各种各样的计算机网络应运而生，如MILNET、USENET、BITNET、CSNET等，在网络的规模和数量上都得到了很大的发展。一系列网络的建设，产生了不同网络之间互联的需求，并最终导致了TCP/IP协议的诞生。 </p>
</body>
</html>
```
