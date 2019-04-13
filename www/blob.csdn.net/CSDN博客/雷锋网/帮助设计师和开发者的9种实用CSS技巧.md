
# 帮助设计师和开发者的9种实用CSS技巧 - 雷锋网 - CSDN博客


2012年09月27日 10:59:27[leiphone](https://me.csdn.net/leiphone)阅读数：1140


![](http://www.leiphone.com/wp-content/uploads/2012/08/html51.jpg)一个网页设计师脑子里一定会被大量与他的工作有关的知识填满。而说到CSS，它可以帮助对网站进行设计，让设计具有更多的可能性，用更简单的方式让你的网站更具吸引力，而使用传统的技术这些方面并不容易面面俱到。
对于前端，页面而言，CSS的确是一门首选需要掌握的技能。
以下就是9种非常实用的CSS3属性和使用技巧，
## 1.  圆角效果
谈及圆角小编曾在各种讨论中频繁听见：实现麻烦、兼容困难、性能不佳等等缺点。而如今的Web设计需要的就是各种各样新的开发技巧，因此越来越多的前端和页面都开始使用HTML5。
对于HTML5而言，从前必须使用图片来实现的需求，现在都可以通过代码来实现。而CSS3中加入的“border-radius”就可以用通过它来直接定义HTML元素的圆角，并支持所有浏览器的最新版本。
我可以通过如下代码实现：
|1
2
3
4|border-radius: 10px; /* CSS3 Property */
-moz-border-radius: 10px;  /* Firefox */
-webkit-border-radius: 10px; /* Chrome/Safari */
-khtml-border-radius: 10px; /* Linux browsers */|
我们也可以使用下面这样的组合版本，或者用它来对上面的代码进行速记。
|1|-moz-border-radius: 10px 20px 30px 0;|
而IE浏览器对CSS3属性支持，可以从这篇[英文文章](http://smashinghub.com/rounded-corners-with-css3.htm)进一步了解。
## 2.  阴影效果
另一个CSS3可实现的有趣功能是阴影效果（box-shadow），通过“box-shadow”可以非常简单的实现该功能。我们常见的主流浏览器都支持这个属性，而Safari浏览器支持可选加前缀的-webkit-box-shadow属性。
代码包括：
|1
2
3
4
5|\#myDiv{
-moz-box-shadow: 20px 10px 7px \#ccc;
-webkit-box-shadow: 20px 10px 7px \#ccc;
box-shadow: 20px 10px 7px \#ccc;
}|

而如下的JavaScript代码也能实现同样的阴影效果：
|1|object.style.boxShadow="20px 10px 7px \#ccc"|

## 3.  @Media属性
@Media可以直接指定应用样式引入媒体属性，用于相同样式针对不同屏幕尺寸的网页风格变化， 它也有助于响应式网页设计中的造型变化。通过简单的修改如下代码就能实现。
|1
2
3|@media screen and (max-width: 480px) {
}|
而使用如下的@media print甚至可以指定CSS的打印预览：
|1
2
3
4|@media print
{
p.content { color: \#ccc }
}|

## 4.  添加一个渐变填充
在CSS3中的渐变效果是另一个让人惊叹的属性。它目前还未得到全部浏览器的支持，因此最好不要完全依赖它来设置布局。而[这里](http://www.webdesignerwall.com/demo/css3-dropdown-menu/css-gradient-dropdown.html)有一个基于CSS的梯度导航栏可以让大家参考一下。
以下是实现的方式。
|1|background: -webkit-gradient(linear, left top, left bottom, from(darkGray), to(\#7A7A7A));|

## 5.  背景尺寸
背景尺寸（Background size）是CSS3中最实用的效果之一，目前很多浏览器都已经支持。Background size属性可以让你随心所欲的控制背景图的尺寸大小。
从前用于背景的图像大小在样式中是不可调整的，如果你无法想象那样有多么糟糕，可以试着回忆一下，在自己的电脑桌面上做壁纸的时候遇到一张合乎心意却偏偏与桌面尺寸不合的图片，只能通过平铺来填充。
那样的桌面丑的让一个审美观及格的人都会有砸屏幕的冲动。而现在我们可以通过改变一行代码让背景图片适应自己的页面。
|1
2
3
4
5
6|div
{
background:url(bg.jpg);
background-size:800px 600px;
background-repeat:no-repeat;
}|

## 6  @font face
属性在使用CSS3针对各种字体进行转变的时候已经充分证明了它的实用性。从前因为各种字体许可的问题，在设计过程中只能选择几种特定的字体。而通过@font face我们就能自定义字体的名称：
|1
2
3
4
5
6
7|@font-face
{
font-family: mySmashingFont;
src: url('blitz.ttf')
,url('blitz.eot'); /* IE9 */
}|
而后我们能通过一个简单的代码，随时随地的使用自定义的mySmashingFont字体系列
|1
2
3
4|div
{
font-family:mySmashingFont;
}|

## 7.  clearfix属性
如果Overflow: hidden清除浮动的效果实在不是你的首选，那么clearfix应该是你更好的选择，它可以让你个别处理不同的HTML元素。
|1
2
3|.clearfix {
display: inline-block;
}|
|1
2
3
4
5
6
7
8|.clearfix:after {
content: ".";
display: block;
clear: both;
visibility: hidden;
line-height: 0;
height: 0;
}|

## 8.  Margin: 0 auto
Margin: 0 auto实现的是CSS中最基础的功能，我们常常也会用它来实现最简单也最常用的功能—居中。虽然CSS本身没有一个指定居中的功能，但是通过auto margin指定一个边距仍然能非常简单的就实现居中这个功能。
通过这个属性，用一段简单的代码就能实现元素的居中。但需要注意的是，就像以下代码中那样，必须给div设定一个宽度。
|1
2
3
4
5|.myDiv {
margin: 0 auto;
width:600px;
}|

## 9.  Overflow: hidden
Overflow：Hidden，它做作用主要是隐藏溢出，而就像上述提到的，人们都很少用到其另外一个功能，就是清除浮动。
|1
2
3
4|div
{
overflow:hidden;
}|

Via：[Smashing
 HUB](http://smashinghub.com/9-amazing-css-rules-that-save-designers-and-developers.htm)

**（****[Warlial](http://www.leiphone.com/author/xiaolong)****供****雷锋网****专稿，转载请注明来自雷锋网及作者，并链回本页)**

