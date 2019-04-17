# C++文件(夹)选择对话框 - DoubleLi - 博客园






由于各种应用，我们需要调用系统的打开文件对话框或者打开文件夹对话框，或两者兼有。今遇到这个情况已经解决，特写下这篇博文。

1.打开文件对话框常用的方法是使用系统的CFileDialog。这里介绍另外一种方法就是使用OPENFILENAME这个结构体和GetOpenFileName()这个函数，可以实现单选文件或者多选文件，代码如下：

需引入头文件#include "CommDlg.h"



**[cpp]**[view plain](http://blog.csdn.net/love3s/article/details/8439932#)[copy](http://blog.csdn.net/love3s/article/details/8439932#)





- TCHAR szBuffer[MAX_PATH] = {0};   
- OPENFILENAME ofn= {0};   
- ofn.lStructSize = sizeof(ofn);   
- ofn.hwndOwner = m_hWnd;   
- ofn.lpstrFilter = _T("Exe文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0");//要选择的文件后缀   
- ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径   
- ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
- ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer);   
- ofn.nFilterIndex = 0;   
- ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
- BOOL bSel = GetOpenFileName(&ofn);   




这样就可以打开选择文件对话框了。可以选择需要的文件。szBuffer是存放的选择的文件路径。



1.1 多选

下面的代码演示了如何得到GetOpenFileName允许多选的情况下选择的文件名
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
 OPENFILENAME ofn;
 TCHAR szOpenFileNames[80*MAX_PATH];
 TCHAR szPath[MAX_PATH];
 TCHAR szFileName[80*MAX_PATH];
 TCHAR* p;
 int nLen = 0;

 ZeroMemory( &ofn, sizeof(ofn) );
 ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
 ofn.lStructSize = sizeof(ofn);
 ofn.lpstrFile = szOpenFileNames;
 ofn.nMaxFile = sizeof(szOpenFileNames);
 ofn.lpstrFile[0] = '/0';
 ofn.lpstrFilter = TEXT("All Files(*.*)/0*.*/0");

 if( GetOpenFileName( &ofn ) )
 {  
  //把第一个文件名前的复制到szPath,即:
  //如果只选了一个文件,就复制到最后一个'/'
  //如果选了多个文件,就复制到第一个NULL字符
  lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
  //当只选了一个文件时,下面这个NULL字符是必需的.
  //这里不区别对待选了一个和多个文件的情况
  szPath[ ofn.nFileOffset ] = '/0';
  nLen = lstrlen(szPath);

  if( szPath[nLen-1] != '//' )   //如果选了多个文件,则必须加上'//'
  {
   lstrcat(szPath, TEXT("//"));
  }

  p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件

  ZeroMemory(szFileName, sizeof(szFileName));
  while( *p )
  {   
   lstrcat(szFileName, szPath);  //给文件名加上路径  
   lstrcat(szFileName, p);    //加上文件名  
   lstrcat(szFileName, TEXT("/n")); //换行   
   p += lstrlen(p) +1;     //移至下一个文件
  }
  MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);
 }
} 



2.打开文件夹对话框方法：

需引入头文件：#include "Shlobj.h"





