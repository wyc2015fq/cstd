
# PHP+HTML实现登出界面倒计时效果 - 杨秀璋的专栏 - CSDN博客

2015年02月21日 22:59:20[Eastmount](https://me.csdn.net/Eastmount)阅读数：3781所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)



在WAMP(Windows+Apache+MySQL+PHP)做网站时，通常需要使用Session记录表单登陆用户名和密码等变量，而在登出时需要清除Session。通常我想实现的登出是通过alert提示用户然后header跳转，但下面的代码是通过JavaScript实现的时间倒计时并跳转到主页的效果，主要是看到学校的BT网站等都是这个效果。
PS:参考[C\# 系统应用之Cookie\Session基础知识及php读取Cookie\Session](http://blog.csdn.net/eastmount/article/details/18767773)
代码如下：
```python
<?php
	session_start();
	session_destroy(); //清空以创建的所有SESSION
	setcookie(session_name(),'',time()-1); 
	$_SESSION = array();
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>登出系统</title>
<link  href="css/mycss.css" type="text/css" rel="stylesheet"/>
</head>
<body>	
<div id="main">
<TABLE cellSpacing=0 cellPadding=0 width="100%" 
		background="images/header_bg.gif" border=0>
	<!--头部图片-->
  <TR height=80>
    <TD width=260>
    	<IMG height=80 src="images/logo.gif" width=260>
    </TD>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
  <TR bgColor=#1c5db6 height=20>
    <TD></TD></TR>
</TABLE>
<!-- 布局中部 -->
<div id="middle">
<div id="logout_middle">
	<H3 style="COLOR: #880000">注销成功</H3>
	<H4>您已经登出系统！欢迎下次登录!<br />系统将在
	<input type="text" style='font-size:18px; border:0px; width:20px;' 
		readonly="true" value="5" id="time">秒后返回主页游客查看界面
	</H4>
</div>
<!-- 时间函数 -->
<script language="javascript">
	var t = 5;
	var time = document.getElementById("time");
	function fun()
	{
		t--;
		time.value = t;
		if(t<=0)
		{
			location.href="index.php";
			clearInterval(inter);
		}
	}
	var inter = setInterval("fun()",1000);
</script>
</body>
</html>
```
实现的效果如下：
![](https://img-blog.csdn.net/20150221225239832)
最后希望文章对大家有所帮助，也尝试不同风格，写点短文章。
(By:Eastmount 2015-2-21[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


