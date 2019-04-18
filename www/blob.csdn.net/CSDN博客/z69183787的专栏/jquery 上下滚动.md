# jquery 上下滚动 - z69183787的专栏 - CSDN博客
2012年11月15日 16:41:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：639
```
<html>
<head>
<style>
.content_dis {
    height: 400px;
    line-height: 25px;
    overflow: hidden;
}
</style>
<script>
//控制页面滚动
//外层元素的高度值是200px,内层元素的高度值是300px。很明显，“外层元素中的内容”高过了“外层元素”本身.当向下拖动滚动条时，有部分内容会隐没在“外层元素的上边界”之外，scrollTop就等于这部分“不可见的内容”的高度。 
function scroll(n){
	temp=n;
 	var News = document.getElementById("data1");
	News.scrollTop=News.scrollTop+temp;
	if (temp==0) return;
	setTimeout("scroll(temp)",100);
}
</script>
</head>
<body>
  <div id="data1" class="content_dis">
<table cellspacing="0" cellpadding="0" border="0" width="100%" id="fb_list_e">
<tbody>
  <tr><td><ul>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		<li class="f94 textdis">体操帝 同学评《西媒：切尔西将托雷斯加1500万报价法尔考》：</li>
		</ul>
		</td></tr><tr><td><ul>
	</table>
</div>
<div class="clear"></div>
<span id="scroll"> 
<a class="cds_up" onmousedown="scroll(-3)" onmouseover="scroll(-2)" onmouseout="scroll(0)">向上</a>
<a class="cds_down" onmousedown="scroll(3)" onmouseover="scroll(2)" onmouseout="scroll(0)">向下</a>
</span>
<div class="clear"></div>
</div>
</body>
</html>
```
