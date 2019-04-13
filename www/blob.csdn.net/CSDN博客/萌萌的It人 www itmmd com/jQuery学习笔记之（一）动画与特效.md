
# jQuery学习笔记之（一）动画与特效 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月21日 13:07:17[Jlins](https://me.csdn.net/dyllove98)阅读数：1943


本节主要降级和学习jQuery的自动显隐，渐入渐出、飞入飞出。自定义动画等。
1.显示和隐藏hide()和show()
对于动画来说，显示和隐藏是最基本的效果之一，本节简单介绍jQuery的显示和隐藏。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input:first").click(function() {
                    $("p").hide();//隐藏});
                $("input:last").click(function() {
                    $("p").show();//显示});
            });</script><inputtype="button"value="Hide"><inputtype="button"value="Show"><p>点击按钮，看看效果</p><div><em>本节主要降级和学习jQuery的自动显隐，渐入渐出、飞入飞出。自定义动画等。 1.显示和隐藏hide()和show() 对于动画来说，显示和隐藏是最基本的效果之一，本节简单介绍jQuery的显示和隐藏。</em></div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上是对hide()和show()函数的测试。
2.使用show()、hide()和toggle()方法
上个例子对show()和hide()方法做了简单介绍，其实这两个方法可以接受参数控制显隐藏过程。
语法如下
show(duration,[callback]);
hide(duration,[callback]);
其中，duration表示动画执行时间的长短，可以表示速度的字符串，包括slow,normal,fast.也可以是表示时间的整数(毫秒)。callback是可选的回调函数。在动画完成之后执行。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input:first").click(function() {
                    $("p").hide(300);//隐藏});
                $("input:last").click(function() {
                    $("p").show(500);//显示});
            });</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
例子和第一个例子相同，只是对hide()和show()增加了时间参数。其实toogle()也可以加入事件参数。
2.使用fadeIn()和fadeOut()方式
对于动画效果显隐，jQuery还提供了fadeIn()个fadeOut这两个实用的方法，他们的动画效果类似褪色，语法与slow()和hide()完全相同。
fadeIn(duration, [callback]);
            fadeOut(duration, [callback]);
例子
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input:eq(0)").click(function() {
                    $("img").fadeOut(3000);//逐渐fadeOut});
                $("input:eq(1)").click(function() {
                    $("img").fadeIn(1000);//逐渐fadeIn});
            });</script><imgsrc="http://study.ahthw.com/gtimg/book/2014/11/201411120129479688.jpg"><inputtype="button"value="Hide"><inputtype="button"value="Show">![复制代码](http://common.cnblogs.com/images/copycode.gif)
fadeTo()方法的使用。
fadeTo() 方法将被选元素的不透明度逐渐地改变为指定的值。
例子：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input:eq(0)").click(function() {
                    $("img").fadeOut(1000);
                });
                $("input:eq(1)").click(function() {
                    $("img").fadeIn(1000);
                });
                $("input:eq(2)").click(function() {
                    $("img").fadeTo(1000,0.5);
                });
                $("input:eq(3)").click(function() {
                    $("img").fadeTo(1000,0);
                });
            });</script><p><imgsrc="03.jpg"></p><inputtype="button"value="FadeOut"><inputtype="button"value="FadeIn"><inputtype="button"value="FadeTo 0.5"><inputtype="button"value="FadeTo 0">![复制代码](http://common.cnblogs.com/images/copycode.gif)
fadeOut相关参数
|speed|可选。规定元素从当前透明度到指定透明度的速度。
|可能的值：
|毫秒 （比如 1500）
|"slow"
|"normal"
|"fast"
|
|opacity|必需。规定要淡入或淡出的透明度。必须是介于 0.00 与 1.00 之间的数字。|
|callback|可选。fadeTo 函数执行完之后，要执行的函数。
|如需学习更多有关 callback 的内容，请访问我们的 jQuery Callback 这一章。
|除非设置了 speed 参数，否则不能设置该参数。
|
3.幻灯片slideUp和slideDown效果
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input:eq(0)").click(function() {
                    $("div").add("img").slideUp(1000);
                });
                $("input:eq(1)").click(function() {
                    $("div").add("img").slideDown(1000);
                });
                $("input:eq(2)").click(function() {
                    $("div").add("img").hide(1000);
                });
                $("input:eq(3)").click(function() {
                    $("div").add("img").show(1000);
                });
            });</script><inputtype="button"value="SlideUp"><inputtype="button"value="SlideDown"><inputtype="button"value="Hide"><inputtype="button"value="Show"><br><div></div><imgsrc="04.jpg">![复制代码](http://common.cnblogs.com/images/copycode.gif)
前面提到了几种动画效果，jQuery还提供了slideUp()和slideDown()来模拟PPT中的类似幻灯片拉帘效果，它与slow()和hide()完全相同。
以上代码定义了一个div和一个img,用add方法组合在一起。
4.自定义动画
考虑到框架的通用性及代码文件的大小，jQuery不能涵盖所有的动画效果，但它提供了animate()方法，能够使开发者自定义动画。本节主要通过介绍animate()方法的两种形式及应用。
animate()方法给开发者很大的空间。它一共有两种形式。第一种形式比较常用。用法如下
animate(params,[duration],[easing],[callback])
其中params为希望进行变幻的css属性列表，以及希望变化到的最终值，duration为可选项，与show()/hide()的参数含义完全相同。easing为可选参数，通常供动画插件使用。 用来控制节奏的变化过程。jQuery中只提供了linear和swing两个值.callback为可选的回调函数。在动画完成后触发。
需要注意。params中的变量遵循camel命名方式。例如paddingLeft不能写成padding-left.另外，params只能是css中用数值表示的属性。例如width.top.opacity等
像backgroundColor这样的属性不被animate支持。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<style>div{background-color:\#FFFF00;height:40px;width:80px;border:1px solid \#000000;margin-top:5px;padding:5px;text-align:center;}</style><scripttype="text/javascript">$(function() {
                $("button").click(function() {
                    $("\#block").animate({
                        opacity:"0.5",
                        width:"80%",
                        height:"100px",
                        borderWidth:"5px",
                        fontSize:"30px",
                        marginTop:"40px",
                        marginLeft:"20px"},2000);
                });
            });</script><buttonid="go">Go>></button><divid="block">动画！</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
