//#include "globalDef.h"
#include <string.h>
#ifndef debugPrint
#define debugPrint printf
#endif // debugPrint
class CNetS
{
public:
  SOCKET m_Socket;
  BOOL m_Verify;
  BOOL m_NetInitOK;
  BOOL m_Idle;
  addr_in m_MultAdress;
public:
  int Connect(SOCKET hSOCK , BOOL flag);
public:
  CNetS();
  ~CNetS();
public:
  static CNetS& Obj(void);
public:
  BOOL NetInit(int af);
  BOOL NetUinit(void);
  BOOL NetListen(BOOL flag);
  BOOL NetStart(BOOL flag);
  BOOL NetSend(const char* buf, unsigned int* lpLen) {
    return m_Verify && m_Socket && CPM_S_SendData(m_Socket , buf , lpLen);
  }
  BOOL NetDrSend(const char* buf, unsigned int* lpLen) {
    return m_Socket && CPM_S_SendData(m_Socket , buf , lpLen);
  }
  BOOL NetChangeMultAdress(const addr_in* addr);
  BOOL NetAddress(addr_in* addr);
  BOOL NetDisconnect(void);
  BOOL IsNetConnect(void);
  BOOL NetClientVerify(BOOL flag) {
    m_Verify = flag;
    return m_Verify;
  }
  BOOL VerifyOK(void) {
    return m_Verify;
  }
  void IdleMode(BOOL flag) {
    m_Idle = flag;
  }
};
void Reply(CNetS& netObj , NCMD* pCmd , char err);
#define SINGLE_OBJ_DEC( T ) static T& Obj( void ) { static T l_Obj; return l_Obj; } \
  void DoCmd( NCMD* pCmd , unsigned int len , CNetS& netObj ) { printf("m_MCmd = %d\n", pCmd->m_MCmd);\
    Reply( netObj , pCmd , 1 ? SUC_EXECUT : ERR_SET_FAIL ); }
typedef unsigned int uint;
class QMutex {};
//网络命令业务处理基类
class NetCmdHandle
{
public:
  virtual void DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj) = 0;
};
//客户端身份验证
class AuthCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(AuthCmd);
};
//用户管理
class UserCmd : public NetCmdHandle
{
  int m_TempUser[ 1024 * 40 ]; //90K+64K < 160K
protected:
  void RealTimeUserNotify(void* pUser);
  void RealTimeCapNotify(void* pUser);
protected:
  void DLUser(NCMD* pCmd , unsigned int len , CNetS& netObj);
  void ULUser(NCMD* pCmd , unsigned int len , CNetS& netObj);
public:
  SINGLE_OBJ_DEC(UserCmd);
};
//用户批量上传下载扩展接口
class CUserEx : public NetCmdHandle
{
  void UserDoBatchNotify(void* pCmd , uint len , void* pNetObj);
public:
  SINGLE_OBJ_DEC(CUserEx);
};
//记录管理
class RecordCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(RecordCmd);
};
//迭代起
class RangObjIter
{
protected:
  CNetS* m_pNet;
  NCMD* m_pCmd;
public:
  char* ObjMem(void) {
    return (char*)(m_pCmd + 1);
  }
  int Handle(char* pObj , int objSize , bool end , int param);
  void Setup(NCMD* pCmd , CNetS* pNet) {
    m_pCmd = pCmd;
    m_pNet = pNet;
  }
};
class RangExObjIter
{
protected:
  enum { R_RANGE_EX_SENDCOUNT = 50 };
  char* m_pStart;
  CARRAY* m_pArry;
public:
  char* ObjMem(void) {
    return m_pStart;
  }
  int Handle(char* pObj , int objSize , bool end , int param);
  void Setup(NCMD* pCmd , CNetS* pNet);
};
//区间下载记录
class CRange: public NetCmdHandle
{
protected:
  RangObjIter m_RangIter;
  RangExObjIter m_RangExIter;
public:
  SINGLE_OBJ_DEC(CRange);
};
//时间处理
class TimeCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(TimeCmd);
};
//心跳包
class HeartCmd : public NetCmdHandle
{
  CUtimer m_HeartVaild;
  CUtimer m_HeartTime;
  CUtimer m_BeatTime;
  bool m_Enable;
  enum { HEART_TIMEOUT = 30000 , HEART_VAILD = 10000 };
public:
  SINGLE_OBJ_DEC(HeartCmd);
  void HeartBeat(void);
  void Enable(bool flag) {
    m_HeartVaild.Restart();
    m_HeartTime.Restart();
    m_Enable = flag;
  }
};
//设备信息
class DevInfoCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(DevInfoCmd);
};
//统计信息
class StatisCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(StatisCmd);
};
//设备基本设置
class DevSetCmd : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(DevSetCmd);
};
//设备格式化
class QSplashScreen;
class DevFormat1 : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(DevFormat1);
  void LongTimeFormatTips(void);
  static bool FormatDevice(bool reset);
  QSplashScreen* m_pSplash;
