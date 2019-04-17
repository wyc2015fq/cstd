# 孙鑫-MFC笔记五--对话框 - 工作笔记 - CSDN博客





2012年05月03日 10:57:41[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8819








对话框的创建

### 模态对话框

资源添加对话框，双击对话框创建对话框类。

在menu中增加对话框菜单项，将其设为非popup的，并为其在view类下添加响应函数。View类中增加#include
 "TestDlg.h"。

在响应函数void CMyboleView::OnDialog()中：

CTestDlg dlg;

       dlg.DoModal();

模态对话框即程序运行到此处被阻滞，停在此处，不可以执行其他内容。

### 非模态对话框

CTestDlg m_dlg;

m_dlg .Create (IDD_DIALOG1,this);   //this为对话框父窗口的指针

m_dlg .ShowWindow (SW_SHOW);

注意：这里非模态对话框要设置为成员变量。另外非模态对话框点击非模态对话框的OnOK按纽时，它并没有关闭，而是隐藏了。需要调用destroyWindow()。

### 在对话框上动态创建一个button对象

在对话框资源中从工具中拖一个button到对话框中，将其名称改为ADD，为其在CTestDlg类中添加响应函数。

void CTestDlg::OnBnClickedBtnAdd()

{

       if(m_blsCreate==false)        // m_blsCreate是bool型成员变量判断是否已经创建

       {

              m_btn.Create (_T("维新"),BS_DEFPUSHBUTTON|WS_VISIBLE|WS_CHILD,

CRect(0,0,100,100),this,123);  //动态创建button，其中m_btn为CBtton类型的成员变量

              m_blsCreate=true;

       }

else

       {

              m_btn.DestroyWindow ();

              m_blsCreate=false;

       }

}

另外m_blsCreate可以不定义为成员变量，而定义为static bool blsCreate=false;

每个窗口都有一个m_hWnd的成员变量，当窗口被创建后m_hWnd为非零值，当窗口被销毁后（窗口类对象仍然存在）值为零，因此可以通过判断m_hWnd的方式来判断button对象是否被创建。

void CTestDlg::OnBnClickedBtnAdd()

{

if(!m_btn.m_hWnd )

    {

           m_btn.Create (_T("维新"), BS_DEFPUSHBUTTON|WS_VISIBLE|WS_CHILD,
 CRect(0,0,100,100),this,123);

    }

else

    {

           m_btn.DestroyWindow ();

    }

}

## 2 对话框加法操作

### 静态文本框

在对话框资源文件中从工具箱拖出三个静态文本框Static Text，将其名称改为Number1，Number2，Number3，将ID号改为IDC_NUMBER1,
 IDC_NUMBER2, IDC_NUMBER3,将Notify项设为True，如果不这么设置则无法接受相应消息。

在CTestDlg类中给文本框添加点击响应函数：

CString str;

if(GetDlgItem(IDC_NUMBER1)->GetWindowText(str),str==_T("Number1"))

       {

              GetDlgItem(IDC_NUMBER1)->SetWindowText(_T("数值："));

       }

else

       {

              GetDlgItem(IDC_NUMBER1)->SetWindowText(_T("Number1"));

       }

注释：GetDlgItem(IDC_NUMBER1)通过对话框中的控件ID获取控件句柄Cwnd*。

      GetWindowText(str)  拷贝控件的名称到str。

      SetWindowText(_T("数值："))设置控件名称。



### 对话框实现加法操作

在CTestDlg::OnBnClickedBtnAdd()函数中添加如下响应函数：

方法一：使用GetDlgItem(),GetWindowText(),SetWindowText()

 int num1,num2,num3;

 TCHAR ch1[10], ch2[10], ch3[10];

 GetDlgItem(IDC_EDIT1)->GetWindowText(ch1, 10);

 GetDlgItem(IDC_EDIT2)->GetWindowText(ch2, 10);

 num1 = _ttoi(ch1);

 num2 = _ttoi(ch2);

 num3 = num1 + num2;

 _itot(num3, ch3,10);

 GetDlgItem(IDC_EDIT3)->SetWindowText(ch3);注释：atoi(ch1)将char*类型转换为int，itoa将int转换为char*，最后一个参数为转换进制数。

在VS2008中，

char 改成TCHAR

atoi 改成 _ttoi

itoa 改成 _itot



方法二：使用GetDlgItemText()和SetDlgItemText()



 int num1,num2,num3;

 TCHAR ch1[10], ch2[10], ch3[10];

 GetDlgItemText(IDC_EDIT1, ch1, 10);

 GetDlgItemText(IDC_EDIT2, ch2, 10);

 num1 = _ttoi(ch1);

 num2 = _ttoi(ch2);

 num3 = num1 + num2;

 _itot(num3, ch3,10);

 SetDlgItemText(IDC_EDIT3, ch3);

注释：GetDlgItemText()相当于GetDlgItem(),GetWindowText()的组合。

方法三：使用GetDlgItemInt()，SetDlgItemInt()

 int num1,num2,num3;

 num1 = GetDlgItemInt(IDC_EDIT1);

 num2 = GetDlgItemInt(IDC_EDIT2);

 num3 = num1 + num2;

 SetDlgItemInt(IDC_EDIT3, num3);

       SetDlgItemInt(IDC_EDIT3,num3);

注释：GetDlgItemInt(

**int*** nIDDlgItem***,            **//控件id

**BOOL ****lpTranslated***,       **// success state，默认为NULL，不警告任何错误

**BOOL*** bSigned*            //有符号或无符号，默认为真有符号

**);**



BOOL SetDlgItemInt(int nIDDlgItem,UINT uValue,BOOL bSigned);

方法四：关联成员变量法

 UpdateData();

 m_edit3 = m_edit1 + m_edit2;

 UpdateData(FALSE);



 m_cEdit1.GetWindowText(ch1, 10);

 m_cEdit2.GetWindowText(ch2, 10);

 num1 = _ttoi(ch1);

 num2 = _ttoi(ch2);

 num3 = num1 + num2;

 _itot(num3, ch3,10);

 m_cEdit3.SetWindowText(ch3); 

方法五：发送消息SendMessage

方法六：发送消息SendDlgItemMessage

## 3 关联成员变量

资源文件中控件上右击添加变量，private，变量类型int，变量名：m_num1,控件类别将control改为value。

可以看到在代码中增加了如下内容：

CTestDlg.h中：

private:

// 控件关联成员变量

       int m_num1;

       int m_num2;

       int m_num3;



CTestDlg.cpp中：

void CTestDlg::DoDataExchange(CDataExchange* pDX)

{

       CDialogEx::DoDataExchange(pDX);

       DDX_Text(pDX, IDC_EDIT1, m_num1);

       DDX_Text(pDX, IDC_EDIT2, m_num2);

       DDX_Text(pDX, IDC_EDIT3, m_num3);

}

在CTestDlg::OnBnClickedBtnAdd()函数中添加如下响应函数：

UpdateData();

       m_num3=m_num1+m_num2;

       UpdateData(false);

注释： UpdateData()参数只有一个，默认为TRUE。为TRUE时刷新控件的值到对应的变量(外部输入值交给内部变量)。为false时拷贝变量值到控件显示(变量的最终运算结果值交给外部输出显示)。即初始化

### 设定值的范围

在函数DoDataExchange中DDX_Text(pDX, IDC_EDIT1, m_num1);语句后添加：

           DDV_MinMaxInt(pDX,m_num1,0,100);           //m_num限定在0到100

## 4 关联控件变量

资源文件中控件上右击添加变量，private，变量类型CEdit，变量名：m_edit1,控件类别为control。

可以看到在代码中增加了如下内容：

CTestDlg.h中：

private:

// 控件关联成员变量

       CEdit m_edit1;

       CEdit m_edit2;

       CEdit m_edit3;



CTestDlg.cpp中：

void CTestDlg::DoDataExchange(CDataExchange* pDX)

{

       CDialogEx::DoDataExchange(pDX);

       DDX_Control(pDX, IDC_EDIT1, m_edit1);

       DDX_Control(pDX, IDC_EDIT2, m_edit2);

       DDX_Control(pDX, IDC_EDIT3, m_edit3);

 }

由于CEdit类型也是从Cwnd继承的，所以在CTestDlg::OnBnClickedBtnAdd()函数中添加如下响应函数：

int num1,num2,num3;

char ch1[10],ch2[10],ch3[10];



       m_edit1.GetWindowText(ch1,10);

       m_edit2.GetWindowText(ch2,10);



       num1=atoi(ch1);

       num2=atoi(ch2);

       num3=num1+num2;

       itoa(num3,ch3,10);

       m_edit3.SetWindowTextA (ch3);

## 5 发送消息

### 使用SendMessage函数WM_GETTEXT和WM_SETTEXT消息

在CTestDlg::OnBnClickedBtnAdd()函数中添加如下响应函数：

int num1,num2,num3;

char ch1[10],ch2[10],ch3[10];

//::SendMessageA(GetDlgItem(IDC_EDIT1)->m_hWnd ,  WM_GETTEXT，10,
 (LPARAM)ch1 );

