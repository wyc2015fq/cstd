# CFileDialog和ShellExecute冲突的解决方法 - 战斗蜗牛的专栏 - CSDN博客





2010年01月31日 16:26:00[vbskj](https://me.csdn.net/vbskj)阅读数：757








因为用CFileDialog,选择了文件后,后会将当前目录改为你所选择文件所在的目录,所以就出现找不到del.bat了,   
  所以在打开CFileDialog之前要先记录当前路径   
  char   path[256];   
  GetCurrentDirectory   (250,path);   
  TCHAR   lpszFilter[]="Asm   Files   (*.asm)|*.asm|All   Files   (*.*)|*.*||";   
  CFileDialog   FileDialog(TRUE,"asm,"*.asm",OFN_HIDEREADONLY,(LPCTSTR)lpszFilter);   
  if(FileDialog.DoModal()==IDOK)     
  {   
      CString   PathName=FileDialog.GetPathName();     
                      ///////////////其他操作   

        SetCurrentDirectory   (path);         /////////////////将目录改回程序的当前目录   
  }   

      ShellExecute(NULL,"open","del.bat",NULL,NULL,SW_SHOW);  



