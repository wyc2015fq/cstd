
#include "VideoSDK.h"
#include "yuv2bmp.inl"

//校验和计算
UINT16 sock_checksum(UINT16* buffer, UINT32 size)
{
  UINT32 cksum = 0;

  while (size > 1) {
    cksum += *buffer++;
    size -= sizeof(UINT16);
  }

  if (size) {
    cksum += *(UINT8*)buffer;
  }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (UINT16)(~cksum);
}

//解析设备控制命令命令头
int ResolveNCMD(const char* pBuffer, const int nLen , int* nRlt)
{
  NCMD* pCMD = (NCMD*)(pBuffer + OFFSET_SIZE);
  *nRlt = GET_ERR(pCMD->m_CFlag);
  return pCMD->m_MCmd;
}


//接收视频数据线程
static DWORD WINAPI RecvVideoDataThread(VideoPro* pv)
{
  char* pTemp = 0; //缓冲大小必须不小于sizeof(SOCK_DATA_HEADER) + PACK_SIZE
  int cn = pv->cn;
  int nLen = 0;
  int nCmd = 0;
  int nRlt = 0;

  SOCKADDR_IN addrClient = {0};
  int len = sizeof(SOCKADDR);
  char* pData = NULL;
  DEVIMAGE* pDevImage = NULL;

  typedef struct databuf_t {
    UINT32 id;
    char* buf;
  } databuf_t;
  enum { maxbufnum = 32 };
  databuf_t databuf[maxbufnum] = {0};
  int bufnum = 0;
  char* pBuffer = NULL;
  enum {pack_size = sizeof(SOCK_DATA_HEADER) + PACK_SIZE};
  pTemp = (char*)malloc(pack_size);

  while (!pv->m_VideoClose) {
    databuf_t* it = NULL;
    int i;
    //接收视频数据
    nLen = recvfrom(pv->m_sVVIDEOSck, pTemp, pack_size, 0, (struct sockaddr*)&addrClient, &len);

    if (nLen > 0) {
      //获取包头
      SOCK_DATA_HEADER Head = *((SOCK_DATA_HEADER*)(pTemp));

      //校验
      if (0 != sock_checksum((UINT16*)(&Head), sizeof(SOCK_DATA_HEADER))) {
        continue;  //校验出错,丢弃
      }

      if (Head.msg_offset == 0) { //第一个分包
        //解析网络的数据包
        nCmd = ResolveNCMD(pTemp, nLen, &nRlt);

        if (nCmd == M_VIDEO_OUT) {
          ;//第一个分包,命令头在第一个分包里面。
        }
        else {
          continue;  //丢弃，非法包
        }
      }

      //查找本次标识的数据接收缓冲
      for (i = 0; i < bufnum; ++i) {
        if (databuf[i].id == Head.msg_identify) {
          it = databuf + i;
          break;
        }
      }

      if (NULL == it) { //没找到，则为新视频数据包。
        int nSize;

        if (Head.msg_offset != 0) { //检验是否丢包头，否则为无命令头的非法视频数据。
          continue;
        }

        //为接收本次视频数据开辟一个快缓存，并插入到MAP映射表中
        nSize = Head.msg_total + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32) + 4;
        pBuffer = (char*)malloc(nSize);
        ZeroMemory(pBuffer, nSize);

        if (bufnum < maxbufnum) {
          it = databuf + bufnum++;
        }
        else {
          for (i = 0; i < maxbufnum; ++i) {
            free(databuf[i].buf);
            databuf[i].buf = 0;
            databuf[i].id = 0;
          }

          it = databuf + bufnum++;
        }

        it->buf = pBuffer;
        it->id = Head.msg_identify;
      }
      else {
        pBuffer = it->buf; // 找到本次视频数据包的数据接收缓存
      }

      assert((long)pBuffer % 4 == 0);//验证缓存是否4字节对齐

      //组包
      memcpy(pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32) + Head.msg_offset, pTemp + sizeof(SOCK_DATA_HEADER), Head.msg_len);

      //累积当前包的已接收的分包总长度
      {
        SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*) pBuffer;
        pHdr->msg_total += Head.msg_len;

        //判断组包是否完成，完成则播放
        if (pHdr->msg_total == Head.msg_total) {
          NCMD* pNCM = (NCMD*)(pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32));
          pDevImage = (DEVIMAGE*)(pNCM + 1);

          if (pDevImage->m_W != pv->w || pDevImage->m_H != pv->h) {
            pv->buf = (unsigned char*)realloc(pv->buf, pDevImage->m_H * pDevImage->m_W * cn*2);
            pv->pRGBTMP = pv->buf;
            pv->pRGBTRANS = pv->buf + pDevImage->m_H * pDevImage->m_W * cn;
          }


          if (IMAGE_UYVY == pDevImage->m_Type) {
            pv->w = pDevImage->m_H;
            pv->h = pDevImage->m_W;
            YUV422_C_RGB(pDevImage->m_H, pDevImage->m_W, (unsigned char*)pDevImage->m_Block, pDevImage->m_W * 2, pv->pRGBTMP, pDevImage->m_W * cn, cn);
            RGB_EDDY(pv->pRGBTMP, pDevImage->m_H, pDevImage->m_W, pv->pRGBTRANS, cn, pv->flipud);
          }
          else if (IMAGE_YUYV == pDevImage->m_Type) {
            pv->w = pDevImage->m_W;
            pv->h = pDevImage->m_H;
            YUYV_C_RGB((unsigned char*)pDevImage->m_Block, pv->pRGBTRANS, pDevImage->m_H, pDevImage->m_W, cn);
          }

          //显示视频
          if (pv->m_framefun) {
            pv->m_framefun(pv->h, pv->w, pv->pRGBTRANS, pv->w * cn, cn);
          }
          else {
            printf("%d %d %d\n", pv->m_nIndex, pv->h, pv->w);
          }

          //清除，当前已经播放的视频数据缓存
          for (i = 0; i < maxbufnum; ++i) {
            free(databuf[i].buf);
            databuf[i].buf = 0;
            databuf[i].id = 0;
          }

          bufnum = 0;
        }
      }

      ZeroMemory(pTemp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE);
    }

    usleep(1);
  }

  if (pv->buf) {
    free(pv->buf);
    pv->buf = 0;
    pv->pRGBTMP = 0;
    pv->pRGBTRANS = 0;
  }

  free(pTemp);

  pv->m_VideoClose = 0;
  return 0;
}

