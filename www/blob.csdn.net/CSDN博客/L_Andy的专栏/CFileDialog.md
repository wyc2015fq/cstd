# CFileDialog - L_Andy的专栏 - CSDN博客

2013年02月19日 17:40:56[卡哥](https://me.csdn.net/L_Andy)阅读数：2554


**CFileDialog**
文件选择对话框的使用：首先构造一个对象并提供相应的参数，构造函数原型如下：

CFileDialog::CFileDialog( BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL,LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL );
参数意义如下：

bOpenFileDialog 为TRUE则显示打开对话框，为FALSE则显示保存对话文件对话框。

lpszDefExt 指定默认的文件扩展名。

lpszFileName 指定默认的文件名。

dwFlags 指明一些特定风格。

lpszFilter 是最重要的一个参数，它指明可供选择的文件类型和相应的扩展名。参数格式如：

"Chart Files (*.xlc)|*.xlc|Worksheet Files (*.xls)|*.xls|Data Files(*.xlc;*.xls)|*.xlc; *.xls|All Files (*.*)|*.*||";文件类型说明和扩展名间用 |分隔，同种类型文件的扩展名间可以用
 ;分割，每种文件类型间用 |分隔，末尾用 ||指明。

pParentWnd 为父窗口指针。
创建文件对话框可以使用DoModal()，在返回后可以利用下面的函数得到用户选择：

CString CFileDialog::GetPathName( ) 得到完整的文件名，包括目录名和扩展名如：c: est est1.txt

CString CFileDialog::GetFileName( ) 得到完整的文件名，包括扩展名如：test1.txt

CString CFileDialog::GetExtName( ) 得到完整的文件扩展名，如：txt

CString CFileDialog::GetFileTitle ( ) 得到完整的文件名，不包括目录名和扩展名如：test1

POSITION CFileDialog::GetStartPosition( ) 对于选择了多个文件的情况得到第一个文件位置。

CString CFileDialog::GetNextPathName( POSITION& pos ) 对于选择了多个文件的情况得到下一个文件位置，并同时返回当前文件名。但必须已经调用过POSITION CFileDialog::GetStartPosition( )来得到最初的POSITION变量。
例如

{

CString

FilePathName;

CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE
 AS对话框

if(dlg.DoModal()==IDOK)

FilePathName=dlg.GetPathName();

}
相关信息：CFileDialog用于取文件名的几个成员函数：
假如选择的文件是C:WINDOWSTEST.EXE
则:

(1)GetPathName();取文件名全称，包括完整路径。取回C:WINDOWSTEST.EXE

(2)GetFileTitle();取文件全名：TEST.EXE

(3)GetFileName();取回TEST

(4)GetFileExt();取扩展名EXE

以上摘自：[http://www.cnblogs.com/eric_lgf/archive/2009/08/02/1536885.html](http://www.cnblogs.com/eric_lgf/archive/2009/08/02/1536885.html)

CFileDialog pCFileDialog(true,NULL,NULL,0,"文本文件(*.txt)|*.txt");

//CFileDialog pCFileDialog最后一个参数是打开文件对话框的筛选器内容,不是文件路径

pCFileDialog.m_ofn.lpstrTitle = "打开文件 ";

if(pCFileDialog.DoModal()!=IDOK)

{

return;

}

CFile pCFile;

char charArrMsg[1024];

int hResult=pCFile.Open(pCFileDialog.GetFileName(),CFile::modeRead);

if(hResult==0)

{

MessageBox("打开文件E:\\FFF.TXT失败!");

return;

}

::memset(charArrMsg,'\0',1024);

pCFile.Read(charArrMsg,1024);

MessageBox(charArrMsg);

pCFile.Close();

------

**第2个回答**

CFile文件操作详解
各种关于文件的操作在程序设计中是十分常见，如果能对其各种操作都了如指掌，就可以根据实际情况找到最佳的解决方案，从而在较短的时间内编写出高效的代码，因而熟练的掌握文件操作是十分重要的。本文将对Visual C++中有关文件操作进行全面的介绍，并对在文件操作中经常遇到的一些疑难问题进行详细的分析。

**1．文件的查找**

当对一个文件操作时，如果不知道该文件是否存在，就要首先进行查找。MFC中有一个专门用来进行文件查找的类CFileFind，使用它可以方便快捷地进行文件的查找。下面这段代码演示了这个类的最基本使用方法。

CString strFileTitle;

CFileFind finder;

BOOL bWorking=finder.FindFile("C:\\windows\\sysbkup\\*.cab");

while(bWorking)

{

bWorking=finder.FindNextFile();

strFileTitle=finder.GetFileTitle();

}

**2．文件的打开/保存对话框**

让用户选择文件进行打开和存储操作时，就要用到文件打开/保存对话框。MFC的类CFileDialog用于实现这种功能。使用CFileDialog声明一个对象时，第一个BOOL型参数用于指定文件的打开或保存，当为TRUE时将构造一个文件打开对话框，为FALSE时构造一个文件保存对话框。
在构造CFileDialog对象时，如果在参数中指定了OFN_ALLOWMULTISELECT风格，则在此对话框中可以进行多选操作。此时要重点注意为此CFileDialog对象的m_ofn.lpstrFile分配一块内存，用于存储多选操作所返回的所有文件路径名，如果不进行分配或分配的内存过小就会导致操作失败。下面这段程序演示了文件打开对话框的使用方法。

CFileDialog mFileDlg(TRUE,NULL,NULL,

OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,

"All Files (*.*) |*.* ||",AfxGetMainWnd());

CString str(" ",10000);

mFileDlg.m_ofn.lpstrFile=str.GetBuffer(10000);

str.ReleaseBuffer();

POSITION mPos=mFileDlg.GetStartPosition();

CString pathName(" ",128);

CFileStatus status;

while(mPos!=NULL)

{

pathName=mFileDlg.GetNextPathName(mPos);

CFile::GetStatus(pathName,status);

}

**3．文件的读写**

文件的读写非常重要，下面将重点进行介绍。文件读写的最普通的方法是直接使用CFile进行，如文件的读写可以使用下面的方法：

//对文件进行读操作

char sRead[2];

CFile mFile(_T("user.txt "),CFile::modeRead);

if(mFile.GetLength()<2)

return;

mFile.Read(sRead,2);

mFile.Close();

//对文件进行写操作

CFile mFile(_T("user.txt"),CFile::modeWrite|CFile::modeCreate);

mFile.Write(sRead,2);

mFile.Flush();                                 //  清除该流的所有缓冲区，使得所有缓冲的数据都被写入到基础设备(清空输入流)

mFile.Close();
虽然这种方法最为基本，但是它的使用繁琐，而且功能非常简单。我向你推荐的是使用CArchive，它的使用方法简单且功能十分强大。首先还是用CFile声明一个对象，然后用这个对象的指针做参数声明一个CArchive对象，你就可以非常方便地存储各种复杂的数据类型了。它的使用方法见下例。

//对文件进行写操作

CString strTemp;

CFile mFile;

mFile.Open("d:\\dd\\try.TRY",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);

CArchive ar(&mFile,CArchive::store);

ar<<ar.Close();

mFile.Close();

//对文件进行读操作

CFile mFile;

if(mFile.Open("d:\\dd\\try.TRY",CFile::modeRead)==0)

return;

CArchive ar(&mFile,CArchive::load);

ar>>strTemp;

ar.Close();

mFile.Close();

CArchive的<<和>>操作符用于简单数据类型的读写，对于CObject派生类的对象的存取要使用ReadObject()和WriteObject()。使用CArchive的ReadClass()和WriteClass()还可以进行类的读写，如：

//存储CAboutDlg类

ar.WriteClass(RUNTIME_CLASS(CAboutDlg));

//读取CAboutDlg类

CRuntimeClass* mRunClass=ar.ReadClass();

//使用CAboutDlg类

CObject* pObject=mRunClass->CreateObject();

((CDialog*)pObject)->DoModal();
虽然VC提供的文档/视结构中的文档也可进行这些操作，但是不容易理解、使用和管理，因此虽然很多VC入门的书上花费大量篇幅讲述文档/视结构，但我建议你最好不要使用它的文档。关于如何进行文档/视的分离有很多书介绍，包括非常著名的《VisualC++技术内幕》。
如果你要进行的文件操作只是简单的读写整行的字符串，我建议你使用CStdioFile，用它来进行此类操作非常方便，如下例。

CStdioFile mFile;

CFileException mExcept;

mFile.Open("d:\\temp\\aa.bat",CFile::modeWrite,&mExcept);

CString string= "I am a string. ";

mFile.WriteString(string);

mFile.Close();

**4．临时文件的使用**

正规软件经常用到临时文件，你经常可以会看到C:\Windows\Temp目录下有大量的扩展名为tmp的文件，这些就是程序运行是建立的临时文件。临时文件的使用方法基本与常规文件一样，只是文件名应该调用函数GetTempFileName()获得。它的第一个参数是建立此临时文件的路径，第二个参数是建立临时文件名的前缀，第四个参数用于得到建立的临时文件名。得到此临时文件名以后，你就可以用它来建立并操作文件了，如：

char szTempPath[_MAX_PATH],szTempfile[_MAX_PATH];

GetTempPath(_MAX_PATH, szTempPath);

GetTempFileName(szTempPath,_T("my_"),0,szTempfile);

CFile m_tempFile(szTempfile,CFile::modeCreate|CFile::modeWrite);

char m_char='a';

m_tempFile.Write(&m_char,2);

m_tempFile.Close();

**5．文件的复制、删除等**

MFC中没有提供直接进行这些操作的功能，因而要使用SDK。SDK中的文件相关函数常用的有CopyFile()、CreateDirectory()、DeleteFile()、MoveFile()。它们的用法很简单，可参考MSDN。

1,判断文件是否存在
access(filename,mode);

2,对于不同用途又不同的文件操作,其中API函数CreateFile()也是比较有用处理方式,对于巨型文件很合适的其他的楼上的大都说了,不重复了.

[1]显示对话框，取得文件名

CString FilePathName;

CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为S***EAS对话框

if(dlg.DoModal()==IDOK)
FilePathName=dlg.GetPathName();

相关信息：CFileDialog用于取文件名的几个成员函数：
假如选择的文件是C:\WINDOWS\TEST.EXE
则(1)GetPathName();取文件名全称，包括完整路径。取回C:\WINDOWS\TEST.EXE

(2)GetFileTitle();取文件全名：TEST.EXE

(3)GetFileName();取回TEST

(4)GetFileExt();取扩展名EXE

[2]打开文件

CFile file("C:\HELLO.TXT",CFile::modeRead);//只读方式打开

//CFile::modeRead可改为CFile::modeWrite(只写),

//CFile::modeReadWrite(读写),CFile::modeCreate(新建)
例子：

{

CFile file;

file.Open("C:\HELLO.TXT",CFile::modeCreate|Cfile::modeWrite);

}

[3]移动文件指针

file.Seek(100,CFile::begin);///从文件头开始往下移动100字节

file.Seek(-50,CFile::end);///从文件末尾往上移动50字节

file.Seek(-30,CFile::current);///从当前位置往上移动30字节

file.SeekToBegin();///移到文件头

file.SeekToEnd();///移到文件尾

[4]读写文件
读文件：

char buffer[1000];

file.Read(buffer,1000);
写文件：

CString string( "自强不息 ");

file.Write(string,8);

[5]关闭文件

file.Close();

-----

**第3个回答**

CFile的Open
或者API CreateFile,ReadFile,WriteFile

