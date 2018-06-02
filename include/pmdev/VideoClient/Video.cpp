#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  

using namespace std;  

#define M_AUTH        0x07                //客户端身份验证
#define M_HEART       0x09               //心跳包
#define M_VIDEO_OUT   0x23               //UDP网络视频传输 NET_ADDRESS

//动作子命令
#define S_SET             0x81                //启动视频
#define S_DEL             0x82                //关闭视频

#define F_REQ_FLAG		0x40000000  //请勿占用此位
#define F_REPLY_FLAG	    0x20000000  //请勿占用此位
//用户验证
#define AUTH_VERIFY       0x00000100   //用户身份验证

#define PACK_SIZE (60*1024) //一个包为60大小

//命令结构体，所有上传下载的网络数据,通过此数据结构来辨别数据类型
typedef struct
{
	unsigned int m_MCmd;           //主命令
	unsigned int m_SCmd;           //子命令
	unsigned int m_CFlag;            //特定命令配置参数，最后一个字节为命令执行状态字
	unsigned int m_Param1;         //用户自定义参数1
	unsigned int m_Param2;         //用户自定义参数2
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
	int     m_Size;
	//图像宽
	int     m_W;
	//图像高
	int     m_H;
	//行数据步长
	int     m_Step;
	//图像类型
	char    m_Type;
	//图像数据
	char    m_Block[3];
}DEVIMAGE;

typedef struct  
{
	DEV_CID  m_IP;
	SOCKET   m_hSck;
}ENDPNT;

static int g_nIndex = 0;//命令包序号
static bool g_Author = false;//登陆设备权限验证通过标志
static bool g_VideoClose  = false ;//关闭视频标志

 
