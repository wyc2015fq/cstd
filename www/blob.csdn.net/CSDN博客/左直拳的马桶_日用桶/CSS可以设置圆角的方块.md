# CSS可以设置圆角的方块 - 左直拳的马桶_日用桶 - CSDN博客
2010年03月29日 11:11:00[左直拳](https://me.csdn.net/leftfist)阅读数：2464
原来用CSS可以设置圆角的方块。
究其原理，是用3条长度不同的直线（或更多）叠在一起，模拟出这个弯曲的圆角，正如多边形可以组成圆形一样，只要边够多，就可以无限逼近一个圆。
===============================================================
附网上找到的示例
<html>
<head>
<title> 用CSS做圆角方块</title>
<style type="text/css">
.roundBorder
{
 display: block;
}
.roundBorder *
{
 background: white;
 display: block;
 height: 1px;
 overflow: hidden;
}
.roundBorderLayer1
{
 border-left: 1px solid #C4C4C4;
 border-right: 1px solid #C4C4C4;
 margin: 0px 1px;
}
.roundBorderLayer2
{
 border-left: 1px solid #D3D4D5;
 border-right: 1px solid #D3D4D5;
 margin: 0px 2px;
 padding: 0px;
}
.roundBorderLayer3
{ 
 background: #C4C4C4;
 border-left: 1px solid #D3D4D5;
 border-right: 1px solid #D3D4D5;
 margin: 0px 3px;
}
.roundContent
{
 background:white;
 border-left: 1px solid #C4C4C4;
 border-right: 1px solid #C4C4C4;
 overflow: hidden;
 padding: 4px 10px;
}
</style>
</head>
<body>
<b class="roundBorder">
 <b class="roundBorderLayer3"></b>
 <b class="roundBorderLayer2"></b>
 <b class="roundBorderLayer1"></b>
</b>
<div class="roundContent">
 Hello!!
 <br/>
 用CSS做圆角方块
 <br/>
 真的很简单
</div>
<b class="roundBorder">
 <b class="roundBorderLayer1"></b>
 <b class="roundBorderLayer2"></b>
 <b class="roundBorderLayer3"></b>
</b>
</body>
</html>
