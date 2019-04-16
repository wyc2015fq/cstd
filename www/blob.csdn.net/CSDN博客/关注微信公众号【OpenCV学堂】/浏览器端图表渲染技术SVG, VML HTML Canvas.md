# 浏览器端图表渲染技术SVG, VML HTML Canvas - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年08月27日 22:29:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：7574








最近一直在研究浏览器端的图形图表绘制技术，从种类上可以分为两种：

一类是矢量图形绘制API，典型代表是微软的VML与W3C主推的SVG

另外一类是基于像素的绘制API，典型代表是HTML5 Canvas



背景介绍：

VML是微软推出的基于IE的矢量图形绘制API，注意VML只能在IE中支持，而SVG是W3C

推出的矢量图型API，显然IE不待见它，所以只能在非IE的浏览器上支持，IE上要想显示

SVG格式的图形，必须额外安装插件。

HTML5 Canvas是W3C推出的新一代浏览器端绘图API，微软从IE9+开始支持HTML5

Canvas渲染技术，其它主流浏览器都支持该技术。



技术优缺点：

VML是基于XML方式来组织绘制对象，支持相对与绝对定位方式，同时支持group元素，

可以将一组元素对象绘制在一个group元素上，group上默认支持绝对定位方式绘制图形

VML技术绘制的图形每个都是元素对象，可以任意添加鼠标监听事件，而且每个元素对

象内置支持tooltip功能，但是VML本身不支持动画。不支持单独的文字绘制，文字绘制

必须依附其它顶级的VML 元素。同时微软在IE8时候的改动，导致VML在不同的IE浏览

器版本上使用略有不同。一个最常用的支持VML调用JavaScript脚本如下：

**var** e=["shape","shapetype","group","background","path","formulas",

"handles","fill","stroke","shadow","textbox","textpath","imagedata","line",

"polyline","curve","roundrect","oval","rect","arc","image"],

s=document.createStyleSheet(); 

**for**(**var** i=0; i<e.length;i++) {

s.addRule("v\\:"+e[i],"behavior: url(#default#VML);");}

document.namespaces.add("v","urn:schemas-microsoft-com:vml");

}

VML支持2D与3D渲染技术，可以绘制出效果非常好的图行。当前支持VML技术绘制的

开源的前端图表库有很多，可能大家最熟悉就是JQuery的图表库与Highchart的图表库



SVG技术与VML技术都是提供矢量图形绘制元素，绘制图形上每个元素都支持鼠标监听

SVG比VML做的好地方就是文本绘制，直接支持文本对象，不得不说VML的文本绘制太

TMD的变态。SVG也是基于XML文本描述的，这点跟VML非常相似。值得称赞的是SVG

的JavaScript调用非常的容易与简洁。同样IE6～IE8都不支持SVG渲染与显示，解决办

法就自己装个adobe的SVG Viewer插件，IE9+之后版本支持SVG，所以说IE才是最坑

爹的浏览器，而且是在中国使用人数最多的浏览器，这都拜微软捆绑销售所赐。



HTML5 Canvas是W3C推出来用来替代SVG的前端图形技术，Canvas是基于像素的渲

染技术实现，所以它跟SVG与VML相比，放大与缩小很麻烦，位图大家都懂。但是

Canvas相比VML与SVG还是有很多优点，第一个废除了每个绘制元素都是对象的做法，

浪费资源啊！支持操作图像像素，可以支持Base64编码，在前端就可以保存为图像，

这个都是SVG与VML做不到的啊，Canvas借鉴编程语言图形库的设计思想，对有图形

开发经验的人掌握起来非常容易。而VML与SVG是基于XML的设计，只能通过

JavaScript来操作元素，只有对每个元素指定一个ID，你才可以获取到它，而Canvas支

持擦除与重绘制非常的方便。



最后想瞎掰一下那些开源的前端图表库

名头最吓人的应该是Google推出的google chart开源前端图表库，据它自己说基于

HTML5与SVG技术，在老版本的IE上使用VML技术。

功能最强大的应该是HighChart图表库，官方说它们是基于SVG与VML技术，下一

个版本将融入HTML5技术

canvasXpress自己说是完全标准的HTML5技术实现的图表库，但是如果想在IE9之

下版本支持必须额外包括FlashCanvas 或者ExplorerCanvas,而且还说IE上面运行很

慢，建议IE上使用时安装Google iframe框架。

RGraph自己说是完全HTML5技术实现，实际上是VML 与HTML Canvas混合实现，

支持大多数主流浏览器。



这些都国外的，国内淘宝有个开源的前端图表库，但是支持的功能实在有限，通用性

有待提高。



SVG, VML, HTML5 Canvas三种前端图形渲染技术，没有一种是可以支持全部浏览

器的，这个就给实际项目中使用带来不变，一般都会借助开源的图表框架实现跨浏览

器支持。

**转载请注明-gloomyfish**



