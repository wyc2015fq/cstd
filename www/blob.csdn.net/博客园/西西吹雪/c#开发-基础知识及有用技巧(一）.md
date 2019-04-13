
# c\#开发-基础知识及有用技巧(一） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[c\#开发-基础知识及有用技巧(一）](https://www.cnblogs.com/watsonyin/archive/2006/09/14/503987.html)
1、时间长度的计算 TimeSpan类。
例如：TimeSpan span = dateTime1 - dateTime2   方便啊
2、从类（Class)返回一个System.Type类型，用typeof关键字
3、从一个对象实例(Object)返回一个System.Type类型，用GetType方法
4、判断是否处于设计状态：DesignMode属性
5、根据GUID创建对象实例
System.Guid pGuid=newGuid(guid);
System.Type ObjectCustorm=Type.GetTypeFromCLSID(pGuid);
Object obj=Activator.CreateInstance(ObjectCustorm);
6、GDI+不支持xor绘制模式的近似解决方法：
ControlPaint.DrawReversibleFrame、ControlPaint.DrawReversibleLine方法
7、获取Enum类型中的所有枚举值：
Enum.GetNames方法
将字符串转换成枚举值
Enum.Parse方法
8、Label放在图片上时，使Label透明
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)picLogo.Controls.Add(lblStatus);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)lblStatus.BackColor=Color.Transparent;
9、调用帮助文件
打开帮助文件
Help.ShowHelp(this,@"c:/windows/help/mspaint.chm");
打开帮助文件，并跳转到指定的主题
Help.ShowHelp(this,@"c:/windows/help/mspaint.chm","paint_lines.htm");
打开帮助文件，并转到“索引”选项卡
Help.ShowHelpIndex(this,@"c:/windows/help/mspaint.chm","paint_lines.htm");
在屏幕上显示一条浮动的帮助信息
Help.ShowPopup(this,"这里是帮助信息",new Point(100,100));
10、通过AppDomain在应用程序之间传递数据
例如，两个系统可能会共用登录信息，登录一个系统后，再启动另一个系统时，不需要重新登录。
先定义一个在应用程序之间传递的数据的类，该类必须从MarshalByRefObject继承:
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)/**////<summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)///用于在不同的appdomain之间传递参数
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)///</summary>
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclassAppDomainInfo:MarshalByRefObject
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicintUserID;
![](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
然后可以这样打开新的应用程序
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)AppDomainSetup setup=newAppDomainSetup();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)setup.ApplicationName="测试程序";
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)AppDomain appDomain=AppDomain.CreateDomain("TestDomain",null, setup);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)AppDomainInfo domainInfo=newAppDomainInfo();
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)domainInfo.UserID=Winsharp.BaseClass.AppConfigInfo.UserID;
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)appDomain.SetData("domainInfo",domainInfo);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)objectobj=appDomain.CreateInstanceFromAndUnwrap(str,"TestDomain.Test");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)(objasForm).Show();
11、换行字符串，相当于"\r\n"，Environment.NewLine
API中有GetTickCount函数，C\#中为Environment.TickCount
12、取得安装操作系统输入的用户姓名和公司名称：
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)Microsoft.Win32.RegistryKey   cmicRegKey=Microsoft.Win32.Registry.CurrentUser.OpenSubKey("Software");
cmicRegKey=cmicRegKey.OpenSubKey("Microsoft");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)cmicRegKey=cmicRegKey.OpenSubKey("MS Setup (ACME)");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)cmicRegKey=cmicRegKey.OpenSubKey("User Info");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)objectcmicCompany=cmicRegKey.GetValue("DefCompany");
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)objectcmicUser=cmicRegKey.GetValue("DefName");
13、C\# WinForm 捕获最小化事件（来自Limon Tea的随笔[http://limon7.cnblogs.com/archive/2006/07/23/457865.html](http://limon7.cnblogs.com/archive/2006/07/23/457865.html)）
虽然Form类没有提供Minimize的事件，但还是可以通过重载Deactive来实现
当Form失去焦点后，测试WindowState取得Form状态，若为Minimized既是最小化事件。
本例为最小化后隐藏窗口：
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)privatevoidForm1_Deactivate(objectsender, EventArgs e)
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)[        ](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(this.WindowState==FormWindowState.Minimized)
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.Visible=false;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
还有种方法更加直接，重载WndProc：
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)constintWM_SYSCOMMAND=0x112;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)constintSC_CLOSE=0xF060;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)constintSC_MINIMIZE=0xF020;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)constintSC_MAXIMIZE=0xF030;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/None.gif)protectedoverridevoidWndProc(refMessage m)
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m.Msg==WM_SYSCOMMAND)
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[    ](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(m.WParam.ToInt32()==SC_MINIMIZE)
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)[        ](https://limon7.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](https://www.cnblogs.com/Images/dot.gif){
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)this.Visible=false;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return;
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](https://limon7.cnblogs.com/Images/OutliningIndicators/InBlock.gif)base.WndProc(refm);
![](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
[
](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)
14、FromBase64String的问题
在使用Convert.ToBase64String()对字符串进行Base64编码时，注意的几点：
例：string s = "Hello";
byte[] bytes = Convert.FromBase64String(s);
以上代码在运行时会抛出FormatException异常.提示为：Base-64字符数组的无效长度
原因：当Convert.FromBase64String方法的参数s的长度小于 4 或不是 4 的偶数倍时，将会抛出FormatException。
例:
Convert.FromBase64String("Hell");      // Normal.
Convert.FromBase64String("Hell ");     // Normal.（忽略空格）
Convert.FromBase64String("Hello!");    // throw FormatException.
Convert.FromBase64String("Hello Net"); // Normal.（忽略空格）
15、避免程序重复运行。（即只允许运行一个实例）
System.Threading.Mutex appSingleton=newSystem.Threading.Mutex(false,"MyProgInstance_PPP");
if(appSingleton.WaitOne(0,false))
{
Application.Run(newFormMain(););
}
else
{
MessageBox.Show("程序已经运行");
}
16、VB中的chr和asc函数在C\#中没有，C\#中只要用Convert类中的函数进行转换就可以了，如：
int n = Convert.ToInt32('a');
char c = Convert.ToChar(99);
另外，空的char值的获得：Convert.ToChar(0) 例如，在设置了一个textBox的PasswordChar属性后，要清除它，就只能这样了：textBox1.PasswordChar = Convert.ToChar(0) ;
17、C\#的String.Format举例（[http://blog.sina.com.cn/u/4a99b1ba010005ax](http://blog.sina.com.cn/u/4a99b1ba010005ax)）
|
stringstr1 = String.Format("{0:N1}",56789);//result: 56,789.0
stringstr2 = String.Format("{0:N2}",56789);//result: 56,789.00
stringstr3 = String.Format("{0:N3}",56789);//result: 56,789.000
stringstr8 = String.Format("{0:F1}",56789);//result: 56789.0
stringstr9 = String.Format("{0:F2}",56789);//result: 56789.00
stringstr11 =(56789 / 100.0).ToString("\#.\#\#");//result: 567.89
stringstr12 =(56789 / 100).ToString("\#.\#\#");//result: 567
字符说明示例输出|
C 或 c
货币
Console.Write("{0:C}", 2.5);
Console.Write("{0:C}", -2.5);
$2.50
($2.50)
|
D 或 d
十进制数
Console.Write("{0:D5}", 25);
00025
|
E 或 e
科学型
Console.Write("{0:E}", 250000);
2.500000E+005
|
F 或 f
固定点
Console.Write("{0:F2}", 25);
Console.Write("{0:F0}", 25);
25.00
25
|
G 或 g
常规
Console.Write("{0:G}", 2.5);
2.5
|
N 或 n
数字
Console.Write("{0:N}", 2500000);
2,500,000.00
|
X 或 x
十六进制
Console.Write("{0:X}", 250);
Console.Write("{0:X}", 0xffff);
FA
FFFF
|
|
|
|
C\#的String.Format举例
|
|
stringstr1 = String.Format("{0:N1}",56789);//result: 56,789.0
stringstr2 = String.Format("{0:N2}",56789);//result: 56,789.00
stringstr3 = String.Format("{0:N3}",56789);//result: 56,789.000
stringstr8 = String.Format("{0:F1}",56789);//result: 56789.0
stringstr9 = String.Format("{0:F2}",56789);//result: 56789.00
stringstr11 =(56789 / 100.0).ToString("\#.\#\#");//result: 567.89
stringstr12 =(56789 / 100).ToString("\#.\#\#");//result: 567
字符说明示例输出|
C 或 c
货币
Console.Write("{0:C}", 2.5);
Console.Write("{0:C}", -2.5);
$2.50
($2.50)
|
D 或 d
十进制数
Console.Write("{0:D5}", 25);
00025
|
E 或 e
科学型
Console.Write("{0:E}", 250000);
2.500000E+005
|
F 或 f
固定点
Console.Write("{0:F2}", 25);
Console.Write("{0:F0}", 25);
25.00
25
|
G 或 g
常规
Console.Write("{0:G}", 2.5);
2.5
|
N 或 n
数字
Console.Write("{0:N}", 2500000);
2,500,000.00
|
X 或 x
十六进制
Console.Write("{0:X}", 250);
Console.Write("{0:X}", 0xffff);
FA
FFFF
|
|
|
18、bytes[]和int的相互转换
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)ints=100;
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)byte[]   shi=System.BitConverter.GetBytes(s);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)intsh=System.BitConverter.ToInt32(shi,0);
19、BitArray和int的相互转换
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)int[]   a=newint[1];
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)a[0]=100;
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)int[]   b=newint[1];
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)System.Collections.BitArray   shit=newBitArray(a);
![](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)shit.CopyTo(b,0);
20、随机数：
1) System.Random
2) xdev所提到的System.Security.Cryptography.RNGCryptoServiceProvider
3) 直接调用Platform SDK中的CryptGenRandom()函数。
其中，和1)相比，2)和3)可以被作为真正的随机数来使用。
21、最简单的实现双缓冲绘图：
this.SetStyle(ControlStyles.DoubleBuffer |
ControlStyles.UserPaint |
ControlStyles.AllPaintingInWmPaint,
true);
this.UpdateStyles();
还有一种方法，是先建一个bmp，在该bmp上绘制完毕后，最后一次性复制到目标画布上。但在实际使用中，我发现该方法有时反倒闪烁得厉害，不知是为什么，郁闷~
22、取得局域网内本机IP地址
String name = Dns.GetHostName();
IPHostEntry ips = Dns.GetHostByName(name);
return ips.AddressList[0].ToString();
23、取得局域网广播地址
IPv4广播地址的定义是网络号码与网络掩码逻辑“非”之间的逻辑“或”。
例如一个地址为：197.8.43.211,其子网掩码为255.255.255.240则其网络号码为两者的二进制逻辑"与",即197.8.43.208 网络掩码的逻辑"非"为0.0.0.15,与网络号码197.8.43.208二进制逻辑"或"的结果为197.8.43.223,即该子网的广播地址.
24、Asp.net直接下载文件
FileStream fileStream=newFileStream(pFileName,FileMode.Open)
longfileSize=fileStream.Length;
Context.Response.ContentType="application/octet-stream";
Context.Response.AddHeader("Content-Disposition","attachment; filename=\""+ fileName +"\";");
Context.Response.AddHeader("Content-Length",fileSize.ToString());
byte[] fileBuffer=newbyte[fileSize];
fileStream.Read(fileBuffer,0, (int)fileSize);
Context.Response.BinaryWrite(fileBuffer);
Context.Response.End();
25、十进制数转成十六进制字符串
int num =123;
string str = Convert.ToString(num,16);
十六进制字符串转十进制数
string str = "7b";
int num = Convert.ToInt32(str,16);
其它进制也是一样，只要把16改进2或8或10就可以了.
26、在网页上实现html编辑的最简单代码（来自微软网站）
<html>
<head>
<title>DHTML 编辑控件代理示例</title>
<scripttype="text/javascript">
functionFillEditor() {
vardoc=editor.document;
doc.designMode="on";
doc.write("<body><p><i>可视化</i> <u>安静</u></p></body>");
doc.close();
}
</script>
</head>
<bodyonload="FillEditor()">
<iframeid="editor"scrolling="yes"height="100"width="300">
</iframe>
<p>
<inputtype=button
value="Submit"
onclick=
"alert(editor.document.getElementsByTagName('HTML')[0].outerHTML)">
</body>
</html>
此页面显示了包括格式化 HTML 的可编辑区域。您可在此区域中输入内容，也可使用热键，如 CTRL+I 切换斜体，CTRL+U 切换下划线。当按下 Submit 按钮时，出现一个消息框，显示 <iframe> 元素的 HTML 内容。要等效地实现 DHTML 编辑控件的属性、方法和事件，必须要使用脚本。
出于安全考虑，只有来自相同域的内容才可跨过 <iframe> 界限进行访问。要确保框架的内容可被访问，必须使用 src 属性，从同一域的 URL 中对其进行初始化，正如下列示例所示：
<iframe id="editor"
scrolling="yes"
height="100"
width="300"
src="templates/blank.htm">
</iframe>有关在 Internet Explorer 中使用 designMode 属性进行编辑的详细信息，请参阅 Microsoft 网站上的 Introduction to MSHTML Editing（英文）页面。
27、强制类型转换与as类型转换的区别
当类型转换非法时，强制类型转换将抛出一个System.InvalidCastException异常，而as不会抛出异常，它返回一个null值。
28、FromBase64String(string s)函数，对s有如下要求，否则会报错“Base-64字符中的无效字符”：
s   由基   64   数字、空白字符和尾随填充字符组成。从零开始以升序排列的以   64   为基的数字为大写字符“A”到“Z”、小写字符“a”到“z”、数字“0”到“9”以及符号“+”和“/”。   空白字符为   Tab、空格、回车和换行。s   中可以出现任意数目的空白字符，因为所有空白字符都将被忽略。  无值字符“=”用于尾部的空白。s   的末尾可以包含零个、一个或两个填充字符。
29、这一条不是C\#的，是数据库的，有用，收藏一下~
几个删除重复记录的sql语句
(1)用rowid方法据据oracle带的rowid属性，进行判断，是否存在重复,语句如下：
查数据:
select * from table1 a where rowid !=(select  max(rowid)
from table1 b where a.name1=b.name1 and a.name2=b.name2......)
删数据：
delete  from table1 a where rowid !=(select  max(rowid)
from table1 b where a.name1=b.name1 and a.name2=b.name2......)
(2)group by方法
查数据:
select count(num), max(name) from student --列出重复的记录数，并列出他的name属性
group by num
having count(num) >1 --按num分组后找出表中num列重复，即出现次数大于一次
删数据：
delete from student
group by num
having count(num) >1
这样的话就把所有重复的都删除了。
(3)用distinct方法 -对于小的表比较有用
create table table_new as  select distinct *  from table1 minux
truncate table table1;
insert into table1 select * from table_new;

随时补充吧

[
](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)
[
](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)


[

		](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)posted on2006-09-14 11:04[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=503987)[收藏](#)
[
	](https://limon7.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)

[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
