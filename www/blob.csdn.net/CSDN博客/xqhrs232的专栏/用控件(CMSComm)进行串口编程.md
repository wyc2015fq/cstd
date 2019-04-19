# 用控件(CMSComm)进行串口编程 - xqhrs232的专栏 - CSDN博客
2011年01月26日 16:54:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1305标签：[编程																[microsoft																[initialization																[byte																[components																[styles](https://so.csdn.net/so/search/s.do?q=styles&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://blog.csdn.net/luheng2006/archive/2010/04/16/5491801.aspx](http://blog.csdn.net/luheng2006/archive/2010/04/16/5491801.aspx)
1.建立项目  
2.在项目中插入MSComm控件
3.利用ClassWizard定义CMSComm类控制变量
4.在对话框中添加控件  
5.添加串口事件消息处理函数OnComm()
6.打开和设置串口参数
7.发送数据
开始吧：
1.建立项目：打开VC＋＋6.0，建立一个基于对话框的MFC应用程序SCommTest（与我源代码一致，等会你会方便一点）；
2.在项目中插入MSComm控件    选择Project菜单下Add To Project子菜单中的 Components and Controls…选项，在弹出的对话框中双击Registered ActiveX Controls项（稍等一会，这个过程较慢），则所有注册过的ActiveX控件出现在列表框中。 选择Microsoft Communications Control, version 6.0，，单击Insert按钮将它插入到我们的Project中来，接受缺省的选项。（如果你在控件列表中看不到Microsoft
 Communications Control, version 6.0，那可能是你在安装VC6时没有把ActiveX一项选上，重新安装VC6，选上ActiveX就可以了），
这时在ClassView视窗中就可以看到CMSComm类了，（注意：此类在ClassWizard中看不到，重构clw文件也一样），并且在控件工具栏Controls中出现了电话图标（如图1所示），现在要做的是用鼠标将此图标拖到对话框中，程序运行后，这个图标是看不到的。
3.利用ClassWizard定义CMSComm类控制对象   打开ClassWizard－>Member Viariables选项卡，选择CSCommTestDlg类，为IDC_MSCOMM1添加控制变量：m_ctrlComm，这时你可以看一看，在对话框头文件中自动加入了//{{AFX_INCLUDES()   #include "mscomm.h"   //}}AFX_INCLUDES （这时运行程序，如果有错，那就再从头开始）。
4.在对话框中添加控件   向主对话框中添加两个编辑框，一个用于接收显示数据ID为IDC_EDIT_RXDATA，另一个用于输入发送数据，ID为IDC_EDIT_TXDATA，再添加一个按钮，功能是按一次就把发送编辑框中的内容发送一次，将其ID设为IDC_BUTTON_MANUALSEND。别忘记了将接收编辑框的Properties－>Styles中把Miltiline和Vertical Scroll属性选上，发送编辑框若你想输入多行文字，也可选上Miltiline。
再打开ClassWizard－>Member Viariables选项卡，选择CSCommTestDlg类， 为IDC_EDIT_RXDATA添加CString变量m_strRXData， 为IDC_EDIT_TXDATA添加CString变量m_strTXData。说明： m_strRXData和m_strTXData分别用来放入接收和发送的字符数据。
5.添加串口事件消息处理函数OnComm() 打开ClassWizard－>Message Maps，选择类CSCommTestDlg，选择IDC_MSCOMM1，双击消息OnComm，将弹出的对话框中将函数名改为OnComm，（好记而已）OK。
这个函数是用来处理串口消息事件的，如每当串口接收到数据，就会产生一个串口接收数据缓冲区中有字符的消息事件，我们刚才添加的函数就会执行，我们在OnComm()函数加入相应的处理代码就能实现自已想要的功能了。请你在函数中加入如下代码：
void CSCommTestDlg::OnComm() 
{
     // TODO: Add your control notification handler code here
     VARIANT variant_inp;
     COleSafeArray safearray_inp;
     LONG len,k;
     BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integerthat is not signed.
     CString strtemp;
     if(m_ctrlComm.GetCommEvent()==2) //事件值为2表示接收缓冲区内有字符
     {              ////////以下你可以根据自己的通信协议加入处理代码
         variant_inp=m_ctrlComm.GetInput(); //读缓冲区
         safearray_inp=variant_inp; //VARIANT型变量转换为ColeSafeArray型变量
         len=safearray_inp.GetOneDimSize(); //得到有效数据长度
         for(k=0;k<len;k++)
             safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组
         for(k=0;k<len;k++) //将数组转换为Cstring型变量
         {
             BYTE bt=*(char*)(rxdata+k); //字符型
             strtemp.Format("%c",bt); //将字符送入临时变量strtemp存放
             m_strRXData+=strtemp; //加入接收编辑框对应字符串 
         }
     }
     UpdateData(FALSE); //更新编辑框内容
}
到目前为止还不能在接收编辑框中看到数据，因为我们还没有打开串口，但运行程序不应该有任何错误，不然，你肯定哪儿没看仔细，因为我是打开VC6对照着做一步写一行的，运行试试。没错吧？那么做下一步：
6.打开串口和设置串口参数   你可以在你需要的时候打开串口，例如在程序中做一个开始按钮，在该按钮的处理函数中打开串口。现在我们在主对话框的CSCommTestDlg::OnInitDialog()打开串口，加入如下代码：
// TODO: Add extra initialization here
if(m_ctrlComm.GetPortOpen())
m_ctrlComm.SetPortOpen(FALSE);
m_ctrlComm.SetCommPort(1); //选择com1
if( !m_ctrlComm.GetPortOpen())
m_ctrlComm.SetPortOpen(TRUE);//打开串口
else
AfxMessageBox("cannot open serial port");
m_ctrlComm.SetSettings("9600,n,8,1"); //波特率9600，无校验，8个数据位，1个停止位
m_ctrlComm.SetInputMode(1);   // 以二进制方式检取数据
m_ctrlComm.SetRThreshold(1); 
//参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
m_ctrlComm.SetInputLen(0); //设置当前接收区数据长度为0
m_ctrlComm.GetInput();//先预读缓冲区以清除残留数据
现在你可以试试程序了，将串口线接好后,打开串口调试助手，并将串口设在com2，选上自动发送，也可以等会手动发送。再执行你编写的程序，接收框里应该有数据显示了。
7.发送数据   先为发送按钮添加一个单击消息即BN_CLICKED处理函数，打开ClassWizard－>Message Maps，选择类CSCommTestDlg，选择IDC_BUTTON_MANUALSEND，双击BN_CLICKED添加OnButtonManualsend()函数，并在函数中添加如下代码：
void CSCommTestDlg::OnButtonManualsend() 
{
// TODO: Add your control notification handler code here
UpdateData(TRUE); //读取编辑框内容
m_ctrlComm.SetOutput(COleVariant(m_strTXData));//发送数据
}
运行程序，在发送编辑框中随意输入点什么，单击发送按钮，啊！看看，在另一端的串口调试助手（或别的调试工具）接收框里出现了什么。
如果你真是初次涉猎串口编程，又一次成功，那该说声谢谢我了，因为我第一次做串口程序时可费劲了，那时网上的资料也不好找。开开玩笑，谢谢你的支持，有什么好东西别忘了给我寄一份。
最后说明一下，由于用到VC控件，在没有安装VC的计算机上运行时要从VC中把mscomm32.ocx、msvcrt.dll、mfc42.dll拷到Windows目录下的System子目录中（win2000为System32）
COleVariant
　　COleVariant类是对VARIANT结构的封装。它的构造函数具有极为强大的功能，当对象构造时首先调用VariantInit进行 初始化，然后根据参数中的标准类型调用相应的构造函数，并使用VariantCopy进行转换赋值操作，当VARIANT对象不在有效范围时，它的析构函 数就会被自动调用，由于析构函数调用了VariantClear，因而相应的内存就会被自动清除。除此之外，COleVariant的赋值操作符在与 VARIANT类型转换中为我们提供极大的方便
文章引自:[http://blog.chinaunix.net/u2/88620/showart_2136003.html](http://blog.chinaunix.net/u2/88620/showart_2136003.html)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/luheng2006/archive/2010/04/16/5491801.aspx](http://blog.csdn.net/luheng2006/archive/2010/04/16/5491801.aspx)

