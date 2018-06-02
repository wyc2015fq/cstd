// VideoSDK.cpp: implementation of the VideoSDK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoClient.h"
#include "VideoSDK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int     VideoSDK::g_nIndex      = 0;//命令包序号
bool    VideoSDK::g_Author      = false;//登陆设备权限验证通过标志
bool    VideoSDK::g_VideoClose  = false ;//关闭视频标志
bool    VideoSDK::g_exitThread  = false;
HWND    VideoSDK::g_hVideoWND   = NULL;
HANDLE  VideoSDK::g_hTCPThread  = NULL;
HANDLE  VideoSDK::g_hUDPThread  = NULL;

static DWORD WINAPI RecvVideoDataThread( void* p );

unsigned long _inet_addr( const char* ip )
{
	if ( NULL == ip ) return 0;
	unsigned long a = 0 ;
	unsigned long b = 0;
	unsigned long c = 0;
	unsigned long d = 0;
	sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
	unsigned long ipInt = 0;
	BYTE* pInt = (BYTE*)(&ipInt);
	pInt[0] = a;
	pInt[1] = b;
	pInt[2] = c;
	pInt[3] = d;
	return ipInt;
}

//初始化
bool Init()
{
	WSADATA wsd; //WSADATA变量  
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)  
	{  
		cout << "WSAStartup failed!" << endl;  
		return false;  
	}  
    
	//初始化YVU转RGB的表，显示视频必须先初始化。
	InitialTable();
    return true;
} 

// 与设备建立连接
SOCKET ConnectServer( char* sSeverIP, int nPort )
{ 
	SOCKET sHost = 0;

    SOCKADDR_IN servAddr; //服务器地址  
	//创建套接字  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(INVALID_SOCKET == sHost)  
	{  
		cout << "socket failed!" << endl;  
		WSACleanup();//释放套接字资源  
		return  0;  
	}  

	//设置服务器地址  
	servAddr.sin_family      = AF_INET;  
	servAddr.sin_addr.s_addr = inet_addr(sSeverIP);  
	servAddr.sin_port        = htons((short)nPort);  
	int nServAddlen          = sizeof(servAddr);  

	//连接服务器  
    int	retVal=connect(sHost,(LPSOCKADDR)&servAddr, sizeof(servAddr));  
	if(SOCKET_ERROR == retVal)  
	{  
		cout << "connect failed!" << endl;  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return 0;  
	} 

	return sHost;
}


//关闭和设备的连接
void DisConnectServer(SOCKET& sHost)
{ 
	if (sHost)
	{
		closesocket(sHost); //关闭套接字 
		sHost = NULL;
	}
}

//校验和计算
UINT16 sock_checksum( UINT16 *buffer, UINT32 size )
{
    UINT32 cksum = 0;
    while(size >1)
	{ 
        cksum += *buffer++; 
        size  -= sizeof(UINT16); 
    }     
    if(size )
	{ 
        cksum += *(UINT8*)buffer; 
    } 
    cksum = (cksum >> 16) + (cksum & 0xffff); 
    cksum += (cksum >>16); 
    return (UINT16)(~cksum); 
}

//包头固定程度为20
#define OFFSET_SIZE 20

//组装网络命令包
bool NETCMDPacket(const char*  pCMDPacketData, const int nlen, const int nSeq)
{
	//加包头
	SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(pCMDPacketData);
	hdr->msg_identify = nSeq;		//标识
	hdr->msg_offset	  = 0 ;	//偏移
	hdr->msg_len	  = nlen;//当已经传送过来的命令数据长度，包括命名头的长度，不包括网络包头长度      
	hdr->msg_total	  = nlen;//总的命令数据长度，包括命令头长度和命令的参数长度 	    
	hdr->msg_checksum = 0;//检验和
    hdr->msg_checksum = sock_checksum( (UINT16*)hdr, sizeof(SOCK_DATA_HEADER) );

	return true;
}

//登陆设备
bool  Login(const SOCKET sHost,  const char* pServerIP, const char* sLoginName, const char* sPassword)
{
	char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包
	//组命令头
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
	pCmd->m_MCmd   = M_AUTH;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_CFlag  = F_REQ_FLAG;
	pCmd->m_CFlag |= AUTH_VERIFY;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);

	//组命令数据
	DEV_AUTH* pAuth = (DEV_AUTH*)( pCmd + 1) ;
	memcpy( pAuth->m_UserName.m_ID, sLoginName, DEV_ID_LEN); 
	memcpy( pAuth->m_Password.m_ID, sPassword, DEV_ID_LEN); 

	//组包头
	NETCMDPacket( pBuffer,  sizeof(NCMD)+sizeof(DEV_AUTH), pCmd->m_Param1);

	//发送
	int retVal = send(sHost, pBuffer, sizeof(NCMD) + sizeof(DEV_AUTH) + OFFSET_SIZE, 0);  

	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send Author CMD failed!" << endl;  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return false;  
	}  

	return true;
}

