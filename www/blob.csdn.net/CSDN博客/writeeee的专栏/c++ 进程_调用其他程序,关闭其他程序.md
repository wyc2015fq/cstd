# c++ 进程_调用其他程序,关闭其他程序 - writeeee的专栏 - CSDN博客
2016年01月25日 15:31:21[writeeee](https://me.csdn.net/writeeee)阅读数：626
```cpp
void CMulThread_1Dlg::OnBnClickedButton1()
{	
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	dlg.DoModal();
	m_FilePath=dlg.GetPathName();	
	LoadFileContent(m_FilePath);
}
void CMulThread_1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	PROCESS_INFORMATION pi;
	STARTUPINFO startIno= {sizeof(pi)};
	TCHAR cmdLine[_MAX_PATH];
	memset(cmdLine,0,_MAX_PATH);
	_stprintf(cmdLine,"notepad.exe \"%s\" ",m_FilePath);
	if (CreateProcess(NULL,cmdLine,NULL,NULL,FALSE,0,NULL,NULL,&startIno,&pi))
	{
		m_Process=pi.hProcess;
		/*WaitForSingleObject(pi.hProcess,INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		LoadFileContent(m_FilePath);*/
	}
}
void CMulThread_1Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_Process !=NULL)
	{
		TerminateProcess(m_Process,-1);
		CloseHandle(m_Process);
		LoadFileContent(m_FilePath);
	}
	
}
void CMulThread_1Dlg::LoadFileContent(CString filePath)
{
	m_FileContent=" ";
	CFile curFile;
	BOOL bRet= curFile.Open(filePath,CFile::modeRead);
	if (!bRet)
	{
		return ;
	}
	char tmpBuff[1024];
	while(TRUE)
	{
		memset(tmpBuff,0,sizeof(tmpBuff));
		int readingSize =curFile.Read(tmpBuff,sizeof(tmpBuff));
		m_FileContent +=tmpBuff;
		if (readingSize<sizeof(tmpBuff))
		{
			break;
		}
	}
	UpdateData(FALSE);
	curFile.Close();
}
```
