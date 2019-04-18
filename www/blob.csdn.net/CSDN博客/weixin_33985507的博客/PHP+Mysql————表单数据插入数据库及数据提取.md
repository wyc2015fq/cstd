# PHP+Mysql————表单数据插入数据库及数据提取 - weixin_33985507的博客 - CSDN博客
2014年10月18日 19:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

**站点在进行新用户注冊时，都会将用户的注冊信息存入数据库中，须要的时候再进行提取。今天写了一个简单的实例。**
**主要完毕下面几点功能：**
**（1）用户进行注冊，实现password反复确认，验证码校对功能。**
**（2）注冊成功后，将用户进行插入数据库中进行保存。**
**（3）将数据库表中数据进行提取，并打印。**
# **1.注冊表单**
**在曾经的几篇博客中，分享过注冊及登录表单的代码。这次的代码，大致同样，仅仅是略有变化。仅作为实例探讨**
**表单页面实在没什么可讲的，除了格式对齐上加几个 （空格）。**
```php
<html>
	<head>
		<title>注冊页面</title>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	</head>
	<body>
		<h1>新用户注冊</h1>
		<hr>
		<form method="post" action="regsubmit.php">
			<table>
				<tr>
					<td>请输入用户名：<input type="text" name="username"></td>
				</tr>
	
				<tr>
					<td>请输入密码：    <input type="password" name="password"></td>
				</tr>
				<tr>
					<td>请确认密码：    <input type="password" name="repassword"></td>
				</tr>
				<tr>
					<td>请输入验证码：<input type="text" name="vcode"><img src="regauth.php"></td>
				</tr>
				<tr>
					<td><input type="submit" value="注冊">    <input type="reset" value="重置"></td>
					
				</tr>
			</table>
		</form>
	</body>
</html>
```
**效果图：**
**![](https://img-blog.csdn.net/20141006230850997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pxbmxz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
## 2.验证码页面
```php
<strong><span style="font-family:KaiTi_GB2312;font-size:18px;"><?php
	header("content-type:text/html charset=utf-8");
	
	//开启session
	session_start();
	//准备画布
	$im=imagecreatetruecolor(50,25);
	//准备颜料
	$black=imagecolorallocate($im,0,0,0);
	$gray=imagecolorallocate($im,200,200,200);
	//背景填充
	imagefill($im,0,0,$gray);
	//文字居中
	$x=(50-10*4)/2;
	$y=(25-5)/2+5;
	//加入�干扰素
	for($i=0;$i<50;$i++){
		imagesetpixel($im,mt_rand(0,50),mt_rand(0,25),$black);
	}
	//准备文字
	$arr=array_merge(range(0,9),range('a','z'),range('A','Z'));
	shuffle($arr);
	$str=implode(array_slice($arr,0,4));
	//把$str放入session中，方便全部页面中调用
	$_SESSION['vstr']=$str;
	$file="../fonts/simsun.ttc";
	imagettftext($im,15,0,$x,$y,$black,$file,$str);
	//输出到浏览器上或保存起来
	header("content-type:image/png");
	imagepng($im);
	//关闭画布
	imagedestory($im);
?></span></strong>
```
**对于验证码功能，在曾经的一篇博客中，曾进行具体的解说过。这次代码也基本直接拿过来用了，唯一升级了一点就是加入�了干扰素，使得验证码没有干巴巴的四个字符在哪里。用到了imagesetpixel()函数，用于制造一些干扰点。具体用法请查看php手冊。**
### **3.提交页面（数据提取页面）**
```php
<?php
	header("content-type:text/html;charset=utf-8");
	//开启session
	session_start();
	
	//将验证码与输入框中字符串都转为小写
	$code=strtolower($_POST['vcode']);  
	$str=strtolower($_SESSION['vstr']); 
	
	//接收表单传递的username与password
	$name=$_POST['username'];
	$pwd=$_POST['password'];
	$repwd=$_POST['repassword'];
	
	//推断密码是否一致
	if($pwd!=$repwd){
		echo"<script>alert('两次密码输入不一致，请又一次输入');</script>";
		echo"<script>location='regform.html'</script>";
	}else{
		//推断验证码是否正确
		if($code!=$str){  
			echo "<script>alert('验证码输入错误,请又一次输入');</script>";  
			echo"<script>location='regform.html'</script>";
		}else{  
			//通过php连接到mysql数据库
			$conn=mysql_connect("localhost","","");
			
			//选择数据库
			mysql_select_db("test");
			//设置client和连接字符集
			mysql_query("set names utf8");
			//通过php进行insert操作
			$sqlinsert="insert into t1(username,password) values('{$name}','{$pwd}')";
			//通过php进行select操作
			$sqlselect="select * from t1 order by id";
			//加入�用户信息到数据库
			mysql_query($sqlinsert);
			
			//返回用户信息字符集
			$result=mysql_query($sqlselect);
			
			echo "<h1>USER INFORMATION</h1>";
			echo "<hr>";
			echo "<table width='700px' border='1px'>";
			//从结果中拿出一行
			echo "<tr>";
			echo "<th>ID</th><th>USERNAME</th><th>PASSWORD</th>";
			echo "</tr>";
			while($row=mysql_fetch_assoc($result)){
				echo "<tr>";
				//打印出$row这一行
				echo "<td>{$row['id']}</td><td>{$row['username']}</td><td>{$row['password']}</td>";
				
				echo "</tr>";
			}
			echo "</table>";
			//释放连接资源
			mysql_close($conn);
						  
			} 
	}
?>
```
**这个页面的主要作用完毕了最重要的几个功能。将表单提交的数据都存入变量，然后进行password和验证码的推断，都正确以后，将用户信息存入数据库并将数据库存放用户信息的表中全部数据提取打印出来。说白了，后半句就是数据存入和提取。代码中凝视写的相当的清楚了，基本每一个用处都涉及到了。但还是有些easy疏忽的点要指出来：<1>连接数据库的函数mysql_connect("localhost","username","password")。当中的用户名和password，根据大家自己的配置进行编写，我的数据库懒得加密所以就都为空了。**
**<2>数据插入数据库时**
```php
$sqlinsert="insert into t1(username,password) values('{$name}','{$pwd}')";
```
values('','')一定要加单引號（外层加了双引號，里面仅仅能用单引號），由于本身数据就是字符串，不能直接写个变量就完事了。假设不加单引號，$name和$pwd变量解析以后变成几个字符，不在引號内，谁认识谁呀。必定会报错。
**<3>打印提取数据时，用到了while循环。里面的mysql_fetch_assoc()函数提取后返回的数组，下标就是数据所属字段名。相似功能的函数还有mysql_fetch_array(),mysql_fetch_row(),mysql_fetch_field()。详细使用请看需求，一般来说mysql_fetch_assoc()比較的经常使用。**
**效果图：**
**![](https://img-blog.csdn.net/20141006231007827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3pxbmxz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
