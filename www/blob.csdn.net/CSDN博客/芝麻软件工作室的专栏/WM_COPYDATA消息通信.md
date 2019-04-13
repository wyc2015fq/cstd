
# WM_COPYDATA消息通信 -  芝麻软件工作室的专栏 - CSDN博客


2012年06月21日 18:31:28[seven-soft](https://me.csdn.net/softn)阅读数：420


进程通信——使用WM_COPYDATA消息通信
1　发送消息的程序中比较简单，在要发送的地方加入如下代码就行了：
void CSendDlg::OnSend()
{
// TODO: Add your control notification handler code here
CString str="这是我要发送的给另外一个进程字符串。";
COPYDATASTRUCT myCopyDATA;
myCopyDATA.cbData=str.GetLength();
myCopyDATA.lpData=str.GetBuffer(0);
str.ReleaseBuffer();
HWND hwnd=::FindWindow(NULL,"Receive");//假设目标程序窗口标题为"Receive"
if (hwnd)
::SendMessage(hwnd,WM_COPYDATA,NULL,(LPARAM)&myCopyDATA);
else
AfxMessageBox("目标程序没有运行。");
}
2　接收消息的程序(这里假设是基于对话框的工程)请按如下步骤来实现：
先给对话框类CReceiveDlg(如果你的接收程序为SDI工程的话，把CReceiveDlg类换成CMainFrame类)添加一个消息处理函数LRESULTOnReceive(WPARAM wParam,LPARAM lParam)，代码如下：
LRESULT CReceiveDlg::OnReceive(WPARAM wParam,LPARAM lParam)
{
COPYDATASTRUCT *p = NULL ;
p = (COPYDATASTRUCT*)lParam;
CString strTemp;
memcpy(strTemp.GetBuffer((int)p->cbData),p->lpData,p->cbData);
strTemp.ReleaseBuffer();
AfxMessageBox(strTemp);
return 0;
}
接着再给工程设定刚才添加的函数OnReceive为WM_COPYDATA消息的响应函数，在ReceiveDlg.cpp文件中如下地方加入一行代码：
BEGIN_MESSAGE_MAP(CReceiveDlg, CDialog)
//{{AFX_MSG_MAP(CReceiveDlg)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_MESSAGE(WM_COPYDATA,OnReceive) //只要添加这一行进行WM_COPYDATA消息映射
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
好了，现在分别运行两个程序，应该可以在A程序中给B程序发送消息了，B收到消息后会弹出个MessageBox来显示从A程序发送来消息了。



3.4  使用WM_COPYDATA消息通信
对于少量数据可以用WM_COPYDATA方便地实现通信。由于SendMessage()是阻塞的，只有接收方响应了消息，SendMessage()才能返回，否则一直阻塞。所以，对于大量数据来说，用SendMessage()就容易造成窗口假死。
3.4.1  通过WM_COPYDATA消息实现进程间通信的方法
在Win32中，WM_COPYDATA消息主要目的是允许在进程间传递只读数据。SDK文档推荐用户使用SendMessage()函数，接收方在数据复制完成前不返回，这样发送方就不可能删除和修改数据。这个函数的原型如下:
SendMessage(WM_COPYDATA,wParam,lParam)
其中wParam设置为包含数据的窗口句柄，lParam指向一个COPYDATASTRUCT的结构，其定义为：
typedef struct tagCOPYDATASTRUCT{
DWORD dwData;
DWORD cbData;
PVOID lpData;
}COPYDATASTRUCT;
其中dwData为自定义数据， cbData为数据大小，lpData为指向数据的指针。需要注意的是，WM_COPYDATA消息保证发送的数据从原进程复制到目标进程。但是，WM_COPYDATA消息不能发送HDC、HBITMAP之类的东西，它们对于目标进程来说是无效的。目标进程得到这些数据不能在原进程作任何事情，因为它们属于不同的进程。
与其他进程通信方法一样，要实现进程间的数据通信，在发送数据的程序中，首先要找到接收数据进程的窗口句柄pWnd，可以用CWnd::FindWindow(NULL,_T("DataRecv"))函数来得到，其中字符串"DataRecv"为接收数据的程序名。然后用SendMessage()函数发送数据，其具体的做法见后面的实例。
在接收数据的程序中，首先在消息映射表中增加WM_COPYDATA消息映射，然后定义消息映射函数，其函数的格式为：
BOOL CDataRecvDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT*pCopyDataStruct)
{
// 增加用户自定义程序代码
…
}
3.4.2  通过WM_COPYDATA消息实现进程间通信的实例
与前面所说的自定义消息不一样，WM_COPYDATA消息是Win32提供的消息。与自定义消息相比较，WM_COPYDATA消息可以传递一个较大的数据块。这里仍然用两个对话框程序来实现WM_COPYDATA消息的通信。
以下分别给出发送数据程序的发送函数和接收数据程序的接收函数。在发送数据的对话框类CDataSendDlg中，用MFCClassWizard工具或者手工的方法增加函数voidCDataSendDlg::OnSendCopydata()，其具体代码如下：
void CDataSendDlg::OnSendCopydata()
{
UpdateData();                                      //
 更新数据
CWnd*pWnd=CWnd::FindWindow(NULL,_T("DataRecv"));   // 查找DataRecv进程
if(pWnd==NULL){
AfxMessageBox("Unable to find DataRecv.");
return;
}
COPYDATASTRUCTcpd;                    // 给COPYDATASTRUCT结构赋值
cpd.dwData = 0;
cpd.cbData = m_strCopyData.GetLength();
cpd.lpData = (void*)m_strCopyData.GetBuffer(cpd.cbData);
pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);  // 发送
}
在用MFCAppWizard(exe)创建接收数据的对话框程序后，生成对话框类CDataRecvDlg。在这个类中，首先要定义接收WM_COPYDATA消息的映射，可以用ClassWizard工具来增加，也可以手动增加，但手动增加需要修改三个地方：①在消息映射表中增加ON_WM_COPYDATA()；②增加成员函数BOOLCDataRecvDlg::OnCopyData()；③在CDataRecvDlg类中增加WM_COPYDATA消息映射函数的定义。
WM_COPYDATA消息的映射如下：
BEGIN_MESSAGE_MAP(CDataRecvDlg, CDialog)
//{{AFX_MSG_MAP(CDataRecvDlg)
ON_WM_COPYDATA()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CDataRecvDlg::OnCopyData()函数的定义如下：
BOOL CDataRecvDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT*pCopyDataStruct)
{
m_strCopyData=(LPSTR)pCopyDataStruct->lpData;
//获得实际长度的字符串
m_strCopyData=m_strCopyData.Left(pCopyDataStruct->cbData);
//更新数据
UpdateData(FALSE);
returnCDialog::OnCopyData(pWnd, pCopyDataStruct);
}
其中m_strCopyData为接收到的字符串，pCopyDataStruct为COPYDATASTRUCT结构指针。注意由pCopyDataStruct直接得到的m_strCopyData字符串长度可能不是实际发送的字符串长度，需要用发送字符串时所给定的字符串长度来进一步确定，其长度由pCopyDataStruct->cbData来得到。


