# 设置img属性 图片显示灰度图 - z69183787的专栏 - CSDN博客
2013年11月14日 18:00:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3411
```
<!DOCTYPE html>
<html lang="en">
	<head>
<style>
.highlightit img{
filter:gray;
-webkit-filter: grayscale(0.5);
gray:1;
}
.highlightit img{
filter:progid:DXImageTransform.Microsoft.Alpha(opacity=50);
-moz-opacity: 1;
opacity:0.5;}
</style>
</head>
HTML代码
<h3>www.865171.cn</h3>
<a href="http://www.865171.cn/" class="highlightit">
<img border="0" src="1.jpg"></a>
</html>
```
