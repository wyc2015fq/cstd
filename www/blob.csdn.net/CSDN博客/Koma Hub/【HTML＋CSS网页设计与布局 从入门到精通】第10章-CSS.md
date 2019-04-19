# 【HTML＋CSS网页设计与布局 从入门到精通】第10章-CSS - Koma Hub - CSDN博客
2019年01月07日 21:32:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：44
**目录**
[开始](#%E5%BC%80%E5%A7%8B)
[新增设置](#%E6%96%B0%E5%A2%9E%E8%AE%BE%E7%BD%AE)
[英文字母全部小写](#%E8%8B%B1%E6%96%87%E5%AD%97%E6%AF%8D%E5%85%A8%E9%83%A8%E5%B0%8F%E5%86%99)
[字号设置](#%E5%AD%97%E5%8F%B7%E8%AE%BE%E7%BD%AE)
[首字母“大”写-大一点显示](#%E9%A6%96%E5%AD%97%E6%AF%8D%E2%80%9C%E5%A4%A7%E2%80%9D%E5%86%99-%E5%A4%A7%E4%B8%80%E7%82%B9%E6%98%BE%E7%A4%BA)
[上划线，下划线text-decoration:underline overline;](#%E4%B8%8A%E5%88%92%E7%BA%BF%EF%BC%8C%E4%B8%8B%E5%88%92%E7%BA%BFtext-decoration%3Aunderline%20overline%3B)
[缩进text-indent:2em;](#%E7%BC%A9%E8%BF%9Btext-indent%3A2em%3B)
[缩进为负数text-indent:-2em;](#%E7%BC%A9%E8%BF%9B%E4%B8%BA%E8%B4%9F%E6%95%B0text-indent%3A-2em%3B)
[字母间距letter-spacing:-1px; ，单词间距word-spacing:10px;](#%E5%AD%97%E6%AF%8D%E9%97%B4%E8%B7%9Dletter-spacing%3A-1px%3B%20%EF%BC%8C%E5%8D%95%E8%AF%8D%E9%97%B4%E8%B7%9Dword-spacing%3A10px%3B)
[行高line-height:2;](#%E8%A1%8C%E9%AB%98line-height%3A2%3B)
[边框border:1px red solid; ，行间距margin:5px 0px;](#%E8%BE%B9%E6%A1%86border%3A1px%20red%20solid%3B%20%EF%BC%8C%E8%A1%8C%E9%97%B4%E8%B7%9Dmargin%3A5px%200px%3B)
[文本布局：居中text-align:center;，恰当text-align:justify;](#%E6%96%87%E6%9C%AC%E5%B8%83%E5%B1%80%EF%BC%9A%E5%B1%85%E4%B8%ADtext-align%3Acenter%3B%EF%BC%8C%E6%81%B0%E5%BD%93text-align%3Ajustify%3B)
[背景颜色background:#678; ，字体颜色color:white;](#%E8%83%8C%E6%99%AF%E9%A2%9C%E8%89%B2background%3A%23678%3B%20%EF%BC%8C%E5%AD%97%E4%BD%93%E9%A2%9C%E8%89%B2color%3Awhite%3B)
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
# 新增设置
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;}
p{font-family: Arial, "Times New Roman"}
#p1{font-style:italic;   /*新增的设置*/}
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
# 英文字母全部小写
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;}
p{font-family: Arial, "Times New Roman"}
#p1{font-style:italic;
text-transform:capitalize;}
#p2{
text-transform:lowercase;}
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
# 字号设置
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;}
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	}
#p2{
	text-transform:lowercase;
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
# 首字母“大”写-大一点显示
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	}
#p2{
	text-transform:lowercase;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 上划线，下划线text-decoration:underline overline;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	}
#p2{
	text-transform:lowercase;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 缩进text-indent:2em;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 缩进为负数text-indent:-2em;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	}
#p2{
	text-transform:lowercase;
	text-indent:-2em;
	padding-left:2em;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 字母间距letter-spacing:-1px; ，单词间距word-spacing:10px;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	word-spacing:10px;
	letter-spacing:-1px;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 行高line-height:2;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	word-spacing:10px;
	letter-spacing:-1px;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	line-height:2;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 边框border:1px red solid; ，行间距margin:5px 0px;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;
   border:1px red solid;
   margin:5px 0px;}
#p1{
	font-style:italic;
	text-transform:capitalize;
	word-spacing:10px;
	letter-spacing:-1px;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	line-height:2;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 文本布局：居中text-align:center;，恰当text-align:justify;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
text-align:center;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;
   margin:5px 0px;
   text-align:justify;
   }
#p1{
	font-style:italic;
	text-transform:capitalize;
	word-spacing:10px;
	letter-spacing:-1px;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	line-height:2;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
# 背景颜色background:#678; ，字体颜色color:white;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 		"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>体验CSS</title>
<style type="text/css">
<!--
h1{font-family:黑体;
text-decoration:underline overline;
text-align:center;
   }
p{ font-family: Arial, "Times New Roman";
   font-size:12px;
   margin:5px 0px;
   text-align:justify;
   }
#p1{
	font-style:italic;
	text-transform:capitalize;
	word-spacing:10px;
	letter-spacing:-1px;
	}
#p2{
	text-transform:lowercase;
	text-indent:2em;
	line-height:2;
	}
#firstLetter{
	font-size:3em;
	float:left;
	}
h1{
	background:#678;
	color:white;
	}
-->
</style>
</head>
<body>
<h1>互联网发展的起源</h1>
<p id="p1"><span id="firstLetter">A</span> very simple ascii map of the first network link on ARPANET between UCLA and SRI taken from RFC-4 Network Timetable, by Elmer B. Shapiro, March 1969.</p>
<p id="p2">1969年，为了保障通信联络，美国国防部高级研究计划署ARPA资助建立了世界上第一个分组交换试验网ARPANET，连接美国四个大学。ARPANET的建成和不断发展标志着计算机网络发展的新纪元。</p>
</body>
</html>
```
