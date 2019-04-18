# CRichEdit控件操作使用 - 深之JohnChen的专栏 - CSDN博客

2010年03月18日 17:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6083


#### CRichEdit控件操作相关< 转载>

一般性问题
1.    代码编译通过了，运行后RichEdit控件不显示，在InitInstance中添加
BOOL CWinApp::InitInstance ()
{
…
AfxInitRichEdit(); - RichEdit 1.0
或 AfxInitRichEdit2(); - RichEdit 2.0
…
}

2.    升级默认的RichEdit版本，默认的版本有BUG，在InitInstance中添加
BOOL CWinApp::InitInstance ()
{
…
LoadLibrary("RICHED20.DLL");
…
FreeLibrary();
}

如果是CRichEditView基类的话，可用如下消息机制:
BOOL CRichEditView::PreCreateWindow(CREATESTRUCT& cs)
{
if (LoadLibraryA("RICHED20.DLL") == NULL)
{
AfxMessageBox(_T("Fail to load /"riched20.dll/"."),MB_OK | MB_ICONERROR);
PostMessage(WM_QUIT,0,0);
return FALSE;
}

m_strClass = RICHEDIT_CLASSA; 
return CRichEditView::PreCreateWindow(cs);
}

3.    在RichEdit控件的输入框中最后追加一行
CRichEditCtrl.SetSel(-1, -1);
CRichEditCtrl.ReplaceSel((LPCTSTR)str);

4.    限制RichEdit控件的字数
CRichEditCtrl.LimitText(long nChars)

5.    设置RichEdit控件的换行切换
如果是CRichEditView基类的话，可用如下消息机制:
BOOL CRichEditView::OnInitialUpdate()
{
…
m_nWordWrap = WrapNone;
WrapChanged(); 
…
}

如果是在Dialog，可使用SetTargetDevice，并在RichEdit的属性里面加上want return
CRichEditCtrl.SetTargetDevice(m_dcTarget, GetPrintWidth()); 

WrapChanged实际上也是调用
CRichEditCtrl.SetTargetDevice(NULL, 0); //m_nWordWrap == WrapToWindow
CRichEditCtrl.SetTargetDevice(NULL, 1); //m_nWordWrap == WrapNone
CRichEditCtrl.SetTargetDevice(NULL, 2);// m_nWordWrap == WrapToTargetDevice

6.    设置RichEdit控件不带格式的数据粘贴
CRichEditCtrl.PasteSpecial(CF_TEXT);

7.    设置RichEdit控件的滚动条自动随输入滚动到最后一行
int nFirstVisible = CRichEditCtrl.GetFirstVisibleLine();
if (nFirstVisible > 0)
{
CRichEditCtrl.LineScroll(-nFirstVisible, 0);
}
或CRichEditCtrl.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

8.设置RichEdit控件的撤销输入次数(只能用在RICHED20以上)
SendMessage(EM_SETTEXTMODE, TM_MULTILEVELUNDO, 0);
SendMessage(EM_SETUNDOLIMIT, 100, 0);
TM_MULTILEVELUNDO 支持多取消(默认值)，可通过EM_SETUNDOLIMIT设置最大次数

9.设置RichEdit控件的OnChange事件
EM_SETEVENTMASK 设置 ENM_CHANGE 
long lMask = GetEventMask();
lMask |= ENM_CHANGE;
lMask &= ~ENM_PROTECTED;
SetEventMask(lMask);

10.设置RichEdit控件的只读属性
CRichEditCtrl::SetReadOnly(BOOL bReadOnly = TRUE );
通过设置PROTECTED实现选中的文本只读，参见如下文章：
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c2401/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c2401/)

