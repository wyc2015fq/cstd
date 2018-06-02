
#include <winsock.h>
#include <process.h>
#include "cstd.h"
#include "Eagleeye2_RMVA2xxSC_Viewer.h"

//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")

//Recv image data thread
void  RecvTrgImagedata(LPVOID pParam);  //触发模式下与相机进行数据交换的网络线程函数声明
void  RecvConImagedata(LPVOID pParam);  //全分辨率连续模式下与相机进行数据交换的网络线程函数声明
void  RecvHfconImagedata(LPVOID pParam);  //高祯率模式下与相机进行数据交换的网络线程函数声明
void  RecvHfBinconImagedata(LPVOID pParam);//高祯率模式下与相机进行数据交换的网络线程函数声明

//global variables
UINT      uiJpgDataLen;        //接受到的jpg图像数据的长度
BUFFNODE  listJpgBuff[NUM_BUFF];    //装载JPG图像数据的环形缓冲数组
LONG      PrevBuff;          //环形缓冲的前一次使用索引
LONG      CurrBuff;          //环形缓冲的当前使用索引
volatile BYTE  TargetIPAdd1, TargetIPAdd2; //
volatile BYTE  TargetIPAdd3, TargetIPAdd4; //维护当前目标相机IP地址的4个变量
char      cRcvBuf[4096];         //与目标相机进行命令交互的接受缓冲区
char      cmdbuf[1024 * 5];      //与目标相机进行命令交互的发送缓冲区
volatile unsigned char  pImgBuf8[IMGLENGTH * 3];
EagleEye2ImgInfStruct   EagleEye2_ImgInf = {
  {'\0'},      //ui8LocusInf[101]
  {0},      //ui8TimeInf[7]
  0,        //ui8WayNum
  0,        //ui8TrgMode
  0.0        //fSpeed
};;


/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc construction/destruction

void CEagleeye2_ViewerDoc(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  int nBuff = 0;
  //一些通用控制变量的初始化
  ev->m_bIsTrgMode  = FALSE;    //是否触发模式
  ev->m_bIsFconMode  = FALSE;    //是否全分辨率连续模式
  ev->m_bIsHFRMode  = FALSE;    //是否高祯率模式
  ev->m_bIsHFRBinMode = FALSE;    //是否高祯率高灵敏度模式
  ev->m_bIsEnsureTgtIP = FALSE;    //是否确认目标相机的IP地址
  ev->m_bRelayIsOpen = TRUE;      //相机继电器是否断开
  ev->m_bLUTEnable = TRUE;      //相机是否使能图像的LUT映射变换
  ev->m_bAutoTrgEnable = FALSE;    //切换到触发模式后是否允许自动触发抓取一祯图像
  ev->m_bFirstShow = TRUE ;      //浏览程序是否为初次显示，用来维护视图的刷新
  ev->m_bIsDataFromCamera = FALSE;  //标志当前视图显示的图像是否来自相机，本浏览程序也可打开硬盘中的静态图像
  ev->m_bIsJpeg = FALSE;        //标志当前视图显示的图像是否是JPG格式
  ev->m_bIsHighFR = FALSE;      //标志当前需要显示的图像是否来自高祯率模式，以此区别于其它两种模式，用来在OnDraw函数中进行显示控制
  ev->m_bIsRGBArray = TRUE;      //相机采集的图像格式是否为RGB格式
  ev->m_bIsYUVArray = FALSE;      //相机采集的图像格式是否为YUV格式
  ev->m_bIsBAYERArray = FALSE;    //相机采集的图像格式是否为BAYER格式
  ev->m_bTrgShutterIsManual = TRUE;  //触发模式快门速度是否是手动模式

  //初始化环形缓冲区
  for (nBuff = 0; nBuff < NUM_BUFF; nBuff++) {
    listJpgBuff[nBuff].sizeBuff = 0;
    listJpgBuff[nBuff].bIsLocked = FALSE;
  }

  CurrBuff = 0;
  PrevBuff = -1;
}

