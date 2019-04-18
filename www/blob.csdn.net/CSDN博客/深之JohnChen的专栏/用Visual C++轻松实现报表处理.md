# 用Visual C++轻松实现报表处理 - 深之JohnChen的专栏 - CSDN博客

2006年03月16日 10:59:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2818


**用Visual C++轻松实现报表处理**作 者：王骏

前言

　　数据库在企业软件系统中应用广泛，而报表的显示与打印成为此类软件必备的功能。可惜VC++并没有集成报表处理工具，但其强大的功能再加上市面上功能完备的报表处理工具使这一工作变得容易，本文介绍了在VC++环境中利用Seagate公司的Crystal report（水晶报表）来实现报表的显示与打印，并提供示例代码。 

　　一、准备工作

　　（1）安装Crystal Report软件
Crystal Report 4.6是一个免费的报表工具，可以在VB5或WINDOWS NT 4.0 BackOffice等光盘上找到，Crystal report4.6中包含了报表设计工具与报表打印控件。程序的发行只需要安装打印控件相关部分即可。

　　（2）设计报表

　　我们生成一个名为sample.rpt的报表，这一过程可以参考 Crystal report自带的帮助文档，本文不作详细介绍。

　　二、创建VC工程添加报表控件

　　在VC集成环境中用AppWizard新建一个MFC单文档工程，其余选项都为默认。菜单中选择Project->Add To Project->Components and Controls...弹出组件、控件选择对话框，进入Registered ActiveX Controls,选中Crystal Report Control 4.6 ,单击Insert按钮，确认后进入类配置对话框，按默认即可。关闭控件选择对话框完成控件的添加。

　　三、实现报表的显示与打印

　　下面我们将在对话框中演示控件的静态创建过程，在主视图中演示控件的动态创建过程。

　　3.1在对话框中打印出报表

　　在资源编辑器中打开ID为IDD_ABOUTBOX的对话框模板，在Controls工具条中我们可以找到刚加入到工程中的Crystal Report Control 4.6控件按钮，把它插入到对话框合适的位置处。

　　右键单击对话框中的控件，选择属性，此时可以设置控件的许多属性。我们在Control选项页ReportFileName中输入报表文件名sample.rpt(可以加上完整路径),在Print Window选项页中设置控件的合适位置,回到对话框模板中按住Ctrl键，双击鼠标左键，弹出Add Member Variable对话框,我们将成员变量命名为m_CrystalReport,打开ClassWizard,为CAboutDlg对话框添加WM_INITDIALOG消息的处理函数:BOOL OnInitDialog(),并在函数中做如下处理

BOOL CAboutDlg::OnInitDialog() 
{
CDialog::OnInitDialog();
/////此行设置控件的父窗口，你也可以去掉该行看看运行效果/////////
m_CrystalReport.SetWindowParentHandle((long)(this->m_hWnd));
/////打印报表到窗口中/////
m_CrystalReport.PrintReport(); 
return TRUE; 
}
　　至此，程序的第一部分编写完成，编译运行,打开About对话框看看效果吧!

![](http://www.vczx.com/article/img/20060301212954_144947.gif)

　　哦！还不错!

　　假如您并没有显示出报表，有如下可能原因：

　　(1)控件没有放置在合适的位置或尺寸不对。

　　(2)报表文件本身存在诸如数据源不可用等错误。

　　控件窗口中的工具条提供了缩放、打印等功能，您也可以试试在打印机上打印出来的效果。

　　3.2 在程序主视窗中显示报表

　　打开ClassWizard增加对ID_FILE_OPEN和ID_FILE_PRINT的处理函数,代码如下

void CMyReportView::OnFileOpen()
{
char Filter[] = "Crystal Report files(*.rpt)|*.rpt|All files(*.*)|*.*||";
CRect rect;
CFileDialog OpenDlg(TRUE,0,0,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,(LPCTSTR)Filter,NULL);
if(OpenDlg.DoModal()!=IDOK) ///显示文件对话框
return;
CString m_fName=OpenDlg.GetPathName(); ///取得文件名
if(m_CrystalReport)
m_CrystalReport.DestroyWindow();
GetClientRect(rect);
///////////////////创建控件///////////////
if (!m_CrystalReport.Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),WS_CHILD|
_ WS_VISIBLE,rect,this,IDC_CRYSTALREPORT1))
{
AfxMessageBox("控件创建失败!");
return ;
}
m_CrystalReport.SetWindowParentHandle((long)(this->m_hWnd));///设置父窗口
m_CrystalReport.SetWindowBorderStyle(0); ///设置为没有边框
m_CrystalReport.SetWindowLeft(0); ///左空间
m_CrystalReport.SetWindowTop(0); ///顶部空间
m_CrystalReport.SetWindowControls(FALSE); ///不显示工具条
m_CrystalReport.SetReportFileName(m_fName); ///设置报表文件
m_CrystalReport.SetWindowWidth(rect.Width()); ///设置窗口宽度
m_CrystalReport.SetWindowHeight(rect.Height()); ///设置窗口高度
m_CrystalReport.SetFormulas(0, "Company=/"VC知识库/""); 
///将报表中的Company变量的值设置为VC知识库
m_CrystalReport.SetDestination(0); ///设置输出对象是屏幕
m_CrystalReport.PrintReport(); ///显示报表
} 
void CMyReportView::OnFilePrint() 
{
if(m_CrystalReport && m_CrystalReport.GetReportFileName() != "")
{
m_CrystalReport.SetDestination(1); ///设置输出对象是打印机 
m_CrystalReport.PrintReport(); ///打印
}
}
　　后记：我们利用Crystal Report 4.6在VC++环境下实现了报表处理，但Crystal Report 4.6报表控件的功能及可控性能可能无法满足您的要求，Seagate公司此款软件的最新版本是Crystal Report 8.0，各方面表现都非常出色，但此款软件的免费版本并不包括报表设计器，可喜的是8.0中的报表显示控件兼容以前版本的报表格式，所以笔者建议用4.6版本来设计报表，依靠8.0中的报表显示控件来显示、打印。 