函数应用
1. 设置RichEdit控件的显示字体
CHARFORMAT cf;
ZeroMemory(&cf, sizeof(CHARFORMAT));
cf.cbSize = sizeof(CHARFORMAT);
cf.dwMask|=CFM_BOLD;
cf.dwEffects|=CFE_BOLD; //粗体，取消用cf.dwEffects&=~CFE_BOLD;
cf.dwMask|=CFM_ITALIC;
cf.dwEffects|=CFE_ITALIC; //斜体，取消用cf.dwEffects&=~CFE_ITALIC;
cf.dwMask|=CFM_UNDERLINE;
cf.dwEffects|=CFE_UNDERLINE; //斜体，取消用cf.dwEffects&=~CFE_UNDERLINE;
cf.dwMask|=CFM_COLOR;
cf.crTextColor = RGB(255,0,0); //设置颜色
cf.dwMask|=CFM_SIZE;
cf.yHeight =200; //设置高度
cf.dwMask|=CFM_FACE;
strcpy(cf.szFaceName ,_T("隶书")); //设置字体
CRichEditCtrl.SetSelectionCharFormat(cf);

2.设置RichEdit控件的显示字体的行间距（只能用在RICHED20以上）
PARAFORMAT2 pf;
pf.cbSize = sizeof(PARAFORMAT2);
pf.dwMask = PFM_NUMBERING | PFM_OFFSET;
pf.wNumbering = PFN_BULLET; //注意PFM_NUMBERING 
pf.dxOffset = 10;
VERIFY(SetParaFormat(pf)); 

常用的dwMask有如下列表：
PFM_NUMBERING 成员 wNumbering 才起作用
1 项目符号，默认用PFN_BULLET.
2 使用阿拉伯数字 (1, 2, 3, ...).
3 使用小写字母 (a, b, c, ...).
4 使用大写字母 (A, B, C, ...).
5 使用小写罗马数字 (i, ii, iii, ...).
6 使用大写罗马数字 (I, II, III, ...).
7 自定义，字符见成员 wNumberingStart.
PFM_OFFSET 成员 dxOffset 才起作用，缩进，单位twips
PFM_STARTINDENT 成员 dxStartIndent 才起作用，首行缩进
PFM_SPACEAFTER 成员 dySpaceAfter 才起作用，段间距
PFM_LINESPACING 成员 dyLineSpacing 才起作用，行间距

3.设置RichEdit控件的透明背景（只能用在RICHED20以上）
long style = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
style &= WS_EX_TRANSPARENT;
::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, style);
或 CreateEx()，然后把WS_EX_TRANSPARENT样式加上

4.获取或者写入RichEdit控件的内容
Ø         GetWindowText
Ø         使用EM_GETTEXTEX
GETTEXTEX gt;
gt.cb = 200;
gt.flags = GT_DEFAULT;
gt.codepage = CP_ACP ;
gt.lpDefaultChar = NULL;
gt.lpUsedDefChar = NULL;
SendMessage(EM_GETTEXTEX,(WPARAM)>,(LPARAM)text);
Ø         StreamOut(主要用于RTF等格式输出)
static DWORD CALLBACK；
MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
CFile* pFile = (CFile*) dwCookie;
pFile->Write(pbBuff, cb);
*pcb = cb;
return 0;
}
CFile cFile(TEXT("myfile.rtf"), CFile::modeCreate|CFile::modeWrite);
EDITSTREAM es;
es.dwCookie = (DWORD) &cFile; //设置用例参数,以便回调函数调用
es.pfnCallback = MyStreamOutCallback;
pmyRichEditCtrl->StreamOut(SF_RTF, es);

读入可以此类推，SetWindowText, EM_SETTEXTEX, StreamIn

5.查找RichEdit控件中的字符串
FINDTEXTEX ft;
ft.chrg.cpMin = 0;
ft.chrg.cpMax = -1;
ft.lpstrText = "|";
long lPos = FindText(0, &ft);

如果要继续查找，修改cpMin,如
int nCount = 0;
do
{
long lPos = GetRichEditCtrl().FindText(0, &ft);
if( -1 == lPos) break;
ft.chrg.cpMin = lPos + strlen(ft.lpstrText);
++nCount;
}while(TRUE);

