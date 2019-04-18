# CSS :after 伪元素 - z69183787的专栏 - CSDN博客
2014年02月08日 10:00:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1020
CSS :after 伪元素定义和用法
:after 伪元素在元素之后添加内容。
这个伪元素允许创作人员在元素内容的最后面插入生成内容。默认地，这个伪元素是行内元素，
不过可以使用属性 display 改变这一点。
实例
此样式会在每个 h1 元素之后播放一段声音：
h1:after { content:url(beep.wav); } <style type="text/css">
h1:after {content:url(logo.gif)}
</style>在显示时，标题内容后会插入一张图片。这就是伪元素:after的作用。
伪元素:after另外一个常用的作用在原来的文章中曾经提到过，有些同学可能还记得，那就是清除浮动。
在CSS中加入带有CSS伪类:after的内容：
.clear:after {
height:0;
content:".";
clear:both;
display:block;
visibility:hidden;
}
然后在外面的Div容器box中引用这个class，比如：
<div id="box"class="clear">
……
……
</div>
:after 伪元素在元素之后添加内容。
这个伪元素允许创作人员在元素内容的最后面插入生成内容。默认地，这个伪元素是行内元素，不过可以使用属性 display 改变这一点。
.clearfix:after {       <----在类名为“clearfix”的元素内最后面加入内容；
content: ".";     <----内容为“.”就是一个英文的句号而已。也可以不写。
display: block;   <----加入的这个元素转换为块级元素。
clear: both;     <----清除左右两边浮动。
visibility: hidden;      <----可见度设为隐藏。注意它和display:none;是有区别的。visibility:hidden;仍然占据空间，只是看不到而已；
line-height: 0;    <----行高为0；
height: 0;     <----高度为0；
font-size:0;    <----字体大小为0；
}
.clearfix { *zoom:1;}   <----这是针对于IE6的，因为IE6不支持:after伪类，这个神奇的zoom:1让IE6的元素可以清除浮动来包裹内部元素。具体意思的话，不用深究，听说微软的工程师自己都无法解释清楚。height:1%效果也是一样。
整段代码就相当于在浮动元素后面跟了个宽高为0的空div，然后设定它clear:both来达到清除浮动的效果。
之所以用它，是因为，你不必在html文件中写入大量无意义的空标签，又能清除浮动。
