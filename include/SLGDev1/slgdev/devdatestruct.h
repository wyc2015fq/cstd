
#include "./dspdatestruct.h"

#undef GET_ERR
#define GET_ERR( state )   (DEV_OPER_RESULT)( (state) & 0xff )
//说明: M表示主命, 区间0x01 - 0x80, S表示子命令, 区间0x81-0xff

#define M_WPA_SET                0x16

#if 1

#define CMD_M_USER               0x01            //用户
#define CMD_M_VERIFY             0x02            //验证记录
#define CMD_M_ALARM              0x03            //报警记录
#define CMD_M_OPTLOG             0x04            //日志记录
#define CMD_M_CIMAGE             0x05            //变更注册照记录
#define CMD_M_RANGE              0x06            //区间集合数据下载
#define CMD_M_AUTH               0x07            //客户端身份验证
#define CMD_M_TIME               0x08            //系统时间
#define CMD_M_HEART              0x09            //心跳包
#define CMD_M_DEVINFO            0x0a            //设备基本信息
#define CMD_M_STATIS             0x0b            //统计信息
#define CMD_M_DEVSET             0x0c            //设备基本设置
#define CMD_M_FORMAT             0x0d            //设备格式化
#define CMD_M_DOOR_STATE         0x0e            //门状态设置 ,共3种状态：0正常状态 ， 1常开状态， 2常闭状态 CMD_M_EOPEN
#define CMD_M_REMOTE_OPEN        0x0f            //远程开关门,
#define CMD_M_TGROUP             0x10            //时间组
#define CMD_M_DSPRIGHT           0x11            //权限
#define CMD_M_UGROUP             0x12            //用户组
#define CMD_M_MCAST              0x13            //多播地址
#define CMD_M_APPUPDATE          0x14            //程序更新
#define CMD_M_IMGCHANGE          0x15            //待机界面图片更新
#define CMD_M_VIDEO_OUT          0x23   //UDP网络视频传输 NET_ADDRESS
#define CMD_M_USER_EX            0x24   //批量用户上传现在
#define CMD_M_REBOOT             0x25   //设备重启
#define CMD_M_RANGE_EX          0x26   //记录批量下载 CMD_M_RANGE_EX
#define CMD_M_SET_IP             0x27   //远程修改IP地址
#define CMD_M_NOPEN              0x28   //N+1开门请求
#define CMD_M_IOCTRL             0x29   //IO控制
#define CMD_M_NETPRO             0x30   //网络设置
#define CMD_M_RESETSCREEN        0x32   //屏幕校验
#define CMD_M_ID                 0xA1
#else
#define CMD_M_USER          M_USER                         //用户
#define CMD_M_VERIFY        M_VERIFY                       //验证记录
#define CMD_M_ALARM         M_ALARM                        //报警记录
#define CMD_M_OPTLOG        M_OPTLOG                       //日志记录
#define CMD_M_CIMAGE        M_CIMAGE                       //变更注册照记录
#define CMD_M_RANGE         M_RANGE                        //区间集合数据下载
#define CMD_M_AUTH          M_AUTH                         //客户端身份验证
#define CMD_M_TIME          M_TIME                         //系统时间
#define CMD_M_HEART         M_HEART                        //心跳包
#define CMD_M_DEVINFO       M_DEVINFO                      //设备基本信息
#define CMD_M_STATIS        M_STATIS                       //统计信息
#define CMD_M_DEVSET        M_DEVSET                       //设备基本设置
#define CMD_M_FORMAT        M_FORMAT                       //设备格式化
#define CMD_M_DOOR_STATE    M_DOOR_STATE                   //门状态设置 ,共3种状态：0正常状态 ， 1常开状态， 2常闭状态 CMD_M_EOPEN
#define CMD_M_REMOTE_OPEN   M_REMOTE_OPEN                  //远程开关门,
#define CMD_M_TGROUP        M_TGROUP                       //时间组
#define CMD_M_DSPRIGHT      M_DSPRIGHT                     //权限
#define CMD_M_UGROUP        M_UGROUP                       //用户组
#define CMD_M_MCAST         M_MCAST                        //多播地址
#define CMD_M_APPUPDATE     M_APPUPDATE                    //程序更新
#define CMD_M_IMGCHANGE     M_IMGCHANGE                    //待机界面图片更新
#define CMD_M_VIDEO_OUT     M_VIDEO_OUT           //UDP网络视频传输 NET_ADDRESS
#define CMD_M_USER_EX       M_USER_EX             //批量用户上传现在
#define CMD_M_REBOOT        M_REBOOT              //设备重启
#define CMD_M_RANGE_EX      M_RANGE_EX           //记录批量下载 CMD_M_RANGE_EX
#define CMD_M_SET_IP        M_SET_IP              //远程修改IP地址
#define CMD_M_NOPEN         M_NOPEN               //N+1开门请求
#define CMD_M_IOCTRL        M_IOCTRL              //IO控制
#define CMD_M_NETPRO        M_NETPRO              //网络设置
#define CMD_M_RESETSCREEN   M_RESETSCREEN         //屏幕校验
#define CMD_M_ID            M_ID

#endif
#define CMD_S_SET                0x81          //上传\设置
#define CMD_S_DEL                0x82          //删除
#define CMD_S_GET                0x83          //下载\获得
#define CMD_S_REALTIME_SEND      0x84          //采集实时发送
#define CMD_S_REALTIME_LOAD      0x85          //实时加载
#define CMD_S_REALTIME_CAP       0x86          //实时用户采集
#define CMD_S_REALTIME_CAP_INFO  0x88          //远程采集实是进度信息
#define CMD_S_UPDATE_SEND        0x87          //更新实时发送
#define CMD_S_CLEAR_INOUT        0x88          //清空统计计数

