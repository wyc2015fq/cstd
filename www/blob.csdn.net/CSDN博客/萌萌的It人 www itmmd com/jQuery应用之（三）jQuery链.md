
# jQuery应用之（三）jQuery链 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月21日 13:10:53[Jlins](https://me.csdn.net/dyllove98)阅读数：1806


从前文的实例中，我们按到jQuery语句可以链接在一起，这不仅可以缩短代码长度，而且很多时候可以实现特殊的效果。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("div").addClass("css1").filter(function(index) {returnindex==1||$(this).attr("id")=="fourth";
                }).addClass("css2");
            });</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码为整个<div>列表增加样式css1,然后再进行筛选，再为筛选的元素单独增加css2样式。如果不采用jQuery，实现上述的效果将非常麻烦。
在jQuery链中，后面的操作都是以前面的操作结果为对象的，如果希望操作对象为上一步的对象，则可以使用end()方法。
用end()方法来控制jQuery链。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").find("span").addClass("css1").end().addClass("css2");
            });</script><p>Hello,<span>how</span>are you?</p><span>very nice,</span>Thank you.![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码在<p>中搜索<span>标记，然后添加风格css1，利用end()方法将操作对象往回设置为$("p")并添加样式风格css2.
另外，还可以通过andSelf()将前面两个对象进行组合后共同处理。
用andSelf()方法控制jQuery链。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("div").find("p").addClass("css1").andSelf().addClass("css2");
            });</script><div><p>第一段</p><p>第二段</p></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上jQuery代码首先在<div>中搜索<p>标记，添加css1，这个风格只对<p>标记有效，然后利用andSelf()方法将<div>和<p>组合在一起，然后添加样式css2,这个风格对<div>和<p>均有效。
效果：
<divclass="css2"><pclass="css1 css2">第一段</p><pclass="css1 css2">第二段</p></div>


**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