//::SendMessageA(m_edit1->m_hWnd , WM_GETTEXT ,10,(LPARAM)ch1);

//GetDlgItem(IDC_EDIT1)->SendMessage (WM_GETTEXT,10,(LPARAM)ch1);

       m_edit1.SendMessage (WM_GETTEXT,10,(LPARAM)ch1);

m_edit2.SendMessage (WM_GETTEXT,10,(LPARAM)ch2); //第二个参数为发送数据量，第三个为接受数据的buffer



       num1=atoi(ch1);

       num2=atoi(ch2);

       num3=num1+num2;

       itoa(num3,ch3,10);

       m_edit3.SendMessage (WM_SETTEXT,0,(LPARAM)ch3);

### 使用SendDlgItemMessage函数

在CTestDlg::OnBnClickedBtnAdd()函数中添加如下响应函数：

int num1,num2,num3;

char ch1[10],ch2[10],ch3[10];

       SendDlgItemMessage(IDC_EDIT1,WM_GETTEXT,10,(LPARAM)ch1);

       SendDlgItemMessage(IDC_EDIT2,WM_GETTEXT,10,(LPARAM)ch2);

       num1=atoi(ch1);

       num2=atoi(ch2);

       num3=num1+num2;

       itoa(num3,ch3,10);

       SendDlgItemMessage(IDC_EDIT3,WM_SETTEXT,10,(LPARAM)ch3);

