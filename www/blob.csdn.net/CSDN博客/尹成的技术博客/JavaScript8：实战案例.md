
# JavaScript8：实战案例 - 尹成的技术博客 - CSDN博客

2018年11月14日 15:03:28[尹成](https://me.csdn.net/yincheng01)阅读数：41


\#1.for循环图片的应用
在页面上显示一个矩形，鼠标在这个矩形的某个地方划过，则在该位置以矩形的形式显示图片的一部分。
效果图：
![这里写图片描述](https://img-blog.csdn.net/2018091214464521?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
鼠标滑过之后：
![这里写图片描述](https://img-blog.csdn.net/20180912144702858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>for循环图片应用</title>
	<style type="text/css">
	body{margin:0;padding:0;background:#ccc;text-align:center;}
	#content{width:510px;height:510px;background:#ddd;margin:20px auto auto 15px;position: relative;z-index:2;}
	p{font-size:18px;}
	ul{list-style:none}
	li{width:50px;height:50px;background:#ccc;position: absolute;left:0;top:0;opacity:0;filter:alpha(opacity:0);}
	</style>
<script type="text/javascript">
	window.onload=function()
	{
		var str="";
		var j=0;
		var oDiv=document.getElementById("content");
		var	oUl=oDiv.getElementsByTagName('ul')[0];
		var aLi=oUl.getElementsByTagName('li');
		for(var i=0;i<100;i++)
		{
			str+="<li></li>";
		}
		oUl.innerHTML=str;
		for(var i=0;i<aLi.length;i++)
		{	aLi[i].iNow=i;
			aLi[i].style.left=((i%10)*51)+"px";
			aLi[i].style.opacity=0;
			if(i%10==0)
			{
				++j;
			}
			aLi[i].style.top=((j-1)*51)+"px";
			aLi[i].style.background="url(images/MM.jpg)";
			aLi[i].style.backgroundPosition=(-(i%10)*51)+"px"+" "+(-(j-1)*51)+"px";
			aLi[i].onmouseover=function()
			{
				aLi[this.iNow].style.filter='alpha(opacity:'+100+')';
				aLi[this.iNow].style.opacity=1;
			}
		}
	}
</script>
</head>
<body>
<div id="content">
	<ul></ul>
</div>
<p>鼠标滑过块来查看效果->兼容浏览器:IE6-8,FireFox,Chrome,Opera,搜狗,世界之窗,傲游</p>
</body>
</html>`\#2.日历
在页面上显示12个月份，鼠标在某个月份上划过，则显示该月的活动安排。
效果图：
![这里写图片描述](https://img-blog.csdn.net/20180912144955252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>JS简易日历</title>
	<style type="text/css">
		body,h1,h2,p,ul,li{margin:0;padding:0;}
		body{font-size:12px;text-align:center;font-family:"幼圆";background:#31322C;}
		h1,h2{font-weight:normal;}
		em{font-style:normal;}
		ul{list-style-type:none;}
		/*动画部分*/
		@-webkit-keyframes changeShadow{
			0%{
				-webkit-box-shadow:0 0 0 #000;
			}
			25%{
				-webkit-box-shadow:0 0 10px #282828;
			}
			50%{
				-webkit-box-shadow:0 0 15px #5D5D5D;
			}
			100%{
				-webkit-box-shadow:0 0 22px #FFF;
			}
		}
		#calender{
			width:240px;
			height:453px;
			background-image:-webkit-linear-gradient(0deg,#EFF3A3,#F0F0EE);
			background-image:-moz-linear-gradient(0deg,#EFF3A3,#F0F0EE);
			-webkit-box-shadow:0 0 20px #FFF; 
			-webkit-animation:changeShadow 1s ease-in infinite alternate;
			-moz-box-shadow:0 0 20px #FFFF;
			box-shadow:0 0 20px #FFF;
			margin:20px auto 0 auto;
		}
		#calender h1{
			padding:5px 0;
			font-size:16px;
			font-weight:bold;
		}
		#calender em{
				color:#f00;
		}
		#calender li{
			float: left;
			width:60px;
			height:60px;
			margin:10px;
			_display:inline;
			font-size:16px;
			-webkit-box-sizing:border-box;
			-moz-box-sizing:border-box;
			-webkit-transition:background .5s,color .5s ;
			color:#fff;
			cursor:pointer;
			line-height:30px;
			background:#1E1E1A;
		}
		#activity_content{
			clear:both;
			width:210px;
			height:80px;
			margin:0 10px;
			padding:12px 0 0 10px;
			background:#636556;
		}
		#activity_content h2{
			text-align:left;
			font-size:18px;
			font-weight:bold;
		}
		.active{
			border:2px solid #F90000;
			color:#f00!important;
			background:#fff!important;
			font-weight:bold;
		}
	</style>
<script>
	window.onload=function(){
		var oDiv1=document.getElementById("calender");
		var aLi=oDiv1.getElementsByTagName('li');
		var oDiv2=document.getElementById("activity_content");
		var arrText=["和朋友出去打篮球","和朋友出去唱K",
					 "和朋友出去逛街","庆祝项目酒会",
					 "在家歇着","在家看电影",
					 "去网吧打游戏","和朋友去台球馆打台球",
					 "和朋友去爬山","参加前端面试",
					 "在家歇着","出去旅游"
					 ]
		for(var i=0;i<aLi.length;i++)
		{
			aLi[i].iNow=i;
			aLi[i].onmouseover=function()
			{
				for(var i=0;i<aLi.length;i++)
				{
					aLi[i].className="";
				}
				aLi[this.iNow].className="active";
				oDiv2.innerHTML="<h2>"+(1+this.iNow)+"月活动"+"</h2><br/><strong style='color:#ff0'>"+arrText[this.iNow]+"</strong>"
			}
		}
	}
</script>
</head>
<body>
	<div id="calender">
		<h1><em>★</em> 活动计划 <em>★</em></h1>
		<ul>
			<li><h2>1</h2><span>JAN</span></li>
			<li><h2>2</h2><span>FEB</span></li>
			<li><h2>3</h2><span>MAR</span></li>
			<li><h2>4</h2><span>APR</span></li>
			<li><h2>5</h2><span>MAY</span></li>
			<li><h2>6</h2><span>JUN</span></li>
			<li><h2>7</h2><span>JUL</span></li>
			<li><h2>8</h2><span>AUG</span></li>
			<li><h2>9</h2><span>SEP</span></li>
			<li><h2>10</h2><span>OCT</span></li>
			<li><h2>11</h2><span>NOV</span></li>
			<li><h2>12</h2><span>DEC</span></li>
		</ul>
		<div id="activity_content"></div>
	</div>
</body>
</html>`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

