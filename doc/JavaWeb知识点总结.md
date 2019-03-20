# JavaWeb知识点总结



**》一：**

创建Web项目
项目说明：
1、java Resources：java源文件
2、WebContent:网页内容
html、css、js、jsp、资源、配置文件等


HTML:Hyper Text Markup Language超文本标记语言
作用：
标记描述网页内容
语法规则：
1、不区分大小写
2、固定标签
3、标签成对出现，单标签
4、标签可以嵌套使用
5、属性的值必须使用双引号


HTML中属性，一般不建议使用
属性名称固定，属性的值都需要使用双引号

网页中颜色：
1、rgb(r,g,b)
2、#xxxx
3、英文单词
网页中单位：
1、数字或数字px
2、数字%:占用屏幕的比例
网页中注释
<!--注释内容-->
路径：
1、相对路径
不带盘符的路径，相对，一般都是相对当前项目
2、绝对路径
带盘符的路径，又叫物理路径


一、文档标签：
1、<!DOCTYPE> 该标签告知浏览器文档所使用的 HTML 规范
2、<html>标签告知浏览器这是一个 HTML 文档。
html 元素是 HTML 文档中最外层的元素。
html 元素也可称为根元素
3、<head>不会出现在网页正文中
4、<meta>出现在<head>可以标记编码格式、刷新、作者、描述、搜索关键字等
5、<body>网页正在内容
常用标签都是出现在<body>中


二、文字标签
1、<title>出现在<head>中，网页标题
2、<h1>-<h6>标题，从1-到6逐渐变小
3、<font> 规定文本的字体、字体尺寸、字体颜色

三、列表标签
1、<ul>无序标签，前面没有序号，属性：type:可以更改列表签名的符号
2、<li>列表中元素标签
3、<ol>有序标签，前面有序号，属性：start:起始的值，type:类型，数字、字母等

四、块标签
1、<span>行内块标签
2、<p>段落，默认换行
3、<div>块，将网友分割成若干部分,常用属性：align:对齐方式，left\right\center

五、表格标签
1、<table>表格，一般有行和列组成
常用属性：
1、width:宽度
2、border:边框的宽度
3、bgcolor:背景色
4、cellspacing:表格之间的间距--外边距
5、cellpadding：表格内容的间距--内边矩
2、<tr>：行，内部只能出现<th>或<td>
常用属性：
1、align:对齐方式，内部元素的对齐
3、<th>列的标题，出现在tr中，表头单元格
4、<td>列，数据单元格
常用属性：
1、colspan：跨列，切记要包含自己
2、rowspan：跨行
th和td的区别？
th就是内部文字：加粗、居中
td内部文字正常。居左

5、<caption>表格的标题
6、<thead>表格的头部，一般内部存放<th>
7、<tbody>表格的内容，一般存储数据
8、<tfoot>表格的底部

六、表单标签
1、<form>:表单标签，可以搜集用户的信息并提交给指定的服务器
2、<input>:用户输入标签
常用属性：
1、type:类型
取值：
text:文本框
password:密码框
checkbox:复选框,可以选择多个
radio:单选框,只能选择其一，但是要求拥有相同的name
button:按钮
submit:提交，结合<form>表单，将表单内的数据提交给action
reset:重置，结合<form>表单,清空表单的内容

2、name:为当前的标签命名，将来获取标签数据时，会用到
3、class:为当前标签起个类名，主要用在CSS中
4、value:值，标签的值，主要用在按钮上
5、checked:是否选中，h4支持checked,h5支持true或false
3、<select>:下拉框，实现多个元素的单选或复选,需要结合<option>使用
常用属性：
multiple:是否多选
name：名称
disabled：是否可用
4、<textarea>文本域，可以输入多行和多列数据
常用属性：
1、rows:行数
2、cols:列数

七、图像&超链接标签
1、<img>图片
常用属性：
1、alt:鼠标进入时的文字
2、src:加载的图片路径
3、width:设置图片的宽度
4、height:设置图片的高度
2、<a>超链接标签，可以跳转到指定的url
常用属性：
1、href:要打开的路径
2、name:名称

八、框架标签
1、<frameset>框架集合标签，主要用来包括框架的标签
常用属性：
1、cols:内部框架按照宽进行排布
2、rows:内部框架按照高度进行排布
2、<frame>框架，引用其他网页
常用属性：
1、src:要加载的路径
2、scrolling：滚动条的样式，取值：yes 、no 、auto 

九、HTML的特殊字符
1、< &lt;
2、> &gt;
3、空格 &nbsp;
4、版本编号 &copy;

十、H5新增标签
1、<video>视频，可以在网页播放视频
格式：
<video src="movie.ogg" controls="controls">
</video>
2、<audio>音频
3、<canvas>画布
可以绘制2d或3d图
切记画布需要结合js使用

 

form表单的get提交和post提交的区别
HTTP协议的get请求和postde 区别
get:得到，将传递的参数随着url进行传输
特点：
1、大小有限制
2、不安全，拼接url
3、中文乱码，url编码为ISO-8859-1（不支持中文）
4、效率高
post:传输，将传递的参数以流的形式传输
特点：
1、大小无限制
2、安全
3、乱码好解决
4、效率低

 

十一、其他标签
1、<br/>换行
2、<hr/>分割线
3、&nbsp;一个空格

 

**》二 CSS**

HTML:超文本标记语言
描述内容显示在网页中
语法规则：
不区分大小写
标签成对，但是单标签
属性值必须双引号
标签固定
标签嵌套使用

常用标签：
meta
title
div
p
span
a:href
img:src
input
select
textarea
form:action&method
table：border

get和post

 

MIME类型：
就是标记文档的类型
格式：大类型/小类型
css:text/css
js:text/javascript
图片：image/jpg
所有图片：image/*

 

1、CSS是什么？
层叠样式表 (Cascading Style Sheets) 
定义如何显示HTML元素的样式
2、CSS的作用是什么？
1、提高工作效率
2、多样式层叠
3、提高了内容和样式的分离

就是美化HTML显示的内容

3、CSS的语法规则
规则：
选择器{属性名称:值;属性名称:值}
eg:
h1{color:red;}
CSS不区分大小写，但是id和class选择器区分大小写

4、CSS的引入方式有多少？
1、内嵌样式
Html元素都有一个属性：style,直接在html元素内使用
eg:
<h1 style="font-size:20px">今天好好听课</h1>
2、内部样式
在html文档的<head>标签中使用
<style type="text/css">
选择器{属性名称:值}
</style>
3、外部样式表
创建css文件，在网页中的<head>中通过<link>标签引用
eg:
<link rel="stylesheet" type="text/css" href="theme.css" />
rel：文本类型，告知引用的类型
type:MIME类型
href:外部样式文件所在路径
4、导入式外部样式
@import url("外部样式文件路径")
不推荐使用，主要针对IE老版本

样式作用的优先级：
内嵌->内部->外部
就近原则

5、CSS的选择器
1、标签选择器
格式：HTML标签名称{属性名:值}
2、类选择器
要求html的标签使用class属性命名类名
格式：.类名{属性名:值}
HTML标签可以拥有同一个class
3、ID选择器
要求HTML的标签使用id属性名称，id名不可重复
格式：#id名称{属性名:值}
4、派生选择器
规则：选择器(标签、类、id) 内部标签名称{属性名:值}
就是通过标签的内部嵌套来修改指定标签的样式
5、属性选择器
规则：标签名称(可以省略)[属性名称="值"]{属性名:值}
就是通过标签的属性名称来修改指定标签的样式

6、伪类选择器
规则：标签名称:伪类名称{属性名:值}
常用的伪类：
:active 向被激活的元素添加样式。 1 
:focus 向拥有键盘输入焦点的元素添加样式。 2 
:hover 当鼠标悬浮在元素上方时，向元素添加样式。 1 
:link 向未被访问的链接添加样式。 1 
:visited 向已被访问的链接添加样式。 1 
:first-child 向元素的第一个子元素添加样式。 2 
:lang 向带有指定 lang 属性的元素添加样式。 
:first-letter 向文本的第一个字母添加特殊样式。 1 
:first-line 向文本的首行添加特殊样式。 1 
:before 在元素之前添加内容。 2 
:after 在元素之后添加内容。 


6、CSS的属性
1、背景属性
background:背景色/图
background-color:背景色
background-image:背景图
图片过小，标签过大默认会出现背景重复
background-repeat:设置重复样式
取值：
repeat 默认。背景图像将在垂直方向和水平方向重复。 
repeat-x 背景图像将在水平方向重复。 
repeat-y 背景图像将在垂直方向重复。 
no-repeat 背景图像将仅显示一次。
background-position:定位，显示的位置
取值：
left
right
top
bottom
center
具体数值
background-attachment:设置背景图是否跟着一起动
取值：
scroll 默认值。背景图像会随着页面其余部分的滚动而移动。 
fixed 当页面的其余部分滚动时，背景图像不会移动。 
2、文本属性 
1、text-indent：缩进
2、text-align:对齐方式
3、letter-spacing: 字母的间距
4、word-spacing:单词的间距
5、text-transform:变换，大小写转换
取值：
none 
uppercase 
lowercase 
6、 text-decoration:文本装饰
取值：
none 默认。定义标准的文本。 
underline 定义文本下的一条线。 
overline 定义文本上的一条线。 
line-through 定义穿过文本下的一条线。 
blink 定义闪烁的文本。 
7、direction: 文本方向
取值：
ltr 默认。文本方向从左到右。 
rtl 文本方向从右到左。 
3、字体属性
1、font-family：字体类型
楷体、黑体、宋体等
2、font-style：设置是否倾斜
3、font-weight:设置字体的粗细
取值：
[100,900]
正常：400
加粗：700
4、font-size：字体大小
4、列表属性(不常用)
1、list-style-image 将图象设置为列表项标志。 
2、list-style-type 设置列表项标志的类型 

5、表格属性
1、width:宽
2、height:高
3、border:边框
4、text-align 属性设置水平对齐方式，比如左对齐、右对齐或者居中
5、vertical-align 属性设置垂直对齐方式，比如顶部对齐、底部对齐或居中对齐：

6、轮廓属性(不常用)
1、outline-color 设置轮廓的颜色。  
2、outline-style 设置轮廓的样式。 
取值：
none 默认。定义无轮廓。 
dotted 定义点状的轮廓。 
dashed 定义虚线轮廓。 
solid 定义实线轮廓。 
double 定义双线轮廓。双线的宽度等同于 outline-width 的值。 
groove 定义 3D 凹槽轮廓。此效果取决于 outline-color 值。 
ridge 定义 3D 凸槽轮廓。此效果取决于 outline-color 值。 
inset 定义 3D 凹边轮廓。此效果取决于 outline-color 值。 
outset 定义 3D 凸边轮廓。此效果取决于 outline-color 值。 

3、outline-width 设置轮廓的宽度。 

 

7、CSS新增属性

CSS的注释方式：
在<style>标签下使用
/*注释内容*/
文字的尺寸
em:自动适应尺寸，方便字体的放大或缩小
1em=16px
0.75em=12px

默认字体大小16px
px：像素，表达字体大小，缩放的时候有点不好用
为了网页的实用性，推荐字体大小单位em
%:百分比表达字体大小
italic和oblique的区别？
体（italic）是一种简单的字体风格，对每个字母的结构有一些小改动，来反映变化的外观。
与此不同，倾斜（oblique）文本则是正常竖直文本的一个倾斜版本。
通常情况下，italic 和 oblique 文本在 web 浏览器中看上去完全一样。

 

**》三HTML+CSS综合案例**

 

网页中：多个空格会起到一个空格的作用，换行也会起到一个空格的作用

注释：
java中的注释：
1、单行注释 eg：//注释
2、多行注释 eg:/*多行注释*/
3、文档注释 eg:/**文档注释*/
HTML中的注释：
1、<!--html的注释-->
CSS中的注释：只出现在<style>或css文件中
1、/*CSS注释*/
JavaScript中的注释：只出现在<script>或js文件中
1、单行注释 eg://单行注释
2、多行注释 eg:/*多行注释*/

 

1、JS是什么？
JavaScript:脚本语言，运行浏览器
让HTML进行交互

JAVA和JS没任何关系

2、JS的作用是什么？
1、获取和设置HTML元素
2、动态改变页面
3、校验、检测、对事件做出响应等
4、获取浏览器、Cookie等信息


3、JS的引用方式？
1、在<head>引用
<head>
<script type="text/javascript">

</script>
2、在<body>引用
eg:
<body>
<script type="text/javascript">

</script>
</body>
</head>
3、引用外部js文件
<script type="text/javascript" src="js文件路径"/>

js写在head和body的区别？
head中，获取不到HTML元素，因为此时HTML还未加载完成(函数外)
body中，可以获取HTML元素


4、JS的数据类型
java的数据类型：
1、基本数据类型：byte short int long float double char boolean
2、引用数据类型:数组、枚举、类、集合、接口等

JS的数据类型：
1、原始数据类型
Undefined 、number、boolean、string、NULL
原始就是声明之后直接赋值而不通过new来创建
var v=12;
var v=true;
var v="aaa";
2、引用数据类型
Number\Boolean\String\内置类型
就是通过new 创建的额类型都属于引用类型

js中的typeof&instanceof 区别？
 typeof：主要用来显示原始类型，如果显示引用类型无论什么类型都为Object
                   格式：typeof 变量名称
        eg: typeof v1

 instanceof:主要用来验证引用类型，返回值为boolean类型
                   格式： 变量名称 instanceof 引用数据类型  返回true或flase 
        eg: v1     instanceof Boolean

例如：
<body>
<script type="text/javascript">

var v1=new Number(2);

document.write("<br/>类型："+(typeof v1));//类型：object
document.write("<br/>类型："+(v1 instanceof Boolean));//类型：false 

</script>
</body>

5、JS的基本语法

JS的语法规则：
1、区分大小写
2、变量可以声明也可以不声明
3、出现在<script>标签内部或js文件中
4、定义函数需要使用关键字function
5、函数可以有参数和返回值
6、JS内置了很多对象可以直接使用


1、变量
格式：var 变量名称[=值]
js的变量可以不声明直接使用
var v;
v=12;
t=11;