#define AfxMessageBox printf
///////////////////////////////////////////////////////////////////////////////
//              Sub Threads
///////////////////////////////////////////////////////////////////////////////
void RecvTrgImagedata(LPVOID pParam)
{
  //监听DSP图像数据的线程
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockTrgClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  static char    strFileNamebak[256] = "\0";
  static int      iIndex = 0;
  //保存抓拍图像
  char strFileName[256], strDirectory[256];
  char strFolderName[256];
  char tmpStr[20];

  //获取目标相机的IP地址
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
    return;
  }

  sockTrgClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockTrgClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(TRGIMGPORT);

  //连接目标相机
  if (connect(sockTrgClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //开始接受相机上传图像数据
  while (1) {
    //接受JPG码流长度
    i = 0;

    while (i < 4) {
      //CFileFind   f;
      cnt = recv(sockTrgClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }

      //如果相机发送回0xffff则表示结束本次通讯
      if (recvdatalen == 0x00) {
        goto leave;
      }
    }

    //接受抓拍信息
    i = 0;

    while (i < IMGINFLEN) {
      cnt = recv(sockTrgClient, (char*)&EagleEye2_ImgInf + i, IMGINFLEN - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    //接受图像数据
    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockTrgClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;
    //通知视图进行显示
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);

    //地点和时间
    if (strlen(EagleEye2_ImgInf.cLocusInf) > 0) {
      strcat(strFileName, EagleEye2_ImgInf.cLocusInf);

      if (EagleEye2_ImgInf.byTimeInf[0] > 9) {
        strcat(strFileName , "_20");
        strcat(strFolderName , "20");
      }
      else {
        strcat(strFileName , "_200");
        strcat(strFolderName , "200");
      }
    }
    else {
      if (EagleEye2_ImgInf.byTimeInf[0] > 9) {
        strcat(strFileName , "20");
        strcat(strFolderName , "20");
      }
      else {
        strcat(strFileName , "200");
        strcat(strFolderName , "200");
      }
    }

    strcat(strFileName , itoa(EagleEye2_ImgInf.byTimeInf[0], tmpStr, 10));
    strcat(strFolderName , itoa(EagleEye2_ImgInf.byTimeInf[0], tmpStr, 10));

    if (EagleEye2_ImgInf.byTimeInf[1] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[2] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[4] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[5] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[5], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[5], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[6] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[6], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[6], tmpStr, 10));
    }

    //车道号
    strcat(strFileName , "_");
    strcat(strFileName , itoa(EagleEye2_ImgInf.byWayNum, tmpStr, 10));
    strcat(strFileName , "车道");

    //祯索引
    if (!strcmp(strFileName, strFileNamebak)) {
      //备份文件名
      strcpy(strFileNamebak , strFileName);
      //增加索引项
      strcat(strFileName , "_");
      strcat(strFileName , itoa(++iIndex, tmpStr, 10));
    }
    else {
      //备份文件名
      strcpy(strFileNamebak , strFileName);
      iIndex = 0;
    }

    //触发类型
    switch (EagleEye2_ImgInf.byTrgMode) {
    case 0://软触发
      strcat(strFileName , "_软触发");
      break;

    case 1://单线圈
      strcat(strFileName , "_单线圈触发");
      break;

    case 2://双线圈
      strcat(strFileName , "_双线圈触发车速为");

      if (EagleEye2_ImgInf.fSpeed - (int)EagleEye2_ImgInf.fSpeed < 0.0001) {
        strcat(strFileName , itoa((int)EagleEye2_ImgInf.fSpeed, tmpStr, 10));
      }
      else {
        int tttt;
        strcat(strFileName , _gcvt(EagleEye2_ImgInf.fSpeed, 5, tmpStr));
        tttt = strlen(strFileName);

        if (strFileName[(strlen(strFileName) - 1)] == '.') {
          strcat(strFileName , "00");
        }
      }

      strcat(strFileName , "公里");
      break;

    case 3://专用硬触发
      strcat(strFileName , "_专用硬触发");
      break;

    default:
      break;
    }

    //Creat Directory
    strcpy(strDirectory, "D:\\Picture\\");
    CreateDirectory(strDirectory, NULL);
    strcat(strDirectory, strFolderName);
    strcat(strDirectory, "\\");
    CreateDirectory(strDirectory, NULL);
    //end Creat Directory
    strcat(strDirectory , strFileName);
    strcat(strDirectory , ".jpg");
    imfree(pDoc->m_fPic1);
    pDoc->m_fPic1 = imread2_mem((BYTE*)listJpgBuff[PrevBuff].pBuff, listJpgBuff[PrevBuff].sizeBuff, 0);
    imwrite2(strDirectory, pDoc->m_fPic1);
  }

leave:
  //清理环境，退出线程
  closesocket(sockTrgClient);
  WSACleanup();
}

