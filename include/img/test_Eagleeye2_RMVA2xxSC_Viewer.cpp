
#include <winsock.h>
#include <process.h>
#include <stdio.h>
#include "chalg.h"
#include "macro.h"
#include "utime.h"
#include "./三维跟踪/NewTrack.h"
#include "Eagleeye2_RMVA2xxSC_Viewer.h"
#include "sftime.h"
#include "inifile.h"
#include "findfile.h"

//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")


int asdf[] = {
#include "./Eagleeye2_RMVA2xxSC_Viewer/asdf.txt"
};

BUFFNODE  listJpgBuff[NUM_BUFF];
LONG      CurrBuff;

BYTE        TargetIPAdd1,TargetIPAdd2,TargetIPAdd3,TargetIPAdd4;

#define MAKEIP(tmp1,tmp2,tmp3,tmp4)  (unsigned int)((tmp1) | ((tmp2) << 8) | ((tmp3) << 16) | ((tmp4) << 24))

#define TargetIP  MAKEIP(192,168,1,218)

//////////////////////////////////////////////////////////////////////////
//向目标相机发送命令
BOOL SendCMD(unsigned int CmdId, unsigned int ReplyFlag,
             unsigned int cmdlen, const char* cmdbuf,
             char* cRcvBuf, unsigned int* precvdatalen)
{
  char        cmdbuf0[6];
  WORD        wVersionRequested;
  WSADATA     wsaData;
  SOCKET      sockCMDClient;
  struct sockaddr_in  addr;
  UINT        i,cnt;
  u_long      bNonblock = 1;
  UINT        datalen,recvdatalen;
  BOOL ret = FALSE;

  CC_FUNCNAME("SendCMD");
  
  __CC_BEGIN__;

  //建立使用TCP协议的向目标相机发送指令的套接口对象
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);
  if (i != 0){
    CC_ERROR(CC_StsError, "无法初始化套接口对象");
  }
  sockCMDClient = socket( AF_INET, SOCK_STREAM, 0 );
  if( sockCMDClient < 0){
    CC_ERROR(CC_StsError, "socket");
  }
  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = TargetIP;
  addr.sin_port    = htons(COMMANDPORT);
  //连接目标相机
  if ( connect( sockCMDClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
    CC_ERROR(CC_StsError, "connect");
  }
  cmdbuf0[0] = CmdId;
  cmdbuf0[1] = ReplyFlag;//不需要相机回传数据
  *(UINT*)(cmdbuf0+2) = cmdlen;
  //发送命令ID和参数的长度(字节数)
  if( send( sockCMDClient, cmdbuf0, 6, 0 ) < 0 ){
    CC_ERROR(CC_StsError, "send");
  }
  //发送命令参数
  if (cmdbuf && cmdlen) {
    if( send( sockCMDClient, cmdbuf, cmdlen, 0 ) < 0 ){
      CC_ERROR(CC_StsError, "send");
    }
  }
  //若需相机应答，接受相机回应
  if( ReplyFlag ) {
    i = 0;
    while( i < 4) {
      cnt = recv( sockCMDClient, (char*)&datalen+i, 4-i, 0 );
      if( cnt <=0) {
        CC_ERROR(CC_StsError, "recv");
      }
      i += cnt;
      if( i == 4) {
        recvdatalen = datalen;
      }
    }
    i = 0;
    while( i < recvdatalen) {
      cnt = recv( sockCMDClient, cRcvBuf+i, recvdatalen-i, 0 );
      if( cnt <=0) {
        CC_ERROR(CC_StsError, "recv");
      }
      i += cnt;
    }
    if (precvdatalen) {
      *precvdatalen = recvdatalen;
    }
  }
  ret = TRUE;

  //遇到错误，关闭套接口对象并返回
  //关闭套接口对象并返回真值
  __CC_END__;
  closesocket(sockCMDClient);
  WSACleanup();
  return ret;
}

static EagleEye2ImgInfStruct	EagleEye2_ImgInf =
{
  {'\0'},			//ui8LocusInf[101]
  {0},			//ui8TimeInf[7]
  0,				//ui8WayNum
  0,				//ui8TrgMode
  0.0				//fSpeed
};;