### 使用EM_SETSEL消息设置复选信息

在上面代码后面加上：

SendDlgItemMessage(IDC_EDIT3,EM_SETSEL,1,3);

       m_edit3.SetFocus ();

< xmlnamespace prefix ="v" ns ="urn:schemas-microsoft-com:vml" />

SendDlgItemMessage开始位置为0，结束位置为-1，可将整个框内数据复选上。

## 6 收缩扩展功能

### 添加分割线

在对话框窗口资源上加一个picture control画一条线，更改其属性，他的ID是static的，不能接受notify消息，将其改为IDC_SEPARATOR，Sunken设置为true，visiable设置为false。



### 添加button控制收缩

增加一个button，命名为”收缩<<”，并对其添加消息处理函数。

void CTestDlg::OnBnClickedButton2()

{

       CString str;

if(GetDlgItemText(IDC_BUTTON2,str),str=="收缩《")

       {

              SetDlgItemText(IDC_BUTTON2,"扩展》");

       }

else

       {

              SetDlgItemText(IDC_BUTTON2,"收缩《");

       }



static CRect rectLarge;

static CRect rectSmall;



//判定矩形是否为空，有两个方法

isRectNull 不考虑是否复合逻辑

isRecgEmpty

if(rectLarge.IsRectNull())

       {

              CRect rectSeparator;

              GetWindowRect(&rectLarge);

              GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);



              rectSmall.left =rectLarge.left ;

              rectSmall.top=rectLarge.top;

              rectSmall.right =rectLarge.right;

              rectSmall.bottom=rectSeparator.top ;

       }

if(str=="收缩《")

       {

              SetWindowPos(NULL,0,0,rectSmall.Width (),rectSmall.Height(),

                     SWP_NOMOVE|SWP_NOZORDER);                  //重新设置大小

       }

