# ASP网站远程客户实现EXCEL打印功能 - 深之JohnChen的专栏 - CSDN博客

2009年02月24日 19:49:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：964


ASP网站远程客户实现EXCEL打印功能

在进行ASP网站开发时，有时需在客户端调用MSSQL数据库的数据进行打印，若调用数据量小，可以通过在客户端运用FileSystemObject生成文件对象的方法实现打印，这里不再赘述。若需调用大量数据，可在客户端脚本中实例化RDS.DataSpace（Remote Data Service）对象,并采用远程提供程序通过ASP网站访问MSSQL数据库（设置成只能通过RDS Default Handler或自定义商业对象才能访问数据库，可保证数据库的安全），再在客户端实例化EXCEL.APPLICATION对象，把数据集中的数据写入EXCEL中，再进行保存或打印。代码如下： 

＜html＞ 

＜head＞ 

＜META content="text/html; charset=gb2312" http-equiv=Content-Type＞ 

＜title＞客户端电子表格打印＜/title＞ 

＜/head＞ 

＜body bgColor=skyblue topMargin=5 leftMargin="20" oncontextmenu="return false" rightMargin=0 bottomMargin="0"＞ 

＜div align="center"＞＜center＞ 

＜table border="1" bgcolor="#ffe4b5" style="HEIGHT: 1px; TOP: 0px" bordercolor="#0000ff"＞ 

＜tr＞ 

＜td align="middle" bgcolor="#ffffff" bordercolor="#000080"＞ 

＜font color="#000080" size="3"＞ 

客户端电子表格打印 

＜/font＞ 

＜/td＞ 

＜/tr＞ 

＜/table＞ 

＜/div＞ 

＜form name="myform"＞ 

＜DIV align=left＞ 

＜input type="button" value="Excel Report" name="report" language="vbscript" onclick="fun_excel（）" style="HEIGHT: 32px; WIDTH: 90px"＞ 

＜/div＞ 

＜/form＞ 

＜/body＞ 

＜/html＞ 

＜script language="vbscript"＞ 

sub fun_excel（） 

Dim rds,rs,df 

dim strCn,strSQL,StrRs 

Dim xlApp, xlBook, xlSheet1 

set rds = CreateObject（"RDS.DataSpace"） 

Set df = rds.CreateObject（"RDSServer.DataFactory","http://192.168.0.1"） '192.168.0.1 为WEB服务器IP地址 

strcn="provider=ms remote;remote server=http://192.168.0.1;handler=msdfmap.handler;data source=pubsdatabase;" '192.168.0.1 为WEB服务器IP地址 

strsql= "getalljobs" 

Set rs = df.Query（strCn, strSQL） 

Set xlApp = CreateObject（"EXCEL.APPLICATION"） '注意不是：Server.CreateObject（"EXCEL.APPLICATION"） 

Set xlBook = xlApp.Workbooks.Add 

Set xlSheet1 = xlBook.Worksheets（1） 

xlSheet1.cells（1,1）.value ="职务表" 

xlSheet1.range（"A1:D1"）.merge 

xlSheet1.cells（2,1）.value = "job_id" 

xlSheet1.cells（2,2）.value = "job_desc" 

xlSheet1.cells（2,3）.value = "max_lvl" 

xlSheet1.cells（2,4）.value = "min_lvl" 

cnt =3 

do while not rs.eof 

xlSheet1.cells（cnt,1）.value = rs（"job_id"） 

xlSheet1.cells（cnt,2）.value = rs（"job_desc"） 

xlSheet1.cells（cnt,3）.value = rs（"max_lvl"） 

xlSheet1.cells（cnt,4）.value = rs（"min_lvl"） 

rs.movenext 

cnt = cint（cnt） + 1 

loop 

xlSheet1.Application.Visible = True 

end sub 

＜/script＞ 
　　也可以实例化RDS DataControl，只需把以上部分代码进行修改： 

set rds = CreateObject（"RDS.DataSpace"） 

Set df = rds.CreateObject（"RDSServer.DataFactory","http://192.168.0.1"） '192.168.0.1 为WEB服务器IP地址 

strcn="provider=ms remote;remote server=http://192.168.0.1;handler=msdfmap.handler;data source=pubsdatabase;" '192.168.0.1 为WEB服务器IP地址 

strsql= "getalljobs" 

Set rs = df.Query（strCn, strSQL） 
　　修改为： 

set DC = createobject（"RDS.DataControl"） 

dc.ExecuteOptions =1 '设置成同步执行，可以简化下步代码 

dc.FetchOptions = 1 

With dc 

.Server = "http://192.168.0.1" 

.Handler = "MSDFMAP.Handler" 

