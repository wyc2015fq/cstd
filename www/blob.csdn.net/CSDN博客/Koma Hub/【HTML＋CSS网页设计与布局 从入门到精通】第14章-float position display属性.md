# 【HTML＋CSS网页设计与布局 从入门到精通】第14章-float/position/display属性 - Koma Hub - CSDN博客
2019年01月08日 21:52:55[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：39
个人分类：[CSS																[HTML](https://blog.csdn.net/Rong_Toa/article/category/7651718)](https://blog.csdn.net/Rong_Toa/article/category/8590132)
**目录**
[float属性](#float%E5%B1%9E%E6%80%A7)
[float：left](#float%EF%BC%9Aleft)
[float：left-例2](#float%EF%BC%9Aleft-%E4%BE%8B2)
[float：left-例3](#float%EF%BC%9Aleft-%E4%BE%8B3)
[float:right;](#float%3Aright%3B)
[对比father p{clear:right;}](#%E5%AF%B9%E6%AF%94father%20p%7Bclear%3Aright%3B%7D)
[例1](#%E4%BE%8B1)
[例2](#%E4%BE%8B2)
[.father .clear{...}](#.father%20.clear%7B...%7D)
[position属性](#position%E5%B1%9E%E6%80%A7)
[div position](#div%20position)
[例1](#%E4%BE%8B1)
[例2](#%E4%BE%8B2)
[position:absolute;](#position%3Aabsolute%3B)
[例1](#%E4%BE%8B1)
[例2：position:absolute](#%E4%BE%8B2%EF%BC%9Aposition%3Aabsolute)
[display：inline/block/none](#display%EF%BC%9Ainline%2Fblock%2Fnone)
# float属性
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
}
.son2{
/* 这里设置son1的浮动方式*/
}
.son3{
/* 这里设置son1的浮动方式*/
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3</div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108212741182.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# float：left
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son2{
/* 这里设置son1的浮动方式*/
}
.son3{
/* 这里设置son1的浮动方式*/
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3</div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108212822763.PNG)
# float：left-例2
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son2{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son1的浮动方式*/
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3</div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108212955644.PNG)
# float：left-例3
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son2{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son1的浮动方式*/
float:left;
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3</div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108213132145.PNG)
## float:right;
```
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son2{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son1的浮动方式*/
float:right;
}
```
# 对比father p{clear:right;}
## 例1
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son3 的浮动方式*/
float:left;
}
.son2 {
/* 这里设置son2 的浮动方式*/
float:right;
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3</div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108213634787.PNG)
## 例2
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	clear:right;
	}
	
.son1{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son3 的浮动方式*/
float:right;
}
.son2 {
/* 这里设置son2 的浮动方式*/
float:left;
}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3<br/>Box-3<br/>Box-3<br/>Box-3<br/></div>
		<p>这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字，这里是浮动框外围的文字.</p>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108213707384.PNG)
# .father .clear{...}
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>float属性</title>
<style type="text/css">
body{
	margin:15px;
	font-family:Arial; font-size:12px;
	}
.father{
	background-color:#ffff99;
	border:1px solid #111111;
	padding:5px;				
	}
.father div{
	padding:10px;				
	margin:15px;					
	border:1px dashed #111111;
	background-color:#90baff;
	}
.father p{
	border:1px dashed #111111;
	background-color:#ff90ba;
	clear:right;
	}
	
.son1{
float:left;
/* 这里设置son1的浮动方式*/
}
.son2{
/* 这里设置son1的浮动方式*/
float:left;
}
.son3{
/* 这里设置son1的浮动方式*/
float:right;
}
.father .clear{
margin:0;
padding:0;
border:0;
clear:both;	}
</style>
</head>
<body>
	<div class="father">
		<div class="son1">Box-1</div>
		<div class="son2">Box-2</div>
		<div class="son3">Box-3<br />
			Box-3<br />
			Box-3<br />
			Box-3</div>
		<div class="clear"></div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108214149360.PNG)
# position属性
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 					"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>position属性</title>
<style type="text/css">
body{
	margin:20px;
	font :Arial 12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
}
#block1{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
}
</style>
</head>
<body>
	<div id="father">
		<div id="block1">Box-1</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108214353932.PNG)
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 					"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>position属性</title>
<style type="text/css">
body{
	margin:20px;
	font :Arial 12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
}
#block1{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
	position:relative;		/* relative相对定位 */
	left:30px;
	top:30px;
}
</style>
</head>
<body>
	<div id="father">
		<div id="block1">Box-1</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108214406745.PNG)
# div position
## 例1
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Language" content="zh-cn" />
<title>position属性</title>
<style type="text/css">
<!--
body{
	margin:20px;
	font-family:Arial;
	font-size:12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
}
#father div{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
	}
#block1{
}
#block2{
}
-->
</style>
</head>
<body>
	<div id="father">
		<div id="block1">Box-1</div>
		<div id="block2">Box-2</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108214647641.PNG)
## 例2
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Language" content="zh-cn" />
<title>position属性</title>
<style type="text/css">
<!--
body{
	margin:20px;
	font-family:Arial;
	font-size:12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
}
#father div{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
	}
#block1{
position:relative;
bottom:30px;
right:30px;
}
#block2{
position:relative;
top:30px;
left:30px;
}
-->
</style>
</head>
<body>
	<div id="father">
		<div id="block1">Box-1</div>
		<div id="block2">Box-2</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/2019010821471551.PNG)
# position:absolute;
## 例1
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>absolute属性</title>
<style type="text/css">
body{
	margin:20px;
	font-family:Arial;
	font-size:12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
}
#father div{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
	}
#block2{
}
</style>
</head>
<body>
	<div id="father">
		<div >Box-1</div>
		<div id="block2">Box-2</div>
		<div >Box-3</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108214938875.PNG)
## 例2：position:absolute
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>absolute属性</title>
<style type="text/css">
body{
	margin:20px;
	font-family:Arial;
	font-size:12px;
}
#father{
	background-color:#a0c8ff;
	border:1px dashed #000000;
	padding:15px;
	position:relative;
}
#father div{
	background-color:#fff0ac;
	border:1px dashed #000000;
	padding:10px;
	}
#block2{
position:absolute;
top:0px;
right:0px;
}
</style>
</head>
<body>
	<div id="father">
		<div >Box-1</div>
		<div id="block2">Box-2</div>
		<div >Box-3</div>
	</div>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108215021874.PNG)
# display：inline/block/none
```
<!DOCTYPE html PUBliC "-//W3C//DTD XHTML 1.0 Transitional//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>display属性</title>
</head>
<body>
	<div style="display:inline">Box-1</div>
	<div style="display:inline">Box-2</div>
	<div style="display:inline">Box-3</div>
	<span style="display:block">Box-4</span>
	<span style="display:block">Box-5</span>
	<span style="display:block">Box-6</span>
	<div style="display:none">Box-7</div>
	<span style="display:none">Box-8</span>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/2019010821522063.PNG)
