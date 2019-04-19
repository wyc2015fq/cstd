# 获取CPU厂商名称程序（直接粘贴进去即可用） - SHINING的博客 - CSDN博客
2011年03月24日 21:28:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1486
HKEY hKey;
 LPCTSTR StrKey="HARDWARE//DESCRIPTION//System//CentralProcessor//0";
 if (ERROR_SUCCESS==::RegOpenKeyEx(HKEY_LOCAL_MACHINE,StrKey,
                                                                    NULL,KEY_ALL_ACCESS,&hKey))
 {
  DWORD dwSize=255,dwType=REG_SZ;
  char String[256]; 
  LPCSTR KeyValue="VendorIdentifier";
  if (ERROR_SUCCESS==::RegQueryValueEx(hKey,KeyValue,0,&dwType,
                                   (BYTE *)String,&dwSize))
  {
   CString StrData=String;
   MessageBox("CPU的制造商为："+StrData,"信息提示",MB_OK);
  }
  ::RegCloseKey(hKey);
 } 
