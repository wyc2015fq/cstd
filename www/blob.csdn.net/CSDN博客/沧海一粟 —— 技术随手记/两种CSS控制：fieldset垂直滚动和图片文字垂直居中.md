# 两种CSS控制：fieldset垂直滚动和图片文字垂直居中 - 沧海一粟 —— 技术随手记 - CSDN博客





2014年04月23日 15:40:04[慢游](https://me.csdn.net/eengel)阅读数：4979








# 1. 让fieldset里面出现垂直滚动条

很多情况下，当fieldset中的内容很多时，我们希望不用自动延长fieldset的高度，而是将其控制在一个固定高度上，然后用垂直滚动条来查看所有的内容。方法很简单，只是有几个需要注意的地方，如注释中所示。



```
<fieldset style="width: 230px; height: 100px;">
	<legend>basic information</legend>
	<!-- 1. 不管fieldset中的内容是什么，用一个div全包起来 -->
	<!-- 2. 设置div的height,并设置overflow:auto -->
	<div style="height:100%;overflow:auto;">
		<table>
			<tr>
				<td>name:</td>
				<td><input type="text" /> </td>
			</tr>
			<tr>
				<td>gender:</td>
				<td><input type="text" /> </td>
			</tr>
			<tr>
				<td>age:</td>
				<td><input type="text" /> </td>
			</tr>
			<tr>
				<td>height:</td>
				<td><input type="text" /> </td>
			</tr>
			<tr>
				<td>weight:</td>
				<td><input type="text" /> </td>
			</tr>
		</table>
	</div>
</fieldset>
```
最终效果如下图所示：



![](https://img-blog.csdn.net/20140423133353640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





# 2. 垂直居中图片和文字

下面是一个图片和文字混排的例子。这种情况下，如果不进行额外控制，图片和文字是没有办法做到垂直居中的（图中上半部分）。下面给出的代码主要通过设置span的vertical-align来实现垂直居中的效果（图中下半部分）。

![](https://img-blog.csdn.net/20140423152756203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZWVuZ2Vs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




```
<!-- 图片和文字没有垂直居中的效果 -->
<div style="border:1px solid #000000;margin-bottom: 2px;width: 160px;">
	<span style="font-size:18px;">Degree:</span>
	<img src="../img/cbar1.jpg"/>
	<span style="font-size:12px;">2.37</span>
	<img src="../img/info.png"/>
</div>
<!-- 图片和文字垂直居中 -->
<div style="border:1px solid #000000;width: 160px;">
	<!-- 1. 首先需要设定span的vertical-align: middle -->
	<!-- 2. 然后可以通过padding-bottom之类的调整文字的位置，并最终和图片垂直居中 -->
	<span style="vertical-align: middle;padding-bottom: 12px; font-size:18px;">Degree:</span>
	<img src="../img/cbar1.jpg"/>
	<span style="vertical-align: middle;padding-bottom: 10px; font-size:12px;">2.37</span>
	<img src="../img/info.png"/>
</div>
```







