# 获取本地IP地址，并在IP CONTROL控件中显示出来 - SHINING的博客 - CSDN博客
2010年05月13日 09:27:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：6964
char sHostName[100];
 gethostname(sHostName,sizeof(sHostName));
 struct hostent FAR * lpHostEnt=gethostbyname(sHostName);
 if(lpHostEnt==NULL)
 {
  MessageBox(_T("error"));
 }
 //获取IP
 LPSTR lpAddr=lpHostEnt->h_addr_list[0];
 if(lpAddr)
 {
  struct in_addr inAddr;
  memmove(&inAddr,lpAddr,4);
  CString str;
  str=inet_ntoa(inAddr);
  m_edit=str;
  UpdateData(0);
  DWORD  dw1=inet_addr(inet_ntoa(inAddr));
  WORD  wd1=LOWORD(dw1);//取高十六位
  WORD  wd2=HIWORD(dw1);//取低十六位
  BYTE  bte1=LOBYTE(wd1);
  BYTE  bte2=HIBYTE(wd1);
  BYTE  bte3=LOBYTE(wd2);
  BYTE  bte4=HIBYTE(wd2);
  m_ip.SetAddress(bte1,bte2,bte3,bte4);
 //m_ip.SetAddress(inet_addr(inet_ntoa(inAddr)));
 }
取出的IP地址，先转化为CString，紧接着转化为DWORD。如果这时候直接将DWORD在IP CONTROL中显示出来的话，将是一个不正确顺序的IP，低位和高位的顺序完全相反。只有将里面的数据取出，放在BYTE里，然后按照从低到高的顺序在IP CONTROL中显示出来，才能正确的显示本地IP。
该程序在VS2008.NET SP1环境下调试通过。
