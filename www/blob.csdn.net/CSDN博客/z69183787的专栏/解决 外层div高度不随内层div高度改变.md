# 解决 外层div高度不随内层div高度改变 - z69183787的专栏 - CSDN博客
2014年01月27日 15:11:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6351
 内层div展示新闻，内容或多或少，因此内层div做成自适应高度。height:auto!important;height:400px;min-height:400px;
外层div也做成自适应高度。
但是展示效果是，内层div高度被拉高时，外层div没有跟随被拉高，导致内部div溢出外层div之外，导致外层div 之外的div进入了内部div。
想到是外层div没有跟随内部div高度变化所致。
搜索有以下结果
### [](http://blog.sina.com.cn/ljshtxx)[IE8下div嵌套时，外层div高度不随内层div高度改变的问题解决](http://blog.sina.com.cn/ljshtxx)
<div id=”a1″>
       <div id=”b1″></div>
        <div id=”b2″></div>
</div>
当b1和b2都是float=left时候，a1层的高度不会被b1和b2的高度撑开。(这个现象只有IE8发生，其他版本IE以及IE8选择兼容模式后就没有问题。)
解决方法：a1的display=table
二. 11 高度不适应
高度不适应是当内层对象的高度发生变化时外层高度不能自动进行调节，特别是当内层对象使用
margin 或paddign 时。例：
<div id=”box”>
<p>p对象中的内容</p>
</div>
CSS：
#box {background-color:#eee; }
#box p {margin-top: 20px;margin-bottom: 20px; text-align:center; }
解决方法：在P对象上下各加2个空的div对象CSS代码：.1{height:0px;overflow:hidden;}或者为DIV加上border属性。
三. 额外标签法
　　这种方法就是向父容器的末尾再插入一个额外的标签，并令其清除浮动（clear）以撑大父容器。这种方法浏览器兼容性好，没有什么问题，缺点就是需要额外的（而且通常是无语义的）标签。我个人不喜欢这种方法，但是它确实是W3C推荐的方法:
**<div style="clear:both;"></div>**
或者使用
**<br style="clear:both;" />**
使用after伪类
　　这种方法就是对父容器使用after伪类和内容声明在指定的现在内容末尾添加新的内容。经常的做法就是添加一个“点”，因为它比较小不太引人注意。然后我们再利用它来清除浮动（闭合浮动元素），并隐藏这个内容。
　　这种方法兼容性一般，但经过各种 hack 也可以应付不同浏览器了，同时又可以保证html 比较干净，所以用得还是比较多的。
复制代码代码如下:
**#outer:after{**
**content:".";**
**height:0;**
**visibility:hidden;**
**display:block;**
**clear:both;**
**}**
设置overflow为hidden或者auto {IE8常见内部浮动外部高度不适应解决方法}
　　这种做法就是将父容器的overflow设为hidden或auot就可以在标准兼容浏览器中闭合浮动元素.
不过使用overflow的时候，可能会对页面表现带来影响，而且这种影响是不确定的，你最好是能在多个浏览器上测试你的页面
**.box:after {     content: ".";     display: block;     height: 0px;     clear: both;     visibility: hidden;}**
然后将父层DIV代码中的<div id="main">改为<div id="main" class="box">。
转载自[http://blog.sina.com.cn/s/blog_4cb8dae20100un07.html](http://blog.sina.com.cn/s/blog_4cb8dae20100un07.html)
按照额外标签法解决问题，新建多余div，放底部，clear:both;
