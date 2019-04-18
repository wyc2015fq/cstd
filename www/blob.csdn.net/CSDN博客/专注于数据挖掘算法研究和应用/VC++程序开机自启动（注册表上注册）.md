# VC++程序开机自启动（注册表上注册） - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年11月26日 06:49:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3399








//实现程序开机自启动，在注册表上注册
BOOL CClientTracerDlg::autoRun()
{
 HKEY   RegKey; 
 LPCSTR KeyPath="Software//Microsoft//Windows//CurrentVersion//Run"; 
 //打开Run注册项
 if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,KeyPath,0,KEY_READ|KEY_WRITE,&RegKey)==ERROR_SUCCESS)
 {
  LPBYTE lpData = new BYTE[80]; 
  DWORD cbData = 80; 
  DWORD dwType = REG_SZ; 
  char keyValue[]="GD10000-fjs-ClientTracer";
  if(::RegQueryValueEx(RegKey,keyValue,0,&dwType,lpData,&cbData) == ERROR_SUCCESS) //已存在
  {
   //将lpData 转换成CString 类型 
   //CString strValue; 
   //strValue = CString(lpData); //键值
   TRACE("GD10000-fjs-ClientTracer已经存在/n");
   //::RegDeleteValue(RegKey,keyValue);//删除该注册值，区别与RegDeleteKey函数
   ::RegCloseKey(RegKey);//关闭已经打开的注册项
   return TRUE;//已注册值，返回
  }
  else
  {
   //不存在注册自启动项
   ::RegCloseKey(RegKey);//关闭已经打开的注册项
   CString   sPath;   
   GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
   sPath.ReleaseBuffer();   
   int   nPos;   
   nPos=sPath.ReverseFind('//');   
   sPath=sPath.Left(nPos);   
   CString   lpszFile=sPath+"//ClientTracer.exe";
   CFileFind   fFind;   
   BOOL   bSuccess;   
   bSuccess=fFind.FindFile(lpszFile);   
   fFind.Close();   
   if(bSuccess)   
   {
    CString  fullName;
    fullName=lpszFile;
    RegKey=NULL;
    ::RegOpenKey(HKEY_LOCAL_MACHINE,KeyPath,&RegKey);
    ::RegSetValueEx(RegKey,"GD10000-fjs-ClientTracer",REG_SZ,(const   unsigned   char*)(LPCTSTR)fullName,fullName.GetLength());
    this->UpdateData(FALSE);
    ::RegCloseKey(RegKey);//关闭已经打开的注册项
    return TRUE;//注册表注册自动启动成功
   }   
   else
    return FALSE; //注册表注册自动启动失败
  }
 }
 else return FALSE;//打开注册表Run项失败
}