//心跳
bool  Heart(const SOCKET sHost,  const char* pServerIP )
{
	char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包
	//组命令头
	NCMD*  pCmd    = ( NCMD* ) ( pBuffer + OFFSET_SIZE );
	pCmd->m_MCmd   = M_HEART;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  = 0;

	//封包头
	NETCMDPacket( pBuffer, sizeof(NCMD) , pCmd->m_Param1);

	//发送
	int retVal = send(sHost, pBuffer, sizeof(NCMD) + OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send Heart CMD failed!" << endl;  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return false;  
	}  

	return true;
}

SOCKET RunRecvVideoData(const char* pServerIP, const char* pClientIP,unsigned short& nUPDPort)
{
	//创建接收视频数据的SOKECT
	SOCKET sockSrv               = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr =  inet_addr(pClientIP);
	addrSrv.sin_family           =  AF_INET;
	addrSrv.sin_port             =  htons(0);//随机获取一个可用的UDP端口
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	
	int nLen         = sizeof( sockaddr_in );
	sockaddr_in addr = {0};
	if ( SOCKET_ERROR == getsockname( sockSrv, (sockaddr*)&addr, &nLen) ) return 0;
	nUPDPort         =  ntohs( addr.sin_port );//获得已经绑定的端口号
	
	//启动接收视频数据线程
	ENDPNT*  pEnPnt = new ENDPNT;
	pEnPnt->m_hSck = sockSrv;
	memcpy( pEnPnt->m_IP.m_ID, pServerIP , DEV_ID_LEN);
	VideoSDK::g_hUDPThread = CreateThread( NULL, 0, RecvVideoDataThread, (void*) pEnPnt,  NULL, NULL );
	return sockSrv;
}

//视频
SOCKET StartVideo(const SOCKET sHost,  const char* pServerIP, const char* pClientIP,unsigned short & nUPDPort )
{
	//创建接收视频数据的SOKECT,已经启动接收视频数据包的线程。
	nUPDPort = 0;
	SOCKET sockSrv = RunRecvVideoData( pServerIP, pClientIP, nUPDPort);

	//向设备发起打开视频的命令
	char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包
	//组命令头
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
    pCmd->m_MCmd   = M_VIDEO_OUT;
	pCmd->m_SCmd   = S_SET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  =  F_REQ_FLAG;//本地

	//组命令数据
	SOCK_ADDRESS* Addr = (SOCK_ADDRESS*)(pCmd+1);
	strcpy( Addr->IP_Address, pClientIP );
	Addr->Port = nUPDPort;

	UINT32 len = ALIGN( sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4 ); //必须4字节对齐

	//封包头
	NETCMDPacket( pBuffer, len  , pCmd->m_Param1);
	
	//发送
	int retVal = send(sHost, pBuffer, len + OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send StartVideo failed!" << endl;  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return 0;  
	}  

	return sockSrv;
}

//关闭视频
bool StopVideo(const SOCKET sHost, const char* pServerIP, const char* pClientIP,unsigned short nUPDPort)
{
	char pBuffer[PACK_SIZE] = { 0 };//最大60K大小一个包

	//组命令头
	NCMD*  pCmd    = (NCMD*)(pBuffer+OFFSET_SIZE);
    pCmd->m_MCmd   = M_VIDEO_OUT;
	pCmd->m_SCmd   = S_GET;
	pCmd->m_Param1 = VideoSDK::g_nIndex++;
	pCmd->m_Param2 = _inet_addr(pServerIP);
	pCmd->m_CFlag  = F_REQ_FLAG;//本地
	
	//组命令数据
	SOCK_ADDRESS* Addr = (SOCK_ADDRESS*)(pCmd+1);
	strcpy( Addr->IP_Address, pClientIP );
	Addr->Port = nUPDPort;
	UINT32 len = ALIGN( sizeof(NCMD) + sizeof(SOCK_ADDRESS) , 4 );//必须4字节对齐

	//封包头
	NETCMDPacket( pBuffer, len , pCmd->m_Param1);
	
	//发送
	int retVal = send(sHost, pBuffer, len+OFFSET_SIZE, 0);  
	if (SOCKET_ERROR == retVal)  
	{  
		cout << "send StopVideo failed!" << endl;  
		closesocket(sHost); //关闭套接字  
		WSACleanup(); //释放套接字资源  
		return false;  
	}  

	return true;
}

