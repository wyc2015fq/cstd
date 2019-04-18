# CSS中position属性的说明 - z69183787的专栏 - CSDN博客
2012年10月22日 14:54:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：667标签：[css																[class																[html																[文档](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[前端-Css](https://blog.csdn.net/z69183787/article/category/2175041)
## relative
相对定位是一个非常容易掌握的概念。如果对一个元素进行相对定位，它将出现在它所在的位置上。然后，可以通过设置垂直或水平位置，让这个元素“相对于”它的起点进行移动。
```
<!--相对定位!-->
<h2>relative</h2>
<p>相对定位是一个非常容易掌握的概念。如果对一个元素进行相对定位，它将出现在它所在的位置上。然后，可以通过设置垂直或水平位置，让这个元素“相对于”它的起点进行移动。</p>
<div class="parent">
<div>child 1</div>
<div style="position:relative;left:20px;top:20px;">child 2</div>
<div>child 3</div>
</div>
```
child 1
child 2
child 3
## absolute
绝对定位的元素的位置相对于最近的已定位祖先元素，如果元素没有已定位的祖先元素，那么它的位置相对于最初的包含块。 对于定位的主要问题是要记住每种定位的意义。
绝对定位是“相对于”最近的已定位祖先元素，如果不存在已定位的祖先元素，那么“相对于”最初的包含块。所以如果要设定元素与其父元素的绝对位置定位就必须设定父元素的定位。
注释：根据用户代理的不同，最初的包含块可能是画布或 HTML 元素。
```
<!--绝对定位!-->
<h2>absolute</h2>
<p>绝对定位的元素的位置相对于最近的已定位祖先元素，如果元素没有已定位的祖先元素，那么它的位置相对于最初的包含块。
对于定位的主要问题是要记住每种定位的意义。</p>
<p>绝对定位是“相对于”最近的已定位祖先元素，如果不存在已定位的祖先元素，那么“相对于”最初的包含块。所以如果要设定元素与其父元素的绝对位置定位就必须设定父元素的定位。</p>
<p>注释：根据用户代理的不同，最初的包含块可能是画布或 HTML 元素。</p>
<div class="parent" style="position:relative;"><!--如果该处不定位，那么child5框的定位是相对于最初的包含块!-->
<div>child 4</div>
<div style="position:absolute;left:50px;top:50px;">child 5</div>
<div>child 6</div>
</div>
```
child 4
child 5
child 6
## fixed
元素框的表现类似于将 position 设置为 absolute，不过其包含块是视窗本身。
```
<h2>fixed</h2>
<p>元素框的表现类似于将 position 设置为 absolute，不过其包含块是视窗本身。</p>
<div class="parent">
<div>child 7</div>
<div style="position:fixed;right:20px;top:20px;">child 8</div>
<div>child 9</div>
</div>
```
child 7
child 8
child 9
## static
元素框正常生成。块级元素生成一个矩形框，作为文档流的一部分，行内元素则会创建一个或多个行框，置于其父元素中。
```
<h2>static</h2>
<p>元素框正常生成。块级元素生成一个矩形框，作为文档流的一部分，行内元素则会创建一个或多个行框，置于其父元素中。</p>
<div class="parent">
<div>child 10</div>
<div style="position:static;right:20px;top:20px;">child 11</div>
<div>child 12</div>
</div>
```
child 10
child 11
child 12