else

       {

              SetWindowPos(NULL,0,0,rectLarge.Width (),rectLarge.Height(),

                     SWP_NOMOVE|SWP_NOZORDER);

       }

}




       窗口的Z次序表明了重叠窗口堆中窗口的位置，这个窗口堆是按一个假想的轴定位的，这个轴就是从屏幕向外伸展的Z轴。Z次序最上面的窗口覆盖所有其它的窗口，Z次序最底层的窗口被所有其它的窗口覆盖。应用程序设置窗口在Z次序中的位置是通过把它放在一个给定窗口的后面，或是放在窗口堆的顶部或底部。

        Windows系统管理三个独立的Z次序——一个用于顶层窗口、一个用于兄弟窗口，还有一个是用于最顶层窗口。最顶层窗口覆盖所有其它非最顶层窗口，而不管它是不是活动窗口或是前台窗口。应用程序通过设置WS_EX_TOPMOST风格创建最顶层窗口。

        一般情况下，Windows系统把刚刚创建的窗口放在Z次序的顶部，用户可通过激活另外一个窗口来改变Z次序；Windows系统总是把活动的窗口放在Z次序的顶部，应用程序可用函数BringWindowToTop把一个窗口放置到Z次序的顶部。函数SetWindowPos和DeferWindowPos用来重排Z次序。



兄弟窗口 

     共享同一个父窗口的多个子窗口叫兄弟窗口。 

活动窗口 

              活动窗口是应用程序的顶层窗口，也就是当前使用的窗口。只有一个顶层窗口可以是活动窗口，如果用户使用的是一个子窗口，Windows系统就激活与这个子窗口相应的顶层窗口。

            任何时候系统中只能有一个顶层窗口是活动的。用户通过单击窗口（或其中的一个子窗口）、使用ALT+TAB或ALT+ESC组合键来激活一个顶层窗口，应用程序则调用函数SetActiveWindow来激活一个顶层窗口。






## 7 按下回车键自动转到下一个输入框

Ok按钮是缺省的按钮，属性中Default button为true。当按下回车系统采用缺省的响应函数，处理回车消息，相当于按下Ok键。增加ok消息响应函数，在其中屏蔽对基类CDialogEx::OnOK();的调用。可以屏蔽掉回车的缺省实现。

传递输入焦点的方式，一种是为编辑框关联一个类，在类中捕获键盘按键消息，另外一种是自己编写过程函数，替换MFC默认的窗口过程函数。

窗口创建之后，利用

SetWindowLong()改变窗口过程函数。然后在窗口过程中，捕获按键消息，并判断，移动焦点。

SetWindowLong()函数

在窗口创建完成之后，利用该函数修改窗口过程函数。该函数可以改变指定窗口的属性。

WM_INITDIALOG消息，是窗口以及子类控件创建完成之后，显示之前发送的一个消息。因此在此消息响应函数中，方式setwindowlong函数比较合适。

### 方法1：

改变控件的回调函数，注意IDC_EDIT1的MultiLine要复选上，这样才能响应回车操作。

WNDPROC prevProc;

LRESULT CALLBACK WinSunProc(

  HWND hwnd,      // handle to window

  UINT uMsg,      // message identifier

  WPARAM wParam,  // first message parameter

  LPARAM lParam   // second message parameter

)

{

 if(uMsg==WM_CHAR && wParam==0x0d)

 {

  //::SetFocus(::GetNextWindow(hwnd,GW_HWNDNEXT)); //获取窗口句柄方法1

  //SetFocus(::GetWindow(hwnd,GW_HWNDNEXT)); ////获取窗口句柄方法2

  CString str;

  str.Format("%d",hwnd);

  AfxMessageBox(str);//, UINT nType = MB_OK, UINT nIDHelp = 0 );

//  AfxGetApp()->

  SetFocus(::GetNextDlgTabItem(::GetParent(hwnd),hwnd,FALSE));//获取窗口句柄方法3

  return 1;

 }

 else

 {

  return prevProc(hwnd,uMsg,wParam,lParam);

 }

}

BOOL CTestDlg::OnInitDialog() 

{

 CDialog::OnInitDialog();



 // TOD Add extra initialization here

 prevProc=(WNDPROC)SetWindowLong(GetDlgItem(IDC_EDIT1)->m_hWnd,GWL_WNDPROC,

  (LONG)WinSunProc);//设置回调函数

 return TRUE;  // return TRUE unless you set the focus to a control

               // EXCEPTION: OCX Property Pages should return FALSE

}

### 方法2：

