# Nivoslider：丰富的图片切换效果 - z69183787的专栏 - CSDN博客
2012年11月02日 11:59:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：848
Nivoslider是一款基于jQuery的，能实现多种切换效果的图片切换插件。Nivoslider提供了11种切换效果，支持多种参数配置和方法函数调用，使用简单，是一款非常理想的图片切换插件。
![](http://www.helloweba.com/attachments/fck/nivoslider.jpg)
![查看演示DEMO](http://www.helloweba.com/images/demo_btn.jpg)![下载源码](http://www.helloweba.com/images/download_btn.jpg)
#### XHTML
```
<div id="slider" class="nivoSlider"> 
    <img src="images/s1.jpg" alt="" title="好漂亮的狗狗..."  /> 
    <a href="http://www.helloweba.com"><img src="images/s2.jpg" alt="" title="图片标题" /></a> 
    <img src="images/s3.jpg" alt="" title="#htmlcaption" /> 
</div> 
<div id="htmlcaption" class="nivo-html-caption"> 
    <strong>图片标题</strong>，支持HTML标签 <a href="http://www.helloweba.com">helloweba.com</a>. 
</div>
```
在ID为slider的DIV中，放置了三张需要切换的图片。如果你想给让单击图片时打开一个页面地址，就给图片加个<a>标签链接，如果你想给图片加个标题说明，可以使用title属性，如果你想让标题有样式或者链接，可以将图片的title属性设置为像"#htmlcaption"，即#号后面紧跟指向的ID，如上所示代码，在ID为htmlcaption的DIV中，你可以编辑你想要的HTML代码，Nivoslider插件支持html的图片标题。
#### CSS
首先需要载入插件所需的CSS文件。
```
<link rel="stylesheet" type="text/css" href="nivo-slider.css" />
```
然后设置展示插件所需的样式，像控制导航键，导航按钮样式，当然你可以自己写个样式，代码参照如下：
```
#slider{width:680px; height:272px; margin:40px auto;background:url(images/loading.gif) 
 no-repeat 50% 50%;} 
#slider img {position:absolute;top:0px;left:0px;display:none;} 
.nivo-controlNav{position:absolute;left:300px;bottom:-42px;} 
.nivo-controlNav a {display:block;width:22px;height:22px;background:url(images/bullets.png) 
 no-repeat;text-indent:-9999px;border:0;margin-right:3px;float:left;} 
.nivo-controlNav a.active {background-position:0 -22px;} 
.nivo-directionNav a {display:block;width:30px;height:30px;background:url(images/arrows.png) 
 no-repeat;text-indent:-9999px;border:0;} 
a.nivo-nextNav {background-position:-30px 0;right:15px;} 
a.nivo-prevNav {left:15px;} 
.nivo-caption {text-shadow:none; font-family: Helvetica, Arial, sans-serif;} 
.nivo-caption a {color:#efe9d1;text-decoration:underline;}
```
#### jQuery
先载入jQuery库和插件。
```
<script type="text/javascript" src="js/jquery.js"></script> 
<script type="text/javascript" src="js/jquery.nivo.slider.pack.js"></script>
```
然后调用Nivoslider插件。
```
$(function() { 
     $('#slider').nivoSlider(); 
});
```
这样，一个非常漂亮的图片切换效果就完成了。如果你需要特别的要求可以修改插件的配置参数和方法函数。
#### Nivoslider插件参数和方法一览表
|参数/方法|描述|默认值|
|----|----|----|
|基本| | |
|effect|图片切换效果。提供了11种效果：'sliceDownRight','sliceDownLeft','sliceUpRight','sliceUpLeft', 'sliceUpDown','sliceUpDownLeft', 'fold','fade','slideInRight','slideInLeft'， 诸如渐入渐出，滑动，百叶窗等等都有。|random|
|animSpeed|图片切换速度（毫秒）|500|
|pauseTime|图片停留时间（毫秒）|3000|
|startSlide|开始切换的位置（即从第几张图开始切换）。|0|
|directionNav|是否使用左右按钮导航|true|
|directionNavHide|是否当鼠标滑上图片时出现左右导航按钮|true|
|controlNav|是否使用导航控制条，即显示每张图片的按钮|true|
|controlNavThumbs|是否使用缩略图控制导航|false|
|controlNavThumbsFromRel|是否使用图片的rel属性关联缩略图|false|
|controlNavThumbsSearch|缩略图类型|'.jpg'|
|controlNavThumbsReplace|缩略图后缀|'_thumb.jpg'|
|keyboardNav|是否支持键盘方向键切换（貌视IE不支持）|true|
|pauseOnHover|当鼠标滑向图片时，停止切换|true|
|manualAdvance|是否不自动切换，当为true时，需要手动切换|false|
|captionOpacity|设置图片标题说明的背景透明度|0.8|
|方法| | |
|beforeChange|当发生切换前，回调函数||
|afterChange|当发生切换后，回调函数||
|slideshowEnd|完成所有的切换动作后，回调函数||
|lastSlide|切换最后一张图片时，回调函数||
|afterLoad|当加载完成时，回调函数||
了解更多信息，请访问Nivoslider插件网站[http://nivo.dev7studios.com/](http://nivo.dev7studios.com/)
