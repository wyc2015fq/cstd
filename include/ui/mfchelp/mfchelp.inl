CString GetfolderPath()  
{  
  CString folderPath;  
  BROWSEINFO bi;  
  char buf[MAX_PATH];  
  
  //初始化入口参数bi  
  bi.hwndOwner = NULL;  
  bi.pidlRoot = NULL;  
  bi.pszDisplayName = buf;    //此参数如为NULL则不能显示对话框  
  bi.lpszTitle = "选择路径";  
  bi.ulFlags = BIF_RETURNONLYFSDIRS;  
  bi.lpfn = NULL;  
  bi.iImage = 0;  
  
  LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);  //调用显示选择对话框  
  if(pIDList)  
  {  
    SHGetPathFromIDList(pIDList,buf);           //取得文件夹路径到buf里  
    folderPath = buf;       //将路径保存在一个CString对象里  
  }  
  folderPath += ("\\");  
  
  return folderPath;  
}  