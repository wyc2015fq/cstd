# MFC窗口支持文件拖入 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月11日 16:56:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：23
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










修改对话框属性为Accept Files
 或者在初始化时直接通过函数DragAcceptFiles()使得窗口支持文件拖曳
 然后，增加对话框窗口的WM_DROPFILES消息响应函数

 单个文件：

```cpp
OnDropFiles(HDROP hDropInfo)
{
	char * pFilePath = new char[256];
	::DragQueryFile(hDropInfo, 0, pFilePath, 256);  // 获取拖放第一个文件的完整文件名
	CString m_FilePath;
	m_FilePath.Format("%s", pFilePath);
	delete[] pFilePath;
/*
	CString m_FilePath;
	::DragQueryFile(hDropInfo, 0, m_FilePath.GetBuffer(256), 256);
	m_FilePath.ReleaseBuffer();
*/
	::DragFinish(hDropInfo);   // 注意这个不能少，它用于释放Windows 为处理文件拖放而分配的内存
}
```



 多个文件：

```cpp
OnDropFiles(HDROP hDropInfo)
{
	int  nFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 256);   //获取拖入的文件数量


	char * pFilePath = new char[256];
	for (int i=0; i<nFileCount; i++)
	{
		UINT nChars = ::DragQueryFile(hDropInfo, i, pFilePath,256);   // 获取拖入的第i个文件的文件名
		CString m_FilePath(pFilePath, nChars); 
	}
	delete[] pFileFilePath;
/*
	CString * m_FilePath = new CString[nFileCount];
	for (int i=0; i<nFileCount; i++)
	{
		UINT nChars = ::DragQueryFile(hDropInfo, i, m_FilePath[i].GetBuffer(256),256);   // 获取拖入的第i个文件的文件名
		m_FilePath[i].ReleaseBuffer();
	}
*/
	::DragFinish (hDropInfo);
}
```