在OnOK响应函数中加入代码

 //GetDlgItem(IDC_EDIT1)->GetNextWindow()->SetFocus();

 //GetFocus()->GetNextWindow()->SetFocus();

 //GetFocus()->GetWindow(GW_HWNDNEXT)->SetFocus();

 GetNextDlgTabItem(GetFocus())->SetFocus();        //焦点可以依次传递，查找完一遍不会出问题

1.如何改变按纽的字体？在对话框的属性中改变字体的属性即可

2.逃跑按纽的实现

  1.从CButton派生一个类，CWeixinBtn

  2.将IDC_EDIT1关联成员变量m_btn1,类型为CWeixinBtn，注意要包含头文件。

  3.在CWeixinBtn中加一个指针成员变量CWeixinBtn *pWeixinBtn,然后将其地址初始化。

  4.在新类中增加鼠标移动的消息处理。

3.属性表单

  1.插入属性页资源。Insert->new Resource->Dialog

  2.当选择Classwizard菜单时，系统提示是否为创建新的类，我们将其从CPropertyPage派生！这样可以为



方便为其增加消息响应函数。

  3.插入新的从CPropertySheet派生的类，在类中增加3个CPropertyPage的实例。

  4.在view中增加菜单项，当点击时显示属性表单，出现中文乱码，修改CPropertyPage属性为中文，另外将



其字体设为宋体。

  5.在CPropertyPage中设置SetWizardButtons可将其属性改为上一步、完成！

  6.为IDC_RADIO1关联成员变量，需要先设置Group属性才行。另外别忘记调用UpdateData().

  7.为CPropertyPage增加虚函数，OnWizardNext,如果用户点击下一步时，不想让他进入下一步，刚返回-1！

  8.将用户的选择输出到屏幕上，此时可以在View中增加几个成员变量，用来接收用户选择的数据。

 4.memset()的用法！ memset(m_bLike,0,sizeof(m_bLike));

上面是VC6.0方法。

在VS2008中，方法一样，思路一样。不同的是，按照相同方法设计出来后，会有所需资源不可用的错误。解决办法：

用记事本打开.rc文件，把跟属性页相关的如下部分代码 及其对应的#endif删除


#if defined(APSTUDIO_INVOKED) || defined(PAGE2)

#if defined(APSTUDIO_INVOKED)

IDD_PROP2$(PAGE2) DIALOGEX 0, 0, 235, 156

#else

切记，代码块后面的#endif也要删除。一起删除三次。

VS中，是不会出现乱码的。不用设置字体。

建立属性对象后，创建属性窗口前，添加setWiardMode函数，即可变成向导。

做向导的时候我们想改变属性页按钮的功能   需要在三个属性页类中分别重写OnSetActive函数   在其中添加((CPropSheet *)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

GetParent()得到父窗口的指针，然后转换为属性表单指针，调用设置按钮函数。



Radio Button 是通过GROUP属性来划分组的  通过在具有组属性的按钮上添加一个成员变量  根据该变量的值来判断组里有谁被选中  注意  被关联的变量与控件间的数据交换需要地用UpdateData函数参数为TRUE时是从控件中取回指  参数为FALSE是将成员变量的值赋予控件

要在点击下一步的时候判断RadioButton是否被选中 需要在该属性页的类中重写OnWizardNext函数

对于要在属性页的list里添加了内容  可以在OnInitDialog函数中写  该函数是用来相应WM_INITDIALOG消息的  该消息是在对话框将要显示之前发送的  在VC6++中是通过添加消息映射函数建立该函数的   而在VS2008中 则是通过重写

MFC中对控件操作都是通过调用相关的类  比如list box控件  就要用CListBox类   然后可以通过得到控件指针的函数 GetDlgItem 来操作

如:

((CListBox *)GetDlgItem(IDC_LIST1))->AddString(L"枫江");
判断list box中的选项是否被选择的方法  也是关联一个成员变量  CString类型的   如果为空 则未选择

Combo Box的Sort属性选FALSE 就不会自动排序了

可以用CComboBox的一个成员函数SetCurSel来进行缺省的设置 

然后用getTBText函数，完成对索引值内容的读取。

*VS2008中 添加数组变量 方法举例

类型 int (&)[10] 

名字 m 

但是 添加进去之后最好还是改成 int m[10]的形式













