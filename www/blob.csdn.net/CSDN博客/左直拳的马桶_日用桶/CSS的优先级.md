# CSS的优先级 - 左直拳的马桶_日用桶 - CSDN博客
2013年11月27日 16:29:40[左直拳](https://me.csdn.net/leftfist)阅读数：1407
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
                1）内嵌样式最高，其次是内部样式与外部样式并列，以最后定义的为准
2）同等条件下，ID选择符 > 类选择符 > 标记选择符
3）可以用!important进行提升
例如：
```
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8" />
	<title>CSS优先级</title>
	<style type="text/css">
		.title{width:350px;margin:auto;}
		h1{color:red!important;}
		.color{color:pink;}
		#h1Title{color:orange;}
	</style>
</head>
<body>
	<div>
		<div class="title"><h1 style="color:gray;"  id="h1Title" class="color">纤笔一支谁与似？</h1></div>
	</div>
</body>
</html>
```
这样下来，字体的颜色就是红色，尽管标记选择符(在这里是 h1)原本最低，但经过!important提升以后，就成了最优先。
注意这个!important不是针对选择符，而是针对属性。比如这个h1选择符里，如果有多个属性的话，只有后面附上!important的属性才会提升，而并非一人得道，鸡犬升天。
