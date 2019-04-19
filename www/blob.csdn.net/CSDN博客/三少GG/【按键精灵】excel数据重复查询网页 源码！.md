# 【按键精灵】excel数据重复查询网页 源码！ - 三少GG - CSDN博客
2010年05月08日 18:22:00[三少GG](https://me.csdn.net/scut1135)阅读数：2557
[General]
Description=Duan
BeginHotkey=121
BeginHotkeyMod=0
PauseHotkey=0
PauseHotkeyMod=0
StopHotkey=123
StopHotkeyMod=0
RunOnce=1
EnableWindow=
Enable=1
AutoRun=0
[Repeat]
Type=0
Number=1
[Relative]
SetupOCXFile=
[Comment]
Content=
[Script]
//Chris Grant
// PKU  2010.5.7
UserVar Num
Plugin Web.Bind("WQM.exe")
Plugin Web.Go("[http://www.ip138.com:8080/search.asp](http://www.ip138.com:8080/search.asp)")
//建立对象
Set xlApp=CreateObject("Excel.Application")
//这里是前台运行，不可见用False
xlApp.Visible = False
Set xlBook = xlApp.Workbooks.Open("d:/phone.xls")
Set xlSheet = xlBook.ActiveSheet
//最小化
xlApp.WindowState = -4140
Delay 2000
VBS dim j,a
VBS j=1
Rem [循环提交]
//*下面开始读一行,2个单元格(第一次就是1A,1B)
If Len(xlSheet.cells(j,1).value)=0
    //判断有无数据,没就退出
    Goto [没数据了]
EndIf 
VBS a=xlSheet.cells(j,1).value
VBS Num=a
//如果你要C格的数据,就是xlSheet.cells(j,i+2).value 再多同理
Delay 10
//*一行的两个数据读完,a是姓名,b是密码。下面你可以直接用输入变量：a b
//------这里接你需要到网页提交的身份证和姓名，如果想示范结果就是下面这句
//MessageBox "a"&j&":"&a
Plugin Web.HtmlInput(Num,"tag:input&name:mobile")
Delay 1000
//查询
Plugin Web.HtmlClick("tag:input&value:查 询")
Delay 2000
//返回结果
Plugin receive=Web.HtmlGet("text","tag:TD&index:8")
Delay 1000
VBS xlSheet.cells(j,2).value=receive
VBS j=j+1
//交完后,下面的语句就又开始下一个数据读取再输出
Goto [循环提交]
Rem [没数据了]
VBS xlApp.quit
//关闭工作表
VBS Set xlApp=Nothing
//释放对象
