# JS打印 - z69183787的专栏 - CSDN博客
2013年07月16日 15:24:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：895
Javascript网页打印大全
普通打印（整页打）      打印网页内部分内容（自定义）        打印去掉/添加页眉页脚            使用外部控件/方法实现多功能打印            打印背景
以上为代码控制
设置“页面设置”实现打印参数设置（Window系统图文版）
一、普通打印（整页打）
这个不用多说，直接用
引用:window.print();
二、打印网页内部分内容（自定义）
分三种方法实现
1、用css控制
引用:@media print
.a {display:block}
.b {display:hidden}
把你不想打印的部分class设为b
首先在网页中添加：
引用:<OBJECT id="WebBrowser" height="0" width="0" classid="CLSID:8856F961-340A-11D0-A96B-00C04FD705A2"
      VIEWASTEXT>
      </OBJECT>
然后就可以依次加入功能按钮了：
引用:<input  type="button" value="打印"> <input  type="button" value="直接打印">
      <input  type="button" value="页面设置">
      <input  type="button" value="打印预览"> <INPUT type="button" value="关闭窗口" >
将这两块东西放到<center class=noprint></center>就不会打印这些按钮了。当然要定义noprint了：
<style media="print">.Noprint { DISPLAY: none }</style>只要把不想打印的东西的css设置成noprint就可以了。
现在就实现了基本的web打印，需要注意的情况如下：
    a. 必须将ie的internet选项的安全设置中对于没有标记为安全的ActiveX控件进行...设置成提示或者启用，否则会报错，导致不可用。 
    b. 如果在vs.net编辑环境下编辑该页面，它经常自动的给object添加多余的参数，有了这些东西，打印就会出错，所以要记得最后保存的时候删除它们。
