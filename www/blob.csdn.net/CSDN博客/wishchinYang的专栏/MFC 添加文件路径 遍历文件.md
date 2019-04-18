# MFC 添加文件路径 遍历文件 - wishchinYang的专栏 - CSDN博客
2013年07月17日 12:44:49[wishchin](https://me.csdn.net/wishchin)阅读数：796
个人分类：[MFC编程																[C++编程](https://blog.csdn.net/wishchin/article/category/1508333)](https://blog.csdn.net/wishchin/article/category/1508409)
.添加MFC选择文件路径，使用MessageBox显示信息。
```cpp
void CMyCalLawsDlg::OnBnClickedAddfolder()
{
    wchar_t* p;
    wchar_t szPath[MAX_PATH];    //存放选择的目录路径 
    p = szPath;
    CString str;
    CString str2;
    CString FileName;
    ZeroMemory(szPath, sizeof(szPath));  
    BROWSEINFO bi;      //windows API
    bi.hwndOwner = m_hWnd;      
    bi.pidlRoot = NULL;      
    bi.pszDisplayName = (LPSTR)szPath;      
    bi.lpszTitle = (LPSTR)"请选择需要打包的目录：";      
    bi.ulFlags = 0;      
    bi.lpfn = NULL;      
    bi.lParam = 0;      
    bi.iImage = 0;      //弹出选择目录对话框    
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);  
    if(lp && SHGetPathFromIDList(lp, (LPSTR)szPath))      
    {        
        //str.Format("选择的目录为 %s", (const wchar_t *)szPath); //不能将参数 1 从“const char [16]”转换为“const wchar_t *”
        str.Format("选择的目录为 %s", p); //在你的项目属性里面把编码方式改为"使用多字节字符集"
        AfxMessageBox(str);
    }    
    else
    {
        str2.Format("无效的目录，请重新选择%d", 2);
        AfxMessageBox(str2);
        //AfxMessageBox("无效的目录，请重新选择",NULL,NULL);//error C2665: “AfxMessageBox”: 2 个重载中没有一个可以转换所有参数类型 0换为NULL
    }
    FileFolderName = str;
    CFile myFile;
    //载入图像！！！
    CString FileLastName;
    FileFolderName ="/res/SignSample/";
    FileLastName.Format("%s正样本（%d）.bmp",FileFolderName, 1); //
    IplImage* LoadImage;
    LoadImage= cvLoadImage(FileLastName, 1);//经测试，未能载入！！！
    cvShowImage("Test",LoadImage);
    IplImage* ShowImage =MyResizeImage(LoadImage);
    MyShowImage(ShowImage,IDC_SHOWPIC);
}
//.添加MFC选择文件路径,使用Boost方法或许更好些，可以移植！
```
