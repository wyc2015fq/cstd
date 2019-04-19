# 移动web滑动效果Swipe JS插件 - 零度的博客专栏 - CSDN博客
2016年04月25日 14:52:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：1660
                
最近要做一个移动web的项目，前端后端都自己一人来搞。由于之前一直是做后端的开发，没涉及过前端的开发，甚是无从下手，但又不得不去弄，于是乎，先把w3school上前端相关的html、js、css都预览了一遍，然后在仿照别人的站点自己尝试着去实现。
虽然很多人都是前台很简单，没啥搞的（至少像我做后台的那些同事这么认为），但是真正开始弄起来，还是有些困难的，要做到美观易用，更得下工夫研究里面的所以然。
以下是我要实现一个在手机上滑动（图片，文字等）的效果。我先是把别人的页面下载下来，然后根据它的在页面呈现出来的“模块”进行切割，把各个模块单独提起出来，这样做：（1）理解页面的布局；（开始的时候，在怀疑，前、后台开发的思维模式是否不一样，因为一开始的时候，看页面的那些html、css、js实在晕，而且感觉代码也很凌乱）（2）熟悉html标签和css、js的用法；（这个需要花费一定的时间去熟悉每个标签的特性和用法）。
这个示例，就是我提取出来后，把原来多余的css等删除，仅仅保留滑动效果最简洁的部分，由于刚接触，在描述和理解上有不妥的地方，还烦请指正 ：）
一、先简要说下swipe.js（参见：[http://www.swipejs.com/](http://www.swipejs.com/)）
之所以用到这个js，主要是：
（1）我在百度里搜索“页面滑动”等关键词时，就出现了它老人家。
（2）在我下载下来别人滑动页面里面也用到了它。
二、如何使用
swipe是一个纯的javascript，里面没用到任何的其他的js框架（由于时间问题，里面的东西没很细研究，以后有时间了，再来学习），要实现移动端的滑动，必须要的元素有：
（1）html，这个当然不必说
（2）css，需要一定的css做样式。swipe里面会查找关键标签的样式，根据样式来现实
（3）js，这里面有自己定义和实现的js function
以下分别对上面的3大块进行描述：
（1）html。必须的html有：
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- <divid="slider"class="swipe"style="visibility:visible;">
- <divclass="swipe-wrap">
- <figure>
- <div>
- <imgsrc="img/1.JPG"width="100%"height="100%"/>
- </div>
- </figure>
- <figure>
- <div>
- <imgsrc="img/9.JPG"width="100%"height="100%"/>
- </div>
- </figure>
- </div>
- </div>
【1】最外层的div的id是自定义的，这个是需要传入到swipe中的
【2】最外层div的class="swipe"和第二层div的class="swipe-wrap"是写死的，swipe会查找这两个样式然后做相应的处理
【3】第三层的figure，就是我们要滑动元素的单元。
【4】之后我们的要滑动的内容，就是以figure为单位进行的
（2）css。必须的css有：
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- <style>
- .swipe {  
-     overflow: hidden;  
-     visibility: hidden;  
-     position: relative;  
- }  
- .swipe-wrap {  
-     overflow: hidden;  
-     position: relative;  
- }  
- .swipe-wrap > figure {  
-     float: left;  
-     width: 100%;  
-     position: relative;  
- }  
- #slider {  
-     max-width: 650px;/* 设置最大的宽度 */  
-     margin: 0px auto; /* 居中对齐 */  
- }  
- figure {  
-     margin: 0;/* 对齐，四周宽度都为0，在轮播的时候，以整张图片进行移动 */  
- }  
- </style>
【1】swipe、swipe-wrap、swipe-wrap > figure，这3个是swipe必须要使用到的，而且里面的样式最好也不要改动（感兴趣的同学可以试着去改动，看会呈现什么效果，蛮好玩的 :)）
【2】#slider是我自己设定的，用于整个空间居中
【3】figure样式也是自己设定的，但是是必须的，指定所有的figure对齐方式，而且只能设置为0，否则在滑动图片的时候会出现位置的错乱。（为什么会错乱，自己简单研究了下，swipe滑动宽度的像素是根据最外层div的宽度，如果figure的margin没有设置为0，而有间隔时，滑动的宽度就不会是一整张的figure了（figure之间存在空虚））
（3）js。必须有的js：
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- <scriptsrc="js/swipe.js"></script>
- <script>
- var slider =  
- Swipe(document.getElementById('slider'), {  
-     auto: 3000,  
-     continuous: true,  
-     callback: function(pos) {  
-     }  
-   });  
- </script>
这个就是调用swipe了，这里面的参数有些是可有可无，根据自己的需求来选择，简要说明下swipe的接口参数（js里面是这么称呼的么？）：
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- startSlide Integer (default:0) - 开始滚动的位置  
- speed Integer (default:300) - 动画滚动的间隔（秒数）  
- auto Integer - 开始自动幻灯片（以毫秒为单位幻灯片之间的时间）  
- continuous Boolean (default:true) - 创建一个无限的循环（当滑动到所有动画结束时是否循环滑动）  
- disableScroll Boolean (default:false) - 当滚动滚动条时是否停止幻灯片滚动  
- stopPropagation Boolean (default:false) - 是否停止事件冒泡  
- callback Function - 幻灯片运行中的回调函数  
- transitionEnd Function - 动画运行结束的回调函数  
api:
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- prev()：上一页  
- next()：下一页  
- getPos()：获取当前页的索引  
- getNumSlides()：获取所有项的个数  
- slide(index, duration)：滑动方法  
然后自己要实现的代码就可以写在那些回调函数中（如callback等）
三、其他
要在移动设备上要比较好的显示出来，除了上面的主要配置外，还需要另外一些辅助：
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- <metaname="viewport"content="width=device-width, minimum-scale=1.0, maximum-scale=1.0">
- <metaname="app-mobile-web-app-capable"content="yes">
至于上面的含义，大家可百度（程序猿都用Google，但是我上不了，就只能是百度了）之。
好了，swipe就先介绍到此了，以下在附上我实例的源码，里面还加了一段nav。（swipe自己去下载）
由于才第一次再csdn上写这么多的文字，弱弱的问下，我看到很其他的文章里面的代码有显示行数，刚在编辑的时候，到处找设置代码行数的地方，没找着，这个不支持么？
**[html]**[view
 plain](http://blog.csdn.net/xiongxianhe/article/details/39993193#)[copy](http://blog.csdn.net/xiongxianhe/article/details/39993193#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/482351/fork)
- <!DOCTYPE html>
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=utf-8"/>
- <title> xx的相册 </title>
- <metaname="description"content="xx的相册">
- <metaname="keywords"content="xxh,album,相册">
- <metaname="viewport"content="width=device-width, minimum-scale=1.0, maximum-scale=1.0">
- <metaname="app-mobile-web-app-capable"content="yes">
- </head>
- <style>
- .swipe {  
-     overflow: hidden;  
-     visibility: hidden;  
-     position: relative;  
- }  
- .swipe-wrap {  
-     overflow: hidden;  
-     position: relative;  
- }  
- .swipe-wrap > figure {  
-     float: left;  
-     width: 100%;  
-     position: relative;  
- }  
- #slider {  
-     max-width: 650px;/* 设置最大的宽度 */  
-     margin: 0px auto; /* 居中对齐 */  
- }  
- 
- figure {  
-     margin: 0;/* 对齐，四周宽度都为0，在轮播的时候，以整张图片进行移动 */  
- }  
- div.swipe {  
-     border: 1px solid blue;  
- }  
- .page-swipe nav #position {  
-     text-align: center;  
-     list-style: none;  
-     margin: 0;  
-     padding: 0  
- }  
- .page-swipe nav #position li {  
-     display: inline-block;  
-     width: 10px;  
-     height: 10px;  
-     border-radius: 10px;  
-     background: #141414;  
-     box-shadow: inset 0 1px 3px black,0 0 1px 1px #202020;  
-     margin: 0 2px;  
-     cursor: pointer  
- }  
- .page-swipe nav #position li.on {  
-     box-shadow: inset 0 1px 3px -1px #28b4ea,0 1px 2px rgba(0,0,0,.5);  
-     background-color: #1293dc;  
-     background-image: -webkit-gradient(linear,left top,left bottom,color-stop(0%,#1293dc),color-stop(100%,#0f6297));  
-     background-image: -webkit-linear-gradient(top,#1293dc,#0f6297);  
-     background-image: -moz-linear-gradient(top,#1293dc,#0f6297);  
-     background-image: -ms-linear-gradient(top,#1293dc,#0f6297);  
-     background-image: -o-linear-gradient(top,#1293dc,#0f6297);  
-     background-image: linear-gradient(top,#1293dc,#0f6297)  
- }  
- </style>
- <bodyclass="page-swipe">
- <divid="slider"class="swipe"style="visibility:visible;">
- <divclass="swipe-wrap">
- <figure>
- <divclass="face faceInner">
- <imgsrc="img/1.JPG"width="100%"height="100%"/>
- </div>
- </figure>
- <figure>
- <divclass="face faceInner">
- <imgsrc="img/2.JPG"width="100%"height="100%"/>
- </div>
- </figure>
- <figure>
- <divclass="face faceInner">
- <imgsrc="img/3.JPG"width="100%"height="100%"/>
- </div>
- </figure>
- </div>
- </div>
- 
- <nav>
- <ulid="position">
- <liclass="on"></li>
- <liclass=""></li>
- <liclass=""></li>
- </ul>
- </nav>
- 
- <scriptsrc="js/swipe.js"></script>
- <script>
- var slider =  
- Swipe(document.getElementById('slider'), {  
-     auto: 3000,  
-     continuous: true,  
-     callback: function(pos) {  
-         var i = bullets.length;  
-         while(i--){  
-             bullets[i].className = ' ';  
-         }  
-         bullets[pos].className = 'on';  
-     }  
-   });  
- var bullets = document.getElementById('position').getElementsByTagName('li');  
- </script>
- </body>
- </html>
