# 变化多端 – 多种纯CSS的HTML表格设计 - 沧海一粟 —— 技术随手记 - CSDN博客





2015年08月03日 20:00:47[慢游](https://me.csdn.net/eengel)阅读数：22235标签：[css																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[CSS](https://blog.csdn.net/eengel/article/category/2215619)








# 介绍

在HTML中，Table节点由于其层层嵌套的节点结构，一度名声很臭，且一度被呼吁用DIV+CSS取而代之。但在实际项目开发中，一碰到规整的数据显示，不知不觉又会用起它。可见其生命力之顽强。

这儿就探讨下几种不同的通过CSS实现的Table设计 。通过JavaScript渲染的效果不在此文讨论范围。表格最终渲染效果在Firefox 36.0或者Safari 8.0.7下实现。很可惜不是所有效果都能同时在两个浏览器中出现。

要说明的是，本文大部分内容可以说是对于 [Dudley Storey](http://demosthenes.info/about.php)的博文”[CSS Design Patterns For HTML5 Tables](http://demosthenes.info/blog/559/CSS-Design-Patterns-For-HTML5-Tables)”中表格实现的一些整理，偷懒的原因，使用的样例表格也是来自其中。 有兴趣的童鞋可以逛逛他的博客，或者看看他出的书”[Pro
 CSS3 Animation](http://www.amazon.ca/gp/product/1430247223/ref=as_li_ss_tl?ie=UTF8&camp=15121&creative=390961&creativeASIN=1430247223&linkCode=as2&tag=thedigitalw0e-20)”（不知道有没有中译本？）。当然根据思路后面也增加了一些本人的实现。



# 表格的HTML代码

没有特别研究过下面使用的HTML元素及属性是否符合HTML5规范，但设置doctype为支持HTML5的形式。



```
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
</head>
<body>
	<table>
		<caption>American Film Institute’s Top Five Films</caption>
		<thead>
			<tr>
				<th>Position
				<th>Movie
				<th>Year of Release
		</thead>
		<tbody>
			<tr>
				<td>1
				<td>Citizen Kane
				<td>1941
			<tr>
				<td>2
				<td>The Godfather
				<td>1972
			<tr>
				<td>3
				<td>Casablanca
				<td>1942
			<tr>
				<td>4
				<td>Raging Bull
				<td>1980
			<tr>
				<td>5
				<td>Singin’ In The Rain
				<td>1952
		</tbody>
	</table>
</body>
</html>
```




# 1.    传统网格状表格

## 效果

![](https://img-blog.csdn.net/20150803195810537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

```
table {

     border-collapse: collapse;

     font-family: Futura, Arial, sans-serif;

}

caption {

     font-size: larger;

     margin: 1em auto;

}

th,td {

     padding: .65em;

}

th {

     background: #555 nonerepeat scroll 0 0;

   border: 1px solid #777;

   color: #fff;

}

td {

     border: 1px solid#777;

}
```



## 说明

几个要注意的地方：

- 当渲染网格状表格时，往往说明对每个格子要加上边框。因此对于<th>, <td>，都要加上border属性。可想而知，如果这样的话，每个格子之间会出现两条边框。而我们又希望它们之间的边框只有一条，因此需要设置<table>的border-collapse属性为collapse。

- 表头和表格内容不一样，需要单独渲染。这儿通过设置<th>的background、color属性改变了表头的背景色和文字颜色。



# 2.    斑马条纹表格

## 效果

![](https://img-blog.csdn.net/20150803195817218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

在上面CSS基础上增加以下代码：

```
tbody tr:nth-child(odd) {

     background: #ccc;

}
```


## 说明

带有条纹的表格也非常经典。当表格行数较多时，这样的表格比传统网格状表格更能看得清楚。这个效果就是通过在<tr>上调用CSS选择器nth-child(odd)完成的。这儿的传入参数可以是一个表达式（an+b），或者build-in的函数，比如odd表示奇数(2n+1)，而even是偶数（2n）。



# 3.    圆角表格

## 效果

![](https://img-blog.csdn.net/20150803195822054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

```
table {

     border-collapse: collapse;

     font-family: Futura, Arial, sans-serif;

}

caption {

     font-size: larger;

     margin: 1em auto;

}

th,td {

     padding: .65em;

}

th {

     background: #555 nonerepeat scroll 0 0;

   /* border: 1px solid #777; */

   color: #fff;

}

td {

     /* border: 1px solid #777; */

}

tbody tr:nth-child(odd) {

     background: #ccc;

}

th:first-child {

     border-radius: 9px 0 0 0;

}

th:last-child {

     border-radius: 0 9px 0 0;

}

tr:last-child td:first-child {

     border-radius: 0 0 0 9px;

}

tr:last-child td:last-child {

     border-radius: 0 0 9px 0;

}
```



## 说明

从最后面几个CSS 来看，圆角主要通过border-radius属性实现。选择的正是表格四个角上的四个<th>/<td>元素。不幸的是，在Firefox中只出现了上面两个圆角效果。Safari中则都渲染出来了。

而border-radius属性和border属性并不捆绑，因此要做圆角时，需要将原来的网格线取消。



# 4.    线型表格

## 效果

![](https://img-blog.csdn.net/20150803195827694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

```
table {

     border-collapse: collapse;

     font-family: Futura, Arial, sans-serif;

}

caption {

     font-size: larger;

     margin: 1em auto;

}

th,td {

     padding: .65em;

}

th,td {

     border-bottom: 1px solid #ddd;

   border-top: 1px solid #ddd;

   text-align: center;

}
```



## 说明

这也是非常常用的一种表格设计，多用在展示科学结果上。很多科学论文中往往会用这样的设计方式：简洁、明了。实现的原理也很简单，将原来<th>/<td>上的border属性拆分开来，通过border-top和border-bottom只渲染上下的网格线。当然为了格子之间不出现两条线，每个格子的线还要连到一起，务必设置<table>的border-collapse属性为collapse。

# 5.    表格行上的立体悬停效果

## 效果

![](https://img-blog.csdn.net/20150803195834092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## CSS代码

在上面CSS基础上增加以下代码：

```
tbody tr:hover {

     background: linear-gradient(#fff,#aaa);

     font-size: 17px;

}
```



## 说明

总的来说，表格是一种二维呈现，HTML中产生的表格看起来就像和纸上画的一样。但其实它也能实现一些立体的效果，就像这儿展示的，当鼠标悬停在某一行上时，该行突出在渐变的灰色背景上，字体微微变大，有一种被放大的效果。这是通过在<tr>上加上hover选择器，并设置了background属性值为inear-gradient(#fff, #aaa)，并增加了font-size。




