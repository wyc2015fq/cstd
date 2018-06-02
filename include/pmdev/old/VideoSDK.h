
//////////////////////////////////////////////////////////////////////

#ifndef _VIDEOSDK_H_
#define _VIDEOSDK_H_

#include <assert.h>
#include "net.h"
#include "thread.h"
#include "thread.h"


#define M_AUTH      0x07 //客户端身份验证
#define M_HEART     0x09 //心跳包
#define M_VIDEO_OUT 0x23 //UDP网络视频传输 NET_ADDRESS

//动作子命令
#define S_SET 0x81 //启动视频
#define S_GET 0x83 //关闭视频

#define F_REQ_FLAG    0x40000000 //请勿占用此位
#define F_REPLY_FLAG  0x20000000 //请勿占用此位

//用户验证
#define AUTH_VERIFY 0x00000100 //用户身份验证

#define PACK_SIZE (60*1024) //一个包为60大小

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))


//命令结构体，所有上传下载的网络数据,通过此数据结构来辨别数据类型
typedef struct {
  UINT32 m_MCmd; //主命令
  UINT32 m_SCmd; //子命令
  UINT32 m_CFlag; //特定命令配置参数，最后一个字节为命令执行状态字
  UINT32 m_Param1; //用户自定义参数1
  UINT32 m_Param2; //用户自定义参数2
} NCMD;


//Sock数据包头
typedef struct {
  UINT32 msg_identify; //标识
  UINT32 msg_offset ; //偏移
  UINT32 msg_len; //数据长度
  UINT32 msg_total; //数据总长度
  UINT32 msg_checksum; //检验和
  //  int8 msg_data[4]; //数据
} SOCK_DATA_HEADER;


typedef struct {
  unsigned short Port; //端口
  char IP_Address[16]; //点分十进制IP地址
} SOCK_ADDRESS, *PSOCK_ADDRESS;


#define DEV_ID_LEN 24
typedef struct {
  char m_ID[ DEV_ID_LEN ];
} DEV_CID; //ID标识结构体

//客户端身份验证
typedef struct {
  DEV_CID m_UserName; //用户名
  DEV_CID m_Password; //密码
} DEV_AUTH;

#define IMAGE_UYVY 0x01
#define IMAGE_YUYV 0x02
#define IMAGE_RGB24 0x03
typedef struct {
  //图像数据总大小BYTES
  INT32 m_Size;
  //图像宽
  INT32 m_W;
  //图像高
  INT32 m_H;
  //行数据步长
  INT32 m_Step;
  //图像类型
  char m_Type;
  //图像数据
  char m_Block[3];
} DEVIMAGE;

typedef int (*frame_fun_t)(int h, int w, const unsigned char* img, int step, int cn);

typedef struct VideoPro {
  DEV_CID m_IP;
  SOCKET m_sHost;//TCP套接字,和设备建立连接，对设备端的实时控制
  SOCKET m_sVVIDEOSck;//UDP套接字，接收设备发送过来的实时视频数据

  int h, w;
  int m_nIndex;//命令包序号
  int m_Author;//登陆设备权限验证通过标志
  int m_VideoClose;//关闭视频标志
  int m_exitThread;
  thread_t m_hTCPThread;//接收实时控制响应数据的线程
  thread_t m_hUDPThread;//接收实时视频数据的线程
  frame_fun_t m_framefun;
  void* userdata;
  //int m_nTCPPort;//TCP端口 13333
  int m_nUDPPort;//UDP端口,随机选择的可用端口
  char m_strDevIP[32];//设备IP
  char m_strClientIP[32];//本机IP
  int cn;
  int flipud;
  unsigned char* pRGBTMP;
  unsigned char* pRGBTRANS;
  unsigned char* buf;
  int run;
} VideoPro;

#define TCPPORT   13333
#define ERR_VERIFY_FAIL 0x06 //验证失败
#define SUC_EXECUT      0x00 //成功
#define GET_ERR( state )   ( (state) & 0xff )

//包头固定程度为20
#define OFFSET_SIZE 20

#endif // _VIDEOSDK_H_
