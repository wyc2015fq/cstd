
# [WAMP网站开发] PHP实现select新闻查询及超链接详情跳转 - 杨秀璋的专栏 - CSDN博客

2017年05月02日 16:49:36[Eastmount](https://me.csdn.net/Eastmount)阅读数：2691所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)



这篇文章是最近教学生使用WAMP(Windows+Apache+[MySQL](http://lib.csdn.net/base/mysql)+[PHP](http://lib.csdn.net/base/php))开发网站的基础知识。前文 "[[WAMP网站开发] PHP连接MySQL数据库基础知识](http://blog.csdn.net/eastmount/article/details/56685372)" 讲解了HTML布局首页，PHP连接[数据库](http://lib.csdn.net/base/mysql)以及XAMPP操作数据库等基础知识。这篇文章主要讲解网站新闻查询，点击每条新闻查看详情的功能。
这是是一篇教学文章，比较基础，主要是给PHP网站开发的学生做个入门介绍，希望对你有所帮助，不足之处还请海涵。
下载地址：


## 一. 运行结果
运行结果如下图所示，显示所有的新闻信息。注意：这里我使用我从CSDN爬取我的博客信息数据库作为新闻信息。
爬虫参考：[[python爬虫] Selenium爬取内容并存储至MySQL数据库](http://blog.csdn.net/eastmount/article/details/61914613)

![](https://img-blog.csdn.net/20170502155551504)
然后点击每一条新闻，去到详情页面并显示。比如：点击最后一条新闻，“2016年总结：教师路的开启，爱情味 的 初尝”。
![](https://img-blog.csdn.net/20170502155642816)

核心考点：
1.PHP如何连接数据库，并实现查询；
2.PHP通过<a href="info.php?id=xxx"></a>进行页面跳转；
3.PHP在写SQL语句时，如何通过.$name.连接变量，进行查询。

## 二. PHP数据库连接方法
连接数据库的方法我见解了学生ZT的方法，代码比较简洁明了。
1.建立两个PHP文件：config.php 和 connect.php；
2.然后在需要连接数据库的PHP文件，调用 require_once('connect.php');
3.如果是含登录的界面，使用<?php session_start();$_SESSION['name'];?>
config.php 代码：

```python
<?php
	header('content-type:text/html;charset=utf-8');
	define('HOST','localhost');
	define('USERNAME','root');
	define('PASSWORD','123456');
?>
```
connect.php 代码，注意调用 config.php，数据库名：hcpmanage（火车票管理）。
```python
<?php
	require_once('config.php');
	if(!($con=mysql_connect(HOST,USERNAME,PASSWORD)))
	{	
		die('Could not connect database: ' . mysql_error()); 
	}
	if(!mysql_select_db('hcpmanage'))
	{
		die ("Can\'t use hcpmanage : " . mysql_error()); 
	}
	mysql_query('set names utf8');
?>
```
main.php 使用的时候，核心代码如下所示，这也是获取Select结果核心内容。
```python
<?php
	//PHP连接数据库 
	require_once('connect.php');
	$sql="select ID, Artitle, FBTime from csdn_blog 
			order by FBTime desc limit 6;";
	$query = mysql_query($sql);
	if($query&&mysql_num_rows($query))
	{
		while($row = mysql_fetch_assoc($query))
		{
			$data[] = $row;
		}
	}
	
?>
```


## 三. 首页新闻查询
建立的数据库如下表所示，包括标题、URL、作者、时间、摘要、阅读数、评论数等信息，这些都是使用Python爬取的内容。哈哈~

![](https://img-blog.csdn.net/20170502161312475)

创建数据库代码如下：

```python
DROP TABLE IF EXISTS `csdn_blog`;
CREATE TABLE `csdn_blog` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `URL` varchar(100) COLLATE utf8_bin DEFAULT NULL,
  `Author` varchar(50) COLLATE utf8_bin DEFAULT NULL COMMENT '作者',
  `Artitle` varchar(100) COLLATE utf8_bin DEFAULT NULL COMMENT '标题',
  `Description` varchar(400) COLLATE utf8_bin DEFAULT NULL COMMENT '摘要',
  `FBTime` date DEFAULT NULL COMMENT '发布日期',
  `YDNum` int(11) DEFAULT NULL COMMENT '阅读数',
  `PLNum` int(11) DEFAULT NULL COMMENT '评论数',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3593 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
```
main.php 主界面，代码如下：
```python
<?php
	//PHP连接数据库 
	require_once('connect.php');
	$sql="select ID, Artitle, FBTime from csdn_blog 
			order by FBTime desc limit 6;";
	$query = mysql_query($sql);
	if($query&&mysql_num_rows($query))
	{
		while($row = mysql_fetch_assoc($query))
		{
			$data[] = $row;
		}
	}
	
?>
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
				<img src="./image/logo.jpg" alt="logo" 
					width="1024" height="149">		
			</td></tr>		
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
			<tr><td align="center">	<font size="3" color="blue">用户：xxx，欢迎您！</font></td></tr>
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
					当前位置: <a href="main.jsp">首页</a> > 车次信息管理
					<br /><br />
				<form action="main_new.jsp" method="post">
					输入关键词: 
					<input type="text" id="info" name="start" style='font-size:18px' width=400/>
					<input type="submit" name="Select" value="搜索" style='font-size:18px;'/>
				</form>
					<table width="90%" height="50%" 
					align="cneter" border="0" cellpadding="0" cellspacing="0" >
				<?php
					//获取值
					if(!empty($data))
					{
						foreach($data as $value)
						{
							$Nid = $value['ID'];  //获取新闻标号
							echo "<tr><td width='80%' style='line-height:2em;'>
									<a href='main_news.php?id=".$Nid."'>"
								 .$value['Artitle']."</a></td>";
							echo "<td align='right'>".$value['FBTime'].
								 "</td></tr>"; 
						}
					}
				?>
						<tr><td><br /></td></tr>
						<tr>
							<td width="80%"><a href="">更多</a></td>
						</tr>
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
				>网站声明</A> <BR>版权所有&copy;2016-2017 贵州财经大学 Eastmount   <BR>
		      	信息学院10006号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
SQL语句按照发布时间排序，然后输出前6行数据。
select ID, Artitle, FBTime from csdn_blog order by FBTime desc limit 6;

其中的重点是，在显示新闻内容过程中的方法，这里介绍两种方法，Select查询也显示两种方法供大家学习。
**方法一：采用<table></table>循环显示。**

```python
<?php
	require_once('connect.php');
	$start=$_POST['start'];
	$end=$_POST['end'];
	$sql="select * from ticket_tb where start like $start and end like $end";
	$query=mysql_query($sql);
	if($query&&mysql_num_rows($query))
	{
		while($row=mysql_fetch_assoc($query))
		{
			$data[]=$row;
		}
	}
?>
<div class="row" style="margin-top:50px;margin-left:30px;">
	<table border="1" cellpadding="0">
		<tr style="background:#06F;color:#FFF;height:30px;text-align:center;font-weight:bold;font-size:12px;font-family:雅黑">
            <td style="width:80px;">汽车编号</td>
            <td style="width:100px;">出发地</td>
            <td style="width:100px;">目的地</td>
            <td style="width:140px;">途径站</td>
            <td style="width:100px;">票价</td>
            <td style="width:100px;">票数</td>
            <td style="width:120px;">操作</td>
        </tr>
        <?php
			if(!empty($data))
			{
				foreach($data as $value)
				{
        ?>
        <tr>
	    	<td><?php echo $value['busid']?></td>
	        <td><?php echo $value['start']?></td>
	        <td><?php echo $value['end']?></td>
	        <td><?php echo $value['startTime']?></td>
	        <td><?php echo $value['busid']?></td>
	        <td><?php echo $value['busid']?></td>
	        <td><?php echo $value['busid']?></td>
	    </tr>
        <?php
				}
			}
		?>
   </table>
</div>
```
输出结果如下图所示，它相当于table显示表头，然后再html中结合php代码，while循环输出查询的信息。注意： <?php } ?>需要匹配循环的结束括号。
![](https://img-blog.csdn.net/20170502162309832)
**方法二：PHP中调用echo "<tr><td>"循环显示。**
```python
<table width="90%" height="50%" align="cneter" 
	border="0" cellpadding="0" cellspacing="0" >
<?php
	//PHP连接数据库 
	require_once('connect.php');
	$sql="select ID, Artitle, FBTime from csdn_blog 
			order by FBTime desc limit 6;";
	$query = mysql_query($sql);
	while($row = mysql_fetch_row($result))
	{
		list($ID, $Artitle, $FBTime) = $row; 
		
		$Nid = $id;  //获取新闻标号
		echo "<tr><td width='80%' style='line-height:2em;'>
					<a href='main_news.php?id=".$Nid."'>"
				 .$Artitle."</a></td>";
		echo "<td align='right'>".$FBTime.
				 "</td></tr>"; 
	}
?>					
</table>
```
重点内容：如何在PHP中通过“.”连接句子及变量，如 .$name. 。

![](https://img-blog.csdn.net/20170502155551504)

**方法三：使用printf规则输出。**
```python
<table border="1" cellpadding="0">
<tr style="background:#06F;color:#FFF;height:30px;text-align:center;font-weight:bold;font-size:12px;font-family:雅黑">
    <td style="width:80px;">汽车编号</td>
    <td style="width:100px;">出发地</td>
    <td style="width:100px;">目的地</td>
    <td style="width:140px;">出发日期</td>
    <td style="width:100px;">出发时间</td>
    <td style="width:100px;">票价</td>
    <td style="width:100px;">余票数</td>
    <td style="width:120px;">操作</td>
</tr>
<?php
   $sql="select a.id, a.busid,a.start,a.end,busDate,startTime,price,total-sellnum from ticket_tb a,saleticket_tb b where a.busid=b.busid and a.start like '%".$start."%' and a.end like '%".$end."%' and busDate ='".$busDate."' limit 11";
   $query=mysql_query($sql);
   while($data = mysql_fetch_row($query)) 
    {
        printf("<tr style='text-align: center;''>
                   <td>%s</td>
                   <td>%s</td>
                   <td>%s</td>
                   <td>%s</td>
                   <td>%s</td>
                   <td>%s</td>
                   <td>%s</td>
                   <td><a href='buy.php?id=%s>'>购票
                       </a></td>
                </tr>", 
                $data[1],$data[2],$data[3],$data[4],
                $data[5],$data[6],$data[7],$data[0]);    
    }
    // 结束 显示首页 地点查询
?>
</table>
```


## 四. 详情显示
详情页面，主要通过超链接进行跳转。
跳转前URL：http://localhost:8080/Eastmount/main.php
跳转后URL：http://localhost:8080/Eastmount/main_news.php?id=79
main.php 设置超链接：

![](https://img-blog.csdn.net/20170502164049638)
main_news.php?id=70 表示该跳转的ID为70，通过select找到这条新闻的详细信息，再通过 <?php echo $Description;?> 输出显示。

![](https://img-blog.csdn.net/20170502164400137)
```python
<?php	
	$sql="select Artitle, Author, URL, FBTime, 
			Description, PLNum, YDNum 
		  from csdn_blog where ID=".$_GET["id"]."";
	$query = mysql_query($sql);
	while($row = mysql_fetch_row($query))
	{
		list($Artitle, $Author, $URL, $FBTime, 
				$Description, $PLNum, $YDNum) = $row;  
	}
?>
```
main_news.php 完整代：
```python
<?php
	//PHP连接数据库 
	require_once('connect.php');
?>
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
				<img src="./image/logo.jpg" alt="logo" 
					width="1024" height="149">		
			</td></tr>		
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
			<tr><td align="center">	<font size="3" color="blue">用户：xxx，欢迎您！</font></td></tr>
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
					当前位置: <a href="main.php">首页</a> > 车次信息管理
					<br /><br />
					<?php	
						$sql="select Artitle, Author, URL, FBTime, 
								Description, PLNum, YDNum 
							  from csdn_blog where ID=".$_GET["id"]."";
						$query = mysql_query($sql);
						while($row = mysql_fetch_row($query))
						{
							list($Artitle, $Author, $URL, $FBTime, 
									$Description, $PLNum, $YDNum) = $row;  
					?>
					<div style="text-align:center;">
						<h3><?php echo $Artitle;?></h3>
                    </div>
                    <hr/>
                    <div style="padding-top:10px;">
                        <p>  时间：<?php echo $FBTime;?>
                            发布人：<?php echo $Author;?>
                            评论数：<?php echo $PLNum;?>
                            阅读数：<?php echo $YDNum;?> <br />
                             链接：<?php echo $URL;?> 
                        </p>
                        <p>
                               <?php echo $Description;?>
                        </p>
                    </div>  
					 <!--上面的php中while循环并没有结束,还有右括号-->
                     <?php
                         }
                     ?>
					
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
				>网站声明</A> <BR>版权所有&copy;2016-2017 贵州财经大学 Eastmount   <BR>
		      	信息学院10006号 </font>
		  </TD></TR>
		  </TBODY>
	 </TABLE>
	</div>
	</div>
  </body>
</html>
```
希望基础性文章对你有所帮助，如果文章中存在不足或错误的地方还请您海涵~
不管秀璋本人多么平庸，但我总觉得对你的爱真的很美。
(By:Eastmount 2017-05-02 下午5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



