
# 网站设计之常见简单实用的JavaScript特效总结（上篇） - 杨秀璋的专栏 - CSDN博客

2016年12月19日 00:54:55[Eastmount](https://me.csdn.net/Eastmount)阅读数：4289所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)



这篇主要是总结JavaScript常见简单实用的特效，主要从代码量短、简单实用几个方面进行叙述。其中特效包括：
1.鼠标悬停图片切换查看器；
2.鼠标移动图片放大；
3.鼠标移动切换内容；
4.贵财下拉菜单案例；
5.JS图片放大镜功能-类似淘宝；
6.下一页翻页跳转功能。
下载地址：
希望文章对你有所帮助，尤其是学习前端JavaScript的同学。

## 一. 鼠标悬停图片切换查看器
代码如下所示，通过JavaScript函数showDaTu显示大图，重点是在<img>中调用onmouseover鼠标函数，然后通过document.getElementById函数实现换图。

```python
<html>
	<head>
		<title>
			JavaScript 图片切换 
		</title>
	</head>
	<body>
		<script>
			function showDaTu(src){
				document.getElementById("defaultImg").src=src;
			}
		</script>
		
		<img src="wall1.jpg" id="defaultImg">
		<br><br><br>
		<img src='wall_s1.jpg' onmouseover="showDaTu('wall1.jpg')">
		<img src='wall_s2.jpg' onmouseover="showDaTu('wall2.jpg')">
		<img src='wall_s3.jpg' onmouseover="showDaTu('wall3.jpg')">
		<img src='wall_s4.jpg' onmouseover="showDaTu('wall4.jpg')">
		<br>因图片较大，请等待图片加载完成……然后鼠标放小图上就会切换了。
	</body>
</html>
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20161219000404769)


## 二. 鼠标移动图片放大
该部分参考：[http://blog.csdn.net/u014175572/article/details/51535768](http://blog.csdn.net/u014175572/article/details/51535768)
CSS3的transform:scale()可以实现按比例放大或者缩小功能。
CSS3的transition允许CSS的属性值在一定的时间区间内平滑地过渡。这种效果可以在鼠标单击、获得焦点、被点击或对元素任何改变中触发，并圆滑地以动画效果改变CSS的属性值。
代码如下所示：

```python
<html>  
    <head>  
        <meta charset="UTF-8">  
        <title></title>  
        <style type="text/css">  
            div{  
                width: 300px;  
                height: 300px;  
                border: #000 solid 1px;  
                margin: 50px auto;  
                overflow: hidden;  
            }  
            div img{  
                cursor: pointer;  
                transition: all 0.6s;  
            }  
            div img:hover{  
                transform: scale(1.4);  
            }  
        </style>  
    </head>  
    <body>  
        <div>  
            <img src="focus.jpg" />  
        </div>  
    </body>  
</html>
```
效果如下图所示，包括缩放前后的对比。
![](https://img-blog.csdn.net/20161219002207219)![](https://img-blog.csdn.net/20161219002226058)

transition: all 0.6s;表示所有的属性变化在0.6s的时间段内完成。
transform: scale(1.4);表示在鼠标放到图片上的时候图片按比例放大1.4倍。
PS：这部分代码参考博主"简单就是美"，推荐大家可以去学习下，非常不错。


## 三. 鼠标移动内容切换
这段代码参考文章：[http://blog.csdn.net/hill_kinsham/article/details/52448668](http://blog.csdn.net/hill_kinsham/article/details/52448668)
重点说一下关键功能的几个函数。
1.onmouseover=" change('zs', this) " 函数的功能是鼠标移动到目标区域时，响应函数。这里的'zs'用id与后面要变更的区域绑定。this的功能不太了解，效果是改变当前的值。
2.onmouseout="change2(this)";函数的功能是鼠标移开目标区域时，响应函数。
3.display. display 属性规定元素应该生成的框的类型。
none 此元素不会被显示。block 此元素将显示为块级元素，此元素前后会带有换行符。inline 默认。此元素会被显示为内联元素，元素前后没有换行符。
4.用<ul>时，去掉行号，并把它放到最左边。 list-style-type: none;


```python
<html>
<head>
    <meta charset="UTF-8">
    <style>
     body{
	    font-size: 12px;
	}
	.div1{
	    width: 126px;
	    height: 156px;
	   /* background-color: peachpuff;*/
	}
	.navi{
	    width: 21px;
	    height: 156px;
	   /* background-color: yellowgreen;*/
	    float: left;
	}
	.navi ul{
	    padding: 0px;
	    margin-left: 0px;
	    margin-top: 0px;
	}
	.navi ul li{
	    list-style-type: none;
	    width: 21px;
	    height: 43px;
	    margin-top: 4px;
	    text-align: center;
	    padding-top: 5px;
	    background-color: silver;
	}
	.zs, .rz,.ky{
	    width: 101px;
	    margin-left: 4px;
	    height: 156px;
	    margin-top: 0px;
	    /*background-color: rosybrown;*/
	    float: left;
	}
	.zs ul,.rz ul,.ky ul{
	    padding: 0px;
	    margin-left: 0px;
	    margin-top: 3px;
	    float: left;
	}
	.zs ul li,.rz ul li,.ky ul li{
	    list-style-type: none;
	    line-height: 19px;
	}
	.rz,.ky{
	    display: none;
	}
    </style>
    <title>souhu</title>
    <script language="JavaScript">
        <!--
         function change(val,obj) {
             obj.style.backgroundColor="#FFC12D";
             if(val=='zs'){
                 zs.style.display='block';
                 rz.style.display='none';
                 ky.style.display='none';
             }else if(val=='rz'){
                 ky.style.display='none';
                 zs.style.display='none';
                 rz.style.display='block';
             }else if(val=='ky'){
                 ky.style.display='block';
                 zs.style.display='none';
                 rz.style.display='none';
             }
         }
        function change2(val) {
            val.style.backgroundColor="silver";
        }
        //-->
    </script>
</head>
<body>
<div class="div1">
<div class="navi">
    <ul>
        <li onmouseover="change('zs',this)" onmouseout="change2(this)">招生</li>
        <li onmouseover="change('rz',this)" onmouseout="change2(this)">热招</li>
        <li onmouseover="change('ky',this)" onmouseout="change2(this)">考研</li>
    </ul>
</div>
    <div id="zs" class="zs">
        <ul>
            <li><a href="#">招生招生招生招生</a></li>
            <li><a href="#">招生招生招生招生</a></li>
            <li><a href="#">招生招生招生招生</a></li>
            <li><a href="#">招生招生招生招生</a></li>
            <li><a href="#">招生招生招生招生</a></li>
            <li><a href="#">招生招生招生招生</a></li>
        </ul>
    </div>
    <div id="rz" class="rz" >
        <ul>
            <li><a href="#">热招热招热招热招</a></li>
            <li><a href="#">热招热招热招热招</a></li>
            <li><a href="#">热招热招热招热招</a></li>
            <li><a href="#">热招热招热招热招</a></li>
            <li><a href="#">热招热招热招热招</a></li>
            <li><a href="#">热招热招热招热招</a></li>
        </ul>
    </div>
    <div id="ky" class="ky" >
        <ul>
            <li><a href="#">考研考研考研考研</a></li>
            <li><a href="#">考研考研考研考研</a></li>
            <li><a href="#">考研考研考研考研</a></li>
            <li><a href="#">考研考研考研考研</a></li>
            <li><a href="#">考研考研考研考研</a></li>
            <li><a href="#">考研考研考研考研</a></li>
        </ul>
    </div>
    <div></div>
    <div></div>
</div>
</body>
</html>
```
运行结果如下图所示：
![](https://img-blog.csdn.net/20161219003432032)![](https://img-blog.csdn.net/20161219003437828)

## 四. 贵财下拉菜单案例
这是模仿贵州财经大学主页下拉菜单的一段代码，非常有用。希望对你有所帮助，感谢我的学生。
详见下载地址：

```python
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>下菜单实现</title>
<style>
body{
	width:100%;
	padding:0px;
	margin:0px;
	}
#layout{
	margin-top:10px;
	padding:0px;
	width:1024px;
	margin-left:auto;
	margin-right:auto;
	}
