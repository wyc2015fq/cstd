
# jQuery使用之（五）处理页面的事件 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月20日 11:44:53[Jlins](https://me.csdn.net/dyllove98)阅读数：1487


在之前dom操作中提到了javascript对事件处理的介绍。由于不同浏览器处理事件各不相相同，这给开发者带来了不必要的麻烦，jQuery的方便的解决了这个方面的麻烦。
1.绑定事件监听
（[http://www.cnblogs.com/ahthw/p/4213521.html](http://www.cnblogs.com/ahthw/p/4213521.html)）对事件的监听做了详细的介绍，看到了iE和DOM标准浏览器对待事件监听的区别，并对多个监听事件执行顺序和方式也不相同。
在jQuery中，通过bind()对事件绑定，相当于IE浏览器的attachEvent()和标准DOM的addEventListener()。如下例子：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("img")
                    .bind("click",function() {
                        $("\#show").append("<div>点击事件1</div>");
                    })
                    .bind("click",function() {
                        $("\#show").append("<div>点击事件2</div>");
                    })
                    .bind("click",function() {
                        $("\#show").append("<div>点击事件3</div>");
                    });
            });</script><imgsrc="11.jpg"><divid="show"></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码对img绑定了三个click监听事件.
bind()通用语法为
bind(eventType,[data],Listener)
其中，eventType为事件的类型，可以是blur/focus/load/resize/scroll/unload/click/dblclick/onmousedown/mouseup/onmouseover/onmouseover/onmouseout/mouseenter/onmouseleave/change/select/submit/onkeydown/keypress/keyup/error
data为可选参数，用来传递一些特殊的数据供监听函数使用。而listener为事件监听函数，上面的例子中使用匿名函数
对于多个事件类型，如果希望使用同一个监听函数，可以同事添加在eventType中，事件之间使用空格分离。
$(function() {
                $("p").bind("mouseenter mouseleave", function() {
                    $(this).toggleClass("over")
                })
            });
另外一些特殊的事件类型可以直接利用事件名称作为绑定函数，接受参数为监听函数。例如之前反复使用的
$("p").click(function(){
                //添加click事件监听函数
            })
其中，通用语法为
eventTypeName(fn)
可以使用的eventTypeName包括
blur/focus/load/resize/scroll/unload/click/dblclick/onmousedown/onmouseup/mousemove/mouseover/mouseout/change/select/submit/
keydown/keypress/keyup/error等
除了bind()外，jQuery还提供了一个很实用的one()方法来绑定事件。该方法绑定回见出发了一次后就会自动删除，不再生效。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//首先创建10个<div>块
            for (var i = 0; i<10; i++)
                $(document.body).append($("<div>Click<br>Me!</div>"));
            var iCounter = 1;
             //每个都用one添加click事件
            $("div").one("click", function() {
                $(this).css({
                    background: "\#8f0000",
                    color: "\#FFFFFF"
                }).html("Clicked!<br>" + (iCounter++));
            });![复制代码](http://common.cnblogs.com/images/copycode.gif)
例如上例，创建10个div,给每个div绑定一个函数事件，当点击div块时，函数执行一次便不再执行。
2.移除事件监听
jQuery采用unbind()来移除事件，该方法可以接受两个可选函数，也可不设置任何参数，例如下面的代码表示移除div标记的所有事件和P标记的所有click事件。
$("p").unbind("click");
            $("div").unbind();
如果希望移除某个指定的事件，则必须使用unbind(eventType,listener)方法的第二个参数，例如：
var myFunc = function() {
                //监听函数体
            };
            $("p").bind("click",myFunc);
            $("p").unbind("click",myFunc);
例如以下代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {varfnMyFunc1;//函数变量$("img")
                    .bind("click", fnMyFunc1=function() {//赋给函数变量$("\#show").append("<div>点击事件1</div>");
                    })
                    .bind("click",function() {
                        $("\#show").append("<div>点击事件2</div>");
                    })
                    .bind("click",function() {
                        $("\#show").append("<div>点击事件3</div>");
                    });
                $("input[type=button]").click(function() {
                    $("img").unbind("click", fnMyFunc1);//移除事件监听myFunc1});
            });</script><imgsrc="11.jpg"><inputtype="button"value="移除事件1"><divid="show"></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
例如以上代码，添加了fnMyFunc1函数bind()绑定时将匿名函数赋值给他，从而作为unbind()函数调用名称。
3.传递事件对象。
http://www.cnblogs.com/ahthw/p/4216017.html介绍了对象的概念，并且分析了事件对象常用的属性和方法，可以看到事件对象在不同的浏览器之间存在很多区别，jQuery中没时间对象是通过唯一的方法传递给事件监听函数的。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("p").bind("click",function(e) {//传递事件对象evarsPosPage="("+e.pageX+","+e.pageY+")";varsPosScreen="("+e.screenX+","+e.screenY+")";
                    $("span").html("<br>Page:"+sPosPage+"<br>Screen:"+sPosScreen);
                });
            });</script><p>点击此处</p><spanid=""></span>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上的代码给p绑定了鼠标click事件监听函数，并将事件对象作为参数传递，从而获取了鼠标事件触发点的坐标值。
对于事件的属性和方法，jQuery最重要的工作就是替开发者解决了兼容性问题，常用的属性和方法
|属性|说明|
|altKey|按下alt键为ture，否则为false|
|ctrlKey|按下ctrl键为ture，否则为false|
|shiftKey|按下shift键为ture，否则为false|
|keyCode|对于keyup和keydown事件，返回按键的值（即a和A的值是一样的，都是65）|
|pageX，pageY|鼠标在客户端的位置，不包括工具栏，滚动条等|
|relateTarget|鼠标事件中，鼠标指针进入或离开元素。
|
|screenX,screenY|鼠标在整个屏幕的位置。|
|target|引起事件的元素/对象|
|type|事件的名称，如click,mouseover等|
|which|键盘事件中为按键的unicode值，鼠标按键中代表鼠标按键（1左键 2中键 3为右键）|
|stopPropagation()|阻止事件向上冒泡。|
|preventDefault()|阻止事件默认行为|
|
4.事件的触发
有些时候希望用户在没有任何操作的情况下来触发事件。比如在打开页面后自动单击一次运行监听函数。希望单击一个按钮时其它按钮也同事被单击等。jQuery土工了tigger(eventTepe)来实现事件的触发，其中参数ebentType为合法的事件类型，例如click,submit等
例如下面例子：有两个按钮，分别都有自己的事件监听函数，当单击按钮1时运行自己的监听函数，单击按钮2时除了运行自己的监听函数，还运行了按钮1的监听函数，仿佛按钮1也被单击了
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">functionCounter(oSpan) {variNum=parseInt(oSpan.text());//获取span中本身的值oSpan.text(iNum+1);//点击次数加1}
            $(function() {
                $("input:eq(0)").click(function() {
                    Counter($("span:first"));
                });
                $("input:eq(1)").click(function() {
                    Counter($("span:last"));
                    $("input:eq(0)").trigger("click");//触发按钮1的点击事件});
            });</script><inputtype="button"value="Button 1"><inputtype="button"value="Button 2"><br><br><div>按钮1点击次数：<span>0</span></div><div>按钮2点击次数：<span>0</span></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
对于特殊的事件类型 ，例如blur,change,click,focus,select,submit等，还可以直接以事件名称作为触发函数。以上触发按钮1的语句等于： $("input:eq(0)").click();

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


