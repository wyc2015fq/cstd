# jquery的鼠标滚轮插件 Mousewheel响应跨浏览器的鼠标滚轮事件 - z69183787的专栏 - CSDN博客
2014年02月25日 11:31:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12719
jquery插件默认是不支持鼠标中轮滚轮事件的，现在我们可以用于添加跨浏览器的鼠标滚轮支持可以使用jquery的Mousewheel插件。
使用mousewheel事件有以下两种方式：
使用mousewheel和unmousewheel事件函数；
使用经典的bind和unbind函数。 
**[javascript]**[view
 plain](http://blog.csdn.net/qq542369628/article/details/8471535#)[copy](http://blog.csdn.net/qq542369628/article/details/8471535#)
- $('div.mousewheel_example').mousewheel(fn);    
- $('div.mousewheel_example').bind('mousewheel', fn);    
mousewheel事件的处理函数有一点小小的变化，它除了第一个参数event外，还接收到第二个参数delta。通过参数delta可以获取鼠标滚轮的方向和速度。如果delta的值是负的，那么滚轮就是向下滚动，正的就是向上。
以下是示例的源代码：
**[javascript]**[view
 plain](http://blog.csdn.net/qq542369628/article/details/8471535#)[copy](http://blog.csdn.net/qq542369628/article/details/8471535#)
- jQuery(function($) {    
-     $('div.mousewheel_example')    
-         .bind('mousewheel', function(event, delta) {    
- var dir = delta > 0 ? 'Up' : 'Down',    
-                 vel = Math.abs(delta);    
-             $(this).text(dir + ' at a velocity of ' + vel);    
- returnfalse;    
-         });    
- });    
**jquery的鼠标滚轮插件 Mousewheel下载**
[从GitHub下载其他版本](http://github.com/brandonaaron/jquery-mousewheel/downloads)。
**使用**
要使用这个功能，只需对目标元素的’mousewheel’事件绑定事件处理函数即可。
Mousewheel插件还提供了两个事件函数：mousewheel和unmousewheel，可以对目标元素调用这两个函数，并在参数中指定回调函数。
事件的回调函数第一个参数为event，第二个参数为delta，代表鼠标滚轮的变化值。
以下是上面两种使用方式的示例： 
**[javascript]**[view
 plain](http://blog.csdn.net/qq542369628/article/details/8471535#)[copy](http://blog.csdn.net/qq542369628/article/details/8471535#)
- // 绑定方式  
- $('#my_elem').bind('mousewheel', function(event, delta) {    
-     console.log(delta);    
- });    
- 
- // 事件函数方式  
- $('#my_elem').mousewheel(function(event, delta) {    
-     console.log(delta);    
- });    
- 
