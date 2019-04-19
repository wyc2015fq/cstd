# 【HTML＋CSS网页设计与布局 从入门到精通】第15章-表格格式：边距/居中/边框/折叠 - Koma Hub - CSDN博客
2019年01月08日 22:05:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：28
个人分类：[CSS																[HTML](https://blog.csdn.net/Rong_Toa/article/category/7651718)](https://blog.csdn.net/Rong_Toa/article/category/8590132)
**目录**
[初始表格](#%E5%88%9D%E5%A7%8B%E8%A1%A8%E6%A0%BC)
[边距、居中、边框](#%E8%BE%B9%E8%B7%9D%E3%80%81%E5%B1%85%E4%B8%AD%E3%80%81%E8%BE%B9%E6%A1%86)
[单元格边距cellspacing](#%E5%8D%95%E5%85%83%E6%A0%BC%E8%BE%B9%E8%B7%9Dcellspacing)
[边距折叠（两个叠加不会变粗）border-collapse:collapse;](#%E8%BE%B9%E8%B7%9D%E6%8A%98%E5%8F%A0%EF%BC%88%E4%B8%A4%E4%B8%AA%E5%8F%A0%E5%8A%A0%E4%B8%8D%E4%BC%9A%E5%8F%98%E7%B2%97%EF%BC%89border-collapse%3Acollapse%3B)
[单元格边距border-spacing](#%E5%8D%95%E5%85%83%E6%A0%BC%E8%BE%B9%E8%B7%9Dborder-spacing)
# 初始表格
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>奖牌榜</title>
</head>
<body>
<table border="2" cellpadding="2" cellspacing="2" bgcolor="#eeeeee">
    <caption>期中考试成绩单</caption>
	<tr>
		<th>姓名</th> <th>物理</th> <th>化学</th> <th>数学</th> <th>总分</th>
	</tr>
	<tr>
		<th>牛小顿</th> <td>32</td> <td>17</td> <td>14</td> <td>63</td>
	</tr>
	<tr>
		<th>伽小略</th> <td>28</td> <td>16</td> <td>15</td><td >59</td>
	</tr>
	<tr>
		<th>薛小谔</th> <td>26</td> <td>22</td> <td>12</td> <td>60</td>
	</tr>
	<tr>
		<th>海小堡</th> <td>16</td> <td>22</td> <td>16</td> <td>54</td>
	</tr>
	<tr>
		<th>波小尔</th> <td>25</td> <td>11</td> <td>12</td><td >48</td>
	</tr>
	<tr>
		<th>狄小克</th> <td>15</td> <td>8</td> <td>9</td> <td>32</td>
	</tr>
</table>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108215854479.PNG)
# 边距、居中、边框
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>成绩单</title>
<style type="text/css">
.record{
    font: 14px 宋体;
	border:2px #777 solid;
	text-align:center;
}
.record td{
	border:1px #777 dashed;
}
.record th{
	border:1px #777 solid;
}
</style>
</head>
<body>
<table class="record">
    <caption>期中考试成绩单</caption>
	<tr>
		<th>姓名</th> <th>物理</th> <th>化学</th> <th>数学</th> <th>总分</th>
	</tr>
	<tr>
		<th>牛小顿</th> <td>32</td> <td>17</td> <td>14</td> <td>63</td>
	</tr>
	<tr>
		<th>伽小略</th> <td>28</td> <td>16</td> <td>15</td><td >59</td>
	</tr>
	<tr>
		<th>薛小谔</th> <td>26</td> <td>22</td> <td>12</td> <td>60</td>
	</tr>
	<tr>
		<th>海小堡</th> <td>16</td> <td>22</td> <td>16</td> <td>54</td>
	</tr>
	<tr>
		<th>波小尔</th> <td>25</td> <td>11</td> <td>12</td><td >48</td>
	</tr>
	<tr>
		<th>狄小克</th> <td>15</td> <td>8</td> <td>9</td> <td>32</td>
	</tr>
</table>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108220036672.PNG)
# 单元格边距cellspacing
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>成绩单</title>
<style type="text/css">
.record{
    font: 14px 宋体;
	border:2px #777 solid;
	text-align:center;
}
.record td{
	border:1px #777 dashed;
}
.record th{
	border:1px #777 solid;
}
</style>
</head>
<body>
<table class="record" cellspacing="0">
    <caption>期中考试成绩单</caption>
	<tr>
		<th>姓名</th> <th>物理</th> <th>化学</th> <th>数学</th> <th>总分</th>
	</tr>
	<tr>
		<th>牛小顿</th> <td>32</td> <td>17</td> <td>14</td> <td>63</td>
	</tr>
	<tr>
		<th>伽小略</th> <td>28</td> <td>16</td> <td>15</td><td >59</td>
	</tr>
	<tr>
		<th>薛小谔</th> <td>26</td> <td>22</td> <td>12</td> <td>60</td>
	</tr>
	<tr>
		<th>海小堡</th> <td>16</td> <td>22</td> <td>16</td> <td>54</td>
	</tr>
	<tr>
		<th>波小尔</th> <td>25</td> <td>11</td> <td>12</td><td >48</td>
	</tr>
	<tr>
		<th>狄小克</th> <td>15</td> <td>8</td> <td>9</td> <td>32</td>
	</tr>
</table>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108220139206.PNG)
# 边距折叠（两个叠加不会变粗）border-collapse:collapse;
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>成绩单</title>
<style type="text/css">
.record{
    font: 14px 宋体;
	border:2px #777 solid;
	text-align:center;
	border-collapse:collapse;
}
.record td{
	border:1px #777 dashed;
}
.record th{
	border:1px #777 solid;
}
</style>
</head>
<body>
<table class="record" cellspacing="0">
    <caption>期中考试成绩单</caption>
	<tr>
		<th>姓名</th> <th>物理</th> <th>化学</th> <th>数学</th> <th>总分</th>
	</tr>
	<tr>
		<th>牛小顿</th> <td>32</td> <td>17</td> <td>14</td> <td>63</td>
	</tr>
	<tr>
		<th>伽小略</th> <td>28</td> <td>16</td> <td>15</td><td >59</td>
	</tr>
	<tr>
		<th>薛小谔</th> <td>26</td> <td>22</td> <td>12</td> <td>60</td>
	</tr>
	<tr>
		<th>海小堡</th> <td>16</td> <td>22</td> <td>16</td> <td>54</td>
	</tr>
	<tr>
		<th>波小尔</th> <td>25</td> <td>11</td> <td>12</td><td >48</td>
	</tr>
	<tr>
		<th>狄小克</th> <td>15</td> <td>8</td> <td>9</td> <td>32</td>
	</tr>
</table>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108220341733.PNG)
# 单元格边距border-spacing
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>成绩单</title>
<style type="text/css">
.record{
    font: 14px 宋体;
	border:2px #777 solid;
	text-align:center;
	border-spacing:10px;
}
.record td{
	border:1px #777 dashed;
}
.record th{
	border:1px #777 solid;
}
</style>
</head>
<body>
<table class="record" cellspacing="0">
    <caption>期中考试成绩单</caption>
	<tr>
		<th>姓名</th> <th>物理</th> <th>化学</th> <th>数学</th> <th>总分</th>
	</tr>
	<tr>
		<th>牛小顿</th> <td>32</td> <td>17</td> <td>14</td> <td>63</td>
	</tr>
	<tr>
		<th>伽小略</th> <td>28</td> <td>16</td> <td>15</td><td >59</td>
	</tr>
	<tr>
		<th>薛小谔</th> <td>26</td> <td>22</td> <td>12</td> <td>60</td>
	</tr>
	<tr>
		<th>海小堡</th> <td>16</td> <td>22</td> <td>16</td> <td>54</td>
	</tr>
	<tr>
		<th>波小尔</th> <td>25</td> <td>11</td> <td>12</td><td >48</td>
	</tr>
	<tr>
		<th>狄小克</th> <td>15</td> <td>8</td> <td>9</td> <td>32</td>
	</tr>
</table>
</body>
</html>
```
![](https://img-blog.csdnimg.cn/20190108220435843.PNG)