private:
  CUtimer m_FormatTimer;
  bool m_bIsFormating;
  DevFormat1() : m_pSplash(NULL) , m_bIsFormating(false) {};
};
//时间组
class TimeGroup : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(TimeGroup);
};
//权限
class DspRight : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(DspRight);
};
//用户组
class LogicGroup : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(LogicGroup);
};
//紧急开门
class EOpen : public NetCmdHandle
{
public:
  int m_DoorState;
  bool m_GetOK;
public:
  EOpen() : m_DoorState(-1) , m_GetOK(false) {}
public:
  SINGLE_OBJ_DEC(EOpen);
  void SetDoorState(int state) {
    m_DoorState = state;
    m_GetOK = true;
  }
  int GetDoorState(void);
};
//远程开门
class ROpen : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(ROpen);
};
//多播地址设定
class MCast : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(MCast);
};
//网络程序更新
class AppUpdate : public NetCmdHandle
{
  void NotifyUpdate(void);
  void NotifyStandby(void*);
public:
  SINGLE_OBJ_DEC(AppUpdate);
  void DoUpdate(void) {
    NotifyUpdate();
  }
};
//网络视频传输
class CVideoOut : public NetCmdHandle
{
  addr_in m_CNetAdr;
  NCMD m_NCMD;
  SOCKET m_Socket;
  int m_Count;
  CUtimer m_Timer;
  QMutex m_SynSend;
protected:
  bool CreateUdp(NCMD* pCmd);
  bool DeleteUdp(void) {
    memset(&m_CNetAdr , 0 , sizeof(m_CNetAdr));
    memset(&m_NCMD , 0 , sizeof(m_NCMD));
    m_Count = 0;
    return true;
  }
public:
  CVideoOut() : m_Socket(0), m_Count(0) {
    memset(&m_CNetAdr , 0 , sizeof(m_CNetAdr));
    memset(&m_NCMD , 0 , sizeof(m_NCMD));
  }
  ~CVideoOut() {
    DeleteUdp();
  }
public:
  SINGLE_OBJ_DEC(CVideoOut);
  void StopSend(void) {
    DeleteUdp();
  }
  void SendVideo(int type , int w , int h , int ch , int step , char* pdata);
};
#define CMD_LEN 64 //控制命令长度
typedef struct {
  unsigned char m_Cmd[ CMD_LEN ];
} CMD;
typedef struct VerifyResult VerifyResult;
//IOCTRL
class CIOCtrl : public NetCmdHandle
{
protected:
  CIOCtrl() {}
public:
  void DoPortCmd(const CMD& cmd);
  bool SetMode(char source, char mode);
  int AnalyzeWG(const USER* pUser, const VerifyResult* pVerify);
  int DoAction(int mode, char source, const USER* pUser, const VerifyResult* pVerify);
public:
  SINGLE_OBJ_DEC(CIOCtrl);
};
//NETPRO
class CNetPro1 : public NetCmdHandle
{
public:
  SINGLE_OBJ_DEC(CNetPro1);
public:
  void Accept(NETPRO& pro , CNetS& netObj);
};
//网络命令工厂类
class NetCmdFactory
{
public:
  static NetCmdHandle* CreatCmdHandle(int mCmd) {
    switch (mCmd) {
    case M_USER:
      //处理管理端对DSP端用户操作
      return &UserCmd::Obj();
    case M_USER_EX:
      //用户批量上传下载
      return &CUserEx::Obj();
    case M_VERIFY:
    case M_ALARM:
    case M_OPTLOG:
    case M_CIMAGE:
      //下载指定类型记录的区间信息
      return &RecordCmd::Obj();
    case M_RANGE_EX:
    case M_RANGE:
      //区间数据下载
      return &CRange::Obj();
    case M_AUTH:
      //处理管理端身份验证
      return &AuthCmd::Obj();
    case M_TIME:
      //DSP端时间获得/设置
      return &TimeCmd::Obj();
    case M_HEART:
      //心跳包
      return &HeartCmd::Obj();
    case M_DEVINFO:
      //获得设备信息
      return &DevInfoCmd::Obj();
    case M_STATIS:
      //获得DSP端用户或者记录等统计信息
      return &StatisCmd::Obj();
    case M_DEVSET:
      //获得/设置DSP工作设置
      return &DevSetCmd::Obj();
    case M_FORMAT:
      //格式化设备
      return &DevFormat1::Obj();
    case M_TGROUP:
      //时间组
      return &TimeGroup::Obj();
    case M_DSPRIGHT:
      //权限
      return &DspRight::Obj();
    case M_UGROUP:
      //用户组
      return &LogicGroup::Obj();
    case M_DOOR_STATE:
      //紧急开门
      return &EOpen::Obj();
    case M_REMOTE_OPEN:
      //远程开关门
      return &ROpen::Obj();
    case M_MCAST:
      //多播地址
      return &MCast::Obj();
    case M_REBOOT:
      //设备重启
    case M_APPUPDATE:
      //网络程序更新
      return &AppUpdate::Obj();
    case M_VIDEO_OUT:
      //网络视频传输
      return &CVideoOut::Obj();
      //IO控制
    case M_IOCTRL:
      return &CIOCtrl::Obj();
      //网络属性设置
    case M_NETPRO:
      return &CNetPro1::Obj();
    default:
      break;
    }
    return NULL;
  }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAIN_THREAD_OK 0x01
#define COLOR_CAMERA_OK 0x02
#define IR_CAMERA_OK 0x04
#define COM_THREAD_OK 0x08
#define ALL_OK (MAIN_THREAD_OK|COLOR_CAMERA_OK|IR_CAMERA_OK|COM_THREAD_OK)
//看门狗
class WatchDog
{
  int m_DogFd;
  int m_DogType;
  int m_Flag;
  bool m_EnableDog;
  QMutex m_SynAlive;
  CUtimer l_irTImer;
  CUtimer l_comTimer;
  CUtimer l_guiTimer;
public:
  enum { SOFT_DOG = 0 , HARD_DOG = 1 };
  WatchDog();
  ~WatchDog();
public:
  SINGLE_OBJ_DEC(WatchDog);
  bool Start(int dogType = WatchDog::HARD_DOG);
  bool Stop(void);
  bool Alive(int flag);
  bool Enable(bool flag);
};
static BOOL InvalidID(const CID& id)
{
  return !id.m_ID[0];
}
static int l_MemBlock[ 256 * 1024 ] = { 0 }; //1M
void RecvData(SOCKET hSock , const char* buf, unsigned int len, void* param)
{
  assert(param != 0);
  assert(buf != 0);
  assert(len >= sizeof(NCMD));
  CNetS* pNetObj = (CNetS*)param;
  NCMD* pCmd = (NCMD*)buf;
  if (hSock != pNetObj->m_Socket) {
    return;
  }
  assert(pNetObj->m_Socket);
  // DEBUG_NETCMD( *pCmd , len );
  if (!pNetObj->m_Verify && (pCmd->m_MCmd != M_AUTH || !(pCmd->m_CFlag & AUTH_VERIFY))) {
    Reply(*pNetObj, pCmd, ERR_VERIFY_FAIL);
    return;
  }
  NetCmdHandle* pCmdHandle = NetCmdFactory::CreatCmdHandle(pCmd->m_MCmd);
  if (!pCmdHandle) {
    Reply(*pNetObj, pCmd, ERR_SUPPORT);
    debugPrint("net cmd = %x : ERR_SUPPORT" , pCmd->m_MCmd);
    return ;
  }
  if (pNetObj->m_Idle) {
    switch (pCmd->m_MCmd) {
    case M_AUTH:
    case M_HEART:
    case M_DEVINFO:
    case M_STATIS:
    case M_REMOTE_OPEN:
    case M_DOOR_STATE:
    case M_VIDEO_OUT:
    case M_IOCTRL:
      break;
    case M_USER:
      if (S_REALTIME_LOAD != pCmd->m_SCmd) {
        Reply(*pNetObj, pCmd, ERR_BUSY);
        return ;
      }
      break;
    case M_DEVSET:
      if (S_SET == pCmd->m_SCmd) {
        Reply(*pNetObj, pCmd, ERR_BUSY);
        return ;
      }
      break;
    case M_RANGE_EX:
    case M_RANGE: {
      RECORDRANGE* range = (RECORDRANGE*)(pCmd + 1);
      if (!InvalidID(range->m_SID)) {
        if (M_RANGE == pCmd->m_MCmd) {
          pCmd->m_MCmd = range->m_SID.m_ID[6] - '0';
        }
        else {
          pCmd->m_SCmd = range->m_SID.m_ID[6] - '0';
        }
      }
    }
    default: {
      Reply(*pNetObj, pCmd, ERR_BUSY);
      return ;
    }
    }
  }
  if (pCmd->m_MCmd != M_HEART) {
    HeartCmd::Obj().Enable(false);
  }
  switch (pCmd->m_MCmd) {
  case M_AUTH:
  case M_HEART:
  case M_FORMAT:
  case M_APPUPDATE:
  case M_REBOOT:
  case M_USER_EX:
    pCmdHandle->DoCmd(pCmd , len , *pNetObj);
    break;
  default:
    if (len < sizeof(l_MemBlock)) {
      memcpy((char*)l_MemBlock + OFFSET_SIZE , pCmd , len);
      pCmdHandle->DoCmd((NCMD*)((char*)l_MemBlock + OFFSET_SIZE) , len , *pNetObj);
    }
    break;
  }
  if (pCmd->m_MCmd != M_HEART && pCmd->m_MCmd != M_APPUPDATE && pCmd->m_MCmd != M_REBOOT) {
    HeartCmd::Obj().Enable(true);
  }
}
int ConnetcStatus(SOCKET hSock , BOOL flag , void* param)
{
  CNetS* pNetObj = (CNetS*)param;
  assert(pNetObj != 0);
  pNetObj->Connect(hSock , flag);
  return 0;
}
void Reply(CNetS& netObj, NCMD* pCmd, char err)
{
  assert(pCmd);
  int temp[ OFFSET_SIZE + sizeof(NCMD) ] = { 0 };
  NCMD* pBK = (NCMD*)((char*)temp + OFFSET_SIZE);
  memcpy(pBK , pCmd , sizeof(NCMD));
  SET_ERR(pBK->m_CFlag , err);
  pBK->m_CFlag |= CMD_STATE_REPLY;
  unsigned int len = sizeof(NCMD);
  netObj.NetDrSend((const char*)pBK , &len);
}
CNetS& CNetS::Obj(void)
{
  static CNetS l_Net;
  return l_Net;
}
CNetS::CNetS() : m_Socket(NULL) , m_Verify(false) , m_NetInitOK(false) , m_Idle(false)
{
  memset(&m_MultAdress , 0 , sizeof(m_MultAdress));
}
CNetS::~CNetS()
{
  NetUinit();
}
void NetCallBack(void* handle, const char* buf, unsigned int len, void* param)
{
  CNetS* pNetObj = (CNetS*)param;
  if (!pNetObj->IsNetConnect()) {
    return ;
  }
  BOOL rs = pNetObj->NetSend(buf, &len);
  if (handle) {
    *(BOOL*)(handle) = rs;
  }
}
BOOL CNetS::NetInit(int af)
{
  if (m_NetInitOK) {
    return true;
  }
  if (CPM_S_Init(af, 1024 * 1024 , SERVER_SELECTED)) {
    //CPMDB::SetCallBack( NetCallBack , this );
    CPM_S_SetRecvCallBack(RecvData , this);
    CPM_S_SetCNCallBack(ConnetcStatus , this);
    memset(&m_MultAdress , 0 , sizeof(m_MultAdress));
    //net_addr(MULTYGROUP_IP, MULTYGROUP_PORT, &m_MultAdress);
    net_addr(ANYIP(af), MULTYGROUP_PORT, &m_MultAdress);
    CPM_S_AddInMultyGroup(true , &m_MultAdress);
    m_NetInitOK = CPM_S_StartListen(true);
    return m_NetInitOK;
  }
  assert(false);
  return false;
}
BOOL CNetS::NetUinit(void)
{
  m_NetInitOK = false;
  NetDisconnect();
  CPM_S_StartListen(false);
  CPM_S_AddInMultyGroup(false , &m_MultAdress);
  CPM_S_SetRecvCallBack(0 , this);
  CPM_S_SetCNCallBack(0 , this);
  //CPMDB::SetCallBack( 0 , 0 );
  CPM_S_UnInit();
  m_Socket = NULL;
  return true;
}
#ifndef debugPrint
#define debugPrint printf
#endif // debugPrint
#define TEST_INFO printf("TEST_INFO : %s %d\n", __FILE__, __LINE__)
BOOL CNetS::NetListen(BOOL flag)
{
  debugPrint(flag ? "mult listen start" : "mult listen stop");
  BOOL ret = CPM_S_StartListen(flag);
  TEST_INFO;
  return ret;
}
BOOL CNetS::NetStart(BOOL flag)
{
  return CPM_S_StartWork(flag);
}
BOOL CNetS::NetChangeMultAdress(const addr_in* addr)
{
  assert(NULL != m_Socket);
  if (CPM_S_AddInMultyGroup(false , &m_MultAdress)) {
    if (CPM_S_AddInMultyGroup(true , addr)) {
      m_MultAdress = *addr;
      //memcpy( MULTYGROUP_IP , addr.IP_Address , 15 );
      //GlobalConfig::ConfigObj().SaveConfig();
      return true;
    }
    else {
      CPM_S_AddInMultyGroup(true , &m_MultAdress);
    }
  }
  return false;
}
BOOL CNetS::NetAddress(addr_in* addr)
{
  if (NULL != m_Socket && NULL != addr) {
    return CPM_S_Address(m_Socket , addr);
  }
  else {
    return false;
  }
}
BOOL CNetS::NetDisconnect(void)
{
  if (m_Socket != NULL) {
    if (!CPM_S_DCN(m_Socket)) {
      debugPrint("Clinet Net disconnect exception!!!");
    }
    else {
      debugPrint("Clinet Net disconnect OK!!!");
    }
  }
  return true;
}
BOOL CNetS::IsNetConnect(void)
{
  return m_Socket;
}
int CNetS::Connect(SOCKET hSock , BOOL flag)
{
  if (flag) {
    if (m_Socket) {
      if (m_Socket != hSock) {
        CPM_S_DCN(hSock);
      }
      return -1;
    }
    m_Socket = hSock;
    debugPrint("ConnetcStatus = client connect!!! , m_Socket = %d" , (int)m_Socket);
    //NetListen( false );
    HeartCmd::Obj().Enable(true);
    return 1;
  }
  else if (hSock == m_Socket) {
    debugPrint("m_Socket == %p , ConnetcStatus = Disconnect........", m_Socket);
    //
    CVideoOut::Obj().StopSend();
    //
    HeartCmd::Obj().Enable(false);
    m_Socket = NULL;
    m_Verify = false;
    //NetListen( true );
    return -1;
  }
  return 0;
}