//解析设备控制命令命令头
int    ResolveNCMD(const char* pBuffer,const int nLen , int& nRlt )
{
	NCMD* pCMD = (NCMD*)(pBuffer+OFFSET_SIZE);
	nRlt       = GET_ERR(pCMD->m_CFlag);
	return pCMD->m_MCmd;
}



//接收实时控制数据线程
static DWORD WINAPI RecvRealControlDataThread( void* p )
{
	char pBuffer[PACK_SIZE] = {0};//最大60K大小一个包

	ENDPNT* pEndPnt =  (ENDPNT*) p;
	SOCKET sHost    =  pEndPnt->m_hSck;
	int nLen = 0;
	int nCmd = 0;
	int nRlt = 0;
	while( !VideoSDK::g_exitThread )
	{
		//初始化缓存
		ZeroMemory(pBuffer, PACK_SIZE);  
		nLen = recv( sHost, pBuffer, PACK_SIZE , 0);
		
		if ( nLen > 0 )//接收到数据
		{
			//解析网络的数据包
			nCmd = ResolveNCMD( pBuffer, nLen, nRlt );
			
			if (nCmd == M_AUTH)//登陆权限验证
			{
				if ( nRlt == SUC_EXECUT )//权限验证成功
					VideoSDK::g_Author = true;
			}

			//心跳
			if( nCmd == M_HEART)
			{
				if ( nRlt == SUC_EXECUT )
				{
					 if(!Heart(sHost,pEndPnt->m_IP.m_ID))//客户端回送心跳响应包
						 break;
				}
			}
			
			//视频命令响应
			if( nCmd == M_VIDEO_OUT)
			{
				if ( nRlt == SUC_EXECUT )
				{
					bool bVideo = false;

				}

			}
		}

		Sleep(50);
	}
	delete pEndPnt;
	VideoSDK::g_exitThread = false;

	return 0;
}


//显示视频
void  ShowVideo( const DEVIMAGE* pData, VideoPro* pPro)
{
	if ( NULL == pPro->m_pRGBTMP || pData->m_W  != pPro->m_pBitmapInfo->bmiHeader.biWidth || pData->m_H != pPro->m_pBitmapInfo->bmiHeader.biHeight)
	{
		delete [] pPro->m_pRGBTMP;
		delete [] pPro->m_pRGBTRANS;
		free( pPro->m_pBitmapInfo );

		pPro->m_pRGBTMP     = new BYTE[pData->m_H*pData->m_W*3];
		pPro->m_pRGBTRANS   = new BYTE[pData->m_H*pData->m_W*3];
		pPro->m_pBitmapInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));	

		pPro->m_pBitmapInfo->bmiHeader.biSize          = sizeof(BITMAPINFO);
		pPro->m_pBitmapInfo->bmiHeader.biPlanes        = 1;
		pPro->m_pBitmapInfo->bmiHeader.biBitCount      = 24;
		pPro->m_pBitmapInfo->bmiHeader.biCompression   = BI_RGB;
		pPro->m_pBitmapInfo->bmiHeader.biSizeImage     = 0;
		pPro->m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
		pPro->m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
		pPro->m_pBitmapInfo->bmiHeader.biClrUsed       = 0;
		pPro->m_pBitmapInfo->bmiHeader.biClrImportant  = 0;
	}

	if ( IMAGE_UYVY == pData->m_Type )
	{
		pPro->m_pBitmapInfo->bmiHeader.biWidth  =  pData->m_H;
		pPro->m_pBitmapInfo->bmiHeader.biHeight = pData->m_W;
		YUV422_C_RGB( (BYTE*)pData->m_Block, pPro->m_pRGBTMP, pData->m_H, pData->m_W );
		RGB_EDDY( pPro->m_pRGBTMP, pData->m_H, pData->m_W, pPro->m_pRGBTRANS );
	}
	else if ( IMAGE_YUYV ==  pData->m_Type )
	{
		pPro->m_pBitmapInfo->bmiHeader.biWidth  = pData->m_W;
		pPro->m_pBitmapInfo->bmiHeader.biHeight = pData->m_H;
		YUYV_C_RGB( (BYTE*)pData->m_Block, pPro->m_pRGBTRANS, pData->m_H, pData->m_W);
	}
	
	long lStillWidth  = pPro->m_Rect.right - pPro->m_Rect.left;
	long lStillHeight = pPro->m_Rect.bottom - pPro->m_Rect.top;
	
	HDC hdc = ::GetDC( (HWND)(pPro->m_hWnd) );
	::SetStretchBltMode( hdc, COLORONCOLOR);
	::StretchDIBits(  
		hdc, 0, 0,  
		lStillWidth, lStillHeight,  
		0, 0,  
		pPro->m_pBitmapInfo->bmiHeader.biWidth, 
		pPro->m_pBitmapInfo->bmiHeader.biHeight, 
		pPro->m_pRGBTRANS,  
		pPro->m_pBitmapInfo,  
		DIB_RGB_COLORS,  
		SRCCOPY );
	::ReleaseDC( pPro->m_hWnd, hdc );
}