变量命名规则：
1、变量对大小写敏感（y 和 Y 是两个不同的变量） 
2、变量必须以字母或下划线开始 
3、不能是关键字


JS中的变量
全局变量：声明在函数外的变量或者是函数内不带var的变量
局部变量：声明在函数内的变量且带有var，作用域在函数内部

变量如果在函数内部不使用var进行声明，那么默认为全局变量
如果使用var进行声明那么就属于局部变量

2、运算符
java的运算符：算术、比较、赋值、逻辑、位、三目、字符串连接运算符
JS运算符差不多
需要特别注意的：
1、==和===的区别
==只比较内容(number)
===比较内容和类型

例：
x=5；
运算符            描述                                 例子 
 ==      等于                                 x==8 为 false 
 ===     全等（值和类型）          x===5 为 true；x==="5" 为 false 
 
1.1 逻辑运算符&&和||和！
  逻辑运算符
    逻辑运算符用于测定变量或值之间的逻辑。

   给定 x=6 以及 y=3，下表解释了逻辑运算符：

​    运算符                    描述                            例子 
   &&         and          (x < 10 && y > 1) 为 true 
   ||         or           (x==5 || y==5) 为 false 
   !          not          !(x==y) 为 true 


2、js也支持三目运算符
X?Y:Z

程序结构：
1、顺序结构
2、分支结构
3、循环结构
3、分支语句
1、if(条件)
2、switch

