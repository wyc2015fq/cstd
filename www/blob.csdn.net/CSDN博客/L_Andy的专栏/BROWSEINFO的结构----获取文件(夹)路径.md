# BROWSEINFO的结构----获取文件(夹)路径 - L_Andy的专栏 - CSDN博客

2013年11月19日 15:02:56[卡哥](https://me.csdn.net/L_Andy)阅读数：8761


                
BROWSEINFO的结构如下：

typedef struct _browseinfo { 

    HWND hwndOwner;        

       // 父窗口句柄

    LPCITEMIDLIST pidlRoot;              

      // 要显示的文件夾的根(Root)

    LPTSTR pszDisplayName; 

      // 保存被选取的文件夾路径的缓冲区

    LPCTSTR lpszTitle;            

       // 显示位于对话框左上部的标题

    UINT ulFlags;              

      // 指定对话框的外观和功能的標志

    BFFCALLBACK lpfn;                    

      // 处理事件的回调函数

    LPARAM lParam;               

      // 应用程序传给回调函数的参数

    int iImage;               

      // 保存被选取的文件夾的图片索引

} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO

pszDisplayName

 　　-- 這個参数指向一個缓冲区，SHBrowseForFolder默认这个缓冲区的大小为MAX_PATH(系统定义的宏，表示一个路径名的最大长度)，并用它来保存被选取的文件夹的路径。

　　ulFlags -- 

　　SHBrowseForFolder允许我们指定对话框的功能和外观，这个参数正是起这样的用途。它的值可以由十几个宏组合而成。比如BIF_RETURNNONLYFSDIRS是返回文件系统的目录，BIF_BROWSEFORCOMPUTER是返回网络上的计算机名，BIF_EDITBOX是显示一个编辑框，允许用户输入文件夾名。一般来说，如果是简单的用于选择一个文件夹，设置ulFlags = BIF_RETURNONLYLYFSDIRS即可。

　　pidRoot -- 

　　它指向一个ITEMIDLIST结构，作为在列表中显示的文件夹的根目录，如果为NULL則默认为Desktop。ITEMIDLIST结构除了用在这里外，它还用作函数的返回值(參看函数原型)。那为什么不用字符串来表示这个目录呢，这个结构怎样表示一个目录呢?下面作一個简单的介紹。

ITEMID和ITEMIDLIST

　　Windows Shell的一个功能在于管理并提供方法存取系统中的众多对像，這些对像包括了文件，网路上的计算机，控制面板程序，回收站等等，为了识别每一个对像，Windows Shell使用了Item ID来表示它們，而Iten ID Lists用来表示一个对像的路径。所以，ITEMID和ITEMIDLIST的关系类似于文件名和路径的关系。如果只对文件系统而言的话，ITEMIDLIST可以看成是路径的另一中表示法，Windows Shell也提供了函数來进行转化。

　　BOOL SHGetPathFromIDList( LPCITEMIDLIST pidl, LPSTR pszPath )

　这个函数將一个ITEMLIST转换成文件系统中的路径。


返回值

　　SHBrowseForFolder的返回值也是一个指向ITEMIDLIST的指針。这个ITEMIDLIST正式表示了用戶所选择的文件夹。如上所述，通过SHGetPathFromIDList，就可以获得一个文件夹Path的字符串。

　　要注意的是返回的指針必须由应用程序自己来释放。并且，由于SHBrowseForFolder是通贵调用IMalloc Interface来分配Memory，所以，也必须通过这个Interface來释放。

实例一：

char szPath[256];
BROWSEINFO bi;
LPITEMIDLIST pIDL;
bi.hwndOwner = m_hWnd;
bi.pidlRoot = NULL;
bi.pszDisplayName = szPath; 
bi.lpszTitle = m_strTitle;
bi.ulFlags = BIF_RETURNONLYFSDIRS;
bi.lpfn = NULL;
bi.lParam = 0;
bi.iImage = 0;
pIDL = SHBrowseForFolder( &bi );
if(SHGetPathFromIDList(pIDL,szPath))
{
m_sMapPath = szPath;
UpdateData(FALSE);
}

实例2

//获取路径

//CString CMyDlg::BrowseDlg(CString lpszTitle)

//{

//ITEMIDLIST *ppidl;

//SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP ,&ppidl);

//if ( ppidl == NULL)

//{

//AfxMessageBox(_T("启动路径浏览失败") ) ; 

//return _T("error");

//}

//TCHAR path[255] = _T("");

//BROWSEINFO *bi=new BROWSEINFO;

//bi->hwndOwner=NULL;

//bi->pidlRoot=ppidl;  

//bi->pszDisplayName=NULL;     

//bi->lpszTitle=lpszTitle;     

//bi->lpfn=NULL;

//bi->ulFlags=/*BIF_BROWSEINCLUDEFILES|*/BIF_EDITBOX |BIF_RETURNONLYFSDIRS  ;

//ppidl = SHBrowseForFolder(bi); 

//if ( !SHGetPathFromIDList(ppidl,path) ) 

//{

//delete bi;

//return _T("error") ; 

//}

//delete bi;

//CString s = path ; 

//if ( s.Right( 1 ) != _T("\\") ) 

//s+= _T("\\") ; 

//

//return s ; 

//}

