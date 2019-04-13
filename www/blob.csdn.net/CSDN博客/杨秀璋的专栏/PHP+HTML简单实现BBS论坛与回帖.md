
# PHP+HTML简单实现BBS论坛与回帖 - 杨秀璋的专栏 - CSDN博客

2015年03月14日 16:12:07[Eastmount](https://me.csdn.net/Eastmount)阅读数：14799标签：[php																](https://so.csdn.net/so/search/s.do?q=php&t=blog)[Html																](https://so.csdn.net/so/search/s.do?q=Html&t=blog)[BBS论坛																](https://so.csdn.net/so/search/s.do?q=BBS论坛&t=blog)[php类																](https://so.csdn.net/so/search/s.do?q=php类&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=BBS论坛&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Html&t=blog)个人分类：[PHP基础知识																](https://blog.csdn.net/Eastmount/article/category/1644197)
[
																					](https://so.csdn.net/so/search/s.do?q=Html&t=blog)所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)[
							](https://so.csdn.net/so/search/s.do?q=Html&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=php&t=blog)


本文主要讲述如何通过PHP+HTML简单实现BBS论坛和发帖/回帖的功能，这是提取我们php项目的部分内容。主要内容包括：
1.[通过JavaScript和Iframe实现局部布局界面](http://blog.csdn.net/eastmount/article/details/43848585)
2.PHP如何定义类实现访问数据库功能
3.实现简单的BBS论坛和发帖/回帖功能
由于这个项目是十个人在寒假完成，所以采用了SAE搭建在线的后台数据库，其他人在通过Apache本地设计网页、访问数据库的方法实现，相当于简单的BS三端访问。
源码下载地址：[http://download.csdn.net/detail/eastmount/8501443](http://download.csdn.net/detail/eastmount/8501443)
访问[http://localhost:8080/testphp/index.php](http://localhost:8080/testphp/index.php)效果如下图所示：
![](https://img-blog.csdn.net/20150313153658937)
![](https://img-blog.csdn.net/20150313153805628)
![](https://img-blog.csdn.net/20150313153857093)

## 一. 首页布局搭建
首先是搭建首页的布局，主要布局分为head、main_left和main_right。其中右边显示部分采用的是iframe实现的局部显示，而左边采用JavaScript实现点击隐藏和显示功能。其中index.php首页代码如下：
```python
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>《分布式系统》精品课程学习</title>
<link  href="css/mycss.css" type="text/css" rel="stylesheet"/>
<style type="text/css">
	/* 这个链接改变颜色 */
	a.one:link {color: #ff0000}
	a.one:visited {color: #0000ff}
	a.one:hover {color: #ffcc00}
</style>
</head>
<body>
	
<!-- 布局顶部登录状态 -->
<div id="main">
<TABLE cellSpacing=0 cellPadding=0 width="100%" 
		background="images/header_bg.gif" border=0>
	<!--头部图片-->
  <TR height=80>
    <TD width=260>
    	<IMG height=80 src="images/logo.png" width=260>
    </TD>
    <TD style="FONT-SIZE: 12px; FONT-WEIGHT: bold; COLOR: #000;
    	PADDING-TOP: 20px; PADDING-RIGHT: 20px" align=right>
	  	 您还未登录! |
	    <A class="one" href="login.php" >登录</A>|
	    <A class="one" href="register_student.php" >学生注册</A>|
	    <A class="one" href="register_teacher.php" >教师注册</A> 
	  </TD>
  </TR>
</TABLE>
<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
  <TR bgColor=#1c5db6 height=4>
    <TD></TD></TR>
</TABLE>
<br />
<!-- 布局中部 -->
<div id="middle">
<!-- 布局中部右边 -->	
<div id="index_right">
<iframe height="100%" width="100%" border="0" frameborder="0" 
	src="main_right.php" name="rightFrame" id="rightFrame" title="rightFrame">
</iframe>
</div>
<!-- 布局中部左边 -->
<div id="index_left">
<?php
	include('main_left.php');
?>
</div>
</div>
```
这里的CSS代码布局调用css文件中的mycss.css实现：
```python
html, body {
    height: '100%';
    width: '100%';
}
/*头部布局*/
#main{
	margin-left:200px;
	width:950px;
}
/*中部布局*/
#middle{
	width:950px;
	height:500px;
	background-color:#b6effa;
}
/*中部左边*/
#index_left{
	float:left;
	margin-top:10px;
	margin-left:10px;
	width:180px;
	height:480px;
}
/*中部右边*/
#index_right{
	float:right;
	margin-top:10px;
	margin-right:20px;
	width:720px;
	height:480px;
	border-style:double;
}
/*中部右边*/
#logout_middle{
	float:right;
	margin-top:100px;
	margin-right:220px;
	width:500px;
	height:300px;
	border-style:double;
	background-color:#ffffff;
}
```
在Iframe中主要的代码是<iframe src="main_right.php" name="rightFrame" id="rightFrame">，调用时“<A href="main_right3-1.php" target="rightFrame">BBS论坛</A>”声明target为rightFrame即可。同时隐藏显示设置style.display为block或none。
其中main_left.php代码如下，此时即可实现上面的布局。
```python
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD>
<META http-equiv=Content-Type content="text/html; charset=utf-8">
<LINK href="css/admin.css" type="text/css" rel="stylesheet">
<SCRIPT language=javascript>
	function expand(el) {
		childObj = document.getElementById("child" + el);
		if (childObj.style.display == 'none') {
			childObj.style.display = 'block';
		} else {
			childObj.style.display = 'none';
		}
		return;
	}
</SCRIPT>
</HEAD>
<BODY>
<TABLE height="100%" cellSpacing=0 cellPadding=0 width=170 
background=images/menu_bg.jpg border=0 align=left>
  <TR>
    <TD vAlign=top align=middle>
      <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
        <TR>
          <TD height=10></TD></TR>
      </TABLE>
			<!-- 第一选项 -->
			<TABLE cellSpacing=0 cellPadding=0 width=150 border=0>  
			  <TR height=30>
			    <TD style="PADDING-LEFT: 20px; FONT-SIZE: 15px" background=images/menu_bt.jpg>
			    	<A class=menuParent onclick=expand(1) 
			      	href="javascript:void(0);">课程首页</A></TD></TR>
			  <TR height=4>
			    <TD></TD></TR>
			</TABLE>
			<TABLE id=child1 style="DISPLAY: none" cellSpacing=0 cellPadding=0 
			width=150 border=0>
			  <TR height=20>
			    <TD align=middle width=30><IMG height=9 
			      src="images/menu_icon.gif" width=9></TD>
			    <TD><A href="main_right1-1.php" 
			    	target="rightFrame">首页介绍</A></TD></TR>
			  <TR height=4>
			    <TD colSpan=2></TD></TR>
			</TABLE>
			<!-- 第二选项 -->
      <TABLE cellSpacing=0 cellPadding=0 width=150 border=0>
        <TR height=30>
          <TD style="PADDING-LEFT: 20px; FONT-SIZE: 15px" background=images/menu_bt.jpg>
          	<A class=menuParent onclick=expand(2) 
            	href="javascript:void(0);">课程概况</A></TD></TR>
        <TR height=4>
          <TD></TD></TR>
      </TABLE>
			<TABLE id=child2 style="DISPLAY: none" cellSpacing=0 cellPadding=0 
			width=150 border=0>
			  <TR height=20>
			    <TD align=middle width=30><IMG height=9 
			      src="images/menu_icon.gif" width=9></TD>
			    <TD><A href="guest/main_right2-1.php" 
			    	target="rightFrame">课程简介</A></TD></TR>
			  <TR height=20>
			    <TD align=middle width=30><IMG height=9 
			      src="images/menu_icon.gif" width=9></TD>
			    <TD><A href="guest/main_right2-2.php" 
			    	target="rightFrame">教师团队</A></TD></TR>
			  <TR height=20>
			    <TD align=middle width=30><IMG height=9 
			      src="images/menu_icon.gif" width=9></TD>
			    <TD><A href="guest/main_right2-3.php" 
			    	target="rightFrame">教学条件</A></TD></TR>
			  <TR height=4>
			    <TD colSpan=2></TD></TR>
			</TABLE>
      <!-- 第三选项 -->
      <TABLE cellSpacing=0 cellPadding=0 width=150 border=0>
        <TR height=30>
          <TD style="PADDING-LEFT: 20px; FONT-SIZE: 15px" background=images/menu_bt.jpg>
          	<A class=menuParent onclick=expand(3) 
            	href="javascript:void(0);">互动交流</A></TD></TR>
        <TR height=4>
          <TD></TD></TR>
      </TABLE>
      <TABLE id=child3 style="DISPLAY: block" cellSpacing=0 cellPadding=0 
      width=150 border=0>
        <TR height=20>
          <TD align=middle width=30><IMG height=9 
            src="images/menu_icon.gif" width=9></TD>
          <TD><A href="main_right3-1.php" 
			    	target="rightFrame">BBS论坛</A></TD></TR>
        <TR height=20>
          <TD align=middle width=30><IMG height=9 
            src="images/menu_icon.gif" width=9></TD>
          <TD><A href="main_right3-2.php" 
			    	target="rightFrame">通知公告</A></TD></TR>
        <TR height=4>
          <TD colSpan=2></TD></TR>
      </TABLE>
</BODY>
</HTML>
```
显示的效果如下：
![](https://img-blog.csdn.net/20150314151304114)

## 二. 实现BBS论坛及回复
论坛主要通过自定义类database文件夹下Httppost.php实现，其中main_right3-1.php是显示所有BBS论坛帖子的主题，而main_right3-1-content.php是对应帖子的内容，包括回复内容等。
其中main_right3-1.php的代码如下：
```python
<?php
	header("Content-Type: text/html; charset=utf-8");
	//注意：include一个php文件不能重复，要递归检查是否重复引用
	include ("../database/httppost.php");
?>
<!-- 需要注意在Html中“..”表示回到上一级目录 而css与main_right3-1.php同级目录 -->
<link rel="stylesheet" href="css/main.css" type="text/css"  />
<TABLE cellSpacing=0 cellPadding=0 width="100%" align=center border=0>
  <TR height=28>
    <TD >当前位置: BBS论坛</TD>
    <TD align=right><A href="main_right.php" class="one"> 返回 </A></TD>    
  </TR>
  <TR>
    <TD bgColor=#b1ceef height=1 colspan="2"></TD></TR>
  <TR height=10>
    <TD background=images/shadow_bg.jpg colspan="2"></TD></TR>
</TABLE>
<div class="middle">
<div class="wrap">
	<div class="wrap_left">
    	<div class="wenda-head">
            <a href="#" class="quealltab  onactive" >最新</a>
            <a href="#" class="quealltab " >热门</a>
            <a href="#" class="quealltab " >等待回复</a>
    	</div>
    	<?php
    		//查询贴
				$hgi=new HttpPostInf();
				$result=$hgi->doquery('2',"select * from BBS_Post;");
				//解析json
				$dj=json_decode($result,true);
				$jcount=count($dj);
				for($i=0; $i<$jcount; $i++){	
    	?>
     <div class="wenda-list">
        	<div class="headslider">
            	<img src='images/bbs3.jpg' class="wenda_list_head" width='40' height='40' />
      				<a href="" title="name" class="wenda-nickname"><?php echo $dj[$i]['BP_Pid']; ?></a>
            </div>
            <div class="qaslider">
                <ul>
                    <li><img src="images/bbs1.jpg" class="icon"/><a 
                    		href="main_right3-1-content.php?bbsid=<?php echo $dj[$i]['BP_Pid']; ?>" 
                    		class="wendatitle"><?php echo $dj[$i]['BP_Ptitle']; ?></a></li>
                    <li><img src="images/bbs2.png" class="icon2"/><span class="replydet">发帖身份：
                    	<?php 
                    		if($dj[$i]['BP_Ptype']==0) echo "教师";
                    		if($dj[$i]['BP_Ptype']==1) echo "学生";
                    	?>
                      |  发帖人编号：
                    <?php echo $dj[$i]['BP_Puserid']; ?></span></li>
                    <li><span class="time">提问时间：<?php echo $dj[$i]['BP_Ptime']; ?></span>
                </ul>
            </div><!--qaslider-->
    </div><!--wenda-list-->
    <?php
    	//结束循环
    	}	
    ?>
</div><!--wrap-->
</div><!--wrap_right-->    
</div><!--middle-->
<TABLE cellSpacing=0 cellPadding=2 width="95%" align=center border=0 >
	<TR height=20>
    <TD></TD>
  </TR>
</TABLE>
```
这里需要指出HTML代码"../database/httppost.php"中".."表示返回上一级目录，该代码的目录如下图所示，源代码放在Apache中htdocs文件夹中。
![](https://img-blog.csdn.net/20150314155405836)
其中main_right3-1-content.php代码如下：
```python
<?php
	header("Content-Type: text/html; charset=utf-8");
	include ("../database/human.php");
	session_start(); 
?>
<link rel="stylesheet" href="css/main.css" type="text/css"  />
<TABLE cellSpacing=0 cellPadding=0 width="100%" align=center border=0>
  <TR height=28>
    <TD background=images/title_bg1.jpg>当前位置: BBS论坛</TD>
  	<TD align=right><A href="main_right6-1.php" class="one"> 返回 </A></TD>  
  </TR>
  <TR>
    <TD bgColor=#b1ceef height=1 colspan="2"></TD></TR>
  <TR height=10>
    <TD background=images/shadow_bg.jpg colspan="2"></TD></TR>
</TABLE>
<!-- PHP数据库中获取内容 -->
<?php
	//查询BBS
	$hgi=new HttpPostInf();
	$result=$hgi->doquery('2',"select * from BBS_Post where BP_Pid='".$_GET['bbsid']."';"); 
	//解析json
	$dj=json_decode($result,true);
	$jcount=count($dj);
	for($i=0; $i<$jcount; $i++){	
		$_SESSION['bbsid']=$dj[$i]['BP_Pid'];
?>
<div class="middle">
<div class="wrap">
	<div class="wrap_left">
			<div class="wenda-head">
        <TABLE cellSpacing=0 cellPadding=0 width="100%" align=center border=0 bgcolor="blue">
				  <TR height=10>
				    <TD></TD></TR>
				  <TR height=22>
				    <TD style="PADDING-LEFT: 10px; FONT-WEIGHT: bold; COLOR: #ffffff" 
				    	align=left>[贴] <?php echo $dj[$i]['BP_Ptitle']; ?></TD></TR>
				  <TR height=10>
				    <TD></TD></TR>
				</TABLE>    
    	</div>
    <!-- 楼主内容 -->
     <TABLE cellSpacing=0 cellPadding=0 width="95%" align=center border=0>
			  <tr>
			    <td valign="top"><div align="middle" width=150><BR /><BR /><img 
			    	src='images/tx.jpg' width='60' height='60' />
			    	<BR /><BR />楼主 1# <BR />
			    	<?php echo $dj[$i]['BP_Puserid']; ?>
			   			<BR />身份
			    		<?php 
                if($dj[$i]['BP_Ptype']==0) echo "教师";
                if($dj[$i]['BP_Ptype']==1) echo "学生";
               ?><BR /><?php echo $dj[$i]['BP_Ptime']; ?></div></td>
			    <!-- 多行文本输入控件 disabled="disabled"不可编辑 -->
			    <td ><textarea rows="15" type="text" width="400px" style="resize:none;font-size:16px;" 
			    	maxlength="2000" name="content" cols="60" disabled="disabled"><?php echo $dj[$i]['BP_Cont']; ?></textarea></td>
			  </tr>
			  <TR height=4>
			    <TD colspan="3"></TD>
			  </TR>
			  <tr>
			    <td colspan="2"><hr width="100%" size="2" color="#FF0000"></td>
			  </tr>
			</TABLE>
			<!--上面的php中while循环并没有结束,还有右括号-->
			<?php
				}
				//查询BBS
				$hgi=new HttpPostInf();
				$result=$hgi->doquery('2',"select * from BBS_Reply where BR_Pid='".$_GET['bbsid']."';"); 
				//解析json
				$dj=json_decode($result,true);
				$jcount=count($dj);
				for($i=0; $i<$jcount; $i++){	
			?>
			<!-- 楼层回复内容 -->
			<TABLE cellSpacing=0 cellPadding=0 width="95%" align=center border=0>
			  <tr>
			    <td valign="top"><div align="middle" width=150><BR /><BR />
			    	<img src='images/tx.jpg' width='60' height='60' />
			    	<BR /><BR />楼层 <?php $_SESSION['bbshfid']=time(); echo $i+2;?># <BR />
			    	<?php echo $dj[$i]['BR_Ruserid']; ?>
			   			<BR />身份
			    		<?php 
                if($dj[$i]['BR_Rtype']==0) echo "教师";
                if($dj[$i]['BR_Rtype']==1) echo "学生";
               ?>
        			 <BR /><?php echo $dj[$i]['BR_Ptime']; ?>
               <BR />回复楼层<?php echo $dj[$i]['BR_Pfloor']; ?>#</div></td>
			    <!-- 多行文本输入控件 disabled="disabled"不可编辑 -->
			    <td ><textarea rows="15" type="text" width="400px" style="resize:none;font-size:16px;" 
			    	maxlength="2000" name="content" cols="60" disabled="disabled"><?php echo $dj[$i]['BR_Cont']; ?></textarea></td>
			  </tr>
			  <TR height=4>
			    <TD colspan="3">
			    </TD>
			  </TR>
			  <tr>
			    <td colspan="2"><hr width="100%" size="2" color="#FF0000"></td>
			  </tr>
			</TABLE>
			<?php
				}
			?>
			<!-- 回复内容 -->
			<form id="form1" name="form1" method="post" action="main_right3-1-content.php">
			<TABLE cellSpacing=0 cellPadding=0 width="95%" align=center border=0>
			  <tr>
			  	<td colspan="2">  回复楼层
			  		<input type="text" name="lc" id="lc" value="" style=width:150pt; maxlength="50"/>
			  	</td>
			  </tr>
			  <tr>
			    <td valign="top"><div align="middle" width=150>
			    	<BR /><BR />   回复   <BR /></div></td>
			    	<!-- 多行文本输入控件 disabled="disabled"不可编辑 -->
			    <td ><textarea rows="10" type="text" width="400px" style="resize:none;font-size:16px;" maxlength="2000" 
			    	name="bbscontent" cols="60" ></textarea></td>
			  </tr>
			  <tr>
			  	<td colspan="2" align="middle">
			  		<input type="submit" style='font-size:22px' name="Submit" value="提交回复"/>
			  	</td>
			  </tr>
		  	<TR height=4><TD colspan="3"></TD></TR>
			  <tr>
			    <td colspan="2"><hr width="100%" size="2" color="#FF0000"></td>
			  </tr>
			</TABLE>
     	</form>
</div><!--wrap-->
</div><!--wrap_right-->    
</div><!--middle-->
<TABLE cellSpacing=0 cellPadding=2 width="95%" align=center border=0>
	<TR height=20>
    <TD></TD></TR>
</TABLE>
<!-- 提交当前页面POST数据库表单处理 -->
<?php
	if (!empty($_POST['bbscontent'])) 
	{
		if($_SESSION['radio']==1) $a=1; //学生
		if($_SESSION['radio']==2) $a=0; //老师
		//SQL更新
		$sql = "INSERT INTO BBS_Reply (BR_Rid,BR_Pid,BR_Ruserid,BR_Rtype,BR_Ptime,BR_Pfloor,BR_Cont) VALUES (";
		$sql .= "'".$_SESSION['bbshfid']."',";
		$sql .= "'".$_SESSION['bbsid']."',";
		$sql .= "'".$_SESSION['number']."',";
		$sql .= "'".$a."',";
		$sql .= "'".date('Y-m-d')."',";
		$sql .= "'".$_POST['lc']."',";
		$sql .= "'".$_POST['bbscontent']."'";
		$sql .= ");";	
		//更新信息
		//echo $sql;
		$hgi=new HttpPostInf();
		$result=$hgi->doquery('1',$sql);
		//echo "<script>alert('恭喜你修改成功!');</script>";
		header('Location:main_right3-1.php');
	}
?>
```
发帖回复显示效果如下图所示：
![](https://img-blog.csdn.net/20150314155809419)
![](https://img-blog.csdn.net/20150314155855300)
发帖的代码就不贴出来了，可以查看源代码。写到此处可能你会疑惑数据库访问部分的内容，下面进行简单讲解吧！



## 三. PHP定义类及成员函数
在PHP中我们通常课程学习时完成的网站都是基于面向过程的方法，从而忽略了它的面向对象知识，我显然也犯了这样的错误。
PS:由于该项目是《面向对象分析与开发》课程作业，所以需要用面向对象的知识去分析和实现。突然有一天我发现PHP以前用的都是HTML和PHP布局后台实现的，没用用到类、封装、继承的知识，但查阅相关资料后发现它都是有这些东西的。
后来在实现这个项目过程中我们定义了不同的类(感谢PU哥)，这里仅仅举例数据库方面和Human类。
```python
<?php
header("Content-Type: text/html; charset=utf-8");
class HttpPostInf{
    function __construct(){ //无参构造函数
    }
    function doquery($type , $sql){ //网路通信函数
			$data = array ('sqlType' => $type , 'sqlExe' => $sql); 
			$data = http_build_query($data);  
			$opts = array ('http' => array ('method' => 'POST','header'=> 
				"Content-type: application/x-www-form-urlencoded\r\n" ."Content-Length: "
				. strlen($data) . "\r\n",'content' => $data)); 
			$context = stream_context_create($opts);
			$html = file_get_contents('http://courseweb.sinaapp.com/courseweb.php', false, $context);
			return $html;
	}	
}
?>
```
同时定义Human类，而继承该类的Student和Teacher是两种身份的登陆用户。其中构造函数、实例化都已经在成员函数中写好。
```python
<?php
header("Content-Type: text/html; charset=utf-8");
include_once("httppost.php");
//人类，拥有教师和学生的共同属性和方法
class Human{
	private $hid; //唯一编号
	private $pwd; //密码
	private $name; //名字
	private $sex; //性别
	private $email; //邮箱
  function __construct(){ //无参构造函数
  }
	//静态验证函数	
	static function id_vf($id,$spwd,$type){ //类静态方法 type=1为学生，2为老师
		$hpi=new HttpPostInf();	
		if($type==1)
		{
      $result=$hpi->doquery('2',"select * from Student_Info where Sinf_id='".$id."';");
			if($result =='error' || $result=='null'){ //返回无值
        return -1; //没有用户，返回0
			}
      else{
      	$dj=json_decode($result,true);
        $jcount=count($dj);
        for($i=0; $i<$jcount; $i++){
            //只有一个密码
            $right_pwd=$dj[$i]['SInf_Pwd'];  //学生登录密码
        }	   
        if($spwd == $right_pwd) return 1;    //验证成功，可以登陆
        else return 0;//密码错误
      }
		}
		else if($type==2)
		{
      $result=$hpi->doquery('2',"select * from Teacher_Info where TI_id='".$id."';");
			if($result =='error' || $result=='null'){ //返回无值
       	return -1; //没有用户，返回0
			}
      else{
      	$dj=json_decode($result,true);
        $jcount=count($dj);
        for($i=0; $i<$jcount; $i++){
            //只有一个密码
            $right_pwd=$dj[$i]['TI_Pwd']; //学生登录密码
        }	   
        if($spwd==$right_pwd) return 1; //验证成功，可以登陆
        else return 0;//密码错误
      }
		}
	}
}
class Student extends Human{
	private $bir; //出生日期
	private $maj; //专业
	private $eym; //入学年份
	private $score; //得分
	function __construct($sid){ //1参构造函数
		$this->hid=$sid;
		$stu=new HttpPostInf();	
		$result=$stu->doquery('2',"select * from Student_Info where Sinf_ID='".$sid."';");
        //echo 'result:'.$result;
		//解析json
		$dj=json_decode($result,true);
		$jcount=count($dj);
		for($i=0; $i<$jcount; $i++){
			//只返回1个，所以只取一个即可，jcount=1
			$this->bir=$dj[$i]['SInf_Bir'];
			$this->maj=$dj[$i]['SInf_Maj'];
			$this->eym=$dj[$i]['SInf_Eym'];
			$this->score=$dj[$i]['SInf_Score'];
            //或者写：$score=$dj->Sinf_Bir;
			$this->pwd=$dj[$i]['SInf_Pwd'];
			$this->name=$dj[$i]['SInf_Name'];
			$this->sex=$dj[$i]['SInf_Sex'];
			$this->email=$dj[$i]['SInf_Email'];	
		}   
	}
  function getname(){return $this->name;}
	function getsex(){return $this->sex;}
	function getemail(){return $this->email;}
	function getpwd(){return $this->pwd;}
	function getbir(){return $this->bir;}
	function getmaj(){return $this->maj;}
	function geteym(){return $this->eym;}
	function getscore(){return $this->score;}
}
class Teacher extends Human{
	private $ptitle; //职称
	private $res;    //成果
	private $award;  //获奖
	
	function __construct($tid){ //1参构造函数
		$this->hid=$tid;
		$stu=new HttpPostInf();	
		$result=$stu->doquery('2',"select * from Teacher_Info where TI_ID='".$tid."';");
		//echo 'result:'.$result;
		//解析json
		$dj=json_decode($result,true);
		$jcount=count($dj);
		for($i=0; $i<$jcount; $i++){
			//只返回1个，所以只取一个即可，jcount=1
			$this->ptitle=$dj[$i]['TI_Ptitle'];
			$this->res=$dj[$i]['TI_Res'];
			$this->award=$dj[$i]['TI_Award'];
      //或者写：$score=$dj->Sinf_Bir;
			$this->pwd=$dj[$i]['TI_Pwd'];
			$this->name=$dj[$i]['TI_Name'];
			$this->sex=$dj[$i]['TI_Sex'];
			$this->email=$dj[$i]['TI_Email'];				
		}	
	}
   function getname(){return $this->name;}
		function getsex(){return $this->sex;}
		function getemail(){return $this->email;}
		function getpwd(){return $this->pwd;}
		function getptitle(){return $this->ptitle;}
		function getres(){return $this->res;}
		function getaward(){return $this->award;}
}
?>
```
再调用上面定义的类HttpPostInf和Human，实现访问数据库的简单代码如下所示：
<?php
//查询贴
$hgi=new HttpPostInf();
$result=$hgi->doquery('2',"select * from BBS_Post;");//SQL语句
//解析json
$dj=json_decode($result,true);
$jcount=count($dj);
for($i=0; $i<$jcount; $i++) {
echo $dj[$i]['BP_Pid'];
}
?>
而当用户登录后会使用Session记录用户的id，然后通过类实例化即可实现获取该用户的所有信息，简单的代码如下：
$h1=new Student('2220140598');
echo $h1->getname(); echo '<br>';
echo $h1->getsex(); echo '<br>';
echo $h1->getemail(); echo '<br>';
最后希望文章对大家有所帮助，尤其发现PHP方面搭建的文章比较少！相对多的文章是PHP关于某个细节处理，同时更希望大家学习下PHP类相关的知识。总感觉自己写博客有些问题，但自己说不出来，希望大家提出不足之处！该代码数据库是布局在SAE上的，所以直接运行可以出现上面所有图形的效果。
（By：Eastmount 2015-3-14 下午4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


