# VC基于MSCOMM控件串口通讯 - xqhrs232的专栏 - CSDN博客
2011年01月26日 16:57:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1643标签：[microsoft																[编程																[windows																[input																[api																[通讯](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://sytlm.ycool.com/post.2593142.html](http://sytlm.ycool.com/post.2593142.html)
在mfc中进行串口通讯最简单的方法莫过于在对话框中使用MSCOMM控件了，MSComm通信控件提供了一系列标准通信命令的接口，它允许建立串口连接，可以连接到其他通信设备（如Modem）．
还可以发送命令、进行数据交换以及监视和响应在通信过程中可能发生的各种错误和事件，从而可以用它创建全双工 、事件驱动的、高效实用的通信程序。
一、用MSComm控件通信
1．串口通信基础知识
    一般悦来，计算机都有一个或多个串行端口，它们依次为com1、Com2、…，这些串口还提供了外部设备与pC进行数据传输和
皿信的通道。这些串口在CPU和外设之间充当解释器的角色。当字符数据从CPU发送给外设时，这些字符数据将被转换成串行比特
流数据；当接收数据时，比特流数据被转换为字符数据传递给CPU，再进一步说，在操作系统方面，Windows用通信驱动程序
（COMM.DRV）调用API函数发送和接收数据，当用通信控件或声明调用API函数时，它门由COMM. DRV解释并传递给设备驱动程序，
作为一个vB程序员，要编写通信程序．只需知道通信控件提供给Windows通信AP1函数的接口即可．换句话说，只需设定和监视通
信控件的属性和事件即可。
2．使用Mscomm控件
**在开始使用MSComm控件之前。需要先了解其属性、事件或错误**
属性            描述
CommPort    设置或返回通信端口号
Settings    以字符串的形式设置或返回波特率、奇偶校验、数据位和停止位
PortOpen    设置或返回通信端口的状态。也可以打开和关闭端口
Input       返回和删除接收缓冲区中的字符
Output      将字符串写入发送缓冲区
**CommEvent属性为通信事件或错误返回下列值之一。在该控件的对象库中也可以找到这些常量。**
常量           值        描述
ComEventBreak   1001    收到了断开信号
ComEventCTSTO   1002    Clear To Send Timeout。在发送字符时，在系统指定的事1件内，CTS（Clear To Send）线是低电平
ComEventDSRTO   1003    Data Set Ready Timeout。在发送字符时，在系统指定的事件内，DSR（Data Set Ready）线是低电平
ComEventFrame   1004    数据帧错误。硬件检测到一个数据帧错误
ComEventOverrun 1006    端口溢出。硬件中的字符尚未读，下一个字符又到达，并且丢失
ComEventCDTO    1007    Carrier Detect Time。在发送字符时，在系统指定的事件内，CD（Carrier Detect）线是低电平。CD
                        也称为RLSD（Receive Line Singal Detect，接收线信号检测）
ComEventRxOver  1008    接收缓冲区溢出。在接收缓冲区中没有空间
ComEventRxParity 1009   奇偶校验错。硬件检测到奇偶校验错误7
ComEventTxFull  1010    发送缓冲区满。在对发送字符排队时，发送缓冲区满
ComEventDCB     1011    检取端口DCB（Device Control Blick）时发生了没有预料到的错误
**通信事件包含了下面的设置：**
常量         值        描述
ComEvSend      1    发送缓冲区中的字符数比Sthreshold值低
ComEvReceive   2    接收到了Rthreshold个字符。持续产生该事件，直到使用了Input属性删除了接收缓冲区中的数据
ComEvCTS       3    CTS（Clear To Send）线改变
ComEvDSR       4    DSR（Data Set Ready）线改变。当DSR从1到0改变时，该事件发生
ComEvCD        5    CD（Carrier Detect）线改变ComEvRing6检测到响铃信号。一些URAT（Universal AsynchronousReciver-
                    -Transmitters,通用异步收发器）不支持该事件
ComEvEOF       7    收到了EOF字符（ASCII字符26）
**Error消息（MSComm控件）下表列出了MSComm控件可捕获的错误消息：**
常量                       值      描述
ComInvalidPropertyValue    380   无效的属性值
ComSetNotSupported         383   属性只读
ComGetNotSupported         394   属性只读
ComPortOpen               8000   端口打开时该存在无效
                          8001   超时设置必须比0值大
ComPortInvalid            8002   无效的端口号
                          8003   属性只在运行时有效
                          8004   属性在运行时是只读的
ComPortAleadyOpen         8005   端口已经打开
                          8006   设备标识符无效或不支持
                          8007   不支持设备的波特率
                          8008   指定的字节大小无效
                          8009   缺省参数错误
                          8010   硬件不可用（被其他设备锁住）
                          8011   函数不能分配队列
ComNoOpen                 8012   设备没有打开
                          8013   设备已经打开
                          8014   不能使用通信通知
ComSetCommStateFailed     8015   不能设置通信状态
                          8016   不能设置通信事件屏蔽
ComPortNotOpen            8018   该存在只在端口打开是有效
                          8019   设备忙
ComReadError              8020   通信设备读错误
ComDCBError               8021   检取端口设备控制块时出现内部错误
注意在使用的时候一定要保证两个通讯串口的设置是相同的，否则受到的信息将会产生错误！
由于取值位数的不同，有可能发送的信息要读很多次才能组合成需要的信息！
1。建立mfc工程，都会撒。  
   将控件加进来：打开“Project->Add To Project->Components and Controls->Registered Activex Controls”,然后选择控件：Microsoft Communication Control,version 6.0插入到当前的工程中。这样就将类 CMSComm 的相关文件 mscomm.cpp 和 mscomm.h 一并加入到了工程中。编程时只需将控件对话中的 MSComm 控件拖至你的应用对话框中就OK了
2。定义串口对象：
    CMSComm      m_MSComm;
3。串口初始化：
DWORD style=WS_VISIBLE;
m_MSComm.Create(NULL,style,CRect(0,0,0,0),this,IDC_MSCOMM);
if(m_MSComm.GetPortOpen()) //如果串口是打开的，则行关闭串口
{
 m_MSComm.SetPortOpen(FALSE);
}
m_MSComm.SetCommPort(1); //选择COM1
m_MSComm.SetInBufferSize(1024); //接收缓冲区
m_MSComm.SetOutBufferSize(1024);//发送缓冲区
m_MSComm.SetInputLen(0);//设置当前接收区数据长度为0,表示全部读取
m_MSComm.SetInputMode(1);//以二进制方式读写数据
m_MSComm.SetRThreshold(1);//接收缓冲区有1个及1个以上字符时，将引发接收数据的OnComm事件
m_MSComm.SetSettings("9600,n,8,1");//波特率9600无检验位，8个数据位，1个停止位
if(!m_MSComm.GetPortOpen())//如果串口没有打开则打开
 m_MSComm.SetPortOpen(TRUE);//打开串口
else
{
 m_MSComm.SetOutBufferCount(0);
 AfxMessageBox("Open The Serial Port 1 Failurre!");
}
4。串口数据读写：
　　MSComm 类的读写函数比较简单：GetInput()和SetOutput()。函数原形分别为VARIANT GetInput()和void SetOutput（const VARIANT newValue）,均使用VARIANT类型。但PC机发送和接收数据时习惯用字符串形式。MSDN中查阅VARIANT类型，可以用BSTR表示字符串，但所有的BSTR都包含宽字符，而只有Windows NT支持宽字符，Windows 9X并不支持。所以要完成一个适应各平台的串口应用程序必须解决这个问题。这里使用CbyteArray即可解决之。
发数据：在对话框对加入 按钮 控件并给你添加消息
   void CTest_mscommDlg::OnSend() 
{
 // TODO: Add your control notification handler code here
 int i,Count;
 CString m_SendData;
 m_SendData="Hello!"; 
 Count=m_SendData.GetLength();
 CByteArray m_Array;
 m_Array.RemoveAll();
 m_Array.SetSize(Count);
 for(i=0;i  m_Array.SetAt(i,m_SendData[i]);
 m_MSComm.SetOutput(COleVariant(m_Array));
}
收数据：给串口控件添加消息 
void CTest_mscommDlg::OnOnCommMscomm() 
{
 VARIANT m_input;
 char *str,*str1;
 int k,nEvent,i;
 CString str2,m_RcvData;
 nEvent=m_MSComm.GetCommEvent();
 switch(nEvent)
 {
 case 2:
  k=m_MSComm.GetInBufferCount();     //接收缓冲区的字符数目
  if(k>0)
  {
   m_input=m_MSComm.GetInput();
   str=(char*)(unsigned char*)m_input.parray->pvData;
  }
  i=0;
  str1=str;
  while(i  {
   i++;
   str1++;
  }
  *str1='{post.content}';                              
  str2=(const char*)str;             //清除字符串中的不必要字符
  m_RcvData=(const char *)str;
 }
 //数据显示处理
m_disp+=m_RcvData;
UpdateData(false);
}
 ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
摘要：本文介绍了在Microsoft Visual C++ 6.0环境下通过对Active X控件的编程来实现串口的通信的一般方法。 
　　 一、 引言 
　　 当我们在Windows操作系统下开发串行通信程序时通常不得不面对许多复杂的API函数，因为在Windows操作系统下不能直接对设备端口进行操作，也不能在系统级（Ring 3级别）使用任何DOS或BIOS中断，如要对端口进行编程则只能以文件的形式来对端口进行操作，这就使开发人员不得不面对非常烦琐的API函数编程。本文对此提出了另外一种封装性很好的使用Microsoft Visual C++ 6.0自带的"Microsoft Communications Control，version 6.0"Active
 X控件的编程方法，通过对该控件的正确使用，我们可以比较轻松地编写出所需的串行通信程序。 
　　 下面，我们将结合一个实际的程序示例来对此方法进行说明。本程序的编程环境是Windows 98和Microsoft Visual C++ 6.0。在本程序示例中对为避免阻塞而对线程的使用以及在使用中遇到的一些问题也做了详细的介绍。
　　 二、 程序的设计实现 
　　 在开始进行代码编程前，首先以在工程中插入组件或控件的方式将Active X控件"Microsoft Communications Control，version 6.0"加入到工程中来，此时将会在工程中添加一个关于此控件的新类。使用该控件的一些方法和属性时不能象使用类一样简单的声明一个实例对象，而要通ClassWizard为该控件和一个成员变量建立起绑定关系，在此我们将该控件同变量m_Comm相绑定后就可以通过该控件提供的方法来对串口的各种通讯参数进行设置了。为了编程方便起见，也可以在资源视图中直接对该控件的属性进行设置，如无特别要求，对下表所列属性进行设置就基本可以满足编程要求了。现将常用的属性列表如下：
属性 设定值 属性说明 
CommPort 1 串口号，一般从1到4 
InBufferSize 30720 接收缓冲区大小，为保持程序的稳定，建议设得值足够大 
InputMode 0-Text 接收数据的类型，0表示文本类型，1表示二进制类型 
InputLen 0 从接收缓冲区读取的字节数，0表示全部读取 
OutBufferSize 512 发送缓冲区大小 
Settings 4800,n,8,1 串口的参数设置,依次为波特率、奇偶校验(n-无校验,e-偶校验,o-奇校验)、数据位数、停止位数 
RThreshold 1 设定当接收几个字符时触发OnComm事件，0表示不产生事件， 
1表示每接收一个字符就产生一个事件 
SThreshold 0 设定在触发OnComm事件前，发送缓冲区内所允许的最少的字符数， 
0表示发送数据时不产生事件，1表示当发送缓冲区空时产生OnComm事件 
　　 我们要求能在程序启动的同时就打开串口以便即时对从串口到达的数据进行接收、处理。一般来说可以将下面的打开端口的代码写在OnCreate()、OnInitialUpdate()、InitInstance ()等程序入口函数中：
…… 
if(!m_Comm.GetPortOpen()) //检测是否已经打开过端口 
m_Comm.SetPortOpen(TRUE); //如没有打开则将端口打开 
…… 
接下来的工作就是对数据的发送与接收了，这也是本文所要介绍的重点所在。发送数据的代码原则上是可以写到一个成员函数中被直接调用的，但这并不是一个良好的编程习惯：我们应当把比较耗时的操作，如文件拷贝、打印、端口传输等工作放到一个单独的线程当中，以避免其在工作时会引起整个进程的阻塞，以提高整个系统对CPU的利用率。例如我们可以在视类中菜单或按钮的响应函数中用AfxBeginThread(WriteProc,this)函数来开启一个名为"WriteProc"的线程，由于在线程中还需要使用视类的函数和变量，为了不产生新的视类的实例对象，我们通过该函数的第二个参数将指向当前的视类的指针this作为参数传递给线程。在线程中可以用如下两种方法之中的一种调用视类的成员函数：
((COLECommView*) pParam)->DoSendProc(); 
　　 或是： 
COLECommView* view=(COLECommView*) pParam; 
View->DoSendProc(); 
　　 其中从pParam传来的变量就是指向视类的指针。在线程中通过调用视类中的DoSendProc函数来完成对数据的发送，正是由于该函数是被全局的线程所调用的，我们就不可以使用取编辑框上的数据时通常所用的UpdateData()函数了，取而带之的是API 函数GetDlgItemText()，取到输入的数据后通过控件的SetOutput() 方法就把数据从串口发出去了，其中发送数据必须经ColeVariant类将其转换为通用的VARIANT型变量。实现
主要代码如下： 
…… 
char a[255]; 
HWND hwnd=GetSafeHwnd(); 
::GetDlgItemText(hwnd,IDC_ED99v1,a,255); 
int i=0; 
CString str; 
while(a[i]!='{post.content}') 
{ 
str.Format("%c",a[i]); 
m_SendData+=str; 
i++; 
} 
str.Format("%c",10); 
m_SendData+=str; 
m_Comm.SetOutput(COleVariant(m_SendData)); 
…… 
　　 至于数据的接收，我们可以通过让MS Comm控件响应其OnComm事件来完成，通过ClassWizard加入其对事件的响应后，通过下面的事件映射，当有字符到达时便会通知 OnComm()函数去处理，从而实现数据的异步接收：
…… 
BEGIN_EVENTSINK_MAP(COLECommView, CFormView) 
//{{AFX_EVENTSINK_MAP(COLECommView) 
ON_EVENT(COLECommView, IDC_MSCOMM1, 1 /* OnComm */, OnComm, VTS_NONE) 
//}}AFX_EVENTSINK_MAP 
END_EVENTSINK_MAP() 
…… 
void COLECommView::OnComm() 
{ 
VARIANT Input; 
if(m_Comm.GetCommEvent()==2)//接收缓冲区内有字符 
{ 
Input=m_Comm.GetInput();//读取缓冲区内的数据 
CString msg=Input.bstrVal; 
CString str; 
str.Format("%c",10); 
if(msg.Right(1)==str) 
{ 
m_RecvData+=msg; 
m_History.AddString(m_RecvData); 
m_RecvData=""; 
} 
else 
m_RecvData+=msg; 
} 
} 
　　 当数据被接收到接收缓冲区后，对于字符可以从VARIANT型结构变量的bstrVal成员变量中获取，VARIANT数据结构相当复杂，并牵扯到COM(Component Object Model，组件对象模型)中的一些概念，具体详情请参阅Microsoft Corpration发布的Msdn
