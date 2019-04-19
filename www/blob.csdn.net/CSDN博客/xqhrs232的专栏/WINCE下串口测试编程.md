# WINCE下串口测试编程 - xqhrs232的专栏 - CSDN博客
2010年01月08日 17:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2406
串口编程存在的几个问题
1。创建串口接收线程函数这个函数为什么必须为静态函数？
2。静态函数里面调到的函数/变量都必须为静态的函数/变量？(通过this指针就可以避免这样)
3。创建串口接收线程函数传的this指针如何灵活使用,当然使用前必须转换一下
4。很多串口数据怎么传出串口接收线程？通过WPARAM/LPARAM传数组数据的个数是不是有一个上限？这个上限是多少？
5。一般的LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)也是一个回调函数啊！
6。回调函数的意义有那些？为什么串口接收线程函数要固定发给回调函数？(通过this指针就可以避免这样)
7。利用好传给串口接收线程的那个this指针就可以避免必须去使用静态成员函数/变量，通过这个指针就可以访问类的一般成员函数/变量了。
网上一个不错的串口参考实例
[http://dl3.csdn.net/fd.php?i=80026885801081&s=274c1fe767fc552505479f66a4652f99](http://dl3.csdn.net/fd.php?i=80026885801081&s=274c1fe767fc552505479f66a4652f99)
汪兵的《Windows+CE/EVC嵌入式高级编程及其实例详解》书里面也有一个不错的串口参考实例
//#################################################################################################################
// 2010_1_7_WINCE50_Com_TestDlg.h : header file
//
#if !defined(AFX_2010_1_7_WINCE50_COM_TESTDLG_H__A48FD2F2_B5CC_46E6_98A0_A9CE2FF7183E__INCLUDED_)
#define AFX_2010_1_7_WINCE50_COM_TESTDLG_H__A48FD2F2_B5CC_46E6_98A0_A9CE2FF7183E__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////
// CMy2010_1_7_WINCE50_Com_TestDlg dialog
class CMy2010_1_7_WINCE50_Com_TestDlg : public CDialog
{
// Construction
public:
 static void ProcessRecData(LPVOID lparam,BYTE* buf,DWORD bufLen);
 void InitButton_State(void);
 // 串口接收线程
 static DWORD WINAPI CommRecvTread(LPVOID lparam);
 void InitCombox_BaudSelect(void);
 void InitCombox_ComSelect(void);
 static HANDLE  ghSerial;
 int m_iCurComNum;
 int m_iCurComBaudRate;
    void Com_Open(CString strCom);
 void Com_Init(int iBaudRate);
 CMy2010_1_7_WINCE50_Com_TestDlg(CWnd* pParent = NULL); // standard constructor
// Dialog Data
 //{{AFX_DATA(CMy2010_1_7_WINCE50_Com_TestDlg)
 enum { IDD = IDD_MY2010_1_7_WINCE50_COM_TEST_DIALOG };
 CComboBox m_BaudRateSelect;
 CComboBox m_ComNumSelect;
 static  CString m_ComReceStr;
 CString m_ComSendStr;
 //}}AFX_DATA
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(CMy2010_1_7_WINCE50_Com_TestDlg)
 protected:
 virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
 virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
 //}}AFX_VIRTUAL
// Implementation
protected:
 HICON m_hIcon;
 // Generated message map functions
 //{{AFX_MSG(CMy2010_1_7_WINCE50_Com_TestDlg)
 virtual BOOL OnInitDialog();
 afx_msg void OnBtnSend();
 afx_msg void OnBtnClearRece();
 afx_msg void OnSelchangeCOMBOBaudSel();
 afx_msg void OnSelchangeCOMBOComSel();
 afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnBtnOpenCom();
    afx_msg  void OnBtnKeyBoardTest();
 afx_msg void OnKillfocusEDITComSend();
 afx_msg void OnSetfocusEDITComSend();
 //}}AFX_MSG
 DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_2010_1_7_WINCE50_COM_TESTDLG_H__A48FD2F2_B5CC_46E6_98A0_A9CE2FF7183E__INCLUDED_)
//#################################################################################################################
// 2010_1_7_WINCE50_Com_TestDlg.cpp : implementation file
//
#include "stdafx.h"
#include "2010_1_7_WINCE50_Com_Test.h"
#include "2010_1_7_WINCE50_Com_TestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<Sipapi.h>
#include "gMsgDef.h"   //2009--10--12  XQH  增加的消息定义！！！
#define   S3C2450   0
#define   S3C2451   1
#define   Platform_Using      S3C2450
#define  WM_ComTest_Base  WM_USER+100 
#define  WM_ComTest_ReceStr    (WM_ComTest_Base+1)
//==============================================================================
CString  ComNumber[4]=
{
 _T("COM1:"),
 _T("COM2:"),
 _T("COM3:"),
 _T("COM4:"),
};
int   ComBaudRate[9]=
{
    300,//0
 600, 
 1200,
    4800,//3
 9600,
 19200,
    38400,//6
 57600,
 115200,
};
const DWORD DataBitTbl[2] = {7, 8};
const BYTE  StopBitTbl[3] = {ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS};
const BYTE  ParityTbl[4] = {NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY};
HANDLE    CMy2010_1_7_WINCE50_Com_TestDlg::ghSerial=NULL;
CString   CMy2010_1_7_WINCE50_Com_TestDlg::m_ComReceStr=L"";
char   NumberToChar(unsigned char data)  //数字转字符
{
    char retval;
   if((data>=0)&&(data<=9))
   {
       retval=data-0+0x30;
   }
   else if((data>=0x0a)&&(data<=0x0f))
   {
        retval=data-0x09+0x40;
   }
   return   retval;
}
unsigned char   CharToNumber( char c)  //字符转数字
{
    unsigned char retval;
   if((c>='0')&&(c<='9'))
   {
       retval=c-0x30;
   }
   else if((c>='a')&&(c<='f'))
   {
        retval=c-0x57;
   }
   else if((c>='A')&&(c<='F'))
   {
        retval=c-0x37;
   }
   return   retval;
}
/////////////////////////////////////////////////////////////////////////////
// CMy2010_1_7_WINCE50_Com_TestDlg dialog
CMy2010_1_7_WINCE50_Com_TestDlg::CMy2010_1_7_WINCE50_Com_TestDlg(CWnd* pParent /*=NULL*/)
 : CDialog(CMy2010_1_7_WINCE50_Com_TestDlg::IDD, pParent)
{
 //{{AFX_DATA_INIT(CMy2010_1_7_WINCE50_Com_TestDlg)
 m_ComReceStr = _T("");
 m_ComSendStr = _T("");
 //}}AFX_DATA_INIT
 // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
 m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::DoDataExchange(CDataExchange* pDX)
{
 CDialog::DoDataExchange(pDX);
 //{{AFX_DATA_MAP(CMy2010_1_7_WINCE50_Com_TestDlg)
 DDX_Control(pDX, IDC_COMBO_BaudSel, m_BaudRateSelect);
 DDX_Control(pDX, IDC_COMBO_ComSel, m_ComNumSelect);
 DDX_Text(pDX, IDC_EDIT_ComRece, m_ComReceStr);
 DDX_Text(pDX, IDC_EDIT_ComSend, m_ComSendStr);
 //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CMy2010_1_7_WINCE50_Com_TestDlg, CDialog)
 //{{AFX_MSG_MAP(CMy2010_1_7_WINCE50_Com_TestDlg)
 ON_BN_CLICKED(IDC_Btn_Send, OnBtnSend)
 ON_BN_CLICKED(IDC_Btn_ClearRece, OnBtnClearRece)
 ON_CBN_SELCHANGE(IDC_COMBO_BaudSel, OnSelchangeCOMBOBaudSel)
 ON_CBN_SELCHANGE(IDC_COMBO_ComSel, OnSelchangeCOMBOComSel)
 ON_WM_TIMER()
    ON_BN_CLICKED(IDC_Btn_OpenCom, OnBtnOpenCom)
    ON_BN_CLICKED(IDC_Btn_KeyBoard_Test, OnBtnKeyBoardTest)
 ON_EN_KILLFOCUS(IDC_EDIT_ComSend, OnKillfocusEDITComSend)
 ON_EN_SETFOCUS(IDC_EDIT_ComSend, OnSetfocusEDITComSend)
 //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMy2010_1_7_WINCE50_Com_TestDlg message handlers
BOOL CMy2010_1_7_WINCE50_Com_TestDlg::OnInitDialog()
{
 CDialog::OnInitDialog();
 // Set the icon for this dialog.  The framework does this automatically
 //  when the application's main window is not a dialog
 SetIcon(m_hIcon, TRUE);   // Set big icon
 SetIcon(m_hIcon, FALSE);  // Set small icon
 CenterWindow(GetDesktopWindow()); // center to the hpc screen
 // TODO: Add extra initialization here
    if(Platform_Using==S3C2450)
  MoveWindow(0,0,480,272);
 else if(Platform_Using==S3C2451)
      MoveWindow(0,0,800,480);
InitCombox_ComSelect();
m_iCurComNum=m_ComNumSelect.SetCurSel(2);
InitCombox_BaudSelect();
m_iCurComBaudRate=m_BaudRateSelect.SetCurSel(5);
//================================================================================================
InitButton_State( );
//==================================================================================================
  //  m_ComSendStr=L"0123456789";  //预置一个值
m_ComSendStr=L"2e 23 24 44 23";  //预置一个值
 UpdateData(FALSE);
// SetTimer(1,1000,NULL);
 return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMy2010_1_7_WINCE50_Com_TestDlg::Com_Open(CString strCom)
{
#if   1
ghSerial = CreateFile(L"COM3:", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#else
ghSerial = CreateFile(strCom, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#endif
if(ghSerial)
  RETAILMSG(1,(TEXT("++++Com_Open( )----CreateFile  in success!/n")));
else
  RETAILMSG(1,(TEXT("++++Com_Open( )----CreateFile  in failure!/n")));
}
void CMy2010_1_7_WINCE50_Com_TestDlg::Com_Init(int iBaudRate)
{
DCB PortDCB; 
PortDCB.DCBlength = sizeof(DCB); 
// 默认串口参数
GetCommState(ghSerial, &PortDCB);
//PortDCB.BaudRate = 115200; // baud
PortDCB.BaudRate =iBaudRate; 
PortDCB.ByteSize = 8; // Number of bits/byte, 4-8 
PortDCB.Parity = NOPARITY; 
PortDCB.StopBits = ONESTOPBIT; 
if (! SetCommState(ghSerial, &PortDCB))
{
 RETAILMSG(1,(TEXT("++++Com_Init( )----SetCommState  in failure!/n")));
}
////配置超时值
COMMTIMEOUTS CommTimeouts;
GetCommTimeouts(ghSerial, &CommTimeouts);
CommTimeouts.ReadIntervalTimeout = MAXDWORD; 
CommTimeouts.ReadTotalTimeoutMultiplier = 10; 
CommTimeouts.ReadTotalTimeoutConstant = 10; 
CommTimeouts.WriteTotalTimeoutMultiplier = 50; 
CommTimeouts.WriteTotalTimeoutConstant = 100; 
if (!SetCommTimeouts(ghSerial, &CommTimeouts))
{
 RETAILMSG(1,(TEXT("++++Com_Init( )----SetCommTimeouts  in failure!/n")));
}
//==================================================
//指定端口监测的事件集
 SetCommMask (ghSerial, EV_RXCHAR);
 //分配串口设备缓冲区
 SetupComm(ghSerial,512,512);
 //初始化缓冲区中的信息
 PurgeComm(ghSerial,PURGE_TXCLEAR|PURGE_RXCLEAR);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnBtnSend() 
{
 // TODO: Add your control notification handler code here
#if  1
#if  0
 DWORD dwactlen;
 if (ghSerial== INVALID_HANDLE_VALUE)
 {
  MessageBox(_T("串口未打开!"));
  return;
 }
    m_ComSendStr=L"0123456789";
 UpdateData(TRUE);
 int len = m_ComSendStr.GetLength();     /* 取得输入字符串长度 */
 char *psendbuf = new char[len];
 for(int i = 0; i < len;i++)
  psendbuf[i] = (char)m_ComSendStr.GetAt(i);   /* 转换为单字节字符 */
 WriteFile(ghSerial, psendbuf, len, &dwactlen, NULL);  /* 从串口发送数据 */
 delete[] psendbuf;
#else
    int i;
 int iStrLen;
 int iDataNum;
    DWORD dwactlen;
    unsigned char Num;
 BYTE *pDataBuf = NULL;
 BYTE* pRealData=NULL;
     UpdateData(TRUE);
  iStrLen=m_ComSendStr.GetLength( );
 pDataBuf = new BYTE[iStrLen];
  iDataNum=0;
  for(i=0;i<iStrLen;i++)
  {
         char c=m_ComSendStr.GetAt(i);
           if(c==_T(' '))
     continue;    
          *(pDataBuf+iDataNum)=CharToNumber(c);
             iDataNum++;  //有效数据加1
        // RETAILMSG(1,(TEXT("++++1111::OnBtnSend()----the i is %d,the data is 0x%x/r/n"),i,*(pDataBuf+i)));
  }
unsigned char  LByte,HByte;
unsigned char  DataByte;
pRealData=new BYTE[iDataNum/2];
    for(i=0;i<(iDataNum/2);i++)
 {
       HByte=*(pDataBuf+2*i);
       LByte=*(pDataBuf+2*i+1);
    DataByte=HByte*16+LByte;
      *(pRealData+i)=DataByte;
     // RETAILMSG(1,(TEXT("++++2222::OnBtnSend()----the i is %d,the data is 0x%x/r/n"),i,*(pRealData+i)));
 }
    WriteFile(ghSerial, pRealData, iDataNum/2, &dwactlen, NULL);
 //释放内存
delete[] pDataBuf;
 pDataBuf = NULL;
 delete[] pRealData;
 pRealData = NULL;
#endif
#else
//while(1)
{
    int  i;
    DWORD dwactlen;
    unsigned char  SendByte[10];
 BOOL  bRet;
 for(i=0;i<10;i++)
 {
      SendByte[i]=0x30+i;
 }
    bRet=WriteFile(ghSerial,SendByte, 10, &dwactlen, NULL);
    if(bRet)
     RETAILMSG(1,(TEXT("++++OnBtnSend()----WriteFile in success!/n")));
 else
     RETAILMSG(1,(TEXT("++++OnBtnSend()----WriteFile in failure!/n")));
 Sleep(1000);
}
#endif
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnBtnClearRece() 
{
 // TODO: Add your control notification handler code here
    m_ComReceStr=_T("");
    UpdateData(FALSE); //更新编辑框内容
}
void CMy2010_1_7_WINCE50_Com_TestDlg::InitCombox_ComSelect()
{
m_ComNumSelect.ResetContent();
m_ComNumSelect.AddString(L"Com1");
m_ComNumSelect.AddString(L"Com2");
m_ComNumSelect.AddString(L"Com3");
m_ComNumSelect.AddString(L"Com4");
}
void CMy2010_1_7_WINCE50_Com_TestDlg::InitCombox_BaudSelect()
{
m_BaudRateSelect.ResetContent();
m_BaudRateSelect.AddString(L"300");
m_BaudRateSelect.AddString(L"600");
m_BaudRateSelect.AddString(L"1200");
m_BaudRateSelect.AddString(L"4800");
m_BaudRateSelect.AddString(L"9600");
m_BaudRateSelect.AddString(L"19200");
m_BaudRateSelect.AddString(L"38400");
m_BaudRateSelect.AddString(L"57600");
m_BaudRateSelect.AddString(L"115200");
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnSelchangeCOMBOComSel() 
{
 // TODO: Add your control notification handler code here
// CloseHandle(ghSerial);
m_iCurComNum=m_ComNumSelect.GetCurSel();
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnSelchangeCOMBOBaudSel() 
{
 // TODO: Add your control notification handler code here
// CloseHandle(ghSerial);
//-----------------------------------------------------------
m_iCurComBaudRate=m_BaudRateSelect.GetCurSel();
}
// 串口接收线程
DWORD CMy2010_1_7_WINCE50_Com_TestDlg::CommRecvTread(LPVOID lparam)
{
 CMy2010_1_7_WINCE50_Com_TestDlg *pDlg = (CMy2010_1_7_WINCE50_Com_TestDlg*)lparam;
    DWORD evtMask;
 BYTE * readBuf = NULL;//读取的字节
 DWORD actualReadLen=0;//实际读取的字节数
 DWORD willReadLen;
 DWORD dwReadErrors;
 COMSTAT cmState;
    DWORD  k;
    CString strtemp;
//清空串口
 PurgeComm(ghSerial, PURGE_RXCLEAR | PURGE_TXCLEAR );
 SetCommMask (ghSerial, EV_RXCHAR);
  while (TRUE) 
  {
        if (WaitCommEvent(ghSerial,&evtMask,0))
  { 
   SetCommMask (ghSerial, EV_RXCHAR);
   if(evtMask & EV_RXCHAR)//表示串口收到字符 
   {
    ClearCommError(ghSerial,&dwReadErrors,&cmState);
    willReadLen=cmState.cbInQue;
    if (willReadLen<=0)
    {
     continue;
    }
    //分配内存
    readBuf = new BYTE[willReadLen];
    ZeroMemory(readBuf,willReadLen);
    //读取串口数据
    ReadFile(ghSerial, readBuf, willReadLen, &actualReadLen,0);
    if (actualReadLen>0) //如果读取的数据大于0
    {
#if  0
/*
                    for(k=0;k<actualReadLen;k++) //将数组转换为Cstring型变量
     {
                      BYTE bt=*(char*)(readBuf+k); //字符型
                      strtemp.Format(L"%02X ",bt); //将字符以十六进制方式送入临时变量strtemp存放，注意这里加入一个空隔
                      m_ComReceStr+=strtemp; //加入接收编辑框对应字符串
     }
*/
              ProcessRecData(lparam,readBuf,actualReadLen);
#else
/*
for(k=0;k<actualReadLen;k++) //将数组转换为Cstring型变量
{
   RETAILMSG(1,(TEXT("++++CommRecvTread()----the k is %d,the data is  0x%x!/n"),k,*(readBuf+k)  ));
}
*/
 CEdit *pRecvStrEdit = (CEdit*)pDlg->GetDlgItem(IDC_EDIT_ComRece);
   pRecvStrEdit->GetWindowText(m_ComReceStr);
   if(m_ComReceStr!=L"")
   m_ComReceStr+=L"/r/n";
  for(k=0;k<actualReadLen;k++) //将数组转换为Cstring型变量
  {
                      BYTE bt=*(char*)(readBuf+k); //字符型
                      strtemp.Format(L"%02X ",bt); //将字符以十六进制方式送入临时变量strtemp存放，注意这里加入一个空隔
                      m_ComReceStr+=strtemp; //加入接收编辑框对应字符串
 }
    pRecvStrEdit->SetWindowText(m_ComReceStr);
#endif
    }
            //   m_ComReceStr+=L"/r/n";
               //UpdateData(FALSE); //更新编辑框内容
       //SendMessage(WM_ComTest_ReceStr,NULL,NULL);
              // ProcessRecData(lparam);
     // pDlg->PostMessage(WM_ComTest_ReceStr,NULL,NULL);  //ok
    //this->PostMessage(WM_ComTest_ReceStr,NULL,NULL);
    delete[] readBuf;//释放内存
    readBuf = NULL;
   }//if (evtMask & EV_RXCHAR) 
  }//if (WaitCommEvent(ghSerial,&evtMask,0))
  }//while (TRUE) 
 return 0;
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnTimer(UINT nIDEvent) 
{
 // TODO: Add your message handler code here and/or call default
    if(nIDEvent==1)
 {
       RETAILMSG(1,(TEXT("++++OnTimer( )----if(nIDEvent==1)!/n")));
       UpdateData(FALSE); //更新编辑框内容
 }
 CDialog::OnTimer(nIDEvent);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnBtnOpenCom() 
{
CString  str;
str=ComNumber[m_iCurComNum];
#if  1
Com_Open(str);
#else
#if   1
    ghSerial = CreateFile(L"COM3:", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#else
     ghSerial = CreateFile(str,GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
#endif
if(ghSerial)
  RETAILMSG(1,(TEXT("++++Com_Open( )----CreateFile  in success!/n")));
else
  RETAILMSG(1,(TEXT("++++Com_Open( )----CreateFile  in failure!/n")));
#endif
int  iBaudRate;
iBaudRate=ComBaudRate[m_iCurComBaudRate];
Com_Init(iBaudRate);
//=================================================================================================
 DWORD IDThread;
 HANDLE hRecvThread; 
// 创建串口接收线程
 hRecvThread = CreateThread(0, 0, CommRecvTread, this, 0, &IDThread);
   //hRecvThread = CreateThread(0, 0, CommRecvTread, NULL, 0, &IDThread);  //NO ok
 if (hRecvThread == NULL) 
 {
   RETAILMSG(1,(TEXT("++++OnInitDialog( )----CreateThread  in failure!/n")));
 } 
 CloseHandle(hRecvThread);
//=====================================
CButton* btn0=(CButton*)GetDlgItem(IDC_Btn_OpenCom);
btn0->EnableWindow(FALSE);
CButton* btn1=(CButton*)GetDlgItem(IDC_Btn_Send);
btn1->EnableWindow(TRUE);
CButton* btn2=(CButton*)GetDlgItem(IDC_Btn_ClearRece);
btn2->EnableWindow(TRUE);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::InitButton_State()
{
CButton* btn0=(CButton*)GetDlgItem(IDC_Btn_OpenCom);
btn0->EnableWindow(TRUE);
CButton* btn1=(CButton*)GetDlgItem(IDC_Btn_Send);
btn1->EnableWindow(FALSE);
CButton* btn2=(CButton*)GetDlgItem(IDC_Btn_ClearRece);
btn2->EnableWindow(FALSE);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnBtnKeyBoardTest() 
{
static  int  iStatus=0;
iStatus++;
if(iStatus%2==1)
SipShowIM(1);
else
SipShowIM(0);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnKillfocusEDITComSend() 
{
 // TODO: Add your control notification handler code here
 SipShowIM(0);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::OnSetfocusEDITComSend() 
{
 // TODO: Add your control notification handler code here
SipShowIM(1);
}
void CMy2010_1_7_WINCE50_Com_TestDlg::ProcessRecData(LPVOID lparam,BYTE* buf,DWORD bufLen)
{
 CMy2010_1_7_WINCE50_Com_TestDlg *pDlg = (CMy2010_1_7_WINCE50_Com_TestDlg*)lparam;
 // pDlg->PostMessage(WM_RECV_SERIAL_DATA, buf,bufLen);
 pDlg->PostMessage(WM_RECV_SERIAL_DATA, WPARAM(buf),bufLen);
}
LRESULT CMy2010_1_7_WINCE50_Com_TestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
 // TODO: Add your specialized code here and/or call the base class
 int k;
 CString  strtemp;
    BYTE bt;
BYTE *pBuf;
DWORD dwBufLen;
 switch(message)
   {
    case   WM_RECV_SERIAL_DATA:
 pBuf=(BYTE*)wParam;
 dwBufLen=lParam;
for(k=0;k<dwBufLen;k++) //将数组转换为Cstring型变量
  {
                      bt=*(BYTE*)(pBuf+k); //字符型
                      strtemp.Format(L"%02X ",bt); //将字符以十六进制方式送入临时变量strtemp存放，注意这里加入一个空隔
                      m_ComReceStr+=strtemp; //加入接收编辑框对应字符串
 }
 //释放内存
 delete[] pBuf;
 pBuf = NULL;
 m_ComReceStr+=L"/r/n";
UpdateData(FALSE); //更新编辑框内容
  break;
   case   WM_SPI_DirectionKey:
      RETAILMSG(1,(TEXT("[HSP] ++++WindowProc(  )----case   WM_SPI_DirectionKey/r/n"))); 
    break;
 case   WM_SPI_PanelKey:
      RETAILMSG(1,(TEXT("[HSP] ++++WindowProc(  )----case   WM_SPI_PanelKey/r/n"))); 
    break;
   }
 return CDialog::WindowProc(message, wParam, lParam);
}