//#define RUN_MODE_OMC_TRIGGER   0x00 // 触发模式
//#define RUN_MODE_OMC_MJPEG     0x01 // 全分辨率连续模式
//#define RUN_MODE_OMC_HFR       0x02 // 高祯率模式
//#define RUN_MODE_OMC_HFRB      0x03 // 高祯率高灵敏度模式
// 切换相机到以上四中模式

typedef struct EAGLEEYE2CONFIG {
  int SensorRunMode; // 工作模式
  int ImgAcquisition_Format; // 图像采集格式
  int WhiteBalance_environment; // 白平衡环境选项的界面显示
  int m_bIsFlashSynEnable; // 闪光灯同步输出状态
} EAGLEEYE2CONFIG;
/*
//保存目标相机的IP地址，同时发送命令使目标相机启动全分辨率连续模式下的数据上传
void OnConfigTargetip(EAGLEEYE2CONFIG* ev) 
{
  UINT    tmp1 = 0, tmp2 = 0;
  {
    //检测触发模式服务器是否还没有退出连接，若没有则断开
    CMD_CHECK_TRIGGER_MODE();
    if(!SendCMDtoDSP(ev, cmdbuf)){
      AfxMessageBox("连接相机失败!");
      return;
    }
    //查询目标相机的工作模式，若连接成功启动相应的客户端线程

    CMD_GET_RUN_MODE();
    {
      //获取相关参数配置界面
      //配置可编程I/O的口方向和输出值以更新界面
      tmp1 = *(UINT*)(cRcvBuf+16);
      tmp2 = *(UINT*)(cRcvBuf+20);
      //port1方向
      if( (tmp1&0x001) == 0 ){
        //输入
        ev->m_bIsPort1IN = TRUE;
        ev->m_pioIndicator.pio1 = 0;
      }
      else{
        //输出
        ev->m_bIsPort1IN = FALSE;
        ev->m_pioIndicator.pio1 = 1;
        if( (tmp2&0x001) == 0 ){
          ev->m_bPort1OUTValue = FALSE;
        }
        else{
          ev->m_bPort1OUTValue = TRUE;
        }
      }
      //port2方向
      if( (tmp1&0x002) == 0 ){
        //输入
        ev->m_bIsPort2IN = TRUE;
        ev->m_pioIndicator.pio2 = 0;
      }
      else{
        //输出
        ev->m_bIsPort2IN = FALSE;
        ev->m_pioIndicator.pio2 = 1;
        if( (tmp2&0x002) == 0 ){
          ev->m_bPort2OUTValue = FALSE;
        }
        else{
          ev->m_bPort2OUTValue = TRUE;
        }
      }
      //port3方向
      if( (tmp1&0x004) == 0 ){
        //输入
        ev->m_bIsPort3IN = TRUE;
        ev->m_pioIndicator.pio3 = 0;
      }
      else{
        //输出
        ev->m_bIsPort3IN = FALSE;
        ev->m_pioIndicator.pio3 = 1;
        if( (tmp2&0x004) == 0 ){
          ev->m_bPort3OUTValue = FALSE;
        }
        else{
          ev->m_bPort3OUTValue = TRUE;
        }
      }
      //port4方向
      if( (tmp1&0x008) == 0 ){
        //输入
        ev->m_bIsPort4IN = TRUE;
        ev->m_pioIndicator.pio4 = 0;
      }
      else{
        //输出
        ev->m_bIsPort4IN = FALSE;
        ev->m_pioIndicator.pio4 = 1;
        if( (tmp2&0x008) == 0 ){
          ev->m_bPort4OUTValue = FALSE;
        }
        else{
          ev->m_bPort4OUTValue = TRUE;
        }
      }
      //port5方向
      if( (tmp1&0x010) == 0 ){
        //输入
        ev->m_bIsPort5IN = TRUE;
        ev->m_pioIndicator.pio5 = 0;
      }
      else{
        //输出
        ev->m_bIsPort5IN = FALSE;
        ev->m_pioIndicator.pio5 = 1;
        if( (tmp2&0x010) == 0 ){
          ev->m_bPort5OUTValue = FALSE;
        }
        else{
          ev->m_bPort5OUTValue = TRUE;
        }
      }
      //port6方向
      if( (tmp1&0x020) == 0 ){
        //输入
        ev->m_bIsPort6IN = TRUE;
        ev->m_pioIndicator.pio6 = 0;
      }
      else{
        //输出
        ev->m_bIsPort6IN = FALSE;
        ev->m_pioIndicator.pio6 = 1;
        if( (tmp2&0x020) == 0 ){
          ev->m_bPort6OUTValue = FALSE;
        }
        else{
          ev->m_bPort6OUTValue = TRUE;
        }
      }
      //port7方向
      if( (tmp1&0x040) == 0 ){
        //输入
        ev->m_bIsPort7IN = TRUE;
        ev->m_pioIndicator.pio7 = 0;
      }
      else{
        //输出
        ev->m_bIsPort7IN = FALSE;
        ev->m_pioIndicator.pio7 = 1;
        if( (tmp2&0x040) == 0 ){
          ev->m_bPort7OUTValue = FALSE;
        }
        else{
          ev->m_bPort7OUTValue = TRUE;
        }
      }
      //port8方向
      if( (tmp1&0x080) == 0 ){
        //输入
        ev->m_bIsPort8IN = TRUE;
        ev->m_pioIndicator.pio8 = 0;
      }
      else{
        //输出
        ev->m_bIsPort8IN = FALSE;
        ev->m_pioIndicator.pio8 = 1;
        if( (tmp2&0x080) == 0 ){
          ev->m_bPort8OUTValue = FALSE;
        }
        else{
          ev->m_bPort8OUTValue = TRUE;
        }
      }
      //port9方向
      if( (tmp1&0x100) == 0 ){
        //输入
        ev->m_bIsPort9IN = TRUE;
        ev->m_pioIndicator.pio9 = 0;
      }
      else{
        //输出
        ev->m_bIsPort9IN = FALSE;
        ev->m_pioIndicator.pio9 = 1;
        if( (tmp2&0x100) == 0 ){
          ev->m_bPort9OUTValue = FALSE;
        }
        else{
          ev->m_bPort9OUTValue = TRUE;
        }
      }
      //port10方向
      if( (tmp1&0x200) == 0 ){
        //输入
        ev->m_bIsPort10IN = TRUE;
        ev->m_pioIndicator.pio10 = 0;
      }
      else{
        //输出
        ev->m_bIsPort10IN = FALSE;
        ev->m_pioIndicator.pio10 = 1;
        if( (tmp2&0x200) == 0 ){
          ev->m_bPort10OUTValue = FALSE;
        }
        else{
          ev->m_bPort10OUTValue = TRUE;
        }
      }
      //配置图像采集格式
      switch( cRcvBuf[15] ){
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
      switch( cRcvBuf[14] ) {
        case TrgMode:
          ev->m_bIsTrgMode = TRUE;
          _beginthread(RecvTrgImagedata,0,(LPVOID)ev);
          break;
        case FconMode:
          ev->m_bIsFconMode = TRUE;
          _beginthread(RecvConImagedata,0,(LPVOID)ev);
          break;
        case HfrMode:
          ev->m_bIsHFRMode = TRUE;
          _beginthread(RecvHfconImagedata,0,(LPVOID)ev);
          break;
        case HfrBinMode:
          ev->m_bIsHFRBinMode = TRUE;
          _beginthread(RecvHfBinconImagedata,0,(LPVOID)ev);
          break;
        default:
          break;
      }
      //配置白平衡环境选项的界面显示
      switch( cRcvBuf[13] ) {
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
      if( cRcvBuf[8] ){
        ev->m_bIsFlashSynEnable = TRUE;
      }
      else{
        ev->m_bIsFlashSynEnable = FALSE;
      }
      //配置是否允许模式切换到触发模式后是否允许自动触发抓取一祯图像
      if( cRcvBuf[9] ){
        ev->m_bAutoTrgEnable = TRUE;
      }
      else{
        ev->m_bAutoTrgEnable = FALSE;
      }
      //配置是否使能图像LUT映射变换
      if( cRcvBuf[10] ){
        ev->m_bLUTEnable = TRUE;
      }
      else{
        ev->m_bLUTEnable = FALSE;
      }
      //配置继电器是否闭合
      if( cRcvBuf[11] ){
        ev->m_bRelayIsOpen = FALSE;
      }
      else{
        ev->m_bRelayIsOpen = TRUE;
      }
      //配置是否使能看门狗和设置看门狗定时器时间界面显示值
      if( cRcvBuf[12] ){
        ev->m_bIsWDTEnable = TRUE;
      }
      else{
        ev->m_bIsWDTEnable = FALSE;
      }
      ev->m_iWDT = *(unsigned int*)(cRcvBuf+4);
      //设置传感器输出信号增益界面显示值
      ev->m_fAfeVGAGain = *(float*)(cRcvBuf);
      //////////////////////////////////////////////////////////////////////////
      //获得主框架类的窗口对象,通知主框架类更新状态条
      //pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
      //m_hMainFrameWnd = pMain->GetSafeHwnd();
      //PostMessage(m_hMainFrameWnd, WM_SWICHNEWCAMERA, 0, 0);
    }
    else{
      ev->m_bIsEnsureTgtIP = FALSE;
      AfxMessageBox("连接目标相机失败，请核对目标相机的IP地址");
    }
  }
}
*/


