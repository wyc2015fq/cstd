# JQuery Marquee插件(无缝滚动效果)- marquee.js - 零度的博客专栏 - CSDN博客
2016年04月25日 16:04:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：17011
```
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>marquee测试</title>
<script type="text/javascript" src="../../jquery/jquery.js"></script>  
<script type="text/javascript" src="../marquee/lib/jquery.marquee.js"></script>  
<script type="text/javascript">  
	$(function(){      
		 $("#marquee").marquee({
		   yScroll: "bottom",
		   showSpeed: 850,        // 初始下拉速度         , 
           scrollSpeed: 12,       // 滚动速度         , 
           pauseSpeed: 500,      // 滚动完到下一条的间隔时间         , 
           pauseOnHover: true,    // 鼠标滑向文字时是否停止滚动         , 
           loop: -1 ,             // 设置循环滚动次数 （-1为无限循环）         , 
           fxEasingShow: "swing" , // 缓冲效果         , 
           fxEasingScroll: "linear",  // 缓冲效果         , 
           cssShowing: "marquee-showing"  //定义class 
		 });  
	});  
</script>
<style>
   ul.marquee {
	   display: block;
	   line-height: 1;
	   position: relative;
	   overflow: hidden;
	   width: 400px;  
	   height: 22px; 
   }  
   ul.marquee li {
	   position: absolute; 
	   top: -999em;
	   left: 0;
	   display: block;  
	   white-space: nowrap; 
	   padding: 3px 5px; 
	   text-indent:0.8em
	}
</style>
</head>
<body >
<ul id="marquee" class="marquee">        
<li><a href="http://www.bloomylife.com/?cat=153" target="_blank">WEB前端开发</a> [2011-10-20]</li>       
<li><a href="http://www.bloomylife.com/?cat=154" target="_blank">架构设计</a> [2011-09-20]</li>        
<li><a href="http://www.bloomylife.com/?cat=157" target="_blank">系统运维</a> [2011-10-16]</li>     
</ul> 
</body>
</html>
```
该插件提供了许多属性选项，您可以配置定制外观和效果。
{yScroll:"top"// 初始滚动方向 (还可以是"top" 或 "bottom") 
        , showSpeed:850// 初始下拉速度 
        , scrollSpeed:12// 滚动速度 
        , pauseSpeed:5000// 滚动完到下一条的间隔时间 
        , pauseOnHover:true// 鼠标滑向文字时是否停止滚动 
        , loop:-1// 设置循环滚动次数 （-1为无限循环） 
        , fxEasingShow:"swing"// 缓冲效果 
        , fxEasingScroll:"linear"// 缓冲效果 
        , cssShowing:"marquee-showing"//定义class 
          // event handlers 
        ,init:null// 初始调用函数 
        , beforeshow:null// 滚动前回调函数 
        , show:null// 当新的滚动内容显示时回调函数 
        , aftershow:null// 滚动完了回调函数 
}