#top{
	width:1024px;;
	height:59px;
	}
#top #logo{
	float:left;
	margin-bottom:0px;
	
	}
#top #second{
	float:left;
	margin-left:160px;
	margin-top:20px;
	padding:0px;
	}
#top #second li{
	font-size: 12px;
	font-weight: bolder;
	margin-right: 20px;
	list-style-type: none;
	float: left;
	font-family: "微软雅黑";
	}
#top #second li a{
	text-decoration:none;
	color:gray;
	
	}
#top #second li a:hover{
	color:red;
}
#top #third{
	float: left;
	border::gray 1px solid;
	border: 2px solid #FFF;
	background-color:gray;
	margin-left:53px;
	margin-top:10px;
	padding-top:8px;
	padding-bottom:8px;
	padding-left:20px;
	padding-right:5px;
	border-radius:8px;
	}
#top #third li{
	float:left;
	margin-right:10px;
	list-style-type:none;
	font-size:12px;
	}
#top #third li a{
	color:white;
	text-decoration:none;
	}
#top #third li a:hover{
	text-decoration:underline;
	}
#menu{
	width: 2000px;
	height: 50px;
	background-color: #313b4d;
	padding-top: 0px;
	padding-left: 460px;
	padding-bottom: 0px;
	margin-left: -460px;
	z-index: 20;
	}
