# [WAMP网站开发] PHP连接MySQL数据库基础知识 - 杨秀璋的专栏 - CSDN博客





2017年02月24日 12:02:18[Eastmount](https://me.csdn.net/Eastmount)阅读数：6154
所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)









这篇文章主要是最近教学生使用WAMP(Windows+Apache+MySQL+PHP)开发网站，HTML布局首页，PHP连接数据库以及XAMPP操作数据库的基础文章，更是一篇教学文章。文章比较基础，希望对能给PHP网站开发的学生做个入门介绍。




## 一. HTML简单布局首页

首先是页面布局，这里采用我以前给同学做的火车票的简单网站模板。
主要是中间那部分DIV需要设计成模拟的跳转界面。代码如下：


```
<!DOCTYPE>
<html>
  <head>
    <title>火车票订票系统管理系统</title>
    <style type="text/css">
    	#middle {  
		    margin:0 auto;         /* 水平居中 */  
		    margin-top: 10px;  
		    width: 80%;  
		    height: 60%;      
		    background-color: #E6E6FA;  
		}  
		  
		#left {  
		    float: left;  
		    margin-left: 20px;  
		    margin-top: 5px;  
		    width: 25%;  
		    height: 95%;  
		    background-color: #9ACD32;  
		}  
		  
		#right {  
		    float: left;  
		    margin-left: 20px;  
		    margin-top: 5px;  
		    width: 70%;  
		    height: 95%;  
		    background-color: #BFEFFF;  
		}  
		  
    </style>

  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	</div> 	

	<!-- 中部内容查询设计 -->
	<div id="middle">
		<div id="left">
		</div>
		<div id="right">
		</div>
	</div>

	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 贵州财经大学 Eastmount   <BR>
		      	信息学院10006号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
  </body>
</html>
```
运行结果如下所示：


