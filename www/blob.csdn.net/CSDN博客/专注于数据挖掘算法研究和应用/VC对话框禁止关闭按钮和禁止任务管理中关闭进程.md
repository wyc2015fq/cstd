# VC对话框禁止关闭按钮和禁止任务管理中关闭进程 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年11月28日 09:28:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5425








1.BOOL C***Dlg::OnInitDialog()

{

    //禁止关闭对话框
 CMenu* pMenu = this->GetSystemMenu(FALSE);
 pMenu->ModifyMenu(SC_CLOSE,MF_BYCOMMAND | MF_GRAYED );
    //禁止在任务管理器中关闭进程
 SetTimer(1,m_nTimer,NULL);//设置1号定时器，循环调用stopKillProcess()函数，禁止任务管理器中关闭进程

}



2.void CClientTracerDlg::stopKillProcess()//禁止任务管理器中关闭进程
{
 HWND hwnd;
 int  iItem=0;
 LVITEM lvitem, *plvitem;
 char ItemBuf[512],*pItem;
 DWORD PID;
 HANDLE hProcess;

 // 查找任务管理器ListView窗口句柄
 hwnd=::FindWindow("#32770",_T("Windows 任务管理器"));
 hwnd=::FindWindowEx(hwnd,0,"#32770",0);
 hwnd=::FindWindowEx(hwnd,0,"SysListView32",0);

 // Windows任务管理器尚未启动则返回
 if (!hwnd) 
  return;
 else
 {
  // 没有指定目标进程则返回
  iItem=::SendMessage(hwnd,LVM_GETNEXTITEM,-1,LVNI_SELECTED);
  if (iItem==-1) 
   return;
  else
  {
   // 获取进程句柄操作失败则返回
   GetWindowThreadProcessId(hwnd, &PID);
   hProcess=OpenProcess(PROCESS_ALL_ACCESS,false,PID);
   if (!hProcess)
    return;   
   else
   {
    plvitem=(LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);
    pItem=(char*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);

    // 无法分配内存则返回
    if ((!plvitem)||(!pItem))
     return;    
    else
    {
     lvitem.cchTextMax=512;
     lvitem.iSubItem=0;  //ProcessName
     lvitem.pszText=pItem;
     WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEM), NULL);
     ::SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)plvitem);
     ReadProcessMemory(hProcess, pItem, ItemBuf, 512, NULL);

     // 比较字符串,匹配进程映像名
     CString str = (CString)ItemBuf;
     if(str.CompareNoCase(_T("ClientTracer.exe")) == 0)
     {
      HWND hWnd=::FindWindow(NULL,_T("Windows 任务管理器"));
      ::SendMessage(hWnd,WM_DESTROY,0,0);
      Sleep(100);
      ::MessageBox(NULL,_T("禁止关闭系统关键进程!"),_T("提示"),MB_ICONERROR | MB_OK);
     }
    }
   }
  }
 }

 //释放内存
 CloseHandle(hwnd);
 CloseHandle(hProcess);
 VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
 VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
}

3.定时器设置调用stopKillProcess()，也可以多线程调用stopKillProcess()

void C***Dlg::OnTimer(UINT nIDEvent) 
{
 // TODO: Add your message handler code here and/or call default
 if(nIDEvent == 1)
 {
  stopKillProcess();
 }
 CDialog::OnTimer(nIDEvent);
}



