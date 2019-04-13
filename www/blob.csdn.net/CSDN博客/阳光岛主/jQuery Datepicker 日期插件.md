
# jQuery Datepicker 日期插件 - 阳光岛主 - CSDN博客

2013年08月09日 00:28:28[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3887


Datepicker日期选择插件是一个配置灵活的插件，可以自定义其展示方式，包括日期格式、语言、限制选择日期范围、添加相关按钮以及其它导航等。

**1、jQuery Datepicker**
示例代码：
```python
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <title>jQuery UI Datepicker - Default functionality</title>
  <link rel="stylesheet" href="http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css" />
  <script src="http://code.jquery.com/jquery-1.9.1.js"></script>
  <script src="http://code.jquery.com/ui/1.10.3/jquery-ui.js"></script>
  <link rel="stylesheet" href="/resources/demos/style.css" />
  <script>
	$(function() {
		$( "#datepicker" ).datepicker();
	});
  </script>
</head>
<body>
	<p>Date: <input type="text" id="datepicker" /></p>
</body>
</html>
```

**运行结果：**![](https://img-blog.csdn.net/20130809002228140)


**2、my97 Datepicker**
示例代码：
```python
<html>
<head>
<link type="text/css" href="css/ui-lightness/jquery-ui-1.8.16.custom.css" rel="stylesheet" />	
	<script type="text/javascript" src="WdatePicker.js"></script>
</head>
<body>
<input id="d424" class="Wdate" type="text" onfocus="WdatePicker({dateFmt:'yyyy-MM-dd HH:mm:ss'})"/>
</body>
</html>
```
运行结果：
![](https://img-blog.csdn.net/20130809002535625)

[源码下载](http://download.csdn.net/detail/sunboy_2050/5894887)



[jQuery Datepicker](http://jqueryui.com/datepicker/)
[my97 Datepicker](http://www.my97.net/dp/demo/index.htm)（推荐）
[jquery datepicker 时分秒](http://download.csdn.net/download/lhf214111967/4293285)
[JQuery UI datepicker 使用方法](http://www.cnblogs.com/rabbit2012/archive/2013/02/22/2922607.html)


