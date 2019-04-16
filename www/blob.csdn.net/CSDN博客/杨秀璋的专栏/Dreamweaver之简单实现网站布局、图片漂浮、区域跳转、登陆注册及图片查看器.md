# Dreamweaver之简单实现网站布局、图片漂浮、区域跳转、登陆注册及图片查看器 - 杨秀璋的专栏 - CSDN博客





2018年11月22日 17:38:27[Eastmount](https://me.csdn.net/Eastmount)阅读数：409
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









这篇文章是作者《中小型网站开发及web设计》课程给同学们分享的网页设计在线知识，主要涉及Dreamweaver实现网站布局、简单站点开发、区域跳转、注册登录界面、图片查看器等内容，课程前期已经详细讲解了TXT撰写HTML基础代码，后期准备结合软件加深下。非常基础的文章，希望对初学者有所帮助。

1.Dreamweaver创建站点及注册页面

2.DW实现网页区域跳转

3.图片广告漂浮及Spry菜单栏

4.网站简单布局及样式设置

5.JS登陆界面实现

6.图片浏览器
代码下载地址：[https://download.csdn.net/download/eastmount/10801407](https://download.csdn.net/download/eastmount/10801407)

PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

# 一. 创建站点及注册页面

首先，创建站点。点击“站点”=>“新建站点”。
![](https://img-blog.csdnimg.cn/20181115151627314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
创建的站点比如位于桌面“eastmount”文件夹中，整个网站相关的内容都存放在该文件夹中。
![](https://img-blog.csdnimg.cn/20181115151815253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
选中站点，右键新建HTML文件及文件夹。
![](https://img-blog.csdnimg.cn/20181115152151615.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
在站点中创建一个“register.html”文件，如下所示：
![](https://img-blog.csdnimg.cn/20181122162513903.png)
插入的代码如下图所示：
![](https://img-blog.csdnimg.cn/20181122162723789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**register.html**

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

运行结果如下所示：
![](https://img-blog.csdnimg.cn/2018112216312341.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 二. DW实现网页区域跳转

接下来讲解网页区域跳转，首先创建新的网页 main1.html。
![](https://img-blog.csdnimg.cn/20181122163351472.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
然后插入一张图片，如下所示：
![](https://img-blog.csdnimg.cn/2018112216352995.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181122163610104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
通过div设置图片居中，该图片建议学生采用PS技术构建自己的网页主界面。接在在Dreamweaver中点击图片，在左下角中可以插入热点区域。
![](https://img-blog.csdnimg.cn/2018112216371155.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
可以选择矩形、圆形或多边形。
![](https://img-blog.csdnimg.cn/20181122163837655.png)
选中之后可以勾线我们的“个人简介”等字样，当鼠标点击这些区域时能够进行跳转。底部可以添加要跳转的网址，比如百度或本地的HTML网页。
![](https://img-blog.csdnimg.cn/20181122164425748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/2018112216450044.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
其实热点区域跳转对应的HTML标签是MAP，如下：
![](https://img-blog.csdnimg.cn/20181122164610808.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**main1.html**

```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>

<body>
	<div align="center">
		<img src="images/bg.png" width="900" height="486" usemap="#Map" border="0" />
        <map name="Map" id="Map">
          <area shape="rect" coords="51,115,172,156" href="http://www.baidu.com" />
          <area shape="rect" coords="203,82,268,127" href="#" />
          <area shape="rect" coords="286,146,400,194" href="#" />
          <area shape="rect" coords="432,176,531,214" href="#" />
          <area shape="rect" coords="555,112,660,156" href="#" />
          <area shape="rect" coords="715,117,810,155" />
        </map>
    </div>
</body>
</html>
```

跳转过程如下所示，点击个人简介跳转至百度主页。
![](https://img-blog.csdnimg.cn/20181122164728747.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181122164735905.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 三. 图片广告漂浮及Spry菜单栏

接着创建一个 pic.html文件，并添加相应代码。其中images文件夹中插入一张 test.jpg 图片。
![](https://img-blog.csdnimg.cn/20181122165255278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
对应的代码如下所示：

```
<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title></title>
</head>
<body>
    <style type="text/css">
        #demo {
            width: 100px;
            height: 100px;
            position:absolute;
            border-radius:50px;
        }
    </style>
 
    <script type="text/javascript">
        window.onload = function(){
            var demo = document.getElementById('demo');
            var sx = sy = 10;
            var x = y = 0;
 
            function move(){
                if(document.documentElement.clientWidth - demo.offsetWidth-10 < x || x < 0){
                    sx = -sx;
                }
 
                if(document.documentElement.clientHeight - demo.offsetHeight-10 < y || y < 0){
                    sy = -sy;
                }
 
                x = demo.offsetLeft + sx;
                y = demo.offsetTop + sy;
 
                demo.style.left = x + 'px';
                demo.style.top = y + 'px';
            }
 
            var timer = setInterval(move, 100);
 
            demo.onmouseover = function(){
                clearInterval(timer);
            }
 
            demo.onmouseout = function(){
                timer = setInterval(move, 100);
            }
        }
    </script>
 
    <img src="images/test.jpg" id="demo" />
    
</body>
</html>
```

注意：该文件将JavaScript代码切入其中，没有格外的JS文件。

运行结果如下图所示：
![](https://img-blog.csdnimg.cn/2018112216562039.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
接着讲解网站导航栏，可以通过Dreamweaver插入Spry菜单栏实现。
![](https://img-blog.csdnimg.cn/20181122171641522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
选择水平菜单栏。
![](https://img-blog.csdnimg.cn/2018112217171424.png)
最终结果如下图所示，更多知识希望同学们结合TABEL或DIV+CSS布局进行设计。
![](https://img-blog.csdnimg.cn/20181122171812965.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 四. 网站简单布局及样式设置

下面补充一个简单的界面布局，代码如下：
**main2.html**

```
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>无标题文档</title>
<style type="text/css">
	.1{
		position:absolute;
		margin-top:20px;
		width:1200px;
		text-align:center;
	}
	.toubu {
		position:absolute;
		font-size: 36px;
		color:#333;
		margin-top:10px;
		margin-left:230px;
		width:100px;
	}
	.diyi{
		position:absolute;
		height:40px;
		width:600px;
		border-radius:25px;
		background-color:#0C9;
		margin-left:550px;
		margin-top:0px;
		font-size:36px;
		padding-left:15px;
		padding-top:6px;
		padding-bottom:6px;
	}
	a{
		text-decoration:none;
	}
	.dier{
		position:absolute;
		margin-top:60px;
		margin-left:80px;
				
	}
	
	p {
		font-weight: 400;
	}
						
	h2 {
		color: #9FF;
	}
</style>
</head>

<body >
    <header>
      <div class="1">
            <div class="toubu">
              <span>秀璋</span >
            </div>
    
            <span class="diyi">
              <span ><marquee height="140px">欢迎访问Eastmount的主页</marquee></span>
          </span>
      <div>
          
      <div>          
      </div>
    </header>
     <div class="dier">
     <img  height="300px" width="1200px" src="images/bg02.jpg"  />
    </div>
</body>
</html>
```

插入图片bg02.jpg，显示结果如下所示：
![](https://img-blog.csdnimg.cn/20181122172130983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181122172206948.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 五. JS简单登陆界面实现

下面简单讲解一个登陆界面实现，该界面采用JS、CSS等文件。同学们在制作网站过程中，可以从开源网站下载一些比较好的代码，然后结合PS等技术修改为自己的成果。

如下图所示，在站点中插入已下载的图片、CSS、JS等文件。
![](https://img-blog.csdnimg.cn/2018112217260723.png)
然后建立 login.html 并添加对应的代码，代码中含有简单的输入用户名、密码判断。
![](https://img-blog.csdnimg.cn/2018112217243929.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
运行结果如下所示：
![](https://img-blog.csdnimg.cn/20181122172651111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**login.html**

```
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>系统登录</title>
<link href="css/login.css" rel="stylesheet" rev="stylesheet" type="text/css" media="all" />
<link href="css/demo.css" rel="stylesheet" rev="stylesheet" type="text/css" media="all" />
<script type="text/javascript" src="js/jquery1.42.min.js"></script>
<script type="text/javascript" src="js/jquery.SuperSlide.js"></script>
<script type="text/javascript" src="js/Validform_v5.3.2_min.js"></script>
<script>
	$(function(){
		$(".i-text").focus(function(){
			$(this).addClass('h-light');
		});
	
		$(".i-text").focusout(function(){
			$(this).removeClass('h-light');
		});
	
		$("#username").focus(function(){
			var username = $(this).val();
			if(username=='输入账号'){
				$(this).val('');
			}
		});
		
		$("#username").focusout(function(){
			var username = $(this).val();
			if(username==''){
			$(this).val('输入账号');
			}
		});
		
		$("#password").focus(function(){
			 var username = $(this).val();
			 if(username=='输入密码'){
				 $(this).val('');
			}
		});
		
		$("#yzm").focus(function(){
			 var username = $(this).val();
			 if(username=='输入验证码'){
			 $(this).val('');
			}
		});
		
		$("#yzm").focusout(function(){
			 var username = $(this).val();
			 if(username==''){
				$(this).val('输入验证码');
			 }
		});
	
		$(".registerform").Validform({
			tiptype:function(msg,o,cssctl){
				var objtip=$(".error-box");
				cssctl(objtip,o.type);
				objtip.text(msg);
			},
			ajaxPost:true
		});
	
	});

</script>
</head>
<body>

<div class="header">
  <h1 class="headerLogo"><a title="后台管理系统" target="_blank" href="http://www.jq22.com"><img alt="logo" src="images/logo.gif"></a></h1>
	<div class="headerNav">
		<a target="_blank" href="http://www.jq22.com">华软官网</a>
		<a target="_blank" href="http://www.jq22.com">关于华软</a>
		<a target="_blank" href="http://www.jq22.com">开发团队</a>
		<a target="_blank" href="http://www.jq22.com">意见反馈</a>
		<a target="_blank" href="http://www.jq22.com">帮助</a>	
	</div>
</div>

<div class="banner">
<div class="login-aside">
  <div id="o-box-up"></div>
  <div id="o-box-down"  style="table-layout:fixed;">
  <div class="error-box"></div>
  <form class="registerform"action="demo/ajax_post.jsp">
   <div class="fm-item">
	   <label for="logonId" class="form-label">MISS系统登陆：</label>
	   <input type="text" value="输入账号" maxlength="100" id="username" class="i-text" ajaxurl="demo/valid.jsp"  datatype="s6-18" errormsg="用户名至少6个字符,最多18个字符！"  >    
       <div class="ui-form-explain"></div>
  </div>

  <div class="fm-item">
	   <label for="logonId" class="form-label">登陆密码：</label>
	   <input type="password" value="" maxlength="100" id="password" class="i-text" datatype="*6-16" nullmsg="请设置密码！" errormsg="密码范围在6~16位之间！">    
       <div class="ui-form-explain"></div>
  </div>

  <div class="fm-item pos-r">
	   <label for="logonId" class="form-label">验证码</label>
	   <input type="text" value="输入验证码" maxlength="100" id="yzm" class="i-text yzm" nullmsg="请输入验证码！" >    
       <div class="ui-form-explain"><img src="images/yzm.jpg" class="yzm-img" /></div>
  </div>

  <div class="fm-item">
	   <label for="logonId" class="form-label"></label>
	   <input type="submit" value="" tabindex="4" id="send-btn" class="btn-login"> 
       <div class="ui-form-explain"></div>
  </div>
  </form>
  </div>
</div>

	<div class="bd">
		<ul>
			<li style="background:url(themes/theme-pic11.jpg) #CCE1F3 center 0 no-repeat;"><a target="_blank" href="http://www.jq22.com"></a></li>
			<li style="background:url(themes/theme-pic21.jpg) #BCE0FF center 0 no-repeat;"><a target="_blank" href="http://www.jq22.com"></a></li>
		</ul>
	</div>
	<div class="hd"><ul></ul></div>
</div>
<script type="text/javascript">jQuery(".banner").slide({ titCell:".hd ul", mainCell:".bd ul", effect:"fold",  autoPlay:true, autoPage:true, trigger:"click" });</script>
<div class="banner-shadow"></div>
</body>
</html>
```

# 六. 图片浏览器

就个人而言，我非常推崇同学们学习别人的代码进行重构，再制作属于自己的作品，但需要注明出处。比如下面这是来自 HTML5 Tricks 网站的图片查看器。

官网地址：[https://www.html5tricks.com/jquery-hor-thumb-slider.html](https://www.html5tricks.com/jquery-hor-thumb-slider.html)

下载地址：[https://download.csdn.net/download/eastmount/10801407](https://download.csdn.net/download/eastmount/10801407)![](https://img-blog.csdnimg.cn/20181122173335956.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181122173324350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
代码如下：

```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>带缩略图水平切换的jQuery焦点图插件DEMO演示</title>

<script type="text/javascript" src="js/jquery-1.9.1.min.js"></script>

<style type="text/css">
*{margin:0;padding:0;list-style-type:none;}
body{background:url(img/bg.gif);}
a,img{border:none;}

.zx_slider{position:relative;margin:40px auto 0 auto;width:1002px;height:350px;overflow:hidden;border-radius:5px;}
.zx_slider .imgbox{width:3750px;height:310px;position:absolute;left:5px;top:4px;}
.zx_slider .imgbox img{float:left;width:750px;height:310px;}
.zx_slider .png{width:1002px;height:350px;left:0px;top:0px;position:absolute;background:url(img/slider_bg.png);_background:none;filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src="img/slider_bg.png");}
.zx_slider .lbtn{cursor:pointer;width:45px;height:55px;position:absolute;left:0px;bottom:-3px;background:url(img/slider_l_btn.png);_background:none;filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src="img/slider_l_btn.png");}
.zx_slider .rbtn{cursor:pointer;width:45px;height:55px;position:absolute;left:718px;bottom:-3px;background:url(img/slider_r_btn.png);_background:none;filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(src="img/slider_r_btn.png");}
.zx_slider .btnbox{width:750px;height:60px;position:absolute;left:30px;bottom:0px;}
.zx_slider .btnbox a{cursor:pointer;display:inline-block;float:left;width:126px;height:54px;margin:0 0 0 10px;}
.zx_slider .btnbox a img{position:relative;display:inline;width:120px;height:48px;float:left;border:3px solid #392101;border-radius:2px;}
.zx_slider .btnbox a .img_hover{border:3px solid #e4471f;border-radius:2px;}
.zx_slider .slider_p{overflow:hidden;width:220px;height:290px;position:absolute;right:15px;top:15px;}
.zx_slider .slider_p h3{margin:20px 0 10px 0;font-size:20px;text-align:center;color:#faa629;}
.zx_slider .slider_p p{line-height:26px;font-size:14px;color:#faa629;}
.hi_btn{width:740px;height:330px;position:absolute;left:0px;top:0px;display:block;}
</style>

</head>

<body>

<div class="zx_slider">

    <div class="imgbox">
        <img alt="Danx" src="img/danx1.jpg" width="750" height="310" />
        <img alt="Danx" src="img/danx2.jpg" width="750" height="310" />
        <img alt="Danx" src="img/danx3.jpg" width="750" height="310" />
        <img alt="Danx" src="img/danx4.jpg" width="750" height="310" />
        <img alt="Danx" src="img/danx5.jpg" width="750" height="310" />
    </div>
	
    <div class="png"></div>
	
    <a href="#" id="hi_btn0" class="hi_btn"></a>
    <a href="#" id="hi_btn1" class="hi_btn"></a>
    <a href="#" id="hi_btn2" class="hi_btn"></a>
    <a href="#" id="hi_btn3" class="hi_btn"></a>
    <a href="#" id="hi_btn4" class="hi_btn"></a>
	
    <div class="btnbox">
        <a onmouseover="btn_m_over(0)"><img alt="Danx" id="btn_img0" class="thumb img_hover" src="img/danx1.jpg" /></a>
        <a onmouseover="btn_m_over(1)"><img alt="Danx" id="btn_img1" class="thumb" src="img/danx2.jpg" /></a>
        <a onmouseover="btn_m_over(2)"><img alt="Danx" id="btn_img2" class="thumb" src="img/danx3.jpg" /></a>
        <a onmouseover="btn_m_over(3)"><img alt="Danx" id="btn_img3" class="thumb" src="img/danx4.jpg" /></a>
        <a onmouseover="btn_m_over(4)"><img alt="Danx" id="btn_img4" class="thumb" src="img/danx5.jpg" /></a>
    </div>
	
    <div class="lbtn"></div>
    <div class="rbtn"></div>
	
    <div class="slider_p">
	
        <div id="slider_p0" class="slider_p_div">
			<h3>标题1</h3>
			<p>百度（Nasdaq简称：BIDU）是全球最大的中文搜索引擎，2000年1月由李彦宏、徐勇两人创立于北京中关村，致力于向人们提供“简单，可依赖”的信息获取方式。“百度”二字源于中国宋朝词人辛弃疾的《青玉案·元夕》词句“众里寻他千百度”，象征着百度对中文信息检索技术的执著追求。</p>
        </div>
		
        <div id="slider_p1" class="slider_p_div">
			<h3>标题2</h3>
			<p>2000年1月由李彦宏、徐勇两人创立于北京中关村，致力于向人们提供“简单，可依赖”的信息获取方式。“百度”二字源于中国宋朝词人辛弃疾的《青玉案·元夕》词句“众里寻他千百度”，象征着百度对中文信息检索技术的执著追求。</p>
        </div>
		
        <div id="slider_p2" class="slider_p_div">
			<h3>标题3</h3>
			<p>致力于向人们提供“简单，可依赖”的信息获取方式。“百度”二字源于中国宋朝词人辛弃疾的《青玉案·元夕》词句“众里寻他千百度”，象征着百度对中文信息检索技术的</p>
        </div>
		
        <div id="slider_p3" class="slider_p_div">
			<h3>标题4</h3>
			<p>您到卡上黑暗圣经卡萨丁</p>
        </div>
		
        <div id="slider_p4" class="slider_p_div">
			<h3>标题5</h3>
			<p>打考勤卡和饭来欺骗卡号发</p>
        </div>
		
    </div>
	
</div><!--zx_slider end-->


<script type="text/javascript">
var slider_count=4;
$(".slider_p_div").hide();
$("#slider_p0").show();
$(".hi_btn").hide();
$("#hi_btn0").show();
$(".img_hover").animate({bottom:'10px'});
var slider_i=1;
var m_over=true;

function zx_slider(){
	if(m_over){
		if(slider_i<0){slider_i=slider_count;}
		//p
		$(".slider_p_div").hide();
		$("#slider_p"+slider_i).show();
		//p end
		//hi_btn
		$(".hi_btn").hide();
		$("#hi_btn"+slider_i).show();
		//hi_btn end
		//btn
		$(".btnbox img").stop(true,true);
		$(".btnbox img").removeClass("img_hover");
		$(".btnbox img").animate({bottom:'0px'},200);
		$("#btn_img"+slider_i).addClass("img_hover");
		$("#btn_img"+slider_i).animate({bottom:'10px'},200);
		//btn end
		$(".imgbox").stop();
		$(".imgbox").animate({left:'-750'*slider_i+'px'});
		if(slider_i<slider_count){
			slider_i++;	
		}else{
			slider_i=0;
		}
	}
}

$(".lbtn").click(function(){
	m_over=true;
	if(slider_i==0){slider_i=slider_count-1}else{slider_i=slider_i-2;}
	zx_slider();
	m_over=false;
});

$(".rbtn").click(function(){
	m_over=true;
	zx_slider();
	m_over=false;
});

function btn_m_over(btn_i){
	if(slider_i-1!=btn_i){
		m_over=true;
		slider_i=btn_i;
		zx_slider();
		m_over=false;
	}
}

zx_timer=setInterval("zx_slider();",5000); 
$(".zx_slider").mouseover(function(){
	m_over=false;
});
$(".zx_slider").mouseout(function(){
	m_over=true;
});
</script>

<div style="text-align:center;clear:both">
<script src="/gg_bd_ad_720x90.js" type="text/javascript"></script>
<script src="/follow.js" type="text/javascript"></script>
</div>
</body>
</html>
```

希望文章对大家有所帮助，如果有错误或不足之处，还请原谅。浮躁了两天，又开始回归正规，致深夜编程奋斗的青春少年，一起加油。分享一首喜欢的诗:

一生百年凡尘苦，

风华燃尽指尖输。

人间岁月尽虚度，

崎岖坎坷走迷途。

梅傲寒霜香如故，

旭日依旧东方出。

回首前世无为路，

三十余载形神孤。

晚安，娜璋同在同乐[小狗]
代码下载地址：[https://download.csdn.net/download/eastmount/10801407](https://download.csdn.net/download/eastmount/10801407)

（By：Eastmount 2018-11-22 下午5点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)






