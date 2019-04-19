# VC++USB及串口通信程序（附工程） - xqhrs232的专栏 - CSDN博客
2017年08月31日 17:32:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：680
原文地址::[http://blog.csdn.net/phmatthaus/article/details/51131329](http://blog.csdn.net/phmatthaus/article/details/51131329)
相关文章
1、[VC++ USB接口程序函数](http://blog.csdn.net/luckywang1103/article/details/12393889)----[http://blog.csdn.net/luckywang1103/article/details/12393889](http://blog.csdn.net/luckywang1103/article/details/12393889)
2、[用VC++实现USB接口读写数据的程序](http://blog.csdn.net/sha_jinhao/article/details/8618244)----[http://blog.csdn.net/sha_jinhao/article/details/8618244](http://blog.csdn.net/sha_jinhao/article/details/8618244)
3、[VC++编写USB接口通讯程序](http://blog.csdn.net/junaszxaz1/article/details/6393636)----[http://blog.csdn.net/junaszxaz1/article/details/6393636](http://blog.csdn.net/junaszxaz1/article/details/6393636)
4、用VC++编写USB接口通讯程序----[https://wenku.baidu.com/view/2251c2af162ded630b1c59eef8c75fbfc77d948a.html](https://wenku.baidu.com/view/2251c2af162ded630b1c59eef8c75fbfc77d948a.html)
5、用VC++编写USB接口通信程序----[http://download.csdn.net/download/shen_wei/4913169](http://download.csdn.net/download/shen_wei/4913169)
6、[vs2010 VC++ 无法打开包括文件 hidsdi.h](http://blog.csdn.net/cokewei/article/details/37671781)----[http://blog.csdn.net/cokewei/article/details/37671781](http://blog.csdn.net/cokewei/article/details/37671781)
 为公司开发产品[测试](http://lib.csdn.net/base/softwaretest)程序，实际上是基于VC++的USB通信及串口通信。当初编写程序时，网上找了很多资料及程序，感觉大都只是说了一点点，没有一个完整的工程以供其他开发者借鉴，完全要靠开发者东拼西凑，最后凑成一个整体工程，还要在这个工程基础上进行相当艰苦的排查问题、解决问题的调试过程。本人对这一过程极其厌烦，现将走完的以上的“艰苦历程”之后的成果列出，以供后来者借鉴。
      这一工程中包含了以下知识点：
      1. VC++ USB通信；
      2. VC++串口通信；
      3. 文件（设备）同步异步操作；
      4. 动态改变自体颜色
工程资源地址：[http://download.csdn.net/detail/phmatthaus/9488394](http://download.csdn.net/detail/phmatthaus/9488394)
// VRGate_Product_TestDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "VRGate_Product_Test.h"
#include "VRGate_Product_TestDlg.h"
#include "afxdialogex.h"
#include "setupapi.h"
extern "C" {
#include "hidsdi.h"
}
#pragma comment( lib, "setupapi.lib" )
#pragma comment( lib, "hid.lib" )
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BOOL Read8250RegFlag = FALSE;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
 CAboutDlg();
// 对话框数据
 enum { IDD = IDD_ABOUTBOX };
 protected:
 virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
// 实现
protected:
 DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// CVRGate_Product_TestDlg 对话框
CVRGate_Product_TestDlg::CVRGate_Product_TestDlg(CWnd* pParent /*=NULL*/)
 : CDialogEx(CVRGate_Product_TestDlg::IDD, pParent)
{
 m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
 devicePathName = _T("");
 InputReportLength = 0;
 OutputReportLength = 0;
}
void CVRGate_Product_TestDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CVRGate_Product_TestDlg, CDialogEx)
 ON_WM_SYSCOMMAND()
 ON_WM_PAINT()
 ON_WM_QUERYDRAGICON()
 ON_BN_CLICKED(IDC_FIND, &CVRGate_Product_TestDlg::OnBnClickedFind)
 //ON_BN_CLICKED(IDOK, &CVRGate_Product_TestDlg::OnBnClickedOk)
 ON_BN_CLICKED(IDC_OPEN, &CVRGate_Product_TestDlg::OnBnClickedOpen)
 ON_BN_CLICKED(IDC_CLOSE, &CVRGate_Product_TestDlg::OnBnClickedClose)
 ON_BN_CLICKED(IDC_OPEN2, &CVRGate_Product_TestDlg::OnBnClickedOpen2)
 ON_BN_CLICKED(IDC_CLOSE2, &CVRGate_Product_TestDlg::OnBnClickedClose2)
 ON_BN_CLICKED(IDC_ENTER_DEBUG_MODE, &CVRGate_Product_TestDlg::OnBnClickedEnterDebugMode)
 ON_BN_CLICKED(IDC_EXIT_DEBUG_MODE, &CVRGate_Product_TestDlg::OnBnClickedExitDebugMode)
 ON_BN_CLICKED(IDC_GET_PROTOCOL_VERSION, &CVRGate_Product_TestDlg::OnBnClickedGetProtocolVersion)
 ON_BN_CLICKED(IDC_GET_HARDWARE_VERSION, &CVRGate_Product_TestDlg::OnBnClickedGetHardwareVersion)
 ON_BN_CLICKED(IDC_GET_SOFTWARE_VERSION, &CVRGate_Product_TestDlg::OnBnClickedGetSoftwareVersion)
 ON_BN_CLICKED(IDC_READ_EDID, &CVRGate_Product_TestDlg::OnBnClickedReadEdid)
 ON_BN_CLICKED(IDC_READ_8520_REGISTER, &CVRGate_Product_TestDlg::OnBnClickedRead8520Register)
 ON_BN_CLICKED(IDC_READ_HDMI_TIMECHK, &CVRGate_Product_TestDlg::OnBnClickedReadHdmiTimechk)
 ON_BN_CLICKED(IDC_READ_SENSOR, &CVRGate_Product_TestDlg::OnBnClickedReadSensor)
 ON_BN_CLICKED(IDC_OPEN_BACKLIGHT, &CVRGate_Product_TestDlg::OnBnClickedOpenBacklight)
 ON_BN_CLICKED(IDC_CLOSE_BACKLIGHT, &CVRGate_Product_TestDlg::OnBnClickedCloseBacklight)
 ON_BN_CLICKED(IDC_CLOSE_PWM, &CVRGate_Product_TestDlg::OnBnClickedClosePwm)
 ON_BN_CLICKED(IDC_SET_SENSOR_SPEED, &CVRGate_Product_TestDlg::OnBnClickedSetSensorSpeed)
 ON_BN_CLICKED(IDC_ENTER_DEBUG_MODE2, &CVRGate_Product_TestDlg::OnBnClickedEnterDebugMode2)
 ON_BN_CLICKED(IDC_EXIT_DEBUG_MODE2, &CVRGate_Product_TestDlg::OnBnClickedExitDebugMode2)
 ON_BN_CLICKED(IDC_GET_HARDWARE_VERSION2, &CVRGate_Product_TestDlg::OnBnClickedGetHardwareVersion2)
 ON_BN_CLICKED(IDC_GET_SOFTWARE_VERSION2, &CVRGate_Product_TestDlg::OnBnClickedGetSoftwareVersion2)
 ON_BN_CLICKED(IDC_GET_PROTOCOL_VERSION2, &CVRGate_Product_TestDlg::OnBnClickedGetProtocolVersion2)
 ON_BN_CLICKED(IDC_READ_EDID2, &CVRGate_Product_TestDlg::OnBnClickedReadEdid2)
 ON_BN_CLICKED(IDC_READ_8520_REGISTER2, &CVRGate_Product_TestDlg::OnBnClickedRead8520Register2)
 ON_BN_CLICKED(IDC_READ_HDMI_TIMECHK2, &CVRGate_Product_TestDlg::OnBnClickedReadHdmiTimechk2)
 ON_BN_CLICKED(IDC_READ_SENSOR2, &CVRGate_Product_TestDlg::OnBnClickedReadSensor2)
 ON_BN_CLICKED(IDC_OPEN_BACKLIGHT2, &CVRGate_Product_TestDlg::OnBnClickedOpenBacklight2)
 ON_BN_CLICKED(IDC_CLOSE_PWM2, &CVRGate_Product_TestDlg::OnBnClickedClosePwm2)
 ON_BN_CLICKED(IDC_CLOSE_BACKLIGHT2, &CVRGate_Product_TestDlg::OnBnClickedCloseBacklight2)
 ON_BN_CLICKED(IDC_SET_SENSOR_SPEED2, &CVRGate_Product_TestDlg::OnBnClickedSetSensorSpeed2)
 ON_WM_CTLCOLOR()
 ON_BN_CLICKED(IDC_UPDATE_FIRMWARE, &CVRGate_Product_TestDlg::OnBnClickedUpdateFirmware)
END_MESSAGE_MAP()
// CVRGate_Product_TestDlg 消息处理程序
BOOL CVRGate_Product_TestDlg::OnInitDialog()
{
 CDialogEx::OnInitDialog();
 // 将“关于...”菜单项添加到系统菜单中。
 // IDM_ABOUTBOX 必须在系统命令范围内。
 ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
 ASSERT(IDM_ABOUTBOX < 0xF000);
 CMenu* pSysMenu = GetSystemMenu(FALSE);
 if (pSysMenu != NULL)
 {
  BOOL bNameValid;
  CString strAboutMenu;
  bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
  ASSERT(bNameValid);
  if (!strAboutMenu.IsEmpty())
  {
   pSysMenu->AppendMenu(MF_SEPARATOR);
   pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
  }
 }
 // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
 //  执行此操作
 SetIcon(m_hIcon, TRUE);   // 设置大图标
 SetIcon(m_hIcon, FALSE);  // 设置小图标
 ShowWindow(SW_MAXIMIZE);
 // TODO: 在此添加额外的初始化代码
 //初始化状态栏字体
 /*m_Font.CreateFont(-11, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Arial"));
    CEdit *m_Edit = (CEdit *)GetDlgItem(IDC_STATUS);
 m_Edit->SetFont(&m_Font,FALSE);
 m_Font2.CreateFont(-11, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Arial"));
    CEdit *m_Edit2 = (CEdit *)GetDlgItem(IDC_STATUS2);
 m_Edit2->SetFont(&m_Font2,FALSE); */
 //设置状态栏初始字体颜色
 m_color = RGB(0, 0, 0);
 //设置对话框初始状态
 GetDlgItem(IDC_FIND)->EnableWindow(1);
 GetDlgItem(IDC_OPEN)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE)->EnableWindow(0);
 GetDlgItem(IDC_ENTER_DEBUG_MODE)->EnableWindow(0);
 GetDlgItem(IDC_EXIT_DEBUG_MODE)->EnableWindow(0);
 GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_HARDWARE_VERSION)->EnableWindow(0);
 GetDlgItem(IDC_GET_SOFTWARE_VERSION)->EnableWindow(0);
 GetDlgItem(IDC_GET_PROTOCOL_VERSION)->EnableWindow(0);
 GetDlgItem(IDC_READ_EDID)->EnableWindow(0);
 GetDlgItem(IDC_READ_8520_REGISTER)->EnableWindow(0);
 GetDlgItem(IDC_READ_HDMI_TIMECHK)->EnableWindow(0);
 GetDlgItem(IDC_READ_SENSOR)->EnableWindow(0);
 GetDlgItem(IDC_OPEN_BACKLIGHT)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE_PWM)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE_BACKLIGHT)->EnableWindow(0);
 GetDlgItem(IDC_RADIO1)->EnableWindow(0);
 GetDlgItem(IDC_RADIO2)->EnableWindow(0);
 GetDlgItem(IDC_RADIO3)->EnableWindow(0);
 GetDlgItem(IDC_SET_SENSOR_SPEED)->EnableWindow(0);
 GetDlgItem(IDC_OPEN2)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE2)->EnableWindow(0);
 GetDlgItem(IDC_ENTER_DEBUG_MODE2)->EnableWindow(0);
 GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(0);
 GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(0);
 GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(0);
 GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(0);
 GetDlgItem(IDC_READ_EDID2)->EnableWindow(0);
 GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(0);
 GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(0);
 GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(0);
 GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(0);
 GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(0);
 GetDlgItem(IDC_RADIO4)->EnableWindow(0);
 GetDlgItem(IDC_RADIO5)->EnableWindow(0);
 GetDlgItem(IDC_RADIO6)->EnableWindow(0);
 GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(0);
 bHaveCom = FALSE;
 bComOpened = FALSE;
 if(CheckSerialPort() != FALSE)
 {
  bHaveCom = TRUE;
  GetDlgItem(IDC_OPEN2)->EnableWindow(1);
  ((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();
  for (int i = 0; i < sizeof(m_nComArray)/sizeof(int); i++)  
  {
   if(-1 == m_nComArray[i])
    break;
   CString string1;
   string1.Format(_T("COM%d"), m_nComArray[i]);
   ((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(string1);
  }
  ((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
  UpdateData(false);
 }
 else
 {
  bHaveCom = FALSE;
  AfxMessageBox(_T("PC机没有COM口....."));
 }
 return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CVRGate_Product_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
 if ((nID & 0xFFF0) == IDM_ABOUTBOX)
 {
  CAboutDlg dlgAbout;
  dlgAbout.DoModal();
 }
 else
 {
  CDialogEx::OnSysCommand(nID, lParam);
 }
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CVRGate_Product_TestDlg::OnPaint()
{
 if (IsIconic())
 {
  CPaintDC dc(this); // 用于绘制的设备上下文
  SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
  // 使图标在工作区矩形中居中
  int cxIcon = GetSystemMetrics(SM_CXICON);
  int cyIcon = GetSystemMetrics(SM_CYICON);
  CRect rect;
  GetClientRect(&rect);
  int x = (rect.Width() - cxIcon + 1) / 2;
  int y = (rect.Height() - cyIcon + 1) / 2;
  // 绘制图标
  dc.DrawIcon(x, y, m_hIcon);
 }
 else
 {
  CDialogEx::OnPaint();
 }
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVRGate_Product_TestDlg::OnQueryDragIcon()
{
 return static_cast<HCURSOR>(m_hIcon);
}
BOOL CVRGate_Product_TestDlg::USBReceivePacket(void)
{
 BOOL bResult = TRUE;
 DWORD dwError = 0;
 DWORD dwBytesRead = InputReportLength;
 CString tmpstr;
 ResetEvent(USBReadOverlapped.hEvent);
 bResult = ReadFile(HidHandle, USBRxBuff, dwBytesRead, &dwBytesRead, &USBReadOverlapped);
 if (!bResult)
 {
  if (ERROR_IO_PENDING == GetLastError()) //函数返回ERROR_IO_PENDING,表明USB正在进行读操作
  {
   WaitForSingleObject(USBReadOverlapped.hEvent, 2000); //使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟.当USB读操作进行完毕后，USBReadOverlapped的hEvent事件会变为有信号
  }
  else
  {
   ResetEvent(USBReadOverlapped.hEvent);
   AfxMessageBox(_T("USB接收数据失败!"));
   return FALSE;
  }
 }
 if((USBRxBuff[1] = 0xAA) && (USBRxBuff[2] = 0x55) && (USBRxBuff[8+USBRxBuff[3]] == 0x0D) && (USBRxBuff[8+USBRxBuff[3]+1] == 0x0A)) //判断帧头帧尾
 {
  switch(USBRxBuff[5])
  {
   //Debug使用
   case 0xA0:
    if((USBRxBuff[8] == 'D') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'b') && (USBRxBuff[11] == 'u') && (USBRxBuff[12] == 'g') && (USBRxBuff[13] == ' ') \
     && (USBRxBuff[14] == 'M') && (USBRxBuff[15] == 'o') && (USBRxBuff[16] == 'd') && (USBRxBuff[17] == 'e'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n调试模式");
    }
    else if((USBRxBuff[8] == 'N') && (USBRxBuff[9] == 'o') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'm') && (USBRxBuff[12] == 'a') && (USBRxBuff[13] == 'l') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == 'M') && (USBRxBuff[16] == 'o') && (USBRxBuff[17] == 'd') && (USBRxBuff[18] == 'e'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n正常模式");
    }
    else if((USBRxBuff[8] == 'U') && (USBRxBuff[9] == 'p') && (USBRxBuff[10] == 'd') && (USBRxBuff[11] == 'a') && (USBRxBuff[12] == 't') && (USBRxBuff[13] == 'e') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == 'M') && (USBRxBuff[16] == 'o') && (USBRxBuff[17] == 'd') && (USBRxBuff[18] == 'e'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n升级模式");
    }
    else if ((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'r') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'o') && (USBRxBuff[12] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //获取版本信息类
   case 0xC0:
    if((USBRxBuff[8] == 'V') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'i') && (USBRxBuff[13] == 'o') && (USBRxBuff[14] == 'n') && (USBRxBuff[15] == '_') \
     && (USBRxBuff[16] == 'H') && (USBRxBuff[17] == 'a') && (USBRxBuff[18] == 'r') && (USBRxBuff[19] == 'd') && (USBRxBuff[20] == 'w') && (USBRxBuff[21] == 'a') && (USBRxBuff[22] == 'r') && (USBRxBuff[23] == 'e') \
     && (USBRxBuff[24] == '_') && (USBRxBuff[25] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), USBRxBuff[26],USBRxBuff[27]);
     InfoMsg = _T("\r\n硬件板卡版本号: ") + tmpstr;
    }
    else if((USBRxBuff[8] == 'V') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'i') && (USBRxBuff[13] == 'o') && (USBRxBuff[14] == 'n') && (USBRxBuff[15] == '_') \
     && (USBRxBuff[16] == 'S') && (USBRxBuff[17] == 'o') && (USBRxBuff[18] == 'f') && (USBRxBuff[19] == 't') && (USBRxBuff[20] == 'w') && (USBRxBuff[21] == 'a') && (USBRxBuff[22] == 'r') && (USBRxBuff[23] == 'e') \
     && (USBRxBuff[24] == '_') && (USBRxBuff[25] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), USBRxBuff[26],USBRxBuff[27]);
     InfoMsg = _T("\r\n固件版本号: ") + tmpstr;
    }
    else if((USBRxBuff[8] == 'V') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'i') && (USBRxBuff[13] == 'o') && (USBRxBuff[14] == 'n') && (USBRxBuff[15] == '_') \
     && (USBRxBuff[16] == 'P') && (USBRxBuff[17] == 'r') && (USBRxBuff[18] == 'o') && (USBRxBuff[19] == 't') && (USBRxBuff[20] == 'o') && (USBRxBuff[21] == 'c') && (USBRxBuff[22] == 'o') && (USBRxBuff[23] == 'l') \
     && (USBRxBuff[24] == '_') && (USBRxBuff[25] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), USBRxBuff[26],USBRxBuff[27]);
     InfoMsg = _T("\r\n通讯协议版本号: ") + tmpstr;
    }
    else if ((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'r') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'o') && (USBRxBuff[12] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //测试通讯类收发类
   case 0xD0:
    if((USBRxBuff[8] == 0x00) && (USBRxBuff[9] == 0xFF) && (USBRxBuff[10] == 0xFF) && (USBRxBuff[11] == 0xFF) && (USBRxBuff[12] == 0xFF) && (USBRxBuff[13] == 0xFF) && (USBRxBuff[14] == 0xFF) && (USBRxBuff[15] == 0x00)) //EDID数据头
    {
     bResult = TRUE;
     tmpstr.Format(_T("0x%x%x"), USBRxBuff[16], USBRxBuff[17]);
     InfoMsg = _T("\r\n厂商ID: ") + tmpstr;
     tmpstr.Format(_T("0x%x%x"), USBRxBuff[18], USBRxBuff[19]);
     InfoMsg += _T("\r\n产品ID: ") + tmpstr;
     tmpstr.Format(_T("0x%x%x%x%x"), USBRxBuff[20], USBRxBuff[21], USBRxBuff[22], USBRxBuff[23]);
     InfoMsg += _T("\r\n序列号: ") + tmpstr;
     tmpstr.Format(_T("%d"), USBRxBuff[25]+1990);
     InfoMsg += _T("\r\n制造年份: ") + tmpstr;
     tmpstr.Format(_T("%d"), USBRxBuff[24]);
     InfoMsg += _T("\r\n制造周别: ") + tmpstr;
     tmpstr.Format(_T("%c.%c"), USBRxBuff[26], USBRxBuff[27]);
     InfoMsg += _T("\r\nEDIT版本: ") + tmpstr;
    }
    else if(Read8250RegFlag == TRUE)
    {
     Read8250RegFlag = FALSE;
     bResult = TRUE;
     tmpstr.Format(_T("0x%x"), USBRxBuff[8]);
     InfoMsg = _T("\r\n8520寄存器值: ") + tmpstr;
    }
    else if((USBRxBuff[8] == 'T') && (USBRxBuff[9] == 'C') && (USBRxBuff[10] == '3') && (USBRxBuff[11] == '5') && (USBRxBuff[12] == '8') && (USBRxBuff[13] == '8') && (USBRxBuff[14] == '7') && (USBRxBuff[15] == '0') \
     && (USBRxBuff[16] == '_') && (USBRxBuff[17] == 'H') && (USBRxBuff[18] == 'D') && (USBRxBuff[19] == 'M') && (USBRxBuff[20] == 'I') && (USBRxBuff[21] == 'T') && (USBRxBuff[22] == 'i') && (USBRxBuff[23] == 'm') \
     && (USBRxBuff[24] == 'C') && (USBRxBuff[25] == 'h') && (USBRxBuff[26] == 'k') && (USBRxBuff[27] == ':') && (USBRxBuff[28] == ' '))
    {
     bResult = TRUE;
     InfoMsg = _T("TC358870_HDMITimChk: " + USBRxBuff[29] + USBRxBuff[30] + USBRxBuff[31] + USBRxBuff[32] + USBRxBuff[33] + USBRxBuff[34] + USBRxBuff[35]);
    }
    else if((USBRxBuff[8] == 'S') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'n') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'o') && (USBRxBuff[13] == 'r') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == ':') && (USBRxBuff[16] == ' '))
    {
     bResult = TRUE;
     InfoMsg = _T("Sensor: " + USBRxBuff[17] + USBRxBuff[18] + USBRxBuff[19] + USBRxBuff[20]+ USBRxBuff[21] + USBRxBuff[22] + USBRxBuff[23] + USBRxBuff[24]+ USBRxBuff[25]);
    }
    else if ((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'r') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'o') && (USBRxBuff[12] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //测试使能控制开关类
   case 0xE0: 
    if((USBRxBuff[8] == 'C') && (USBRxBuff[9] == 'L') && (USBRxBuff[10] == 'S') && (USBRxBuff[11] == '_') && (USBRxBuff[12] == 'P') && (USBRxBuff[13] == 'W') && (USBRxBuff[14] == 'M') \
     && (USBRxBuff[15] == '_') && (USBRxBuff[16] == 'B') && (USBRxBuff[17] == 'a') && (USBRxBuff[18] == 'c') && (USBRxBuff[19] == 'k') && (USBRxBuff[20] == 'L') && (USBRxBuff[21] == 'i') && (USBRxBuff[22] == 'g') && (USBRxBuff[23] == 'h') && (USBRxBuff[24]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-关闭背光");
    }
    else if((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'N') && (USBRxBuff[10] == '_') && (USBRxBuff[11] == 'P') && (USBRxBuff[12] == 'W') && (USBRxBuff[13] == 'M') \
     && (USBRxBuff[14] == '_') && (USBRxBuff[15] == 'B') && (USBRxBuff[16] == 'a') && (USBRxBuff[17] == 'c') && (USBRxBuff[18] == 'k') && (USBRxBuff[19] == 'L') && (USBRxBuff[20] == 'i') && (USBRxBuff[21] == 'g') && (USBRxBuff[22] == 'h') && (USBRxBuff[23]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-普通亮度");
    }
    else if((USBRxBuff[8] == 'M') && (USBRxBuff[9] == 'A') && (USBRxBuff[10] == 'X') && (USBRxBuff[11] == '_') && (USBRxBuff[12] == 'P') && (USBRxBuff[13] == 'W') && (USBRxBuff[14] == 'M') \
     && (USBRxBuff[15] == '_') && (USBRxBuff[16] == 'B') && (USBRxBuff[17] == 'a') && (USBRxBuff[18] == 'c') && (USBRxBuff[19] == 'k') && (USBRxBuff[20] == 'L') && (USBRxBuff[21] == 'i') && (USBRxBuff[22] == 'g') && (USBRxBuff[23] == 'h') && (USBRxBuff[24]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-最大亮度");
    }
    else if ((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'r') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'o') && (USBRxBuff[12] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //设置陀螺仪等Sensor的传输速度
   case 0xE1:
    if((USBRxBuff[8] == 'S') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'n') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'o') && (USBRxBuff[13] == 'r') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == '1') && (USBRxBuff[16] == '0') && (USBRxBuff[17] == '0') && (USBRxBuff[18] == '0') && (USBRxBuff[19] == 'H') && (USBRxBuff[20] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：1000Hz");
    }
    else if((USBRxBuff[8] == 'S') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'n') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'o') && (USBRxBuff[13] == 'r') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == '5') && (USBRxBuff[16] == '0') && (USBRxBuff[17] == '0') && (USBRxBuff[18] == 'H') && (USBRxBuff[19] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：500Hz");
    }
    else if((USBRxBuff[8] == 'S') && (USBRxBuff[9] == 'e') && (USBRxBuff[10] == 'n') && (USBRxBuff[11] == 's') && (USBRxBuff[12] == 'o') && (USBRxBuff[13] == 'r') && (USBRxBuff[14] == ' ') \
     && (USBRxBuff[15] == '1') && (USBRxBuff[16] == '0') && (USBRxBuff[17] == '0') && (USBRxBuff[18] == 'H') && (USBRxBuff[19] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：100Hz");
    }
    else if ((USBRxBuff[8] == 'E') && (USBRxBuff[9] == 'r') && (USBRxBuff[10] == 'r') && (USBRxBuff[11] == 'o') && (USBRxBuff[12] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   default:
    bResult = FALSE;
    ErrMsg = _T("\r\n其他错误: 未定义指令");
    break;
  }
 }
 else
 {
  bResult = FALSE;
  ErrMsg = _T("\r\n帧数据校验错误");
 }
 ResetEvent(USBReadOverlapped.hEvent);
 return bResult;
}
BOOL CVRGate_Product_TestDlg::ComReceivePacket(void)
{
 BOOL bResult = TRUE;
 DWORD dwError = 0;
 DWORD dwBytesRead = sizeof(ComRxBuff);
 CString tmpstr;
 memset(ComRxBuff, 0x00 ,sizeof(ComRxBuff));
/* 不能有这段程序，否则直接清空了串口输入缓冲区，导致数据收不到
 ResetEvent(ComReadOverlapped.hEvent);
 //清空串口的输入缓冲区
 PurgeComm(ComHandle, PURGE_RXABORT | PURGE_RXCLEAR); //立即中断所有读操作并立即返回，即使读操作还没有完成 | 清空输入缓冲区
 */
 bResult = ClearCommError(ComHandle, &dwError, &ComReadStat); //在使用ReadFile 函数进行读操作前，应先使用ClearCommError函数清除错误.该函数获得通信错误并报告串口的当前状态，同时，该函数清除串口的错误标志以便继续输入、输出操作
 if (ComReadStat.cbInQue == 0) //输入缓冲区有多少数据
  return FALSE;
 dwBytesRead = min(dwBytesRead, (DWORD)ComReadStat.cbInQue);
 if(!dwBytesRead)
  return FALSE;
 bResult = ReadFile(ComHandle, ComRxBuff, dwBytesRead, &dwBytesRead, &ComReadOverlapped);
 if (!bResult)
 {
  if (ERROR_IO_PENDING == GetLastError()) //函数返回ERROR_IO_PENDING,表明串口正在进行读操作
  {
   WaitForSingleObject(ComReadOverlapped.hEvent, 2000); ////使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟.当串口读操作进行完毕后，m_ov的hEvent事件会变为有信号
  }
  else
  {
   ClearCommError(ComHandle, &dwError, &ComReadStat);
   PurgeComm(ComHandle, PURGE_RXABORT | PURGE_RXCLEAR); //立即中断所有读操作并立即返回，即使读操作还没有完成 | 清空输入缓冲区
   ResetEvent(ComReadOverlapped.hEvent);
   AfxMessageBox(_T("串口接收数据失败!"));
   return FALSE;
  }
 }
 if((ComRxBuff[0] = 0xAA) && (ComRxBuff[1] = 0x55) && (ComRxBuff[8+ComRxBuff[2]-1] == 0x0D) && (ComRxBuff[8+ComRxBuff[2]] == 0x0A)) //判断帧头帧尾
 {
  switch(ComRxBuff[4])
  {
   //Debug使用
   case 0xA0:
    if((ComRxBuff[7] == 'D') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'b') && (ComRxBuff[10] == 'u') && (ComRxBuff[11] == 'g') && (ComRxBuff[12] == ' ') \
     && (ComRxBuff[13] == 'M') && (ComRxBuff[14] == 'o') && (ComRxBuff[15] == 'd') && (ComRxBuff[16] == 'e'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n调试模式");
    }
    else if((ComRxBuff[7] == 'N') && (ComRxBuff[8] == 'o') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'm') && (ComRxBuff[11] == 'a') && (ComRxBuff[12] == 'l') && (ComRxBuff[13] == ' ') \
     && (ComRxBuff[14] == 'M') && (ComRxBuff[15] == 'o') && (ComRxBuff[16] == 'd') && (ComRxBuff[17] == 'e'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n正常模式");
    }
    else if ((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'r') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'o') && (ComRxBuff[11] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //获取版本信息类
   case 0xC0:
    if((ComRxBuff[7] == 'V') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'i') && (ComRxBuff[12] == 'o') && (ComRxBuff[13] == 'n') && (ComRxBuff[14] == '_') \
     && (ComRxBuff[15] == 'H') && (ComRxBuff[16] == 'a') && (ComRxBuff[17] == 'r') && (ComRxBuff[18] == 'd') && (ComRxBuff[19] == 'w') && (ComRxBuff[20] == 'a') && (ComRxBuff[21] == 'r') && (ComRxBuff[22] == 'e') \
     && (ComRxBuff[23] == '_') && (ComRxBuff[24] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), ComRxBuff[25],ComRxBuff[26]);
     InfoMsg = _T("\r\n硬件板卡版本号: ") + tmpstr;
    }
    else if((ComRxBuff[7] == 'V') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'i') && (ComRxBuff[12] == 'o') && (ComRxBuff[13] == 'n') && (ComRxBuff[14] == '_') \
     && (ComRxBuff[15] == 'S') && (ComRxBuff[16] == 'o') && (ComRxBuff[17] == 'f') && (ComRxBuff[18] == 't') && (ComRxBuff[19] == 'w') && (ComRxBuff[20] == 'a') && (ComRxBuff[21] == 'r') && (ComRxBuff[22] == 'e') \
     && (ComRxBuff[23] == '_') && (ComRxBuff[24] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), ComRxBuff[25],ComRxBuff[26]);
     InfoMsg = _T("\r\n固件版本号: ") + tmpstr;
    }
    else if((ComRxBuff[7] == 'V') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'i') && (ComRxBuff[12] == 'o') && (ComRxBuff[13] == 'n') && (ComRxBuff[14] == '_') \
     && (ComRxBuff[15] == 'P') && (ComRxBuff[16] == 'r') && (ComRxBuff[17] == 'o') && (ComRxBuff[18] == 't') && (ComRxBuff[19] == 'o') && (ComRxBuff[20] == 'c') && (ComRxBuff[21] == 'o') && (ComRxBuff[22] == 'l') \
     && (ComRxBuff[23] == '_') && (ComRxBuff[24] == 'V'))
    {
     bResult = TRUE;
     tmpstr.Format(_T("%c%c"), ComRxBuff[25],ComRxBuff[26]);
     InfoMsg = _T("\r\n通讯协议版本号: ") + tmpstr;
    }
    else if ((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'r') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'o') && (ComRxBuff[11] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //测试通讯类收发类
   case 0xD0:
    if((ComRxBuff[7] == 0x00) && (ComRxBuff[8] == 0xFF) && (ComRxBuff[9] == 0xFF) && (ComRxBuff[10] == 0xFF) && (ComRxBuff[11] == 0xFF) && (ComRxBuff[12] == 0xFF) && (ComRxBuff[13] == 0xFF) && (ComRxBuff[14] == 0x00))
    {
     bResult = TRUE;
     tmpstr.Format(_T("0x%x%x"), ComRxBuff[15], ComRxBuff[16]);
     InfoMsg = _T("\r\n厂商ID: ") + tmpstr;
     tmpstr.Format(_T("0x%x%x"), ComRxBuff[17], ComRxBuff[18]);
     InfoMsg += _T("\r\n产品ID: ") + tmpstr;
     tmpstr.Format(_T("0x%x%x%x%x"), ComRxBuff[19], ComRxBuff[20], ComRxBuff[21], ComRxBuff[22]);
     InfoMsg += _T("\r\n序列号: ") + tmpstr;
     tmpstr.Format(_T("%d"), ComRxBuff[24]+1990);
     InfoMsg += _T("\r\n制造年份: ") + tmpstr;
     tmpstr.Format(_T("%d"), ComRxBuff[23]);
     InfoMsg += _T("\r\n制造周别: ") + tmpstr;
     tmpstr.Format(_T("%c.%c"), ComRxBuff[25], ComRxBuff[26]);
     InfoMsg += _T("\r\nEDIT版本: ") + tmpstr;
    }
    else if(Read8250RegFlag == TRUE)
    {
     Read8250RegFlag = FALSE;
     bResult = TRUE;
     tmpstr.Format(_T("0x%x"), ComRxBuff[7]);
     InfoMsg = _T("\r\n8520寄存器值: ") + tmpstr;
    }
    else if((ComRxBuff[7] == 'T') && (ComRxBuff[8] == 'C') && (ComRxBuff[9] == '3') && (ComRxBuff[10] == '5') && (ComRxBuff[11] == '8') && (ComRxBuff[12] == '8') && (ComRxBuff[13] == '7') && (ComRxBuff[14] == '0') \
     && (ComRxBuff[15] == '_') && (ComRxBuff[16] == 'H') && (ComRxBuff[17] == 'D') && (ComRxBuff[18] == 'M') && (ComRxBuff[19] == 'I') && (ComRxBuff[20] == 'T') && (ComRxBuff[21] == 'i') && (ComRxBuff[22] == 'm') \
     && (ComRxBuff[23] == 'C') && (ComRxBuff[24] == 'h') && (ComRxBuff[25] == 'k') && (ComRxBuff[26] == ':') && (ComRxBuff[27] == ' '))
    {
     bResult = TRUE;
     InfoMsg = _T("TC358870_HDMITimChk: " + ComRxBuff[28] + ComRxBuff[29] + ComRxBuff[30] + ComRxBuff[31] + ComRxBuff[32] + ComRxBuff[33] + ComRxBuff[34]);
    }
    else if((ComRxBuff[7] == 'S') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'n') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'o') && (ComRxBuff[12] == 'r') && (ComRxBuff[13] == ' ') \
     && (ComRxBuff[14] == ':') && (ComRxBuff[15] == ' '))
    {
     bResult = TRUE;
     InfoMsg = _T("Sensor: " + ComRxBuff[16] + ComRxBuff[17] + ComRxBuff[18] + ComRxBuff[19] + ComRxBuff[20] + ComRxBuff[21] + ComRxBuff[22] + ComRxBuff[23] + ComRxBuff[24]);
    }
    else if ((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'r') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'o') && (ComRxBuff[11] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //测试使能控制开关类
   case 0xE0: 
    if((ComRxBuff[7] == 'C') && (ComRxBuff[8] == 'L') && (ComRxBuff[9] == 'S') && (ComRxBuff[10] == '_') && (ComRxBuff[11] == 'P') && (ComRxBuff[12] == 'W') && (ComRxBuff[13] == 'M') \
     && (ComRxBuff[14] == '_') && (ComRxBuff[15] == 'B') && (ComRxBuff[16] == 'a') && (ComRxBuff[17] == 'c') && (ComRxBuff[18] == 'k') && (ComRxBuff[19] == 'L') && (ComRxBuff[20] == 'i') && (ComRxBuff[21] == 'g') && (ComRxBuff[22] == 'h') && (ComRxBuff[23]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-关闭背光");
    }
    else if((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'N') && (ComRxBuff[9] == '_') && (ComRxBuff[10] == 'P') && (ComRxBuff[11] == 'W') && (ComRxBuff[12] == 'M') \
     && (ComRxBuff[13] == '_') && (ComRxBuff[14] == 'B') && (ComRxBuff[15] == 'a') && (ComRxBuff[16] == 'c') && (ComRxBuff[17] == 'k') && (ComRxBuff[18] == 'L') && (ComRxBuff[19] == 'i') && (ComRxBuff[20] == 'g') && (ComRxBuff[21] == 'h') && (ComRxBuff[22]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-普通亮度");
    }
    else if((ComRxBuff[7] == 'M') && (ComRxBuff[8] == 'A') && (ComRxBuff[9] == 'X') && (ComRxBuff[10] == '_') && (ComRxBuff[11] == 'P') && (ComRxBuff[12] == 'W') && (ComRxBuff[13] == 'M') \
     && (ComRxBuff[14] == '_') && (ComRxBuff[15] == 'B') && (ComRxBuff[16] == 'a') && (ComRxBuff[17] == 'c') && (ComRxBuff[18] == 'k') && (ComRxBuff[19] == 'L') && (ComRxBuff[20] == 'i') && (ComRxBuff[21] == 'g') && (ComRxBuff[22] == 'h') && (ComRxBuff[23]
 == 't'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n屏幕背光-最大亮度");
    }
    else if ((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'r') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'o') && (ComRxBuff[11] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   //设置陀螺仪等Sensor的传输速度
   case 0xE1:
    if((ComRxBuff[7] == 'S') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'n') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'o') && (ComRxBuff[12] == 'r') && (ComRxBuff[13] == ' ') \
     && (ComRxBuff[14] == '1') && (ComRxBuff[15] == '0') && (ComRxBuff[16] == '0') && (ComRxBuff[17] == '0') && (ComRxBuff[18] == 'H') && (ComRxBuff[19] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：1000Hz");
    }
    else if((ComRxBuff[7] == 'S') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'n') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'o') && (ComRxBuff[12] == 'r') && (ComRxBuff[13] == ' ') \
     && (ComRxBuff[14] == '5') && (ComRxBuff[15] == '0') && (ComRxBuff[16] == '0') && (ComRxBuff[17] == 'H') && (ComRxBuff[18] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：500Hz");
    }
    else if((ComRxBuff[7] == 'S') && (ComRxBuff[8] == 'e') && (ComRxBuff[9] == 'n') && (ComRxBuff[10] == 's') && (ComRxBuff[11] == 'o') && (ComRxBuff[12] == 'r') && (ComRxBuff[13] == ' ') \
     && (ComRxBuff[14] == '1') && (ComRxBuff[15] == '0') && (ComRxBuff[16] == '0') && (ComRxBuff[17] == 'H') && (ComRxBuff[18] == 'z'))
    {
     bResult = TRUE;
     InfoMsg = _T("\r\n传感器采样率：100Hz");
    }
    else if ((ComRxBuff[7] == 'E') && (ComRxBuff[8] == 'r') && (ComRxBuff[9] == 'r') && (ComRxBuff[10] == 'o') && (ComRxBuff[11] == 'r'))
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n下位机返回Error");
    }
    else
    {
     bResult = FALSE;
     ErrMsg = _T("\r\n其他错误");
    }
    break;
   default:
    bResult = FALSE;
    ErrMsg = _T("\r\n其他错误: 未定义指令");
    break;
  }
 }
 else
 {
  bResult = FALSE;
  ErrMsg = _T("\r\n帧数据校验错误");
 }
 ClearCommError(ComHandle, &dwError, &ComReadStat);
 //清空串口的输入缓冲区
 PurgeComm(ComHandle, PURGE_RXABORT | PURGE_RXCLEAR); //立即中断所有读操作并立即返回，即使读操作还没有完成 | 清空输入缓冲区
 ResetEvent(ComReadOverlapped.hEvent);
 return bResult;
}
BOOL CVRGate_Product_TestDlg::USBSendPacket(unsigned char *sendbuffer)
{
 DWORD dwBytesWrite = OutputReportLength;
 BOOL bWriteStat;
 if(!sendbuffer)
  return FALSE;
 ResetEvent(USBWriteOverlapped.hEvent);
 bWriteStat = WriteFile(HidHandle, sendbuffer, dwBytesWrite, &dwBytesWrite, &USBWriteOverlapped);
 if(!bWriteStat)
 {
  if(GetLastError() == ERROR_IO_PENDING) 
  {
   WaitForSingleObject(USBWriteOverlapped.hEvent,2000);
  }
  else
  {
   ResetEvent(USBWriteOverlapped.hEvent);
   AfxMessageBox(_T("USB发送数据失败!"));
   return FALSE;
  }
 }
 ResetEvent(USBWriteOverlapped.hEvent);
 return TRUE;
}
BOOL CVRGate_Product_TestDlg::ComSendPacket(unsigned char *sendbuffer, int length)
{
 DWORD dwBytesWrite = length;
 DWORD dwErrorFlags;
 BOOL bWriteStat;
 if(!sendbuffer)
  return FALSE;
 ResetEvent(ComWriteOverlapped.hEvent);
 //清空串口的输出缓冲区
 PurgeComm(ComHandle, PURGE_TXABORT | PURGE_TXCLEAR); //立即中断所有写操作并立即返回，即使写操作还没有完成 | 清空输出缓冲区
 ClearCommError(ComHandle, &dwErrorFlags, &ComWriteStat);
 bWriteStat = WriteFile(ComHandle, sendbuffer, dwBytesWrite, &dwBytesWrite, &ComWriteOverlapped);
 if(!bWriteStat)
 {
  if(GetLastError() == ERROR_IO_PENDING) 
  {
   WaitForSingleObject(ComWriteOverlapped.hEvent,2000);
  }
  else
  {
   ClearCommError(ComHandle, &dwErrorFlags, &ComWriteStat);
   //清空串口的输出缓冲区
   PurgeComm(ComHandle, PURGE_TXABORT | PURGE_TXCLEAR); //立即中断所有写操作并立即返回，即使写操作还没有完成 | 清空输出缓冲区
   ResetEvent(ComWriteOverlapped.hEvent);
   AfxMessageBox(_T("串口发送数据失败!"));
   return FALSE;
  }
 }
 ClearCommError(ComHandle, &dwErrorFlags, &ComWriteStat);
 //清空串口的输出缓冲区
 PurgeComm(ComHandle, PURGE_TXABORT | PURGE_TXCLEAR); //立即中断所有写操作并立即返回，即使写操作还没有完成 | 清空输出缓冲区
 ResetEvent(ComWriteOverlapped.hEvent);
 return TRUE;
}
/*
DWORD WINAPI USBReadThread(LPVOID pParam)
{
 while(bIsUSBThreadWorking)
 {
  USBReceivePacket();
 }
 bIsUSBThreadStop = 1;//到这里表示已经跳出循环了
 return 0;
}
*/
CString CVRGate_Product_TestDlg::FindUSBDevice(CString Vid, CString Pid)
{
 //查找本系统中HID类的GUID标识
 GUID guidHID;
 HidD_GetHidGuid(&guidHID);
 //查找符合HID规范的USB设备
 HDEVINFO hDevInfo = SetupDiGetClassDevs(&guidHID, NULL, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
 //失败
 if (hDevInfo == INVALID_HANDLE_VALUE)
 {
  return NULL;
 }
 int InterfaceIndex = 0;
 while (TRUE)
 {
  //查找USB设备接口
  SP_DEVICE_INTERFACE_DATA strtInterfaceData;
  strtInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);  //必须有这句，否则下边枚举会失败！
  //枚举接口，TRUE表示成功
  BOOL result = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guidHID, InterfaceIndex, &strtInterfaceData);
  if (!result)
  {
   break;
  }
  InterfaceIndex++;
  int bufferSize = 0;
  //若找到了接口，则读取设备(接口)路径名
  SP_DEVINFO_DATA strtDevInfoData;
  strtDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
  //第一次调用出错，但可以返回正确的Size
  result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &strtInterfaceData, NULL, 0, (PDWORD)&bufferSize, &strtDevInfoData);
  //第二次调用传递返回值，调用即可成功
  /* Allocate memory for Device Detailed Data */
  PSP_DEVICE_INTERFACE_DETAIL_DATA strtDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(bufferSize);  //此处必须用动态分配而不能定义局部变量，因为要在此结构体后边存放很大的内容，而局部变量没有如此大的空间！
  /* Set cbSize in the DevDetail structure */
  strtDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
  result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &strtInterfaceData, strtDetailData, bufferSize, (PDWORD)&bufferSize, &strtDevInfoData);
  if (result == FALSE)
  {
   free(strtDetailData);
   strtDetailData = NULL;
   break;
  }
  //获取设备路径
  wchar_t *pdevicePathName = strtDetailData->DevicePath;
  CString tmpDevicePathName = CString(pdevicePathName);
  free(strtDetailData);
  strtDetailData = NULL;
  pdevicePathName = NULL;
  CString tmpstr = _T("hid#");
  tmpstr += _T("vid_") + Vid.MakeLower() + '&' + _T("pid_") + Pid.MakeLower();  //tmpstr should be: "hid#vid_046d&pid_c077"
  int idx = -1;
  idx = tmpDevicePathName.Find(tmpstr);
  if (idx >= 0)
  {
   //释放设备资源(hDevInfo是SetupDiGetClassDevs获取的)
   if (hDevInfo != INVALID_HANDLE_VALUE)
    SetupDiDestroyDeviceInfoList(hDevInfo);
   return tmpDevicePathName;
  }
 }  //End of while
 //释放设备资源(hDevInfo是SetupDiGetClassDevs获取的)
 if (hDevInfo != INVALID_HANDLE_VALUE)
  SetupDiDestroyDeviceInfoList(hDevInfo);
 return NULL;
}
BOOL CVRGate_Product_TestDlg::OpenUSBDevice(CString DeviceName)
{
 //Create File for Device Read/Write
 HidHandle = CreateFile(DeviceName, 
  GENERIC_READ | GENERIC_WRITE, 
  FILE_SHARE_READ | FILE_SHARE_WRITE, 
  (LPSECURITY_ATTRIBUTES)NULL,
  OPEN_EXISTING, 
  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //文件属性与标志,用于指定USB是否进行异步操作，该值为FILE_FLAG_OVERLAPPED，表示使用异步的I/O；该值为0，表示同步I/O操作
  //0, //同步方式,
  NULL
 );
 if (HidHandle == INVALID_HANDLE_VALUE)
 {
  return FALSE;
 }
 //Set Input Buffer Size
 int NumInputBuffers = 2;
 BOOL ret = HidD_SetNumInputBuffers(HidHandle, NumInputBuffers);
 if(!ret)
 {
  return FALSE;
 }
 //Get Preparsed Data
 PHIDP_PREPARSED_DATA PreparsedData;
 ret = HidD_GetPreparsedData(HidHandle, &PreparsedData);
 if(!ret)
 {
  return FALSE;
 }
 //Get Device's Capabilities
 HIDP_CAPS Capabilities;
 HidP_GetCaps(PreparsedData, &Capabilities);
 InputReportLength = Capabilities.InputReportByteLength;
 OutputReportLength = Capabilities.OutputReportByteLength;
 //释放HidD_GetPreparsedData使用的资源
 HidD_FreePreparsedData(PreparsedData);
 return TRUE;
}
void CVRGate_Product_TestDlg::CloseUSBDevice(void)
{
 if (HidHandle != INVALID_HANDLE_VALUE)
  CloseHandle(HidHandle);
 HidHandle = NULL;
}
BOOL CVRGate_Product_TestDlg::SetupDCB(void)
{
 DCB dcb;
 memset(&dcb,0,sizeof(dcb));
 if(!GetCommState(ComHandle,&dcb)) //获取当前DCB配置
 {
  AfxMessageBox(_T("获取DCB配置失败!"));
  return FALSE;
 }
 dcb.DCBlength = sizeof(DCB);
 //serial port config
 dcb.BaudRate = 115200; //波特率
 dcb.ByteSize = 8; //数据位
 dcb.fParity = FALSE; //TRUE;  //是否奇偶校验
 dcb.Parity = NOPARITY; //ODDPARITY;  //奇偶校验方式
 dcb.StopBits = ONESTOPBIT;  //停止位
 dcb.fOutxCtsFlow = 0;
 dcb.fOutxDsrFlow = 0;
 dcb.fDtrControl = DTR_CONTROL_DISABLE;
 dcb.fDsrSensitivity = 0;
 dcb.fRtsControl = RTS_CONTROL_DISABLE;
 dcb.fOutX = 0;
 dcb.fInX = 0;
 //misc parameters
 dcb.fErrorChar = 0;
 dcb.fBinary        = 1; //是否允许传二进制
 dcb.fNull          = 0;
 dcb.fAbortOnError  = 0;
 dcb.wReserved      = 0;
 dcb.XonLim         = 2;
 dcb.XoffLim        = 4;
 dcb.XonChar        = 0x13;
 dcb.XoffChar       = 0x19;
 dcb.EvtChar        = 0;
 if(!SetCommState(ComHandle, &dcb)) //设置DCB
 {
  AfxMessageBox(_T("设置DCB失败!"));
  return FALSE;
 }
 return TRUE;
}
BOOL CVRGate_Product_TestDlg::CheckSerialPort(void)
{
#define MAX_KEY_LENGTH   255
#define MAX_VALUE_NAME  16383
 HKEY hKey;
 if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) )
 {
  TCHAR achClass[MAX_PATH] = TEXT(""); //buffer for class name
  DWORD cchClassName = MAX_PATH; //size of class string
  DWORD cSubKeys=0; //number of subkeys
  DWORD cbMaxSubKey; //longest subkey size
  DWORD cchMaxClass; //longest class string
  DWORD cValues; //number of values for key
  DWORD cchMaxValue; //longest value name
  DWORD cbMaxValueData; //longest value data
  DWORD cbSecurityDescriptor; //size of security descriptor
  FILETIME ftLastWriteTime; //last write time
  DWORD i, retCode;
  TCHAR achValue[MAX_VALUE_NAME];
  DWORD cchValue = MAX_VALUE_NAME;
  //Get the class name and the value count
        retCode = RegQueryInfoKey(
     hKey,    //key handle
     achClass,    //buffer for class name
     &cchClassName,   //size of class string
     NULL,     //reserved
     &cSubKeys,    //number of subkeys
     &cbMaxSubKey,   //longest subkey size
     &cchMaxClass,   //longest class string
     &cValues,    //number of values for this key
     &cchMaxValue,   //longest value name
     &cbMaxValueData,  //longest value data
     &cbSecurityDescriptor, //security descriptor
     &ftLastWriteTime);  //last write time
  if (cValues > 0)
  {
   for (i=0; i<sizeof(m_nComArray)/sizeof(int); i++)
   {
    m_nComArray[i] = -1;
   }
   for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++)
   {
    cchValue = MAX_VALUE_NAME;
    achValue[0] = '\0';
    if (ERROR_SUCCESS == RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL))
    {
     CString szName(achValue);
     if (-1 != szName.Find(_T("Serial")) || -1 != szName.Find(_T("VCom")) || -1 != szName.Find(_T("VCP")))
     {
      BYTE strDSName[10];
      memset(strDSName, 0, 10);
      DWORD nValueType = 0, nBuffLen = 10;
      if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPCTSTR)achValue, NULL, &nValueType, strDSName, &nBuffLen))
      {
       int nIndex = -1;
       while(++nIndex < sizeof(m_nComArray)/sizeof(int))
       {
        if (-1 == m_nComArray[nIndex])
        {
         m_nComArray[nIndex] = atoi((char*)(strDSName + sizeof(TCHAR)*3)); 
         break;
        }
       }
      }
     }
    }
   }
  }
  else
  {
   //AfxMessageBox(_T("PC机没有COM口....."));
   return FALSE;
  }
 }
 RegCloseKey(hKey);
 return TRUE;
}
BOOL CVRGate_Product_TestDlg::SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant)
{
 COMMTIMEOUTS timeouts;
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //读写串口的超时有两种：间隔超时和总超时。
 //间隔超时是指在接收时两个字符之间的最大时延。总超时是指读写操作总共花费的最大时间。
 //写操作只支持总超时，而读操作两种超时均支持。用COMMTIMEOUTS结构可以规定读写操作的超时。
 //
 //COMMTIMEOUTS结构的成员都以毫秒为单位。总超时的计算公式是：
 //总超时＝时间系数×要求读/写的字符数＋时间常量 
 //例如，要读入10个字符，那么读操作的总超时的计算公式为：
 //读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant
 //
 //如果所有写超时参数均为0，那么就不使用写超时
 //如果ReadIntervalTimeout为0，那么就不使用读间隔超时
 //如果ReadTotalTimeoutMultiplier 和 ReadTotalTimeoutConstant 都为0，则不使用读总超时
 //
 //如果读间隔超时被设置成MAXDWORD并且读时间系数和读时间常量都为0，那么在读一次输入缓冲区的内容后读操作就立即返回，而不管是否读入了要求的字符
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //设定读超时
 timeouts.ReadIntervalTimeout = ReadInterval; //读间隔超时
 timeouts.ReadTotalTimeoutConstant = ReadTotalconstant; //读时间系数
 timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier; //读时间常量
 //设定写超时
 timeouts.WriteTotalTimeoutConstant = WriteTotalconstant; //写时间系数
 timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier; //写时间常量
 if(!SetCommTimeouts(ComHandle, &timeouts))
 {
  AfxMessageBox(_T("设置超时时间失败!"));
  return FALSE;
 }
 else
      return TRUE;
}
BOOL CVRGate_Product_TestDlg::OpenSerialDevice(int PortNum)
{
 CString szCom;
 szCom.Format(_T("\\\\.\\COM%d"), PortNum);
 ComHandle = CreateFile(szCom.GetBuffer(50), //逻辑串口号,用字符串”COMX”表示,”X”是串口序号,关于电脑中的逻辑串口号,在注册表HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP目录下,包含字符"Serial"或"VCom"项下面的值就是,可以有多个
   GENERIC_READ | GENERIC_WRITE, //是对串口的访问权限,串口只有读/写(GENERIC_READ/GENERIC_WRITE)权限,可以同时设置成读/写权限,也可以单独只设置读或写权限
   0, //共享模式,有读/写/删除共享,对串口通信只能独占模式,即是0
   NULL, //文件安全属性,对串口设置成NULL
   OPEN_EXISTING, //创建方式,有CREATE_ALWAYS/ CREATE_NEW/ OPEN_EXISTING等方式,但是对串口只能是OPEN_EXISTING,只能打开存在的串口,不像文件一样可创建之类
   FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, //文件属性与标志,用于指定该串口是否进行异步操作，该值为FILE_FLAG_OVERLAPPED，表示使用异步的I/O；该值为0，表示同步I/O操作
   //0, //同步方式
   NULL //文件句柄,新文件从该句柄复制或扩展,如果函数执行成功,返回新的句柄,对串口通信,设置成NULL
 ); 
 if(ComHandle == INVALID_HANDLE_VALUE)
 {
  AfxMessageBox(_T("串口打开失败!"));
  return FALSE;
 }
 SetupComm(ComHandle, 1024, 1024); //输入缓冲区和输出缓冲区的大小均设为1024
 SetupDCB();
 SetupTimeout(0,0,0,0,0); //设置读写超时时间
 PurgeComm(ComHandle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT); //清除输入缓冲区 | 清除输出缓冲区 | 中断所有读操作并立即返回，即使读操作还没有完成 |立即中断所有写操作并立即返回，即使写操作还没有完成
 return TRUE;
}
void CVRGate_Product_TestDlg::CloseSerialDevice(void)
{
 //禁止串行端口所有事件 
 SetCommMask(ComHandle, 0);
 //清除数据终端就绪信号
 EscapeCommFunction(ComHandle, CLRDTR);
 //丢弃通信资源的输出或输入缓冲区字符并终止在通信资源上挂起的读、写操操作
 PurgeComm(ComHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
 CloseHandle(ComHandle);
 ComHandle = NULL;
}
void CVRGate_Product_TestDlg::OnBnClickedFind()
{
 // TODO: 在此添加控件通知处理程序代码
 devicePathName = FindUSBDevice(_T("0483"), _T("5750"));
 if (devicePathName.IsEmpty())
 {
  AfxMessageBox(_T("找不到指定USB设备!"));
  GetDlgItem(IDC_OPEN)->EnableWindow(0);
  GetDlgItem(IDC_CLOSE)->EnableWindow(0);
 }
 else
 {
  if(bComOpened != TRUE)
   GetDlgItem(IDC_OPEN)->EnableWindow(1);
  MessageBox(_T("找到了指定USB设备!"));
 }
}
/*
DWORD WINAPI SerialReadThread(LPVOID pParam)
{
 while(bIsComThreadWorking)
 {
  SerialReceivePacket();
 }
 bIsComThreadStop = 1;//到这里表示已经跳出循环了
 return 0;
}
*/
void CVRGate_Product_TestDlg::OnBnClickedOpen()
{
 // TODO: 在此添加控件通知处理程序代码
 if(OpenUSBDevice(devicePathName) != FALSE)
 {
  GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE)->EnableWindow(TRUE);
  GetDlgItem(IDC_ENTER_DEBUG_MODE)->EnableWindow(TRUE);
  GetDlgItem(IDC_OPEN2)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE2)->EnableWindow(FALSE);
  GetDlgItem(IDC_ENTER_DEBUG_MODE2)->EnableWindow(FALSE);
  GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_EDID2)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(FALSE);
  GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(FALSE);
  GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(FALSE);
  memset(&USBReadOverlapped, 0, sizeof(OVERLAPPED));
  USBReadOverlapped.hEvent = CreateEvent(NULL, //指向SECURITY_ATTRIBUTES结构体，此结构体决定函数的返回句柄是否可以让子进程继承。如果这个参数为NULL，这个句柄是不能继承的
    TRUE, //指定将创建的EVENT是自动复位还是手动复位。如果为TRUE，需要用ResetEvent(HANDLE)函数手动复位状态为无信号，即一旦改EVENT被设置成有信号，则它会一直等到ResetEvent调用时才为无信号状态。如果为FALSE，当一个有信号的等待线程被释放后，系统会自动复位状态为无信号状态
    FALSE, //指定事件对象的初始状态。如果为TRUE，初始状态为有信号，否则为无信号
    NULL //如果为NULL，创建一个没有名称的事件
   );
  memset(&USBWriteOverlapped, 0, sizeof(OVERLAPPED));
  USBWriteOverlapped.hEvent = CreateEvent(NULL, //指向SECURITY_ATTRIBUTES结构体，此结构体决定函数的返回句柄是否可以让子进程继承。如果这个参数为NULL，这个句柄是不能继承的
    TRUE, //指定将创建的EVENT是自动复位还是手动复位。如果为TRUE，需要用ResetEvent(HANDLE)函数手动复位状态为无信号，即一旦改EVENT被设置成有信号，则它会一直等到ResetEvent调用时才为无信号状态。如果为FALSE，当一个有信号的等待线程被释放后，系统会自动复位状态为无信号状态
    FALSE, //指定事件对象的初始状态。如果为TRUE，初始状态为有信号，否则为无信号
    NULL //如果为NULL，创建一个没有名称的事件
   );
  //hUSBThread = CreateThread(NULL, 0, USBReadThread, 0, 0, NULL); //创建读线程
  MessageBox(_T("打开指定USB设备成功!"));
 }
 else
 {
  AfxMessageBox(_T("打开指定USB设备失败，请检查连接并确保设备为非系统独占设备!"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedClose()
{
 // TODO: 在此添加控件通知处理程序代码
 CloseUSBDevice();
 GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
 GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);
 GetDlgItem(IDC_ENTER_DEBUG_MODE)->EnableWindow(FALSE);
 GetDlgItem(IDC_EXIT_DEBUG_MODE)->EnableWindow(FALSE);
 GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_HARDWARE_VERSION)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_SOFTWARE_VERSION)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_PROTOCOL_VERSION)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_EDID)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_8520_REGISTER)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_HDMI_TIMECHK)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_SENSOR)->EnableWindow(FALSE);
 GetDlgItem(IDC_OPEN_BACKLIGHT)->EnableWindow(FALSE);
 GetDlgItem(IDC_CLOSE_PWM)->EnableWindow(FALSE);
 GetDlgItem(IDC_CLOSE_BACKLIGHT)->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO1))->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO2))->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(FALSE);
 GetDlgItem(IDC_SET_SENSOR_SPEED)->EnableWindow(FALSE);
 if(bHaveCom)
  GetDlgItem(IDC_OPEN2)->EnableWindow(TRUE);
 CloseHandle(USBReadOverlapped.hEvent);
 CloseHandle(USBWriteOverlapped.hEvent);
 /*bIsUSBThreadWorking = FALSE;
 while(TRUE)
 {
  if (bIsUSBThreadStop)
  {
   break;
  }
  ::Sleep(10);
 }
 ::Sleep(10);
 CloseHandle(hUSBThread);*/
}
void CVRGate_Product_TestDlg::OnBnClickedOpen2()
{
 // TODO: 在此添加控件通知处理程序代码
 int index = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
 if(OpenSerialDevice(m_nComArray[index]) != FALSE)
 {
  GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);
  GetDlgItem(IDC_ENTER_DEBUG_MODE)->EnableWindow(FALSE);
  GetDlgItem(IDC_EXIT_DEBUG_MODE)->EnableWindow(FALSE);
  GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_HARDWARE_VERSION)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_SOFTWARE_VERSION)->EnableWindow(FALSE);
  GetDlgItem(IDC_GET_PROTOCOL_VERSION)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_EDID)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_8520_REGISTER)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_HDMI_TIMECHK)->EnableWindow(FALSE);
  GetDlgItem(IDC_READ_SENSOR)->EnableWindow(FALSE);
  GetDlgItem(IDC_OPEN_BACKLIGHT)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE_PWM)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE_BACKLIGHT)->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO1))->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO2))->EnableWindow(FALSE);
  ((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(FALSE);
  GetDlgItem(IDC_SET_SENSOR_SPEED)->EnableWindow(FALSE);
  GetDlgItem(IDC_OPEN2)->EnableWindow(FALSE);
  GetDlgItem(IDC_CLOSE2)->EnableWindow(TRUE);
  GetDlgItem(IDC_ENTER_DEBUG_MODE2)->EnableWindow(TRUE);
  GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(TRUE);
  GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(TRUE);
  GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(TRUE);
  GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(TRUE);
  GetDlgItem(IDC_READ_EDID2)->EnableWindow(TRUE);
  GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(TRUE);
  GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(TRUE);
  GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(TRUE);
  GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(TRUE);
  GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(TRUE);
  GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(TRUE);
  ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE); //选上
  ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE); //不选上
  ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE); //不选上
  ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(TRUE);
  ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(TRUE);
  ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(TRUE);
  GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(TRUE);
  memset(&ComReadOverlapped, 0, sizeof(OVERLAPPED));
  ComReadOverlapped.hEvent = CreateEvent(NULL, //指向SECURITY_ATTRIBUTES结构体，此结构体决定函数的返回句柄是否可以让子进程继承。如果这个参数为NULL，这个句柄是不能继承的
     TRUE, //指定将创建的EVENT是自动复位还是手动复位。如果为TRUE，需要用ResetEvent(HANDLE)函数手动复位状态为无信号，即一旦改EVENT被设置成有信号，则它会一直等到ResetEvent调用时才为无信号状态。如果为FALSE，当一个有信号的等待线程被释放后，系统会自动复位状态为无信号状态
     FALSE, //指定事件对象的初始状态。如果为TRUE，初始状态为有信号，否则为无信号
     NULL //如果为NULL，创建一个没有名称的事件
    );
  memset(&ComWriteOverlapped, 0, sizeof(OVERLAPPED));
  ComWriteOverlapped.hEvent = CreateEvent(NULL, //指向SECURITY_ATTRIBUTES结构体，此结构体决定函数的返回句柄是否可以让子进程继承。如果这个参数为NULL，这个句柄是不能继承的
     TRUE, //指定将创建的EVENT是自动复位还是手动复位。如果为TRUE，需要用ResetEvent(HANDLE)函数手动复位状态为无信号，即一旦改EVENT被设置成有信号，则它会一直等到ResetEvent调用时才为无信号状态。如果为FALSE，当一个有信号的等待线程被释放后，系统会自动复位状态为无信号状态
     FALSE, //指定事件对象的初始状态。如果为TRUE，初始状态为有信号，否则为无信号
     NULL //如果为NULL，创建一个没有名称的事件
    );
  bComOpened = TRUE;
  //hCOMThread = CreateThread(NULL, 0, SerialReadThread, 0, 0, NULL); //创建读线程
  MessageBox(_T("打开串口成功."));
 }
 else
 {
  bComOpened = FALSE;
  AfxMessageBox(_T("打开串口失败!")); 
 }
}
void CVRGate_Product_TestDlg::OnBnClickedClose2()
{
 // TODO: 在此添加控件通知处理程序代码
 CloseSerialDevice();
 GetDlgItem(IDC_OPEN2)->EnableWindow(TRUE);
 GetDlgItem(IDC_CLOSE2)->EnableWindow(FALSE);
 GetDlgItem(IDC_ENTER_DEBUG_MODE2)->EnableWindow(FALSE);
 GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(FALSE);
 GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_EDID2)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(FALSE);
 GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(FALSE);
 GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(FALSE);
 GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(FALSE);
 GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(FALSE);
 ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(FALSE);
 GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(FALSE);
 CloseHandle(ComReadOverlapped.hEvent);
 CloseHandle(ComWriteOverlapped.hEvent);
 bComOpened = FALSE;
 /*bIsComThreadWorking = FALSE;
 while(TRUE)
 {
  if (bIsComThreadStop)
  {
   break;
  }
  ::Sleep(10);
 }
 ::Sleep(10);
 CloseHandle(hCOMThread);*/
}
void CVRGate_Product_TestDlg::OnBnClickedEnterDebugMode()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 int count = 0;
 //进入调试模式（暂停自动上传等）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xA0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE1; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  ::Sleep(20); //延时20豪秒，确保下位机程序停止发送数据
  while(USBReceivePacket() != TRUE)
  {
   count++;
   if(count >= 100)
    break;
  }
  //CString s;
  //s.Format(_T("%d"), count);
  //GetDlgItem(IDC_STATUS2)->SetWindowTextW(s);
  if(count < 100)
  {
   DisplayInfoMsg(0, _T("进入调试模式成功。" + InfoMsg));
   GetDlgItem(IDC_EXIT_DEBUG_MODE)->EnableWindow(TRUE);
   GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(TRUE);
   GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_HARDWARE_VERSION)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_SOFTWARE_VERSION)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_PROTOCOL_VERSION)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_EDID)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_8520_REGISTER)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_HDMI_TIMECHK)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_SENSOR)->EnableWindow(TRUE);
   GetDlgItem(IDC_OPEN_BACKLIGHT)->EnableWindow(TRUE);
   GetDlgItem(IDC_CLOSE_PWM)->EnableWindow(TRUE);
   GetDlgItem(IDC_CLOSE_BACKLIGHT)->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE); //选上
   ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE); //不选上
   ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE); //不选上
   ((CButton *)GetDlgItem(IDC_RADIO1))->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO2))->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(TRUE);
   GetDlgItem(IDC_SET_SENSOR_SPEED)->EnableWindow(TRUE);
  }
  else
  {
   DisplayErrMsg(0, _T("进入调试模式失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("进入调试模式失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedEnterDebugMode2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 int count = 0;
 //进入调试模式（暂停自动上传等）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xA0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE1; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  ::Sleep(20); //延时20豪秒，确保下位机程序停止发送数据
  while(ComReceivePacket() != TRUE)
  {
   count++;
   if(count >= 100)
    break;
  }
  if(count < 100)
  {
   DisplayInfoMsg(0, _T("进入调试模式成功。" + InfoMsg));
   GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_EDID2)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(TRUE);
   GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(TRUE);
   GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(TRUE);
   GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(TRUE);
   GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE); //选上
   ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE); //不选上
   ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE); //不选上
   ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(TRUE);
   ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(TRUE);
   GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(TRUE);
  }
  else
  {
   DisplayErrMsg(1, _T("进入调试模式失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("进入调试模式失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedExitDebugMode()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //退出调试模式（恢复自动上传等）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xA0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE0; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("退出调试模式成功。" + InfoMsg));
   GetDlgItem(IDC_ENTER_DEBUG_MODE)->EnableWindow(TRUE);
   GetDlgItem(IDC_EXIT_DEBUG_MODE)->EnableWindow(FALSE);
   GetDlgItem(IDC_UPDATE_FIRMWARE)->EnableWindow(TRUE);
   GetDlgItem(IDC_GET_HARDWARE_VERSION)->EnableWindow(FALSE);
   GetDlgItem(IDC_GET_SOFTWARE_VERSION)->EnableWindow(FALSE);
   GetDlgItem(IDC_GET_PROTOCOL_VERSION)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_EDID)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_8520_REGISTER)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_HDMI_TIMECHK)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_SENSOR)->EnableWindow(FALSE);
   GetDlgItem(IDC_OPEN_BACKLIGHT)->EnableWindow(FALSE);
   GetDlgItem(IDC_CLOSE_PWM)->EnableWindow(FALSE);
   GetDlgItem(IDC_CLOSE_BACKLIGHT)->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO1))->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO2))->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO3))->EnableWindow(FALSE);
   GetDlgItem(IDC_SET_SENSOR_SPEED)->EnableWindow(FALSE);
  }
  else
  {
   DisplayErrMsg(0, _T("退出调试模式失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("退出调试模式失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedExitDebugMode2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //退出调试模式（恢复自动上传等）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xA0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE1; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("退出调试模式成功。" + InfoMsg));
   GetDlgItem(IDC_ENTER_DEBUG_MODE2)->EnableWindow(TRUE);
   GetDlgItem(IDC_EXIT_DEBUG_MODE2)->EnableWindow(FALSE);
   GetDlgItem(IDC_GET_HARDWARE_VERSION2)->EnableWindow(FALSE);
   GetDlgItem(IDC_GET_SOFTWARE_VERSION2)->EnableWindow(FALSE);
   GetDlgItem(IDC_GET_PROTOCOL_VERSION2)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_EDID2)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_8520_REGISTER2)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_HDMI_TIMECHK2)->EnableWindow(FALSE);
   GetDlgItem(IDC_READ_SENSOR2)->EnableWindow(FALSE);
   GetDlgItem(IDC_OPEN_BACKLIGHT2)->EnableWindow(FALSE);
   GetDlgItem(IDC_CLOSE_PWM2)->EnableWindow(FALSE);
   GetDlgItem(IDC_CLOSE_BACKLIGHT2)->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO6))->SetCheck(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO4))->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO5))->EnableWindow(FALSE);
   ((CButton *)GetDlgItem(IDC_RADIO6))->EnableWindow(FALSE);
   GetDlgItem(IDC_SET_SENSOR_SPEED2)->EnableWindow(FALSE);
  }
  else
  {
   DisplayErrMsg(1, _T("退出调试模式失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("退出调试模式失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetHardwareVersion()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //获取版本号-Hardware
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xC0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE1; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("获取硬件版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("获取硬件版本号失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("获取硬件版本号失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetHardwareVersion2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //获取版本号-Hardware
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xC0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE1; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("获取硬件版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("获取硬件版本号失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("获取硬件版本号失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetSoftwareVersion()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //获取版本号-Software
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xC0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE2; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("获取软件版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("获取软件版本号失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("获取软件版本号失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetSoftwareVersion2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //获取版本号-Software
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xC0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE2; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("获取固件版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("获取软件版本号失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("获取软件版本号失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetProtocolVersion()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 DWORD dwBytesWrite = OutputReportLength;
 //获取版本号-Protocol
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xC0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE3; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("获取协议版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("获取协议版本号失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("获取协议版本号失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedGetProtocolVersion2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //获取版本号-Protocol
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xC0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE3; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("获取协议版本号成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("获取协议版本号失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("获取协议版本号失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadEdid()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 DWORD dwBytesWrite = OutputReportLength;
 //测IIC收发-读取EDID信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xD0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE1; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("读取EDID信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("读取EDID信息失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("读取EDID信息失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadEdid2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //测IIC收发-读取EDID信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xD0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE1; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("读取EDID信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("读取EDID信息失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("读取EDID信息失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedRead8520Register()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //测IIC收发-读取8520寄存器信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xD0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE2; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  Read8250RegFlag = TRUE;
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("读取8520寄存器信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("读取8520寄存器信息失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("读取8520寄存器信息失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedRead8520Register2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //测IIC收发-读取8520寄存器信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xD0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE2; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  Read8250RegFlag = TRUE;
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("读取8520寄存器信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("读取8520寄存器信息失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("读取8520寄存器信息失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadHdmiTimechk()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //测IIC收发-读取HDMI TimChk
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xD0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE3; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("读取HDMI TimChk成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("读取HDMI TimChk失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("读取HDMI TimChk失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadHdmiTimechk2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //测IIC收发-读取HDMI TimChk
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xD0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE3; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("读取HDMI TimChk成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("读取HDMI TimChk失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("读取HDMI TimChk失败！串口发送错误！") + ErrMsg);
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadSensor()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //测SPI收发-读取Sensor信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xD0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE5; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("读取Sensor信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("读取Sensor信息失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("读取Sensor信息失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedReadSensor2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //测SPI收发-读取Sensor信息
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xD0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE5; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("读取Sensor信息成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("读取Sensor信息失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("读取Sensor信息失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedOpenBacklight()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //打开背光普通亮度
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xE0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE1; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("打开背光普通亮度成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("打开背光普通亮度失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("打开背光普通亮度失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedOpenBacklight2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //打开背光普通亮度
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xE0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE1; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("打开背光普通亮度成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("打开背光普通亮度失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("打开背光普通亮度失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedClosePwm()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //控制背光最亮（关闭PWM）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xE0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xEF; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("控制背光最亮成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("控制背光最亮失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("控制背光最亮失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedClosePwm2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //控制背光最亮（关闭PWM）
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xE0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xEF; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("控制背光最亮成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("控制背光最亮失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("控制背光最亮失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedCloseBacklight()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //测试屏幕背光-关闭背光
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xE0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE0; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("关闭背光成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("关闭背光失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("关闭背光失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedCloseBacklight2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //测试屏幕背光-关闭背光
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xE0; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xE0; //命令字
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("关闭背光成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("关闭背光失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("关闭背光失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedSetSensorSpeed()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //设置Sensor传输速度
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xE1; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 //int sel = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);
 if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
 {
  sendbuffer[8] = 0xE2; //命令字
 }
 else if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
 {
  sendbuffer[8] = 0xE1; //命令字
 }
 else
 {
  sendbuffer[8] = 0xE0; //命令字
 }
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("设置Sensor传输速度成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("设置Sensor传输速度失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("设置Sensor传输速度失败！USB发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedSetSensorSpeed2()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[64];
 //设置Sensor传输速度
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0xAA; //帧头1
 sendbuffer[1] = 0xBB; //帧头2
 sendbuffer[2] = 1; //帧数据段长度
 sendbuffer[3] = 0xCC; //设备地址
 sendbuffer[4] = 0xE1; //命令组
 sendbuffer[5] = 0xFF; //保留备用
 sendbuffer[6] = 0xFF; //保留备用
 if(((CButton *)GetDlgItem(IDC_RADIO6))->GetCheck())
 {
  sendbuffer[7] = 0xE2; //命令字
 }
 else if(((CButton *)GetDlgItem(IDC_RADIO5))->GetCheck())
 {
  sendbuffer[7] = 0xE1; //命令字
 }
 else
 {
  sendbuffer[7] = 0xE0; //命令字
 }
 sendbuffer[8] = 0x0D; //帧尾1
 sendbuffer[9] = 0x0A; //帧尾2
 if(ComSendPacket(sendbuffer, 10) == TRUE)
 {
  if(ComReceivePacket() == TRUE)
  {
   DisplayInfoMsg(1, _T("设置Sensor传输速度成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(1, _T("设置Sensor传输速度失败！串口接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(1, _T("设置Sensor传输速度失败！串口发送错误！"));
 }
}
void CVRGate_Product_TestDlg::OnBnClickedUpdateFirmware()
{
 // TODO: 在此添加控件通知处理程序代码
 unsigned char sendbuffer[65];
 //进入固件升级模式
 memset(sendbuffer, 0x00, sizeof(sendbuffer));
 sendbuffer[0] = 0x00; //报告ID，必须为0x00，否则WriteFile会返回87号错误
 sendbuffer[1] = 0xAA; //帧头1
 sendbuffer[2] = 0xBB; //帧头2
 sendbuffer[3] = 1; //帧数据段长度
 sendbuffer[4] = 0xCC; //设备地址
 sendbuffer[5] = 0xA0; //命令组
 sendbuffer[6] = 0xFF; //保留备用
 sendbuffer[7] = 0xFF; //保留备用
 sendbuffer[8] = 0xE3; //命令字
 sendbuffer[9] = 0x0D; //帧尾1
 sendbuffer[10] = 0x0A; //帧尾2
 if(USBSendPacket(sendbuffer) == TRUE)
 {
  if(USBReceivePacket() == TRUE)
  {
   DisplayInfoMsg(0, _T("进入固件升级模式成功。") + InfoMsg);
  }
  else
  {
   DisplayErrMsg(0, _T("进入固件升级模式失败！USB接收错误！") + ErrMsg);
  }
 }
 else
 {
  DisplayErrMsg(0, _T("进入固件升级模式失败！USB发送错误！"));
 }
}
HBRUSH CVRGate_Product_TestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
 HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
 // TODO:  在此更改 DC 的任何特性
 if(nCtlColor== CTLCOLOR_STATIC)
 {
  //pDC->SetBkMode(TRANSPARENT);
  pDC->SetTextColor(m_color);
  //return (HBRUSH)m_brush.GetSafeHandle();
 }
 // TODO:  如果默认的不是所需画笔，则返回另一个画笔
 return hbr;
}
void CVRGate_Product_TestDlg::DisplayInfoMsg(int type, LPCTSTR msg)
{
 m_color = RGB(0, 255, 0);//此处改变字体的颜色
 if(type == 0)
 {
  (CStatic*)GetDlgItem(IDC_STATUS)->RedrawWindow();
  GetDlgItem(IDC_STATUS)->SetWindowTextW(msg);
 }
 else
 {
  (CStatic*)GetDlgItem(IDC_STATUS2)->RedrawWindow();
  GetDlgItem(IDC_STATUS2)->SetWindowTextW(msg);
 }
}
void CVRGate_Product_TestDlg::DisplayErrMsg(int type, LPCTSTR msg)
{
 m_color = RGB(255, 0, 0);//此处改变字体的颜色
 if(type == 0)
 {
  (CStatic*)GetDlgItem(IDC_STATUS)->RedrawWindow();
  GetDlgItem(IDC_STATUS)->SetWindowTextW(msg);
 }
 else
 {
  (CStatic*)GetDlgItem(IDC_STATUS2)->RedrawWindow();
  GetDlgItem(IDC_STATUS2)->SetWindowTextW(msg);
 }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VRGate_Product_TestDlg.h : 头文件
//
#pragma once
// CVRGate_Product_TestDlg 对话框
class CVRGate_Product_TestDlg : public CDialogEx
{
#define WM_MY_MESSAGE WM_USER+100
// 构造
public:
 CVRGate_Product_TestDlg(CWnd* pParent = NULL); // 标准构造函数
// 对话框数据
 enum { IDD = IDD_VRGATE_PRODUCT_TEST_DIALOG };
 protected:
 virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持
// 实现
protected:
 HICON m_hIcon;
 // 生成的消息映射函数
 virtual BOOL OnInitDialog();
 afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
 afx_msg void OnPaint();
 afx_msg HCURSOR OnQueryDragIcon();
 afx_msg LRESULT MyMessage(WPARAM wParam, LPARAM lParam);
 DECLARE_MESSAGE_MAP()
private:
 CString FindUSBDevice(CString Vid, CString Pid);
 BOOL SetupDCB(void);
 BOOL SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant);
 BOOL USBReceivePacket(void);
 BOOL USBSendPacket(unsigned char *sendbuffer);
 BOOL OpenUSBDevice(CString DeviceName);
 void CloseUSBDevice(void);
 BOOL CheckSerialPort(void);
 BOOL ComReceivePacket(void);
 BOOL ComSendPacket(unsigned char *sendbuffer, int length);
 BOOL OpenSerialDevice(int PortNum);
 void CloseSerialDevice(void);
 void DisplayInfoMsg(int type, LPCTSTR msg);
 void DisplayErrMsg(int type, LPCTSTR msg);
 int m_nComArray[20];
 BOOL bHaveCom;
 BOOL bComOpened;
 int InputReportLength;
 int OutputReportLength;
 HANDLE HidHandle;
 OVERLAPPED USBReadOverlapped;
 OVERLAPPED USBWriteOverlapped;
 COMSTAT USBReadStat;
 COMSTAT USBWriteStat;
 unsigned char USBRxBuff[65];
 HANDLE ComHandle;
 OVERLAPPED ComReadOverlapped;
 OVERLAPPED ComWriteOverlapped;
 COMSTAT ComReadStat;
 COMSTAT ComWriteStat;
 unsigned char ComRxBuff[64];
 CString devicePathName;
 HANDLE hUSBThread;
 HANDLE hCOMThread;
 CString InfoMsg;
 CString ErrMsg;
 //CFont m_Font;
 //CFont m_Font2;
 COLORREF m_color;
public:
 afx_msg void OnBnClickedFind();
 afx_msg void OnBnClickedOpen();
 afx_msg void OnBnClickedClose();
 afx_msg void OnBnClickedOpen2();
 afx_msg void OnBnClickedClose2();
 afx_msg void OnBnClickedEnterDebugMode();
 afx_msg void OnBnClickedExitDebugMode();
 afx_msg void OnBnClickedGetProtocolVersion();
 afx_msg void OnBnClickedGetHardwareVersion();
 afx_msg void OnBnClickedGetSoftwareVersion();
 afx_msg void OnBnClickedReadEdid();
 afx_msg void OnBnClickedRead8520Register();
 afx_msg void OnBnClickedReadHdmiTimechk();
 afx_msg void OnBnClickedReadSensor();
 afx_msg void OnBnClickedOpenBacklight();
 afx_msg void OnBnClickedCloseBacklight();
 afx_msg void OnBnClickedClosePwm();
 afx_msg void OnBnClickedClose3();
 afx_msg void OnBnClickedSetSensorSpeed();
 afx_msg void OnBnClickedEnterDebugMode2();
 afx_msg void OnBnClickedExitDebugMode2();
 afx_msg void OnBnClickedGetHardwareVersion2();
 afx_msg void OnBnClickedGetSoftwareVersion2();
 afx_msg void OnBnClickedGetProtocolVersion2();
 afx_msg void OnBnClickedReadEdid2();
 afx_msg void OnBnClickedRead8520Register2();
 afx_msg void OnBnClickedReadHdmiTimechk2();
 afx_msg void OnBnClickedReadSensor2();
 afx_msg void OnBnClickedOpenBacklight2();
 afx_msg void OnBnClickedClosePwm2();
 afx_msg void OnBnClickedCloseBacklight2();
 afx_msg void OnBnClickedSetSensorSpeed2();
 afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
 afx_msg void OnBnClickedUpdateFirmware();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
