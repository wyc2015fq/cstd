# VC中使用GetModuleFileName获取应用程序路径（整理） - L_Andy的专栏 - CSDN博客

2012年09月13日 15:59:11[卡哥](https://me.csdn.net/L_Andy)阅读数：3134


.\\与API函数GetModuleFileName获取应用程序目录有何不一样？                                                                                                                                                                    张明奇（卡哥） 

--------------------------------------------------------------------------------

采用.\\也能获得应用程序目录，采用GetModuleFileName也能获得，二者有何不同？ 

--------------------------------------------------------------------------------

一样！

一个是相对路径，一个是绝对路径 

--------------------------------------------------------------------------------

.\\是的到应用程序的当前目录，但当前目录不一定等于应用程序执行文件的所在目录，一个应用程序被启动时，当前目录是可以被任意设置的。

GetModuleFileName()得到模块的完整路径名，例如，你载入c:\windows\system32\a.dll，得到模块句柄h，则你可以用GetModuleFileName()得到h模块的完整路径名。

--------------------------------------------------------------------------------

.\\一般用在包含头文件的语句中。

另一个是程序编译后起作用的，例如，打开自定义的配置文件等。 

--------------------------------------------------------------------------------

如何去取得这个Hanlde? 

--------------------------------------------------------------------------------

如果你直接用LoadLibrary()或AfxLoadLibrary()载入dll，该函数返回值就是handle；

如果你隐式载入dll, 用GetModuleHandle("dll文件名")也可以得到handle； 

MFC程序得到本身路径

在开发工程中，往往需要知道当前程序本身所在目录。

一种方法是在程序安装的时候利用安装程序把文件路径写入注册表。在较大的程序中，这种方法比较常用

另一种，就是在程序得到路径。这样，程序随便移动到哪里，都可以得到正确的路径。这也是本文介绍的方法。

方法一：

[code]

//得到帮助文件的路径

CString strFullName = AfxGetApp()->m_pszHelpFilePath;

//得到的是：X:\XXXX\XXX.hlp

//解析路径，得到当前运行程序所在目录

char drive[_MAX_DRIVE];

char dir[_MAX_DIR];

_splitpath(strAppName, drive, dir, NULL,NULL);

CString strPath;

strPath.Format("%s%s", drive, dir);

//strPath即为得到的当前运行程序所在目录

[/code]

另外，AfxGetApp()->m_pszAppName 得到应用程序名称

AfxGetApp()->m_pszExeName 得到程序文件名，不包括扩展名

方法二：

得到全路径

TCHAR exeFullPath[MAX_PATH]; // MAX_PATH

GetModuleFileName(NULL,exeFullPath,MAX_PATH);//得到程序模块名称，全路径

也就是当前运行程序的全路径

利用方法一的解析路径的方法，即可得到程序所在路径。

GetModuleFileName函数原型

DWORD GetModuleFileName(

   HMODULE hModule,     // handle to module。将要得到的模块的句柄。如果是当前模块，NULL

   LPTSTR lpFilename,   // path buffer   得到的文件名。

   DWORD nSize          // size of buffer   一般MAX_PATH就可以了

//eg:

 CString CWorkModleApp::GetExePath()

{//  获得应用程序的路径

 CHAR pFileName[256];

 ::GetModuleFileName( NULL, pFileName, 255 );//得到绝对路径

 CString csFullPath(pFileName);        //参考cstring

 int iPos = csFullPath.ReverseFind( _T('\\') );    // 从路径字符串的最后开始搜素"\"

 if(iPos < 0 )

 {

  return CString("");

 }

 else 

 {

  return csFullPath.Left( iPos + 1);// 截取字符串，将最后一个"\"以及其左方的所有字符截取出来，也就是全路径

 }

}