unsigned long _inet_addr(const char* ip)
{
  if (NULL == ip) {
    return 0;
  }
  else {
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    unsigned int ipInt = 0;
    BYTE* pInt = (BYTE*)(&ipInt);

    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    pInt[0] = (BYTE)a;
    pInt[1] = (BYTE)b;
    pInt[2] = (BYTE)c;
    pInt[3] = (BYTE)d;
    return ipInt;
  }
}

//组装网络命令包
int NETCMDPacket(const char* pCMDPacketData, const int nlen, const int nSeq)
{
  //加包头
  SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(pCMDPacketData);
  hdr->msg_identify = nSeq;   //标识
  hdr->msg_offset  = 0 ;  //偏移
  hdr->msg_len   = nlen;//当已经传送过来的命令数据长度，包括命名头的长度，不包括网络包头长度
  hdr->msg_total   = nlen;//总的命令数据长度，包括命令头长度和命令的参数长度
  hdr->msg_checksum = 0;//检验和
  hdr->msg_checksum = sock_checksum((UINT16*)hdr, sizeof(SOCK_DATA_HEADER));
  return 1;
}

//登陆设备
int Login(VideoPro* pv, const char* sLoginName, const char* sPassword)
{
  int retVal;
  DEV_AUTH* pAuth = 0;
  char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包
  //组命令头
  NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);
  pCmd->m_MCmd = M_AUTH;
  pCmd->m_SCmd = S_SET;
  pCmd->m_CFlag = F_REQ_FLAG;
  pCmd->m_CFlag |= AUTH_VERIFY;
  pCmd->m_Param1 = pv->m_nIndex++;
  pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);

  //组命令数据
  pAuth = (DEV_AUTH*)(pCmd + 1) ;
  memcpy(pAuth->m_UserName.m_ID, sLoginName, DEV_ID_LEN);
  memcpy(pAuth->m_Password.m_ID, sPassword, DEV_ID_LEN);

  //组包头
  NETCMDPacket(pBuffer, sizeof(NCMD) + sizeof(DEV_AUTH), pCmd->m_Param1);

  //发送
  retVal = send(pv->m_sHost, pBuffer, sizeof(NCMD) + sizeof(DEV_AUTH) + OFFSET_SIZE, 0);

  if (SOCKET_ERROR == retVal) {
    printf("send Author CMD failed!\n");
    closesocket(pv->m_sHost); //关闭套接字
    return FALSE;
  }

  return TRUE;
}

