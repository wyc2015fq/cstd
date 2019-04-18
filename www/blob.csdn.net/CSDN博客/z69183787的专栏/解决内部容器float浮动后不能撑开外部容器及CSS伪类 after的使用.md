# 解决内部容器float浮动后不能撑开外部容器及CSS伪类:after的使用 - z69183787的专栏 - CSDN博客
2014年02月08日 09:56:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1617
解决内部容器float浮动后不能撑开外部容器，外部容器塌陷问题
方法一：
直接在内部添加一个空的容器<div style="clear:both;"></div>可以撑开外部容器。
方法二：
.clearfix:after {
    content:".";
    display:block;
    height:0;
    clear:both;
    visibility:hidden;
}
*html .clearfix {
    height:1%;
}
*+html .clearfix {
    height:1%;
}
有点意思，content:"."会在指定了.clearfix样式的容器内追加一个字符，一个英文句号。可想而知，如果样式是.clearfix:before{content:"我想加的内容";……}，那么会在前面插入一个字符串“我想加的内容”。
IE8+ 和其他较新浏览器都支持:after和content，我们可以使用:after+content方法清除浮动造成的塌陷。但IE6和IE7不支 持:after伪类，*、*+是分别解决IE6和IE7下的浮动塌陷问题，当然你也可以使用IE私有的zoom属性，让div远离浮动塌陷问题。代码如 下：
.clearfix {
    *zoom:1;
}
.clearfix:after {
    display:block;
    content:".";
    height:0;
    clear:both;
    overflow:hidden;
    visibility:hidden;
}
需要说明的是：overflow:hidden:的作用只是隐藏元素溢出，visibility:hidden;的作用是隐藏元素但仍然影响布局，元素占有的空间大小还在哦。
另 外，:after伪类+content 可以让大小不固定的图片垂直居中，与清除浮动影响类似，IE6/7不支持:after伪类，我们可以用另外的方法让图片垂直居中，例如font-size 方法，设一个比较大点的字体大小，IE6/7就可以实现图片垂直对齐了，至于其他浏览器，就用:after伪类+content内容生成一个 vertical-align:middle属性的元素就可以了。
CSS：
.pic_box {
    width:300px;
    height:300px;
    background:#999;
    font-size:0;
    *font-size:200px;
    text-align:center;
}
.pic_box img {
    vertical-align:middle;
}
.pic_box:after {
    display:inline-block;
    width:0;
    height:100%;
    content:"center";
    vertical-align:middle;
    overflow:hidden;
}
HTML：
<div class="pic_box">
    <img src="http://blog.163.com/zzjjll_cool/blog/example.jpg">
</div>
目前图片居中的测试结果是所有浏览器都垂直居中，但Chrome浏览器的水平居中貌似左边偏移了点，这是因为Chrome浏览器font-size:0，不能消除空格产生的水平距离的原因。这或许是唯一点瑕疵吧。