6. 以Html格式保存RichEdit控件中的内容
临时做法可先转为RTF格式，再通过RTF-to-HTML Converter，参考如下文章：
[http://www.codeguru.com/Cpp/controls/richedit/conversions/article.php/c5377/](http://www.codeguru.com/Cpp/controls/richedit/conversions/article.php/c5377/)

8.    重载OnProtected函数以得到RichEdit对应的消息，如粘贴等

void CRichEditorView::OnProtected(NMHDR* pNMHDR, LRESULT* pResult)
{
ENPROTECTED* pEP = (ENPROTECTED*)pNMHDR;
switch (pEP->msg)
{
case WM_KEYDOWN: //按键，判断pEP->wParam
case WM_PASTE: //粘贴
case WM_CUT: //剪切
case EM_SETCHARFORMAT:
default:
break;
};
*pResult = FALSE;
}

聊天常用
1. 设置RichEdit控件的链接功能（只能用在RICHED20以上）
Ø         LoadLibrary(_T("Riched20.dll"));
Ø         创建RichEdit2.0控件
CreateEx(0, _T("RichEdit20A"), NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP 

|ES_READONLY|ES_WANTRETURN|ES_MULTILINE, rect.left, rect.top, cx, cy, pParentWnd->m_hWnd, (HMENU)nID, NULL);
Ø         设置选中的文字为链接显示
CHARFORMAT2 cf2;
ZeroMemory(&cf2, sizeof(CHARFORMAT2));//
cf2.cbSize = sizeof(CHARFORMAT2);
cf2.dwMask = CFM_LINK;
cf2.dwEffects |= CFE_LINK;
m_cRichEdit.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
Ø         支持链接的点击响应
m_cRichEdit.SetEventMask(ENM_LINK);
Ø         响应链接的点击EN_LINK
BEGIN_MESSAGE_MAP(CMyRichEdit, CRichEditCtrl)
ON_NOTIFY_REFLECT(EN_LINK, OnURL)
END_MESSAGE_MAP()
......
void CMyRichEdit::OnURLClick(NMHDR *pNmhdr, LRESULT *pResult)
{
TCHAR LinkChar[512];
ENLINK *pLink = (ENLINK *)pNmhdr;
if (pLink->msg == WM_LBUTTONUP)
{
SetSel(penLink->chrg); //这是链接的文字范围
long Res = GetSelText((char *)LinkChar); //这是链接文字
…
}
}
Ø         自动识别链接
UINT nMask = SendDlgItemMessage(IDC_RICHEDIT_MESSAGE, EM_GETEVENTMASK, 0, 0);
SendDlgItemMessage(IDC_RICHEDIT_MESSAGE, EM_SETEVENTMASK, 0, nMask | NM_LINK);
SendDlgItemMessage(IDC_RICHEDIT_MESSAGE, EM_AUTOURLDETECT, TRUE, 0);
BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
NMHDR* pNmHdr = (NMHDR *)lParam;
if(pNmHdr->idFrom == IDC_RICHEDIT_MESSAGE && pNmHdr->code == EN_LINK)
{
ENLINK *pLink = (ENLINK *)lParam;
if (pLink->msg == WM_LBUTTONDOWN)
{
SendDlgItemMessage(IDC_RICHEDIT_MESSAGE, EM_EXSETSEL, 0, (LPARAM)&(pLink->chrg));
ShellExecute(GetSafeHwnd(), "open", m_wndREMessage.GetSelText(), 0, 0, SW_SHOWNORMAL);
}
}
return CDialog::OnNotify(wParam, lParam, pResult);
}

2.在RichEdit中插入位图
Ø         参考如下文章：
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c2417/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c2417/)
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c5383/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c5383/)

