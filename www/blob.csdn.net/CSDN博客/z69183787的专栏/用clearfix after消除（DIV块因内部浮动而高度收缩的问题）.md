# 用clearfix:after消除（DIV块因内部浮动而高度收缩的问题） - z69183787的专栏 - CSDN博客
2013年02月05日 20:07:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3411
在写HTML代码的时候，创造在Firefox等符合W3C标准的浏览器中，如果有一个DIV作为外部容器，内部的DIV如果设置了float样式，则外部的容器DIV因为内部没有clear，导致不能被撑开。看下面的例子：
<div style="border:2px solid red;">
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">CSSBBS</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
</div>
运行这段代码，大家可以看到，作为外部容器的边框为红色的DIV，没有被撑开。这是因为内部的DIV因为float:left之后，就丧失了clear:both和display:block的样式，所以外部的DIV不会被撑开。
我们想让外部容器的DIV随着内部DIV增多而增加高度，要怎么解决呢？
以前我都是用这样的法子 来解决：
<div style="border:2px solid red;">
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
<div style="clear:both;"></div>
</div>
我们看到，在容器DIV内要显示出来的float:left的所有的DIV之后，我们添加了这样的一个DIV：<div style="clear:both"></div> 。这样，其实就在最后增加了clear的动作。
但是，我总感觉，这么多加一个DIV有点不妥，多了一个没有意义的DIV。所以，我一直在寻找更好的解决措施。
解决的措施：
首先设置这样的CSS：
CSS代码：
##### 代码:
```
.clearfix:after {
content: ".";
display: block;
height: 0;
clear: both;
visibility: hidden;
}
然后，我们再修正原本的HTML代码，让外部的容器DIV来应用这个CSS.
```
``
原本，这个clearfix的CSS应用了after这个伪对象，它将在利用 clearfix的元素的结尾添加content中的内容。在这里添加了一个句号"."，并且把它的display设置成block；高度设为0；clear设为both；visibility设为潜藏 。这样就达到 了撑开容器的目标啦。
但是，在文章中说，Windows IE并不支撑这样做。所以要让IE也完善显示，则必须 在clearfix这个CSS定义的后面加上一些专门为IE设定的HACK。CSS如下：
CSS代码：
##### 代码:
```
.clearfix:after {
content: ".";
display: block;
height: 0;
clear: both;
visibility: hidden;
}
* html .clearfix {height: 1%;}
因为转义字符"\"，Mac IE涉猎器会漠视 掉这段Hack，但Windows IE不会，它会利用 * html .clearfix {height: 1%;} 来达到 撑开DIV容器的目标.
```
.clearfix { *zoom:1;}   <----这是针对于IE6的，因为IE6不支持:after伪类，这个神奇的zoom:1让IE6的元素可以清除浮动来包裹内部元素。具体意思的话，和height:1%效果也是一样。
/* clear */
.clearfix:after {content:".";display:block;height:0;clear:both;visibility:hidden;}
.clearfix {zoom:1;display: inline-block;_height:1px;}
