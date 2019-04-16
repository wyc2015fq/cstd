# 网站开发之DIV+CSS简单布局网站入门篇（五） - 杨秀璋的专栏 - CSDN博客





2016年11月08日 12:05:34[Eastmount](https://me.csdn.net/Eastmount)阅读数：6790
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)









        这篇文章主要介绍如何使用DIV和CSS简单布局一个网站的首页，通常将网站划分为顶部（Logo、导航条）、中部（页面主要内容、左右栏目）、底部（制作方介绍、超链接）。这是非常基础的一篇引入性文章，采用案例的方式进行介绍的，希望对你有所帮助。运行结果如下图所示：

![](https://img-blog.csdn.net/20161108111307674)



**main.html主页面代码**

        主要通过div进行布局的，同时<h2><a></a></h2>用户设置导航条，鼠标悬停时背景颜色切换。



```
<html>
<head>
	<title>
		网站主页
	</title>
	<link rel="stylesheet" href="css/main.css" >
</head>
<!-- 首页 -->
<body>
		<div id="bg">
			<img src="images/bg.jpg" width="100%" height="100%" />
		</div>
		<div id="bt">
			<h2 class="xz"><a href="#">学校概况</a></h2>
			<h2><a href="jsjj.html">教师简介</a></h2>
			<h2><a href="#">校园生活</a></h2>
			<h2><a href="#">校园文化</a></h2>
			<h2><a href="#">联系我们</a></h2>
		</div>

		<div id="main">
			<div id="left">
			</div>
			<div id="right">
			</div>
		</div>

		<div id="footer">

		</div>

</body>
</html>
```

**css/main.css代码：**

        该部分代码主要是CSS文件，用于布局和设置主页面。



```
#bg {
	text-align: center;
	margin:0 auto;
	width: 80%;
	height: 100px;
	/* background-image: url("../images/bg.jpg"); */
}

#bt {
	margin:0 auto;         /* 水平居中 */
	margin-top: 10px;
	width: 80%;
	height: 40px;
	background-color: yellow;

}

h2 {
	float: left;           /* 水平显示,否则竖直显示 */
	margin-top: 0px;       /* 置顶 */
	margin-left: 10px;
	width: 18%;
	background-color: red;
	height: 34px;
	font-size: 24px;
	text-align: center;
	padding-top: 6px;
}

a {
	text-decoration: none;  
}

a:hover {
	color: black;
}

h2:hover {
	background-color: #0F0;
}

#main {
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
	width: 40%;
	height: 90%;
	background-color: #9ACD32;
}

#right {
	float: left;
	margin-left: 20px;
	margin-top: 5px;
	width: 50%;
	height: 90%;
	background-color: #BFEFFF;
}

#footer {
	margin:0 auto;         /* 水平居中 */
	margin-top: 10px;
	width: 80%;
	height: 50px;
	background-color: #8B2500;
}

#main2 {
	margin:0 auto;         /* 水平居中 */
	margin-top: 10px;
	width: 80%;
	height: 60%;	
	background-color: #F00;

}

.xz {
	float: left;           /* 水平显示,否则竖直显示 */
	margin-top: 0px;       /* 置顶 */
	margin-left: 10px;
	width: 18%;
	background-color: #0F0;
	height: 34px;
	font-size: 24px;
	text-align: center;
	padding-top: 6px;

}
```


        下面讲解几个重点：
        1、在<div class="bg">布局过程中，需要在CSS中设置"margin:0 auto;"，才能让它居中显示，这段代码的含义是：第一个值就是元素的上下边距0，第二个值就是左右边距。当元素的定义了width属性时，auto还能实现居中的效果。

        2、在CSS中设置h2，需要添加"float: left;"，使其水平显示，不增加该句的效果如下所示：


![](https://img-blog.csdn.net/20161106003400051)




        3、整个DIV布局代码如下所示，h2会向下移动一段距离，这时CSS中通过"margin-top: 4px;"进行微调。



![](https://img-blog.csdn.net/20161106003617895)



        4、悬停的核心代码如下所示，其中"text-decoration: none;"设置超链接无下划线，然后是悬停在超链接a和字体h2的变换效果。注意冒号(:)和hover之间不能有空格，否则效果消失。



![](https://img-blog.csdn.net/20161106003852399)





        简单补充CSS内容，更多的是希望你从课本中学习，这篇文章我以案例为主。

**什么是CSS**
CSS（Cascading Style Sheet，层叠样式表）是一种格式化网页的标准方式，它扩展了 HTML 的功能，使网页设计者能够以更有效的方式设置网页格式。它是将样式信息与网页内容分离的一种标记性语言。

**样式定义的语法**
        样式表项的组成：
        Selector{property1:value1;property2:value2;property3:value3;……}
        Selector定义样式作用的对象，property为CSS属性，value为属性对应的值。



![](https://img-blog.csdn.net/20161018142432363)


**CSS直接在标记符中嵌套**
        HTML 标记符的 style 属性。
        例如：<P style=“text-align:center” >，其中，style属性的取值形式为“CSS属性:CSS属性值”，使用多个属性用分号分隔。
        在STYLE 标记符定义样式
        <STYLE>样式定义 </STYLE>
        样式定义的方式为：
        Selector{property1:value1;property2:value2;property3:value3;……}

```
<HTML>
<HEAD><TITLE>在标记符中直接嵌套样式信息</TITLE>
<STYLE>

P{ font-size:24px; text-align:center }
H1{ font-family:楷体_gb2312; text-align:center }

</STYLE>
</HEAD>
<BODY>
  <H1>一代人</H1>
  <P>黑夜给了我黑色的眼睛<BR>我却用它寻找光明</P>
</BODY>
</HTML>
```

        运行结果如下所示：

![](https://img-blog.csdn.net/20161018145914383)


**链接外部样式表文件**使用LINK 标记符：
        <LINK rel=“stylesheet” type=“text/css” href=“stylesheet.css”>
        样式表文件可以用文本编辑器编辑，也可以用FP或DW编辑，内容为样式定义。

```
<HTML>
<HEAD><TITLE>链接式样式示例</TITLE>
<LINK rel="stylesheet" type="text/css" href="mycss.css">
</HEAD>
<BODY>
  <H1>一代人</H1>
  <P>黑夜给了我黑色的眼睛<BR>我却用它寻找光明</P>
</BODY>
</HTML>
```

        其中CSS的代码如下所示：



```
P{ font-size:12px; text-align:center }
H1{ font-family:黑体; text-align:center;background-color:green }
```

        运行结果如下所示：

![](https://img-blog.csdn.net/20161018145950336)





        希望文章对你有所帮助，上课内容还需要继续探索。
(By:Eastmount 2016-11-08 中午12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))




