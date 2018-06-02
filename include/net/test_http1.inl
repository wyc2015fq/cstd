#include "HttpSocket.inl"
#include "SocketBuffer.inl"
UINT Download(const char* strFileName)
{
  CHttpSocket HttpSocket;
  CString strServer, strObject;
  unsigned short nPort;
  DWORD dwServiceType;
  long nLength;
  const char* pRequestHeader = NULL;
  AfxParseURL(pMainWnd->m_strRequest, dwServiceType, strServer, strObject, nPort);
  pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer, (LPTSTR)(LPCTSTR)strObject, nLength);
  HttpSocket.Socket();
  HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer);
  HttpSocket.SendRequest();
  HttpSocket.SetTimeout(10000, 0);
  /*
  int nSize;
  HttpSocket.GetResponseHeader(nSize);*/
  pMainWnd->m_edtEdit1.SetWindowText(pRequestHeader);
  int nLineSize = 0;
  char szLine[256];
  while (nLineSize != -1) {
    nLineSize = HttpSocket.GetResponseLine(szLine, 256);
    if (nLineSize > -1) {
      szLine[nLineSize] = '\0';
      pMainWnd->m_ctrlList.AddString(szLine);
    }
  }
  char szValue[30];
  HttpSocket.GetField("Content-Length", szValue, 30);
  int nSvrState = HttpSocket.GetServerState();
  int nFileSize = atoi(szValue);
  pMainWnd->m_ctrlProgress.ShowWindow(SW_SHOW);
  pMainWnd->m_ctrlProgress.SetRange(0, nFileSize / 1024);
  int nCompletedSize = 0;
  CFile DownloadFile;
  DownloadFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite);
  char pData[1024];
  int nReceSize = 0;
  DWORD dwStartTime, dwEndTime;
  while (nCompletedSize < nFileSize) {
    dwStartTime = GetTickCount();
    nReceSize = HttpSocket.Receive(pData, 1024);
    if (nReceSize == 0) {
      AfxMessageBox("服务器已经关闭连接.");
      break;
    }
    if (nReceSize == -1) {
      AfxMessageBox("接收数据超时.");
      break;
    }
    dwEndTime = GetTickCount();
    DownloadFile.Write(pData, nReceSize);
    nCompletedSize += nReceSize;
    pMainWnd->m_ctrlProgress.SetPos(nCompletedSize / 1024);
    //Speed
    CString strSpeed;
    //strSpeed.Format("%d",dwStartTime -dwEndTime);
    strSpeed.Format("%d", nReceSize);
    pMainWnd->m_stcSpeed.SetWindowText(strSpeed);
  }
  DownloadFile.Close();
  pMainWnd->m_ctrlProgress.ShowWindow(SW_HIDE);
  pMainWnd->m_ctrlProgress.SetPos(0);
  return 0;
}

