#  			简单实例让你快速掌握css sprites精髓 



这段时间有几次接触到了css  sprites的概念，一个就是在用css做滑动门的时候，另外一个就是在用YSlow分析网站性能的时候，于是对css  sprites这个概念产生了浓厚的兴趣。在网上查找了很多的资料，但可惜的是大部分都是只言片语，其中很多都是直接翻译国外的资料，也有直接推荐国外的资料网站，无奈英语没有过关，基本上没有理解到什么css  sprites，更别谈如何使用了。

最后还是在蓝色理想中的一篇文章受到启发，琢磨了老半天，才算弄清楚里面的内涵，下面将通过本人的理解帮助其他人更加快速的去掌握css sprites。

先简单介绍下概念，关于它的概念，网上那到处都是，这里简单的提下。

## 什么是css sprites

css  sprites直译过来就是CSS精灵，但是这种翻译显然是不够的，其实就是通过将多个图片融合到一副图里面，然后通过CSS的一些技术布局到网页上。这样做的好处也是显而易见的，因为图片多的话，会增加http的请求，无疑促使了网站性能的减低，特别是图片特别多的网站，如果能用css  sprites降低图片数量，带来的将是速度的提升。

下面我们来用一个实例来理解css sprites，受蓝色理想中的《制作一副扑克牌系列教程》启发。

我们仅仅只需要制作一个扑克牌，拿黑桃10为例子。

可以直接把蓝色理想中融合好的一幅大图作为背景，这也是css sprites的一个中心思想，就是把多个图片融进一个图里面。