int get_no_zero_pix_number_u8( int n, const unsigned char* x ) {
  int i, k;
  k = 0;
  for ( i = 0; i < n; ++i ) {
    k += !!x[ i ];
  }
  return k;
}

int get_no_zero_pix_number_i32(int n, const unsigned int* x) {
  int i, k;
  k=0;
  for (i=0; i<n; ++i) {
    k += !!x[i];
  }
  return k;
}

int select_object = 0;
CvRect selection = {0};
CvPoint origin = {0};

void on_mouse( int event, int x, int y, int flags, void* param )
{
  if( select_object )
  {
    selection.x = MIN(x,origin.x);
    selection.y = MIN(y,origin.y);
    selection.width = selection.x + CC_IABS(x - origin.x);
    selection.height = selection.y + CC_IABS(y - origin.y);
    
    selection.x = MAX( selection.x, 0 );
    selection.y = MAX( selection.y, 0 );
    selection.width = MIN( selection.width, 100000 );
    selection.height = MIN( selection.height, 100000 );
    selection.width -= selection.x;
    selection.height -= selection.y;
  }
  
  switch( event )
  {
  case CC_EVENT_LBUTTONDOWN:
    origin = cvPoint(x,y);
    selection = cvRect(x,y,0,0);
    select_object = 1;
    break;
  case CC_EVENT_LBUTTONUP:
    select_object = 0;
    break;
  }
}