#menu #first{
	position: relative;
	width:100%;
	margin-left:10px;
	padding:0px;
}
#menu #first li{
	float: left;
	list-style-type: none;
	font-size: 14px;
	margin-right: 40px;
	margin-top: 15px;
	margin-bottom: 0px;
	padding-bottom: 15px;
	font-family: "微软雅黑";
	}
#tit1:hover#tit1:hover,#tit2:hover,#tit3:hover,#tit4:hover,#tit5:hover,#tit6:hover,#tit7:hover{
	background-image: url(backgroundimg.png);
	background-repeat: no-repeat;
	background-position: center bottom;
	}
#tit1:hover #one,#tit2:hover #two,#tit3:hover #three,#tit4:hover #four,#tit5:hover #five,#tit6:hover #six,#tit7:hover #seven{
	display:inline;
	}
#tit1 a, #tit2 a, #tit3 a,#tit4 a,#tit5 a,#tit6 a,#tit7 a{
	color:white;
	text-decoration:none;
	font-weight:bolder;
	padding:0px;
	margin:0px;
	}
#one,#two,#three,#four,#five,#six,#seven{
	position: absolute;
	background-color:white;
	top: 50px;
	left:-460px;
	width: 1920px;
	padding-top: 25px;
	padding-left: 0px;
	padding-bottom: 25px;
	display: none;
	margin-right:0px;
	}
#one img,#two img,#three img,#four img,#five img,#six img,#seven img{
	float:left;
	margin-right:50px;
	margin-left:460px;
}
#one ul,#two ul,#three ul,#four ul,#five ul,#six ul,#seven ul{
	font-family: "微软雅黑";
	margin-top:10px;
	width:1200px;
	}
#one li a,#two li a,#three li a,#four li a,#five li a,#six li a,#seven li a{
	color:#313b4d;
	font-weight:normal;
	margin-right:5px;
	}
#one li a:hover,#two li a:hover,#three li a:hover,#four li a:hover,#five li a:hover,#six li a:hover,#seven li a:hover{
	color: #AD0000;	
	}
form{
	float:left;
	width:200px;
	border:white solid 2px;
	border-radius:8px;;
	padding-left:5px;
	padding-bottom:5px;
	margin-top:10px;
	margin-left:133px;
	}
#search{
	font-family: Arial, Helvetica, sans-serif;
	color: gray;
	font-size: 12px;
	border: none;
	padding:0px;
	margin-left:5px;
	margin-top: 5px;
	margin-bottom:3px;
	background-color: transparent;
	letter-spacing:1px;
	}
#search_img{
	margin-top:17px;
	margin-bottom:3px;
	margin-left:-30px;
	
	}
#bg{
	padding:0px;
	width:100%;
	height:520px;
	background-color:black;
	margin-top:0px;
	}