4、循环
4.1for语句
格式：for(表达式1;表达式2;表达式3）
	  {循环体}
执行过程：
先执行表达式1再验证表达式2true则执行循环体接着执行表达式3再执行表达式2true继续false结束
4.2while语句
格式：while(表达式)
	{
		循环体;
	}
4.3do/while语句
格式：do{
	循环体;
}while(表达式);

 


6、JS的消息框
js主要提供了三种消息框
  1、普通警告框
   alert("内容");
  2、确认框
   confirm("内容")
会返回一个boolean值，如果点击"确定"返回true，反之false
  3、提示框
   prompt("提示内容","默认值");
会返回提示框中的输入框的内容

7、JS函数
也就是java中的方法
提高代码的复用
格式：

​    function 函数名称([参数名]) {

​        //代码块
​       [return 返回值] //return 返回值   可有可没有
​    }
​    
例：
<head>
<script type="text/javascript">

​      function show1(){
​	  document.write("OK<br/>");
}
</script>
</head>

8、JS事件
就是HTML的事件属性
事件属性的值一般都是JS函数
常用事件：
onclick 单击
ondblclick 双击
onkeydown：键盘键按下
onmouseout 鼠标移出 
onmouseover 鼠标进入
onblur 失去焦点
onfocus 获取焦点
onchange 内容变化
onselect 选择
onsubmit 提交-表单
onload 加载完

 

 


9、JS常用对象

 

10、JS之Window、History 、Location 

 

11、JS的DOM

二级联动

 

12、JS综合案例
点名、抽奖、贪吃蛇、瀑布流等

 

**》四**

 

注释：
java中的注释：
1、单行注释 eg：//注释
2、多行注释 eg:/*多行注释*/
3、文档注释 eg:/**文档注释*/
HTML中的注释：
1、<!--html的注释-->
CSS中的注释：只出现在<style>或css文件中
1、/*CSS注释*/
JavaScript中的注释：只出现在<script>或js文件中
1、单行注释 eg://单行注释
2、多行注释 eg:/*多行注释*/

 

1、JS是什么？
JavaScript:脚本语言，运行浏览器
让HTML进行交互

JAVA和JS没任何关系

2、JS的作用是什么？
	2.1、获取和设置HTML元素
	2.2、动态改变页面
	2.3、校验、检测、对事件做出响应等
	2.4、获取浏览器、Cookie等信息

3、JS的引用方式？
	3.1、在<body>引用
	eg:
		<body>
		<script type="text/javascript">

​		</script>
​		</body>
​	3.2、在<head>引用
		<head>
		<script type="text/javascript">

​		</script>
​		</head>
​	3.3、引用外部js文件

		<script type="text/javascript" src="js文件路径"/>

js写在head和body的区别？
	head中，获取不到HTML元素，因为此时HTML还未加载完成(函数外)
	body中，可以获取HTML元素


4、JS的数据类型
	java的数据类型：
	1、基本数据类型：byte short int long float double char boolean
	2、引用数据类型:数组、枚举、类、集合、接口等

JS的数据类型：
1、原始数据类型
	Undefined 、number、boolean、string、NULL
	原始就是声明之后直接赋值而不通过new来创建
	var v=12;
	var v=true;
	var v="aaa";
	2、引用数据类型
Number\Boolean\String\内置类型
就是通过new 创建的额类型都属于引用类型

js中的typeof&instanceof 区别？
	typeof：主要用来显示原始类型，如果显示引用类型无论什么类型都为Object
	格式：typeof 变量名称
	instanceof:主要用来验证引用类型，返回值为boolean类型
	格式： 变量名称 instanceof 引用数据类型  返回true或flase 

5、JS的基本语法

JS的语法规则：
	1、区分大小写
	2、变量可以声明也可以不声明
	3、出现在<script>标签内部或js文件中
	4、定义函数需要使用关键字function
	5、函数可以有参数和返回值
	6、JS内置了很多对象可以直接使用


1、变量
	格式：var 变量名称[=值]
	js的变量可以不声明直接使用
	var v;
	v=12;
	t=11;

变量命名规则：
	1、变量对大小写敏感（y 和 Y 是两个不同的变量） 
	2、变量必须以字母或下划线开始 
	3、不能是关键字


JS中的变量
	全局变量：声明在函数外的变量或者是函数内不带var的变量
	局部变量：声明在函数内的变量且带有var，作用域在函数内部
	变量如果在函数内部不使用var进行声明，那么默认为全局变量
	如果使用var进行声明那么就属于局部变量

2、运算符
java的运算符：算术、比较、赋值、逻辑、位、三目、字符串连接运算符
	JS运算符差不多
	需要特别注意的：
	1、==和===的区别
	==只比较内容(number)
	===比较内容和类型
	2、js也支持三目运算符
	X?Y:Z

程序结构：
	1、顺序结构
	2、分支结构
	3、循环结构
	3、分支语句
	1、if(条件)
	2、switch

4、循环
	4.1for语句
		格式：for(表达式1;表达式2;表达式3）
			  {循环体}
		执行过程：
		先执行表达式1再验证表达式2true则执行循环体接着执行表达式3再执行表达式2true继续false结束
	4.2while语句
		格式：while(表达式)
			{
				循环体;
			}
	4.3do/while语句
		格式：do{
			循环体;
		}while(表达式);

 


6、JS的消息框
js主要提供了三种消息框
	6.1、普通警告框
	alert("内容");
	6.2、确认框
	confirm("内容")
	会返回一个boolean值，如果点击"确定"返回true，反之false
	6.3、提示框
	prompt("提示内容","默认值");
	会返回提示框中的输入框的内容

7、JS函数
也就是java中的方法
	提高代码的复用
	格式：
	function 函数名称([参数名]) {
	
	//代码块
	[return 返回值]
	}

 

8、JS事件
就是HTML的事件属性
事件属性的值一般都是JS函数
常用事件：
	onclick 单击
	ondblclick 双击
	onkeydown：键盘键按下
	onmouseout 鼠标移出 
	onmouseover 鼠标进入
	onblur 失去焦点
	onfocus 获取焦点
	onchange 内容变化
	onselect 选择
	onsubmit 提交-表单
	onload 加载完

 

 


9、JS常用对象
JS是面向对象的编程语言
9.1Date日期类
获取浏览器的时间(本地电脑的时间)
常用方法：
	getDate() 从 Date 对象返回一个月中的某一天 (1 ~ 31)。 1 3 
	getDay() 从 Date 对象返回一周中的某一天 (0 ~ 6)。 1 3 
	getMonth() 从 Date 对象返回月份 (0 ~ 11)。 1 3 
	getFullYear() 从 Date 对象以四位数字返回年份。 1 4 
	getYear() 请使用 getFullYear() 方法代替。 1 3 
	getHours() 返回 Date 对象的小时 (0 ~ 23)。 1 3 
	getMinutes() 返回 Date 对象的分钟 (0 ~ 59)。 1 3 
	getSeconds() 返回 Date 对象的秒数 (0 ~ 59)。 1 3 
	getMilliseconds() 返回 Date 对象的毫秒(0 ~ 999)。 

 

JS中的定时器：
	1、setTimeout:设置延迟执行的方法和延迟时间
	参数说明：
		1、要重复执行的方法
		2、延迟的时间，毫秒
	返回值：
		1、当前定时器的标记(为了停止)
		2、clearTimeout:停止重复执行的方法
	参数说明：
		1、定时器开始的标记
重复间隔执行方法、计时器、定时刷新等
9.2Number数字
原始类型数字对应的引用类型
常用方法：
	valueOf 返回一个 Number 对象的基本数字值。 
	toFixed 把数字转换为字符串，结果的小数点后有指定位数的数字。 
	toString 把数字转换为字符串，使用指定的基数。 
9.3Math算术运算类
常用的算术运算
常用方法：
	abs(x) 返回数的绝对值。 
	floor(x) 对数进行下舍入。 
	pow(x,y) 返回 x 的 y 次幂。 
	random() 返回 0 ~ 1 之间的随机数。 1 3 
	round(x) 把数四舍五入为最接近的整数。 
9.4String字符串类
常用的字符串的引用类型
常用属性：
	length 字符串的长度 
常用方法：
	charAt() 返回在指定位置的字符。 1 3 
	charCodeAt() 返回在指定的位置的字符的 Unicode 编码。 
	concat() 连接字符串。 
	substr() 从起始索引号提取字符串中指定数目的字符。 1 4 
	substring() 提取字符串中两个指定的索引号之间的字符。 
	toLowerCase() 把字符串转换为小写。 1 3 
	toUpperCase() 把字符串转换为大写。 
	split() 把字符串分割为字符串数组。 
	indexOf() 检索字符串。 
	lastIndexOf() 从后向前搜索字符串。 
	match() 找到一个或多个正在表达式的匹配。 1 4 
	replace() 替换与正则表达式匹配的子串。 1 4 
	search() 检索与正则表达式相匹配的值。 

 

String的substr()和substring的区别？
在截取的起始索引都为正数或0的话，没有区别
如果截取的开始索引为负数，那么substr从要截取的字符串.length+负数索引处开始
而substring则把负数作为0处理
简而言之：substr负数的话倒着截取，substring从0截取

9.5Array数组类
存储多个变量的引用类型，且长度可变
9.1数组的使用：
	9.1.1、创建数组对象三种方式：
			9.1.1.1:new Array();
			9.1.1.2:new Array(长度);
			9.1.1.3:new Array(元素);
9.2常用属性
	length:获取数组的长度
9.3常用方法
	concat() 连接两个或更多的数组，并返回结果。 1 4 
	join() 把数组的所有元素放入一个字符串。元素通过指定的分隔符进行分隔。 1 4 
	pop() 删除并返回数组的最后一个元素 1 5.5 
	push() 向数组的末尾添加一个或更多元素，并返回新的长度。 1 5.5 
	reverse() 颠倒数组中元素的顺序。 1 4 
	shift() 删除并返回数组的第一个元素 1 5.5 
	slice() 从某个已有的数组返回选定的元素 1 4 
	sort() 对数组的元素进行排序 
	splice() 删除元素，并向数组添加新元素。 


注意：
1、java中的数组和JS中数组不一样
2、JS中数组的sort方法按照字母的升序进行排列，如果是数字进行排序需要定义排序函数(2个参数)
3、slice方法，
参数说明：
	第一个参数：指定索引，删除和新增的索引
	第二个参数：删除的元素个数 可以为0
	第三个参数：新增的元素内容，可以有多个

9.6Regexp正则类
校验、匹配、替换等字符串的操作
JS中的正则语法：/正则语法/属性
9.6.1正则中的语法
	9.6.1.1:正则语法
		跟java中一样
		. + * ? [] {} \d \D \w \D \b \B等
	9.6.1.2:正则属性
		i 执行对大小写不敏感的匹配。 
		g 执行全局匹配（查找所有匹配而非在找到第一个匹配后停止）。 
		m 执行多行匹配。 
9.6.2正则类的常用方法
	1、exec:执行正则验证，返回匹配的内容,字符串索引停留在最后一次匹配的索引处
	2、test:执行正则验证，返回true(验证成功)或flase
9.6.3String类中支持正则验证的方法
	1、match:匹配，支持全局匹配(可以返回匹配到的所有的内容)
	2、search:查找，从前往后查找是否符合正则的内容(不支持全局，第一次出现的)
	3、replace:替换，参数说明：第一个参数：字符串或正则表达式，第二个参数：字符串、规定的符合、函数
	4、split：切割
			
	
	
	
10、JS的全局函数
可以在任何一个网页中直接使用
常用函数：
	isNaN() 检查某个值是否是数字。 1 3 
	Number() 把对象的值转换为数字。 1   
	parseFloat() 解析一个字符串并返回一个浮点数。 1 3 
	parseInt() 解析一个字符串并返回一个整数。 1 3 
	String() 把对象的值转换为字符串。 
	encodeURI() 把字符串编码为 URI。 
	decodeURI() 解码某个编码的 URI。 	


	
10、JS的DOM
Document：文档
通过DOM可以获取并操作HTML的元素
10.1DOM获取HTML标签对象的方式
	10.1.1:通过ID
		返回的是一个HTML元素对象
		document.getElementById("id值");
	10.1.2:通过name
		返回的是网页网页中所有此name值的HTML元素数组
		document.getElementsByName("name值");	
	10.1.3:通过HTML标签名称
		返回的是网页中所有此标签的元素数组
		document.getElementsByTagName("name值");
10.2DOM创建HTML元素
	10.2.1、通过元素的innerHTML属性添加
	10.2.2、通过document的createElement方法创建并通过appenChild添加到指定元素下
	方法说明：
		1、createElement：创建元素，参数说明：1、元素名称
		2、appenChild：添加子元素，参数说明：1、要添加的子元素的对象

10.3DOM设置HTML属性	
	1、通过对象的style类设置
	2、通过对象的setAttribute设置
	setAttribute方法说明
	参数：
	1、属性名称
	2、属性的值
	
	
11、JS之BOM
浏览器对象，操作浏览器
Window、History 、Location 
11.1Window窗口
	1、open方法，创建或打开新的窗口
	open(URL,name,features,replace)：
	参数说明：
	1、要打开的url
	2、名称，标记窗口
	3、窗口特征属性，常用的值：width\height\left\top等
	4、替换，boolean类型，true:在当前窗口打开，false新建一个窗口
11.2History浏览器的历史记录
	常用属性：length:历史记录的数量
	常用方法：
	back() 加载 history 列表中的前一个 URL。 
	forward() 加载 history 列表中的下一个 URL。 
	go() 加载 history 列表中的某个具体页面。 需要参数：要打开的历史记录的网页
	
11.3Location网址信息
	常用方法：
		1、assign() 加载新的文档。 
		2、reload() 重新加载当前文档。
		3、replace() 用新的文档替换当前文档。 
注意：
assign和replace的区别？
assign只是在当前窗口中打开新的网址，所以可以后退
replace将当前显示的网页的内容替换为新的内容
11.4Navigator获取浏览器信息
常用属性：
	appName 返回浏览器的名称。 
	appVersion 返回浏览器的平台和版本信息。 


JS重点：
1、函数
2、事件
3、对象
4、DOM

 

 


JS综合：
1、验证码
2、注册页面校验
3、浮动窗
4、二级联动
5、贪吃蛇
6、点名


H5的面板
canvans绘制流程：
1、获取对应的<canvans>对象
var cvn=document.getElementById("id名称");
2、获取Context对象
var cn=cvn.getContext("2d");
3、设置样式
cn.fileStyle="颜色";
4、绘制
cn.fillRect(x,y,width,height):绘制矩形
参数说明：
	4.1x:左上角的x坐标
	4.2y:左上角的y坐标
	4.3width:宽度
	4.4height:高度
moveTo:移动
lineTo：绘制线
arc：绘制圆

Web前端：
HTML:描述网页
特点：
固定标签
不区分大小写
属性的值必须双引号

网页:
<html>
<head></head>
<body></body>
</html>

重点记忆的标签：
form表单
div
a\img\textarea\hr\ol\ul\table

H5:
video
audio
canvas
离线数据库(SQLITE)
CSS：样式表
美化网页的
1、引用方式：内嵌、内部、外部
2、格式：选择器{属性名称:值}
3、选择器：标签、类(.)、id(#)、派生、属性
4、常用的属性：背景、字体、文字、盒子(边框、外边距、内边距)、列表、浮动、定位
JS:浏览器脚本语言
动态交互HTML和CSS
1、基本语法
2、内置对象：Array\String\Math\Date\RegExp
3、函数：function 函数名称([参数]){ [return 返回值]}
4、属性事件：onclick\onchange\onsubmit\onmouseover\onmouseout
5、JS之DOM:getElementById/getElementsByName/getElementsByTagName/createElement
6、JS之BOM:Window\History\Location

 

**》五MySQL**

数据存储的方式：
1、非持久性存储
就是会随着程序的关闭而消失的存储
存储在内存中
数组和集合
2、持久性存储
就是存储在硬盘上
IO流和数据库


一、数据库：存储数据的仓库
好处：
1、效率高
2、容差率低
3、稳定性
4、备份和恢复

二、数据库涉及的概念：
数据库：DataBase，db文件，存储在磁盘上，按照一定的规则进行数据的处理，可以让用户共享
数据管理系统：DataBase Management System,DBMS就是操作数据库的软件

市场主流数据流：SQL Server、Oracle、MySQL、DB2等，移动端数据库：SQLITE

三、数据库的安装与卸载
MySQL的安装：
按照图解，无脑下一步
MYSQL的卸载：
1、打开服务，停止mysql的服务
2、打开控制面板->程序->mysql卸载
3、在C盘和安装盘下，搜索mysql,将搜索到的删除
4、打开注册表，搜索mysql，将找到的全部删除

四、打开数据的方式
打开：
1、开始-所有程序-mysql
直接输入密码
2、在系统变量path上添加mysql的安装目录(C:\Program Files\MySQL\MySQL Server 5.5\bin)
先打开cmd
再输入：mysql [-h主机地址] -u 用户名 -p 
会提示输入密码：

五、MySQL数据库的组成
5.1数据库服务器
就是数据库安装所在的机器，可以供别人访问
5.2数据库
一般来说一个应用程序对应一个数据库
切记：进入数据库软件之后，要选择数据库或创建数据库
5.3表---对应的是类
一个数据库会对应多个表，表是存储数据
表的组成：
1、表名
同一个数据库的表名不能重复
2、字段：域、属性、表头、列名
规定该列表示的意思
同一表中列名不可重复
3、数据
表中记录的一条条数据

my.ini：MySQL的配置信息
六、SQL
SQL:Structure Query Language:结构化查询语言
专门用在操作关系型数据库的语言
任何关系型数据库都支持SQL,但是每门数据库都有细微的差别

SQL语句分类：
DDL(*)（Data Definition Language）：数据定义语言，用来定义数据库对象：库、表、列等；
 DML(**)（Data Manipulation Language）：数据操作语言，用来定义数据库记录（数据）；增删改
 DCL（Data Control Language）：数据控制语言，用来定义访问权限和安全级别；
 DQL(*****)（Data Query Language）：数据查询语言，用来查询记录（数据）。查询（select）
 注意：SQL语句必须以;结尾

七、SQL语句
sql语句不区分大小写 
7.1DDL数据定义语句
包含的关键字：
7.1 CREATE：新建
可以新建数据库、表
7.1.1创建数据库格式：
	CREATE database [ if NOT EXISTS] 名称 [CHARACTER SET 编码格式] [COLLATE 比较方式];

创建一个编码格式为utf8的数据库
create database db_study01 ;//默认编码格式
create database db_study02 character set utf8 collate utf8_general_ci;//创建数据库编码格式utf8比较方式对应的

7.1.2创建表的格式：
CREATE table [ if NOT EXISTS] 名称 (字段名称 数据类型,……) [CHARACTER SET 编码格式] [COLLATE 比较方式];

创建一张user表
create table user (id int,name varchar(20),age int);


7.2ALTER:修改
可以修改数据库的编码和表的字段
7.2.1修改数据库格式：ALTER database 名称 [character set 编码][cllate 比较格式];

修改数据库mydb2的编码格式为gbk
alter database mydb2 character set gbk; 
7.2.2修改数据库表的格式：
ALTER table 名称 [ADD|CHANGE|DROP|MODIFY] 字段名称 [新名称] [数据类型]

为user表新增一个字段password 字符串32
alter table user add password varchar(32);
alter table user add test varchar(32);
为user表中的name字段重命名为username
alter table user change name username varchar(20);
删除user表中的test字段
alter table user drop test;
修改user表中username字段的长度为30
alter table user modify username varchar(30);


修改user的表名为user1
rename table user to user1;

7.3DROP删除
可以删除数据库或表
7.3.1删除数据库格式：
	DROP database [ if EXISTS] 名称;
7.3.2删除数据表格式：
	DROP table 名称;

删除数据库mydb4
drop database if exists mydb4;
删除test3表
drop table if exists test3;


DML语句：主要是对表中的数据进行新增、修改、删除操作
7.4INSERT新增语句
格式：INSERT INTO 表名[(字段名称)] values(值1,值2);

为user表新增数据
insert into user (id,username,password) values(2,'贾金山','8888');
insert into user (id,username,password,flag) values(5,'战旗','838',1);
insert into user (id,username,password,flag) values(6,'战旗','838',1);
insert into user (id,username,password,flag) values(7,'战旗','838',1);
insert into user (id,username,password,flag) values(8,'战旗','838',1);
//简写形式
insert into user values(4,'曹怡',"123456");
insert into user(username,id,password) values('王星',5,"123");
7.5UPDATE修改语句
格式：UPDATE 表名 SET 字段名称=值[多个字段] [where 条件]

修改id为2的用户名字为金星
update user set username='金星' where id=2;
修改id为3的用户的名字和密码
update user set username='网警',password="654321" where id=3;
修改user中所有的密码为123456
update user set password='123456';

 

7.6DELETE删除语句
删除表中的数据
格式：DELETE FROM 表名 [where 条件];

删除user中名称为王星的人
delete from user where username='王星';

开发中的删除：
1、真删除
直接使用delete语句删除指定的数据
2、假删除
使用update 修该指定字段

为user表新增一字段，flag标记是否有效，0有效1删除
alter table user add flag int;
update user set flag=0;
select * from user where flag=0;
update user set flag=1 where id=4;

DQL语句：数据查询语句，关键字：select
7.7SELECT查询语句
主要用来查询数据
格式：SELECT *|字段名称|函数 from 表名 [where 条件] [order by 字段名称] [group by 字段名称][having 条件][limit 限制]

7.7.1*
*在查询语句中表示当前表的所有字段(顺序为建表时字段顺序)
7.7.2where条件：
常用的条件符合：
	1、=、<、>、!=、<>、<=、>=
	2、and:并且，同时满足
	3、or:或者，满足其一
	4、in (值,……):对应的字段的值再()里，一般不连续的多个值
	5、between A and B:在A和B之间,[A,B]
	6、IS NULL:为空
	7、NOT:不
	8、子查询

查询user表中所有信息
select * from user;
等价于
select id,username,password,flag from user;
查询有效的名字
select username from user where flag=0;
查询id大于1小于6的人
select * from user where id<6 and id>1;
等价
select * from user where id between 2 and 5;
查询id为4或者id为8
select * from user where id=4 or id=8;
查询id不为6的人
select * from user where id!=6;
等价
select * from user where id<>6;
查询id为2或5或6的人
select * from user where id=2 or id=5 or id=6;
等价
select * from user where id in (2,5,6);

八、数据库的备份和恢复
8.1数据的备份
在cmd中，输入：mysqldump -u 用户名 -p 要备份的数据库 >路径/文件名称.sql
8.2数据的恢复
需要进入到Mysql
创建数据库
并使用数据库
source 要恢复的sql文件的路径;

 

 

查看当前的所有数据库：
show databases;
查看指定数据库的创建语句
show create database 数据库名称;
查看数据库的编码格式
show variables like 'character%';
查看mySQL的支持的编码 
show character set;
查看对应编码的比较集
 show collation;
查看数据库的所有表名
show tables;
查看数据库中建表语句
show create table 表名; 

MySQL数据类型：
INT 数字类型(4个字节)
SMALLINT 数字(2个字节)
BIGINT 数字(4个字节)
DATE 日期
TIME 时间
DATETIME 日期时间
VARCHAR(长度) 字符串
TEXT 文本
BLOB 二进制
FLOAT 浮点型
DOUBLE 双精度

 


数据库基本操作：
1、创建数据库(create)
2、选择数据(use)
3、创建表(create)
4、操作表数据(insert\delete\update\select)

 

**》六**

 

where 条件：
=\<\>\<>\!=\>=
or:或者
and：并且
between A and B：在A和B之间
in (值)：在……内部
is null：为null
not:不
select的标准格式：select *|字段列表|聚合函数 from 表名 [where 条件] [group by 字段] [having 字段][order by 字段 ASC|DESC][limit 起始索引,数量]]

一、模糊查询
like:模糊查询的关键字
_:1个字符
%:任意个字符

1.1以XX开头
格式：select *|字段 from 表名 where 字段 like 'XX%';

查询商品表中名字以华为开头的
select * from goods where name like '华为%';
查询商品表中名字以华为开头的且后面就一个字符的商品
select * from goods where name like '华为_' ;

1.2以XX结尾
格式：select *|字段 from 表名 where 字段 like '%XX';

查询商品表中价格以9结尾的
select * from goods where price like '%9';
查询商品表中价格为三位数且以9结尾
select * from goods where price like '__9';
等价于
select * from goods where price like '%9' and price >99 and price <1000;

1.3包含XX
格式：select *|字段 from 表名 where 字段 like '%XX%';

查询商品表中名字包含o的商品
select * from goods where name like '%o%';


创建一个商品表：
create table goods(id int,name varchar(50),address varchar(50),price int);
新增数据
insert into goods(id,name,address,price) values(1,'华为手机P9',"河南",3999);
insert into goods(id,name,address,price) values(2,'华为手机P10',"河南",2999);
insert into goods(id,name,address,price) values(3,'苹果7',"天津",999);
insert into goods(id,name,address,price) values(4,'苹果7s',"北京",30999);
insert into goods(id,name,address,price) values(5,'小米6',"河南",9999);
insert into goods(id,name,address,price) values(6,'三星note7',"河南",99);
insert into goods(id,name,address,price) values(7,'三星note8',"首尔",999);
insert into goods(id,name,address,price) values(8,'中兴G7',"山东",888);
insert into goods(id,name,address,price) values(9,'oppoR9',"河北",7777);


二、字段控制查询

2.1查询结果去重
distinct:对查询结果去重

//查询goods表中所有数据并去除重复的
select DISTINCT * from goods;

2.2对null值转换
ifnull(字段名称 , 待转换的值)：标记某个字段为null的话转换为指定的数据参与运算
查询goods表中id和price的和
select id+price from goods;
null值和任何类型的数据运算结果都是null

select id+ifnull(price,10000) from goods;

2.3别名
可以为运算的结果起别名： as 别名
也可以为表起别名，as可以省略

查询goods表中id和价格的和，要求价格为null则作为10000处理
select id+ifnull(price,10000) as total from goods;
等价于
select id+ifnull(price,10000) total from goods;

查询goods表名价格小于100的商品
select * from goods where price <100;
等价于
select * from goods where goods.price <100;
等价于
select * from goods g where g.price <100;

 

三、排序
order by 字段 排序方式,……
排序方式：ASC:升序(默认排序)，DESC:降序

查询goods表中按照价格升序排列
select * from goods order by price;
等价于
select * from goods order by price ASC;

查询goods表中按照价格降序排列
select * from goods order by price DESC;

查询goods表中按照价格降序排列,如果价格相同按照id进行升序排列
select * from goods order by price DESC, id asc;

四、聚合函数
COUNT:计数，计算指定列不为null的数量
MAX:计算指定列的最大值
MIN:计算指定列的最小值
SUM:计算指定列的总和
AVG:计算指定列的平均值

 

查询goods表中的数据的数量
//查询goods表中price不为null的数据行
select count(price) from goods;
//查询goods表中id不为null的数据行
select count(id) from goods;
//查询goods表中的数据行
select count(1) from goods;

查询goods表中所有price的总和
select sum(price) from goods;
查询goods表中价格最高和和最低的商品
select MAX(price) maxp,min(price) minp from goods;
查询goods表中的评价价格
select avg(price) from goods;


五、分组
group by 字段名称 [having 条件]


create table student (no int ,name varchar(5),sex varchar(2),age int);
insert into student values(17000501,'桑晓东','男',18);
insert into student values(17000502,'张文秀','男',18);
insert into student values(17000503,'曹怡','女',16);
insert into student values(17000504,'杨海婷','女',12);
insert into student values(17000505,'贾金山','女',16);
insert into student values(17000506,'卢林','男',28);
insert into student values(17000506,'卢林','男',28);

查询学生表中男女的数量
select sex,count(no) from student group by sex;
查询学生表中男女的平均年龄
select sex,avg(age) from student group by sex;
查询每个年龄的学生的数量
select age,count(no) from student group by age;
查询学生表中每个年龄的人数并且，人数等于的1

select age,count(no) ct from student group by age having ct=1;

where和having的区别
where:用在表名后，进行条件的筛选，主要用在分组前
having：用在group by之后，主要对分组的数据进行筛选


六、limit分页
格式：limit 行索引,数量：从指定的行索引开始查询，查询指定的数量

查询学生表中前三条数据
select * from student limit 0,3;
查询学生表中第三条到底五条的学生
select * from student order by no limit 2,3;

分页：每页显示2条数据，请查询第三页的数据
select * from student order by no limit 4,2;
//分页算法
int count=2;int page=3;
select * from student order by no limit (page-1)*count,count;

查询的顺序：from-->where--->group by --->having--->order by --->limit

七、数据完整性
约束条件：
主键、唯一约束、自增、不为null、默认值、外键等

7.1主键：唯一标记，不可为null，不能重复
任何表必须有主键

添加主键的方式：
	1、create table (字段 primary key)
	2、create table (字段,primary key(字段名称,字段名称))
	3、alter table 表名 add primary key(字段名称);

//delete from student where no =(select no from student  limit 5,1);

delete from student where no=17000506;
//添加主键为no
alter table student add primary key(no);

create table grade(id int,name varchar(20),primary key(id));

7.2unique:唯一约束 ，不能重复，可以为null(多个)
create table test1(id int unique );

7.3auto_increment:自动增长，每次递增1,一般主键使用
create table test2(id int primary key auto_increment,msg varchar(20) unique);

7.4not null:不为null

create table test3(id int not null);

7.5default:默认值
create table test4(id int primary key auto_increment,psw varchar(6) not null default '123456');
insert into test4(psw) values(default);
insert into test4(psw) values(null);

7.6FOREIGN KEY ：外键
外键：一个表中的某个字段的值必须为另张表中的主键的值，这时我们可以使用外键约束
作用：当新增数据时，如果当前的外键的值在另张表的主键的值不存在，就报错
CONSTRAINT 外键名称(外键表中的名称) foreign key(当前表中的外键) references 另张表名称(主键名称)
外键的添加方式:
	1、create table 表名(字段,外键,CONSTRAINT 外键名称(外键表中的名称) foreign key(当前表中的外键) references 另张表名称(主键名称))；
	2、ALTER TABLE 表名ADD CONSTRAINT 外键名称FOREIGN KEY(当前表中的外键) REFERENCES  另张表名称(主键名称))； 

create table student (sid int primary key auto_increment,name varchar(20),
fk_cid int,
CONSTRAINT fk_student_cid foreign key(fk_cid) references class (cid));
insert into student(name,fk_cid) values('小贾',1);


八、多表关系：
8.1一对一
	2表数据一一对应
8.2一对多
	2表数据，存在一对多
8.3多对多
	2表数据，存在多对多的关系

九、三大范式
阐述如何创建表并定义之间联系
9.1第一大范式：
确定列的原子性
就是每列的内容各不相同，要划分为最小单元
9.2第二大范式
确保有主键
主要用于区分每条数据，并定位
9.3第三大范式
表与表之间通过主键关联
主键唯一

功能单一原则：表描述的或存储的数据单一
关联通过主键

十、多表查询
主要用来查询多张表
1、合并结果集
UNION ：合并2个查询的结果，要求2个查询的列的个数和列的数据类型必须相同，会消除重复的数据
UNION ALL:合并2个查询的结果，要求2个查询的列的个数和列的数据类型必须相同，不会消除重复的数据

2、内部连接查询
inner join on：内部连接
格式：select * from 表1 inner join 表2 on 表1字段=表2字段
查询2个表的数据，内部连接的形式
select * from mytest1 inner join mytest2 on mytest1.id=mytest2.id2;

3、外部连接查询
左外联：left outer join on
格式：select * from 表1 left join 表2 on 表1字段=表2字段
左联就是表1为主表，表一数据都有，表2有就显示，没有就是显示为null
右外联：right outer join on
格式：select * from 表1 left join 表2 on 表1字段=表2字段
右联就是表2为主表，表二数据都有，表1有就显示，没有就显示为null


左联查询2表
select * from mytest1 t1 left join mytest2 t2 on t1.id=t2.id2;
右联查询2表
select * from mytest1 t1 right join mytest2 t2 on t1.id=t2.id2;
等价于
select * from mytest2 t2 left join mytest1 t1 on t1.id=t2.id2;

4、连接查询
查询2个表中所有数据的组合结果
select * from mytest1,mytest2;
产生的结果集：笛卡尔积
结果的数量：表1的数量*表2的数量


十一、内置函数
abs:取绝对值
bin:十进制转换二进制
ceiling:向上取整
floor:向下取整
rand:随机数：0.0-1.0(不包含)
now:现在日期+时间
current_date:现在的日期
current_time:现在时间
instr(字符串,要查找的内容):查询对应的索引
ucase:转换为大写
lcase:转换为小写
replace(字符串,原来的值,新的值):替换
substring(字符串,开始索引,长度):截取

十二、杂谈
1、索引：
提高查询的效率
通过索引快速进行查询，速度会提高
2、视图：
为比较复杂的结果集创建一个虚拟的表
格式：create view 名称 as select * from 表名；
3、触发器：
一张表操作时，会对另外的表页进行操作
格式：
delimiter $修改当前的结束符为$(默认的结束;)
create trigger 触发器名称 before|after insert|delete|update on 表名主 
for each row  
begin
要执行的sql语句
例如：
insert into 表名被(字段名称) values(new.表名主字段);
end $


有2张表，user和mylog
现在要求：新增用户时，默认会记录一条日志

delimiter $
create trigger t_test after insert on user for each row
begin
insert into myLog(msg,time) values(new.name,current_date());
end $

4、快速备份新表
create table 新表名称 select * from 旧表

其他数据库：
select * into 新表名 from 旧表;但是MySQL不支持

 

MySQL:
数据库重点内容：
一、七大SQL语句
1、CREATE
2、ALTER
3、DROP
4、INSERT
5、UPDATE
6、DELETE
7、SELECT
where\group by \having\order by \limit
二、约束条件
主键、唯一、非null、default、自增、外键
三、多表查询：
内联、外联、合并
四、聚合函数
AVG\MAX\MIN\SUM\COUNT

根据个人情况进行记忆：
1、内置函数
now\rand\floor\substring\ifnull
2、索引
索引提高操作数据库的效率
3、触发器
为某张表加自动操作

 

**》七**

 

一、JDBC:Java Data Base Connectivity java语言操作数据库的API
是由类和接口组成的，然后交给数据库厂商来实现的，所以想操作数据库，
需要下载对应数据库的驱动jar

JDBC涉及的类和接口：
1、DriverManager:驱动管理器
2、Connection:连接接口
3、Statement:操作SQL接口
4、ResultSet:结果集
5、PreparedStatement：操作SQL类，防止SQL注入，推荐使用

二、使用JDBC的步骤：
1、导入jar
2、动态加载驱动(注册驱动)
Class.forName("com.maysql.jdbc");
3、获取连接对象
Connection conn=DriverManager.getConnection("JDBC:mysql://主机IP:3306/数据库名称",用户名,密码);
4、获取操作SQL对象
Statement stmt=conn.createStatement();
5、执行SQL
stmt.execute(SQL);//执行任何SQL语句。返回boolean类型，是否为select语句
stmt.executeUpdate(SQL);//执行非select语句,返回受影响的行数
stmt.executeQuery(SQL);//执行select语句，返回查询的结果集ResultSet
6、获取结果集
ResultSet rs=stmt.executeQuery(sql);
7、关闭
rs.close();
stmt.close();
conn.close();

三、JDBC类或接口的说明：
3.1DriverManager:驱动管理类
	管理数据库驱动
	常用方法：
		static Connection getConnection(String url) 
          建立到给定数据库 URL 的连接。 
		static Connection getConnection(String url, Properties info) 
          建立到给定数据库 URL 的连接。 
		static Connection getConnection(String url, String user, String password) 
          建立到给定数据库 URL 的连接。 
          
          
3.2Connection:数据库连接接口
	附加连接到指定的数据库
	常用方法：
		createStatement：获取Statement对象

3.3Satement:操作SQL接口
	将指定的sql语句发送到数据库，并获取结果
	常用方法：
		1、boolean execute(sql)
			执行任何SQL语句，返回值true:select语句，这时可以通过getResultSet(),false:非select语句
		2、int executeUpdate(sql)
			执行非select语句，返回操作的行数，如果为DDL语句返回0
		3、ResultSet executeQuery(sql)
			执行select语句，返回结果集
		4、ResultSet getResultSet()
			获取查询的结果，配合execute使用

3.4ResultSet:结果集
	记录查询的结果数据
	常用方法：
		1、boolean next:获取下一行数据 ,返回值：true:下一行有数据，false：没数据
		2、first:返回到第一行
		3、last:返回最后一行
		4、getInt:获取整型
		5、getLong:获取长整型
		6、getFloat:获取浮点型
		7、getDouble:获取双精度浮点型
		8、getString:获取字符串
		9、getBytes:获取二进制
		10、getDate:获取日期
		11、getTime:获取时间
		
注意：getXXX方法为获取指定列索引或列名称的值
切记列索引从1开始
3.5PreparedStatement：防止SQL注入的操作SQL对象
	防止SQL注入，效率高
	常用方法：
	setXXX(index,value):设置对应sql语句的第index个参数对应的值为value
	executeQuery:执行select语句
	接收的SQL语句：
	sql语句 where 字段=? 
	


四、封装数据库操作
1、在src下创建xxx.properties
	将需要用到的变量存储
	键值对形式
2、创建工具类
	实现对数据库的操作：增删改查
3、使用

五、SQL注入
就是sql语句根据输入进来的数据进行拼接，通过sql关键字造成不正确的结果
例如：' or 1=1 or '
如何防止这种行为出现呢，就是使用PreparedStatement代替Statement
PreparedStatement的好处：
1、防止SQL注入
2、优化查询
3、减少查询次数，Statement会频繁的查询数据库，可能会造成缓冲区的溢出
	
	
	

额外：
1、JAVASE加载第三方jar:
	1、创建文件夹lib
	2、拷贝jar到lib文件夹
	3、build path将对应的jar加载到应用程序
2、HashTable和HashMap的区别?
	1、父类不一样
	HashMap基于Map<K,V>
	HashTable基于陈旧的Dictionary<K,V>
	2、contains方法不一样
	HashMap:containsKey,containsValue
	HashTable:contains---等价于containsValue
	3、线程安全
	HashMap：不安全，不能用在多线程并发中，效率高
	HashTable:安全，效率低


3、Properties：持久性属性集类
	主要用来存储和读取配置文件，以.properties结尾
	
	自学Properties类的使用

4、单元测试
@Test:标记这是一个测试方法
测试方法要求：
	1、public修饰
	2、无返回值
	3、无参数

5、工具类设计原则
方便别人，麻烦自己
内部方法一般都是静态方法
考虑尽量全面

6、可变参数
格式：数据类型... 参数名称、
参数可以有0个或多个
出现在方法的声明中，只能在参数末尾
在方法内部，当作数组使用

 

@Override：标记这个方法为重写

 

**》八**

 

温故：
	Web项目使用JDBC
	Web项目导入jar包的方式：直接将jar复制到WebContent\WEB-INF\lib
	
	通过jdbc创建以下表：
	b_account:账户表
	id 账号 16位，字符串
	name 姓名 50位
	money 余额 
	createtime 开户时间
	updatetime 最后一次更新时间
	
	b_log:流水日志表
	id 序号
	a_id 账号
	money 钱 存钱,取钱-
	msg 备注信息
	time 交易时间
	type 类型 1:开户 2：存钱 3：取钱 4：测试
	注意：数据库设计的时候，最好写出一份设计文档，标记表的意义

一、Transaction：事务
1.1事务是什么？
事务就是保证多条数据操作要么都成功，要么都失败，确保数据的完整性
其实就是将一组相同逻辑的操作放在一起进行操作，全部都成功或都失败
主要用在：多条数据的操作的一致性

1.2事务如何使用？
（***重要，记住）数据库指令：
	1、start transaction:开启事务
	2、rollback:回滚，取消本次操作
	3、commit:提交，确定本次操作
JAVA代码中使用：JDBC
	1、通过connection.setAutoCommit(false)设置开启事务，true不开启，false开启事务
	2、在try语句块的结尾：connection.commit:提交
	3、在catch中connection.rollback:回滚事务
练习：
1、请自己创建任意表，通过JDBC实现事务的操作	

 

​	
1.3事务的特性(ACID)
​	原子性（Atomicity）  原子性是指事务是一个不可分割的工作单位，事务中的操作要么都发生，要么都不发生。（例：全部发生或全部不发生） 
​	一致性（Consistency）事务必须使数据库从一个一致性状态变换到另外一个一致性状态。（例：从一起生到一起死）
​	隔离性（Isolation）  事务的隔离性是多个用户并发访问数据库时，数据库为每一个用户开启的事务，
​                               不能被其他事务的操作数据所干扰，多个并发事务之间要相互隔离。（例：犯错误被关进不同的小黑屋）
​	持久性（Durability） 持久性是指一个事务一旦被提交，它对数据库中数据的改变就是永久性的，
​                               接下来即使数据库发生故障也不应该对其有任何影响。（例：淘宝买东西确认付款，一旦确认付款就无法退钱）
​                            （助记：  一位科学家偷了国家的“原子弹”（例：全部发生或全部不发生），领导开会“一致”（例：从一起生到一起死）同意将他逮捕，警察抓到他把他进行“隔离”（例：犯错误被关进不同的小黑屋），法院判处他“持久性”（例：淘宝买东西确认付款，一旦确认付款就无法退钱）蹲监）
​              
1.4事务的隔离性
隔离性就是设置多个线程并发访问数据时的一种权限
如果没有隔离性：可能会引起：脏读、不可重复读、虚读

SQL事务的隔离级别：
	1、Serializable：可避免脏读、不可重复读、虚读情况的发生。（串行化）
	2、Repeatable read：可避免脏读、不可重复读情况的发生。MySQL默认
	3、Read committed：可避免脏读情况发生。SQL Server |Oracle
	4、Read uncommitted：最低级别，以上情况均无法保证。(读未提交)
	设置事务的隔离性：
	set transaction isolation level 设置事务隔离级别
	查询当前数据库的事务的隔离性：
	select @@tx_isolation	查询当前事务隔离级别

串行化：按照一个个的事务的先后顺序执行

 

类和对象
类方法和实例方法
全局变量和局部变量

变量的生命周期
1、类变量--静态属性：类第一次加载就声明，类销毁的时候跟着销毁
2、成员变量--实例属性：对象创建的时候，对象被销毁的时候
3、局部变量--定义在代码块内部的就是局部变量，作用域：当前代码块
java中括号：
{}:代码块，类的内容、方法、静态代码块、构造等
[]:数组
():方法
<>:泛型

方法的定义和调用
接口和类的区别

 

**》九**

 

一、连接池
1.1连接池是什么？
连接池就是管理数据库连接对象
保证连接对象的数量，进而提升操作数据库的效率

主要是并发访问时，会产生n个连接对象，
这些都会对数据库服务器产生很大的压力，
所以需要合理的使用数据库连接池，以减轻这种压力

1.2连接池的工作原理
集合存储连接对象，根据需要进行连接对象的创建、获取、销毁

1.3DBCP连接池
	常用类：
		BasicDataSourceFactory.createDataSource(Properties):通过配置文件获取连接池对象
使用时，需要导入jar和对应的配置文件

1.4C3P0连接池
   常用类：
   ComboPooledDataSource:这是DataSource的子类，直接创建该类对象获取数据库连接池，加载默认配置文件
   
使用时，需要导入jar和对应的配置文件

二、JDBC工具类Dbutils
2.1QueryRunner:执行数据库的增删改查
	常用方法：
		1、update:执行非select语句
		2、batch:批处理，同时处理多条相同的语句
		3、query:执行SELECT语句
		
		query方法都需要ResultSetHandler<T>接口对象
		该接口就一个方法：
		handle：转换，参数：查询结果集，返回值类型：泛型
		
		一般常用默认的实现类：
		1、ArrayHandler:将查询结果的第一行数据，的每个字段存储到数组中并返回
		2、ArrayListHandler:把结果集中的每一行数据都转成一个数组，再存放到List中。
		3、BeanHandler：将结果集中的第一行数据封装到一个对应的JavaBean实例中
		4、BeanListHandler：将结果集中的每一行数据都封装到一个对应的JavaBean实例中，存放到List里
		5、ColumnListHandler：将结果集中某一列的数据存放到List中。
		6、KeyedHandler(name)：将结果集中的每一行数据都封装到一个Map<列名,列值>里，再把这些map再存到一个map里，其key为指定的key
		
		
三、特殊的数据类型的对应：
		PreparedStatement ps;
		1.DATA,TIME,TIMESTAMP date,time,datetime
			存：ps.setDate(i,d); ps.setTime(i,t); ps.setTimestamp(i, ts);
  	  	取：rs.getDate(i); rs.getTime(i); rs.getTimestamp(i);
	2.CLOB  text
	  	存：ps.setCharacterStream(index, reader, length);
	         	         ps.setString(i, s);
	  	取：reader = rs. getCharacterStream(i);
	        	        reader = rs.getClob(i).getCharacterStream();
	                          string = rs.getString(i);
	3.BLOB  blob
	 	存：ps.setBinaryStream(i, inputStream, length);
       			取：rs.getBinaryStream(i);
	        	    rs.getBlob(i).getBinaryStream(); 
Project项目，一般都会框架
三层架构：
1、显示层---H5或JSP
2、业务逻辑层---Servlet
3、数据层----实体类和数据库

外到内：
1、显示层：前端、Swing、main
2、逻辑层：service：处理业务逻辑
3、数据层：
	1、entity/model：实体映射类
	2、dao:操作数据库


Bean类：
1、私有属性
2、公有get和set方法
3、无参构造函数


三、JDBC&数据库

获取刚刚自增的id值：
select last_insert_id()

四、Properties:持久性属性集类
主要用来存储或加载配置信息

常用方法：
	1、load:加载指定的配置文件
	2、store:存储配置信息
	3、setProperties:设置要存储的属性
	4、getProperties:获取配置的属性的值


1、前端
2、MySQL
3、Servlet
4、Ajax+JQuery

 

**》十**

 

一、Web
程序的分类：
1、C/S:Client/Server:客户端服务器
请求流程：客户端程序--->服务器--->数据库服务器
2、B/S:Browser/Server:浏览器服务器
请求流程：浏览器---服务器----数据库服务器

常用服务器：
1、Tomcat：免费，开源APACHE公司
2、WebLogic：BEA公司支持JAVAEE标准规范
3、WebSphere：IBM公司支持JAVAEE标准规范

 

二、Tomcat
目录：都是英文目录
1.1TomCat:服务器
APACHE:一家强大公司
基本上提供了web开发的一整套工具
TomCat服务器就是Apache下的一个服务器
	特点：
	1、开源
	2、免费
	3、兼容性

1.2TomCat目录：
	bin:可执行
	webApps:发布项目
	work:运行的工作目录
	docs:文档
	
1.3运行TomCat:
	1、在bin目录下找到startup.bat启动服务器
	2、在浏览器输入：localhost:8080,能打开就是启动成功

1.4在Eclipse中配置Tomcat
	window-->preferences-->server-->runtime Environments->add->选择TomCat安装目录->完成

1.5发布项目到TomCat中
	1、选择项目，右键export
	2、选择war,选择导出的路径和名称
	3、将war文件放到TomCat的WebApps下
	4、启动Tomcat
	5、访问：localhost:8080/war文件名/

二、Servlet
2.1Servlet是什么？
Servlet就是sun提供的运行在服务器上的API
每一个Servlet都是独立的应用
可以实现：获取Web浏览器或客户端发送来的信息并且响应信息

2.2Servlet的使用
创建方式：
	1、实现Servlet接口
		需要实现方法：
			1、init:初始化
			2、service：服务
			3、destroy：销毁
			4、servletConfig：获取默认配置
			5、servletInfo：获取Servlet信息
			
	2、继承GenericServlet类
		需要实现的方法：
			1、service:服务
	3、继承HttpServlet类---直接new Servlet
		需要实现的方法：
			1、doGet:处理get请求
			2、doPost:处理post请求

Servlet如果想让外界使用，必须注册(配置注册还是注解注册)


2.3Servlet生命周期
生命周期：
	1、init:初始化
	初始化一些默认信息
	2、service：服务
	获取请求和响应
	3、destroy:销毁
	销毁一些信息
Servlet的生命主要是为了服务器优化


Thread生命周期：
			1、新建
			2、就绪
			3、运行
			4、阻塞
			5、死亡

 

2.3Servlet的注册：
Servlet可以设置外界访问的名称
	1、xml中注册(Servlet3.0之前)
	在项目的web.xml中使用固定标签：
	//标记这是一个Servlet
	<servlet>
		<servlet-name>外界访问的名字</servlet-name>
		<servlet-class>Servlet实现类名称</servlet-class>
	</servlet>
	<servlet-mapping>
		<servlet-name>外界访问的名字</servlet-name>
		<url-pattern>/匹配</url-pattern>
	</servlet-mapping>
	2、注解进行注册(Servlet3.0(包含)之后)
	@WebServlet("/外界访问名字")

<!--配置Servlet,一般是在Servlet3.0之前需求配置  -->
  <servlet>
      配置外界访问的名称 
      <servlet-name>MyS2</servlet-name>
      配置Servlet的类名 
      <servlet-class>MyServlet</servlet-class>
  </servlet>
  <servlet-mapping>
       配置外界访问的名称 
      <servlet-name>MyS2</servlet-name>
      <url-pattern>/*.do</url-pattern>
  </servlet-mapping>

 

2.4Servlet的线程安全
Servlet引擎采用多线程进行对用户的处理
并发时，存在线程安全。
尽量使用局部变量，非得是全局时，注意加锁

SingleThreadModel：单线程模型
对于Servlet采用单线程进行。但是会产生多个Servlet对象
在Servlet2.4之后都已经过时

2.5ServletConfig配置信息
获取Servlet的配置信息
	常用方法：
		1、getInitParameter：获取配置信息
		2、getServletContext：获取Web全局对象

ServletContext：Web应用对象
可以设置过滤器、事件、Servlet、路径等
	getRealPath:获取真实路径，发布路径

 


Eclipse和MyEclipse

 

**》十一**

 

一、Request：请求
获取客户端或浏览器的请求内容
1.1HttpServletRequest
Http协议的请求对象
服务器会为每一次的请求创建对应的request和response对象
请求对象包含的信息：
	1、请求行
	2、请求头
	3、消息正文
想要获取客户端信息，那么就使用request

1.2请求行
可以获取请求的方法、查询的内容、协议、资源等
常用方法：
	getMethod方法返回HTTP请求消息中的请求方式。
	getRequestURI方法返回请求行中的资源名部分。
	getQueryString 方法返回请求行中的参数部分。
	getServletPath方法返回Servlet的名称或Servlet所映射的路径。 
	
1.3请求的网络信息
可以获取远程或服务器或本机的网络信息
	常用方法：
	getRemoteAddr方法返回发出请求的客户机的IP地址
	getRequestURL方法返回客户端发出请求时的完整URL
	
1.4请求头信息
获取header数据
	常用方法：
	getHeader：获取指定key的值
	getHeaderNames：获取header的所有的键名称
	getContentType：获取内容的类型MIME类型
	getContentLength：获取内容的长度
	getCharacterEncoding：获取编码格式

referer:请求头属性
记录上一次的网址

1.5请求方法：
1、get:请求，获取
特点：
	1、效率高
	2、大小限制，一般为1kb
	3、不安全，参数信息随着url进行传输
	4、中文乱码，参数随着url传输
而url编码格式为ISO8859-1(不支持中文)

get乱码产生原因：
首先输入的内容会按html页面的编码格式进行
在进行传输的时候url传输，而url编码格式iso8859-1
所以内容由页面的编码格式转换为iso8859-1
get乱码的解决：
在doGet方法中
首先将内容按照iso8859-1转换为字节数组
然后再通过HTML页面的编码格式将字节数组转换为字符串
String msg=request.getParameter("name");
if(msg!=null){
	msg=new String(msg.getBytes("ISO8859-1"),"UTF-8");
}

对安全性要求不高就推荐使用Get
如果对安全性要求较高，推荐使用post

2、post:请求，发送
一般是传递数据到服务器
特点：
	1、效率低
	2、大小无限制
	3、安全性相对较高，参数以流的形式
	4、中文乱码好解决

默认请求为get请求

1.6获取表单内容
表单的内容想提交到服务器，需要为标签设置name属性，其中name的值就是键
<input>\<textarea>\<select>等都是可以提交的

隐藏域：<input type="hidden"/>用户看不见，但是可以提交数据
图片：<input type="image"/>可以提交点击的坐标，而且还能默认提交
下拉框<select>也是可以传递

表单提交时，防止用户操作太快，一般会进行提交校验，只让提交一次

1.7获取参数信息
获取表单请求的参数信息
常用方法:
	1、getParameter:获取指定key的值
	2、getParameterValues：获取指定key的多个值(多选)
	3、getParameterNames:获取所有的参数名称
	4、getParameterMap:获取所有的参数值
	

RandomAccessFile:随机读取文件类
唯一一个具有读取和写入的类

 

 

二、Response：响应
对客户端或浏览器进行数据的响应

2.1响应状态码
就是服务响应的状态码
2开头：成功 200全部成功，206部分成功
3开头：重定向 需要请求另外的网址
4开头：错误，客户端或浏览器错误
5开头：错误，服务端错误

2.2设置消息头
设置响应的消息头
常用方法：
	1、setHander:设置消息头
	2、addHander：添加消息头
	3、setContentType：设置正文类型
	4、setCharacterEncoding：设置响应的编码格式
	
2.3响应内容
向客户端或浏览器回复信息
常用方法：
	1、getWriter:打印输出字符流 print或println
	2、getOutputStream：输出字节流，非文本内容
	
	
	
2.4跳转
	常用跳转分为2种：
	1、重定向
	客户端跳转
	response.sendRedirect("跳转的网址");
	特点：
	1、可以跳转到任何一个网页
	2、地址栏改变
	3、不可携带request和response
	4、客户端进行2次请求
	
	2、转发
	服务端跳转
	request.getRequestDispatcher("当前Web的网页").forward(request, response);
	request.getRequestDispatcher("当前Web的网页").include(request, response);
	
	特点：
	1、只能跳转到当前项目的网页
	2、地址栏不变
	3、可以携带request和response
	4、客户端请求1次
	forward&include的区别？
	forward:可以对Response进行修改状态码和消息头
	include:不可以

 

**》十二**

 

数据交互格式：
1、HTML
2、JSON
3、XML
都是跨平台、跨网络、跨编程语言

一、XML
1.1Extensible Markup Language:可扩展的标记语言
是一种通用型数据交互的格式

与HTML的区别？
HTML:
不同点：
1、固定标签
2、语法不严谨：可以有结束，也可以没有
3、不区分大小写

相同点：
4、属性的值必须使用双引号
5、标签可以嵌套
XML:
不同点：
1、自定义标签
2、语法严谨：标签有开始就必须有结束
3、区分大小写

相同点：
4、属性的值必须双引号
5、标签可以嵌套

1.2XML语法

XML具有严谨的语法
语法规则：
1、自定义标签
2、区分大小写
3、标签有开始必须有结束
4、只有一个根节点
5、标签可以嵌套
6、标签命名，以字母、数字、下划线组成，以字母开头
7、属性的值必须使用双引号

1.3XML组成
xml由2部分组成
	1、前导区
		固定格式：<?xml version="1.0" encoding="编码格式" ?>
		描述此文档为xml文档，标记版本号，编码格式
	2、数据区
		只能以根标签和结尾
		根标签：唯一
		<老邢>
			<子标签></子标签>
			<子标签></子标签>
			<子标签></子标签>
			<子标签></子标签>
			<子标签></子标签>
		</老邢>
1.4XML解析
目前常用解析：
1、dom:标准，java支持
2、sax：java支持
3、dom4j：民间方式，第三方jar
4、pull：谷歌
5、xpath:对xml元素进行查询

1.4.1DOM解析XML
java支持，将整个文档读取完只会再开始解析
解析步骤：
1、创建解析工厂
2、创建解析器
3、设置解析文档并获取文档对象
4、获取根节点
5、依次获取子节点并遍历


xml中的节点和元素
Node:xml中的任何内容
主要包括：
标签(文档标签、元素标签)、文本内容
Element:元素
继承的Node
就是可以拥有子标签和属性、值的节点

元素就是对象
节点就是属性

 

 

1.4.2SAX解析
实时解析，边读取边解析
使用步骤：
1、创建DeafultHander的子类：定义解析模板
2、创建解析工厂
3、创建解析器
4、设置解析文档并开始解析
5、获取解析结果

创建自定义的DeafultHander
需要注意的方法：
1、startDocument:开始文档，无参
2、endDocument:结束文档，无参
3、startElement:开始标签
参数说明:
	1、命名空间
	2、本地名称
	3、标签名称
	4、属性集
4、endElement:结束标签
参数说明：
	1、命名空间
	2、本地名称
	3、标签名称
5、character:文本内容
参数说明：
	1、字符数组
	2、起始索引
	3、长度
1.3Dom for Java
Dom4J:解析xml的方式
使用步骤：
	1、创建解析器
	SAXReader reader=new SAXReader();
	2、设置解析文件并获取文档对象
	Document document=reader.read("students.xml");
	3、获取根元素
	Element root=document.getRootElement();
	4、依次获取所有的子元素
	elements
	5、获取元素的属性或内容
	element.attributeValue("属性名称"):获取属性的内容
	element.getStringValue()：获取元素的内容
	6、获取结果
	Entity
	List<T>
1.4XPATH
主要用来进行查询xml文档的节点信息
也可以用来解析

二、JSON
2.1JSON目前互联网使用最为广泛的一种数据交互的格式
跨平台、跨网络、跨编程语言

json语法符合：
{}:对象
[]:数组
:连接属性名称和值的
""属性名称或字符串类型的值
,多个属性或元素
JSON就是由上述的语法符合组成的字符串
JSON可以用来描述数组或对象

{"no":1,"name":"张三"}
{"stu":{"no":1,"name":"张三"}}
[{"no":1,"name":"张三"}]
[1,2,3]
{"list":[{"no":1,"name":"张三"},{"no":1,"name":"张三"}]}

2.2解析方式
1、JSON官方
JSONArray:解析数组 []
常用方法：
	length:元素个数
	getXXX(index):获取指定索引的元素对象
	
JSONObject：解析对象 {}
常用方法:
	keys:获取所有的属性名称
	getXXX:获取指定属性名称的值
	optXXX:获取指定属性名的值
	putXXX:添加指定的属性
遇到{用JSONObject，遇到[用JSONArray

2、Gson谷歌
要求JSON字符串的数据关系必须为类的映射
其实就是一个{}就是一个类
Gson:类
formJson:解析对应的json字符串
解析对象：fromjson(json,类.class)
解析数组：fromjson(json,new TypeToken<List<类>>)(){}.getType())

3、FASTJSON快速解析
要求类与json字符串相匹配
JSON:工具类
静态方法：
1、parserObject(json,类.class):解析对象
2、parserArray(json,类.class)：解析数组

注意：
Gson、fastjson
都是对类的要求：字符串中有几个{}就有几个类，({}中有几个属性类就几个属性)

 


XML会解析
JSON会解析

 

**》十三**

 

流按照功能划分：
1、节点流：直接操作数据的流
文件流、内存流
2、过滤流：操作流的流

一、导入文件内容到数据库
1、通过流获取要导入的内容
2、获取数据，并按照格式进行切割
3、添加到数据库

create table citycode(id int primary key auto_increment,code int  unique,name varchar(20),s_name varchar(20),f_name varchar(20),time  datetime);

 

二、JSP
2.1JSP:Java Server Pages
就是一个页面：可以写HTML标签还可以java代码

简化的Servlet
弥补Servlet的不足

JSP最终就会转换为Servlet
JSP页面中写的HTML标签或JSP语法最终都是在对应的饿service方法通过输出流写出

JSP语法：
<%!   %>:定义方法、创建全局变量等，这里面的代码最终在jsp对应的类中，方法外部起作用
<% %>：调用内置对象，java语句等代码，这里的代码最终在jsp对应的类的service方法中
<%= %>：显示对应变量的值

2.2JSP的组成：
	1、JSP模板元素：就是jsp页面的HTML
	2、JSP表达式：主要就是在界面显示数据,切记不可有;
	格式：<%= 变量|表达式|方法%>
	3、JSP脚本片段:只能出现java代码，严格准守java语法规则，可以调用内置对象、输出内容、创建局部变量等
	格式：<%  片段;  %>
	4、JSP声明：声明全局变量或方法，这里的代码块会出现在service方法的外部，类的内部
	格式:<%!  声明属性或方法   %>
	5、JSP注释：JSP的注释并不就会输出到页面
	格式：<%--JSP注释--%>

 

三指

九对
四域
2.3三大指令
指令：告诉JSP引擎页面的信息
格式：<%@ 指令名称 属性名称="值" %>
2.3.1page
告诉JSP引擎当前页面的信息
格式：<%@ page 属性=值 %>
常用属性：
language：语言
import:导包
contentType：正文类型，MIME类型和编码格式
session:会话 值为boolean类型，true:有，false:没有
errorPage：错误页  值：为错误页面的url
isErrorPage：是否需要异常对象
2.3.2include
加载其他html\文件、jsp等
格式：<%@ include file="路径" %>
这种加载称为静态加载

2.3.4taglib
引用标签库，常常结合着JSTL使用
格式：<%@ taglib uri="标签库地址" prefix="名称" %>
作用：在JSP页面中导入JSTL标签库。替换jsp中的java代码片段。
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 

2.4九大隐式对象
九大隐式对象就是根据配置会默认创建的对象
以下为常用内置对象：
	2.4.1request:请求对象---HttpServletRequest
	2.4.2response:响应对象--HttpServletResponse
	2.4.3session:会话---HttpSession
	2.4.4out:输出字符流对象---PrintWriter
	2.4.5pagecontext:页面域对象---PageContext
	2.4.6page:this---当前jsp产生的Servlet对象
	2.4.7application:全局域对象---ServletContext
	2.4.8execption:异常----Throwable
	2.4.9config:配置----ServletConfig

2.5域
域就是根据作用域不同而划分的对象，可以携带属性数据
常用的域：
	1、application：全局，整个Web应用，建议少用
	存储的生命周期最为悠久，只有网站停止运行才销毁
	2、session:会话，每一个用户的访问都是一个会话
	使用最为广泛，一般用来做，存储登陆信息、购物车
	3、pagecontext:页面对象，整个页面有效，用的不多
	4、request:每次请求
	主要记录请求的信息，使用最为广泛
一般使用域
setAttribute(key,value):设置属性，对应的key存在就修改，不存在就是添加 value:可以是自定义类对象
getAttribute(key):获取指定key的值	

 

**》十四**

 

一、JSP元素
也称为JSP动作
格式：<jsp:动作名称 属性名称="值"/>
常用：
	1、include
	作用：动态包含其他页面，可以传递参数
	格式：<jsp:include page="包含的文件路径" flush="是否刷新"></jsp:include>
	
	动态包含和静态包含的区别？
	动态包含：jsp元素
	1、格式：<jsp:include page>
	2、加载时机，运行到此处的时候加载，每个jsp都会产生java文件
	3、传递参数
	静态包含：jsp指令
	1、格式：<%@ include file%>
	2、刚开始就加载，只会产生当前的java文件，其他是将最后的静态页面加载到当前页面
	3、不传递参数
	
	注意：如果一个jsp页面被加载到其他页面使用，建议直接写标签或jsp语法
	
	2、forward
	作用：实现页面的跳转，其实就是请求转发
	格式：<jsp:forward page="要跳转的页面路径"></jsp:forward>
	可以携带参数
	注意：部分情况该标签双标签，必须有参数，单标签不需要
	3、param
	作用：在使用include或forward时传递出参数
	格式：<jsp:param name="key" value="value"/>

 

二、EL
EL：Expression Language：EL表达式，简化JSP表达式
格式：${域中的属性名称|param.参数名称|cookie.名称}
显示结果
开发常用EL做以下事情：
1、获取请求参数的值:${param.参数名称}
2、获取域中属性的内容：${XXXScope.属性名称}
3、获取Cookie中的内容：${cookie.名称}

${dog.name}
${dog["name"]}

三、JSTL
JSTL:Java Server Pages Standard Tag Library:JSP标准标签库
在Eclipse中需要导入jar包
myEclipse不需要

使用步骤：
1、导入jar包
2、在JSP文档使用
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
3、在文档中使用JSTL标签
直接<c:out>

常用标签：
<c:out value="输出的内容">
<c:set >
<c:remove>
<c:if>
<c:choose>
<c:forEach>


四、Cookie
会话状态，存储HTTP请求或响应过程的信息
将信息保存在客户端的磁盘

获取Cookie:request.getCookies
新增Cookie:response.addCookie

五、Session

 


六、回顾
Servlet:java运行在服务器的api
主要功能：获取浏览器或客户端的请求，回应数据

Servlet的创建方式：
	1、HttpServlet
	2、GeneralServlet
	3、Servlet
Servlet的生命周期：
	1、init:初始化
	2、service:服务
	3、destroy：销毁
Servlet的注册方式：
	1、使用注解@WebServlet
	2、使用xml配置
Servlet使用步骤：
1、创建Servlet类
2、重写对应的方法doGet、doPost、service
3、注册  注解的形式
4、运行在服务器
5、访问

HttpServletRequest:请求

HTTP协议的消息组成：
1、请求行：获取方式、协议类型、路径等
2、请求头：获取正文类型、编码格式、刷新等
3、请求内容：参数或属性

请求属性：可以存储各种数据类型
getAttribute:获取指定key的属性值
setAttribute：设置属性
removeAttribute：移除属性

请求参数：只能存储字符串
getParameter:获取指定参数的值
getParameterValues：获取指定参数的所有值。checkbox

跳转：
常用的跳转方式：
1、客户端跳转：重定向
response.sednRect("页面路径")
不可传递request
2、服务端跳转:请求转发
request.getRequestDispatcher("页面路径").forward(request,response);
request.getRequestDispatcher("页面路径").include(request,response);


中文乱码：
get如果乱码：iso8859-1---->utf-8

 

HttpServletResponse:响应

回复信息：
	1、response.getWriter:字符流
	2、response.getOutputStream:字节流
	
设置消息头：
	1、setContentType("text/html;charset=utf-8")
	2、setContentLength:设置内容的大小
设置状态响应码：
	1、setStatue()

常用的网络数据交互格式：
1、HTML
2、XML
3、JSON

JSON解析：
{}:对象
[]:数组

根据对应的json字符串创建类，从里往外创
常用解析技术：
1、JSON源生
JSONObject:
getXXX(key)

JSONArray:
getXXX(index)
length():长度

2、Gson
gson.fromJson(json,Class):解析对象
gson.fromJson(json,new TypeToken<List<T>>(){}.getType()):解析数组
3、FastJson
JSON.parserObject(json,Class)
JSON.parserArray(json,Class)


XML：可扩展的标记语言
常用解析：
	1、DOM
	区分：Node和Element
	2、SAX
	自定义DefaultHandler
	3、DOM4J
	SAXReader
	4、XPATH
	selectNodes/selectSingleNode

 

JSP:Java Server Page
就是HTML和Servlet的组合

重点：
<%!  %>:声明	
<%  %>	:操作
<%=  %>	:显示

一、JSP语法：
	1、<%! 声明方法、属性  %>：定义属性、方法
	2、<% 代码块，运行在_jspService %>：操作内置对象
	3、<%=变量|表达式  %>：显示数据

二、JSP组成：
	1、JSP模板元素：HTML标签
	2、JSP代码片段：<%   %>
	3、JSP表达式:<%= %>
	4、JSP声明:<%!  %>
	5、JSP注释:<%--  --%>
三、JSP的指令：就是JSP引擎起作用的时就加载
	1、page:页面信息
	2、include:加载其他页面
	3、taglib:使用JSTL表达式
	格式：<%@ 指令名称 属性名称="值" %>
四、JSP内置对象：
	1、request:请求对象
	2、response:响应对象
	3、session：会话
	4、application:全局
	5、pageContext:当前页面信息
	6、exception:异常
	7、out:输出
	8、page:当前对应的Servlet对象
	9、config:servlet配置信息
五、JSP域对象
各个对象的作用域不同
从大到小：
1、application:整个Web应用
2、session:会话，一个客户端的
3、request:请求
4、pageContext:当前页面
	
六、EL表达式
简化的JSP表达式
格式：${名称}	
作用：
1、获取请求参数
2、获取域的内容
3、获取cookie的内容

七、JSTL
JSP标准标签库
结合指令<%@ taglib prefix="前缀" uri="资源"%>


分类：
1、核心库
2、格式化库
3、数据库
4、函数库
5、XML库

常用：
核心库：
常用标签：
<c:out>:输出
<c:set>:设置
<c:remove>：移除
<c:if>：条件
<c:choose>：多条件
<c:forEach>：循环


Web中的状态：
1、Cookie:本地状态
是将信息存储到本地
Cookie ck=new Cookie(key,v);
ck.setMaxAge(秒)：有效期：0删除
ck.setPath("/"):当前Web

response.addCookie()//添加Cookie
request.getCookies:获取Cookie

可以记录用户名或免登陆等
2、Session:会话服务器状态
是将信息存储到服务器内存
JSP:session内置对象
Servlet:
HttpSession session=request.getSession();
session.setAttribute():存储数据
session.getAttribute:获取数据
session.setMaxInactiveInterval:设置Session最大空闲时间

 

Lambda表达式
简化格式：接口就一个方法的实现类的简化
格式:(需要重新的抽象方法的参数)->代码块

本周重点：
会用Servlet进行获取数据和响应数据
会解析XML\JSON
会使用JSP
会使用Cookie\Session

 

**》十五**

 

回顾：
Tomcat：服务器
Servlet:java语言运行在服务器的api
作用：
1、获取浏览器或客户端的请求信息
2、对浏览器或客户端进行信息的响应
HttpServletRequest:请求
getXXXX:获取
HttpServletResponse:响应
setXX:设置
getOutputStream：获取输出字节流
getWriter：输出字符流

JSP:Java对Servlet和HTML的整合页面

符号：
<%!   %>声明
<%    %>使用
<%=   %>显示

 


三大指令：
page\include\taglib
<%@ 指令名称%>
四大域：
就是根据作用域的不同而区分,主要存储数据
从大到小：application--->session---->request---->pageContext
九大对象：
request
response
session
page
pageContext
application
exception
config
out
三元素
include\forward\param
<jsp:元素名称>

EL:表达式----代替<%=  %>
作用：
1、获取请求参数
2、获取域中属性
3、获取cookie
格式：${ }

JSTL:标准标签库----代替<%    %>
使用步骤：
1、导入jar包
2、在jsp使用taglib标记
3、使用标签

核心库:c
<c:out>
<c:set>
<c:remove>
<c:if>
<c:choose>
<c:forEach>


JSP和Servlet区别？
开发中：
JSP主要用来接收服务器数据并显示
Servlet主要用来进行逻辑处理


三跨数据格式：
HTML
XML
JSON

Cookie
Session

 

内容：
一、反射
反射：就是程序运行时，动态获取类的信息的行为

反射作用：
1、获取类的属性、方法、注解等
2、执行私有的属性或方法

Class:字节码类

Class对象的创建方式：
1、类名.class
2、对象名.getClass
3、Class.forName(类名全称)

常用方法：
getFields:获取所有public字段
getDeclaredFields：获取本类的所有字段不包含继承
getDeclaredField:获取指定字段名称的字段对象
getMethods：获取所有的public方法
getDeclaredMethods：获取本类所有的方法，不包含继承
getDeclaredMethod：获取指定方法名称，形式参数的方法对象
getDeclaredAnnotations() ：获取当前类的注解
newInstance：创建对应类的对象


Method类：
常用方法：
	getAnnotations:获取方法上的注解
	invoke：执行方法
	setAccessible：设置是否忽略访问校验
Filed类：
常用方法：
	set:设置属性的值
	get:获取属性的值
	setAccessible：设置是否忽略访问校验
	getAnnotations:获取字段上的注解


Object

二、注解
注解就是为机器添加的注释，主要为了简化开发

元注解：就是修饰注解的注解
开发中定义注解的时候会使用元注解
四大元注解：
@Target 表示该注解用于什么地方，可能的值在枚举类 ElemenetType 中
设置注解的使用范围
@Retention 表示在什么级别保存该注解信息。可选的参数值在枚举类型 RetentionPolicy 中
@Documented 将此注解包含在 javadoc 中 ，它代表着此注解会被javadoc工具提取成文档。在doc文档中的内容会因为此注解的信息内容不同而不同。相当与@see,@param 等
@Inherited 允许子类继承父类中的注解

创建注解步骤:
1、创建注解对象，@interface
	1、标记注解的使用范围和有效期
	2、定义属性，记录注释内容
2、使用注解
	1、要在注解的使用范围之内使用
3、解析注解
	1、通过反射获取注解的内容使用

开发中主要是使用别人家的注解


3、泛型
就是引用类型的占位符，避免了转型
jdk5.0之后出现的

26个大写字母都可以

记住：泛型无论在哪使用，都必须声明
格式:<字母>
使用:字母

使用方式：
1、泛型类或接口
类名<字母>
2、泛型方法
<字母> 返回值 方法名称(形式参数)
{


}

主要用来开发工具类、框架

基本要求：
反射：会获取类中的信息
注解：使用注解时，可以大概猜出源码流程
泛型：要求会使用


开发实例：
1、模拟Gson解析
2、模拟QueryRunner的查询

 

**》十六**

 

一、文件上传
用户通过网页将选择的图片上传到服务器
1、Servlet
负责接收请求来的文件信息并保存到服务器
2、HTML/JSP
负责用户选择文件并上传到对应的Servlet

服务器将文件存储在
META-INF
WEB-INF
这些下面的时候，浏览器无法访问
如果存在WebContent下可以访问

使用Apache的jar包实现文件的上传

文件上传步骤：
1、创建jsp或html页面
注意使用表单，一定要设置表单的enctype="multipart/form-data属性
表示是文件上传的表单
2、创建Servlet
3、重写doPost方法
4、运行

涉及到的类：
1、DiskFileItemFactory：文件工厂，主要用来创建上传对象
常用方法：
setRepository：设置上传的临时文件夹

2、ServletFileUpload：上传类
静态方法：isMultipartContent验证是否有file标签
常用实例方法：
	setHeaderEncoding：设置编码格式
	setFileSizeMax：设置文件总大小
	setSizeMax：设置总大小
	parseRequest：获取当前请求中的所有参数信息

3、FileItem:文件对象
常用方法：	
	1、isFormField:验证是否为非file标签
	2、getName：获取文件名称
	3、getInputStream：获取文件内容

二、文件下载
不需要任何jar包

主要记忆的是以下消息头：
1、user-agent：获取浏览器类型
2、content-disposition：设置文件名称，值：ttachment;filename=文件名
3、setContentType：设置正文内容的MIME类型
4、setContentLengthLong：设置正文的字节个数

使用步骤：
1、获取要下载的文件路径
2、验证文件是否存在
3、设置消息头
4、创建流，读取内容并返回
5、运行

三、事件
事件：就是一组行为
实现了事件接口的类就叫监听器
事件的使用步骤：
1、创建类实现对应的xxxListener
2、重写方法
3、配置事件
4、运行

注册事件的方式：
1、xml配置
在web.xml中使用
<listener>
<listener-class>监听器的类名</listener-class>
</listener>
2、注解配置
在监听器的类上使用
@WebListener就可以

创建方式：
1、自定义类，实现接口，配置事件
2、直接new Listener


常用事件：
1、ServletContext:
ServletContextListener:监听创建或销毁
ServletContextAttributeListener：监听属性的新增、修改、删除

2、HttpSession:
HttpSessionListener：监听创建或销毁
HttpSessionAttributeListener：监听属性的新增、修改、删除
HttpSessionBindingListener：监听属性为对象的新增、删除

3、ServletRequest:
ServletRequestListener：监听创建、销毁
ServletRequestAttributeListener：监听属性的新增、修改、删除

 

12306的Session失效的效果
10秒无交互，失效

思路：
要不要保存所有的Session
保存在什么地方
怎么验证失效
定时验证

 

重点：文件上传&下载

 

**》十七**

 

File类：
FileNameFilter
FileFilter


Listener：监听，域的变化

Servlet过滤器Filter接口：会对每一次的请求进行过滤
使用步骤：
1、创建Filter类
2、重写对应的方法
3、配置


Filter接口中的方法：
1、init:初始化
参数说明：
FilterConfig：过滤器的配置信息
	常用方法：
	getInitParameter：获取配置的参数的值

2、doFilter:过滤
重要方法，进行条件的筛选，是否允许放行
参数说明：
		1、request:请求对象
		2、response:响应对象
		3、chain:过滤器通行对象
		
3、destroy:销毁


FilterChain：就是调用下一个链

常用方法：
doFilter：继续请求
参数说明：
	1、request:请求对象
	2、response:响应对象

Filter的注册方式：
1、xml配置
在web.xml下使用
<!--配置过滤器  -->
 <!--filter：让服务器创建过滤器对象  -->
  <filter>
      <filter-name>myf1</filter-name>
      <filter-class>org.qf.study1.MyFilter2</filter-class>
      
  </filter>
  <!--filter-mapping：设置当前过滤对象的过滤原则  -->
  <filter-mapping>
      <filter-name>myf1</filter-name>
      <url-pattern>/MyServlet1</url-pattern>
  </filter-mapping>

2、注解
@WebFilter:配置过滤器
注解的常用属性：
	1、urlPatterns：设置要过滤的资源  /* 所有 /Servlet名称 /xxx.jsp 
	2、initParams：设置过滤器的配置信息

注解注册和xml注册的区别？
1、注解方便，xml麻烦
2、注解无法设置过滤器的顺序(是根据过滤器的名称的字符串顺序进行的)
xml配置可以设置，是根据<filter-mapping>的先后顺序进行加载的
	
	

在开发中，get传递中文，一般都乱码
url编码为ISO8859-1,而这种编码不支持中文

过滤器的常用：
1、使用过滤器实现get的自动转码
步骤：
1、创建过滤器
2、在doFilter的方法中验证请求方式
3、get请求需要将请求参数的内容的编码格式都转换
	重写请求对象的参数内容：HttpServletRequestWrapper
4、重写获取参数值的方法
		1、getParameter
		2、getParameterMap
		3、getParameterValues

2、自动登录

 

 

具有登录功能的网页
设计数据库
搭框架

 

Md5:加密，不可逆
目前使用最广泛，因为简单

 

Ajax
JQuery
项目

 

**》十八**

 

同步：单线程：代码会从上到下进行，会引起阻塞
异步：多线程：开启多个线程，各个互不影响


2个子线程分别运行。如何在主线程中获取2个子线程的结束？


一、Ajax:Asynchronous JavaScript And XML
局部刷新网页的技术

1、核心对象：XMLHttpRequest
获取方式：
//根据浏览器的不同创建Ajax对象
function getXMLRequest(){
	var xttp;
	if(window.XMLHttpRequest){//当前浏览器支持直接创建ajax对象
		xttp=new XMLHttpRequest();
	}else{//支持IE5\6
		xttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	return xttp;
}

2、常用属性：
	readyState：类型short；只读  
	取值说明：
		就绪状态码	说    明
			0		XMLHttpRequest对象没有完成初始化即：刚刚创建。
			1		XMLHttpRequest对象开始发送请求调用了open方法，但还没有调用send方法。请求还没有发出
			2		XMLHttpRequest对象的请求发送完成send方法已经调用，数据已经提交到服务器，但没有任何响应
			3		XMLHttpRequest对象开始读取响应，还没有结束收到了所有的响应消息头，但正文还没有完全收到
			4		XMLHttpRequest对象读取响应结束一切都收到了
	
	responseText：类型String；只读，服务器返回内容：一般为普通字符串或json字符串
	responseXML：类型Document；只读，服务器返回内容：xml文档格式
	status：类型short；只读，服务器的状态响应码
	常用取值说明：
		状态码	说    明
		200		服务器响应正常
		400		无法找到请求的资源
		403		没有访问权限
		404		访问的资源不存在
		500		服务器内部错误


一般都需要验证readyState=4 并且 status=200
3、常用方法：
	open(method,URL,async) 	建立与服务器的连接method参数指定请求的HTTP方法,典型的值是GET或POST，URL参数指请求的地址，async参数指定是否使用异步请求，其值为true或false
	send(content) 	发送请求content参数指定请求的参数 
	setRequestHeader(header,value) 	设置请求的头信息 

4、事件属性：
	onreadystatechange：指定回调函数，获取服务器响应内容的函数
	值为函数，用来处理服务器响应内容
	
5、使用步骤：
	1、获取XMLHttpRequest对象------new XMLHttpRequest()
	2、设置回调函数-------onreadystatechange
	3、打开连接-----------open
	4、设置请求头---------setRequestHeader
	5、发送---------------send

 

登陆

练习：

1、开发注册页面
要求：
	1、自己写Servlet(2个，一个负责验证用户名是否存在，另一个负责添加用户)
	http://localhost:8080/Web_Day18_AutoLogin/checkname
	http://localhost:8080/Web_Day18_AutoLogin/register
	2、写一个HTML页面，完成注册功能。
	就是普通的html页面，没有使用表单
	3、Ajax实现用户名是否存在，实现注册
	通过Ajax传输数据并接受	
	
	
	
2、分页
步骤：
1、完成Servlet,接受当前页码和每页显示的数据
2、通过Ajax请求Servlet


JS解析JSON数据：
通常使用JSON.parse进行解析
有点类似JSON源生解析

 

3、搜索
根据商品名称实现模糊搜索
只需在Servlet添加参数，商品名称
html中通过JS控制

 

**》十九**

 

一、jQuery
简化JS的使用
jQuery:就是一个轻量级的JS库
简化JS的使用
目的：写的更少，做的更多

需要引入jQuery库
免费、开源

1.2jQuery的格式：$("选择器").行为(参数);
解释：
	1、$:jQuery的标记
	2、选择器：类似CSS选择器
	3、行为：操作对知道的元素进行的操作
	
	
页面加载完成之后，调用	
$(document).ready(function(){

});
等价于：
window.onload=function(){


}
	
1.3jQuery的选择器：
常用选择器：
	1、标签选择器：直接写HTML标签名称
	2、class:为HTML标签使用class属性的值，.类名
	3、id:为HTML标签使用id属性的值，#id名

注意：选择器可以混合使用，只需记得每种的格式

1.4jQuery的行为/操作
	行为可以是事件、文档、样式、属性、效果

1.4.1事件：对应的就是HTML标签的事件属性
事件的的参数都是函数
	常用事件：
		ready：加载完触发
		click：点击触发
		change：内容改变触发
		blur：失去焦点触发
		focus：获取焦点触发
		dblclick：双击触发

1.4.2文档
操作Html的内容,参数一般为要设置的文本内容，不带参数就是得到
	html:内部的HTML内容----innerHTML
	val:input标签的value属性
	append:追加,在当前的选择器的元素内部追加
	after:添加，在当前选择器元素的后面添加
	before:添加，在当前选择器元素的前面添加
	remove:移除
	

1.4.3样式
就是为HTML元素设置CSS属性
 常用行为：
 css:设置对应标签的属性
 参数：
 	1、css属性敏成功
 	2、css属性的值
 取值说明:
	 1、没有：获取属性的中
	 2、具体的值：设置属性的值
	 3、函数：必须返回当前属性的值


1.4.4属性
改变HTML的标签属性
	常用：
		attr():设置或获取对应的属性
		toggleClass:新增或删除class属性，不存在就新增，存在就删除
		removeAttr:移除属性


1.4.5效果
特效
1、显示和隐藏
	hide:隐藏
	参数说明：
	1、没有参数：就是立即执行
	2、1个参数：就是在指定的毫秒之内隐藏完毕
	3、2个参数：就是在指定的毫秒之内隐藏完毕，并回调指定的函数
	
	show:显示
	参数说明：
		1、没有参数：就是立即执行
		2、1个参数：就是在指定的毫秒之内显示完毕
		3、2个参数：就是在指定的毫秒之内显示完毕，并回调指定的函数
	toggle:显示或隐藏
	参数说明：
		1、没有参数：就是立即执行
		2、1个参数：就是在指定的毫秒之内执行完毕
		3、2个参数：就是在指定的毫秒之内执行完毕，并回调指定的函数


2、透明度
就是可见的清晰度
范围：[0.0,1.0]
其中：0.0看不见
1.0完全可见
常用：
	fadeTo:改变透明度
	参数说明：
		1、速度
		2、透明度
		3、回调函数
	
3、自定义动画
可以自己指定属性的变化
	常用：
	animate：设置动画
	参数说明：
	1、属性内容：json数据，可以同时设置多个属性变化
	2、动画持续时间
	
	
	


1.5jQuery的Ajax
就是jQuery对象Ajax技术进行了封装，简化
常用：
1、load:
使用方式：
$(选择器).load(url,data,callback)
一般都是直接使用
会默认将返回结果设置到对应的选择器

2、ajax：
使用方式：
	$.ajax(json格式数据):参数为{属性名称:值}
	常用的名称：
		url:要请求的路径
		data:参数
		method:方式
		success:成功的回调方法
		error:错误的回调方法
3、get:get请求
使用方式：
	$.get(url,data,callback)
4、post:post请求
使用方式：
	$.post(url,data,callback)

 

 

 

练习：使用jQuery对输入框的内容进行校验

 

课下：
设计模式：建造者模式
链式编程

 

 


二、总结

1、前端
HTML:超文本标记语言
在浏览器中显示数据
格式：<标签名称></标签名称>

​	语法格式：
​		1、固定标签
​		2、标签嵌套
​		3、属性的值必须双引号
​		4、成对出现
​		5、不区分大小写
​	
常用标签：
div\form\p\a\img\input\ul\ol\select\textarea\hr\br\table\tr\td\th\span\h1\label\frameset\frame
常用属性：
width\height\src\href\border\align\style\id\class\name\action\method\enctype\checked\disabled
常用事件：
onload\onclick\ondblclick\onfocus\onblur\onchange\onmouseout\onmouseover\onsubmit\onselect

CSS:层叠样式表
美化HTML

格式：选择器{属性名称:值}

引用方式：
1、外部：link
3、内部：在head使用<style>
4、内嵌：在标签中使用style属性

选择器：
1、标签：标签名称{}
2、class:：.类名
3、id：#id名称
4、属性：[属性名称=值]

常用属性：
	1、背景：backgroundXXX
	2、文本:textXXX
	3、字体:fontXXX
	4、盒子模型
		1、borderXXX:边框
		2、marginXXX:外边距
		3、paddingXXX:内边距
	5、列表：list-style-type
	6、位置：
		1、float:浮动
		2、position:定位
		
JS:JavaScript:浏览器脚本语言
动态交互HTML

引用方式：
1、外部js文件：<script src="路径"/>
2、head内部:<script>
3、body内部：<script>

JS语法：
1、数据类型：
	原始类型：字符串、数字、boolean
	引用类型:
2、变量：可变的量
格式：var 变量名称[=值];

js的变量可以不声明直接使用

3、运算符：算术、比较、赋值、逻辑、位、三目、字符串连接

4、分支语句
	if else
	switch
	
break
return
5、循环语句
	for
	for in 
	while
	do\while
	

6、函数
格式：function 函数名称(参数名称){
	
	[return 返回值]
}

7、全局函数
	1、isNaN
	2、Number
	3、String
	4、parseInt
	5、parseFloat
	
8、内置对象
	1、Number
	2、String
	3、Math:
		floor
		random
		round
	4、Array
		创建数组
		可以改变长度
		常用属性：
		length
		常用方法：
		sort:排序
		
	5、Regex
		正则表达式
		以/正则规则/
		Regex
		Pattern
		
		常用字符串的：
		matchs:匹配
		split:切割
		replace:替换
		
	6、window
	7、Date
	
9、document
	常用方法：
		1、getElementById
		2、getElementsByTagName
		3、getElementsByName
		4、createElement
		5、setAttribute
		6、appendChild
		
		
	value
	src
	innerHTML
	href
	
	
	


2、数据库
MySQL:数据库
存储数据的仓库
SQL:结构化查询语言
分类：
1、DDL:数据库定义语言：create\drop\alter
2、DML:数据库操作语言：insert\update\delete
3、DCL:数据库控制语言：用户、安全等
4、DQL：数据库查询语言：select

七大语句：
create table/database
alter table/database
drop table/database

insert into 表名(字段) values(值)
update 表名 set 
delete from 表名 
select 字段 from 表名 [where] [order by] [group by] [having] [limit] 
 
约束条件：
1、主键 
2、唯一
3、非null
4、默认
5、外键

条件：
and\or
= != >=
like
in
between and
IS NULL
IS NOT NULL


聚合函数：
MAX
MIN
SUM
AVG
COUNT

函数：
now
random
md5

多表操作：
1、内联：inner join
2、左外连：left out join
3、右外连：right out join
4、笛卡尔积：select * from 表1,表2

触发器：自动新增、修改、删除等


事务：保证操作的一致性
start transaction:开启事务
commit：提交事务
rollback：回滚事务

事务特性：
A原子性
C一致性
I隔离性
	防止脏读、幻度、虚度等
D持久性

JDBC:java操作数据库的接口

使用步骤：
1、导入数据库驱动jar
2、动态加载
3、连接
4、获取操作SQL对象
5、执行SQL
6、销毁

DriverManager:驱动管理
Connection：连接
Statement：操作SQL
PreparedStatement:防SQL注入操作SQL
ResultSet:结果集


连接池：存储数据库连接的池子
管理数据库的连接

常用的数据库连接池：
1、C3P0
2、DBCP


QueryRunner:工具类
常用方法：
	1、update
	2、query
	
	BeanHander:将结果从ResultSet中转换为实体对象
	BeanListHander：将结果从ResultSet中转换为list
	
	
	

3、后端
Tomcat:服务器
免费、不要钱

目录：
bin:可执行
lib:第三方jar
config:配置文件
webapps：发布的Web应用
work：工作空间
temp：临时文件
log：日志

可以运行我们的Servlet&JSP

 

Servlet:java语言编写的运行在服务器的api
获取请求数据
响应数据
Servlet创建方式：
1、实现Servlet接口
	重写方法：
		init
		destroy
		service
2、继承GenericServlet
	重写：
		service
3、继承HttpServlet
	重写：
	doGet
	doPost
	
	
配置Servlet:
	1、xml配置
	在web.xml下使用：
		<servlet>
		<servlet-name>名称</servlet-name>
		<servlet-class>类名</servlet-class>
		</servlet>
		<servlet-mapping>
		<servlet-name>名称</servlet-name>
		<url-pattern>匹配规则</url-pattern>
		</servlet-mapping>
		
	2、注解
	@WebServlet()

使用步骤：
1、创建Servlet
2、重写方法doGet、doPost
3、配置
4、发布

Request:请求

常用方法：
	getParameter：获取指定key的值
	getParameterValues：获取指定key的所有值
	getParameterNames：获取所有的Key
	getParameterMap：获取所有的Key和对应的值
	getAttribute:获取指定Key的属性
	setAttribute：新增属性或修改属性
	removeAttribute：移除属性
	getMethod:获取请求方式
	getRemoteAddr:获取远程IP
	
	
属性和参数的区别？
属性可以存储对象
参数不可以

Response:响应
	常用：
	getOutputStream:获取输出字节流：响应数据
	getWriter：获取输出字符流：响应数据
	setCharacterEncoding:设置内容的编码格式
	setContentType:设置内容的类型
	setHeader:设置消息头
	

跳转：
1、客户端跳转
	重定向
2、服务端的跳转
	请求转发
	
请求行：自动携带
消息头：浏览器、服务器识别
	

请求的组成：
请求行
请求消息头
请求内容

 

JSP：Java Server Page
java根据Servlet和HTML结合创建
可以在浏览器直接访问，底层运行转化为诶Servlet

符号：
<%! %>:声明
<% %>：操作
<%= %>：显示

<%--注释--%>


三大指令：
格式：<%@ 指令名称 属性=值 %>
1、page:导入包、标记文档类型等
2、include:静态加载其他页面
3、taglib:标记JSTL标签库

 

九大对象：
内置，默认创建
1、request
2、response
3、application
4、session
5、page
6、pageContent
7、out
8、exception
9、config

 


四大域：
application----ServletContext
作用域：整个Web范围
session----HttpSession
作用域：客户访问产生，默认30分钟无响应销毁
request---HttpServletRequest
作用域：发起请求默认产生，也可以转发
pageContext---ServletContent
作用域：当前页面


都有以下方法：
	getAttribute
	setAttribute
	removeAttribute

三元素：
1、include:动态加载其他页面
2、forward:跳转其他页面
3、param:携带参数	

 


EL表达式：简化jsp <%=  %>
格式：${域中属性名称|param.参数名称|cookie}

 


JSTL:标准标签库
简化jsp-- <%  %>
c:out
c:set
c:forEach
c:choose
c:if
c:remove


Cookie:将信息保存在客户的电脑上，硬盘

request.getCookies:获取所有Cookie
response.addCookie:添加Cookie

记住账号

Session:将信息保存在服务器，内存

request.getSession

保存登录账号等

数据格式：
特点：三跨：跨平、跨网络、跨编程语言

1、HTML：浏览器
2、JSON：
[]数组
{}对象
常用解析：
	1、JSON源生
		JSONObject：解析对象
		JSONArray：解析数组
	2、Gson
		gson.fromJson:解析json数据
		gson.toJson：将对象转换为JSON字符串
	3、fastjson
		JSON.parseObject:解析对象
		JSON.parseArray:解析数组
3、XML

作用：
1、配置文件
2、传送数据

常用解析：
	1、Dom源生解析
	区分元素和节点
	2、SAX
	自定义DeafultHeader
	3、DOM4j
	SAXReader
	4、XPATH
	查询
	

4、技术

1、注解反射泛型
注解：为代码添加的注释
一般为解决特殊情况准备的
比如：json特殊的属性名称

 

创建注解：
1、创建@interface
2、创建属性
3、使用注解
4、解析注解

反射：
动态获取类的信息

作用：
1、获取类的信息：属性、方法、注解、构造器等
2、执行私有属性或方法


Class类

泛型：
用来占位的字母

引用类型占位，避免转型

结合反射使用

切记：泛型也得先声明才可以使用
<字母>这就是声明


文件上传&下载
文件上传使用第三方：
1、页面需要有file标签
2、设置表单的enctype="muilpart/form-data"
3、在对应的Servlet获取FileItem
4、创建保存路径
5、流的读取和写出


Listener:事件
就是获取application\session\request等变化的接口

注册方式：
1、xml
<listener>
<listener-class></listener-class>
</listener>
2、注解
@WebListener

监听器：就是实现了Listener接口的类
创建或销毁、属性内容的变化


Filter:过滤器
可以对每一次的请求和响应作出过滤

注册方式：
1、xml
在web.xml下使用：
		<filter>
		<filter-name>名称</filter-name>
		<filter-class>类名</filter-class>
		</filter>
		<filter-mapping>
		<filter-name>名称</filter-name>
		<url-pattern>匹配规则</url-pattern>
		</filter-mapping>
		
2、注解
@WebFilter(ulrPatterns={})

注解的执行顺序按照字符串顺序
xml是按照<filter-mapping>先后顺序执行


生命周期：
1、init
2、doFilter
继续必须使用chain.doFilter

3、destroy


作用
1、自动转码
2、自动登录

 

Ajax:异步局部刷新
可以对网页的局部内容进行请求服务器病显示返回结果

结果数据格式：
1、字符串
2、JSON
3、xml

 

XMLHttpRequest:Ajax的核心对象
new XMLHttpRequest();


常用属性：
1、readyState:读取状态
	0初始化
	1打开链接
	2发生请求
	3未接收完全
	4全部OK
2、status:服务器返回的状态响应码
	200：OK
	404:未发现：要么服务器宕机、要么服务器将页面删除了、要么url路径写错了
	403：没有权限
	500：服务器错误

事件属性：
onreadystatechange:读取状态变化事件
对应的值：回调函数

常用方法：
1、open:打开链接
2、setRequestHeader:设置消息头
3、send：发送

 

使用步骤：
1、创建XMLHttpRequest对象
2、设置回调函数
3、打开链接
4、设置请求头
5、发送信息


jQuery:简化JS
就是对JS进行了封装

详情看上面

 

每天多努力一点，你将会变得更好。





标签: [java知识](https://www.cnblogs.com/lidar/tag/java%E7%9F%A5%E8%AF%86/)