//接收实时控制数据线程
static DWORD WINAPI RecvRealControlDataThread(VideoPro* pv)
{
  int retVal;
  int nLen = 0;
  int nCmd = 0;
  int nRlt = 0;
  char pBuffer[PACK_SIZE] = {0};//最大60K大小一个包

  while (!pv->m_exitThread) {
    //初始化缓存
    ZeroMemory(pBuffer, PACK_SIZE);
    nLen = recv(pv->m_sHost, pBuffer, PACK_SIZE , 0);

    if (nLen > 0) { //接收到数据
      //解析网络的数据包
      nCmd = ResolveNCMD(pBuffer, nLen, &nRlt);

      if (nCmd == M_AUTH) { //登陆权限验证
        if (nRlt == SUC_EXECUT) { //权限验证成功
          pv->m_Author = TRUE;
          printf("pv->m_Author %d\n", pv->m_Author);
        }
      }

      //心跳
      if (nCmd == M_HEART && nRlt == SUC_EXECUT) {
        //组命令头
        NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);
        ZeroMemory(pBuffer, PACK_SIZE);
        pCmd->m_MCmd = M_HEART;
        pCmd->m_SCmd = S_SET;
        pCmd->m_Param1 = pv->m_nIndex++;
        pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);
        pCmd->m_CFlag = 0;

        //封包头
        NETCMDPacket(pBuffer, sizeof(NCMD) , pCmd->m_Param1);

        //发送
        retVal = send(pv->m_sHost, pBuffer, sizeof(NCMD) + OFFSET_SIZE, 0);

        if (SOCKET_ERROR == retVal) {
          printf("send Heart CMD failed!\n");
          closesocket(pv->m_sHost); //关闭套接字
          pv->m_sHost = 0;
          break;
        }
      }

      //视频命令响应
      if (nCmd == M_VIDEO_OUT) {
        if (nRlt == SUC_EXECUT) {
          int bVideo = FALSE;
        }
      }
    }

    usleep(50);
  }

  pv->m_exitThread = FALSE;
  return FALSE;
}



//初始化
int SDKInit(VideoPro* pv, const char* strDevIP, const char* strClientIP, frame_fun_t framefun1, void* userdata, int cn, int flipud)
{
  netinit();

  //初始化YVU转RGB的表，显示视频必须先初始化。

  pv->m_nIndex = 0;//命令包序号
  pv->m_Author = FALSE;    // 登陆设备权限验证通过标志
  pv->m_VideoClose = FALSE;// 关闭视频标志
  pv->m_exitThread = FALSE;
  pv->m_hTCPThread = 0;
  pv->m_hUDPThread = 0;
  pv->cn = cn;
  pv->flipud = flipud;

  pv->userdata = userdata;
  pv->m_framefun     = framefun1;
  strcpy(pv->m_strDevIP, strDevIP);

  if (strClientIP) {
    strcpy(pv->m_strClientIP, strClientIP);
    printf("IP: %s\n", strClientIP);
  }
#if 0
  else {
    struct hostent* pHost;
    char szHost[256];

    //取得本地主机名称
    if (gethostname(szHost, 256) == 0) {
      printf("host: %s\n", szHost);

      if ((pHost = gethostbyname(szHost)) != NULL) { //这些就是获得IP的函数
        char* ip = inet_ntoa(*(struct in_addr*)*pHost->h_addr_list);
        printf("IP: %s\n", ip);
        strcpy(pv->m_strClientIP, ip);
      }
    }
  }
#endif

  InitialTable();
  return TRUE;
}