void RecvConImagedata(LPVOID pParam)
{
  //监听DSP图像数据的线程
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockFconClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //获取目标相机的IP地址
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
    return;
  }

  sockFconClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockFconClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(FCONIMGPORT);

  //连接目标相机
  if (connect(sockFconClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //开始接受相机上传图像数据
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockFconClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockFconClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //在全分辨率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
    if (!pDoc->m_bIsFconMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockFconClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockFconClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //通知视图进行显示
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //清理环境，退出线程
  closesocket(sockFconClient);
  WSACleanup();
}

void RecvHfconImagedata(LPVOID pParam)
{
  //监听DSP图像数据的线程
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockHfrClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //获取目标相机的IP地址
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
    return;
  }

  sockHfrClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockHfrClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(HFRIMGPORT);

  //连接目标相机
  if (connect(sockHfrClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //开始接受相机上传图像数据
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockHfrClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockHfrClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //在高祯率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
    if (!pDoc->m_bIsHFRMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockHfrClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockHfrClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //通知视图进行显示
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //清理环境，退出线程
  closesocket(sockHfrClient);
  WSACleanup();
}

void RecvHfBinconImagedata(LPVOID pParam)
{
  //监听DSP图像数据的线程
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockHfrBinClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //获取目标相机的IP地址
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
    return;
  }

  sockHfrBinClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockHfrBinClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(HFRBINIMGPORT);

  //连接目标相机
  if (connect(sockHfrBinClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //开始接受相机上传图像数据
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockHfrBinClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockHfrBinClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //在高祯率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
    if (!pDoc->m_bIsHFRBinMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockHfrBinClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockHfrBinClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //通知视图进行显示
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //清理环境，退出线程
  closesocket(sockHfrBinClient);
  WSACleanup();
}

//////////////////////////////////////////////////////////////////////////
//向目标相机发送命令
BOOL SendCMDtoDSP(Eagleeye2_RMVA2xxSC_Viewer* ev, char* cmdbuf)
{
  char*        buf = cmdbuf;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockCMDClient;
  struct sockaddr_in  addr;
  UINT        i, cnt, tmp;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  u_long        bNonblock = 1;
  UINT        datalen, recvdatalen;

  //建立使用TCP协议的向目标相机发送指令的套接口对象
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("无法初始化发送命令套接口对象");
    return FALSE;
  }

  sockCMDClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockCMDClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(COMMANDPORT);

  //连接目标相机
  if (connect(sockCMDClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //发送命令ID和参数的长度(字节数)
  if (send(sockCMDClient, buf, 6, 0) < 0) {
    goto leave;
  }

  //发送命令参数
  if (send(sockCMDClient, buf + 6, *(UINT*)(buf + 2), 0) < 0) {
    goto leave;
  }

  //若需相机应答，接受相机回应
  if (buf[1]) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockCMDClient, (char*)&datalen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto  leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = datalen;
      }
    }

    i = 0;

    while (i < recvdatalen) {
      cnt = recv(sockCMDClient, cRcvBuf + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto  leave;
      }

      i += cnt;
    }
  }

  //关闭套接口对象并返回真值
  closesocket(sockCMDClient);
  WSACleanup();
  return TRUE;

  //遇到错误，关闭套接口对象并返回
leave:
  closesocket(sockCMDClient);
  WSACleanup();
  ev->m_bIsEnsureTgtIP = FALSE;
  return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//保存目标相机的IP地址，同时发送命令使目标相机启动全分辨率连续模式下的数据上传
void OnConfigTargetip(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  UINT    tmp1 = 0, tmp2 = 0;

  {
    TargetIPAdd1 = ev->m_byIPAdd1;
    TargetIPAdd2 = ev->m_byIPAdd2;
    TargetIPAdd3 = ev->m_byIPAdd3;
    TargetIPAdd4 = ev->m_byIPAdd4;
    //复位三种模式的标志和确认目标IP标志
    ev->m_bIsTrgMode  = FALSE;
    ev->m_bIsFconMode  = FALSE;
    ev->m_bIsHFRMode  = FALSE;
    ev->m_bIsHFRBinMode = FALSE;
    ev->m_bIsEnsureTgtIP = FALSE;
    //检测触发模式服务器是否还没有退出连接，若没有则断开
    cmdbuf[0] = 0x23;
    cmdbuf[1] = 0x00;//需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("连接相机失败!");
      return;
    }

    //查询目标相机的工作模式，若连接成功启动相应的客户端线程
    cmdbuf[0] = 0x03;
    cmdbuf[1] = 0x01;//需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (SendCMDtoDSP(ev, cmdbuf)) {
      ev->m_bIsEnsureTgtIP = TRUE;
      //获取相关参数配置界面
      //配置可编程I/O的口方向和输出值以更新界面
      tmp1 = *(UINT*)(cRcvBuf + 16);
      tmp2 = *(UINT*)(cRcvBuf + 20);

      //port1方向
      if ((tmp1 & 0x001) == 0) {
        //输入
        ev->m_bIsPort1IN = TRUE;
        ev->m_pioIndicator.pio1 = 0;
      }
      else {
        //输出
        ev->m_bIsPort1IN = FALSE;
        ev->m_pioIndicator.pio1 = 1;

        if ((tmp2 & 0x001) == 0) {
          ev->m_bPort1OUTValue = FALSE;
        }
        else {
          ev->m_bPort1OUTValue = TRUE;
        }
      }

      //port2方向
      if ((tmp1 & 0x002) == 0) {
        //输入
        ev->m_bIsPort2IN = TRUE;
        ev->m_pioIndicator.pio2 = 0;
      }
      else {
        //输出
        ev->m_bIsPort2IN = FALSE;
        ev->m_pioIndicator.pio2 = 1;

        if ((tmp2 & 0x002) == 0) {
          ev->m_bPort2OUTValue = FALSE;
        }
        else {
          ev->m_bPort2OUTValue = TRUE;
        }
      }

      //port3方向
      if ((tmp1 & 0x004) == 0) {
        //输入
        ev->m_bIsPort3IN = TRUE;
        ev->m_pioIndicator.pio3 = 0;
      }
      else {
        //输出
        ev->m_bIsPort3IN = FALSE;
        ev->m_pioIndicator.pio3 = 1;

        if ((tmp2 & 0x004) == 0) {
          ev->m_bPort3OUTValue = FALSE;
        }
        else {
          ev->m_bPort3OUTValue = TRUE;
        }
      }

      //port4方向
      if ((tmp1 & 0x008) == 0) {
        //输入
        ev->m_bIsPort4IN = TRUE;
        ev->m_pioIndicator.pio4 = 0;
      }
      else {
        //输出
        ev->m_bIsPort4IN = FALSE;
        ev->m_pioIndicator.pio4 = 1;

        if ((tmp2 & 0x008) == 0) {
          ev->m_bPort4OUTValue = FALSE;
        }
        else {
          ev->m_bPort4OUTValue = TRUE;
        }
      }

      //port5方向
      if ((tmp1 & 0x010) == 0) {
        //输入
        ev->m_bIsPort5IN = TRUE;
        ev->m_pioIndicator.pio5 = 0;
      }
      else {
        //输出
        ev->m_bIsPort5IN = FALSE;
        ev->m_pioIndicator.pio5 = 1;

        if ((tmp2 & 0x010) == 0) {
          ev->m_bPort5OUTValue = FALSE;
        }
        else {
          ev->m_bPort5OUTValue = TRUE;
        }
      }

      //port6方向
      if ((tmp1 & 0x020) == 0) {
        //输入
        ev->m_bIsPort6IN = TRUE;
        ev->m_pioIndicator.pio6 = 0;
      }
      else {
        //输出
        ev->m_bIsPort6IN = FALSE;
        ev->m_pioIndicator.pio6 = 1;

        if ((tmp2 & 0x020) == 0) {
          ev->m_bPort6OUTValue = FALSE;
        }
        else {
          ev->m_bPort6OUTValue = TRUE;
        }
      }

      //port7方向
      if ((tmp1 & 0x040) == 0) {
        //输入
        ev->m_bIsPort7IN = TRUE;
        ev->m_pioIndicator.pio7 = 0;
      }
      else {
        //输出
        ev->m_bIsPort7IN = FALSE;
        ev->m_pioIndicator.pio7 = 1;

        if ((tmp2 & 0x040) == 0) {
          ev->m_bPort7OUTValue = FALSE;
        }
        else {
          ev->m_bPort7OUTValue = TRUE;
        }
      }

      //port8方向
      if ((tmp1 & 0x080) == 0) {
        //输入
        ev->m_bIsPort8IN = TRUE;
        ev->m_pioIndicator.pio8 = 0;
      }
      else {
        //输出
        ev->m_bIsPort8IN = FALSE;
        ev->m_pioIndicator.pio8 = 1;

        if ((tmp2 & 0x080) == 0) {
          ev->m_bPort8OUTValue = FALSE;
        }
        else {
          ev->m_bPort8OUTValue = TRUE;
        }
      }

      //port9方向
      if ((tmp1 & 0x100) == 0) {
        //输入
        ev->m_bIsPort9IN = TRUE;
        ev->m_pioIndicator.pio9 = 0;
      }
      else {
        //输出
        ev->m_bIsPort9IN = FALSE;
        ev->m_pioIndicator.pio9 = 1;

        if ((tmp2 & 0x100) == 0) {
          ev->m_bPort9OUTValue = FALSE;
        }
        else {
          ev->m_bPort9OUTValue = TRUE;
        }
      }

      //port10方向
      if ((tmp1 & 0x200) == 0) {
        //输入
        ev->m_bIsPort10IN = TRUE;
        ev->m_pioIndicator.pio10 = 0;
      }
      else {
        //输出
        ev->m_bIsPort10IN = FALSE;
        ev->m_pioIndicator.pio10 = 1;

        if ((tmp2 & 0x200) == 0) {
          ev->m_bPort10OUTValue = FALSE;
        }
        else {
          ev->m_bPort10OUTValue = TRUE;
        }
      }

      //配置图像采集格式
      switch (cRcvBuf[15]) {
      case RGB_ARRAY:
        ev->m_bIsRGBArray = TRUE;
        ev->m_bIsYUVArray = FALSE;
        ev->m_bIsBAYERArray = FALSE;
        break;

      case YUV_ARRAY:
        ev->m_bIsRGBArray = FALSE;
        ev->m_bIsYUVArray = TRUE;
        ev->m_bIsBAYERArray = FALSE;
        break;

      case BAYER_ARRAY:
        ev->m_bIsRGBArray = FALSE;
        ev->m_bIsYUVArray = FALSE;
        ev->m_bIsBAYERArray = TRUE;
        break;

      default:
        break;
      }

      //配置运行模式
      switch (cRcvBuf[14]) {
      case TrgMode:
        ev->m_bIsTrgMode = TRUE;
        _beginthread(RecvTrgImagedata, 0, (LPVOID)ev);
        break;

      case FconMode:
        ev->m_bIsFconMode = TRUE;
        _beginthread(RecvConImagedata, 0, (LPVOID)ev);
        break;

      case HfrMode:
        ev->m_bIsHFRMode = TRUE;
        _beginthread(RecvHfconImagedata, 0, (LPVOID)ev);
        break;

      case HfrBinMode:
        ev->m_bIsHFRBinMode = TRUE;
        _beginthread(RecvHfBinconImagedata, 0, (LPVOID)ev);
        break;

      default:
        break;
      }

      //配置白平衡环境选项的界面显示
      switch (cRcvBuf[13]) {
      case DAYLIGHT:
        ev->m_uiWBType = 0;
        break;

      case FLUORESCENCE:
        ev->m_uiWBType = 1;
        break;

      case WITHOUTWB:
        ev->m_uiWBType = 2;
        break;

      default:
        break;
      }

      //配置闪光灯同步输出状态
      if (cRcvBuf[8]) {
        ev->m_bIsFlashSynEnable = TRUE;
      }
      else {
        ev->m_bIsFlashSynEnable = FALSE;
      }

      //配置是否允许模式切换到触发模式后是否允许自动触发抓取一祯图像
      if (cRcvBuf[9]) {
        ev->m_bAutoTrgEnable = TRUE;
      }
      else {
        ev->m_bAutoTrgEnable = FALSE;
      }

      //配置是否使能图像LUT映射变换
      if (cRcvBuf[10]) {
        ev->m_bLUTEnable = TRUE;
      }
      else {
        ev->m_bLUTEnable = FALSE;
      }

      //配置继电器是否闭合
      if (cRcvBuf[11]) {
        ev->m_bRelayIsOpen = FALSE;
      }
      else {
        ev->m_bRelayIsOpen = TRUE;
      }

      //配置是否使能看门狗和设置看门狗定时器时间界面显示值
      if (cRcvBuf[12]) {
        ev->m_bIsWDTEnable = TRUE;
      }
      else {
        ev->m_bIsWDTEnable = FALSE;
      }

      ev->m_iWDT = *(unsigned int*)(cRcvBuf + 4);
      //设置传感器输出信号增益界面显示值
      ev->m_fAfeVGAGain = *(float*)(cRcvBuf);
      //////////////////////////////////////////////////////////////////////////
      //获得主框架类的窗口对象,通知主框架类更新状态条
      //pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
      //m_hMainFrameWnd = pMain->GetSafeHwnd();
      //PostMessage(m_hMainFrameWnd, WM_SWICHNEWCAMERA, 0, 0);
    }
    else {
      ev->m_bIsEnsureTgtIP = FALSE;
      AfxMessageBox("连接目标相机失败，请核对目标相机的IP地址");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//            相机触发模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到触发模式
void OnOmcTrigger(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bIsTrgMode  = TRUE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = FALSE;
  //发送命令切换相机到触发模式
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  cmdbuf[6] = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
  else {
    _beginthread(RecvTrgImagedata, 0, (LPVOID)ev);
  }
}
//配置相机在触发模式下的工作状态
void OnTrgmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    int cmdlen = 0;
    //触发模式下的相机配置
    cmdbuf[0] = 0x04;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    //抓拍地点
    cmdlen += strlen(ev->m_strLocation);

    if (cmdlen > 0) {
      memcpy(cmdbuf + 6, ev->m_strLocation, cmdlen);
    }

    memset(cmdbuf + 6 + cmdlen, '\0', 101 - cmdlen);
    cmdlen = 101;
    //车道号
    cmdbuf[cmdlen + 6] = ev->m_iWayIndex;
    cmdlen += 1;

    //触发方式
    if (ev->m_bIsSingalCoin) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //若为双线圈触发，线圈间距
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fInterval;
    cmdlen += 4;

    //快门增益控制
    if (ev->m_bIsManualCtrl) {
      cmdbuf[cmdlen + 6] = 0;
    }
    else {
      cmdbuf[cmdlen + 6] = 1;
    }

    cmdlen += 1;
    //手动模式下的快门设置
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_uiTrgShutterTime;
    cmdlen += 4;
    //手动模式下的增益设置
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fTrgPga;
    cmdlen += 4;
    //自动模式下的快门上限设置
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iShutLimit;
    cmdlen += 4;
    //自动模式下的增益上限设置
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fPGALimit;
    cmdlen += 4;
    //自动模式下的当前快门设置
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iCurShut;
    cmdlen += 4;
    //自动模式下的当前增益设置
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fCurPga;
    cmdlen += 4;
    //自动模式下的测光区域起始行
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iSRow;
    cmdlen += 4;
    //自动模式下的测光区域起始列
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iSCol;
    cmdlen += 4;
    //自动模式下的测光区域终止行
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iERow;
    cmdlen += 4;
    //自动模式下的测光区域终止列
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iECol;
    cmdlen += 4;
    //自动模式下的测光区域期望平均亮度
    cmdbuf[cmdlen + 6] = ev->m_byYMean;
    cmdlen += 1;

    //自动模式下是否允许自测光触发
    if (ev->m_bEnableADLI) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //自动模式下是自测光触发周期
    cmdbuf[cmdlen + 6] = ev->m_iADLIP;
    cmdlen += 1;

    //是否保存参数
    if (ev->m_bSaveParam) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //发送命令配置相机在触发模式下的曝光时间
    cmdbuf[0] = 0x04;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = cmdlen;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//软件触发抓取一祯图像
void OnOpeGrab(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //发送命令软触发抓取一祯图像
  cmdbuf[0] = 0x02;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}

//////////////////////////////////////////////////////////////////////////
//          相机全分辨连续模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到全分辨率连续模式
void OnOmcMjpeg(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = TRUE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = FALSE;

  //是否停止相机端触发模式的服务
  if (bStopTrgSvr) {
    //发送停止触发模式下服务的标志
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
      return;
    }
  }

  Sleep(200);
  //切换相机到全分辨率连续模式
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
  else {
    _beginthread(RecvConImagedata, 0, (LPVOID)ev);
  }
}

//////////////////////////////////////////////////////////////////////////
//          相机高祯率模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到高祯率模式
void OnOmcHfr(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = TRUE;
  ev->m_bIsHFRBinMode = FALSE;

  //是否停止相机端触发模式的服务
  if (bStopTrgSvr) {
    //发送停止触发模式下服务的标志
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
      return;
    }
  }

  //切换相机到高祯率模式
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x02;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
  else {
    _beginthread(RecvHfconImagedata, 0, (LPVOID)ev);
  }
}

//配置相机在高祯率模式下的工作状态
void OnHfrmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    if (ev->m_bIsShutPgaManualCtrl) {
      //设置相机在高祯率模式下为手动运行模式
      cmdbuf[0] = 0x11;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 0;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式的手动运行方式下的曝光时间
      cmdbuf[0] = 0x06;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_uiTrgShutterTime;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
    else {
      //设置相机在高祯率模式下为自动运行模式
      cmdbuf[0] = 0x11;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 1;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的信号增益自动调节的区间上限
      cmdbuf[0] = 0x13;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x04;
      *(float*)(cmdbuf + 6) = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的曝光时间自动调节的区间上限
      cmdbuf[0] = 0x12;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_bIsShutPgaAutoCtrl;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的图像平均亮度阈值
      cmdbuf[0] = 0x15;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = ev->m_byYMean;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//          相机高祯率高灵敏度模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到高祯率高灵敏度模式
void OnOmcHfrb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = TRUE;

  //是否停止相机端触发模式的服务
  if (bStopTrgSvr) {
    //发送停止触发模式下服务的标志
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
      return;
    }
  }

  //切换相机到高祯率模式
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x03;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
  else {
    _beginthread(RecvHfBinconImagedata, 0, (LPVOID)ev);
  }
}

void OnHfrbinmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    if (ev->m_bIsShutPgaManualCtrl) {
      //设置相机在高祯率模式下为手动运行模式
      cmdbuf[0] = 0x25;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 0;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式的手动运行方式下的曝光时间
      cmdbuf[0] = 0x24;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_uiTrgShutterTime;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
    else {
      //设置相机在高祯率模式下为自动运行模式
      cmdbuf[0] = 0x25;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 1;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的信号增益自动调节的区间上限
      cmdbuf[0] = 0x13;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x04;
      *(float*)(cmdbuf + 6) = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的曝光时间自动调节的区间上限
      cmdbuf[0] = 0x26;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_bIsShutPgaAutoCtrl;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置相机在高祯率模式下的图像平均亮度阈值
      cmdbuf[0] = 0x27;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = ev->m_byYMean;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//          相机的其它控制
//////////////////////////////////////////////////////////////////////////
//重新配置目标相机的IP地址，配置完毕相机重启后以新的IP地址工作
void OnConfigNewip(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  int          IPAddress;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  //char        macadd[256];

  //if(ev->DoModal()==IDOK)
  {
    tmp1 = ev->m_byIPAdd1;
    tmp2 = ev->m_byIPAdd2;
    tmp3 = ev->m_byIPAdd3;
    tmp4 = ev->m_byIPAdd4;
    IPAddress =  tmp1;
    IPAddress |=  tmp2 << 8;
    IPAddress |=  tmp3 << 16;
    IPAddress |=  tmp4 << 24;
    //设置目标相机的新IP地址
    cmdbuf[0] = 0x1a;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x04;
    *(UINT*)(cmdbuf + 6) = IPAddress;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//重新配置目标相机的MAC地址，配置完毕相机重启后以新的MAC地址工作
void OnConfigNewmac(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  char        macadd[256];
  int          i, j;

  //if(m_MACCfgDlg.DoModal()==IDOK)
  {
    //Set new MAC Address
    //macadd = m_MACCfgDlg.m_strMacAdd4 + m_MACCfgDlg.m_strMacAdd5 + m_MACCfgDlg.m_strMacAdd6;
    i = strlen(macadd);

    if (i != 6) {
      if (i == 0) {
        return;
      }

      AfxMessageBox("MAC address is invaliable!");
      return;
    }

    for (i = 0; i < 3; i++) {
      unsigned char tmp[2];

      for (j = 0; j < 2; j++) {
        if ((macadd[i * 2 + j] >= 48 && macadd[i * 2 + j] <= 57) || (macadd[i * 2 + j] >= 65 && macadd[i * 2 + j] <= 70) || (macadd[i * 2 + j] >= 97 && macadd[i * 2 + j] <= 102)) {
          if (macadd[i * 2 + j] >= 48 && macadd[i * 2 + j] <= 57) {
            tmp[j] = macadd[i * 2 + j] - 48;
          }

          if (macadd[i * 2 + j] >= 65 && macadd[i * 2 + j] <= 70) {
            tmp[j] = macadd[i * 2 + j] - 55;
          }

          if (macadd[i * 2 + j] >= 97 && macadd[i * 2 + j] <= 102) {
            tmp[j] = macadd[i * 2 + j] - 87;
          }
        }
        else {
          AfxMessageBox("MAC address is invaliable!");
          return;
        }
      }

      cmdbuf[i + 9] = (tmp[0] * 16 + tmp[1]);
    }

    //发送命令修改目标相机的MAC地址
    cmdbuf[0] = 0x1b;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x06;
    cmdbuf[6] = 0x08;
    cmdbuf[7] = 0x00;
    cmdbuf[8] = 0x28;  //MAC code for TI

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//CCD信号的增益控制(0 - 35.5dB 可调)
void OnConfigAfegainctrl(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(m_AfeConfigDlg.DoModal()==IDOK)
  {
    //设置传感器输出信号的增益
    cmdbuf[0] = 0x07;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x04;
    *(float*)(cmdbuf + 6) = ev->m_fAfeVGAGain;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//白平衡控制选项
void OnConfigWhitebalance(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    //设置目标相机白平衡校正的光照条件
    cmdbuf[0] = 0x08;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x01;
    *(UINT*)(cmdbuf + 6) = *(UINT*)(&ev->m_uiWBType);

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//可编程IO口和闪光灯同步输出使能控制
void OnConfigGioconfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    //设置目标相机十个可编程I/O的方向
    cmdbuf[0] = 0x0d;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x02;
    *(UINT*)(cmdbuf + 6) = *(UINT*)(&ev->m_pioIndicator) & 0x3ff;
    ev->m_bIsPortDirAdj = FALSE;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }

    //设置目标相机十个可编程I/O的输出电平，仅对输出口有效
    cmdbuf[0] = 0x0e;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x02;
    *(UINT*)(cmdbuf + 6) = (*(UINT*)(&ev->m_pioIndicator) & 0xffc00) >> 10;
    ev->m_bIsPortOutAdj = FALSE;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }

    //设置目标相机的闪光灯同步输出是否使能
    cmdbuf[0] = 0x0c;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x01;

    if (ev->m_bIsFlashSynEnable) {
      *(UINT*)(cmdbuf + 6) = 1; //使能
    }
    else {
      *(UINT*)(cmdbuf + 6) = 0; //禁止
    }

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//相机内部的实时时钟配置
void OnConfigRtcconfig(Eagleeye2_RMVA2xxSC_Viewer* ev, RTCConfig* rtc)
{
  //if(rtc->DoModal()==IDOK)
  {
    //设置目标相机的RTC时间
    cmdbuf[0] = 0x18;
    cmdbuf[1] = 0x00;//不需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0x07;
    cmdbuf[6] = rtc->m_iYear % 100;
    cmdbuf[7] = rtc->m_iMonth;
    cmdbuf[8] = rtc->m_iDay;
    cmdbuf[9] = rtc->m_iWeek;
    cmdbuf[10] = rtc->m_iHour;
    cmdbuf[11] = rtc->m_iMinute;
    cmdbuf[12] = rtc->m_iSecond;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }

    //读取目标相机的RTC时间
    cmdbuf[0] = 0x19;
    cmdbuf[1] = 0x01;//需要相机回传数据
    *(UINT*)(cmdbuf + 2) = 0;

    if (!SendCMDtoDSP(ev,  cmdbuf)) {
      AfxMessageBox("向相机发送命令失败");
    }
  }
}

//相机的看门狗设置
void OnConfigWdtconfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {

    if (ev->m_bIsWDTEnable) {
      //使能目标相机的看门狗
      cmdbuf[0] = 0x16;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = 0x01;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }

      //设置目标相机的看门狗定时器(1~256秒)
      cmdbuf[0] = 0x17;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = ev->m_iWDT;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
    else {
      //禁止目标相机的看门狗
      cmdbuf[0] = 0x16;
      cmdbuf[1] = 0x00;//不需要相机回传数据
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = 0x00;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("向相机发送命令失败");
      }
    }
  }
}

//控制相机抓取一祯当前图像进行自适应白平衡校正
void OnOpeAutowb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //命令相机抓取一祯当前图像进行自适应白平衡校正
  cmdbuf[0] = 0x09;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//控制相机在其它两种模式切换到触发模式后是否自动触发抓取一祯图像
void OnOpeAutotrgEnable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //使能切换后自动触发功能
  ev->m_bAutoTrgEnable = TRUE;
  cmdbuf[0] = 0x1f;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
void OnOpeAutotrgDisable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //禁止切换后自动触发功能
  ev->m_bAutoTrgEnable = FALSE;
  cmdbuf[0] = 0x1f;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//是否使能图像LUT变换功能
void OnOpeLutEnable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //使能LUT变换功能
  ev->m_bLUTEnable = TRUE;
  cmdbuf[0] = 0x0a;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
void OnOpeLutDisable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //禁止LUT变换功能
  ev->m_bLUTEnable = FALSE;
  cmdbuf[0] = 0x0a;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x02;
  cmdbuf[6] = 0x00;
  cmdbuf[7] = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//继电器控制
void OnOpeRelayOpen(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //断开继电器
  ev->m_bRelayIsOpen = TRUE;
  cmdbuf[0] = 0x1d;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
void OnOpeRelayClose(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //闭合继电器
  ev->m_bRelayIsOpen = FALSE;
  cmdbuf[0] = 0x1d;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//设置相机图像采集格式为RGB阵列
void OnOpeImgformatRgb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //设置相机图像采集格式为RGB阵列
  ev->m_bIsRGBArray = TRUE;
  ev->m_bIsYUVArray = FALSE;
  ev->m_bIsBAYERArray = FALSE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//设置相机图像采集格式为YUV阵列
void OnOpeImgformatYuv(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //设置相机图像采集格式为YUV阵列
  ev->m_bIsRGBArray = FALSE;
  ev->m_bIsYUVArray = TRUE;
  ev->m_bIsBAYERArray = FALSE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//设置相机图像采集格式为BAYER阵列
void OnOpeImgformatBayer(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //设置相机图像采集格式为YUV阵列
  ev->m_bIsRGBArray = FALSE;
  ev->m_bIsYUVArray = FALSE;
  ev->m_bIsBAYERArray = TRUE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 2;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//设置触发模式的快门速度的手动模式
void OnOpeTrgshutterctrlManual(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bTrgShutterIsManual = TRUE;
  cmdbuf[0] = 0x22;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//设置触发模式的快门速度的自动模式
void OnOpeTrgshutterctrlAuto(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bTrgShutterIsManual = FALSE;
  cmdbuf[0] = 0x22;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//复位相机
void OnOpeResetsys(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //远程重启目标相机
  cmdbuf[0] = 0x00;
  cmdbuf[1] = 0x00;//不需要相机回传数据
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("向相机发送命令失败");
  }
}
//远程更新用户程序
void OnOpeRemoteupdatedspprrom()
{
  char CmdLine[] = "EagleeyeDSPRemoteUpdate";
  UINT RES = WinExec(CmdLine, SW_SHOW);
}
