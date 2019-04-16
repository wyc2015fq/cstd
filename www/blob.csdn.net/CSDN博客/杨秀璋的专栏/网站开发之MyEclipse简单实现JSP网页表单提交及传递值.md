# 网站开发之MyEclipse简单实现JSP网页表单提交及传递值 - 杨秀璋的专栏 - CSDN博客





2018年11月11日 00:48:28[Eastmount](https://me.csdn.net/Eastmount)阅读数：1299
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









本文主要是作者《中小型网站开发与设计》课程的内容，非常基础的文章，主要是指导学生学会用MyEclipse实现JSP网页表单提交及传递参数。希望大家喜欢这篇文章，基础文章，不喜勿喷~

PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一. MyEclipse实现网页动态表单提交

**1.新建Web Service工程，名称为test02。**
![](https://img-blog.csdnimg.cn/20181110161721272.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181110162115244.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.查看网页JSP源代码，位于WebRoot文件下，src用于编写JAVA后台代码。**
![](https://img-blog.csdnimg.cn/20181110162200450.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.右键项目，点击MyEclipse Server Application运行程序。**
![](https://img-blog.csdnimg.cn/20181111000514845.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
运行结果如下图所示，显示“This is JSP page.”。
![](https://img-blog.csdnimg.cn/2018111100092471.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**4.插入HTML登录页面代码，并显示相关结果。**

```
<body>
  	<form action="" method="post" name="form">
    <table border="1" align="center">
    	<tr><td colspan="2" align="center"><h2>贵州财经大学登陆</h2></td></tr>
    	<tr><td>用户名:</td>
    		<td><input type="text" name="t1" value=""/></td></tr>
    	<tr><td>密码:</td>
    		<td><input type="password" name="t2" /></td></tr>
    	<tr><td colspan="2" align="center"><input type="submit" name="t3" value="点击试试">
    		<input type="reset" name="t4" value="取消算了"/></td></tr>
    </table>
    </form>
  </body>
```

运行结果如下所示，一个比较简单的登录页面。
![](https://img-blog.csdnimg.cn/20181111001522420.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181111001531584.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**5.通过 request.getParameter(标签参数) 获取传递的值。**

完整代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>Login Page</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
  </head>
  
  <body>
  	<form action="" method="post" name="form">
    <table border="1" align="center">
    	<tr><td colspan="2" align="center"><h2>贵州财经大学登陆</h2></td></tr>
    	<tr><td>用户名:</td>
    		<td><input type="text" name="t1" value=""/></td></tr>
    	<tr><td>密码:</td>
    		<td><input type="password" name="t2" /></td></tr>
    	<tr><td colspan="2" align="center"><input type="submit" name="t3" value="点击试试">
    		<input type="reset" name="t4" value="取消算了"/></td></tr>
    </table>
    </form>
    <% String a = request.getParameter("t1");
    	if(a!=null) {
    		try {
        		out.print("当前用户名为:"+a);
        	}catch(NumberFormatException e) {
        		out.print("请输入用户名和密码");
        	}
    	}
    
    %>
  </body>
</html>
```

其中核心代码为 String a = request.getParameter(“t1”) 获取用户名。运行结果如下：
![](https://img-blog.csdnimg.cn/20181111002156999.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
但是通常网站提交的值会传递至另一个页面显示，接下来创建一个新的页面MyJsp.jsp。

**6.创建新的页面 MyJsp.jsp 显示表单提交的结果。**

在WebRoot文件夹下右键新建JSP文件。
![](https://img-blog.csdnimg.cn/20181111002840626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
创建的文件名为MyJsp.jsp。
![](https://img-blog.csdnimg.cn/20181111002536731.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
代码如下：

```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    <title>Main page</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
     <%
     	request.setCharacterEncoding("UTF-8");
    	String a = request.getParameter("t1");
    	String b = request.getParameter("t2");
    	if(a!=null) {
    		out.print("<marquee direction=left>当前用户名为:" + a +"<br />");
    		out.print("密码为:" + b + "</marquee>");
    	}
    %>
  </body>
</html>
```

**index.jsp 核心代码如下：**
![](https://img-blog.csdnimg.cn/20181111002807777.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**MyJsp.jsp 核心代码如下：**
![](https://img-blog.csdnimg.cn/20181111002746176.PNG)
最后的显示结果如下所示：
![](https://img-blog.csdnimg.cn/20181111002910659.PNG)
# 二. HTML注册及登录页面

最后补充三段比较好看的HTML代码供大家学习，建议大家寻找比较好的模板，然后用 MyEclipse 修改成自己的网站。

**1.注册页面**

```
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
<style type="text/css">
<!--
.STYLE1 {color: #FF0000}
-->
</style>
</head>
 
<body>
<form id="form1" name="form1" method="post">
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
      <td><div align="right">插入文件:<span class="STYLE1">*</span></div></td>
      <td><input type="file" name="wj" id="wj" value="" style=width:165pt; maxlength="50"/></td>
    </tr>
    <tr>
      <td><div align="right">性别:<span class="STYLE1">*</span></div></td>
      <td>男<input type="radio" value="男" name="sex"> 
	女<input type="radio" value="女" name="sex"> 
      </td>
    </tr>
    <tr>
      <td><div align="right">兴趣:<span class="STYLE1">*</span></div></td>
      <td>吃饭<input type="checkbox" value="吃饭" name="interest1"> 
	睡觉<input type="checkbox" value="睡觉" name="interest2"> 
	写代码<input type="checkbox" value="睡觉" name="interest3"> 
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
      <td><input type="date" name="birth" id="birth" value="" 
      	style=width:165pt; maxlength="50" ></td>
    </tr>
    <tr>
      <td><div align="right">喜欢颜色:<span class="STYLE1">*</span></div></td>
      <td><input type="color" name="col" id="col" value="" 
      	style=width:165pt; maxlength="50" ></td>
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

运行结果如下图 所示：
![](https://img-blog.csdnimg.cn/20181111003952496.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181111004025383.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.table简单布局**

```
<html>
	<head>
		<title>图片查看器</title>
		<script>
			function showDatu(src) {
				document.getElementById("big").src = src;
			}
		</script>
	</head>
	<body>
		<table border="1" style="table-layout:fixed" width="80%" height="80%" align="center">
			<tr>
			<td colspan=4 align="center"><img src="images/gufe_logo.png"></td>
			</tr>
			<tr>
			<td align="center"><h3><a href="#">贵财概况</a></h3></td>
			<td align="center"><h3><a href="#">组织机构</a></h3></td>
			<td align="center"><h3><a href="#">校园服务</a></h3></td>
			<td align="center"><h3><a href="#">教学教育</a></h3></td>
			</tr>
			<tr height="50%" width="60%">
			<td colspan=2 width="60%" align="center" style="padding:10px">
				<img src="images/wall1.jpg" width="70%" height="60%" id="big" />
		<br />
	<img src="images/wall_s1.jpg" width="60" height="60" onmouseover="showDatu('images/wall1.jpg')"/>
	<img src="images/wall_s2.jpg" width="60" height="60" onmouseover="showDatu('images/wall2.jpg')" />
	<img src="images/wall_s3.jpg" width="60" height="60" onmouseover="showDatu('images/wall3.jpg')" />
	<img src="images/wall_s4.jpg" width="60" height="60" onmouseover="showDatu('images/wall4.jpg')" />
			</td>
			<td>
				<a href="#">贵州财经大学数据科学精品案例库采购成交公告</a><br /><br />
				<a href="#">学报编辑部关于邀请参加学术会议的通知</a><br /><br />
				<a href="#">“强体魄、铸儒魂”2018年贵州财经大学足球赛竞赛规程</a><br /><br />
			</td>
			<td >
				<form >	
				Username: <input type="text" placeholder="请输入用户名"/><br />
				Password: <input type="password" /><br />
				<input type="submit" value="登陆"/>&nbsp;&nbsp;<input type="reset" value="取消"/>
				</form>
			</td>
			</tr>
			<tr>
			<td colspan=4 align="center">
			©2018 YXZ 京ICP证XXX号  京公网安备001号 <br /> 
			<a href="http://www.baidu.com">摆渡</a> | <a href="http://www.baidu.com">数字贵才</a>
			</td>
			</tr>
		</table>
	</body>
</html>
```

布局效果如下图所示：
![](https://img-blog.csdnimg.cn/20181111004428426.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.登录界面**
![](https://img-blog.csdnimg.cn/20181111004526828.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181111004539392.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
源代码下载地址：

希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来。

（By：Eastmount 2018-11-11 晚上12点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)



