
# jQuery使用之（三）处理页面的元素 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月19日 15:47:54[Jlins](https://me.csdn.net/dyllove98)阅读数：1033标签：[jquery																](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)个人分类：[jquery																](https://blog.csdn.net/dyllove98/article/category/1349854)



对于页面的元素，在DOM编程中可以通过各种查询、修改手段进行管理，非常麻烦。jQuery提供了一整套的方法来处理页面的元素。包括元素的内容、复制、移动和替换等。本节将介绍一些常用的内容。
1.直接获取、编辑内容。
在jQuery中，主要是通过html()和text()两个方法来获取和编辑页面的内容。其中html()相当于获取节点的innerHTML属性，添加参数html(text)，则为设置innerHtml;而text()则用来获取元素的纯文本，text(content)为设置纯文本。
这两种方法有时配合着使用，text()通过用来过滤页面中的标记，而html(text)用来设置节点中的innerHtml。例如：
$(function() {
                var sString = $("p:first").text(); //获取纯文本
                $("p:last").html(sString);
            });
利用text()的方法获取第一个P的文本，然后用html()方法赋给最后一个<p>。
text()和html()方法的巧用。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").click(function() {varsHtmlStr=$(this).html();//获取innerHTML$(this).text(sHtmlStr);//将代码做为纯文本传入});
            });</script><p><b>文本</b>段 落<em>示</em>例</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
鼠标一次点击、二次点击、三次点击这些可以用在代码获取转移上。
2.移动和复制元素
在普通的DOM中，如果希望在某个元素后面添加一个元素，通常是使用父元素的appendChild()或者inserBefore()方法，很多时候需要反复需找节点的位置。十分麻烦，jQuery中提供了append()方法，可以直接为某个元素添加新的子元素。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {//直接添加HTML代码$("p:last").append("<b>直接添加</b>");
            });</script><p>11<emtitle="huge, gigantic">22</em>...</p><p>33<emtitle="running">44</em>...</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
除了直接添加html代码，append()方法还可以用来添加固定的节点，例如
$("p").append($("a"));
这个情况会有一些不同，倘若添加的<p>是唯一的一个元素，那么$("a")就会被移动到该元素的所有子元素的后面，而如果目标<p>，是多个元素，那么$("a")将会以复制的形式，在每个P中都添加一个子元素，而自身保持不变。例：使用append()方法复制和移动元素。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").append($("a:eq(0)"));//添加目标为多个<p>$("p:eq(1)").append($("a:eq(1)"));//添加目标是唯一的<p>})</script><ahref="\#">链接1</a><ahref="\#">链接2</a><p>文字1</p><p>文字2</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码中设置了两个超链接<a>用于append()调用。对于第1个超链接，添加目标$("p")，一共有两个<p>元素，对于第2个超链接，添加目标是唯一的<p>元素。
可以看到第一个超链接是以复制的形式添加的，第二个超链接则是以移动的方式添加的。
另外从上述可以看出，append()后面的<a>标记被运用了目标<p>的样式风格，同时也是保持了自身的样式风格。这是因为append()是将<a>作为<p>的子标记进行添加的，将<a>放到了<p>的所有子标记（文本）节点后面。
除了append()方法，jQuery还提供了appendTo(target)方法，用来将目标元素添加到指定目标的子元素。它的使用方法和运行结果与append()完全类似。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
$(function() {
                $("img:eq(0)").appendTo($("p")); //添加目标为多个<p>$("img:eq(1)").appendTo($("p:eq(0)")); //添加目标是唯一的<p>});<imgsrc="08.jpg"><imgsrc="09.jpg"><hr><p><imgsrc="10.jpg"></p><p><imgsrc="10.jpg"></p><p><imgsrc="10.jpg"></p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
对于第一幅照片，同事将其添加到3个p标记中，对于第二幅图片则把单独添加到1个P元素中，从执行结果可以看出，第一幅图片是以复制的形式添加到3个P元素中的，而第二幅图片是以移动的方式添加的。
与append()和appendTo()方式对应，JQ还提供了prepend()和prependTo()方法这两种方法是将元素添加到目标的所有子元素之前，也遵循：复制、移动的添加原则。
除了上述的4种方法之外Jq还提供了before()、insertBefore()、after()、和insertAfter()，用来将元素直接添加到某个节点之前或之后，而不是作为子元素插入。
其中before()与insertBefore()完全相同，after()和insertAfter()也是完全一样的，这里以after()为例
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").after($("a:eq(0)"));//添加目标为多个<p>$("p:eq(1)").after($("a:eq(1)"));//添加目标是唯一的<p>});</script><ahref="\#">链接1</a><ahref="\#">链接2</a><p>内容1</p><p>内容2</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码运行结果，可以看到after()方法同样遵循单个目标移动，多个目标复制的原则，并且不再是作为子元素添加。而是紧贴在目标元素之后的兄弟元素。
3.删除元素。
在DOM编程中，要删除某个元素往往借助父元素的removeChild()方法，而jQuery提供了remove()方法，可以直击删除元素。
例如$("p").remove();是删除整个页面中的所有p元素标记。
remove()也接受参数。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").remove(":contains('1')");//$("p:contains('1')").remove();});</script><ahref="\#">链接1</a><ahref="\#">链接2</a><p>内容1</p><p>内容2</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码中remove()使用了过滤选择器，文本内容包含1的P元素被删除掉了。
虽然remove()可以接受参数，但是通常还是建议在选择器阶段就将要删除的对象确定，然后用remove()一次性删除。("p:contains('1')").remove();其效果是完全一样的，并且效果和其它代码的风格是统一的。
在DOM中，如果希望把某个元素的子元素全部删除，往往for循环的配合hasChildNodes()来判断，并用removeChildNode()逐一删除，Jquery提供了empty()方法直接删除所有的子元素。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
            $("p").empty();//删除p的所有子元素});</script><ahref="\#">链接1</a><ahref="\#">链接2</a><p>内容1</p><p>内容2</p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
4.克隆元素。
第二节提到元素的复制和移动，但这取决于目标数的个数，很多时候开发者希望即使目标对象只要一个，同样能执行复制操作。
jQuery提供了clone()方法来完成这项任务。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("img:eq(0)").clone().appendTo($("p"));
                $("img:eq(1)").clone().appendTo($("p:eq(0)"));
            });</script><imgsrc="08.jpg"><imgsrc="09.jpg"><hr><p><imgsrc="10.jpg"></p><p><imgsrc="10.jpg"></p><p><imgsrc="10.jpg"></p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
同样完成了上上节中appendTo()方法达到的结果。
另外clone()函数还接受布尔值对象作为参数，的那个参数为true时，除了克隆本身，它所携带的时间方法将一块被复制。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input[type=button]").click(function() {//克隆自己，并且克隆点击的行为$(this).clone(true).insertAfter(this);
                });
            });</script><inputtype="button"value="Clone Me">![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码在单击按钮时克隆按钮本身，同时克隆单击事件，克隆出来的按钮同样具备有克隆自己的功能。
写博客不容易，欢迎大家给评论以给鼓励，分享是快乐！欢迎大家拍砖和点赞。（JavaScript、ajax、jQuery系列文章不断更新，关注我即可随时关注更新）

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章