//与设备建立连接
int SDKConnect(VideoPro* pv)
{
  int nServAddlen, retVal;
  //与设备建立连接
  SOCKADDR_IN servAddr; //服务器地址
  //创建套接字
  pv->m_sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (INVALID_SOCKET == pv->m_sHost) {
    printf("socket failed!\n");
    return 0;
  }

  //设置服务器地址
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(pv->m_strDevIP);
  servAddr.sin_port = htons((short)TCPPORT);
  nServAddlen = sizeof(servAddr);

  //连接服务器
  retVal = connect(pv->m_sHost, (SOCKADDR*)&servAddr, sizeof(servAddr));

  if (SOCKET_ERROR == retVal) {
    printf("connect failed!\n");
    closesocket(pv->m_sHost); //关闭套接字
    return 0;
  }
  //启动接收实时控制数据线程
  memcpy(pv->m_IP.m_ID, pv->m_strDevIP , DEV_ID_LEN);
  pv->m_hTCPThread = thread_create((thread_cb)RecvRealControlDataThread, (void*)pv);

  //登陆设备，进行身份权限验证，设备初试登陆名和口令为：admin，201031
  if (!Login(pv, "admin", "201031")) {
    return FALSE;
  }

  while (!pv->m_Author) { //等待权限验证成功（设备端的验证响应消息）
    usleep(50);
  }

  return pv->m_Author;
}

//开启视频
int SDKStartVideo(VideoPro* pv)
{
  if (!pv->m_Author) { //通过权限验证
    return FALSE;
  }

  //打开视频，包括启动视频数据接收线程
  //创建接收视频数据的SOKECT,已经启动接收视频数据包的线程。
  {
    int nLen;
    //创建接收视频数据的SOKECT
    SOCKADDR_IN addrSrv;
    struct sockaddr_in addr = {0};
    pv->m_sVVIDEOSck = socket(AF_INET, SOCK_DGRAM, 0);
    //inet_ntoa(m_address.sin_addr);
    addrSrv.sin_addr.s_addr = inet_addr(pv->m_strClientIP);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(0);//随机获取一个可用的UDP端口
    bind(pv->m_sVVIDEOSck, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

    nLen = sizeof(struct sockaddr_in);

    if (SOCKET_ERROR == getsockname(pv->m_sVVIDEOSck, (struct sockaddr*)&addr, &nLen)) {
      return 0;
    }

    pv->m_nUDPPort = ntohs(addr.sin_port);  //获得已经绑定的端口号

    //启动接收视频数据线程
    memcpy(pv->m_IP.m_ID, pv->m_strDevIP , DEV_ID_LEN);
    pv->m_hUDPThread = thread_create((thread_cb)RecvVideoDataThread, (void*)pv);
  }
  {
    int len, retVal;
    SOCK_ADDRESS* Addr;
    //向设备发起打开视频的命令
    char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包
    //组命令头
    NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);

    pCmd->m_MCmd = M_VIDEO_OUT;
    pCmd->m_SCmd = S_SET;
    pCmd->m_Param1 = pv->m_nIndex++;
    pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);
    pCmd->m_CFlag = F_REQ_FLAG;//本地

    //组命令数据
    Addr = (SOCK_ADDRESS*)(pCmd + 1);
    strcpy(Addr->IP_Address, pv->m_strClientIP);
    Addr->Port = pv->m_nUDPPort;

    len = ALIGN(sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4);   //必须4字节对齐

    //封包头
    NETCMDPacket(pBuffer, len , pCmd->m_Param1);

    //发送
    retVal = send(pv->m_sHost, pBuffer, len + OFFSET_SIZE, 0);

    if (SOCKET_ERROR == retVal) {
      printf("send StartVideo failed!\n");
      closesocket(pv->m_sHost); //关闭套接字
      return 0;
    }

    if (!pv->m_sVVIDEOSck) {
      return FALSE;
    }
  }
  return TRUE;
}

