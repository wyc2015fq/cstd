
# PHP+JavaScript+HTML实现注册界面表单及日历控件 - 杨秀璋的专栏 - CSDN博客

2015年02月23日 17:10:34[Eastmount](https://me.csdn.net/Eastmount)阅读数：3236所属专栏：[PHP基础知识及网站开发](https://blog.csdn.net/column/details/eastmount-php.html)



本文主要是介绍我做PHP网站时的一个HTML的简单静态界面，它的主要功能是用户注册界面，并且参照了网上的例子使用JavaScript判断和My97DatePicker的日历控件。界面效果如下图所示：
![](https://img-blog.csdn.net/20150223165156659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
同时插入数据库显示效果如下图所示：
![](https://img-blog.csdn.net/20150223165051713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到引用My97DatePicker的日历控件及判断效果如下图所示：
![](https://img-blog.csdn.net/20150223165456290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150223165545165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中注册界面register_student.html代码如下：
```python
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<script language="javascript" type="text/javascript">
function do_selected()
{
	var yourchoise="";
	for(var i=0;i<form1.subject.length;i++)
	{
		if(form1.subject.options[i].selected)
		{
			yourchoise +=form1.subject[i].value+";";
		}
	}
	return yourchoise;
}
function win_open()
{
   	msg=window.open("","DisplayWindow","toolbar=no,directories=no,resizable=yes,menubar=no");
   	msg.document.write("学号:");
		msg.document.write(form1.number.value+"<br>");
		msg.document.write("姓名:");
		msg.document.write(form1.name.value+"<br>");
		msg.document.write("性别:");
		msg.document.write(form1.sex.value+"<br>");
		msg.document.write("Email邮件:");
		msg.document.write(form1.email.value+"<br>");
		msg.document.write("学院专业:");
		msg.document.write(form1.zy.value+"<br>");
		msg.document.write("个人简介:");
		msg.document.write(form1.instruction.value+"<br>");
		msg.document.write("<br>");
}
function mycheck()
{
	//判断
	if(document.form1.number.value=="")
	{
		alert("请输入学号");
		document.form1.number.focus();
		return false;
	}
	var number = /^[0-9]{6,14}$/;
	if (!number.exec(document.form1.number.value)) {
        window.alert("学号必须由数字组成且长度为6-14!");
        document.form1.number.focus();
        return false;
  }
	if(document.form1.name.value=="")
	{
		alert("请输入姓名");
		document.form1.name.focus();
		return false;
	}
	if(document.form1.sex.value=="未指定")
	{
		alert("请指明性别");
		document.form1.sex.focus();
		return false;
	}
	if(document.form1.email.value.indexOf("@")==-1||document.form1.email.value.indexOf(".")==-1)
	{
		alert("邮箱格式不正确，请重新输入!");
		document.form1.email.focus();
		return false;
	}
	if(document.form1.pwd.value=="")
	{
		alert("请输入密码");
		document.form1.pwd.focus();
		return false;
	}
	var patrn=/^[a-zA-Z0-9]{5,20}$/;
	if (!patrn.exec(document.form1.pwd.value)) 
	{
        window.alert("密码只能由字母或者数字组成且长度为5-20之间!");
        return false;
  }
	if(document.form1.pwd2.value=="")
	{
		alert("请输入确认密码");
		document.form1.pwd2.focus();
		return false;
	}
	if(document.form1.pwd.value!=document.form1.pwd2.value)
	{
		alert("输入的密码与确认密码不一致!");
		document.form1.pwd2.focus();
	   	return false;
	}
	if(do_selected()=="")
	{
		alert("请至少选择一门课");
		document.form1.subject.focus();
		return false;
	}
	//win_open();
}
</script>
<style type="text/css">
	/* 这个链接改变颜色 */
	a.one:link {color: #ff0000}
	a.one:visited {color: #0000ff}
	a.one:hover {color: #ffcc00}
</style>
<!-- 引用日历控件 百度知道-html日期选择控件 -->
<script language="javascript" type="text/javascript" 
	src="My97DatePicker/WdatePicker.js">
</script>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>用户注册</title>
<style type="text/css">
<!--
.STYLE1 {color: #FF0000}
-->
</style>
</head>
<body>
<!-- autocomplete="off"禁止保存缓存 -->
<form id="form1" name="form1" method="post" autocomplete="off" action="register_sresult.php" onsubmit="return mycheck()">
  <table width="570" border="0" align="center">
  	<tr>
      <td colspan="2" align=center><H2>学生用户注册</H2></td>
    </tr>
    <tr>
      <td colspan="2" align=right><A href="index.php" class="one">返回</A></td>
    </tr>
    <tr>
      <td colspan="2"><hr width="95%" size="1" color="#FF0000"></td>
    </tr>
    <tr>
      <td  width="217"><div align="right">学号:<span class="STYLE1">*</span></div></td>
      <td width="343"><input type="text" name="number" id="number" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td><div align="right">姓名:<span class="STYLE1">*</span></div></td>
      <td><input type="text" name="name" id="name" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td><div align="right">性别:<span class="STYLE1">*</span></div></td>
      <td><select name="sex" id="sex" value="" style=width:165pt; maxlength="50">
        <option value="未指定">未指定</option>
        <option value="男">男</option>
        <option value="女">女</option>
      	</select> 
      </td>
    </tr>
    <tr>
      <td><div align="right">Email邮件:<span class="STYLE1">*</span></div></td>
      <td><input type="text" name="email" id="email" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td><div align="right">密码:<span class="STYLE1">*</span></div></td>
      <td><input type="password" name="pwd" id="pwd" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td><div align="right">确认密码:<span class="STYLE1">*</span></div></td>
      <td><input type="password" name="pwd2" id="pwd2" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td colspan="2"><hr width="95%" size="1" color="#FF0000"></td>
    </tr>
    <tr>
      <td><div align="right">学院:</div></td>
      <td><select name="zy" id="zy" value="" style=width:165pt; maxlength="50">
        <option value="软件学院">软件学院</option>
        <option value="计算机学院">计算机学院</option>
        <option value="自动化学院">自动化学院</option>
        <option value="光电学院">光电学院</option>
        <option value="车辆学院">车辆学院</option>
        <option value="信息与电子学院">信息与电子学院</option>
        <option value="机电学院">机电学院</option>
        <option value="基础教育学院">基础教育学院</option>
        <option value="其他">其他</option>
      	</select>      
      </td>
    </tr>
    <tr>
      <td><div align="right">出生日期:<span class="STYLE1">*</span></div></td>
      <td><input type=text name="birth" id="birth" value="" 
      	style=width:165pt; maxlength="50" onClick="WdatePicker()"></td>
    </tr>
    <tr>
      <td><p align="right">专业:</p>
      <p align="right">（单选）</p></td>
      <td><select name="subject" size="4" id="subject" value="" style=width:165pt; maxlength="50">
        <option value="软件工程">软件工程</option>
        <option value="数字媒体">数字媒体</option>
        <option value="信息安全">信息安全</option>
        <option value="数字仿真">数字仿真</option>
        <option value="移动应用开发">移动应用开发</option>
      </select></td>
    </tr>
		<tr>
      <td><div align="right">入学年月:</div></td>
      <td><select name="year" id="year" value="" style=width:165pt; maxlength="50">
        <option value="2010年">2010年</option>
        <option value="2011年">2011年</option>
        <option value="2012年">2012年</option>
        <option value="2013年">2013年</option>
        <option value="2014年">2014年</option>
        <option value="2015年">2015年</option>
        <option value="2016年">2016年</option>
        <option value="2017年">2017年</option>
        <option value="2018年">2018年</option>
      	</select>      
      </td>
    </tr>
    <tr>
      <td><div align="right"></div></td>
      <td><select name="month" id="month" value="" style=width:165pt; maxlength="50">
        <option value="1月">01月</option>
        <option value="2月">02月</option>
        <option value="3月">03月</option>
        <option value="4月">04月</option>
        <option value="5月">05月</option>
        <option value="6月">06月</option>
        <option value="7月">07月</option>
        <option value="8月">08月</option>
        <option value="9月">09月</option>
        <option value="10月">10月</option>
        <option value="11月">11月</option>
        <option value="12月">12月</option>
      	</select>      
      </td>
    </tr>
    <tr>
      <td colspan="2"><hr width="95%" size="1" color="#FF0000"></td>
    </tr>
    <tr>
      <td><div align="right">
        <input type="submit" style='font-size:15px' name="Submit" value="提交"/>
      </div></td>
      <td><div align="center">
      	<input type="reset" name="button2" style='font-size:15px' id="button2" value="重置" />
      	</div></td>
    </tr>
  </table>
</form>
</body>
</html>
```
JavaScript包括学号、姓名、密码长度、性别等判断，同时register_sresult.php显示结果代码如下，数据库方面是封装在类中，调用下面代码实现的：
//判断插入的用户是否重复
$hpi = new HttpPostInf();
$result = Human::id_vf($_SESSION['name'],$_SESSION['pwd'],1);
//执行插入操作
$hgi=new HttpPostInf();
$result=$hgi->doquery('1',$sql);
同时采用Session记录数据，但Session更好的方面是在登录操作。
```python
<?php
	header("Content-Type: text/html; charset=utf-8");
	include ("database/human.php");
	session_start();   
	//输入POST非空时赋值
	if (!empty($_POST)) 
	{
		$_SESSION['number'] = $_POST['number'];
		$_SESSION['name'] = $_POST['name'];
		$_SESSION['email'] = $_POST['email'];
		$_SESSION['sex'] = $_POST['sex'];
		$_SESSION['pwd'] = $_POST['pwd'];
		$_SESSION['zy'] = $_POST['zy'];
		$_SESSION['subject'] = $_POST['subject'];
		$_SESSION['year'] = $_POST['year'];
		$_SESSION['month'] = $_POST['month'];
		$_SESSION['birth'] = $_POST['birth'];
	}
	if($_SESSION['number']=='' || $_SESSION['name']=='')
	{
		header('Location:index.php');
	}
	//联系数据库
	$hpi = new HttpPostInf();
	//调用静态函数方法 “::” 1-学生
	$result = Human::id_vf($_SESSION['name'],$_SESSION['pwd'],1);
	if($result!=-1) { //-1表示没有用户
		echo "<script>alert('抱歉,您提交的用户名id已存在!');window.history.go(-1)</script>"; 
		session_destroy();
		exit(); 
	}
	if($result==-1) { //插入数据库
		$sql = "INSERT INTO Student_Info (SInf_ID,SInf_Name,SInf_Sex,SInf_Maj,SInf_Eym,SInf_Pwd,SInf_Email,SInf_Score,Sinf_Bir) VALUES (";
		$sql .= "'".$_SESSION['number']."',";
		$sql .= "'".$_SESSION['name']."',";
		$sql .= "'".$_SESSION['sex']."',";
		$sql .= "'".$_SESSION['subject']."',";
		$sql .= "'".$_SESSION['year'].$_SESSION['month']."',";
		$sql .= "'".$_SESSION['pwd']."',";
		$sql .= "'".$_SESSION['email']."',";
		$sql .= "'0',";  //分数非空
		$sql .= "'".$_SESSION['birth']."'";
		$sql .= ");";
		
		//echo $sql;
		$hgi=new HttpPostInf();
		$result=$hgi->doquery('1',$sql);
		//echo $result;
		if($result=='error'){ //添加失败
    	echo "<script>alert('抱歉,您注册用户失败!');window.history.go(-1)</script>"; 
    	session_destroy();
			exit();
		}
	}
?>
<html>
<head>
<style type="text/css">
	/* 这个链接改变颜色 */
	a.one:link {color: #ff0000}
	a.one:visited {color: #0000ff}
	a.one:hover {color: #ffcc00}
</style>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>用户注册</title>
</head>
<body>
<table width="100%" height="80%" border="0" cellspacing="0" cellpadding="0">
	<tr><td align="center" valign="middle">
		<table>
			<tr>
				<td align=center><IMG height=60 src="images/Tasks.png" width=60></td>
      	<td align=center><H2>恭喜您注册成功</H2></td>
	    </tr>
	    <tr>
	      <td colspan="2" align=right><A href="login.php" class="one">立即登陆</A></td>
	    </tr>
	    <tr>
	      <td colspan="2"><hr width="95%" size="1" color="#FF0000"></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">学号:</span></div></td>
	      <td width="343"><?php echo $_SESSION['number']; ?></td>
	    </tr>
			<tr>
	      <td  width="217"><div align="right"><span class="STYLE1">姓名:</span></div></td>
	      <td width="343"><?php echo $_SESSION['name']; ?></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">性别:</span></div></td>
	      <td width="343"><?php echo $_SESSION['sex']; ?></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">专业:</span></div></td>
	      <td width="343"><?php echo $_SESSION['subject']; ?></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">入学时间:</span></div></td>
	      <td width="343"><?php echo $_SESSION['year'].$_SESSION['month']; ?></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">邮件:</span></div></td>
	      <td width="343"><?php echo $_SESSION['email']; ?></td>
	    </tr>
	    <tr>
	      <td  width="217"><div align="right"><span class="STYLE1">出生日期:</span></div></td>
	      <td width="343"><?php echo $_SESSION['birth']; ?></td>
	    </tr>
	    <tr>
	      <td colspan="2"><hr width="95%" size="1" color="#FF0000"></td>
	    </tr>
		</table>
	</td></tr>
</table>
</body>
</html>
```
至于My97DatePicker控件主要是通过文件夹调用，核心代码如下：
<!-- 引用日历控件 百度知道-html日期选择控件 -->
<script language="javascript" type="text/javascript"
src="My97DatePicker/WdatePicker.js">
</script>
<input type=text name="birth" id="birth" value=""
style=width:165pt; maxlength="50" onClick="WdatePicker()">
下载地址：[http://download.csdn.net/detail/eastmount/8454441](http://download.csdn.net/detail/eastmount/8454441)
最后希望文章对大家有所帮助，如果有错误或不足之处，请海涵！
(By:Eastmount 2015-2-23[http://blog.csdn.net/eastmount](http://blog.csdn.net/eastmount))