p{
	font-family:"微软雅黑"
	font-size: 16px;
	font-weight: bolder;
	text-align:center;
	color: gray;
	margin-top:50px;
	border-top-width: 1px;
	border-bottom-width: 1px;
	border-top-style: solid;
	border-bottom-style: solid;
	border-top-color: #333;
	border-right-color: #333;
	border-bottom-color: #333;
	border-left-color: #333;
	}
</style>
</head>
<body>
<div id="layout">
<div id="top">
	<img id="logo" src="gufe_logo.png">
    <ul id="second">
    	<li><a href="#">领导信箱</a></li>
        <li><a href="#">信息公开</a></li>
        <li><a href="#">数字贵财</a></li>
        <li><a href="#">邮箱</a></li>
        <li><a href="#">English</a></li>
    
    </ul>
    <ul id="third">
    	<li><a href="#">学生</a></li>
        <li><a href="#">教职工</a></li>
        <li><a href="#">校友</a></li>
        <li><a href="#">考生/访客</a></li>
    </ul>
</div>
<div id="menu">
	<ul id="first">
    	<li id="tit1"><a href="#">贵财概况</a>
            <div id="one">
              	<img src="gaikuo_img.jpg"/>
				<ul >
            		<li><a href="#">学校简介</a></li>
                	<li><a href="#">贵财标识</a></li>
                	<li><a href="#">现任领导</a></li>
                	<li><a href="#">发展战略</a></li>
                	<li><a href="#">领导关怀</a></li>
                	<li><a href="#">校友风采</a></li>
                	<li><a href="#">大事记</a></li>
                	<li><a href="#">历史沿革</a></li>
                	<li><a href="#">校园风光</a></li>
            	</ul>
                </div>
            </li>
        	
        <li id="tit2"><a href="#">组织机构</a>
        	<div id="two">
                <img src="zuzhi_img.jpg"/>
              	<ul >
                	
            		<li><a href="#">党群部门</a></li>
                	<li><a href="#">行政部门</a></li>
                	<li><a href="#">院系设置</a></li>
                	<li><a href="#">科研机构</a></li>
                	<li><a href="#">教辅部门</a></li>
            	</ul>
                </div></li>
        <li id="tit3"><a href="#">招生就业</a>
        	<div id="three">
              	<img src="zhaosheng_img.jpg" />
                <ul >
               
            		<li><a href="#">本专科招生</a></li>
                	<li><a href="#">研究生招生</a></li>
                	<li><a href="#">本专科就业</a></li>
                	<li><a href="#">研究生就业</a></li>
            	</ul>
                </div></li>	
        <li id="tit4"><a href="#">教育教学</a>
        	<div id="four">
              	<img src="jiaoyu_img.jpg">
                <ul >
                	
            		<li><a href="#">师资队伍</a></li>
                	<li><a href="#">本科生教育</a></li>
                	<li><a href="#">研究生教育</a></li>
                	<li><a href="#">继续教育</a></li>
                	<li><a href="#">留学生教育（国际合作培养）</a></li>
            	</ul>
                </div></li>
        
        <li id="tit5"><a href="#">科学研究</a>
        	<div id="five">
              	<img src="kexue_img.jpg"/>
                <ul >         
            		<li><a href="#">学科建设</a></li>
                	<li><a href="#">科研项目</a></li>
                	<li><a href="#">科研机构</a></li>
                	<li><a href="#">学术刊物</a></li>
            	</ul>
                </div></li>
        	
        <li id="tit6"><a href="#">合作交流</a>
        	<div id="six">
              	<img src="hezuo_img.jpg"/>
                <ul >
                	
            		<li><a href="#">中外合作办学</a></li>
                	<li><a href="#">孔子学院</a></li>
                	<li><a href="#">学术交流</a></li>
                	<li><a href="#">国际交流</a></li>
            	</ul>
                </div></li>
       
        <li id="tit7"><a href="#">校园服务</a>
        	<div id="seven">
              	<img src="fuwu_img.jpg"/>
                <ul >
            		<li><a href="#">校园文化</a></li>
                	<li><a href="#">校园导览</a></li>
                	<li><a href="#">生活指南</a></li>
                	<li><a href="#">校园媒体</a></li>
                	<li><a href="#">道德讲堂</a></li>
                	<li><a href="#">心理健康教育</a></li>
                	<li><a href="#">助学服务</a></li>
                	<li><a href="#">校历</a></li>
                    <li><a href="#">问卷调查</a></li>
            	</ul>
                </div></li>
    </ul> 
    <form>
         <input id="search" type="text" name="search" value="请输入搜索内容..." size="20px;" />
      	
    </form><input id="search_img"type="image" src="search_button.png"/>