2、用javascript打印固定标签内的内容
a、在页面的代码头部处加入JavaScript：
引用:<script language=javascript>
function doPrint() { 
bdhtml=window.document.body.innerHTML; 
sprnstr="<!--startprint-->"; 
eprnstr="<!--endprint-->"; 
prnhtml=bdhtml.substr(bdhtml.indexOf(sprnstr)+17); 
prnhtml=prnhtml.substring(0,prnhtml.indexOf(eprnstr)); 
window.document.body.innerHTML=prnhtml; 
window.print(); 
}
</script>
2、在页面正文处加上<!--startprint-->与<!--endprint-->标识。
也就是在需要用户打印保存的正文所对应的html处附加上。同时，如果采用小偷程序获得远程数据并需打印，可将此等数据置于该定义标签之内即可。
3、截取内容部分已完成，现在加个“打印”的链接：
XML/HTML代码 
<a href="javascript:;" onClick="doPrint()">打印</a>
引用:
<script language="JavaScript">
var hkey_root,hkey_path,hkey_key
hkey_root="HKEY_CURRENT_USER"
hkey_path="\\Software\\Microsoft\\Internet Explorer\\PageSetup\\"
//设置网页打印的页眉页脚为空
function pagesetup_null(){
try{
var RegWsh = new ActiveXObject("WScript.Shell")
hkey_key="header" 
RegWsh.RegWrite(hkey_root+hkey_path+hkey_key,"")
hkey_key="footer"
RegWsh.RegWrite(hkey_root+hkey_path+hkey_key,"")
}catch(e){}
}
//设置网页打印的页眉页脚为默认值
function pagesetup_default(){
try{
var RegWsh = new ActiveXObject("WScript.Shell")
hkey_key="header" 
RegWsh.RegWrite(hkey_root+hkey_path+hkey_key,"&w&b页码，&p/&P")hkey_key="footer"
RegWsh.RegWrite(hkey_root+hkey_path+hkey_key,"&u&b&d")
}catch(e){}
}
</script>
<input type="button" value="清空页码" onclick=pagesetup_null()>
<input type="button" value="恢复页码" onclick=pagesetup_default()>
复制出去，看下效果就可以了
（四）使用外部控件/方法实现多功能打印
1、IEWebBrowser组件
介绍
http://support.microsoft.com/default.aspx?scid=kb%3BEN-US%3BQ267240#top 
http://support.microsoft.com/kb/q247671/#appliesto
代码
引用:<OBJECT classid=CLSID:8856F961-340A-11D0-A96B-00C04FD705A2 height=0 id=WebBrowser width=0></OBJECT>    
<input name=Button onClick=document.all.WebBrowser.ExecWB(1,1) type=button value=打开>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(2,1) type=button value=关闭所有>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(4,1) type=button value=另存为>    
<input name=Button onClick=document.all.WebBrowser.ExecWB(6,1) type=button value=打印>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(6,6) type=button value=直接打印>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(7,1) type=button value=打印预览>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(8,1) type=button value=页面设置>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(10,1) type=button value=属性>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(17,1) type=button value=全选>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(22,1) type=button value=刷新>  
<input name=Button onClick=document.all.WebBrowser.ExecWB(45,1) type=button value=关闭> 
2、使用ScriptX.cab控件
1．下载ScriptX.cab控件
官网http://www.meadroid.com/scriptx/index.asp
2．使用object元素，修改codebase，classid的值
这里调用控件ScriptX.cab
代码
引用:
<OBJECT id="factory" style="DISPLAY: none" codeBase="${rootUrl}js/smsx.cab#VVersion=6,3,435,20"  classid="clsid:1663ed61-23eb-11d2-b92f-008048fdd814" viewastext></OBJECT> 
这段代码用来加载cab文件，clsid和codebase必须要和你下载的cab中的信息对应，否则组件会加载错误，这两项其实不难找，只要你用winrar打开你下载的cab文件，然后找到扩展名是.inf的文件，然后打开之，就能看到了。
3．调用控件脚本
Print.js文件 
代码
引用:
function setPrintBase(headerText,footerText,rootUrl) {  
    // -- advanced features  ，未曾使用过，有待确认。  
        //factory.printing.SetMarginMeasure(2); // measure margins in inches  
        //factory.SetPageRange(false, 1, 3);// need pages from 1 to 3  
        //factory.printing.printer = "HP DeskJet 870C";  
        //factory.printing.copies = 2;  
        //factory.printing.collate = true;  
        //factory.printing.paperSize = "A4";  
        //factory.printing.paperSource = "Manual feed"  
    var header = (headerText==null||headerText=="")?'默认页眉':headerText;  
    var footer = (footerText==null||footerText=="")?'默认页角':footerText;  
  factory.printing.header = "&b"+header+"&b" ;  
  factory.printing.footer = "&b"+footer;  
  factory.printing.portrait = true;  
  factory.printing.leftMargin =10.00;  
  factory.printing.topMargin =10.00;  
  factory.printing.rightMargin =10.00;  
  factory.printing.bottomMargin =10.00;  
} 
例子
引用:
<html>  
<head>  
<meta http-equiv="imagetoolbar" c>  
<script language="javascript" src="print.js"></script>  
<style media="print">  
.Noprint  {DISPLAY:  none;}  
</style>  
<title>打印测试</title>  
</head>  
<OBJECT id="factory" style="DISPLAY: none" codeBase="smsx.cab#VVersion=6,3,435,20"  classid="clsid:1663ed61-23eb-11d2-b92f-008048fdd814" viewastext></OBJECT>  
<script defer>  
function window.onload() {      
setPrintBase('页眉','页脚');  
}  
</script>  
<body topmargin="0" leftmargin="0" rightmargin="0" bottommargin="0" marginwidth="0" marginheight="0">  
<center class="Noprint">  
<input type=button value="打印" >    
<input type=button value="页面设置" >    
<input type=button value="打印预览" >              
<input type="button" value="关闭" >  
</center>  
  <center>  
      <table width="100%" border="0" cellpadding="0" cellspacing="0">  
          <tr><td align="center"><b>内容</b></td></tr>  
      </table>  
    </center>  
