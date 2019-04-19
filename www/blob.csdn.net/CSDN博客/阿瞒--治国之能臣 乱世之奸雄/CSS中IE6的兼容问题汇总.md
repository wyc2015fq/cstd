# CSS中IE6的兼容问题汇总 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年04月01日 10:35:00[baofenny](https://me.csdn.net/jiaminbao)阅读数：1940
一、CSS HACK
以下两种方法几乎能解决现今所有HACK.翻阅很多资料，已测试可以使用。
1, !important
随着IE7对!important的支持, !important 方法现在只针对IE6的HACK.(注意写法.记得该声明位置需要提前.)
PLAIN TEXT
CSS:
#wrapper
{
width: 100px!important; /* IE7+FF */
width: 80px; /* IE6 */
}
2, IE6/IE77对FireFox
*+html 与 *html 是IE特有的标签, firefox 暂不支持.而*+html 又为 IE7特有标签.
PLAIN TEXT
CSS:
#wrapper
{
#wrapper { width: 120px; } /* FireFox */
*html #wrapper { width: 80px;} /* ie6 fixed */
*+html #wrapper { width: 60px;} /* ie7 fixed, 注意顺序 */
}
注意:
*+html 对IE7的HACK 必须保证HTML顶部有如下声明：
PLAIN TEXT
HTML:
<!DOCTYPE HTML PUBLIC “-//W3C//DTD HTML 4.01 Transitional//EN”　”http://www.w3.org/TR/html4/loose.dtd”>
二、万能 float 闭合(非常重要!)
关于 clear float 的原理可参见 [How To Clear Floats Without Structural Markup]
将以下代码加入Global CSS 中,给需要闭合的div加上 class=”clearfix” 即可,屡试不爽.
PLAIN TEXT
CSS:
/* Clear Fix */
.clearfix:after{
content:“.”;
display:block;
height:0;
clear:both;
visibility:hidden;
}
.clearfix{
display:inline-block;
}
/* Hide from IE Mac */
.clearfix {display:block;}
/* End hide from IE Mac */
/* end of clearfix */
三、其他兼容技巧(再次啰嗦)
1, FF下给 div 设置 padding 后会导致 width 和 height 增加, 但IE不会.(可用!important解决)
2, 居中问题.
1).垂直居中.将 line-height 设置为 当前 div 相同的高度, 再通过 vertical-align: middle.( 注意内容不要换行.)
2).水平居中. margin: 0 auto;(当然不是万能)
3, 若需给 a 标签内内容加上 样式, 需要设置 display: block;(常见于导航标签)
4, FF 和 IE 对 BOX 理解的差异导致相差 2px 的还有设为 float的div在ie下 margin加倍等问题.
5, ul 标签在 FF 下面默认有 list-style 和 padding . 最好事先声明, 以避免不必要的麻烦. (常见于导航标签和内容列表)
6, 作为外部 wrapper 的 div 不要定死高度, 最好还加上 overflow: hidden.以达到高度自适应.
7, 关于手形光标. cursor: pointer. 而hand 只适用于 IE.
1 针对firefox ie6 ie7的css样式
现在大部分都是用!important来hack，对于ie6和firefox测试可以正常显示，
但是ie7对!important可以正确解释，会导致页面没按要求显示！找到一个针
对IE7不错的hack方式就是使用“*+html”，现在用IE7浏览一下，应该没有问题了。
现在写一个CSS可以这样：
PLAIN TEXT
CSS:
#1 { color: #333; } /* Moz */
* html #1 { color: #666; } /* IE6 */
*+html #1 { color: #999; } /* IE7 */
那么在firefox下字体颜色显示为#333，IE6下字体颜色显示为#666，IE7下字体颜色显示为#999。
2 css布局中的居中问题
主要的样式定义如下：
PLAIN TEXT
CSS:
body {TEXT-ALIGN: center;}
#center { MARGIN-RIGHT: auto; MARGIN-LEFT: auto; }
说明：
首先在父级元素定义TEXT-ALIGN: center;这个的意思就是在父级元素内的内容居中；对于IE这样设定就已经可以了。
但在mozilla中不能居中。解决办法就是在子元素定义时候设定时再加上“MARGIN-RIGHT: auto;MARGIN-LEFT: auto; ”
需要说明的是，如果你想用这个方法使整个页面要居中，建议不要套在一个DIV里，你可以依次拆出多个div，
只要在每个拆出的div里定义MARGIN-RIGHT: auto;MARGIN-LEFT: auto; 就可以了。
3 盒模型不同解释.
PLAIN TEXT
CSS:
#box{ width:600px; //for ie6.0- w\idth:500px; //for ff+ie6.0}
#box{ width:600px!important //for ff width:600px; //for ff+ie6.0 width /**/:500px; //for ie6.0-}
4 浮动ie产生的双倍距离
PLAIN TEXT
CSS:
#box{ float:left; width:100px; margin:0 0 0 100px; //这种情况之下IE会产生200px的距离 display:inline; //使浮动忽略}
这里细说一下block,inline两个元素,Block元素的特点是:总是在新行上开始,高度,宽度,行高,边距都可以控制(块元素);Inline元素的特点是:和其他元素在同一行上,…不可控制(内嵌元素);
PLAIN TEXT
CSS:
#box{ display:block; //可以为内嵌元素模拟为块元素 display:inline; //实现同一行排列的的效果 diplay:table;
5 IE与宽度和高度的问题
IE不认得min-这个定义，但实际上它把正常的width和height当作有min的情况来使。这样问题就大了，如果只用宽度和高度，
正常的浏览器里这两个值就不会变，如果只用min-width和min-height的话，IE下面根本等于没有设置宽度和高度。
比如要设置背景图片，这个宽度是比较重要的。要解决这个问题，可以这样：
PLAIN TEXT
CSS:
#box{ width: 80px; height: 35px;}html>body #box{ width: auto; height: auto; min-width: 80px; min-height: 35px;}
6 页面的最小宽度
min-width是个非常方便的CSS命令，它可以指定元素最小也不能小于某个宽度，这样就能保证排版一直正确。但IE不认得这个，
而它实际上把width当做最小宽度来使。为了让这一命令在IE上也能用，可以把一个<div> 放到 <body> 标签下，然后为div指定一个类：
然后CSS这样设计：
PLAIN TEXT
CSS:
#container{ min-width: 600px; width:expression(document.body.clientWidth <600? “600px”: “auto” );}
第一个min-width是正常的；但第2行的width使用了Javascript，这只有IE才认得，这也会让你的HTML文档不太正规。它实际上通过Javascript的判断来实现最小宽度。
7 清除浮动
PLAIN TEXT
CSS:
.hackbox{ display:table; //将对象作为块元素级的表格显示}或者.hackbox{ clear:both;}
或者加入:after（伪对象）,设置在对象后发生的内容，通常和content配合使用，IE不支持此伪对象，非Ie 浏览器支持，
所以并不影响到IE/WIN浏览器。这种的最麻烦的……
PLAIN TEXT
CSS:
#box:after{ content: “.”; display: block; height: 0; clear: both; visibility: hidden;}
8 DIV浮动IE文本产生3象素的bug
左边对象浮动，右边采用外补丁的左边距来定位，右边对象内的文本会离左边有3px的间距.
PLAIN TEXT
CSS:
#box{ float:left; width:800px;}#left{ float:left; width:50%;}#right{ width:50%;}*html #left{ margin-right:-3px; //这句是关键}
HTML代码<div id=“box”> <div id=“left”></div> <div id=“right”></div></div>
9 属性选择器(这个不能算是兼容,是隐藏css的一个bug)
PLAIN TEXT
CSS:
p[id]{}div[id]{}
这个对于IE6.0和IE6.0以下的版本都隐藏,FF和OPera作用
属性选择器和子选择器还是有区别的,子选择器的范围从形式来说缩小了,属性选择器的范围比较大,如p[id]中,所有p标签中有id的都是同样式的.
10 IE捉迷藏的问题
当div应用复杂的时候每个栏中又有一些链接，DIV等这个时候容易发生捉迷藏的问题。
有些内容显示不出来，当鼠标选择这个区域是发现内容确实在页面。
解决办法：对#layout使用line-height属性 或者给#layout使用固定高和宽。页面结构尽量简单。
11 高度不适应
高度不适应是当内层对象的高度发生变化时外层高度不能自动进行调节，特别是当内层对象使用
margin 或paddign 时。
例：
PLAIN TEXT
CSS:
<div id=“box”>
<p>p对象中的内容</p>
</div>
CSS：#box {background-color:#eee; }
#box p {margin-top: 20px;margin-bottom: 20px; text-align:center; }
解决方法：在P对象上下各加2个空的div对象CSS代码：.1{height:0px;overflow:hidden;}或者为DIV加上border属性。
网页常用代码-未整理 
27 Jun, 2008 No Comment
如内容超出单元格，则隐藏
style=”TABLE-LAYOUT: fixed”
让弹出窗口总是在最上面: <body onblur=”this.focus();”>
不要滚动条? 让竖条没有: <body style=’overflow:scroll;overflow-y:hidden’> </body>
让横条没有: <body style=’overflow:scroll;overflow-x:hidden’> </body>
两个都去掉？更简单了 <body scroll=”no”> </body>
怎样去掉图片链接点击后，图片周围的虚线？ <a href=”#” onFocus=”this.blur()”><img src=”logo.jpg” border=0></a>
电 子邮件处理提交表单 <form name=”form1″ method=”post” action=”mailto:****@***.com” enctype=”text/plain”> <input type=submit> </form>
在打开的子窗口刷新父窗口的代码里如何写？ window.opener.location.reload()
如何设定打开页面的大小 <body onload=”top.resizeTo(300,200);”>
在 页面中如何加入不是满铺的背景图片,拉动页面时背景图不动 <html><head> <STYLE> body {background-image:url(logo.gif); background-repeat:no-repeat; background-position:center } </STYLE> </head> <body bgproperties=”fixed” > </body> </html>
各种样式的光标 auto ：标准光标
default ：标准箭头
hand ：手形光标
wait ：等待光标
text ：I形光标
vertical-text ：水平I形光标
no-drop ：不可拖动光标
not-allowed ：无效光标
help ：?帮助光标
all-scroll ：三角方向标
move ：移动标
crosshair ：十字标 e-resize n-resize nw-resize w-resize s-resize se-resize sw-resize
本机ip<%=request.servervariables(”remote_addr”)%>
服务器名<%=Request.ServerVariables(”SERVER_NAME”)%>
服务器IP<%=Request.ServerVariables(”LOCAL_ADDR”)%>
服务器端口<%=Request.ServerVariables(”SERVER_PORT”)%>
服务器时间<%=now%> IIS
版本<%=Request.ServerVariables”SERVER_SOFTWARE”)%>
脚本超时时间<%=Server.ScriptTimeout%>
本文件路径<%=server.mappath(Request.ServerVariables(”SCRIPT_NAME”))%>
服务器CPU数量<%=Request.ServerVariables(”NUMBER_OF_PROCESSORS”)%>
服 务器解译引擎<%=ScriptEngine & “/”& ScriptEngineMajorVersion &”.”&ScriptEngineMinorVersion&”.”& ScriptEngineBuildVersion %>
服务器操作系统<%=Request.ServerVariables(”OS”)%>
文本竖排方式
<style type=”text/css”>
<!–
.shupai {Writing-mode:tb-rl}
–>
</style>
超链接去虚线边框
在链接中加上onfocus=”this.blur()”
网页搜索关键字 头里插入
<META NAME=”keywords” CONTENT=”xxxx,xxxx,xxx,xxxxx,xxxx,”>
收藏夹图标
<link rel = “Shortcut Icon” href=”favicon.ico”>
我的电脑
file:///::{20D04FE0-3AEA-1069-A2D8-08002B30309D}
网上邻居
file:///::%7B208D2C60-3AEA-1069-A2D7-08002B30309D%7D
我的文档
file:///::%7B450D8FBA-AD25-11D0-98A8-0800361B1103%7D
控制面板
file:///::{20D04FE0-3AEA-1069-A2D8-08002B30309D}/::{21EC2020-3AEA-1069-A2DD-08002B30309D}
回收站
file:///::%7B645FF040-5081-101B-9F08-00AA002F954E%7D
鼠标控制图片隐现效果
把如下代码加入＜body＞区域中：
<SCRIPT language=”javascript”>
<!–
function makevisible(cur,which){
if (which==0)
cur.filters.alpha.opacity=100
else
cur.filters.alpha.opacity=20
}
//–>
</SCRIPT>
2、把如下代码加入＜body＞区域中：
<img src=”2.gif” style=”filter:alpha(opacity=20)”
onMouseOver=”makevisible(this,0)”
onMouseOut=”makevisible(this,1)”>
禁止图片下载
<A HREF=”javascript:void(0)” onMouseover=”alert(’对不起,此图片不能下载!’)”>
<IMG SRC=”2.gif” Align=”center” Border=”0″ width=”99″ height=”50″></A>
页嵌页
<iframe width=291 height=247 src=”main.files/news.htm” frameBorder=0></iframe>
隐藏滚动条
<body style=”overflow-x:hidden;overflow-y:hidden”
CSS文字阴影（定义在<TD>中）
.abc{
FILTER: dropshadow(color=#666666, offx=1, offy=1, positive=1); FONT-FAMILY: “宋体”; FONT-SIZE: 9pt;COLOR: #ffffff;
}
列表/菜单
onchange=”location=this.options[this.selectedIndex].value”
<iframe id=”frm” src=”k-xinwen.html” scrolling=”no” width=”314″ height=”179″></iframe>
<img src=”xiangshang.jpg” onMouseOver=”sf=setInterval(’frm.scrollBy(0,-2)’,1)” onMouseOut=”clearInterval(sf)” width=”31″ height=”31″>
<img src=”xiangxia.jpg” onMouseOver=”sf=setInterval(’frm.scrollBy(0,2)’,1)” onMouseOut=”clearInterval(sf)” width=”31″ height=”31″ >
reurl=server.htmlencode(request.ServerVariables(”HTTP_REFERER”))
服务器上如何定义连接
MM_www_STRING =”driver={Microsoft access Driver (*.mdb)};dbq=” & server.mappath(”../data/www.mdb”)
链接到
response.redirect”login.asp”
location.href=”xx.asp”
onClick=”window.location=’login.asp’”
onClick=”window.open(”)”
取得IP
userip = Request.ServerVariables(”HTTP_X_FORWARDED_FOR”)
If userip = “” Then userip = Request.ServerVariables(”REMOTE_ADDR”)
sql=”update feedbak set hit=hit+1 where id=”&request(”id”)
conn.execute(sql)
截取字符是否加…
function formatStr(str,len)
if(len(str)>len)
str = left(str,len) + “…”
end if
formatStr = str
end function
接收表单
If Ucase(Request.ServerVariables(”REQUEST_METHOD”)) = “POST” then
end if
图片宽度
<script language=”javascript”>
<!–
var flag=false;
function DrawImage(ckp){
var image=new Image();
image.src=ckp.src;
if(image.width>0 && image.height>0)
{flag=true;
if(image.width>120){
ckp.width=120;
}else{
ckp.width=image.width;
}
ckp.alt=image.width+”×”+image.height;
}
}
//–>
</script>
I’ll be Back 22:18:06
<img src=”<%=formPath%>/<%=rs(”photoname”)%>” border=”0″ onload=”javascript:DrawImage(this);”>
跳转
<meta http-equiv=refresh content=’0; url=/distributor/distributor.aspx’>
溢出栏的设制
visible：超出的部分照样显示；
hidden：超出的部分隐藏；
scrool：不管有否超出，都显示滚动条；
auto：有超出时才出现滚动条；
onMouseOver：鼠标移到目标上；
onMouseUp：按下鼠标再放开左键时；
onMouseOut：鼠标移开时；
onMouseDown：按下鼠标时（不需要放开左键）；
onClink：点击时；
onDblClick：双击时；
onLoad：载入网页时；
onUnload：离开页面时；
onResize：当浏览者改变浏览窗口的大小时；
onScroll：当浏览者拖动滚动条的时。
CSS样式
a:link：表示已经链接；
a:hover：表示鼠标移上链接时；
a:active：表示链接激活时；
a:visited：表示己点击过的链接。
跳出对话框链接
javascript:alert(’lajflsjpjwg’)
后退：javascript:history.back(1)
关闭窗口：javascript:window.close();
窗口还原
function restore(){
window.moveTo(8,8);
window.resizeTo(screen.width-24,screen.availHeight-24);
}
head区是指首页HTML代码的<head>和</head>之间的内容。
必须加入的标签
1.公司版权注释
<!— The site is designed by Maketown,Inc 06/2000 —>
2.网页显示字符集
简体中文：<META HTTP-EQUIV=”Content-Type” CONTENT=”text/html; charset=gb2312″>
繁体中文：<META HTTP-EQUIV=”Content-Type” CONTENT=”text/html; charset=BIG5″>
英 语：<META HTTP-EQUIV=”Content-Type” CONTENT=”text/html; charset=iso-8859-1″>
3.网页制作者信息
<META name=”author” content=”webmaster@maketown.com”>
4.网站简介
<META NAME=”DESCRIPTION” CONTENT=”xxxxxxxxxxxxxxxxxxxxxxxxxx”>
5.搜索关键字
<META NAME=”keywords” CONTENT=”xxxx,xxxx,xxx,xxxxx,xxxx,”>
6.网页的css规范
<LINK href=”style/style.css” rel=”stylesheet” type=”text/css”>
(参见目录及命名规范)
7.网页标题
<title>xxxxxxxxxxxxxxxxxx</title>
.可以选择加入的标签
1.设定网页的到期时间。一旦网页过期，必须到服务器上重新调阅。
<META HTTP-EQUIV=”expires” CONTENT=”Wed, 26 Feb 1997 08:21:57 GMT”>
2.禁止浏览器从本地机的缓存中调阅页面内容。
<META HTTP-EQUIV=”Pragma” CONTENT=”no-cache”>
3.用来防止别人在框架里调用你的页面。
<META HTTP-EQUIV=”Window-target” CONTENT=”_top”>
4.自动跳转。
<META HTTP-EQUIV=”Refresh” CONTENT=”5;URL=http://www.yahoo.com”>
5指时间停留5秒。
5.网页搜索机器人向导.用来告诉搜索机器人哪些页面需要索引，哪些页面不需要索引。
<META NAME=”robots” CONTENT=”none”>
CONTENT的参数有all,none,index,noindex,follow,nofollow。默认是all。
6.收藏夹图标
<link rel = “Shortcut Icon” href=”favicon.ico”>
所有的javascript的调用尽量采取外部调用.
<SCRIPT LANGUAGE=”javascript” SRC=”script/xxxxx.js”></SCRIPT>
附<body>标签：
<body>标签不属于head区，这里强调一下，为了保证浏览器的兼容性，必须设置页面背景<body bgcolor=”#FFFFFF”>
flash透明
在flash的源代码中加上：<param name=”wmode” value=”transparent”>表格透明
style=”FILTER: alpha(opacity=72)”
网址前添加icon的方法
1、上http://www.favicon.com上用他的icon editor online制作一个图标。他会将做好的图标通过email即时发送给你。
2、把这个命名为favicon.ico的图标放置在index.html同一个文件夹中。就可以了。
作一个图标文件，大小为16*16像素。文件扩展名为ico，然后上传到相应目录中。在HTML源文件“＜head＞＜/head＞”之间添加如下代码：
＜Link Rel=”SHORTCUT ICON” href=”http://图片的地址（注意与刚才的目录对应）”＞
其中的“SHORTCUT ICON”即为该图标的名称。当然如果用户使用IE5或以上版本浏览时，就更简单了，只需将图片上传到网站根目录下，自动识别
可以在收藏夹中显示出你的图标<link rel=”Bookmark” href=”favicon.ico”>
状态栏连接说明
<A HREF=”链接到某处” onmouseOver=”window.status=’连接说明’;return true;” onMouseOut=”window.status=’ ‘;”>某某链接</a>
链接说明
<a href=“”Title=链接说明>
禁止鼠标右键
在<body>标签中加入 <body oncontextmenu=”return false”>
DW里输入空格
插入N个 
水平线
<hr width=”长度” size=”高度” color=”颜色代码” noshade> noshade为有无阴影
表单电子邮件提交
< form name=”content” method=”post” action=”mailto:电子邮箱” >< /form>
文本域名为Subject 为邮件的标题
邮件链接定制
Mailto:地址 ? Subject=邮件的标题 &bc=抄送 &bcc=密件抄送
背景音乐
<bgsound src=地址 loop=”-1″>
禁止页面正文选取
<body oncontextmenu=”return false” ondragstart=”return false” onselectstart =”return false” onselect=”document.selection.empty()” oncopy=”document.selection.empty()” onbeforecopy=”return false”onmouseup=”document.selection.empty()”>
消除ie6自动出现的图像工具栏，设置 GALLERYIMG属性为false或no .
<IMG SRC=”mypicture.jpg” HEIGHT=”100px” WIDTH=”100px” GALLERYIMG=”no”>
防止点击空链接时，页面往往重置到页首端。
代码“javascript:void(null)”代替原来的“#”标记
如何避免别人把你的网页放在框架中
<script language=“javascript”><!–if (self!=top){top.location=self.location;} –>< /script>
页面定时刷新
<meta http-equiv=”Refresh” content=”秒” >
页面定时转向新的地址
<meta http-equiv=”refresh” content=”秒;URL=url”>
显示日期
<script language=”javascript”><!–
today=new Date();
var week; var date;
if(today.getDay()==0) week=”星期日”
if(today.getDay()==1) week=”星期一”
if(today.getDay()==2) week=”星期二”
if(today.getDay()==3) week=”星期三”
if(today.getDay()==4) week=”星期四”
if(today.getDay()==5) week=”星期五”
if(today.getDay()==6) week=”星期六”
date=(today.getYear())+”年”+(today.getMonth()+1)+”月”+today.getDate()+”日”+” ”
document.write(”<span style=’font-size: 9pt;’>”+date+week+”</span>”);
// –>
</script>
设为首页
<A href=# onclick=”this.style.behavior=’url(#default#homepage)’;this.setHomePage(’url’);”>设为首页</A>
添加收藏
<A href=”javascript:window.external.AddFavorite(’url’,'title’)”>收藏本站</A>
文字滚动
插入边框为0的1行1列的表格，在表格中输入文字，选中文字，
按ctrl+t输入marquee direction=”up”, 回车即可让文字在表格区域内向上滚动。
（right、down可用于让文字或图象向右及向下滚动，修改html原代码还可以得到需要的滚动速度。
表单验正
<SCRIPT language=javascript>
function checkform(theform){
if(theform.name.value==”"){
alert(”姓名不能为空!”);
theform.name.focus();
return false;
}
if(theform.tel.value==”"){
alert(”电话不能为空!”);
theform.tel.focus();
return false;
}
}
</SCRIPT>
定义鼠标
body{cursor: url(cur.ani或cur);}
以图片方式插视频
<IMG height=240 loop=infinite dynsrc=http://amedia.efu.com.cn/EFUADD0001.rmvb width=320>
层在flash上面
< param name=”wmode” value=”opaque” >
延迟跳转
<meta http-equiv=refresh content=’3; url=javascript:window.close();’>
导航条变色：
单元格<TR后面插入onmouseover=”javascript:this.bgColor=’#57AE00′” onmouseout=”javascript:this.bgColor=’#99CCFF’”
居中
<CENTER></CENTER>
空链接
javascript:;
标题表格
<fieldset>
<legend>表格的说明</legend>
</fieldset>
细线表格
style=”BORDER-COLLAPSE: collapse;”
滚动条颜色代码
BODY{
SCROLLBAR-FACE-COLOR: #FFFFFF;
SCROLLBAR-HIGHLIGHT-COLOR: #FFFFFF;
SCROLLBAR-SHADOW-COLOR: #FFFFFF;
SCROLLBAR-3DLIGHT-COLOR: #FFCBC8;
SCROLLBAR-ARROW-COLOR: #FFFFFF;
SCROLLBAR-TRACK-COLOR: #FFFFFF;
SCROLLBAR-DARKSHADOW-COLOR: #FFCBC8;
SCROLLBAR-BASE-COLOR: #FFFFFF
}
连续的英文或者一堆感叹号!!!不会自动换行的问题
只要在CSS中定义了如下句子，可保网页不会再被撑开了
table{table-layout: fixed;}
td{word-break: break-all; word-wrap:break-word;}
注释一下：
1.第一条table{table-layout: fixed;}，此样式可以让表格中有!!!(感叹号)之类的字符时自动换行。
2.td{word-break: break-all}，一般用这句这OK了，但在有些特殊情况下还是会撑开，因此需要再加上后面一句{word-wrap:break-word;}就可以解决。此样式可以让表格中的一些连续的英文单词自动换行。
1、控制横向和纵向滚动条的显隐？
<body style=”overflow-y:hidden”> 去掉x轴
<body style=”overflow-x:hidden”> 去掉y轴
<body scroll=”no”>不显
2、表格变色
<TD onmouseover=”this.style.backgroundColor=’#FFFFFF’”
onmouseout=”this.style.backgroundColor=””
style=”CURSOR: hand”>
3、 禁止复制，鼠标拖动选取
<body ondragstart=window.event.returnValue=false oncontextmenu=window.event.returnValue=false onselectstart=event.returnValue=false>
4、 普通iframe页面
<iframe name=”name” src=”main.htm” width=”450″ height=”287″ scrolling=”Auto” frameborder=”0″></iframe>
5、iframe自适应高度
<iframe name=”pindex” src=”index.asp” frameborder=false scrolling=”auto” width=”100%” height=”100%” frameborder=no onload=”document.all['pindex'].style.height=pindex.document.body.scrollHeight” ></iframe>
6、IE地址栏前换成自己的图标&可以在收藏夹中显示出你的图标
<link rel=”Shortcut Icon” href=”favicon.ico”>
<link rel=”Bookmark” href=”favicon.ico”>
7、字号缩放
越来越多的人长时间的泡网，眼镜的普及率也越来越高，让文字大点，让更多的用户看的更清楚。
<script type=”text/javascript”>
function doZoom(size)
{document.getElementById(’zoom’).style.fontSize=size+’px’;}
</script>
<span id=”zoom”>需要指定大小的文字</span>
<a href=”javascript:doZoom(16)”>大</a> <a href=”javascript:doZoom(14)”>中</a> <a href=”javascript:doZoom(12)”>小</a>
8、select挡住div的解决方法 
在div里加入下面的代码，根据需要调整就可以了。
<iframe src=”javascript:false” scrolling=”no” frameborder=”0″ style=”z-index:-1;position:absolute; top:5px; left:2px;width:168;height:100px;”>
</iframe>
9、iframe(嵌入式帧)自适应高度
填写的嵌入地址一定要和本页面在同一个站点上，否则会提示“拒绝访问！”。对跨域引用有权限问题，请查阅其他资料。
<iframe name=”guestbook” src=”gbook/index.asp” scrolling=no width=”100%” height=”100%” frameborder=no onload=”document.all['guestbook'].style.height=guestbook.document.body.scrollHeight”></iframe>
10、 跳转菜单新窗口
<select name=”select” onchange=”window.open(this.options[this.selectedIndex].value)”>
<option value=”http://www.microsoft.com/ie”> Internet Explorer</option>
<option value=”http://www.microsoft.com”> Microsoft Home</option>
<option value=”http://msdn.microsoft.com”> Developer Network</option>
</select>
11、flash透明选项
<param name=”wmode” value=”transparent”>
12、添加到收藏夹和设为首页
<a href=# onclick=”this.style.behavior=’url(#default#homepage)’; this.setHomePage(’http://www.makewing.com/lanren/’);”>设为首页</a>
<a href=”javascript:window.external.AddFavorite(’http://www.makewing.com/lanren/’,'懒人图库’)”>收藏本站</a>
13、记录并显示网页的最后修改时间 
<script language=JavaScript>
document.write(”最后更新时间: ” + document.lastModified + “”)
</script>
14、节日倒计时
<Script Language=”JavaScript”>
var timedate= new Date(”October 1,2002″);
var times= “国庆节”;
var now = new Date();
var date = timedate.getTime() - now.getTime();
var time = Math.floor(date / (1000 * 60 * 60 * 24));
if (time >= 0)
document.write( “现在离”+times+”还有: “+time +”天”)
</Script>
15、加在HEAD里 
禁止缓存
<meta http-equiv=”Expires” CONTENT=”0″>
<meta http-equiv=”Cache-Control” CONTENT=”no-cache”>
<meta http-equiv=”Pragma” CONTENT=”no-cache”>
16、 让IFRAME框架内的文档的背景透明
<iframe src=”about:<body style=’background:transparent’>” allowtransparency></iframe>
17、打开窗口即最大化
<script language=”JavaScript”>
<!– Begin
self.moveTo(0,0)
self.resizeTo(screen.availWidth,screen.availHeight)
// End –>
</script>
18、加入背景音乐
<bgsound src=”mid/windblue[1].mid” loop=”-1″> 只适用于IE
<embed src=”music.mid” autostart=”true” loop=”true” hidden=”true”> 对Netscape ,IE 都适用
19、 滚动
<marquee direction=up height=146 onmouseout=start() onmouseover=stop() scrollAmount=2>滚动信息
</marquee>
20、防止点击空链接时，页面往往重置到页首端
代码“javascript:void(null)”代替原来的“#”标记
21、文字或图片弹出指定大小的窗口
在body中加入
<script language=”JavaScript” type=”text/JavaScript”>
function MM_openBrWindow(theURL,winName,features) {window.open(theURL,winName,features);}
</script>
弹出代码
<a href=”#” target=”_self” onClick=”MM_openBrWindow(’windows01.htm’,”,’width=550,height=380′)” width=”550″ height=”380″ border=”0″>图片或文字</a>
22、 flash按钮加链接 
on (press) {
getURL(”http://www.makewing.com”,”_blank”);
}
23、跳转页面代码
<meta http-equiv=”refresh” content=”5;url=http://www.makewing.com”>
24、细线分隔线
<hr noshade size=0 color=#C0C0C0>
25、网页中的自动换行
<td style=”word-break:break-all”>就搞定了。
完整的是
style=”table-layout: fixed;WORD-BREAK: break-all; WORD-WRAP: break-word”
26、消除ie6自动出现的图像工具栏，设置 GALLERYIMG属性为false或no . 
<IMG SRC=”mypicture.jpg” HEIGHT=”100px” WIDTH=”100px” GALLERYIMG=”no”>
27、禁止页面正文内容被选取 
<body oncontextmenu=”return false” ondragstart=”return false” onselectstart =”return false” onselect=”document.selection.empty()” oncopy=”document.selection.empty()” onbeforecopy=”return
false”onmouseup=”document.selection.empty()”>
28、不能点右键，不用CTRL+A，不能复制作!
<body oncontextmenu=”window.event.returnValue=false”
onkeypress=”window.event.returnValue=false”
onkeydown=”window.event.returnValue=false”
onkeyup=”window.event.returnValue=false”
ondragstart=”window.event.returnValue=false”
onselectstart=”event.returnValue=false”>
</body>
29、IE浏览器支持一个 Body 属性 bgproperties，它可以让背景不滚动: 
<Body Background=”图片文件” bgproperties=”fixed”>
30、随机变换背景图象（一个可以刷新心情的特效）
<Script Language=”JavaScript”>
image = new Array(4); //定义image为图片数量的数组
image [0] = ‘tu0.gif’ //背景图象的路径
image [1] = ‘tu1.gif’
image [2] = ‘tu2.gif’
image [3] = ‘tu3.gif’
image [4] = ‘tu4.gif’
number = Math.floor(Math.random() * image.length);
document.write(”<BODY BACKGROUND=”+image[number]+”>”);
</Script>
31、flash载入影片 
on (release)
{
loadMovie(”1-01.swf”, “_root.loaderclip”);
}
32、图片表单按钮
<form id=”form1″ name=”form1″ method=”post” action=”">
<img src=”login.gif” width=”62″ height=”22″ onclick=”document.form1.submit()” />
</form>
33、左右阴影背景的CSS定义方法
body {
text-align:center;
background-repeat: repeat-y;
background-position: center;
background-image: url(../images/bg.jpg);
}
34、划过链接 手型鼠标
style=”cursor:hand”
35、如何关闭层
<div id=”Layer1″></div>
<a href=”#” onClick=”Layer1.style.display=’none’”>关闭层</a>
36、关闭窗口的脚本
<a href=javascript:close()>[关闭窗口]</a>
37、如果文字过长,则将过长的部分变成省略号显示 
<DIV STYLE=”width: 120px; height: 50px; border: 1px solid blue;overflow: hidden; text-overflow:ellipsis”>
<NOBR>就是比如有一行文字，很长，表格内一行显示不下.</NOBR>
</DIV>
38、进入主页后自动最大化
<script>
self.moveTo(0,0)
self.resizeTo(screen.availWidth,screen.availHeight)
</script>
39、凹陷文字 
<div style=”width:300px;padding:20px;overflow:hidden;word-wrap:break-word;word-break:break:all; font-size:12px; line-height:18px; background-color:#eeeeee;”>
<font disabled>
怎么样，我凹下去了吧？<br>
你不想试试吗？<br>
<a href=”http://www.lenvo.cn/”>www.lenvo.cn</a></font>
</div>
40、运行代码框
<script>
function Preview()
{var TestWin=open(”);
TestWin.document.write(code.value);}
</script>
<textarea id=code cols=60 rows=15></textarea>
<br>
<button onclick=Preview() >运行</button>
41、给表格做链接 
<table width=”100%” onclick=”window.open(’http://www.makewing.com/’, ‘_blank’)” style=”CURSOR:hand”>
<tr>
<td height=”100″ bgcolor=”f4f4f4″> </td>
</tr>
</table>
42、让弹出窗口总是在最上面
<body onblur=”this.focus();”>
43、CSS文字阴影 
.shadowfont{FILTER: dropshadow(color=#666666, offx=1, offy=1, positive=1); FONT-FAMILY: “宋体”; FONT-SIZE: 9pt;COLOR: #ffffff;}
44、后退&关闭窗口
后退：javascript:history.back(1)
关闭：javascript:window.close();
45、表格透明 
style=”FILTER: alpha(opacity=72)”
46、如何避免别人把你的网页放在框架中
<script language=“javascript”><!–if (self!=top){top.location=self.location;} –>< /script>
47、Alt和Title的区别 
alt 用来给图片来提示的。Title用来给链接文字或普通文字提示的。
<a href=”#” Title=”给链接文字提示”>文字</a>
<p Title=”给链接文字提示”>文字</p>
<img src=”图片链接” alt=”给图片提示”>
48、所有的javascript的调用尽量采取外部调用
<SCRIPT LANGUAGE=”javascript” SRC=”js/xxxxx.js”></SCRIPT>
49、链接到
response.redirect”login.asp”
location.href=”xx.asp”onClick=”window.location=’login.asp’”
onClick=”window.open(”)”
50、基本链接样式
a:link 表示链接的样式
a:active 表示当前活动连接的样式
a:hover 表示鼠标划过时的样式
a:visited 表示已经访问过的连接的样式
“link-visited-hover-active” 简记为 “lvha” 即 “love”+”hate”
51、在同一页面设置不同文字链接效果的样式
<style type=”text/css”>
.green {COLOR: #309633}
.green A:link {COLOR: #309633; TEXT-DECORATION: none}
.green A:visited {COLOR: #309633; TEXT-DECORATION: none}
.green A:hover {COLOR: #309633; TEXT-DECORATION: underline}
.green A:active {COLOR: #309633; TEXT-DECORATION: none}
</style>
<a href=”#” class=”green”>文字</a>