//接收视频数据线程
static DWORD WINAPI RecvVideoDataThread( void* p )
{
	char pTemp[ sizeof(SOCK_DATA_HEADER) + PACK_SIZE  ] = { 0 }; //缓冲大小必须不小于sizeof(SOCK_DATA_HEADER) + PACK_SIZE 
	
	ENDPNT* pEndPnt = (ENDPNT*) p;
	SOCKET sVUDPSck = pEndPnt->m_hSck;
	int nLen = 0;
	int nCmd = 0;
	int nRlt = 0;

	SOCKADDR_IN addrClient = {0};
	int len                = sizeof(SOCKADDR);
	char* pData            = NULL;
	DEVIMAGE* pDevImage    = NULL;
	VideoPro stPro         = {0};
	stPro.m_hWnd           = VideoSDK::g_hVideoWND;
	::GetWindowRect( (HWND)VideoSDK::g_hVideoWND, &(stPro.m_Rect) );//获取显示视频数据的窗口RECT

	typedef std::map<UINT32, char* > DATA_MAP_EX;
    DATA_MAP_EX mapDataEx;
	char* pBuffer = NULL;
	while( !VideoSDK::g_VideoClose )
	{

    //接收视频数据
		nLen = recvfrom( sVUDPSck,  pTemp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE, 0, (sockaddr*)&addrClient, &len );

		if ( nLen > 0 )
		{
			
			//获取包头
			SOCK_DATA_HEADER Head = *((SOCK_DATA_HEADER*)(pTemp));

			//校验
			if ( 0 != sock_checksum( (UINT16*) (&Head), sizeof(SOCK_DATA_HEADER) ) )
				continue;//校验出错,丢弃

			if (Head.msg_offset == 0)//第一个分包
			{
				//解析网络的数据包
				nCmd = ResolveNCMD( pTemp, nLen, nRlt );
				if ( nCmd == M_VIDEO_OUT )
				{
					;//第一个分包,命令头在第一个分包里面。
				}
				else
					continue;//丢弃，非法包
			}

			 //查找本次标识的数据接收缓冲
			 DATA_MAP_EX::iterator it  = mapDataEx.find( Head.msg_identify );
			 if ( it == mapDataEx.end() ) //没找到，则为新视频数据包。
			 {
				if ( Head.msg_offset != 0 )//检验是否丢包头，否则为无命令头的非法视频数据。
					continue;

				//为接收本次视频数据开辟一个快缓存，并插入到MAP映射表中
				int nSize = Head.msg_total+sizeof(SOCK_DATA_HEADER)+sizeof(UINT32)+4 ;
				pBuffer = new char[ nSize ];
				ZeroMemory( pBuffer, nSize );
				std::pair<DATA_MAP_EX::iterator, bool> IT =  mapDataEx.insert( std::pair< UINT32, char* >( Head.msg_identify, pBuffer ) );
				it = IT.first;
			 }
			 else
				  pBuffer =  it -> second;//找到本次视频数据包的数据接收缓存
				

			 assert( (long)pBuffer%4  == 0 );//验证缓存是否4字节对齐

			 //组包
			 char* pMem = pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32);
			 memcpy( pMem + Head.msg_offset, pTemp + sizeof(SOCK_DATA_HEADER), Head.msg_len );

			 //累积当前包的已接收的分包总长度
			 SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*) pBuffer;
			 pHdr->msg_total += Head.msg_len;

			 //判断组包是否完成，完成则播放
			 if ( pHdr->msg_total ==  Head.msg_total )
			 {
				NCMD* pNCM = (NCMD*) ( pBuffer + sizeof(SOCK_DATA_HEADER) + sizeof(UINT32) );
				pDevImage  = (DEVIMAGE*) ( pNCM + 1 );

				//显示视频
				ShowVideo( pDevImage, &stPro );

				//清除，当前已经播放的视频数据缓存
				delete [] pBuffer;
				mapDataEx.erase(it);

			 }

			 ZeroMemory( pTemp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE );
		}
		Sleep(1);
	}

	if(stPro.m_pRGBTMP)
		delete [] stPro.m_pRGBTMP;

	if (stPro.m_pRGBTRANS)
		delete [] stPro.m_pRGBTRANS;

	delete pEndPnt;

	VideoSDK::g_VideoClose = false;
	return 0;
}

