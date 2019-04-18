# JQuery -> 超级简单的下拉菜单 - weixin_33985507的博客 - CSDN博客
2015年07月04日 10:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

使用jquery实现一个超级简单的下拉菜单。
## 效果图
### 最初的效果
![](https://img-blog.csdn.net/20140516205806546?)
### 鼠标悬浮效果
![](https://img-blog.csdn.net/20140516205425500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRmVlTGFuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 代码
```
<!DOCTYPE html>
<html>
<head>
<script type="text/javascript" src="jquery-1.11.1.js"></script>
<style>
	nav a {
		text-decoration: none;
	}
	nav > ul > li {
		float: left;
		text-align: center;
		padding: 0 0.5em;
	}
	nav li ul.sub-menu {
		display: none;
		padding-left: 0 !important;
	}
	.sub-menu li {
		color: white;
	}
	.sub-menu li:hover {
		background-color: black;
	}
	.sub-menu li:hover a {
		color: white;
	}
	ul {
		list-style: none;
	}
</style>
</head>
<body>
	<nav>
	<ul>
		<li><a href="#">Home
		<ul class="sub-menu">
			<li><a href="#">sub1</a></li>
			<li><a href="#">sub2</a></li>
			<li><a href="#">sub3</a></li>
		</ul>
		</li>
		<li><a href="#">Programming
		<ul class="sub-menu">
		<li><a href="#">sub1</a></li>
			<li><a href="#">sub2</a></li>
			<li><a href="#">sub3</a></li>
		</ul>
		</li>
		<li><a href="#">Japanese
		<ul class="sub-menu">
			<li><a href="#">sub1</a></li>
			<li><a href="#">sub2</a></li>
			<li><a href="#">sub3</a></li>
		</ul>
		</li>		
	</ul>
	</nav>
</body>
<script type="text/javascript">
	$(document).ready(function() {
		$('nav li').hover(function() {
			$(this).find('.sub-menu').css('display', 'block');
		}, function() {
			$(this).find('.sub-menu').css('display', 'none');
		});
	});
</script>
</html>
```
版权声明：本文博客原创文章，博客，未经同意，不得转载。