![img](http://www.blueidea.com/articleimg/2009/02/6382/02.jpg)

这就是融合后的图，相信对PS熟悉的同学应该很容易的理解，通过PS我们就可以组合多个图为一个图。

现在我们书写html的结构：
```html
<div class="card">
   <div class="front">
  <b class="N1 n10"></b>
  <em class="First small_up1"></em>
  <span class="A1 up1"></span>
  <span class="A2 up1"></span>
  <span class="A3 down1"></span>
  <span class="A4 down1"></span>

  <span class="B1 up1"></span>
  <span class="B2 down1"></span>

  <span class="C1 up1"></span>
  <span class="C2 up1"></span>
  <span class="C3 down1"></span>
  <span class="C4 down1"></span>
  <em class="Last small_down1"></em>
  <b class="N2 n10_h"></b> 
   </div>
</div>
```
在这里我们来分析强调几点:

一：card为这个黑桃十的盒子或者说快，对div了解的同学应该很清楚这点。

二：我用span,b,em三种标签分别代表三种不同类型的图片，span用来表标中间的图片，b用来表示数定，em用来表示数字下面的小图标。

三：class里面的声明有2种，一个用来定位黑桃10中间的图片的位置，一个用来定义方向（朝上，朝下）。

上面是DIV基本概念，这还不是重点，不过对DIV不太清楚的同学可以了解。

下面我们重点谈下定义CSS：

```css
span{display:block;width:20px;height:21px; osition:absolute;background:url(images/card.gif) no-repeat;}
```

这个是对span容器的CSS定义，其他属性就不说了，主要谈下如何从这个里面来理解css sprites。

背景图片，大家看地址就是最开始我们融合的一张大图，那么如何将这个大图中的指定位置显示出来呢？因为我们知道我们做的是黑桃10，这个大图其他的图像我们是不需要的，这就用到了css中的**overflow:hidden;**

但大家就奇怪了span的CSS定义里面没有overflow:hidden啊，别急，我们已经在定义card的CSS里面设置了（这是CSS里面的继承关系）：

```css
.card{width:125px;height:170px; position:absolute;overflow:hidden;border:1px #c0c0c0 solid;}
```

理解到这点还不够，还要知道**width:125px;height:170px;** 这个可以理解成是对这个背景图片的准确切割，当然其实并不是切割，而是把多余其他的部分给隐藏了，这就是overflow:hidden的妙用。

通过以上的部分的讲解，你一定可以充分的把握css sprites的运用，通过这个所谓的“切割”，然后再通过CSS的定位技术将这些图准确的分散到这个card里面去！

PS:CSS的定位技术，大家可以参考其他资料，例如相对定位和绝对定位，这里我们只能尝试用绝对定位。

最后我们来补充一点：

为什么要采取`<span class="A1 up1"></span>`这样的结构？

span这个容器是主要作用就是存放这张大的背景图片，并在里面实现”切割“，span里面切割后的背景是所有内容块里面通用的！

后面class为什么要声明2个属性？

很显然，一个是用来定位内容块的位置，一个是用来定义内容块中的图像的朝上和朝下，方位的！

下面我们附上黑桃10的完整源码：

```html
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "<http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd>">
<html xmlns="<http://www.w3.org/1999/xhtml>">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>制作一幅扑克牌--黑桃10</title>
<style type="text/css"><!--
.card{width:125px;height:170px; position:absolute;overflow:hidden;border:1px #c0c0c0 solid;}
/*中间图片通用设置*/
span{display:block;width:20px;height:21px; position:absolute;background:url(<http://www.blueidea.com//articleimg/2009/02/6382/00.gif>) no-repeat;}
/*小图片通用设置*/
b{display:block;width:15px;height:10px; position:absolute;font-size:10pt;text-align:center;font-weight:bold;background:url(<http://www.blueidea.com//articleimg/2009/02/6382/00.gif>) no-repeat; overflow:hidden;}
/*数字通用设置*/
em{display:block;width:15px;height:10px; position:absolute;font-size:10pt;text-align:center;font-weight:bold;background:url(<http://www.blueidea.com//articleimg/2009/02/6382/00.gif>) no-repeat;overflow:hidden;}
/*各坐标点位置*/
.N1{left:1px;top:8px;}
.First{left:5px;top:20px;}
.A1{left:20px;top:20px;}
.A2{left:20px;top:57px;}
.A3{left:20px;top:94px;}
.A4{left:20px;top:131px;}
.B1{left:54px;top:38px;}
.B2{left:54px;top:117px;}
.C1{left:86px;top:20px;}
.C2{left:86px;top:57px;}
.C3{left:86px;top:94px;}
.C4{left:86px;top:131px;}
.Last{bottom:20px;right:0px;}
.N2{bottom:8px;right:5px;
}
/*大图标黑红梅方四种图片-上方向*/
.up1{background-position:0 1px;}/*黑桃*/
/*大图标黑红梅方四种图片-下方向*/
.down1{background-position:0 -19px;}/*黑桃*/
/*小图标黑红梅方四种图片-上方向*/
.small_up1{background-position:0 -40px;}/*黑桃*/
/*小图标黑红梅方四种图片-下方向*/
.small_down1{background-position:0 -51px;}/*黑桃*/
/*A~k数字图片-左上角*/
.n10{background-position:-191px 0px;left:-4px;width:21px;}
/*A~k数字图片-右下角*/
.n10_h{background-position:-191px -22px;right:3px;width:21px;}
/*A~k数字图片-左上角红色字*/
.n10_red{background-position:-191px 0px;}
/*A~k数字图片-右下角红色字*/
.n10_h_red{background-position:-191px -33px;}
-->
</style>

</head>

<body>
<!--10字符-->

<div class="card">
 <div class="front">
  <b class="N1 n10"></b>
  <em class="First small_up1"></em>
  <span class="A1 up1"></span>
  <span class="A2 up1"></span>
  <span class="A3 down1"></span>
  <span class="A4 down1"></span>

  <span class="B1 up1"></span>
  <span class="B2 down1"></span>

  <span class="C1 up1"></span>
  <span class="C2 up1"></span>
  <span class="C3 down1"></span>
  <span class="C4 down1"></span>
  <em class="Last small_down1"></em>
  <b class="N2 n10_h"></b> 
 </div>
</div>
</body>
</html>
```