#define CMD_SET_RUN_MODE(_MODE)    { char _mode = _MODE; if(!SendCMD(0x01, 0x00, 0x01, &_mode, 0, 0)) CC_ERROR(CC_StsError, "SendCMD"); }
#define CMD_CHECK_TRIGGER_MODE()   { if(!SendCMD(0x23, 0x00, 0x00, 0, 0, 0)) CC_ERROR(CC_StsError, "SendCMD"); }
#define CMD_GET_RUN_MODE(cRcvBuf)  { if(!SendCMD(0x03, 0x01, 0x00, 0, cRcvBuf, 0)) CC_ERROR(CC_StsError, "SendCMD"); }
//发送命令软触发抓取一祯图像
#define CMD_GRAB()  { if(!SendCMD(0x02, 0x00, 0x00, 0, 0, 0)) CC_ERROR(CC_StsError, "SendCMD"); }

int savepic_th(int* p) {
  void** param = (void**)p;
  char* name = (char*)param[0];
  img_t* a = (img_t*)param[1];
  imwrite2(name, a);
  imfree(a);
  SAFEFREE(name);
  SAFEFREE(param);
  return 0;
}

HANDLE savepic(const char* buf, img_t* a) {
  int savepic(int* p);
  void** param = MALLOC(void*, 2);
  unsigned long dw;
  param[0] = MALLOC(char, strlen(buf)+1);
  param[1] = imclone(a);
  memcpy(param[0], buf, strlen(buf)+1);
  return CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) savepic_th, ( LPVOID ) ( param ), 0, &dw );
}

#define BUFLEN 21000000
char g_buf[BUFLEN];