**[cpp]**[view plain](http://blog.csdn.net/love3s/article/details/8439932#)[copy](http://blog.csdn.net/love3s/article/details/8439932#)





- TCHAR szBuffer[MAX_PATH] = {0};   
- BROWSEINFO bi;   
- ZeroMemory(&bi,sizeof(BROWSEINFO));   
- bi.hwndOwner = NULL;   
- bi.pszDisplayName = szBuffer;   
- bi.lpszTitle = _T("从下面选文件夹目录:");   
- bi.ulFlags = BIF_RETURNFSANCESTORS;   
- LPITEMIDLIST idl = SHBrowseForFolder(&bi);   
- if (NULL == idl)   
- {   
- return;   
- }   
- SHGetPathFromIDList(idl,szBuffer);   






上面的代码就可以打开一个选择文件夹对话框了。



3.选择文件或文件夹对话框的调用：







**[cpp]**[view plain](http://blog.csdn.net/love3s/article/details/8439932#)[copy](http://blog.csdn.net/love3s/article/details/8439932#)





- TCHAR szBuffer[MAX_PATH] = {0};   
- BROWSEINFO bi;   
- ZeroMemory(&bi,sizeof(BROWSEINFO));   
- bi.hwndOwner = NULL;   
- bi.pszDisplayName = szBuffer;   
- bi.lpszTitle = _T("从下面选择文件或文件夹:");   
- bi.ulFlags = BIF_BROWSEINCLUDEFILES;   
- LPITEMIDLIST idl = SHBrowseForFolder(&bi);   
- if (NULL == idl)   
- {   
- return;   
- }   
- SHGetPathFromIDList(idl,szBuffer);   




使用了上面的代码就可以根据需要选择文件或者文件夹了。



### 4. 创建一个可以新建文件夹且指定选中初始路径的浏览文件夹对话框



由于我在实际工作中需要的就是一个有新建文件夹功能且指定初始选中路径的浏览文件夹对话框，就把这个需求当做扩展应用吧，由于对话框样式由ulFlags标记确定，而在系统头文件SHLOBJ.h头文件中给出的对话框样式只有如下几种

// Browsing for directory.

#define BIF_RETURNONLYFSDIRS   0x0001  // For finding a folder to start document searching

#define BIF_DONTGOBELOWDOMAIN  0x0002  // For starting the Find Computer

#define BIF_STATUSTEXT         0x0004

#define BIF_RETURNFSANCESTORS  0x0008

#define BIF_EDITBOX            0x0010

#define BIF_VALIDATE           0x0020   // insist on valid result (or CANCEL)



#define BIF_BROWSEFORCOMPUTER  0x1000  // Browsing for Computers.

#define BIF_BROWSEFORPRINTER   0x2000  // Browsing for Printers

#define BIF_BROWSEINCLUDEFILES 0x4000  // Browsing for Everything

没有满足我需求的样式，通过csdn查到其实有一个支持新建文件夹功能的样式值0x40，通常网络上给出宏为BIF_NEWDIALOGSTYLE和BIF_USENEWUI，由于不知道在具体哪个头文件中，所以我们可以在代码中自己定义一下这两个宏，具体如下

#define BIF_NEWDIALOGSTYLE   0x40

#define BIF_USENEWUI (BIF_NEWDIALOGSTYLE|BIF_EDITBOX)

这样一来第一个问题解决了，那么如何让对话框有初始选中的文件夹路径呢，我起初想着通过pidlRoot，结果撞了一鼻子灰，原来设定初始选中文件夹路径，是通过那个神奇的回调函数来实现，换句话来说你调用SHBrowseForFolder也就好比你调用了CDialog:: DoModal()函数，具体这个对话框里面的类似初始化，选择等操作的不同实现就通过lpfn这个回调函数来实现了。

下面给出这个简单扩展的代码

#define BIF_NEWDIALOGSTYLE   0x40



int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  

{

       if(uMsg == BFFM_INITIALIZED)

       {  

              SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);

       }

       return 0;  

}



void CSisAppendMidDlg::OnButtonSisdes()

{

       // TODO: Add your control notification handler code here

       BROWSEINFO bi;

       char Buffer[MAX_PATH];

       //初始化入口参数bi开始

       bi.hwndOwner = NULL;

       bi.pidlRoot =NULL;//初始化制定的root目录很不容易

       bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框

       bi.lpszTitle = "选择Sis目标文件路径";

       bi.ulFlags = BIF_EDITBOX|BIF_NEWDIALOGSTYLE;  

       CFileFind   finder;

       if(finder.FindFile(m_cSisDes)==FALSE)

       {

              bi.lParam =0;

              bi.lpfn = NULL;

       }

       else

       {

              bi.lParam = (long)(m_cSisDes.GetBuffer(m_cSisDes.GetLength()));//初始化路径，形如(_T("c:\\Symbian"));

              bi.lpfn = BrowseCallbackProc;

       }

       finder.Close();

       bi.iImage=IDR_MAINFRAME;

       //初始化入口参数bi结束

       LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框

       if(pIDList)

       {

          SHGetPathFromIDList(pIDList, Buffer);

          //取得文件夹路径到Buffer里

          m_cSisDes = Buffer;//将路径保存在一个CString对象里

       }

       UpdateData(FALSE);



       // free memory used     

    IMalloc * imalloc = 0;

       if ( SUCCEEDED(SHGetMalloc( &imalloc)))

       {

              imalloc->Free (pIDList);

              imalloc->Release();

       }



}

好了，简单扩展就到这里为止，至于很多大神需要再做更深一步的扩展，那就沿着这个思路走下去就可以了。毕竟我目前也就走到这一步了，呵呵。