//停止视频
int SDKStopVideo(VideoPro* pv)
{
  int len, retVal;
  SOCK_ADDRESS* Addr;
  //关闭视频
  char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包

  //组命令头
  NCMD* pCmd = (NCMD*)(pBuffer + OFFSET_SIZE);
  pCmd->m_MCmd = M_VIDEO_OUT;
  pCmd->m_SCmd = S_GET;
  pCmd->m_Param1 = pv->m_nIndex++;
  pCmd->m_Param2 = _inet_addr(pv->m_strDevIP);
  pCmd->m_CFlag = F_REQ_FLAG;//本地

  //组命令数据
  Addr = (SOCK_ADDRESS*)(pCmd + 1);
  strcpy(Addr->IP_Address, pv->m_strClientIP);
  Addr->Port = pv->m_nUDPPort;
  len = ALIGN(sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4);  //必须4字节对齐

  //封包头
  NETCMDPacket(pBuffer, len , pCmd->m_Param1);

  //发送
  retVal = send(pv->m_sHost, pBuffer, len + OFFSET_SIZE, 0);

  if (SOCKET_ERROR == retVal) {
    printf("send StopVideo failed!\n");
    closesocket(pv->m_sHost); //关闭套接字
    return FALSE;
  }

  //关闭视频UDP套接字
  if (pv->m_sVVIDEOSck) {
    closesocket(pv->m_sVVIDEOSck);
    pv->m_sVVIDEOSck = 0;
  }

  //关闭视频数据处理线程
  if (pv->m_hUDPThread) {
    pv->m_VideoClose = TRUE;

    thread_join(pv->m_hUDPThread);

    thread_detach(pv->m_hUDPThread);
    pv->m_hUDPThread = 0;
  }

  return TRUE;
}

//断开连接
void SDKDisConnect(VideoPro* pv)
{
  //关闭和设备的连接
  if (pv->m_sHost) {
    closesocket(pv->m_sHost); //关闭套接字
    pv->m_sHost = 0;
  }

  //关闭实时控制数据线程
  if (pv->m_hTCPThread) {
    pv->m_exitThread = TRUE;

    thread_join(pv->m_hTCPThread);
    thread_detach(pv->m_hTCPThread);
    pv->m_hTCPThread = 0;
  }

  pv->m_Author = FALSE;
}

static DWORD WINAPI start_sdk_thd(VideoPro* pv)
{
  pv->run = 1;
  SDKConnect(pv);
  SDKStartVideo(pv);
  
  for (; pv->run;) {
    usleep(1000);
  }
  SDKStopVideo(pv);//关闭视频
  SDKDisConnect(pv);//关闭连接
  netexit();//释放套接字资源
  return 0;
}

thread_t start_sdk(VideoPro* pv, const char* strDevIP, const char* strClientIP, frame_fun_t framefun1, void* userdata, int cn, int flipud) {
  thread_t hthread = 0;
  SDKInit(pv, strDevIP, strClientIP, framefun1, userdata, cn, flipud);
  hthread = thread_create((thread_cb)start_sdk_thd, (void*)pv);
  return hthread;
}

int test_VideoSDK()
{
  VideoPro pv[1] = {0};
  const char* ip;
  const char* cip = 0;
  ip = "192.168.1.155";
  ip = "192.168.1.189";
  ip = "192.168.1.189";
  ip = "192.168.1.152";
  ip = "192.168.0.111";
  //cip = "127.0.0.1";
  cip = "192.168.0.3";
#ifndef WIN32
  cip = "192.168.16.139";
  cip = "192.168.1.34";
  //cip = getlocalip();
  cip = "192.168.16.3";
  cip = "192.168.16.139";
  cip = "192.168.0.112";
#endif
  SDKInit(pv, ip, cip, 0, 0, 3, 1);
  SDKConnect(pv);
  SDKStartVideo(pv);

  for (; getchar() != 'q';) {
  }

  SDKStopVideo(pv);//关闭视频
  SDKDisConnect(pv);//关闭连接
  netexit();//释放套接字资源
  return 0;
}
