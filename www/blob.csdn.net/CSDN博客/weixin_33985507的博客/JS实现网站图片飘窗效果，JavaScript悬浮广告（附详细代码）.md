# JS实现网站图片飘窗效果，JavaScript悬浮广告（附详细代码） - weixin_33985507的博客 - CSDN博客
2018年10月31日 15:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：30
JS实现网站图片飘窗效果，JavaScript悬浮广告，**[郑州SEO](http://www.dingguanghui.com/)**提供以下代码，仅供参考：
<!DOCTYPE HTML>
<html>
<head>
<meta charset="utf-8">
<title>飘窗效果-丁光辉博客（www.dingguanghui.com）</title>
<style type="text/css">
*{margin:0px;padding:0px}
#ad{position:absolute;left:0px;top:0px;}
</style>
</head>
<body>
<div id="ad"><img src="haha.jpg" /></div>
</body>
<script type="text/javascript">
//通过ID获取img
ad=document.getElementById("ad");
//定义横纵坐标
x=0;
y=0;
//设置初始速度
xv=1.5;
yv=1.5;
//根据img横纵坐标位置，设置反方向速度
function fun(){
if(x<0||x>window.innerWidth-ad.offsetWidth){
xv=-xv;
}
if(y<0||y>window.innerHeight-ad.offsetHeight){
yv=-yv;
}
x+=xv;
y+=yv;
//将xy坐标值赋予img css样式中的left与top
ad.style.left=x+"px";
ad.style.top=y+"px";
}
//定时器调用
mytime=setInterval(fun,100);
//ad绑定鼠标悬停事件
ad.onmouseover=function(){
//清除定时器
clearInterval(mytime);
}
//鼠标离开，重新触发定时器
ad.onmouseout=function(){
mytime=setInterval(fun,100);
}
</script>
</html>
**推荐阅读：**
[html是什么？html与html5有什么区别？](http://www.dingguanghui.com/front-end/html/188.html)
[CSS3中em与px怎么换算？rem、em与px的区别是什么？](http://www.dingguanghui.com/front-end/css/399.html)
以上，就是丁光辉博客，针对“JS实现网站图片飘窗效果，JavaScript悬浮广告”问题的解答，欢迎大家加入交流学习群：428773129
本文由鬼叔说运营原创所有，转载请保留此处：(鬼叔说运营www.17seo8.cn)   http://www.dingguanghui.com/front-end/js/912.html
免责声明：图片来源网络，如有侵权请联系作者及时删除!
