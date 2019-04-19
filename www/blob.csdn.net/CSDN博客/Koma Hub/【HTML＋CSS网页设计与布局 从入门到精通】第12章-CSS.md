# 【HTML＋CSS网页设计与布局 从入门到精通】第12章-CSS - Koma Hub - CSDN博客
2019年01月07日 22:05:24[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：43
个人分类：[CSS																[HTML](https://blog.csdn.net/Rong_Toa/article/category/7651718)](https://blog.csdn.net/Rong_Toa/article/category/8590132)
**目录**
[开始](#%E5%BC%80%E5%A7%8B)
[背景+颜色](#%E8%83%8C%E6%99%AF%2B%E9%A2%9C%E8%89%B2)
[将图片当做背景background-image:url(bg-h.gif);](#%E5%B0%86%E5%9B%BE%E7%89%87%E5%BD%93%E5%81%9A%E8%83%8C%E6%99%AFbackground-image%3Aurl(bg-h.gif)%3B)
[x方向（横向）重复显示图片：](#x%E6%96%B9%E5%90%91%EF%BC%88%E6%A8%AA%E5%90%91%EF%BC%89%E9%87%8D%E5%A4%8D%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87%EF%BC%9A)
[y方向（纵向）](#y%E6%96%B9%E5%90%91%EF%BC%88%E7%BA%B5%E5%90%91%EF%BC%89)
[不重复显示图片background-repeat:no-repeat;](#%E4%B8%8D%E9%87%8D%E5%A4%8D%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87background-repeat%3Ano-repeat%3B)
[规定位置显示图片background-position:200px 100px;](#%E8%A7%84%E5%AE%9A%E4%BD%8D%E7%BD%AE%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87background-position%3A200px%20100px%3B)
[规定百分比位置显示图片background-position:30% 60%;](#%E8%A7%84%E5%AE%9A%E7%99%BE%E5%88%86%E6%AF%94%E4%BD%8D%E7%BD%AE%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87background-position%3A30%25%2060%25%3B)
[附着显示图片background-attachment:fixed;](#%E9%99%84%E7%9D%80%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87background-attachment%3Afixed%3B)
[中心显示图片background-position:center;](#%E4%B8%AD%E5%BF%83%E6%98%BE%E7%A4%BA%E5%9B%BE%E7%89%87background-position%3Acenter%3B)
[span display：none](#span%20display%EF%BC%9Anone)
[span display：block](#span%20display%EF%BC%9Ablock)
# 开始
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体}
p{font-family: Arial, "Times New Roman"}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 背景+颜色
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
   background-color:blue;
   color:white;}
p{font-family: Arial, "Times New Roman"}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 将图片当做背景background-image:url(bg-h.gif);
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(bg.gif);
}
h1{font-family:黑体;
   background-image:url(bg-h.gif);
   color:black;}
p{font-family: Arial, "Times New Roman";
  background-color:white}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# x方向（横向）重复显示图片：
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(bg-g.jpg);
	background-repeat:repeat-x;
    background-color:#D2D2D2
}
h1{font-family:黑体;
   color:white;}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
## y方向（纵向）
`background-repeat:repeat-y;`
# 不重复显示图片background-repeat:no-repeat;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(cup.gif);
	background-repeat:no-repeat;
	background-position:right bottom;
	}
	
h1{font-family:黑体;
   color:black;}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 规定位置显示图片background-position:200px 100px;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(cup.gif);
	background-repeat:no-repeat;
	background-position:200px 100px;
	}
	
h1{font-family:黑体;
   color:black;}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 规定百分比位置显示图片background-position:30% 60%;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(cup.gif);
	background-repeat:no-repeat;
	background-position:30% 60%;
	}
	
h1{font-family:黑体;
   color:black;}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 附着显示图片background-attachment:fixed;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(cup.gif);
	background-repeat:no-repeat;
	background-position:30% 60%;
	background-attachment:fixed;
	}
	
h1{font-family:黑体;
   color:black;}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 中心显示图片background-position:center;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(bg-g.jpg);
	background-repeat:repeat-x;
    background-color:#D2D2D2
}
h1{ height:40px;
	 background-image:url(h1.jpg);
	 background-repeat:no-repeat;
	 background-position:center;
}
p{font-family: Arial, "Times New Roman";
  }
-->
</style>
</head>
<body>
<h1><span>互联网发展的起源</span></h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# span display：none
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
body{
	background-image:url(bg-g.jpg);
	background-repeat:repeat-x;
    background-color:#D2D2D2
}
h1{ height:40px;
	 background-image:url(h1.jpg);
	 background-repeat:no-repeat;
	 background-position:center;
}
p{font-family: Arial, "Times New Roman";
  }
span{
	display:none;
}
-->
</style>
</head>
<body>
<h1><span>互联网发展的起源</span></h1>
<p id="p1">A very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# span display：block
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>滑动门演示</title>
<style type="text/css">
h3{
	font-size:13px;
	line-height:21px;
	text-align:center;
	background-image:url(bg-slide.gif);
	background-repeat:no-repeat;
	padding-left:40px;
	color:white;
}
span{
	display:block;
	padding-right:40px;
	background-image:url(bg-slide.gif);
	background-repeat:no-repeat;
	background-position:right;
}
.wide{
	width:300px;  /*修改这个数值，即可改变宽度，可以保持两端的花纹*/
}
	
.narrow{
	width:200px;  /*修改这个数值，即可改变宽度，可以保持两端的花纹*/
}
</style>
</head>
<body>
<h3 class="wide"><span>h3标题文字（宽）</span></h3>
<h3 class="narrow"><span>h3标题文字（窄）</span></h3>
<p>说明：只需要修改h3标记CSS中的width属性即可，只要宽度不超过背景图像的总宽度即可。</p>
</body>
</html>
```