</body>  
</html> 
（五）打印背景
默认情况下，使用IE浏览器打印网页时，不打印网页背景。如果需要打印网页背景，可以采用如下的方法设置：
　　1. 启动IE浏览器。
　　2. 选择【工具】菜单中的【Internet选项】菜单项，打开【Internet选项】对话框。
　　3. 打开【高级】选项卡，在【打印】选区中选择【打印背景颜色和图像】复选框。
　　4. 单击【确定】按钮，关闭对话框。
（六）如何用IE打印网页
    Internet Explorer（简称IE）5.0以上版本强大的打印及打印预览功能。本文就向大家介绍如何用IE来打印网页。（谁？谁？谁在扔西红柿？下面一片嘘声：这还用得着你讲呀？！）咳咳咳，是的是的，在IE中打印网页实在是太简单不过了。只需点一下工具栏上“打印”按钮就可将当前网页打印出来。如下图所示：
    不过，你这样打出来的效果怎么样呢？我就不敢说了。我今天要和大家讲的就是一些打印方面的设置参数。如何大家把这些参数设置好，你就会发现IE的打印功能快追得上Word了。今后你打印网页时就可以想怎么打就怎么打了。好，闲话少说，现在就跟我来一起看看怎么样设置IE的打印参数。
    我们以下图要打印的学生名单（《学生学籍管理系统MX》查询结果网页）为例，讲解如何设置这些打印参数：
    假如我们直接点工具栏的“打印”按钮或点“文件”菜单中的“打印”进行打印这个网页的话，就会打印出很多不必要的信息。所以在打印前我们应先用IE5新增的“打印预览”功能查看当前网页的实际打印效果。如下图所示：
    在如下图所示的“打印预览”对话框中，我们可以看到，网页标题、网页的URL地址及打印日期等我们所不需要的信息也被打印出来了。而页码又在右上角，能不能把它移到其他位置（如我们常见的右下角）呢？
    能不能按照我们所想要的样式来打印网页呢？当然可以。一切就在“页面设置”中。
    弹出如下图所示的“页面设置”对话框，所有的秘密都在这里！
    原来，IE自动给我们在页眉和页脚处加上了这些不必要的打印信息。如果我们不想要任何页眉和页脚的话，直接删除它们就行了。：-）大部分时候我都是这样做的。但如果你想自定义页眉和页脚的时候，该怎么做呢？上面的那些“&w&b&p”等等究竟表示什么意思呀？给你个表格就知道了。 含义  
&w 网页标题 
&u 网页地址 (URL) 
&d 短日期格式（由“控制面板”中的“区域设置”指定） 
&D 长日期格式（由“控制面板”中的“区域设置”指定） 
&t 由“控制面板”中的“区域设置”指定的时间格式 
&T 24 小时时间格式 
&p 当前页码 
&P 总页数       
&b 文本右对齐（请把要右对齐的文字放在“&b”之后） 
&b&b 文字居中（请把要居中的文字放在“&b”和“&b” 之间） 
&& 单个 & 号 (&) 
注：1、这些符号可以与文字组合使用，如本教程中的“页码，&p/&P”。 
    2、页眉和页脚默认是左对齐的，所以IE只提供了右对齐和居中的设置符号。
    3、推荐给大家一种设置方法：页眉为空，页脚设为“&b第 &p 页 / 共 &P 页&b”，打印效果为在页脚居中显示“第 1 页 / 共 4 页”的效果。
    知道了上面的知识，你就可以打印了。但有时你会发现，网页中的一些图像在打印时不见了。Why？为什么会这样？别着急，IE的默认设置是不打印网页的背景颜色和图像的。您只需进入IE的Internet选项将它选中（打上勾）就行了。具体操作方法是：点IE的主菜单中的“工具”——“Internet选项”——再在弹出的对话框中占击“高级”选项卡，找到“打印背景颜色和图像”一项。如下图所示：
    然后将上图中红色圈住的选项打上勾，再点“确定”就OK了。
    至此，我们就完成了对IE的打印设置。接下来，只要我们没有关闭IE，我们就可以一直使用上述设置（最后一项“打印背景颜色和图像”的更改始终有效）进行打印。打印前一般先进行打印预览，效果满意后再开始打印。
3、windows自带功能
按住鼠标的左键，将你想要打印的内容选定，然后单击右键选择“打印”，在弹出的打印对话框中的“页面范围”中选择“选定范围”就可以只打印你选择的内容了。
