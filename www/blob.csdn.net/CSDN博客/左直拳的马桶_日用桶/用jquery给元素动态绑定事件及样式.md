# 用jquery给元素动态绑定事件及样式 - 左直拳的马桶_日用桶 - CSDN博客
2015年04月03日 23:13:25[左直拳](https://me.csdn.net/leftfist)阅读数：5171
网页输出的时候，可以用jquery给各种元素绑定事件，或设置样式。
之所以这样做，好处是节省代码，尤其适合元素很多，并且元素的事件对应的函数雷同的情况。
看看以下代码：
```
<div id="divUserList">
		<span><a href="javascript:;" onclick="hi('001')">张三</a></span>
		<span><a href="javascript:;" onclick="hi('002')">李四</a></span>
		<span><a href="javascript:;" onclick="hi('003')">钱五</a></span>
		<span><a href="javascript:;" onclick="hi('004')">赵六</a></span>
		<span><a href="javascript:;" onclick="hi('005')">陈七</a></span>
		<span><a href="javascript:;" onclick="hi('006')">王八</a></span>
	</div>
	<script type="text/javascript">
		function hi(code){
			alert("my code is:" + code);
		}
	</script>
```
假设这个名单由后台生成，非常长，那么代码将会比较繁多。
如果采用事件动态绑定，则简洁许多：
```
<div id="divUserList">
	<!-- code是我们自定义的属性 -->
	<span code="001">张三</span>
	<span code="002">李四</span>
	<span code="003">钱五</span>
	<span code="004">赵六</span>
	<span code="005">陈七</span>
	<span code="006">王八</span>
</div>
```
```java
<script type="text/javascript">
	$(function () {//相当于javascript onload函数，页面加载完毕后触发，保证动态加载事件的元素都已存在
		$("div#divUserList [code]").each(function () {//id="divUserList"的DIV的子元素中，凡带有“code”属性的都被遍历
			$(this).live("click", function () {//绑定事件
				hi($(this).attr("code"));
			});
			
			$(this).css("cursor","pointer");//设置样式：鼠标指针
		});
	});
	function hi(code){
		alert("my code is:" + code);
	}	
</script>
```