</div>
</div>
<img id="bg" src="bga2.jpg" >
<p>By Eastmount CSDN</p>
</body>
</html>
```
运行如下图所示，鼠标移动到不同位置可以显示不同下拉菜单，非常实用的例子。
![](https://img-blog.csdn.net/20161219004454959)
![](https://img-blog.csdn.net/20161219004523031)



## 五. JS图片放大镜功能-类淘宝
代码如下所示：
```python
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>放大镜</title>
<style type="text/css">
#div1 { width: 120px; height: 90px; padding: 5px; border: 1px solid #ccc; position: relative; }
#div1 .small_pic { width: 120px; height: 90px; background: #eee; position: relative; }
#div1 .float_layer { width: 50px; height: 50px; border: 1px solid #000; background: #fff; filter: alpha(opacity: 30); opacity: 0.3; position: absolute; top: 0; left: 0; display:none; }
#div1 .mark {width:100%; height:100%; position:absolute; z-index:2; left:0px; top:0px; background:red; filter:alpha(opacity:0); opacity:0;}
#div1 .big_pic { position: absolute; top: -1px; left: 215px; width:250px; height:250px; overflow:hidden; border:2px solid #CCC; display:none; }
#div1 .big_pic img { position:absolute; top: -30px; left: -80px; }
</style>
<script type="text/javascript">
function getByClass(oParent, sClass)
{
	var aEle=oParent.getElementsByTagName('*');
	var aTmp=[];
	var i=0;
	
	for(i=0;i<aEle.length;i++)
	{
		if(aEle[i].className==sClass)
		{
			aTmp.push(aEle[i]);
		}
	}
	
	return aTmp;
}
window.onload=function ()
{
	var oDiv=document.getElementById('div1');
	var oMark=getByClass(oDiv, 'mark')[0];
	var oFloat=getByClass(oDiv, 'float_layer')[0];
	var oBig=getByClass(oDiv, 'big_pic')[0];
	var oSmall=getByClass(oDiv, 'small_pic')[0];
	var oImg=oBig.getElementsByTagName('img')[0];
	oMark.onmouseover=function ()
	{
		oFloat.style.display='block';
		oBig.style.display='block';
	};
	
	oMark.onmouseout=function ()
	{
		oFloat.style.display='none';
		oBig.style.display='none';
	};
	
	oMark.onmousemove=function (ev)
	{
		var oEvent=ev||event;
		
		var l=oEvent.clientX-oDiv.offsetLeft-oSmall.offsetLeft-oFloat.offsetWidth/2;
		var t=oEvent.clientY-oDiv.offsetTop-oSmall.offsetTop-oFloat.offsetHeight/2;
		
		if(l<0)
		{
			l=0;
		}
		else if(l>oMark.offsetWidth-oFloat.offsetWidth)
		{
			l=oMark.offsetWidth-oFloat.offsetWidth;
		}
		
		if(t<0)
		{
			t=0;
		}
		else if(t>oMark.offsetHeight-oFloat.offsetHeight)
		{
			t=oMark.offsetHeight-oFloat.offsetHeight;
		}
		oFloat.style.left=l+'px';
		oFloat.style.top=t+'px';
		var percentX=l/(oMark.offsetWidth-oFloat.offsetWidth);
		var percentY=t/(oMark.offsetHeight-oFloat.offsetHeight);
		oImg.style.left=-percentX*(oImg.offsetWidth-oBig.offsetWidth)+'px';
		oImg.style.top=-percentY*(oImg.offsetHeight-oBig.offsetHeight)+'px';
	};
};
</script>
</head>
<body>
<div id="div1">
<div class="small_pic">
<span class="mark"></span>
        <span class="float_layer"></span>
        <img src="wall_s6.jpg" /></div>
    <div class="big_pic"><img src="wall6.jpg" /></div>
