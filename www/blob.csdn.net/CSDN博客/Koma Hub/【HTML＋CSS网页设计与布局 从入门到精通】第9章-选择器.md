# 【HTML＋CSS网页设计与布局 从入门到精通】第9章-选择器 - Koma Hub - CSDN博客
2019年01月06日 21:38:09[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：104
**目录**
[标记选择器+类别选择器](#%E6%A0%87%E8%AE%B0%E9%80%89%E6%8B%A9%E5%99%A8%2B%E7%B1%BB%E5%88%AB%E9%80%89%E6%8B%A9%E5%99%A8)
[并集选择器+集体声明](#%E5%B9%B6%E9%9B%86%E9%80%89%E6%8B%A9%E5%99%A8%2B%E9%9B%86%E4%BD%93%E5%A3%B0%E6%98%8E)
[后代（嵌套）选择器](#%E5%90%8E%E4%BB%A3%EF%BC%88%E5%B5%8C%E5%A5%97%EF%BC%89%E9%80%89%E6%8B%A9%E5%99%A8)
[继承关系演示](#%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB%E6%BC%94%E7%A4%BA)
[前沿Web开发教室](#%E5%89%8D%E6%B2%BFWeb%E5%BC%80%E5%8F%91%E6%95%99%E5%AE%A4)
[继承关系演示-选择器1](#%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB%E6%BC%94%E7%A4%BA-%E9%80%89%E6%8B%A9%E5%99%A81)
[继承关系演示-选择器2](#%E7%BB%A7%E6%89%BF%E5%85%B3%E7%B3%BB%E6%BC%94%E7%A4%BA-%E9%80%89%E6%8B%A9%E5%99%A82)
[还可以写成这样批量的整](#%E8%BF%98%E5%8F%AF%E4%BB%A5%E5%86%99%E6%88%90%E8%BF%99%E6%A0%B7%E6%89%B9%E9%87%8F%E7%9A%84%E6%95%B4)
[或者](#%E6%88%96%E8%80%85)
[再或者](#%E5%86%8D%E6%88%96%E8%80%85)
[or](#or)
[层叠特性](#%E5%B1%82%E5%8F%A0%E7%89%B9%E6%80%A7)
# 标记选择器+类别选择器
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>选择器.class</title>
<style type="text/css">
p{						/* 标记选择器 */
	color:blue;
}
p.special{				/* 标记.类别选择器 */
	color:red;			/* 红色 */
}
.special{				/* 类别选择器 */
	color:green;
}
</style>
</head>
<body>
	<p>普通段落文本（蓝色）</p>
	<h3>普通标题文本（黑色）</h3>
	<p class="special">指定了.special类别的段落文本（红色）</p>
	<h3 class="special">指定了.special类别的标题文本（绿色）</h3>
</body>
</html>
```
# 并集选择器+集体声明
```
<html>
<head>
<title>并集选择器</title>
<style type="text/css">
h1, h2, h3, p{			/*并集选择器*/
	color:purple;				/* 文字颜色 */
	font-size:15px;				/* 字体大小 */
}
h2.special, .special, #one{		/* 集体声明 */
	text-decoration:underline;	/* 下划线 */
}
</style>
</head>
<body>
	<h1>示例文字h1</h1>
	<h2 class="special">示例文字h2</h2>
	<h3>示例文字h3</h3>
	<p>示例文字p1</p>
	<p class="special">示例文字p2</p>
	<p id="one">示例文字p3</p>
</body>
</html>
```
# 后代（嵌套）选择器
```
<html>
<head>
<title>后代选择器</title>
<style type="text/css">
p span{						/* 嵌套声明 */
	color:red;				/* 颜色 */
}
span{
	color:blue;				/* 颜色 */
}
</style>
</head>
<body>
	<p>嵌套使<span>用CSS（红色）</span>标记的方法</p>
	嵌套之外的<span>标记（蓝色）</span>不生效
</body>
</html>
```
# 继承关系演示
```
<html>
<head>
	<title>继承关系演示</title>
</head>
<body>
	<h1>前沿<em>Web开发</em>教室</h1>
	<ul>
		<li>Web设计与开发需要使用以下技术：
			<ul>
				<li>HTML</li>
				<li>CSS
				<ul>
					<li>选择器</li>
					<li>盒子模型</li>
					<li>浮动与定位</li>				
				</ul>
				</li>
				<li>Javascript</li>
			</ul>
		</li>
		<li>此外，还需要掌握：：
			<ol>
				<li>Flash</li>
				<li>Dreamweaver</li>
				<li>Photoshop</li>
			</ol>
		</li>
	</ul>
	<p>如果您有任何问题，欢迎联系我们</p>
</body>
</html>
```
# 前沿*Web开发*教室
- Web设计与开发需要使用以下技术：
	- HTML
- CSS
		- 选择器
- 盒子模型
- 浮动与定位
- Javascript
- 此外，还需要掌握：：
	- Flash
- Dreamweaver
- Photoshop
如果您有任何问题，欢迎联系我们
## 继承关系演示-选择器1
```
<html>
<head>
	<title>继承关系演示</title>
<style>
h1{
	color:blue;					/* 颜色 */
	text-decoration:underline;	/* 下划线 */
	}
em{
	color:red;					/* 颜色 */
	}
</style>
</head>
<body>
	<h1>前沿<em>Web开发</em>教室</h1>
	<ul>
		<li>Web设计与开发需要使用以下技术：
			<ul>
				<li>HTML</li>
				<li>CSS
				<ul>
					<li>选择器</li>
					<li>盒子模型</li>
					<li>浮动与定位</li>				
				</ul>
				</li>
				<li>Javascript</li>
			</ul>
		</li>
		<li>此外，还需要掌握：：
			<ol>
				<li>Flash</li>
				<li>Dreamweaver</li>
				<li>Photoshop</li>
			</ol>
		</li>
	</ul>
	<p>如果您有任何问题，欢迎联系我们</p>
</body>
</html>
```
## 继承关系演示-选择器2
```
<html>
<head>
	<title>继承关系演示</title>
<style>
h1{
	color:blue;					/* 颜色 */
	text-decoration:underline;	/* 下划线 */
	}
em{
	color:red;					/* 颜色 */
	}
li{
	font-weight:bold;
	}
</style>
</head>
<body>
	<h1>前沿<em>Web开发</em>教室</h1>
	<ul>
		<li>Web设计与开发需要使用以下技术：
			<ul>
				<li>HTML</li>
				<li>CSS
				<ul>
					<li>选择器</li>
					<li>盒子模型</li>
					<li>浮动与定位</li>				
				</ul>
				</li>
				<li>Javascript</li>
			</ul>
		</li>
		<li>此外，还需要掌握：：
			<ol>
				<li>Flash</li>
				<li>Dreamweaver</li>
				<li>Photoshop</li>
			</ol>
		</li>
	</ul>
	<p>如果您有任何问题，欢迎联系我们</p>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190106213602416.PNG)
### 还可以写成这样批量的整
```
li ul li ul li{
	font-weight:bold;
	}
```
![](https://img-blog.csdnimg.cn/20190106213617740.PNG)
### 或者
```
li li{
	font-weight:bold;
	}
```
![](https://img-blog.csdnimg.cn/2019010621363114.PNG)
### 再或者
```
ul li{
	font-weight:bold;
	}
```
![](https://img-blog.csdnimg.cn/20190106213647437.PNG)
### or
```
ul ul li{
	font-weight:bold;
	}
```
![](https://img-blog.csdnimg.cn/20190106213659809.PNG)
# 层叠特性
```
<html>
<head>
<title>层叠特性</title>
<style type="text/css">
p{				
	color:green;	
	}
.red{
	color:red;		
	}
.purple{
	color:purple;				
	}
#line3{
	color:blue;
	}
</style>
</head>
<body>
	<p >这是第1行文本</p>
	<p class="red">这是第2行文本</p>
	<p id="line3" class="red">这是第3行文本</p>
	<p style="color:orange;" id="line3">这是第4行文本</p>
	<p class="purple red">这是第5行文本</p>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190106213740993.PNG)