![](https://img-blog.csdn.net/20170224092343679)

这个布局分位顶部（logo）、中部（核心）、底部（署名）三部分，同时中部又分位左部和右部，接下来修改中间，做成左边点击图片，右边跳转的形式。
<!-- 中部内容查询设计 -->
<div id="middle">
<div id="left">
                         添加左边图片超链接
</div>
<div id="right">
                        右边显示具体操作后的结果
</div>
</div>
这部分代码修改为：

```
<!-- 中部内容查询设计 -->
	<div id="middle">
		<div id="left">
		<table width="100%" cellpadding="1" 
			cellspacing="1"  border="1">
			<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="02main.html">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
		</table>
		</div>
		
		<div id="right">
			<table width="100%" height="98%" border="1" cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><td align="left" valign="top" style="padding-left:20px;padding-top:10px;">
					当前位置: <a href="main.jsp">首页</a> > 车次信息管理
					<br /><br />
				<form action="main.jsp" method="post">
					出发地: 
					<input type="text" id="start" name="start" style='font-size:18px' width=200/>
					到达地: <input type="text" id="end" name="end" style='font-size:18px'/><br /><br />
					<input type="submit" name="Select" value="查询车次" style='font-size:18px;'/>
				</form>
					<table width="90%" height="50%" 
					align="cneter" border="1" cellpadding="1" cellspacing="1" >
						<tr style="padding-left:20px;padding-top:10px;">
							<td >AAA</td>
							<td>BBB</td>
						</tr>
						<tr>
							<td>111</td>
							<td>222</td>
						</tr>
					</table>
				</td></tr>
		</table>
		</div>
	</div>
```
运行结果如下所示：


![](https://img-blog.csdn.net/20170224094341426)

注意：这是假的局部跳转，因为相当于左边两张图片切换（灰底、蓝底），然后跳转到新的页面，布局和该页面一样，只是图片切换成灰色，其他蓝色，造成那种部分刷新的效果。哈哈~





## 二. Apache访问PHP网站

接下来，打开XAMPP软件，打开Apache，然后点击"Explore"。



![](https://img-blog.csdn.net/20170224094751108)

打开文件夹，去到htdocs文件夹下，创建"20170224YXZ"文件夹。
PHP通过Apache访问的文件都需放置在该文件夹下，再通过浏览器进行访问。


![](https://img-blog.csdn.net/20170224095339095)




通常我会修改文件后缀名，如："main.php"。



![](https://img-blog.csdn.net/20170224100824743)

再通过浏览器进行访问，如果80端口被占用，可以换成8080端口。
访问地址如下：http://localhost:8080/20170224YXZ/main.php
http://localhost:端口号/文件夹/文件名
配置参考：[PHP XAMPP配置PHP环境和Apache80端口被占用解决方案](http://blog.csdn.net/eastmount/article/details/11823549)


![](https://img-blog.csdn.net/20170224101402495)





## 三. XAMPP操作MySQL

该部分讲述如何通过XAMPP软件简单的操作MySQL。
点击XAMPP中MySQL按钮"Start"，然后点击"Admin"，访问数据库。



![](https://img-blog.csdn.net/20170224102150748)

默认的80被占用的情况下，网址可能需要加上端口8080。
http://localhost:8080/phpmyadmin/
如下图，左部为已经创建的数据库，顶部为各种操作，管理"数据库"、"SQL"等。

![](https://img-blog.csdn.net/20170224102500607)


**第一步 创建数据库**
数据库名称"HCPManage"，火车票管理系统，编码方式utf8，点击"创建"。



![](https://img-blog.csdn.net/20170224103449567)

**第二步 创建表**
新建表infomation，共3个字段。
name 火车车次名，price 火车价格，start 起始地。


![](https://img-blog.csdn.net/20170224104050847)


![](https://img-blog.csdn.net/20170224104632996)

**第三步 插入数据**
点击"插入"，然后插入数据即可。


![](https://img-blog.csdn.net/20170224105948907)


![](https://img-blog.csdn.net/20170224110532347)

**第四步 显示插入的数据**
点击表infomation，可以查看已经插入的数据。


![](https://img-blog.csdn.net/20170224110707852)

**第五步 SQL语句**
点击表infomation，可以查看已经插入的数据。


![](https://img-blog.csdn.net/20170224111346429)





## 四. PHP连接数据库

用PHP进行MySQL数据库编程的基本步骤：
      1 首先建立与MySQL数据库服务器的连接
      2 然后选择要对其进行操作的数据库
      3 再执行相应的数据库操作（对数据的添加、删除、修改和查询等）
      4 最后关闭与MySQL数据库服务器的连接

核心代码及函数如下：
**建立数据库服务器的连接**
mysql_connect([servername[,username[,password]]])

**选择数据库**
mysql_select_db(database[,connection])

**执行数据库操作**
Mysql_query(query[,connection])
函数返回值是资源句柄型。对于select语句,若执行成功，则返回相应的结果标识符，否则返回false；而对于insert、delete、update等非检索语句，若执行成功，则返回true，否则返回false。

**读取结果集**
Mysql_fetch_row(data) 
Mysql_fetch_assoc(data)
Mysql_fetch_array(data[,array_type])
三个函数成功被执行后，其返回值均为数组类型(array)。

**关闭数据库服务器的连接**
Mysql_close([connection])

我们上面连接数据库信息为：
数据库 hcpmanage 表infomation 默认用户 root 密码 123456
核心代码，见sql.php：


```php
<!-- PHP连接数据库  -->
<?php
	//链接数据库
	$con = mysql_connect("localhost", "root", "123456"); 
	if (!$con) 
	{ 
		die('Could not connect database: ' . mysql_error()); 
	} 

	//选择数据库
	$db_selected = mysql_select_db("hcpmanage", $con); 
	if (!$db_selected) 
	{ 
	 	die ("Can\'t use yxz : " . mysql_error()); 
	} 

	//执行MySQL查询-设置UTF8格式
	mysql_query("SET NAMES utf8"); 	

	//查询学生信息
	$sql = "SELECT * FROM `infomation` ";
	$result = mysql_query($sql,$con); 
	$n=0;
	$id;
	$name;

	while ($row = mysql_fetch_row($result))
	{ 
		list($name,$price,$start) = $row;   
		$n++;

		echo $n.' ';  
		echo $name.' ';
		echo $price.' ';
		echo $start;
		echo '<br >';
	}

?>
```
输出如下图所示：


![](https://img-blog.csdn.net/20170224115307727)


## 五. HTML结合PHP输出
完整代码，注意反括号：

```
<!DOCTYPE>
<html>
  <head>
    <title>火车票订票系统管理系统</title>
    <style type="text/css">
    	#middle {  
		    margin:0 auto;         /* 水平居中 */  
		    margin-top: 10px;  
		    width: 80%;  
		    height: 60%;      
		    background-color: #E6E6FA;  
		}  
		  
		#left {  
		    float: left;  
		    margin-left: 20px;  
		    margin-top: 5px;  
		    width: 25%;  
		    height: 95%;  
		    background-color: #9ACD32;  
		}  
		  
		#right {  
		    float: left;  
		    margin-left: 20px;  
		    margin-top: 5px;  
		    width: 70%;  
		    height: 95%;  
		    background-color: #BFEFFF;  
		}  
		  

    </style>

  </head>
  <body>
    <div align="center">
	   <div class="trainCSS">
	   <table width="1024"  height="150" cellpadding="0" cellspacing="0">
			<tr><td colspan="2">
				<img src="./image/logo.jpg" alt="logo" width="1024" height="149">		
			</td></tr>		
			<tr><td width="205"  bgcolor="#b7d7ec">
				<p align="center">
				<script language=JavaScript>
				today=new Date();
				function initArray(){
				this.length=initArray.arguments.length;
				for(var i=0;i<this.length;i++)
				this[i+1]=initArray.arguments[i];  }
				var d=new initArray(
				"星期日","星期一","星期二","星期三","星期四",	"星期五","星期六");
				document.write("<font color=##ff0000 style='font-size:12pt;font-family: 宋体'> ",
				today.getYear()+1900,"年",today.getMonth()+1,"月",today.getDate(),"日    ",d[today.getDay()+1],
							  "</font>" );
				</script> 
				</p>	
			    </td>	
				<td width="819" bgcolor="#b7d7ec">
				<marquee  direction="left" onmouseover=this.stop() onmouseout=this.start() scrollAmount=3 scrollDelay=100>
	             <FONT style="FONT-SIZE: 18px"color=blue>欢迎使用火车票订票系统管理系统 ，如有不足，敬请指导！</FONT>
	            </marquee>
	            </td>
	        </tr>
	        <tr><td height="12"></td></tr>
	  </table>
	  </div>
	</div> 	

	<!-- 中部内容查询设计 -->
	<div id="middle">
		<div id="left">
		<table width="100%" cellpadding="1" 
			cellspacing="1"  border="1">
			<tr><td height="10"></td></tr>
			<tr><td align="center">	<font size="3" color="blue">管理员：xxx，欢迎您！</font></td></tr>
			<tr><td height="10"></td></tr>
			<tr>
				<td align="center"><p><a href="trainManage.php">
				<img src="image/ccxxgl-xz.jpg" width="194" height="37" border="0"></a><td> </td>
			</tr>
			<tr><td height="10"></td></tr>
			<tr><td align="center"><a href="https://www.baidu.com/">
				<img src="image/dpgl.jpg" width="194" height="37" border="0"></a>
			</td></tr>
			<tr><td height="10"></td></tr>
		</table>
		</div>

		<div id="right">
		<table width="100%" height="98%" border="1" cellpadding="0" cellspacing="0"  bgcolor="#dfeaf1">
				<tr><td align="left" valign="top" style="padding-left:20px;padding-top:10px;">
					当前位置: <a href="homepage.jsp">首页</a> > 车次信息管理
					<br /><br />
				<form action="trainManageSelect.jsp" method="post">
					出发地: 
					<input type="text" id="start" name="start" style='font-size:18px' width=200/>
					到达地: <input type="text" id="end" name="end" style='font-size:18px'/><br /><br />
					<input type="submit" name="Select" value="查询车次" style='font-size:18px;'/>
				</form>
					<table width="90%" height="50%" 
					align="cneter" border="1" cellpadding="1" cellspacing="1" >
<!-- PHP连接数据库  -->
<?php
	//链接数据库
	$con = mysql_connect("localhost", "root", "123456"); 
	if (!$con) 
	{ 
		die('Could not connect database: ' . mysql_error()); 
	} 

	//选择数据库
	$db_selected = mysql_select_db("hcpmanage", $con); 
	if (!$db_selected) 
	{ 
	 	die ("Can\'t use yxz : " . mysql_error()); 
	} 

	//执行MySQL查询-设置UTF8格式
	mysql_query("SET NAMES utf8"); 	

	//查询学生信息
	$sql = "SELECT * FROM `infomation` ";
	$result = mysql_query($sql,$con); 
	$n=0;
	$id;
	$name;

	while ($row = mysql_fetch_row($result))
	{ 
		list($name,$price,$start) = $row;   
		$n++;

?>
						<tr>
							<td><?php echo $n.' '; ?></td>
							<td><?php echo $name.' '; ?></td>
							<td><?php echo $price.' '; ?></td>
							<td><?php echo $start; ?></td>
						</tr>
<?php
	}
?>
					</table>
				</td></tr>
				
		</table>
		</div>
	</div>


	<div class="div">
	<!-- 底部版权所有界面 -->
	<TABLE class=wrapper border=0 cellSpacing=0 cellPadding=0 width="100%" align=center>
		 <TBODY>
		  <TR><TD style="LINE-HEIGHT: 100%" align="center"> 
		      <HR style="WIDTH: 96%; HEIGHT: 4px; COLOR: #02457c">
		     <font size="2"> <SCRIPT language=javascript src=""></SCRIPT>
		     <!-- target=_blank开启新的网页 -->
		     <BR><A href="aboutme.jsp">关于我们</A> | <A href="wzsm.jsp" 
				>网站声明</A> <BR>版权所有&copy;2014-2015 贵州财经大学 Eastmount   <BR>
		      	信息学院10006号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
输出如下图所示：


![](https://img-blog.csdn.net/20170224115816000)


        希望基础性文章对你有所帮助，如果文章中存在不足或错误的地方还请您海涵~
        不管秀璋本人多么平庸，但我总觉得对你的爱真的很美。
        (By:Eastmount 2017-02-24 中午12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))