在params中，jQuery还可以用“+=”或者"-="来表示相对变化。如
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<style>div{background-color:\#FFFF00;height:40px;width:80px;border:1px solid \#000000;margin-top:5px;padding:5px;text-align:center;position:absolute;}</style><scripttype="text/javascript">$(function() {
                $("button:first").click(function() {
                    $("\#block").animate({
                        left:"-=80px"//相对左移},300);
                });
                $("button:last").click(function() {
                    $("\#block").animate({
                        left:"+=80px"//相对右移},300);
                });
            });</script><button>Go>></button><button>Go>></button><divid="block">动画！</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
先将div进行绝对定位，然后使用animate()中的-=和+=分别实现相对左移和相对右移。
animate()方法还有另外一种形式，如下所示：
animate(params,options)
其中，params与第一种形式完全相同，options为动画可选参数列表，主要包括duration,esaing，callback,queue等，其中duration.easing.callback与第一种形式完全一样，queue为布尔值，表示当有多个 animate()组成jQuery时，当前animate()紧接这下一个animate(),是按顺序执行(true)还是同时触发false
如下例子，展示了animate()第二种用法。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<style>div{background-color:\#FFFF22;width:100px;text-align:center;border:2px solid \#000000;margin:3px;font-size:13px;font-family:Arial, Helvetica, sans-serif;}input{border:1px solid \#000033;}</style><scripttype="text/javascript">$(function() {
                $("input:eq(0)").click(function() {//第一个animate与第二个animate同时执行，然后再执行第三个$("\#block1").animate({
                            width:"90%"}, {
                            queue:false,
                            duration:1500})
                        .animate({
                            fontSize:"24px"},1000)
                        .animate({
                            borderRightWidth:"20px"},1000);
                });
                $("input:eq(1)").click(function() {//依次执行三个animate$("\#block2").animate({
                            width:"90%"},1500)
                        .animate({
                            fontSize:"24px"},1000)
                        .animate({
                            borderRightWidth:"20px"},1000);
                });
                $("input:eq(2)").click(function() {
                    $("input:eq(0)").click();
                    $("input:eq(1)").click();
                });
                $("input:eq(3)").click(function() {//恢复默认设置$("div").css({
                        width:"",
                        fontSize:"",
                        borderWidth:""});
                });
            });</script><inputtype="button"id="go1"value="Block1动画"><inputtype="button"id="go2"value="Block2动画"><inputtype="button"id="go3"value="同时动画"><inputtype="button"id="go4"value="重置"><divid="block1">Block1</div><divid="block2">Block2</div>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上两个div块同时运用了三个动画效果，其中第一个div快的第一个动画添加了queue:false参数，使得前两项两个动画同时执行。可以通过重置反复测试，熟悉animate()第二种形式。
**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)



