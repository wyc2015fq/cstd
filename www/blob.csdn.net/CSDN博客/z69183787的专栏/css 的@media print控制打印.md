# css 的@media print控制打印 - z69183787的专栏 - CSDN博客
2013年12月13日 10:25:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5328
@media版本：CSS2　   兼容性：IE5+ 
语法：   
@media     sMedia   {   sRules   }   
说明：   
sMedia   :   　指定设备名称。请参阅附录：设备类型   
sRules   :   　样式表定义   
指定样式表规则用于指定的设备类型。请参阅link对象的media属性（特性）。   
示例：   
//   设置显示器用字体尺寸   
@media   screen   {   
BODY   {font-size:12pt;   }   
}   
//   设置打印机用字体尺寸   
@media   print   {   
@import   "print.css "   
BODY   {font-size:8pt;}   
}  
Media   Types         设备类型 
Media   Type 
设备类型   CSS   Version 
版本   Compatibility 
兼容性   Description 
简介   
all   CSS2   IE4+   用于所有设备类型   
aural   CSS2   NONE   用于语音和音乐合成器     
braille   CSS2   NONE   用于触觉反馈设备   
embossed   CSS2   NONE   用于凸点字符（盲文）印刷设备   
handheld   CSS2   NONE   用于小型或手提设备     
print   CSS2   IE4+   用于打印机   
projection   CSS2   NONE   用于投影图像，如幻灯片   
screen   CSS2   IE4+   用于计算机显示器   
tty   CSS2   NONE   用于使用固定间距字符格的设备。如电传打字机和终端   
tv   CSS2   NONE   用于电视类设备
- <style>     
- @media print {   
- .noprint {    
- display: none    
- }   
- }   
- </style>  
- 这里是打需要打印的内容.**<span**class="noprint"**>**我只显示不打印哦，很神奇吧。哈哈**</span>**
- **<div**class="noprint"**>**
- **<table**style="margin:0 auto;width:500px;"**>**
- **<tr**align="center"**><td>**
- **<object**id="WebBrowser"classid="CLSID:8856F961-340A-11D0-A96B-00C04FD705A2"height="0"
- width="0"**>**
- **</object>**
- **<input**type="button"value="打印"onclick="document.all.WebBrowser.ExecWB(6,1)"**>**
- **<input**type="button"value="页面设置"onclick="document.all.WebBrowser.ExecWB(8,1)"**>**
- **<input**type="button"value="直接打印"onclick="document.all.WebBrowser.ExecWB(6,6)"**>**
- **<input**type="button"value="打印预览"onclick="document.all.WebBrowser.ExecWB(7,1)"**>**
- **</td></tr>**
- **</table></div>**
