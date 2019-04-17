# VC用MCI播放mp3等音乐文件 - DoubleLi - 博客园






 VC播放mp3等音乐文件，可以使用MCI。
MCI ( Media Control Interface ) ，即媒体控制接口，向基于Windows操作系统的[应用程序](http://baike.baidu.com/view/330120.htm)提供了高层次的控制媒体设备接口的能力。
    对于所有的命令都是通过mciSendCommand进行发送。
    InfosystemShow中的CMyMCI类是对MCI各种操作的封装，包括打开、播放、暂停等操作，在主程序中，只需调用  
    CMyMCI m_myMCI;
    m_myMCI.MCIOpen("E:\\Kalimba.mp3");
    m_myMCI.MCIPlay();
    while(1)
    {
        Sleep(1000);
    }
    m_myMCI.MCIClose();
    即可完成对音乐的播放。


//打开MCI设备  参数为文件地址
DWORD CMyMCI::MCIOpen(const CString &strPath)
{
  MCI_OPEN_PARMS mciOP;
  DWORD dwReturn;

  ASSERT(m_wID==NULL);
  mciOP.lpstrDeviceType=NULL;
  mciOP.lpstrElementName=strPath;
  dwReturn=mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_WAIT|MCI_OPEN_SHAREABLE,
      (DWORD)(LPVOID)&mciOP);
  if (dwReturn==0)
  {
      m_wID=mciOP.wDeviceID;
      if(this->MCIGetLength(&m_dwLength)!=0)
          m_dwLength=0;
  }
  else
      m_wID=NULL;
  return dwReturn;
}
//关闭MCI设备
DWORD CMyMCI::MCIClose()
{
  MCI_GENERIC_PARMS mciGP;
  DWORD dwReturn;
  dwReturn=mciSendCommand(m_wID,MCI_CLOSE,MCI_NOTIFY|MCI_WAIT,
      (DWORD)(LPVOID)&mciGP);
  m_wID=NULL;
  return dwReturn;
}
//播放
DWORD CMyMCI::MCIPlay()
{
    MCI_PLAY_PARMS mciPP;
    return mciSendCommand(m_wID,MCI_PLAY,MCI_NOTIFY,
        (DWORD)(LPVOID)&mciPP);
}