Ø         自定义在RichEdit中插入对象的图标
[http://www.blogcn.com/user3/jiangsheng/blog/1319738.html](http://www.blogcn.com/user3/jiangsheng/blog/1319738.html)
[http://www.codeguru.com/richedit/richeditrc.html](http://www.codeguru.com/richedit/richeditrc.html)

3.在RichEdit显示GIF动画
Ø         常用的是通过QQ的imageole.dll(也有用Gif89.dll的)
[http://www.xiaozhou.net/cooldog/blogview.asp?logID=82](http://www.xiaozhou.net/cooldog/blogview.asp?logID=82)
[http://www.codeproject.com/richedit/AnimatedEmoticon.asp](http://www.codeproject.com/richedit/AnimatedEmoticon.asp)
Ø         在richedit控件中插入动态GIF(Native C++版)
[http://blog.joycode.com/jiangsheng/archive/2004/12/15/41209.aspx](http://blog.joycode.com/jiangsheng/archive/2004/12/15/41209.aspx)

4.IRichEditOleCallback的使用
[http://61.186.252.131/Expert/topic/905/905844.xml?temp=.8379022](http://61.186.252.131/Expert/topic/905/905844.xml?temp=.8379022)

5.类似MSN信息发送框的制作(上)
[http://www.vckbase.com/document/viewdoc/?id=1087](http://www.vckbase.com/document/viewdoc/?id=1087)
内容包含：实现右键菜单，图片插入，读取/写入RTF格式字符串

6.自定义RichEdit控件
[http://www.vckbase.com/document/viewdoc/?id=328](http://www.vckbase.com/document/viewdoc/?id=328)
内容包含：鼠标右键消息，消息映射，字体变换

转载】MFC中的CEdit与CRichEdit使用技巧2008-03-14 15:41VC中在对话框上使用Rich Edit控件前一定要用AfxInitRichEdit()初

始化RichEdit环境.

--------------------------------------------------------------------------------

1.设置edit只读属性

    方法一：
                m_edit1.SetReadOnly(TRUE);
    方法二：
                ::SendMessage(m_edit1.m_hWnd, EM_SETREADONLY, TRUE, 0);

--------------------------------------------------------------------------------

2.判断edit中光标状态并得到选中内容(richedit同样适用)

        int nStart, nEnd;
        CString strTemp;

        m_edit1.GetSel(nStart, nEnd);
        if(nStart == nEnd)
        {
            strTemp.Format(_T("光标在%d"), nStart);
            AfxMessageBox(strTemp);
        }
        else
        {
            //得到edit选中的内容     
            m_edit1.GetWindowText(strTemp);
            strTemp = strTemp.Mid(nStart) - strTemp.Mid(nEnd);
            AfxMessageBox(strTemp); 
        }
    注：GetSel后，如果nStart和nEnd，表明光标处于某个位置（直观来看就是光标在闪动）；
             如果nStart和nEnd不相等，表明用户在edit中选中了一段内容。

--------------------------------------------------------------------------------

3.在edit最后添加字符串

        CString str;
        m_edit1.SetSel(-1, -1);
        m_edit1.ReplaceSel(str);

--------------------------------------------------------------------------------

4.随输入自动滚动到最后一行(richedit同样适用)

    方法一：（摘自msdn）
        // The pointer to my edit.
        extern CEdit* pmyEdit;
        int nFirstVisible = pmyEdit->GetFirstVisibleLine();

        // Scroll the edit control so that the first visible line
        // is the first line of text.
        if (nFirstVisible > 0)
        {
            pmyEdit->LineScroll(-nFirstVisible, 0);
        }
    方法二：
        m_richedit.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

--------------------------------------------------------------------------------

5.如何限制edit输入指定字符

   可以从CEdit派生一个类，添加WM_CHAR消息映射。下面一个例子实现了限定输入16进制字符的功能。

   void CMyHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
   { 
        if ( (nChar >= '0' && nChar <= '9') || 
             (nChar >= 'a' && nChar <= 'f') || 
         (nChar >= 'A' && nChar <= 'F') || 
              nChar == VK_BACK || 
              nChar == VK_DELETE)    //msdn的virtual key
       { 
            CEdit::OnChar(nChar, nRepCnt, nFlags); 
        }    
   }

--------------------------------------------------------------------------------

6.如何使用richedit

    添加AfxInitRichEdit();
       CxxxApp::InitInstance()
        {
             AfxInitRichEdit();
          .............
       }

   AfxInitRichEdit()功能：装载 RichEdit 1.0 Control (RICHED32.DLL).

--------------------------------------------------------------------------------

7.如何使用richedit2.0 or richedit3.0

    使用原因：由于RichEdit2.0A自动为宽字符（WideChar），所以它可以解决中文乱码以及一些汉字问题

    方法一：（msdn上的做法，适用于用VC.NET及以后版本创建的工程）
            To update rich edit controls in existing Visual C++ applications to version 2.0,
            open the .RC file as text, change the class name of each rich edit control from   "RICHEDIT" to 

"RichEdit20a". 
            Then replace the call to AfxInitRichEdit with AfxInitRichEdit2.
    方法二：以对话框为例：
       (1)    增加一全局变量 HMODULE hMod;
       (2)    在CxxxApp::InitInstance()中添加一句hMod = LoadLibrary(_T("riched20.dll"));
            在CxxxApp::ExitInstance()中添加一句FreeLibrary(hMod);
       (3)    在对话框上放一个richedit，文本方式打开.rc文件修改该richedit控件的类名"RICHEDIT" to "RichEdit20a".
       (4)    在对话框头文件添加 CRichEditCtrl m_richedit;
            在OnInitDialog中添加 m_richedit.SubclassDlgItem(IDC_RICHEDIT1, this);

--------------------------------------------------------------------------------

8.改变richedit指定区域的颜色及字体

        CHARFORMAT cf;
        ZeroMemory(&cf, sizeof(CHARFORMAT));
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                            CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
        cf.dwEffects = 0;
        cf.yHeight = 12*12;//文字高度
        cf.crTextColor = RGB(200, 100, 255); //文字颜色
        strcpy(cf.szFaceName ,_T("隶书"));//设置字体

        m_richedit1.SetSel(1, 5); //设置处理区域
        m_richedit1.SetSelectionCharFormat(cf);

--------------------------------------------------------------------------------

9.设置行间距（只适用于richedit2.0）

        PARAFORMAT2 pf;
        pf2.cbSize = sizeof(PARAFORMAT2);
        pf2.dwMask = PFM_LINESPACING | PFM_SPACEAFTER;
        pf2.dyLineSpacing = 200;
        pf2.bLineSpacingRule = 4;
        m_richedit.SetParaFormat(pf2);

--------------------------------------------------------------------------------

10.richedit插入位图

Q220844：How to insert a bitmap into an RTF document using the RichEdit control in Visual C++ 6.0
[http://support.microsoft.com/default.aspx?scid=kb;en-us;220844](http://support.microsoft.com/default.aspx?scid=kb;en-us;220844)
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c2417/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c2417/)
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c5383/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c5383/)

--------------------------------------------------------------------------------

11.richedit插入gif动画

[http://www.codeproject.com/richedit/AnimatedEmoticon.asp](http://www.codeproject.com/richedit/AnimatedEmoticon.asp)

--------------------------------------------------------------------------------

12.richedit嵌入ole对象

[http://support.microsoft.com/kb/141549/en-us](http://support.microsoft.com/kb/141549/en-us)

--------------------------------------------------------------------------------

13.使richedit选中内容只读

[http://www.codeguru.com/cpp/controls/richedit/article.php/c2401/](http://www.codeguru.com/cpp/controls/richedit/article.php/c2401/)

--------------------------------------------------------------------------------

14.打印richedit

[http://www.protext.com/MFC/RichEdit3.htm](http://www.protext.com/MFC/RichEdit3.htm)

--------------------------------------------------------------------------------

15.richeidt用于聊天消息窗口

[http://www.vckbase.com/document/viewdoc/?id=1087](http://www.vckbase.com/document/viewdoc/?id=1087)
[http://www.codeproject.com/richedit/chatrichedit.asp](http://www.codeproject.com/richedit/chatrichedit.asp)
[http://www.codeguru.com/Cpp/controls/richedit/article.php/c2395/](http://www.codeguru.com/Cpp/controls/richedit/article.php/c2395/)

--------------------------------------------------------------------------------

16.解决richedit的EN_SETFOCUS和EN_KILLFOCUS无响应的问题

[http://support.microsoft.com/kb/181664/en-us](http://support.microsoft.com/kb/181664/en-us)

--------------------------------------------------------------------------------

17.richedit拼写检查

[http://www.codeproject.com/com/AutoSpellCheck.asp](http://www.codeproject.com/com/AutoSpellCheck.asp)

--------------------------------------------------------------------------------

18.改变edit背景色

Q117778：How to change the background color of an MFC edit control
[http://support.microsoft.com/kb/117778/en-us](http://support.microsoft.com/kb/117778/en-us)

--------------------------------------------------------------------------------

19.当edit控件的父窗口属性是带标题栏WS_CAPTION和子窗口WS_CHILD时，不能设置焦点SetFocus

Q230587：PRB: Can't Set Focus to an Edit Control When its Parent Is an Inactive Captioned Child Window
[http://support.microsoft.com/kb/230587/en-us](http://support.microsoft.com/kb/230587/en-us)

--------------------------------------------------------------------------------

20. 在Edit中回车时，会退出对话框 

选中Edit的风格Want Return。

MSDN的解释如下：
ES_WANTRETURN    Specifies that a carriage return be inserted when the user presses the ENTER key while entering 

text into a multiple-line edit control in a dialog box. Without this style, pressing the ENTER key has the same 

effect as pressing the dialog box's default pushbutton. This style has no effect on a single-line edit control.

--------------------------------------------------------------------------------

21. 动态创建的edit没有边框的问题

    m_edit.Create(....);
    m_edit.ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

--------------------------------------------------------------------------------

22. 一个能显示RTF，ole（包括gif， wmv，excel ，ppt）的例子

[http://www.codeproject.com/richedit/COleRichEditCtrl.asp](http://www.codeproject.com/richedit/COleRichEditCtrl.asp)

转自

[http://blog.csdn.net/lixiaosan/archive/2006/04/06/652795.aspx](http://blog.csdn.net/lixiaosan/archive/2006/04/06/652795.aspx)

Environment: VC6 SP4, 2000.

Follow these 10 easy steps to build the OutLookRichEdit control:

Insert a rich edit control into the dialog. 
Call AfxInitRichEdit() in the InitInstance of the App class or in InitDialog. 
If it does not exist, copy OutLookRichEdit.cpp and OutLookRichEdit.h to the project directory. 
Click the menu choice Project-Add to Project-Files and select the above-copied files to add the wrapper class to 

your project. 
Import the hand cursor into the resource and rename it "IDC_LINK". 
Use Classwizard to add a member variable of the rich edit control (CRichEditCtrl). 
Include the OutLookRichEdit.h file in the dialog's header file and change the declaration of rich edit member 

variable, as in 
CRichEditCtrl    m_ctrlText1;
to 
COutLookRichEdit m_ctrlText1;
In InitDialog(), add the following code. 
m_ctrlText1.SetRawHyperText(_T("Click <%$here$#100#%>
                                to see the about box."));
At this level, if you build the project and run it, you can see the rich edit control with linked text, but 

nothing would happen if you clicked on the link.

To Show a dialog while the link is clicked, you have to add some more code in the dialog class. Before that, have 

a closer look at the preceding code and hypertext syntax. The link text is enclosed between the "$" symbols and 

the corresponding dialog's resource value 100 (About Box), enclosed in "#" symbols.

You can find the #define values of dialogs in the resource.h file.

Use ClassWizard to map OnNotify of the dialog and write the corresponding implementation code in .cpp file, like: 
BOOL CDEMODlg::OnNotify(WPARAM wParam,
                        LPARAM lParam,
                        LRESULT* pResult)
{
  NMHDR* pNmHdr = (NMHDR*) lParam;
  if(IDC_RICHEDIT1 == pNmHdr->idFrom){
    switch(pNmHdr->code)
    {
      case IDD_ABOUTBOX:
        CAboutDlg oDlg;
        oDlg.DoModal ();
        break;
    }
  }
  return CDialog::OnNotify(wParam, lParam, pResult);
}
Now, build and run the project. It is recommended that you set the read-only attribute to the rich edit control. 
Downloads
Download demo project - 23 Kb
Download source - 6 Kb

在RichEdit中插入Bitmap
COleDataSource src;
STGMEDIUM sm;
sm.tymed=TYMED_GDI;
sm.hBitmap=hbmp;
sm.pUnkForRelease=NULL;
src.CacheData(CF_BITMAP, &sm);
LPDATAOBJECT lpDataObject =
(LPDATAOBJECT)src.GetInterface(&IID_IDataObject);
pRichEditOle->ImportDataObject(lpDataObject, 0, NULL);
lpDataObject->Release();

字体设置代码

最后添加字体变换函数： 
CHARFORMAT cf; 
LOGFONT lf; 
memset(&cf, 0, sizeof(CHARFORMAT)); 
memset(&lf, 0, sizeof(LOGFONT)); 

//判断是否选择了内容 
BOOL bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE; 
if (bSelect) 
{ 
             GetSelectionCharFormat(cf); 
} 
else 
{ 
             GetDefaultCharFormat(cf); 
} 

//得到相关字体属性 
BOOL bIsBold = cf.dwEffects & CFE_BOLD; 
BOOL bIsItalic = cf.dwEffects & CFE_ITALIC; 
BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE; 
BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT; 

//设置属性 
lf.lfCharSet = cf.bCharSet; 
lf.lfHeight = cf.yHeight/15; 
lf.lfPitchAndFamily = cf.bPitchAndFamily; 
lf.lfItalic = bIsItalic; 
lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL); 
lf.lfUnderline = bIsUnderline; 
lf.lfStrikeOut = bIsStrickout; 
sprintf(lf.lfFaceName, cf.szFaceName); 

CFontDialog dlg(&lf); 
dlg.m_cf.rgbColors = cf.crTextColor; 
if (dlg.DoModal() == IDOK) 
{ 
             dlg.GetCharFormat(cf);//获得所选字体的属性 
             if (bSelect) 
                         SetSelectionCharFormat(cf);     //为选定的内容设定所选字体 
             else 
                         SetWordCharFormat(cf);         //为将要输入的内容设定字体 
}

在RichEdit中实现超链接

在RichEdit中实现超链接 
责任编辑：admin 　 在CBuilder上制作 更新日期：2005-8-6 

首先在Form上放置一个RichEdit。

在窗体的构造函数中添加以下代码：
__fastcall TMainForm::TMainForm(TComponent* Owner)
         : TForm(Owner)
{
     unsigned mask = SendMessage(RichEdit1->Handle, EM_GETEVENTMASK, 0, 0);
     SendMessage(RichEdit1->Handle, EM_SETEVENTMASK, 0, mask | ENM_LINK);
     SendMessage(RichEdit1->Handle, EM_AUTOURLDETECT, true, 0);   //自动检测URL

     RichEdit1->Text = "欢迎访问C++ Builder/n"
                       "网址: [http://www.ccrun.com/n](http://www.ccrun.com/n)"
                       "偶的信箱:/n"
                       "[mailto::info@ccrun.com](mailto::info@ccrun.com) /n"
                       "嘿嘿/n";
}

重载窗体的WndProc

1。在.h中添加：

    protected:
       virtual void __fastcall WndProc(Messages::TMessage &Message);

2。在.cpp中添加：
//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
     if (Message.Msg == WM_NOTIFY)
     {
         if (((LPNMHDR)Message.LParam)->code == EN_LINK)
         {
             ENLINK* p = (ENLINK *)Message.LParam;
             if (p->msg == WM_LBUTTONDOWN)
             {
                 SendMessage(RichEdit1->Handle, EM_EXSETSEL, 0, (LPARAM)&(p->chrg));
                 ShellExecute(Handle, "open", RichEdit1->SelText.c_str(), 0, 0, SW_SHOWNORMAL);
             }
         }
     }
     TForm::WndProc(Message);
} 

