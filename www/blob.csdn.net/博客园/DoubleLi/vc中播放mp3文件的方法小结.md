# vc中播放mp3文件的方法小结 - DoubleLi - 博客园






一般播放MP3常见的有两种方法，一种是自己解码，另外一种用系统的库，比如MCI，当然如果可以用控件直接用个控件会更方便。

1.      使用mci



#include <windows.h>

#include <stdio.h>

#include <mmsystem.h>

#include <shellapi.h>

#include <AFXCOM_.H>



#pragma comment(lib,"winmm.lib")



void main()

{

 char str[128]={0};

 int i = 0;

 char buf[128]={0};



 MCI_OPEN_PARMS mciOpen;

 MCIERROR mciError;

 mciOpen.lpstrDeviceType = "mpegvideo";

 mciOpen.lpstrElementName = "c://1.mp3";



 mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&mciOpen);

 if(mciError)

 {

  mciGetErrorString(mciError,buf,128);

  printf("%s/n",buf);

  goto Error;

 }

 UINT DeviceID = mciOpen.wDeviceID ;

 MCI_PLAY_PARMS mciPlay;



 mciError = mciSendCommand(DeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);

 if(mciError)

 {

  printf("send MCI_PLAY command failed/n");

  goto Error;

 }



 //WinExec("sndvol32.exe",SW_SHOWNORMAL);

//这个可以打开音量控制不过可以用编程实现。



 while(1)

 {

  sprintf(str,"播放时间:/t%d",i);

  printf("%s/r",str);

  i++;

  Sleep(1000);

 }

Error:

 system("pause");



}



2.使用库vfw.h

#include <vfw.h>

#pragma comment(lib,"vfw32.lib")

void CMp3PlayerDlg::OnBnClickedButton1()

{

    // TODO: 在此添加控件通知处理程序代码

    CString szFilters="MyType Files(*.mp3)|All Files(*.*)|*.*||";

CFileDialog file(TRUE,"mp3","*.mp3",OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szFilters,this);



     if(file.DoModal()==IDOK)

     {



         CString filename=file.GetFileName();



         if(hwnd!=NULL)



         {



              MCIWndDestroy(hwnd);



         }

         hwnd=MCIWndCreate(this->m_hWnd,NULL,MCIWNDF_NOPLAYBAR,filename);



         ::ShowWindow(hwnd,SW_HIDE);



         MCIWndSetVolume(hwnd,1000);



         MCIWndPlay(hwnd);



     }

}

3.使用directshow

#include   <dshow.h>

#pragma comment(lib,"strmiids.lib")



    IGraphBuilder*   pGBuilder;  

    IMediaControl*   pMControl;  

    IMediaPosition*   pMPos;  

    IMediaEvent*   pMEvent; 

    CoInitialize(NULL);   //初始化COM  

CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder,(void**)&pGBuilder);  



  pGBuilder->QueryInterface(IID_IMediaControl,(void**)&pMControl);  



  pGBuilder->QueryInterface(IID_IMediaEvent,(void**)&pMEvent);  



  pGBuilder->QueryInterface(IID_IMediaPosition,(void**)&pMPos);



  CHAR   strSoundPath[MAX_PATH];   //存储音乐所在路径 



  WCHAR   wstrSoundPath[MAX_PATH];   //存储UNICODE形式的路径 



  GetCurrentDirectory(MAX_PATH,   strSoundPath);  



  strcat(strSoundPath,   "//a.mp3");   //假设要播放的是子目录下的a.mp3  



//  MultiByteToWideChar(CP_ACP,   0,   strSoundPath,   -1,wstrSoundPath,   MAX_PATH); 

  mbstowcs(wstrSoundPath, strSoundPath, MAX_PATH);



  pGBuilder->RenderFile(wstrSoundPath,   NULL);   //调入文件 





  //播放MP3文件 



  //播放MP3的方法十分简单： 



    pMPos->put_CurrentPosition(0);   //移动到文件头 



    pMControl->Run();   //播放 



     CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT1);

     char buf[20];

     sprintf(buf,"%f",pos);

     pEdit->SetWindowTextA(buf);



 // 停止播放和释放对象 



  //最后，我们要停止播放音乐并释放各个对象： 

    pMControl->Stop();   //停止播放 



    pMControl->Release();

    pMControl=NULL;

    pMEvent->Release();

    pMEvent=NULL;

    pMPos->Release();

    pMPos=NULL;

    pGBuilder->Release();

    pGBuilder=NULL;



  CoUninitialize();   //释放COM  

4.使用mci播放MP3

主要变量定义

MCI_OPEN_PARMS mciOpen;

     MCIERROR mciError;

     MCI_STATUS_PARMS mciStatusParms;

WORD m_wDeviceID;

这里只介绍mciSendCommand命令

1.播放

MCI_PLAY_PARMS mciPlay;

mciOpen.lpstrDeviceType = "mpegvideo";

mciOpen.lpstrElementName =fileName.GetString();

    mciError = mciSendCommand(m_wDeviceID,MCI_PLAY,0 ,(DWORD)&mciPlay);

2.暂停

mciSendCommand(mciOpen.wDeviceID,MCI_PAUSE,NULL ,NULL);

3. 停止

mciSendCommand(mciOpen.wDeviceID,MCI_STOP,NULL ,NULL);

    mciSendCommand(mciOpen.wDeviceID,MCI_SEEK,MCI_SEEK_TO_START ,NULL);

4.获得播放进度

mciStatusParms.dwItem=MCI_STATUS_POSITION;    mciSendCommand(m_wDeviceID,MCI_STATUS,MCI_WAIT|MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);//关键,取得位置

    int m_dPosition=mciStatusParms.dwReturn;

5.获得整个音频的时间长度

UINT DeviceID = mciOpen.wDeviceID ;

 mciStatusParms.dwItem=MCI_STATUS_LENGTH; mciSendCommand(DeviceID,MCI_STATUS,MCI_WAIT|MCI_STATUS_ITEM,(DWORD)(LPVOID)&mciStatusParms);//关键,取得长度

     m_dLength=mciStatusParms.dwReturn;

6.在指定位置播放

 MCI_PLAY_PARMS mciPlay;

 mciPlay.dwFrom=m_dPosition;

 mciError = mciSendCommand(m_wDeviceID,MCI_PLAY,MCI_FROM,(DWORD)&mciPlay);



7.控制音量

MCI_DGV_SETAUDIO_PARMS mciSetAudioPara; 

    mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME; 

    mciSetAudioPara.dwValue = atoi(m_Volume.GetBuffer(m_Volume.GetLength())); //here is the value of the Volume(0--1000) 

    mciSendCommand(m_wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara); 









