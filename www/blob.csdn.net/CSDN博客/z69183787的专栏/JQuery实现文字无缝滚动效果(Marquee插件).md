# JQuery实现文字无缝滚动效果(Marquee插件) - z69183787的专栏 - CSDN博客
2014年08月25日 19:54:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：41936
推荐一个JQuery的无缝文字滚动效果，同时也可以滚动图片，也叫做跑马灯效果。
此jquery插件，依托jquery库，能实现各种滚动效果，且让HTML代码符合W3C标准。
官方演示，如下： 
[Demo](http://www.givainc.com/labs/marquee_example.htm)
使用方法如下：
1、加载javascript。
- <scripttype="text/javascript"src="js/jquery.js"></script>
- <scripttype="text/javascript"src="js/jquery.marquee.js"></script>
- <scripttype="text/javascript">
- $(function(){
- $("#marquee").marquee({yScroll:"bottom"});
- });
- </script>
2、加入CSS样式。
- ul.marquee {display: block;line-height:1;position: relative;overflow: hidden;width:400px;  height:22px;}
- ul.marquee li {position: absolute; top:-999em;left:0;display: block;  white-space: nowrap; padding:3px5px; text-indent:0.8em;}
3、HTML代码如下：
- <ulid="marquee">
- <li><ahref="http://www.bloomylife.com/?cat=153"target="_blank">  WEB前端开发</a> [2011-10-20]</li>
- <li><ahref="http://www.bloomylife.com/?cat=154"target="_blank">  架构设计</a> [2011-09-20]</li>
- <li><ahref="http://www.bloomylife.com/?cat=157"target="_blank">  系统运维</a> [2011-10-16]</li>
- </ul>
该插件提供了许多属性选项，您可以配置定制外观和效果。
- {
-  yScroll:"top"// 初始滚动方向 (还可以是"top" 或 "bottom")         , 
-  showSpeed:850// 初始下拉速度         , 
-  scrollSpeed:12// 滚动速度         , 
-  pauseSpeed:5000// 滚动完到下一条的间隔时间         , 
-  pauseOnHover:true// 鼠标滑向文字时是否停止滚动         , 
-  loop:-1// 设置循环滚动次数 （-1为无限循环）         , 
-  fxEasingShow:"swing"// 缓冲效果         , 
-  fxEasingScroll:"linear"// 缓冲效果         , 
-  cssShowing:"marquee-showing"//定义class           // 
- event handlers         ,
-  init:null// 初始调用函数         , 
-  beforeshow:null// 滚动前回调函数         , 
-  show:null// 当新的滚动内容显示时回调函数         , 
-  aftershow:null// 滚动完了回调函数  
- }
jquery.marquee.js可以到官网地址下载：[http://www.givainc.com/labs/marquee_jquery_plugin.htm](http://www.givainc.com/labs/marquee_jquery_plugin.htm)
