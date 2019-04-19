# div之float,clear特性 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年04月02日 09:42:12[baofenny](https://me.csdn.net/jiaminbao)阅读数：1053
在写HTML代码的时候，发现在Firefox等符合W3C标准的浏览器中，如果有一个DIV作为外部容器，内部的DIV如果设置了float样式，则外部的容器DIV因为内部没有clear，导致不能被撑开。看下面的例子：
HTML4STRICT代码：
- 
<div style="width:200px;border:1px solid red;">
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
</div>
显示的结果如下：
![P_1160709253_0.gif](http://e.hiphotos.baidu.com/album/pic/item/8326cffc1e178a82c539cfc1f703738da977e84e.jpg)
容器DIV没有被撑开
大家可以看到，作为外部容器的边框为红色的DIV，没有被撑开。这是因为内部的DIV因为float:left之后，就丢失了clear:both和display:block的样式，所以外部的DIV不会被撑开。
我们想让外部容器的DIV随着内部DIV增多而增加高度，要怎么解决呢？
以前我都是用这样的方法来解决：
HTML4STRICT代码：
- 
<div style="width:200px;border:1px solid red;">
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="clear:both;"></div>
- 
</div>
显示的结果如下：
![P_1160709321_0.gif](http://d.hiphotos.baidu.com/album/pic/item/4d086e061d950a7b9997fea50bd162d9f2d3c935.jpg)
显示正常了
我们看到，在容器DIV内要显示出来的float:left的所有的DIV之后，我们添加了这样的一个DIV：<div style="clear:both"></div>    。这样，其实就在最后增加了clear的动作。
但是，我总觉得，这么多加一个DIV有点不妥。一是多了一个没有意义的DIV，二是在用dojo做Drag & Drop的时候，由于这个DIV是容器DIV的一个字节点，如果这个节点被移动，则会造成排版上的Bug：如果要显示的蓝框的DIV被移到这个DIV之后，则因为clear:both，它会被强制换一行显示。所以，我一直在寻找更好的解决办法。
昨天在无数次的询问了Google大仙后，我终于找到了***How To Clear Floats Without Structural Markup*** 这篇文章，找到了解决的办法。
首先设置这样的CSS：
CSS代码：
- 
.clearfix:after {
- 
    content: ".";
- 
    display: block;
- 
    height: 0;
- 
    clear: both;
- 
    visibility: hidden;
- 
}
然后，我们再修改原来的HTML代码，让外部的容器DIV来使用这个CSS：
HTML4STRICT代码：
- 
<div style="width:200px;border:1px solid red;" class="clearfix">
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
    <div style="float:left;width:80px;height:80px;border:1px solid blue;">TEST DIV</div>
- 
</div>
在Firefox里测试一下，哈哈，这样做的确很有效，显示正常，而且dojo的 Drag & Drop 也不会有问题了。原来，这个clearfix的CSS使用了after这个伪对象，它将在应用clearfix的元素的结尾添加content中的内容。在这里添加了一个句号"."，并且把它的display设置成block；高度设为0；clear设为both；visibility设为隐藏。这样就达到了撑开容器的目的啦。
但是，在文章中说，Windows IE并不支持这样做。所以要让IE也完美显示，则必须在clearfix这个CSS定义的后面加上一些专门为IE设定的HACK。CSS如下：
CSS代码：
- 
.clearfix:after {
- 
    content: ".";
- 
    display: block;
- 
    height: 0;
- 
    clear: both;
- 
    visibility: hidden;
- 
}
- 
/* Hides from IE-mac \*/
- 
* html .clearfix {height: 1%;}
- 
/* End hide from IE-mac */
因为转义字符"\"，Mac IE浏览器会忽略掉这段Hack，但Windows IE不会，它会应用 * html .clearfix {height: 1%;} 来达到撑开DIV容器的目的（貌似Mac IE没有办法解决这个问题，不过幸好用户数量是在是太少了，Safari支持就可以了:p）。
测试一下，果然大功告成。
![P_1160709321_0.gif](http://a.hiphotos.baidu.com/album/pic/item/7af40ad162d9f2d3f7b3948ea8ec8a136327cc35.jpg)