int test_Eagleeye2_RMVA2xxSC_Viewer() {
  CC_FUNCNAME("test_Eagleeye2_RMVA2xxSC_Viewer");
  
  //监听DSP图像数据的线程
  WORD         wVersionRequested;
  WSADATA      wsaData;
  SOCKET       sockClient[4] = {0};
  struct sockaddr_in  addr;
  int          i,cnt;
  BOOL         flag = TRUE;
  UINT         recvdatalen;
  BYTE         replyBuf[2];
  int port[4] = {TRGIMGPORT, FCONIMGPORT, HFRIMGPORT, HFRBINIMGPORT};
  int mode = HfrMode; // 默认模式
  int ImgAcquisition_Format=0;
  int fram;
  int bigfram=0;
  //struct GMM* gmm = 0;
  CvBGStatModel* pBGStatModel = NULL;
  capcam * cap2 = NULL;
  char buf[256];

  // config
  double pixthd = IniGetFloat2("./config.ini", "相机配置", "抓拍阈值");

  __CC_BEGIN__;
  
  printf("抓拍阈值=%f", pixthd);
  cvNamedWindow( "mmm", 1 );
  cvSetMouseCallback( "mmm", on_mouse, 0 );
  mode = TrgMode;
  mode = HfrMode;
  //获取目标相机的IP地址
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);
  if (i != 0){
    CC_ERROR(CC_StsError, "无法初始化套接口对象");
  }
  CMD_SET_RUN_MODE(mode);
  //开始接受相机上传图像数据
  while(1) {
    UINT      uiJpgDataLen;        //接受到的jpg图像数据的长度
    int m_bIsFconMode = 1;
    char ch = cvWaitKey(10);
    if ('q'==ch) {
      break;
    }
    if (0==sockClient[mode]) {
      CMD_CHECK_TRIGGER_MODE();
      sockClient[mode] = socket( AF_INET, SOCK_STREAM, 0 );
      if( sockClient[mode] < 0){
        CC_ERROR(CC_StsError, "socket");
      }
      addr.sin_family    = AF_INET;
      addr.sin_addr.s_addr  = TargetIP;
      addr.sin_port    = htons(port[mode]);
      //连接目标相机
      if ( connect( sockClient[mode], (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
        CC_ERROR(CC_StsError, "connect");
      }
      if (TrgMode==mode) {
        Sleep(100);
        CMD_GRAB();
      }
    }
    if (TrgMode==mode) {
      if ('c'==ch) {
        CMD_GRAB();
      }
    }
    for (i=0; i < 4; ) {
      cnt = recv( sockClient[mode], (char*)&uiJpgDataLen+i, 4-i, 0 );
      if( cnt <=0){
        CC_ERROR(CC_StsError, "recv");
      }
      i += cnt;
      if( i == 4){
        recvdatalen = uiJpgDataLen;
      }
    }
    if (TrgMode==mode) {
      //接受抓拍信息
      i = 0;
      while( i < IMGINFLEN ){
        cnt = recv( sockClient[mode], (char*)&EagleEye2_ImgInf+i, IMGINFLEN-i, 0 );
        if( cnt <= 0 ){
          CC_ERROR(CC_StsError, "recv");
        }
        i += cnt;
      }
    }
    CurrBuff=0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    for(i=0; i < (int)recvdatalen; ) {
      cnt = recv( sockClient[mode], listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
      if( cnt <= 0 ){
        CC_ERROR(CC_StsError, "recv");
      }
      i += cnt;
    }
    //在全分辨率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
    {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;
      if( send( sockClient[mode], (char*)replyBuf, 2, 0 ) < 0 ){
        CC_ERROR(CC_StsError, "send");
      }
    }
    {
      img_t* a = imread2_mem(listJpgBuff[CurrBuff].pBuff, recvdatalen, 0);
      if (HfrMode==mode) {
        int h = a->height, w = a->width, al = a->step, ai = a->channels;
        unsigned char* img = a->data;
        if (NULL==cap2 && file_exist(".\\cap")) {
          sf_get_time();
          _snprintf(buf, 256, ".\\cap\\cap_%02d%02d%02d%03d.avi", sfhour(), sfminute() , sfsecond() , sfmillis() );
          cap2 = cap_create_avi( buf, h, w, ai );
        }
        if (cap2) {
          cap_writeFrame(cap2, img, al);
        }
        if (selection.width) {
          if (select_object) {
            fram=0;
            if (NULL!=pBGStatModel) {
              cvReleaseBGStatModel( &pBGStatModel );
            }
          } else {
            if (selection.height>0 && selection.width>0) {
              selection.width = MIN( selection.width, w-selection.x );
              selection.height = MIN( selection.height, h-selection.y );
              //gmm = CHC_CreateGMModel(0.005, 0.4, 5.0, 0.05, 0.95, 0.3, 0.3, 0.1,
              //  selection.height, selection.width, 3, 20, 0, 0, 0, 0);
            }
          }
          printf("fram %d ", fram);
          if (selection.height>0 && selection.width>0) {
            unsigned char* img2 = img+selection.x*3 + selection.y*al;
            if (0==fram) {
              ASSERT(ai==3);
              utime_start(_start_time);
              //CHC_SetupBG(gmm, img2, al, fram+1);
              pBGStatModel = cvCreateAvgBGModel( selection.height, selection.width, img2, al, 3, 0 );
              printf(" cvCreateAvgBGModel = %f ", utime_elapsed(_start_time));
            } else {
              //const unsigned int* p;
              int rectnum=0;
              utime_start(_start_time);
              cvUpdateBGStatModel( img2, al, pBGStatModel );
              //rectnum = CHC_ForegroundDetect(gmm, img2, al, 0);
              printf(" CHC_ForegroundDetect = %f ", utime_elapsed(_start_time));
              {
                int n = pBGStatModel->height*pBGStatModel->width;
                int k = get_no_zero_pix_number_u8(n, pBGStatModel->foreground);
                float rate = k*1.f/n;
                printf(" pix = %f", rate);
                if (rate>pixthd) {
                  ch = '0'+TrgMode; // 切换到抓拍模式
                }
              }
            }
            fram++;
          }
          DrawRectangle(h, w, img, al, ai, cvPoint(selection.x, selection.y), 
            cvPoint(selection.width + selection.x, selection.height + selection.y), CC_RGB( 255, 0, 0 ), 3, 8, 0);
        }
        printf("\n");
        imshow("mmm", a);
      }
      if (TrgMode==mode && file_exist(".\\jpg")) {
        sf_get_time();
        _snprintf(buf, 256, ".\\jpg\\pic_%02d%02d%02d%03d.jpg", sfhour(), sfminute() , sfsecond() , sfmillis() );
        printf("%s\n", buf);
        
        {
          int j;
          plate_algo pa = {0};
          utime_start(_start_time);
          pa.h=a->height, pa.w = a->width, pa.bgr = a->data, pa.bgr_step=a->step;
          pa.buf = g_buf, pa.buflen = BUFLEN;
          pa.mode = mode;
          plate_algo_process(&pa);
          for (j=0; j<pa.platelen; ++j) {
            DrawRectangleR( a->height, a->width, a->data, a->step, a->channels,
              &(pa.plate[j].rect), 1, CC_RGB(255, 0, 0), 2, 8, 0);
          }
          printf("%f", utime_elapsed(_start_time));
        }
#if 0
        imwrite2(buf, a);
#else
        savepic(buf, a);
#endif
        bigfram++;
        ch = '0'+HfrMode;
      }
      imfree(a);
    }
    {
      if ('q'==ch) {
        break;
      }
      switch( ch ) {
      case 'q':
        closesocket(sockClient[mode]), sockClient[mode]=0;
        CC_ERROR(CC_StsOk, "");
        break;
      case '0':
      case '1':
      case '2':
      case '3':
        {
          replyBuf[0] = 0xff;
          replyBuf[1] = 0xff;
          if( send( sockClient[mode], (char*)replyBuf, 2, 0 ) < 0 ){
            CC_ERROR(CC_StsError, "send");
          }
          closesocket(sockClient[mode]), sockClient[mode]=0;
          mode = ch-'0';
          CMD_CHECK_TRIGGER_MODE();
          CMD_SET_RUN_MODE(mode);
        }
        break;
      default:
        break;
      }
    }
  }
  __CC_END__;
  if (NULL!=pBGStatModel) {
    cvReleaseBGStatModel( &pBGStatModel );
  }
  //清理环境，退出线程
  WSACleanup();
  cam_stop ( cap2 );
  return 0;
}

int test_savepic() {
  img_t* a = imread2("F:\\pub\\bin\\imgproc\\lena_gray.bmp", 1);
  HANDLE h = savepic("F:\\pub\\bin\\imgproc\\aaa.jpg", a);
  WaitForSingleObject(h, 1000);
  imfree(a);
  return 0;
}

int main() {
  test_Eagleeye2_RMVA2xxSC_Viewer();
  return 0;
}
