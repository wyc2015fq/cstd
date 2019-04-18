# window.showModalDialog以及window.open用法简介(转载) - weixin_33985507的博客 - CSDN博客
2019年01月11日 00:18:31[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

# [window.showModalDialog以及window.open用法简介](http://www.cnblogs.com/zhangyi85/archive/2009/09/03/1559594.html)
一、window.open()支持环境： JavaScript1.0+/JScript1.0+/Nav2+/IE3+/Opera3+
二、基本语法：
window.open(pageURL,name,parameters) 
其中：
pageURL 为子窗口路径 
name 为子窗口句柄 
parameters 为窗口参数(各参数用逗号分隔) 
三、示例：
<SCRIPT>
<!--
window.open ('page.html','newwindow','height=100,width=400,top=0,left=0,toolbar=no,menubar=no,scrollbars=no, resizable=no,location=no, status=no') 
//写成一行 
-->
</SCRIPT>
脚本运行后，page.html将在新窗体newwindow中打开，宽为100，高为400，距屏顶0象素，屏左0象素，无工具条，无菜单条，无滚动条，不可调整大小，无地址栏，无状态栏。
请对照。
上例中涉及的为常用的几个参数，除此以外还有很多其他参数，请见四。
四、各项参数
其中yes/no也可使用1/0；pixel value为具体的数值，单位象素。
参数 | 取值范围 | 说明 
||
alwaysLowered | yes/no | 指定窗口隐藏在所有窗口之后 
alwaysRaised | yes/no | 指定窗口悬浮在所有窗口之上 
depended | yes/no | 是否和父窗口同时关闭 
directories | yes/no | Nav2和3的目录栏是否可见 
height | pixel value | 窗口高度 
hotkeys | yes/no | 在没菜单栏的窗口中设安全退出热键 
innerHeight | pixel value | 窗口中文档的像素高度 
innerWidth | pixel value | 窗口中文档的像素宽度 
location | yes/no | 位置栏是否可见 
menubar | yes/no | 菜单栏是否可见 
outerHeight | pixel value | 设定窗口(包括装饰边框)的像素高度 
outerWidth | pixel value | 设定窗口(包括装饰边框)的像素宽度 
resizable | yes/no | 窗口大小是否可调整 
screenX | pixel value | 窗口距屏幕左边界的像素长度 
screenY | pixel value | 窗口距屏幕上边界的像素长度 
scrollbars | yes/no | 窗口是否可有滚动栏 
titlebar | yes/no | 窗口题目栏是否可见 
toolbar | yes/no | 窗口工具栏是否可见 
Width | pixel value | 窗口的像素宽度 
z-look | yes/no | 窗口被激活后是否浮在其它窗口之上
window.showModalDialog使用手册
基本介绍：
showModalDialog() (IE 4+ 支持)
showModelessDialog() (IE 5+ 支持)
window.showModalDialog()方法用来创建一个显示HTML内容的模态对话框。
window.showModelessDialog()方法用来创建一个显示HTML内容的非模态对话框。
使用方法：
vReturnValue = window.showModalDialog(sURL [, vArguments] [,sFeatures])
vReturnValue = window.showModelessDialog(sURL [, vArguments] [,sFeatures])
参数说明：
sURL--
必选参数，类型：字符串。用来指定对话框要显示的文档的URL。
vArguments--
可选参数，类型：变体。用来向对话框传递参数。传递的参数类型不限，包括数组等。对话框通过window.dialogArguments来取得传递进来的参数。
sFeatures--
可选参数，类型：字符串。用来描述对话框的外观等信息，可以使用以下的一个或几个，用分号“;”隔开。
1.dialogHeight :对话框高度，不小于１００px，ＩＥ４中dialogHeight 和 dialogWidth 默认的单位是em，而ＩＥ５中是px，为方便其见，在定义modal方式的对话框时，用px做单位。
2.dialogWidth: 对话框宽度。
3.dialogLeft: 离屏幕左的距离。
4.dialogTop: 离屏幕上的距离。
5.center: {yes | no |1|0 }：窗口是否居中，默认yes，但仍可以指定高度和宽度。
6.help: {yes | no |1|0 }：是否显示帮助按钮，默认yes。
7.resizable: {yes | no |1|0 } ［ＩＥ５＋］：是否可被改变大小。默认no。
8.status: {yes | no |1|0 } ［IE5+］：是否显示状态栏。默认为yes[ Modeless]或no[Modal]。
9.scroll:{ yes | no |1|0| on | off }：指明对话框是否显示滚动条。默认为yes。
下面几个属性是用在HTA中的，在一般的网页中一般不使用。
10.dialogHide:{ yes | no |1|0| on | off }：在打印或者打印预览时对话框是否隐藏。默认为no。
11.edge:{ sunken | raised }：指明对话框的边框样式。默认为raised。
12.unadorned:{ yes | no |1|0| on | off }：默认为no。
参数传递：
1.要想对话框传递参数，是通过vArguments来进行传递的。类型不限制，对于字符串类型，最大为4096个字符。也可以传递对象，例如：
-------------------------------
parent.htm
<script>
var obj =new Object();
obj.name="51js";
window.showModalDialog("modal.htm",obj,"dialogWidth=200px;dialogHeight=100px");
</script>
modal.htm
<script>
var obj = window.dialogArguments
alert("您传递的参数为："+ obj.name)
</script>
-------------------------------
2.可以通过window.returnValue向打开对话框的窗口返回信息，当然也可以是对象。例如：
------------------------------
parent.htm
<script>
str =window.showModalDialog("modal.htm",,"dialogWidth=200px;dialogHeight=100px");
alert(str);
</script>
modal.htm
<script>
window.returnValue="http://www.51js.com";
</script>
币种定义部分 
var psAddStr="ProcessID="+ProcessID+"&AddFlag="+isAddFlag+"&BZBH="+vsBZBH+"&BZMC="+vsBZMC+"&BZFH="+vsBZFH+"&JD="+vsJD;
var Result=window.showModalDialog("addSave.asp?"+psAddStr,'',"dialogHeight:250px;dialogWidth:250px;status:no;");
分类: [SharePoint](http://www.cnblogs.com/zhangyi85/category/195410.html)
一、window.open()支持环境： JavaScript1.0+/JScript1.0+/Nav2+/IE3+/Opera3+
二、基本语法：
window.open(pageURL,name,parameters) 
其中：
pageURL 为子窗口路径 
name 为子窗口句柄 
parameters 为窗口参数(各参数用逗号分隔) 
三、示例：
<SCRIPT>
<!--
window.open ('page.html','newwindow','height=100,width=400,top=0,left=0,toolbar=no,menubar=no,scrollbars=no, resizable=no,location=no, status=no') 
//写成一行 
-->
</SCRIPT>
脚本运行后，page.html将在新窗体newwindow中打开，宽为100，高为400，距屏顶0象素，屏左0象素，无工具条，无菜单条，无滚动条，不可调整大小，无地址栏，无状态栏。
请对照。
上例中涉及的为常用的几个参数，除此以外还有很多其他参数，请见四。
四、各项参数
其中yes/no也可使用1/0；pixel value为具体的数值，单位象素。
参数 | 取值范围 | 说明 
||
alwaysLowered | yes/no | 指定窗口隐藏在所有窗口之后 
alwaysRaised | yes/no | 指定窗口悬浮在所有窗口之上 
depended | yes/no | 是否和父窗口同时关闭 
directories | yes/no | Nav2和3的目录栏是否可见 
height | pixel value | 窗口高度 
hotkeys | yes/no | 在没菜单栏的窗口中设安全退出热键 
innerHeight | pixel value | 窗口中文档的像素高度 
innerWidth | pixel value | 窗口中文档的像素宽度 
location | yes/no | 位置栏是否可见 
menubar | yes/no | 菜单栏是否可见 
outerHeight | pixel value | 设定窗口(包括装饰边框)的像素高度 
outerWidth | pixel value | 设定窗口(包括装饰边框)的像素宽度 
resizable | yes/no | 窗口大小是否可调整 
screenX | pixel value | 窗口距屏幕左边界的像素长度 
screenY | pixel value | 窗口距屏幕上边界的像素长度 
scrollbars | yes/no | 窗口是否可有滚动栏 
titlebar | yes/no | 窗口题目栏是否可见 
toolbar | yes/no | 窗口工具栏是否可见 
Width | pixel value | 窗口的像素宽度 
z-look | yes/no | 窗口被激活后是否浮在其它窗口之上
window.showModalDialog使用手册
基本介绍：
showModalDialog() (IE 4+ 支持)
showModelessDialog() (IE 5+ 支持)
window.showModalDialog()方法用来创建一个显示HTML内容的模态对话框。
window.showModelessDialog()方法用来创建一个显示HTML内容的非模态对话框。
使用方法：
vReturnValue = window.showModalDialog(sURL [, vArguments] [,sFeatures])
vReturnValue = window.showModelessDialog(sURL [, vArguments] [,sFeatures])
参数说明：
sURL--
必选参数，类型：字符串。用来指定对话框要显示的文档的URL。
vArguments--
可选参数，类型：变体。用来向对话框传递参数。传递的参数类型不限，包括数组等。对话框通过window.dialogArguments来取得传递进来的参数。
sFeatures--
可选参数，类型：字符串。用来描述对话框的外观等信息，可以使用以下的一个或几个，用分号“;”隔开。
1.dialogHeight :对话框高度，不小于１００px，ＩＥ４中dialogHeight 和 dialogWidth 默认的单位是em，而ＩＥ５中是px，为方便其见，在定义modal方式的对话框时，用px做单位。
2.dialogWidth: 对话框宽度。
3.dialogLeft: 离屏幕左的距离。
4.dialogTop: 离屏幕上的距离。
5.center: {yes | no |1|0 }：窗口是否居中，默认yes，但仍可以指定高度和宽度。
6.help: {yes | no |1|0 }：是否显示帮助按钮，默认yes。
7.resizable: {yes | no |1|0 } ［ＩＥ５＋］：是否可被改变大小。默认no。
8.status: {yes | no |1|0 } ［IE5+］：是否显示状态栏。默认为yes[ Modeless]或no[Modal]。
9.scroll:{ yes | no |1|0| on | off }：指明对话框是否显示滚动条。默认为yes。
下面几个属性是用在HTA中的，在一般的网页中一般不使用。
10.dialogHide:{ yes | no |1|0| on | off }：在打印或者打印预览时对话框是否隐藏。默认为no。
11.edge:{ sunken | raised }：指明对话框的边框样式。默认为raised。
12.unadorned:{ yes | no |1|0| on | off }：默认为no。
参数传递：
1.要想对话框传递参数，是通过vArguments来进行传递的。类型不限制，对于字符串类型，最大为4096个字符。也可以传递对象，例如：
-------------------------------
parent.htm
<script>
var obj =new Object();
obj.name="51js";
window.showModalDialog("modal.htm",obj,"dialogWidth=200px;dialogHeight=100px");
</script>
modal.htm
<script>
var obj = window.dialogArguments
alert("您传递的参数为："+ obj.name)
</script>
-------------------------------
2.可以通过window.returnValue向打开对话框的窗口返回信息，当然也可以是对象。例如：
------------------------------
parent.htm
<script>
str =window.showModalDialog("modal.htm",,"dialogWidth=200px;dialogHeight=100px");
alert(str);
</script>
modal.htm
<script>
window.returnValue="http://www.51js.com";
</script>
币种定义部分 
var psAddStr="ProcessID="+ProcessID+"&AddFlag="+isAddFlag+"&BZBH="+vsBZBH+"&BZMC="+vsBZMC+"&BZFH="+vsBZFH+"&JD="+vsJD;
var Result=window.showModalDialog("addSave.asp?"+psAddStr,'',"dialogHeight:250px;dialogWidth:250px;status:no;");
分类: [SharePoint](http://www.cnblogs.com/zhangyi85/category/195410.html)
