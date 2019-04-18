# jQuery 鼠标滚轮插件 mousewheel - z69183787的专栏 - CSDN博客
2014年02月25日 11:25:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2282
jQuery Mousewheel Plugin,用于添加跨浏览器的鼠标滚轮支持。
mousewheel事件的处理函数有一点小小的变化，它除了第一个参数event 外，还接收到第二个参数delta。
通过参数delta可以获取鼠标滚轮的方向和速度。
如果delta的值是负的即-1，那么滚轮就是向下滚动，正的1就是向上。
官方网址：
https://github.com/brandonaaron/jquery-mousewheel
**插件方法：**
1.为了监听滚轮事件，该插件引入了mousewheel事件，所以我们可以监听元素的mousewheel事件
2.该插件还提供了元素快捷方法`mousewheel和``unmousewheel用于监听和去掉元素对鼠标滚轮事件支持`
例如：
Js代码  ![收藏代码](http://liuna718-163-com.iteye.com/images/icon_star.png)
- // using bind
- $('#my_elem').bind('mousewheel', function(event, delta, deltaX, deltaY) {  
- if (window.console && console.log) {  
-          console.log(delta, deltaX, deltaY);  
-     }  
- });  
- 
- // using the event helper
- $('#my_elem').mousewheel(function(event, delta, deltaX, deltaY) {  
- if (window.console && console.log) {  
-          console.log(delta, deltaX, deltaY);  
-     }  
- });  
 效果图：
![](http://dl.iteye.com/upload/attachment/0080/3556/c2b46136-ce72-3123-b255-de9410ebd06a.gif)
