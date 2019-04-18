# 外层DIV 包含内层DIV 样式问题 - z69183787的专栏 - CSDN博客
2014年05月29日 16:11:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5104
```
<!DOCTYPE html>
<html>
  <head>
<style>
.clearfix {
display: block;
float: none;
clear: both;
overflow: hidden;
width: auto;
height: 0;
margin: 0 auto;
padding: 0;
font-size: 0;
line-height: 0;
</style>
 
	</head>
<html>
<div class="main" style="width:500px;border:1px red solid;margin:0 auto;">
<div style="float:left;border:1px blue solid;">
dasdsadsad
</div>
<div style="float:right;border:1px yellow solid;">
dsadsa
</div>
<div class="clearfix"></div>
</div>
</html>
```
关键 就是 在 增加浮动层，使用伪类 及 伪元素（:after） 都可。