</div>
</body>
</html>
```
运行结果如下图所示，代码较难。
![](https://img-blog.csdn.net/20161219005118837)

## 六. 实现下一页翻页功能
代码如下所示，该段代码实现点击"下一页"翻页功能。

```python
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>无标题文档</title>
<style type="text/css">
#top {
	background-color: #8EC7FF;
	height: 45px;
	width: 1030px;
	float: left;
	border-top-width: thin;
	border-right-width: thin;
	border-bottom-width: thin;
	border-left-width: thin;
	border-bottom-style: solid;
	border-top-color: #FFF;
	border-right-color: #FFF;
	border-bottom-color: #FFF;
	border-left-color: #FFF;
}
#own {
	width: 1030px;
	margin: 0 auto;
}
h2 {
	display: inline;
	float: left;
	font-size: 18px;
	font-weight: 400;
	margin-top: -12px;
}
 #message {
	height: 30px;
	width: 1030px;
	background-color: #CCCCCC;
	float: left;
	border-top-width: thin;
	border-right-width: thin;
	border-bottom-width: thin;
	border-left-width: thin;
	border-bottom-style: solid;
	border-top-color: #999;
	border-right-color: #999;
	border-bottom-color: #999;
	border-left-color: #999;
}
#own #message .h1 {
	width: 150px;
}
#own #message .h2 {
	width: 700px;
}
#top  .sb {
	height: 30px;
	width: 100px;
	margin-left: 10px;
	margin-top: 10px;
}
#top  .sb1 {
	height: 30px;
	width: 120px;
	margin-left: 10px;
	margin-top: 10px;
}
#buttom {
	width: 1030px;
	float: left;
	margin-top: 10px;
}
td{
	border-bottom:#CCC solid 1px;
	border-collapse:collapse;
}
a{
	text-decoration:none;
	color:#333;
}
a:link {color: #333}		/* 未访问的链接 */
a:visited {color:#00C;}	/* 已访问的链接 */
a:hover {color:#99C;}	
</style>
<!--实现全选-->
<script type="text/javascript" src="js/jquery-1.4.4.min.js"></script>
<script type="text/javascript">
$(function(){
    $('#selectAll').click(function(){
        $('input[type=checkbox]').attr('checked', $(this).attr('checked'));
    });
});
</script>
<!--批量删除-->
<script language="javascript"> 
function deleteAll(obj){ 
        var checked = document.getElementsByName(obj); 
		for(var i = 0; i < checked.length; i ++){ 
              if(checked[i].checked){ 
                  var tr=checked[i].parentNode.parentNode; 
                  var tbody=tr.parentNode; 
				  tbody.removeChild(tr); 
				  i--; 
				  } 
		} 
} 
</script> 
<script type="text/javascript">
     function goPage(pno,psize){
    var itable = document.getElementById("idData");
    var num = itable.rows.length;//表格所有行数(所有记录数)
    console.log(num);
    var totalPage = 0;//总页数
    var pageSize = psize;//每页显示行数
    //总共分几页 
    if(num/pageSize > parseInt(num/pageSize)){   
            totalPage=parseInt(num/pageSize)+1;   
       }else{   
           totalPage=parseInt(num/pageSize);   
       }   
    var currentPage = pno;//当前页数
    var startRow = (currentPage - 1) * pageSize+1;//开始显示的行  31 
       var endRow = currentPage * pageSize;//结束显示的行   40
       endRow = (endRow > num)? num : endRow;    40
       console.log(endRow);
       //遍历显示数据实现分页
    for(var i=1;i<(num+1);i++){    
        var irow = itable.rows[i-1];
        if(i>=startRow && i<=endRow){
            irow.style.display = "block";    
        }else{
            irow.style.display = "none";
        }
    }
    var pageEnd = document.getElementById("pageEnd");
    var tempStr = "共"+num+"条记录 分"+totalPage+"页 当前第"+currentPage+"页";
    if(currentPage>1){
        tempStr += "<a href=\"#\" onClick=\"goPage("+(1)+","+psize+")\">首页</a>";
        tempStr += "<a href=\"#\" onClick=\"goPage("+(currentPage-1)+","+psize+")\"><上一页</a>"
    }else{
        tempStr += "首页";
        tempStr += "<上一页";    
    }
    if(currentPage<totalPage){
        tempStr += "<a href=\"#\" onClick=\"goPage("+(currentPage+1)+","+psize+")\">下一页></a>";
        tempStr += "<a href=\"#\" onClick=\"goPage("+(totalPage)+","+psize+")\">尾页</a>";
    }else{
        tempStr += "下一页>";
        tempStr += "尾页";    
    }
    document.getElementById("barcon").innerHTML = tempStr;
    
}
</script>
<script>
function td()
{
  document.getElementById("btn1").value="已读";
}
function td1()
{
  document.getElementById("btn2").value="已读";
}
  
</script>
</head>
<body onload ="goPage(1,5);" >
<div id="own">
   <div id="top">
       
          <input class="sb" type="submit" id="" value="删除"  name="delete_button" onclick="deleteAll('range');" />
       
  </div>
  
  <div id="buttom">
   <span>我的消息</span>
     <div><hr color="#00CCFF" width="1030px"/></div>
    <table  cellPadding=0  cellSpacing=0 style="text-align:center" id="idData">
    <tbody>
    <tr style="height:35px;">
         <td width="45px"><input type="checkbox" value="" name="range" id="selectAll"/></td>
         <td width="120px">反馈人</td>
         <td width="680px">反馈内容</td>
        <td width="150px">反馈时间</td>
         <td width="50px">操作</td>
       </tr>
      <tr  style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="xxxiangxi.html" target="menuFrame" onclick="iframe.location='xxxiangxi.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
         <td width="50px"><a href="xxxiangxi.html" target="menuFrame" onclick="iframe.location='xxxiangxi.html'"><input class="sb" type="submit"  value="未读"  id="btn1" onclick="td()"/></a></td>
       </tr>
       
       <tr style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="note.html" target="menuFrame" onclick="iframe.location='note.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
        <td width="50px"><a href="note.html" target="_blank"><input class="sb" type="submit" id="btn2" onclick="td1()" value="未读" /></a></td>
       </tr>
       <tr style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="note.html" target="menuFrame" onclick="iframe.location='note.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
        <td width="50px"><a href="note.html" target="_blank"><input class="sb" type="submit" id="btn2" onclick="td1()" value="未读" /></a></td>
       </tr>
       <tr style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="note.html" target="menuFrame" onclick="iframe.location='note.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
        <td width="50px"><a href="note.html" target="_blank"><input class="sb" type="submit" id="btn2" onclick="td1()" value="未读" /></a></td>
       </tr>
       <tr style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="note.html" target="menuFrame" onclick="iframe.location='note.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
        <td width="50px"><a href="note.html" target="_blank"><input class="sb" type="submit" id="btn2" onclick="td1()" value="未读" /></a></td>
       </tr>
       <tr style="height:32px;">
         <td width="45px"><input type="checkbox" value="" name="range"/></td>
         <td width="120px">沈敏</td>
         <td width="680px"><a href="note.html" target="menuFrame" onclick="iframe.location='note.html'">你好呀！！！！！！！！！！！！！！！！！！！！！！！！！！！！</a></td>
        <td width="150px">2016/11/13</td>
        <td width="50px"><a href="note.html" target="_blank"><input class="sb" type="submit" id="btn2" onclick="td1()" value="未读" /></a></td>
       </tr>
       </tbody>
    </table>
     <table width="100%" align="right">
        <tr><td style="padding-left:640px;"><div id="barcon" name="barcon"></div></td></tr>
    </table>  
  </div>
  
</div>
</body>
</html>
```
运行结果如下所示：
![](https://img-blog.csdn.net/20161219003913925)
![](https://img-blog.csdn.net/20161219003945207)

最后希望这篇文章对你有所帮助，尤其是我的学生和JS学习者。
最近非常开心，感谢娜娜，晚安~
(By:Eastmount 2016-12-19 凌晨1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


