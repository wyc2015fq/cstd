# 一步一步学Edit Control控件的用法 - SHINING的博客 - CSDN博客
2010年03月23日 15:31:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：16685
Edit Control控件最常见的用法，一般有有以下几种：
1、显示默认的字符串；
2、接受用户输入的字符串。
3、作为密码框接受用户输入的字符串。
Edit Control对应的MFC类为CEdit类。而CEdit是继承自CWnd类的，所以也继承了来自CWnd类的GetWindowText()和SetWindowText()。其实从这两个函数的名字大家都可以看出来是做什么用的了。
我们来一步一步实现Edit Control控件的这三种最常见的用法。
首先，先建立一个项目，在VC++中选择MFC，选择MFC应用程序，并选择基于对话框的。项目完全打开后，我们可以看到默认生成的对话框，我们从工具箱拉一个Edit Control到对话框上。使用Ctrl+左键双击，创建一个和Edit Control控件的一个变量，也即一个CEdit类的对象。此处为m_edit.
接下来我们可以在对话框的CXXXDlg::OnInitDialog()中添加代码。此处XXX是项目名称。在此函数中添加:
m_edit.SetWindowText(_T("welcome to edit"));
然后我们调试程序，就可以看到对话框上面放置的Edit Control中显示了上述字符串的内容。其实这个函数很简单，只是一个需要设置的字符串。我们来看下关于这个函数的具体内容：
CWnd::SetWindowText
void SetWindowText( LPCTSTR lpszString );
参数：
|lpszString|指向一个CString对象或以null结尾的字符串，将被用作新的标题或控件文本。|
说明：
这个函数将窗口的标题设为指定的文本。如果窗口为一个控件，则将设置控件内的文本。
这个函数使一条WM_SETTEXT消息被发送到这个窗口。
而我们要得到Edit Control控件中输入的内容的话，我们需要一个触发。此处我们以点击确定键为触发。双击资源中的对话框上的确定键，我们可以直接到达函数CXXXtDlg::OnBnClickedOk()的位置，然后我们可以在这个函数中添加如下代码：
CString lpszStringBuf;
m_edit.GetWindowText(lpszStringBuf);
MessageBox(lpszStringBuf);
这几句代码可以接受控件的文本内容，然后以一个消息的形式显示出来。
我们也来具体分析下GetWindowText:
CWnd::GetWindowText
int GetWindowText( LPTSTR lpszStringBuf, int nMaxCount ) const;
void GetWindowText( CString& rString ) const;
返回值：
指定了要拷贝的字符串的长度，以字节为单位，不包括结尾的空字符。如果CWnd没有标题或标题为空，则为0。
参数：
|lpszStringBuf|指向要接收窗口标题的复制字符串的缓冲区。|
|----|----|
|nMaxCount|指定了要拷贝的缓冲区的最大字符数目。如果字符串比nMaxCount指定的数目还要长，则被截断。|
|rString|用于接收窗口标题的复制字符串的CString对象。|
说明：
这个函数将CWnd的标题（如果有）拷贝到lpszStringBuf指向的缓冲区或者目的字符串rString。如果CWnd对象是一个控件，则GetWindowText成员函数将拷贝控件内的文本（而不是控件的标题）。这个成员函数会向CWnd对象发送一个WM_GETTEXT消息。
其实这两个功能的实现都很简单，只是使用了一个函数就可以实现了。如果要是想做一个密码输入框怎么办呢？其实跟上面的比起来，只需要在Edit Control控件的属性中将Password的属性改为TRUE就可以了。怎么样，很简单吧，我们来试一试吧。