.Connect = "Data Source=pubsdatabase;" 

.Sql = "getalljobs" 

.Refresh 

End With 

set rs= dc.Recordset 
　　修改文件MSDFMAP.INI（若在WIN98，C：/windows/msdfmap.ini；若在WIN2000,D:/winnt/msdfmap.ini；若在WIN2000 SERVER，D：/winnts/msdfmap.ini）。 

[sql getalljobs] 

Sql="SELECT * FROM jobs" 

[connect pubsDatabase] 

Access=Readonly 

Connect="provider=sqloledb;data source=sql server;initial catalog=pubs;UID=userid;PWD=password" 
　　打开注册表HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/ Services/W3SVC/Parameters/ADCLaunch 若无 RDSServer.Datafactory，请添加。本例使用RDS Default Handler访问数据库，若不通过RDS Handler访问数据库，修改注册表HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/DataFactory/ HandlerInfo 将HandlerRequired=1 设置成HandlerRequired =0。请注意，若不通过RDS Handler或自定义商业对象访问数据库，将对数据库带来安全隐患，所以作者极力推荐采用只能通过RDS Handler或自定义商业对象才能访问数据库的方式。 

　　下面用VB编写一个自定义商业对象，代码如下： 

'编写ActiveX DLL，名称：rsget.dll,包含类rsreturn，方法returnrs 

Public Function ReturnRs（strDB As Variant, strSQL As Variant） As ADODB.Recordset 

'Returns an ADODB recordset. 

On Error GoTo ehGetRecordset 

Dim cn As New ADODB.Connection 

Dim rs As New ADODB.Recordset 

Select Case strDB 

Case "ydjjspdatabase" 

strDB = "ydjjsp" 

Case "pubsdatabase" 

strDB = "pubs" 

End Select 

If strSQL = "getallbuy" Then 

strSQL = "select * from buyuser" 

GoTo nextstep 

End If 

If Left（strSQL, InStr（strSQL, "（"） - 1） = "getpubsbyid" Then 

If InStr（strSQL, ","） ＜= 0 Then 

Dim str As String 

str = Mid（strSQL, InStr（strSQL, "（"） + 2, InStr（strSQL, "）"） - InStr（strSQL, "（"） - 3） 

strSQL = "select * from jobs where job_id='" & str & "'" 

Else 

Dim strstart, strend As String 

strstart = Mid（strSQL, InStr（strSQL, "（"） + 2, InStr（strSQL, ","） - InStr（strSQL, "（"） - 3） 

strend = Mid（strSQL, InStr（strSQL, ","） + 2, InStr（strSQL, "）"） - InStr（strSQL, ","） - 3） 

strSQL = "select * from jobs where job_id＞='" & strstart & "' and job_id＜='" & strend & "'" 

End If 

End If 

nextstep: 

Dim strConnect As String 

strConnect = "Provider=SQLOLEDB;Server=ddk;uid=ydj;pwd=ydj; Database=" & strDB & ";" 

cn.Open strConnect 

rs.CursorLocation = adUseClient 

rs.Open strSQL, cn, adOpenStatic, adLockOptimistic, adCmdText 

Set ReturnRs = rs 

Exit Function 

ehGetRecordset: 

Err.Raise Err.Number, Err.Source, Err.Description 

End Function 
　　把rsget.dll复制到C:/WINDOWS或D:/WINNT,开始/运行，输入Regsvr32.exe c:/windows/rsget.dll或Regsvr32.exe d:/winnt/rsget.dll，按确定按钮，注册成WEB服务器组件，并在注册表HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/ Services/W3SVC/Parameters/ADCLaunch　添加rsget.rsreturn。 

　　若使用自定义商业对象，修改上面的ASP文件代码： 

set rds = CreateObject（"RDS.DataSpace"） 

Set df = rds.CreateObject（"RDSServer.DataFactory","http://192.168.0.1"） '192.168.0.1 为WEB服务器IP地址 

strcn="provider=ms remote;remote server=http://192.168.0.1;handler=msdfmap.handler;data source=pubsdatabase;" '192.168.0.1 为WEB服务器IP地址 

strsql= "getalljobs" 

Set rs = df.Query（strCn, strSQL） 
　　改为： 

set rds = CreateObject（"RDS.DataSpace"） 

Set df = rds.CreateObject（"rsget.rsreturn","http://192.168.0.1"） 

set rs=df.returnrs（"pubsdatabase","getpubsbyid（'2','10'）"）
　　另外在浏览器端需做如下配置： 

　　打开控制面板-＞INTERNET选项-＞安全性-＞自定义级别-＞ 对没有标记为安全的ActiveX控件进行初始化和脚本运行-＞开启

