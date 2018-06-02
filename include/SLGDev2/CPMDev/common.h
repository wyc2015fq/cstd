#ifndef __COMMON__H__
#define __COMMON__H__
#include <map>
using namespace std;
//主命令
#define CMD_M_ID          0xA1
#define CMD_M_AUTH          M_AUTH
#define CMD_M_HEART         M_HEART
#define CMD_M_DEVINFO       M_DEVINFO
#define CMD_M_STATIS        M_STATIS
#define CMD_M_DEVSET        M_DEVSET
#define CMD_M_TIME          M_TIME
#define CMD_M_FORMAT        M_FORMAT
#define CMD_M_EOPEN         M_DOOR_STATE
#define CMD_M_REMOTE_OPEN   M_REMOTE_OPEN
#define CMD_M_NOPEN         M_NOPEN
#define CMD_M_USER          M_USER
#define CMD_M_VERIFY        M_VERIFY
#define CMD_M_ALARM         M_ALARM
#define CMD_M_OPTLOG        M_OPTLOG
#define CMD_M_CIMAGE        M_CIMAGE
#define CMD_M_RANGE         M_RANGE
#define CMD_M_TGROUP        M_TGROUP
#define CMD_M_DSPRIGHT      M_DSPRIGHT
#define CMD_M_UGROUP        M_UGROUP
#define CMD_M_MCAST       M_MCAST
#define CMD_M_APPUPDATE     M_APPUPDATE
#define CMD_M_VIDEO_OUT     M_VIDEO_OUT
#define CMD_M_USER_EX     M_USER_EX
#define CMD_M_RECORD_EX     M_RANGE_EX
#define CMD_M_REBOOT      M_REBOOT
#define CMD_M_IOCTRL        M_IOCTRL
#define CMD_M_NETPRO        M_NETPRO
#define CMD_M_RESETSCREEN   M_RESETSCREEN
//子命令
#define CMD_S_SET           S_SET                   //上传\设置
#define CMD_S_DEL           S_DEL                   //删除
#define CMD_S_GET           S_GET                   //下载\获得
#define CMD_S_REALTIME_SEND     S_REALTIME_SEND         //采集实时发送
#define CMD_S_REALTIME_LOAD     S_REALTIME_LOAD         //实时加载
#define CMD_S_REALTIME_CAP      S_REALTIME_CAP          //实时用户采集
#define CMD_S_REALTIME_CAP_INFO S_REALTIME_CAP_INFO     //远程采集实是进度信息
#define CMD_S_UPDATE_SEND     S_UPDATE_SEND           //更新实时发送
#define CMD_S_CLEAR_INOUT       S_CLEAR_INOUT           //清空统计计数
//状态标记位
#define F_ERR_BUSY          ERR_BUSY
#define F_ERR_LIMIT         ERR_LIMIT
#define F_ERR_NOFIND        ERR_NOFIND
#define F_ERR_SAVE_FAIL     ERR_SAVE_FAIL
#define F_ERR_SET_FAIL      ERR_SET_FAIL
#define F_ERR_VERIFY_FAIL   ERR_VERIFY_FAIL
//其他标记位
#define F_DL_ID       CMD_DOWN_ID     //下载ID集合
#define F_DL_ID_ALL     CMD_ID_ALL      //下载所有数据的ID集合
#define F_ID_TIME_RANGE     CMD_ID_TIME_RANGE   //下载制定时间段ID集合
#define F_ID_RANGE      F_ID_TIME_RANGE<<1
#define F_AUTH_VERIFY   AUTH_VERIFY     //用户身份验证
#define F_DL_PIC      DL_IMAGE      //需要下载照片
#define F_DL_FEAT     DL_FEAT       //需要下载人脸特征
#define F_DL_APPENDID   DL_APPENDID     //远程加在时，m_ID表示卡号
#define F_DL_NORMAL_TG    DL_NORMAL_TG    //下载正常时间组
#define F_DL_HOLIDAY_TG   DL_HOLIDAY_TG   //下载节假日
#define F_DOOR_CLOSE    DOOR_CLOSE        //紧急关门，否则为紧急开门
#define F_DOOR_OPEN     DOOR_OPEN       //长开或者长关状态取消
#define F_G_ORDER       G_ORDER             //组合开门有序
#define F_DEFAULT_NO_GROUP  DEFAULT_NO_GROUP
#define TIMEGROUP_NUMS      TGROUP_NUM        //时间组个数
#define USER_COMBINS        MAX_GROUPUSER     //用户组合个数
#define USER_JPEG_SIZE      (64*1024)
#define REC_JPEG_SIZE     (18*1024)
#define FEAT_SIZE       (90*1024)
#define OFFSET_SIZE          20
//#define TRANS_SERVER
#ifndef TRANS_SERVER
#define CMD_S_CONNECT_CUT       DEV_CONNECT_CUT     //断开连接
#define CMD_S_CONNECT_SUCCESS   DEV_CONNECT_SUCCESS   //连接成功
#define CMD_S_CONNECT_FAILUE    DEV_CONNECT_FAILUE    //连接失败
#define CMD_S_CONNECT_NONE      DEV_CONNECT_NONE    //未连接
#define CMD_S_CONNECT_REQ   DEV_CONNECT_NONE+1    //请求连接  
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1   //请求断开连接
#define E_CONNECT_STATUS    DEV_CONNECT_STATUS
#else
#define CMD_S_CONNECT_CUT       NET_CONNECT_CUT     //断开连接
#define CMD_S_CONNECT_SUCCESS   NET_CONNECT_SUCCESS   //连接成功
#define CMD_S_CONNECT_FAILUE    NET_CONNECT_FAILUE    //连接失败
#define CMD_S_CONNECT_NONE      NET_CONNECT_NONE    //未连接
#define CMD_S_CONNECT_REQ   NET_CONNECT_NONE+1    //请求连接  
#define CMD_S_CONNECT_REQ_CUT CMD_S_CONNECT_REQ+1   //请求断开连接
#define E_CONNECT_STATUS    NET_CONNECT_STATUS
#endif
#ifndef TRANS_SERVER
typedef struct {
  DEV_CID m_UserName;        //用户名
  DEV_CID m_Password;        //密码
} DEV_AUTH;
typedef enum {
  DL_SINGLE = 0x01,
  DL_ALL,
  DL_SEGTIME,
  DL_RANGE
} DATA_DL_TYPE;
//获得指定时间段的ID集合
typedef struct {
  DEV_DATE m_Start;  //记录起始时间
  DEV_DATE m_End;    //记录结束时间
} DEV_TIMESEG;
typedef struct {
  DATA_DL_TYPE  m_DLType;
  int       m_DLFlag;
  char          m_DLContent[4];
} DEV_DATA_DL;
#endif
#endif

