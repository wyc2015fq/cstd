# 【HTML＋CSS网页设计与布局 从入门到精通】第11章-CSS - Koma Hub - CSDN博客
2019年01月07日 21:45:21[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：77
个人分类：[CSS																[HTML](https://blog.csdn.net/Rong_Toa/article/category/7651718)](https://blog.csdn.net/Rong_Toa/article/category/8590132)
**目录**
[图像class选择器](#%E5%9B%BE%E5%83%8Fclass%E9%80%89%E6%8B%A9%E5%99%A8)
[设置图片每个边框的格式：颜色，线型](#%E8%AE%BE%E7%BD%AE%E5%9B%BE%E7%89%87%E6%AF%8F%E4%B8%AA%E8%BE%B9%E6%A1%86%E7%9A%84%E6%A0%BC%E5%BC%8F%EF%BC%9A%E9%A2%9C%E8%89%B2%EF%BC%8C%E7%BA%BF%E5%9E%8B)
[另一个例子](#%E5%8F%A6%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90)
[图文混排](#%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92)
[图片左对齐，居中，右对齐](#%E5%9B%BE%E7%89%87%E5%B7%A6%E5%AF%B9%E9%BD%90%EF%BC%8C%E5%B1%85%E4%B8%AD%EF%BC%8C%E5%8F%B3%E5%AF%B9%E9%BD%90)
[竖直对齐](#%E7%AB%96%E7%9B%B4%E5%AF%B9%E9%BD%90)
[竖直对齐：具体数值-负数](#%E7%AB%96%E7%9B%B4%E5%AF%B9%E9%BD%90%EF%BC%9A%E5%85%B7%E4%BD%93%E6%95%B0%E5%80%BC-%E8%B4%9F%E6%95%B0)
[八大行星：图文混排](#%E5%85%AB%E5%A4%A7%E8%A1%8C%E6%98%9F%EF%BC%9A%E5%9B%BE%E6%96%87%E6%B7%B7%E6%8E%92)
# 图像class选择器
```
<html>
   <head>
      <title>图像</title>
<style type="text/css">
.test1{
	border-style:dotted;	/* 点画线 */
	border-color:#996600;	/* 边框颜色 */
	border-width:4px;		/* 边框粗细 */
}
.test2{
	border-style:dashed;	/* 虚线 */
	border-color:blue;		/* 边框颜色 */
	border-width:2px;		/* 边框粗细 */
}
</style>
   </head>
   <body>
      <img src="cup.gif" class="test1"> <img src="cup.gif" class="test2">
   </body>
</html>
```
# 设置图片每个边框的格式：颜色，线型
```
<html>
   <head>
      <title>图像</title>
<style type="text/css">
img{
	border-left-style:dotted;	/* 左点画线 */
	border-left-color:#FF9900;	/* 左边框颜色 */
	border-left-width:3px;		/* 左边框粗细 */
	border-right-style:dashed;
	border-right-color:#33CC33;
	border-right-width:2px;
	border-top-style:solid;		/* 上实线 */
	border-top-color:#CC44FF;	/* 上边框颜色 */
	border-top-width:2px;		/* 上边框粗细 */
	border-bottom-style:groove;
	border-bottom-color:#66cc66;
	border-bottom-width:3px;
}</style>
   </head>
   <body>
      <img src="cup.gif" class="test1">
   </body>
</html>
```
## 另一个例子
```
<html>
   <head>
      <title>图像</title>
<style type="text/css">
img{
	border-style:dashed;
	border-width:2px;
	border-color:red;
	
	border-left-style:solid;	
	border-top-width:4px;		
	border-right-color:blue;		
	}
</style>
   </head>
   <body>
      <img src="cup.gif" class="test1">
   </body>
</html>
```
# 图文混排
```
<html>
<head>
<title>图文混排</title>
<style type="text/css">
<!--
body{
	background-color:#EAECDF;	/* 页面背景颜色 */
	margin:0px;
	padding:0px;
}
img{
	float:right;					/* 文字环绕图片 */
	margin:10px;
}
p{
	color:#000000;				/* 文字颜色 */
	margin:0px;
	padding-top:10px;
	padding-left:5px;
	padding-right:5px;
}
span{
	float:left;					/* 首字放大 */
	font-size:60px;
	font-family:黑体;
	margin:0px;
	padding-right:5px;
}
-->
</style>
   </head>
<body>
	<img src="zcz.jpg" border="0">
	<p><span>祖</span> 冲之（公元429年─公元500年）是中国数学家、科学家。南北朝时期人，字文远。生于未文帝元嘉六年，卒于齐昏侯永元二年。祖籍范阳郡遒县（今河北涞水县）。先世迁入江南，祖父掌管土木建筑，父亲学识渊博。祖冲之从小接受家传的科学知识。青年时进入华林学省，从事学术活动。一生先后任过南徐州（今镇江市）从事史、公府参军、娄县（今昆山县东北）令、谒者仆射、长水校尉等官职。其主要贡献在数学、天文历法和机械三方面。在数学方面，他写了《缀术》一书，被收入著名的《算经十书》中，作为唐代国子监算学课本，可惜后来失传了。《隋书·律历志》留下一小段关于圆周率（π）的记载，祖冲之算出π的真值在3.1415926（朒数）和3.1415927（盈数）之间，相当于精确到小数第7位，成为当时世界上最先进的成就。这一纪录直到15世纪才由阿拉伯数学家卡西打破。</p>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190107213756978.jpg)
# 图片左对齐，居中，右对齐
```
<html>
<head>
<title>水平对齐</title>
</head>
<body>
	<p style="text-align:left;"><img src="cup.gif"></p>
	<p style="text-align:center;"><img src="cup.gif"></p>
	<p style="text-align:right;"><img src="cup.gif"></p>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190107213853394.gif)
# 竖直对齐
```
<html>
<head>
<title>竖直对齐</title>
<style type="text/css">
p{ font-size:15px; 
border:1px red solid;}
img{ border: 1px solid #000055; }
</style>
   </head>
<body>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:baseline;">方式:baseline<img src="miki.jpg" style="vertical-align:baseline;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:top">方式:top<img src="miki.jpg" style="vertical-align:top">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:middle;">方式:middle<img src="miki.jpg" style="vertical-align:middle;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:bottom;">方式:bottom<img src="miki.jpg" style="vertical-align:bottom;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:text-bottom;">方式:text-bottom<img src="miki.jpg" style="vertical-align:text-bottom;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:text-top;">方式:text-top<img src="miki.jpg" style="vertical-align:text-top;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:sub;">方式:sub<img src="miki.jpg" style="vertical-align:sub;">方式</p>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:super;">方式:super<img src="miki.jpg" style="vertical-align:super;">方式</p>
</body>
</html>
```
# 竖直对齐：具体数值-负数
```
<html>
<head>
<title>竖直对齐，具体数值</title>
<style type="text/css">
p{ font-size:15px; 
  border: 1px red solid; }
img{ border: 1px #000055 solid ; }
</style>
</head>
<body>
	<p>竖直对齐<img src="donkey.jpg" style="vertical-align:20px;">方式: 20px</p>
	<p>竖直对齐<img src="miki.jpg" style="vertical-align:-30px;">方式: -30px</p>
</body>
</html>
```
# 八大行星：图文混排
```
<html>
<head>
<title>太阳系的八大行星</title>
<style type="text/css">
<!--
body{
	background-color:black;	/* 页面背景色 */
}
p{
	font-size:13px;				/* 段落文字大小 */
	color:white;
}
p.title1{						/* 左侧标题 */
	text-decoration:underline;	/* 下划线 */
	font-size:18px;
	font-weight:bold;			/* 粗体*/
	text-align:left;			/* 左对齐 */
}
p.title2{						/* 右侧标题 */
	text-decoration:underline;
	font-size:18px;
	font-weight:bold;
	text-align:right;
}
p.content{						/* 正文内容 */
	line-height:1.2em;			/* 正文行间距 */
	margin:0px;
}
img{
	border:1px #999 dashed;	/* 图片边框 */
}
img.pic1{
	float:left;					/* 左侧图片混排 */
	margin-right:10px;			/* 图片右端与文字的距离 */
	margin-bottom:5px;
}
img.pic2{
	float:right;				/* 右侧图片混排 */
	margin-left:10px;			/* 图片左端与文字的距离 */
	margin-bottom:5px;
}
span.first{						/* 首字放大 */
	font-size:60px;
	font-family:黑体;
	float:left;
	font-weight:bold;
	color:#CCC;				/* 首字颜色 */
}
-->
</style>
   </head>
<body>
	<img src="baall.jpg" class="pic2">
	<p><span class="first">太</span>阳系是以太阳为中心，和所有受到太阳的重力约束天体的集合体：8颗行星、至少165颗已知的卫星、3颗已经辨认出来的矮行星（冥王星和他的卫星）和数以亿计的太阳系小天体。这些小天体包括小行星、柯伊伯带的天体、彗星和星际尘埃。依照至太阳的距离，行星序是水星、金星、地球、火星、木星、土星、天王星、和海王星，8颗中的6颗有天然的卫星环绕着。</p>
	<p class="title1">水星</p>
	<img src="ba1.jpg" class="pic1">
	<p class="content">
	水星在八大行星中是最小的行星，比月球大1/3，它同时也是最靠近太阳的行星。 水星目视星等范围从 0.4 到 5.5；水星太接近太阳，常常被猛烈的阳光淹没，所以望远镜很少能够仔细观察它。水星没有自然卫星。唯一靠近过水星的卫星是美国探测器水手10号，在1974年—1975年探索水星时，只拍摄到大约45%的表面。水星是太阳系中运动最快的行星。水星的英文名字Mercury来自罗马神墨丘利（赫耳墨斯）。他是罗马神话中的信使。因为水星约88天绕太阳一圈，是太阳系中公转最快的行星。符号是上面一个圆形下面一个交叉的短垂线和一个半圆形 (Unicode). 是墨丘利所拿魔杖的形状。在前5世纪，水星实际上被认为成二个不同的行星，这是因为它时常交替地出现在太阳的两侧。当它出现在傍晚时，它被叫做墨丘利；但是当它出现在早晨时，为了纪念太阳神阿波罗，它被称为阿波罗。毕达哥拉斯后来指出他们实际上是相同的一颗行星。</p>
	<p class="title2">金星</p>
	<img src="ba2.jpg" class="pic2">
	<p class="content">金星是八大行星之一，按离太阳由近及远的次序是第二颗。它是离地球最近的行星。中国古代称之为太白或太白金星。它有时是晨星，黎明前出现在东方天空，被称为“启明”；有时是昏星，黄昏后出现在西方天空，被称为“长庚”。金星是全天中除太阳和月亮外最亮的星，亮度最大时为-4.4等，比著名的天狼星（除太阳外全天最亮的恒星）还要亮14倍，犹如一颗耀眼的钻石，于是古希腊人称它为阿佛洛狄忒（Aphrodite）——爱与美的女神，而罗马人则称它为维纳斯（Venus）——美神。金星和水星一样，是太阳系中仅有的两个没有天然卫星的大行星。因此金星上的夜空中没有“月亮”，最亮的“星星”是地球。由于离太阳比较近，所以在金星上看太阳，太阳的大小比地球上看到的大1.5倍。</p>
	<p class="title1">地球</p>
	<img src="ba3.jpg" class="pic1">
	<p class="content">水星在八大行星中是最小的行星，比月球大1/3，它同时也是最靠近太阳的行星。 水星目视星等范围从 0.4 到 5.5；水星太接近太阳，常常被猛烈的阳光淹没，所以望远镜很少能够仔细观察它。水星没有自然卫星。唯一靠近过水星的卫星是美国探测器水手10号，在1974年—1975年探索水星时，只拍摄到大约45%的表面。水星是太阳系中运动最快的行星。水星的英文名字Mercury来自罗马神墨丘利（赫耳墨斯）。他是罗马神话中的信使。因为水星约88天绕太阳一圈，是太阳系中公转最快的行星。符号是上面一个圆形下面一个交叉的短垂线和一个半圆形 (Unicode). 是墨丘利所拿魔杖的形状。在前5世纪，水星实际上被认为成二个不同的行星，这是因为它时常交替地出现在太阳的两侧。当它出现在傍晚时，它被叫做墨丘利；但是当它出现在早晨时，为了纪念太阳神阿波罗，它被称为阿波罗。毕达哥拉斯后来指出他们实际上是相同的一颗行星。</p>
	<p class="title2">火星</p>
	<img src="ba4.jpg" class="pic2">
	<p class="content">　火星（Mars）是八大行星之一，符号是♂。因为它在夜空中看起来是血红色的，所以在西方，以希腊神话中的阿瑞斯(或罗马神话中对应的战神玛尔斯)命名它。在古代中国，因为它荧荧如火，故称“荧惑”。火星有两颗小型天然卫星:火卫一Phobos和火卫二Deimos(阿瑞斯儿子们的名字)。两颗卫星都很小而且形状奇特，可能是被引力捕获的小行星。英文里前缀areo-指的就是火星。</p>
	<p class="title1">木星</p>
	<img src="ba5.jpg" class="pic1">
	<p class="content">木星古称岁星，是离太阳远近的第五颗行星，而且是八大行星中最大的一颗，比所有其他的行星的合质量大2倍（地球的318倍）。木星绕太阳公转的周期为4332.589天，约合11.86年。木星(a.k.a. Jove)希腊人称之为 宙斯(众神之王，奥林匹斯山的统治者和罗马国的保护人，它是Cronus（土星的儿子。)木星是天空中第四亮的物体（次于太阳，月球和金星；有时候火星更亮一些），早在史前木星就已被人类所知晓。根据伽利略1610年对木星四颗卫星：木卫一，木卫二，木卫三和木卫四（现常被称作伽利略卫星）的观察，它们是不以地球为中心运转的第一个发现，也是赞同哥白尼的日心说的有关行星运动的主要依据。</p>
	<p class="title2">土星</p>
	<img src="ba6.jpg" class="pic2">
	<p class="content">土星古称镇星或填星,因为土星公转周期大约为29.5年,我国古代有28宿,土星几乎是每年在一个宿中,有镇住或填满该宿的意味,所以称为镇星或填星，直径119300公里（为地球的9.5倍），是太阳系第二大行星。它与邻居木星十分相像，表面也是液态氢和氦的海洋，上方同样覆盖着厚厚的云层。土星上狂风肆虐，沿东西方向的风速可超过每小时1600公里。土星上空的云层就是这些狂风造成的，云层中含有大量的结晶氨。轨道距太阳142，940万千米，公转周期为10759.5天，相当于29.5个地球年，视星等为0.67等。在太阳系的行星中，土星的光环最惹人注目，它使土星看上去就像戴着一顶漂亮的大草帽。观测表明构成光环的物质是碎冰块、岩石块、尘埃、颗粒等，它们排列成一系列的圆圈，绕着土星旋转。
</p>
	<p class="title1">天王星</p>
	<img src="ba7.jpg" class="pic1">
	<p class="content">天王星是太阳系中离太阳第七远行星，从直径来看，是太阳系中第三大行星。天王星的体积比海王星大，质量却比其小。天王星是由威廉·赫歇耳通过望远镜系统地搜寻，在1781年3月13日发现的，它是现代发现的第一颗行星。事实上，它曾经被观测到许多次，只不过当时被误认为是另一颗恒星（早在1690年John Flamsteed便已观测到它的存在，但当时却把它编为34 Tauri）。赫歇耳把它命名为"the Georgium Sidus（天竺葵）"（乔治亚行星）来纪念他的资助者，那个对美国人而言臭名昭著的英国国王：乔治三世；其他人却称天王星为“赫歇耳”。由于其他行星的名字都取自希腊神话，因此为保持一致，由波德首先提出把它称为“乌拉诺斯(Uranus)”（天王星），但直到1850年才开始广泛使用。</p>
	<p class="title2">海王星</p>
	<img src="ba8.jpg" class="pic2">
	<p class="content">海王星（Neptune）是环绕太阳运行的第八颗行星，也是太阳系中第四大天体（直径上）。海王星在直径上小于天王星，但质量比它大。在天王星被发现后，人们注意到它的轨道与根据牛顿理论所推知的并不一致。因此科学家们预测存在着另一颗遥远的行星从而影响了天王星的轨道。Galle和 d'Arrest在1846年9月23日首次观察到海王星，它出现的地点非常靠近于亚当斯和勒威耶根据所观察到的木星、土星和天王星的位置经过计算独立预测出的地点。一场关于谁先发现海王星和谁享有对此命名的权利的国际性争论产生于英国与法国之间（然而，亚当斯和勒威耶个人之间并未有明显的争论）；现在将海王星的发现共同归功于他们两人。后来的观察显示亚当斯和勒威耶计算出的轨道与海王星真实的轨道偏差相当大。如果对海王星的搜寻早几年或晚几年进行的话，人们将无法在他们预测的位置或其附近找到它。</p>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190107214344822.jpg)![](https://img-blog.csdnimg.cn/20190107214354126.jpg)![](https://img-blog.csdnimg.cn/20190107214402853.jpg)![](https://img-blog.csdnimg.cn/20190107214420748.jpg)![](https://img-blog.csdnimg.cn/20190107214428873.jpg)![](https://img-blog.csdnimg.cn/20190107214436770.jpg)![](https://img-blog.csdnimg.cn/20190107214444878.jpg)![](https://img-blog.csdnimg.cn/20190107214454891.jpg)![](https://img-blog.csdnimg.cn/20190107214503344.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
