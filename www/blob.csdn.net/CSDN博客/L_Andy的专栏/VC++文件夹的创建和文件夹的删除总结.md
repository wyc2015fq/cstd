# VC++文件夹的创建和文件夹的删除总结 - L_Andy的专栏 - CSDN博客

2013年11月24日 17:55:04[卡哥](https://me.csdn.net/L_Andy)阅读数：14330


                
假设我想判断C:\Program Files中有没有一个名为hotel的文件夹，有的话就什么也不做，没有的话就创建一个名为hotel的文件夹，

请问这个代码应该怎么写呢？（能创建一个隐藏的文件夹更好）

//答案

一、BOOL CreateDirectory(

  LPCTSTR lpPathName,                         // directory name

  LPSECURITY_ATTRIBUTES lpSecurityAttributes  

               // Pointer to a SECURITY_ATTRIBUTES structure

);

要判断文件夹是否存在，首先用CFindFile类得到文件（可以判断是否存在），存在的话用IsDirectory判断是否文件夹

二、BOOL PathIsDirectory(

    LPCTSTR pszPath

    );////判断一个目录是否存在！

三、

#include <windows.h>

#include <iostream> 

using namespace std;

int main()

{

char szDirName[] = "C:\\Program Files\\hotel";

if( CreateDirectory(szDirName, NULL) ) //判断是否存在，否则创建

{

cout<<szDirName<<" : Not Exist"<<endl; 

SetFileAttributes(szDirName, FILE_ATTRIBUTE_HIDDEN ); //隐藏(设置为隐藏)

cout<<"Create Successed!"<<endl;

}

return 0;

}

四在vc++6.0

#include <direct.h> 　

#include <stdio.h> 　

#include <stdlib.h>//for system()函数 　

int main(void) 　

{ 　　

     _mkdir("D:\\myd"); 　　//这个就是创建一个目录 mkdir 就是make directory 的意思

     system("dir D:\\myd"); 　//这是打开这个目录 的意思

　   system("start D:"); 　　 // 这是打开一个独立窗口程序来运行 也就是开一个黑屏dos界面

     getchar()(); 　           //这个是从屏幕获得输入

　   _rmdir("D:\\myd"); 　　 //这个是移除这个目录

     return 0; 　　            //返回0

}

//////////////////////////////////////////////////////////////////////////////////////////////////////

VC如何删除文件夹下的文件

BOOL CDlgData::DeleteDirectory(char *sDirName)

{

    CFileFind tempFind; 

    char sTempFileFind[200] ;

    sprintf(sTempFileFind,"%s\\*.*",sDirName); 

    BOOL IsFinded = tempFind.FindFile(sTempFileFind); 

    while (IsFinded) 

    { 

        IsFinded = tempFind.FindNextFile(); 

        if (!tempFind.IsDots()) 

        { 

            char sFoundFileName[200]; 

            strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

            if (tempFind.IsDirectory()) 

            { 

                char sTempDir[200]; 

                sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName); 

                DeleteDirectory(sTempDir); 

            } 

            else 

            { 

                char sTempFileName[200]; 

                sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName); 

                DeleteFile(sTempFileName); 

            } 

        } 

    } 

    tempFind.Close(); 

    if(!RemoveDirectory(sDirName)) 

    { 

        return FALSE; 

    } 

    return TRUE; 

}

注释

DeleteDirectory(sTempDir); 

这个函数只能删除空目录。你必须先删除里面的所有文件才行

可以用WINSHELLAPI int WINAPI SHFileOperation(

    LPSHFILEOPSTRUCT lpFileOp

   );函数删除包含文件的目录

////////////////////////////////////////////