VideoSDK::VideoSDK()
{
	m_sHost       = NULL;
	m_sVVIDEOSck  = NULL;
	m_strDevIP    = "";
	m_strClientIP = "";
	m_nTCPPort    = 0;
	m_nUDPPort    = 0;

}

VideoSDK::~VideoSDK()
{

}

//初始化
bool  VideoSDK::SDKInit(HWND hWnd, CString strDevIP, CString strClientIP, int nTCPPort)
{
	VideoSDK::g_hVideoWND  = hWnd;
	m_sHost				   = 0;
	m_sVVIDEOSck		   = 0;
	m_strDevIP			   = strDevIP;
	m_strClientIP		   = strClientIP;
	m_nTCPPort			   = nTCPPort;
		 
	return Init();
}

//与设备建立连接
bool   VideoSDK::SDKConnect()
{
  //与设备建立连接
  m_sHost = ConnectServer( m_strDevIP.GetBuffer(0), m_nTCPPort );

  if (!m_sHost) return false;

  //启动接收实时控制数据线程
  ENDPNT*  pEnPnt = new ENDPNT ;
  pEnPnt->m_hSck  = m_sHost;
  memcpy( pEnPnt->m_IP.m_ID, m_strDevIP.GetBuffer(0) , DEV_ID_LEN);
  g_hTCPThread = CreateThread( NULL, 0, RecvRealControlDataThread, (void*) pEnPnt,  NULL, NULL );


  //登陆设备，进行身份权限验证，设备初试登陆名和口令为：admin，201031
  if( !Login( m_sHost, m_strDevIP.GetBuffer(0), "admin", "201031" ) )
		return false;

  while(!g_Author)//等待权限验证成功（设备端的验证响应消息）
	 Sleep(50);

  return VideoSDK::g_Author;
}

//登陆设备
bool   VideoSDK::SDKLogin(CString sUserName, CString strPassword)
{
	//登陆设备，进行权限验证
	if( !Login( m_sHost, m_strDevIP.GetBuffer(0), sUserName.GetBuffer(0), strPassword.GetBuffer(0) ) )
		return false;

	while(!g_Author)//等待权限验证成功（设备端的验证响应消息）
	 Sleep(50);

    return VideoSDK::g_Author;
}

//开启视频
bool   VideoSDK::SDKStartVideo()
{
	if ( g_Author )//通过权限验证
	{
		//打开视频，包括启动视频数据接收线程
		m_sVVIDEOSck = StartVideo( m_sHost, m_strDevIP.GetBuffer(0), m_strClientIP.GetBuffer(0), m_nUDPPort);
		if (!m_sVVIDEOSck)
			return false;

		return true;
	}
	else
		return false;
}

//停止视频
bool   VideoSDK::SDKStopVideo()
{
	//关闭视频
	bool bRet = StopVideo( m_sHost, m_strDevIP.GetBuffer(0), m_strClientIP.GetBuffer(0), m_nUDPPort);
	if (!bRet)
		return false;

	//关闭视频UDP套接字
	if (m_sVVIDEOSck)
	{
		closesocket(m_sVVIDEOSck);
		m_sVVIDEOSck = NULL;
	}

	//关闭视频数据处理线程
	if(g_hUDPThread)
	{
		VideoSDK::g_VideoClose = true;
   		if( WaitForSingleObject( g_hUDPThread, 5000 ) != WAIT_OBJECT_0 )
			TerminateThread( g_hUDPThread, 0 );
		CloseHandle( g_hUDPThread );
		g_hUDPThread = NULL;
	}
	return true;
}

//断开连接
void   VideoSDK::SDKDisConnect()
{	
	//关闭连接
	DisConnectServer(m_sHost);

	//关闭实时控制数据线程
	if(g_hTCPThread)
	{
		VideoSDK::g_exitThread = true;
   		if( WaitForSingleObject( g_hTCPThread, 5000 ) != WAIT_OBJECT_0 )
			TerminateThread( g_hTCPThread, 0 );
		CloseHandle( g_hTCPThread );
		g_hTCPThread = NULL;
	}
	g_Author = false;
}