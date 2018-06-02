// VideoSDK.h: interface for the VideoSDK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_)
#define AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"  
#include <iostream>  
#include <ASSERT.H>
#include "yuv2bmp.h"
#pragma comment(lib, "ws2_32.lib")  
#include   <map> 
using namespace std;  



#define M_AUTH        0x07                //客户端身份验证
#define M_HEART       0x09               //心跳包
#define M_VIDEO_OUT   0x23               //UDP网络视频传输 NET_ADDRESS

//动作子命令
#define S_SET           0x81                //启动视频
#define S_GET           0x83                      //关闭视频

#define F_REQ_FLAG		0x40000000  //请勿占用此位
#define F_REPLY_FLAG	0x20000000  //请勿占用此位

//用户验证
#define AUTH_VERIFY     0x00000100   //用户身份验证

#define PACK_SIZE (60*1024) //一个包为60大小

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1)) 


//命令结构体，所有上传下载的网络数据,通过此数据结构来辨别数据类型
typedef struct
{
	UINT32 m_MCmd;           //主命令
	UINT32 m_SCmd;           //子命令
	UINT32 m_CFlag;            //特定命令配置参数，最后一个字节为命令执行状态字
	UINT32 m_Param1;         //用户自定义参数1
	UINT32 m_Param2;         //用户自定义参数2
}NCMD;


//Sock数据包头
typedef struct 
{
	UINT32 msg_identify; //标识
	UINT32 msg_offset ;  //偏移
	UINT32 msg_len;      //数据长度
	UINT32 msg_total;    //数据总长度
	UINT32 msg_checksum; //检验和
	//	int8   msg_data[4];  //数据
}SOCK_DATA_HEADER;	


typedef struct
{
	unsigned short   Port;                //端口
	char           IP_Address[16];      //点分十进制IP地址
}SOCK_ADDRESS, *PSOCK_ADDRESS;


#define DEV_ID_LEN 24
typedef struct
{
	char m_ID[ DEV_ID_LEN ];
}DEV_CID; //ID标识结构体
//客户端身份验证
typedef struct
{
	DEV_CID m_UserName;        //用户名
	DEV_CID m_Password;        //密码
}DEV_AUTH;

#define IMAGE_UYVY   0x01
#define IMAGE_YUYV   0x02
#define IMAGE_RGB24  0x03
typedef struct
{
	//图像数据总大小BYTES
	INT32     m_Size;
	//图像宽
	INT32     m_W;
	//图像高
	INT32     m_H;
	//行数据步长
	INT32     m_Step;
	//图像类型
	char      m_Type;
	//图像数据
	char      m_Block[3];
}DEVIMAGE;

typedef struct  
{
	DEV_CID  m_IP;
	SOCKET   m_hSck;
}ENDPNT;

struct VideoPro
{
	BYTE* m_pRGBTMP;
	BYTE* m_pRGBTRANS;
	BITMAPINFO* m_pBitmapInfo;
	HWND  m_hWnd;
	RECT  m_Rect;
};


#define ERR_VERIFY_FAIL  0x06        //验证失败
#define SUC_EXECUT       0x00        //成功
#define GET_ERR( state ) ( (state) & 0xff )

class VideoSDK  
{
public:
	VideoSDK();
	virtual ~VideoSDK();

public:
	//初始化
	bool  SDKInit(HWND hWnd, CString strDevIP, CString strClientIP, int nTCPPort);


	//与设备建立连接
	bool   SDKConnect();

	//登陆设备
	bool   SDKLogin(CString sUserName, CString strPassword);

	//开启视频
	bool   SDKStartVideo();

	//停止视频
	bool   SDKStopVideo();

	//断开连接
	void   SDKDisConnect();

	//反初始化
    void   SDKUInit()
	{
		SDKStopVideo();//关闭视频
		SDKDisConnect();//关闭连接
		WSACleanup();//释放套接字资源  
	}
public:
	static int    g_nIndex;//命令包序号
	static bool   g_Author;//登陆设备权限验证通过标志
	static bool   g_VideoClose;//关闭视频标志
    static bool   g_exitThread;
	static HWND   g_hVideoWND;//播放视频的窗口句柄
	static HANDLE g_hTCPThread;//接收实时控制响应数据的线程
	static HANDLE g_hUDPThread;//接收实时视频数据的线程
private:
	SOCKET        m_sHost;//TCP套接字,和设备建立连接，对设备端的实时控制
    SOCKET        m_sVVIDEOSck;//UDP套接字，接收设备发送过来的实时视频数据

private:
	CString m_strDevIP;//设备IP
	CString m_strClientIP;//本机IP
	int m_nTCPPort;//TCP端口
	unsigned short m_nUDPPort;//UDP端口,随机选择的可用端口
};

#endif // !defined(AFX_VIDEOSDK_H__375BCE24_70DD_4474_93EE_2D2F8C629C3B__INCLUDED_)
