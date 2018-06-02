#include "netcmdhandle.h"
void DownLoadID(NCMD* pCmd, uint len, uint type, CNetS& netObj)
{
#ifdef DEBUG_INFO
  CUtimer timer;
#endif
  CARRAY* pArry = (CARRAY*)(pCmd + 1);
  int count = PGLOABLDB->GetIDS(*pArry, *pCmd, type);
  if (0 == count) {
    pArry->m_Count = 0;
    SET_ERR(pCmd->m_CFlag , ERR_NOFIND);
  }
  len = sizeof(NCMD) + sizeof(CARRAY) + (pArry->m_Count) * sizeof(RECORDRANGE);
  netObj.NetSend((const char*)pCmd , &len);
#ifdef DEBUG_INFO
  debugPrint("DownLoadID Main type = %x count = %d timer.Elapsed() == %ld\r\n" , type , pArry->m_Count, timer.Elapsed());
  assert(pCmd->m_MCmd != M_USER);
  const RECORDRANGE* pStart = (RECORDRANGE*)pArry->m_Block;
  for (int i = 0 ; i < pArry->m_Count ; ++i) {
    debugPrint("pStart[%d].m_SID == %s , count = %d , flag = %x", i , pStart[i].m_SID.m_ID , pStart[i].m_Count , pStart[i].m_Flag);
  }
#endif
}
//客户端身份验证
void AuthCmd::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_AUTH);
  AUTH* pAtuh = (AUTH*)(pCmd + 1);
  debugPrint("Client Auth information UserName = %s , Password = %s" , pAtuh->m_UserName.m_ID , pAtuh->m_Password.m_ID);
  if (pCmd->m_CFlag & AUTH_VERIFY) {
    bool ret = ((NETCOUNT.m_UserName == pAtuh->m_UserName) && (NETCOUNT.m_Password == pAtuh->m_Password));
    if (!netObj.VerifyOK()) {
      netObj.NetClientVerify(ret);
    }
    Reply(netObj , pCmd , ret ? SUC_EXECUT : ERR_VERIFY_FAIL);
    if (!netObj.VerifyOK() && !ret) {
      WAIT_SLEEP(200);
      netObj.NetDisconnect();
    }
    debugPrint(netObj.VerifyOK() ? "S_VERIFY_AUTH OK" : "S_VERIFY_AUTH NO");
  }
  else {
    assert(netObj.VerifyOK());
    NETCOUNT.m_UserName = pAtuh->m_UserName;
    NETCOUNT.m_Password = pAtuh->m_Password;
    GlobalConfig::ConfigObj().SaveConfig();
    Reply(netObj , pCmd , SUC_EXECUT);
  }
}
//用户管理
void UserCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  debugPrint("Enter UserCmd::DoCmd %x , %x" , pCmd->m_MCmd , pCmd->m_CFlag);
  assert(pCmd->m_MCmd == M_USER);
  if (pCmd->m_CFlag & CMD_DOWN_ID) {
    //下载用户ID集合
    CUserTxt::Obj().BatchGet(*((CARRAY*)(pCmd + 1)) , *pCmd , len , netObj);
    return ;
  }
  switch (pCmd->m_SCmd) {
  case S_SET:
  case S_DEL:
    ULUser(pCmd , len , netObj);
    break;
  case S_GET:
    DLUser(pCmd , len , netObj);
    break;
  case S_REALTIME_LOAD:
    if (len < sizeof(m_TempUser)) {
      char source = (pCmd->m_Param1 & 0xFF);
      if (FRONTFACEDOOR(source)) {
        // 人脸前端出发的远程加在，交由GUI线程处理
        memcpy(m_TempUser , pCmd , len);
        emit RealTimeUserNotify(m_TempUser);
        return;
      }
      // 非人脸前端刷卡, 获得远程触发信号属性
      USER* pUser = (USER*)(pCmd + 1);
      CorrectUser(*pUser);
      bool card = (pCmd->m_CFlag & DL_APPENDID);
      char verify = (VERIFY_REMOTE | (card ? VERIFY_CARD : VERIFY_USERID));
      if (card && g_pMenu->m_CamImgShow && !InvalidID(pUser->m_UserID) && !InvalidID(pUser->m_AppendID)) {
        // 用户合法性判断
        int state = g_pMenu->m_CamImgShow->UserState(*pUser , verify);
        if (state) {
          SynSaveRecord(state , source , NULL , NULL , pUser);
          return;
        }
        VerifyResult tmpFr = { 0 };
        tmpFr.m_MatchOk = true;
        tmpFr.m_VerifyMode = verify;
        tmpFr.m_PID = pUser->m_UserID;
        g_pMenu->m_CamImgShow->RightJudge(*pUser , &tmpFr , source);
      }
    }
    break;
  case S_REALTIME_CAP:
    if (len < sizeof(m_TempUser)) {
      memcpy(m_TempUser , pCmd , len);
      emit RealTimeCapNotify(m_TempUser);
    }
    break;
  default:
    assert(false);
  }
}
// 下载用户数据
void UserCmd::DLUser(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(len == sizeof(NCMD) + sizeof(CID));
  USER* pUser = (USER*)(pCmd + 1);
  CID* pUserID = (CID*)(pCmd + 1);
  if (CUserTxt::Obj().Get(*pUserID , *pUser , pCmd->m_CFlag)) {
    len = sizeof(NCMD) + USER_STEP(*pUser);
  }
  else {
    SET_ERR(pCmd->m_CFlag , ERR_NOFIND);
  }
  netObj.NetSend((const char*)pCmd , &len);
  debugPrint("DL_USER ID = %s send bytes = %d" , pUser->m_UserID.m_ID , len);
}
// 上传用户数据
void UserCmd::ULUser(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  PGLOABLDB->DspDBTransaction(10 * 1000 , 10000);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    USER* pUser = (USER*)(pCmd + 1);
    debugPrint("S_ULUSER_ADD ID = %s , rec bytes = %d" , pUser->m_UserID.m_ID , len);
    CorrectUser(*pUser);
    CID userID = pUser->m_UserID;
    assert(!InvalidID(userID));
    assert(len >= sizeof(NCMD) + sizeof(USER) + pUser->m_PicBytes + pUser->m_FeatBytes);
    if (!CUserTxt::Obj().OnSynAdd(1, 1 , ACT_UPLOAD | FROM_NET, pCmd , pUser , this)) {
      debugPrint("AddUser ERR_LIMIT limit = %d" , CUserTxt::Obj().Size());
      SET_ERR(pCmd->m_CFlag , ERR_LIMIT);
    }
    *((CID*)(pCmd + 1)) = userID;
    len = sizeof(NCMD) + sizeof(CID);
    netObj.NetSend((const char*)pCmd , &len);
  }
  break;
  case S_DEL: {
    CID* pID = (CID*)(pCmd + 1);
    assert(len == sizeof(NCMD) + sizeof(CID));
    assert(!InvalidID(*pID));
    debugPrint("S_ULUSER_DEL ID = %s , rec bytes = %d" , pID->m_ID , len);
    CUserTxt::Obj().Del(pID->m_ID , "");
    SET_ERR(pCmd->m_CFlag , SUC_EXECUT);
    netObj.NetSend((const char*)pCmd , &len);
  }
  break;
  default:
    assert(false);
  }
}
//用户批量上传下载扩展接口
void CUserEx::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  emit UserDoBatchNotify(pCmd , len , &netObj);
}
//记录管理
void RecordCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  //下载记录ID集合
  assert(pCmd->m_CFlag & CMD_DOWN_ID);
  DownLoadID(pCmd, len, pCmd->m_MCmd, netObj);
}
//区间迭代
int RangObjIter::Handle(char* pObj , int objSize , bool end , int param)
{
  debugPrint("%s , objSize = %d , end = %d" , __FUNCTION__ , objSize , end);
  if (!pObj || !objSize) {
    return 0;
  }
  unsigned int len = sizeof(NCMD) + objSize;
  return m_pNet->NetSend((const char*)m_pCmd , &len) ? 1 : -1;
}
//扩展区间迭代
int RangExObjIter::Handle(char* pObj , int objSize , bool end , int param)
{
  debugPrint("%s , objSize = %d , count = %d , end = %d" , __FUNCTION__ , objSize , m_pArry->m_Count , end);
  if (pObj && objSize) {
    ++m_pArry->m_Count;
    m_pStart = pObj + objSize;
  }
  if (m_pArry->m_Count >= R_RANGE_EX_SENDCOUNT || (end && m_pArry->m_Count > 0)) {
    if (end) {
      m_pCmd->m_CFlag |= BATCH_END;
    }
    unsigned int len = sizeof(NCMD) + sizeof(CARRAY) + (m_pStart - m_pArry->m_Block);
    if (m_pNet->NetSend((const char*)m_pCmd , &len)) {
      int count = m_pArry->m_Count;
      m_pStart = m_pArry->m_Block;
      m_pArry->m_Count = 0;
      return count;
    }
    else {
      return -1;
    }
  }
  return 0;
}
void RangExObjIter::Setup(NCMD* pCmd , CNetS* pNet)
{
  m_pCmd = pCmd;
  m_pNet = pNet;
  m_pArry = (CARRAY*)(m_pCmd + 1);
  m_pStart = m_pArry->m_Block;
  m_pArry->m_Count = 0;
}
//区间下载记录
void CRange::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  RECORDRANGE range = *((RECORDRANGE*)(pCmd + 1));
  QString table = range.m_SID.m_ID;
  int type = table.mid(6 , 1).toInt();
  bool inputOk = true;
  //下载参数解析配置
  if (M_RANGE == pCmd->m_MCmd) {
    switch (type) {
    case M_VERIFY:
    case M_ALARM:
    case M_OPTLOG:
    case M_CIMAGE:
      pCmd->m_MCmd = type;
      break;
    default:
      inputOk = false;
      break;
    }
  }
  else if (M_RANGE_EX == pCmd->m_MCmd) {
    switch (pCmd->m_SCmd) {
    case M_VERIFY:
    case M_ALARM:
    case M_OPTLOG:
    case M_CIMAGE:
      type = pCmd->m_SCmd;
      break;
    default:
      switch (type) {
      case M_VERIFY:
      case M_ALARM:
      case M_OPTLOG:
      case M_CIMAGE:
        pCmd->m_SCmd = type;
        break;
      default:
        inputOk = false;
        break;
      }
      break;
    }
  }
  else {
    inputOk = false;
  }
  if (!inputOk) {
    debugPrint("ERR SN = %s , type = %x pCmd->m_MCmd = %x, pCmd->m_SCmd = %x", range.m_SID.m_ID, type, pCmd->m_MCmd, pCmd->m_SCmd);
    Reply(netObj , pCmd , ERR_INPUTDATA);
    assert(false);
    return ;
  }
  //下载区间集合
  if (pCmd->m_CFlag & CMD_DOWN_ID) {
    if (!(pCmd->m_CFlag & (CMD_ID_ALL | CMD_ID_TIME_RANGE | CMD_ID_RANGE))) {
      //没有?魏蜗略刂甘荆付ㄎ湎略?
      assert(false);
      pCmd->m_CFlag |= CMD_ID_RANGE;
    }
    DownLoadID(pCmd, len, type, netObj);
    return;
  }
  //下载区间记录
#ifdef DEBUG_INFO
  CUtimer timer;
#endif
  debugPrint("DL SN = %s , type = %x pCmd->m_MCmd = %x , pCmd->m_SCmd = %x", range.m_SID.m_ID, type, pCmd->m_MCmd , pCmd->m_SCmd);
  ObjIter* pObjIter = NULL;
  if (M_RANGE_EX == pCmd->m_MCmd) {
    m_RangExIter.Setup(pCmd , &netObj);
    pObjIter = &m_RangExIter;
  }
  else {
    m_RangIter.Setup(pCmd , &netObj);
    pObjIter = &m_RangIter;
  }
  int nSendCount = 0;
  assert(range.m_Count > 0);
  switch (type) {
  case M_VERIFY:
    nSendCount = PGLOABLDB->SendRecord(table.mid(7 , 8) , range , pCmd , pObjIter , 0);
    break;
  case M_ALARM:
    nSendCount = PGLOABLDB->SendRecord(QString("T_ALARM") , range , pCmd , pObjIter , 0);
    break;
  case M_OPTLOG:
    nSendCount = PGLOABLDB->SendOptLog(QString("T_OPTLOG") , range , pCmd , pObjIter , 0);
    break;
  case M_CIMAGE:
    nSendCount = PGLOABLDB->SendCGImg(QString("T_CGIMG"), range , pCmd , pObjIter , 0);
    break;
  default:
    assert(false);
  }
  if (!nSendCount) {
    SET_ERR(pCmd->m_CFlag , ERR_NOFIND);
    len = sizeof(NCMD);
    netObj.NetSend((const char*)pCmd , &len);
  }
  debugPrint("%s downLoad end , RID = %s , range.m_Count = %d , SendCount = %d , timer = %d" ,
      table.toStdString().c_str() , range.m_SID.m_ID , range.m_Count , nSendCount , timer.Elapsed());
}
//时间处理
void TimeCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_TIME);
  DATETIME* pTime = (DATETIME*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_GET: {
    memset(pTime , 0 , sizeof(DATETIME));
    QDateTime from = QDateTime::currentDateTime();
    QDateTime2NetTime(from , (*pTime));
    len = sizeof(NCMD) + sizeof(DATETIME);
    netObj.NetSend((const char*)pCmd , &len);
    assert(len == sizeof(NCMD) + sizeof(DATETIME));
    debugPrint("TimeCmd::DoCmd S_GET_TIME");
  }
  break;
  case S_SET: {
    Reply(netObj , pCmd , SetSysDateTime(*pTime) ? SUC_EXECUT : ERR_SET_FAIL);
    debugPrint("TimeCmd::DoCmd S_SET_TIME");
  }
  break;
  default:
    assert(false);
  }
}
//心跳包处理
void HeartCmd::DoCmd(NCMD* , unsigned int , CNetS&)
{
  QMutexLocker lock(&m_SynHeart);
  //debugPrint( "HeartBeat recv time = %d" , m_HeartTime.Elapsed() );
  m_HeartTime.Restart();
}
void HeartCmd::Enable(bool flag)
{
  QMutexLocker lock(&m_SynHeart);
  m_HeartVaild.Restart();
  m_HeartTime.Restart();
  m_Enable = flag;
}
void HeartCmd::HeartBeat(void)
{
  if (m_BeatTime.Elapsed() < 8000) {
    //心跳包间隔8s
    return;
  }
  m_BeatTime.Restart();
  m_SynHeart.lock();
  if (!CNetS::Obj().IsNetConnect() || m_HeartVaild.Elapsed() < HEART_VAILD) {
    m_HeartTime.Restart();
    m_SynHeart.unlock();
    return;
  }
  if (m_Enable && m_HeartTime.Elapsed() > HEART_TIMEOUT) {
    //HEART_TIMEOUT秒都没有收到心跳包认为网络物理连接已经断开
    debugPrint("Heart beat timeout %ld , NetDisconnect , [%s]", m_HeartTime.Elapsed(), QDateTime::currentDateTime().toString(DATETIME_FORMAT).toStdString().c_str());
    m_SynHeart.unlock();
    CNetS::Obj().NetDisconnect();
    return;
  }
  m_SynHeart.unlock();
  int temp[OFFSET_SIZE + sizeof(NCMD)] = { 0 };
  NCMD* pBK = (NCMD*)((char*)temp + OFFSET_SIZE);
  pBK->m_MCmd = M_HEART;
  unsigned int len = sizeof(NCMD);
  if (!CNetS::Obj().NetSend((const char*)pBK , &len)) {
    //网络发送失败 认为连接已经断开
    debugPrint("Heart beat send faild NetDisconnect!!!!");
    CNetS::Obj().NetDisconnect();
    return;
  }
  //debugPrint( "HeartBeat send time = %d" , m_HeartTime.Elapsed() );
}
//设备信息
void DevInfoCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_DEVINFO);
  DEVINFO* pDevInfo = (DEVINFO*)(pCmd + 1);
  memset(pDevInfo , 0 , sizeof(DEVINFO));
  pDevInfo->m_DevType = 0x01;//PGLOABLINI->m_Property;
  pDevInfo->m_LimitUser = PGLOABLINI->max_man_num;
  pDevInfo->m_Limit1NUser = PGLOABLINI->max_1N_num;
  pDevInfo->m_SN = GETDEVSID;
  pDevInfo->m_Ver = GETSOFTVER;
  DiskInfo diskInfo = {0};
  GetDiskInfo(((QDir::currentPath() + "/" + SDMOUNT_DIR).toStdString().c_str()) , &diskInfo);
  strncpy(pDevInfo->m_Space.m_ID , diskInfo.m_StrAvailableSize , ID_LEN - 1);
  len = sizeof(NCMD) + sizeof(DEVINFO);
  netObj.NetSend((const char*)pCmd , &len);
  assert(len == sizeof(NCMD) + sizeof(DEVINFO));
  debugPrint("DevInfoCmd::DoCmd M_DEVINFO Limit = %d , Limit1N = %d" , pDevInfo->m_LimitUser , pDevInfo->m_Limit1NUser);
}
//统计信息
void StatisCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_STATIS);
  DEVSTATIS* pStatis = (DEVSTATIS*)(pCmd + 1);
  memset(pStatis , 0 , sizeof(DEVSTATIS));
  PGLOABLDB->GetStatis(*pStatis);
  len = sizeof(NCMD) + sizeof(DEVSTATIS);
  netObj.NetSend((const char*)pCmd , &len);
  assert(len == sizeof(NCMD) + sizeof(DEVSTATIS));
  debugPrint("StatisCmd::DoCmd TotalUser = %d , TotalRecord = %d" , pStatis->m_TotalUsers , pStatis->m_TotalRecords);
}
//设备基本设置
void DevSetCmd::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_DEVSET);
  DEVSET* pSet = (DEVSET*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_GET: {
    *pSet = GETDEVSET;
    len = sizeof(NCMD) + sizeof(DEVSET);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("DevSetCmd::DoCmd S_GET_DEVSET");
  }
  break;
  case S_SET: {
    debugPrint("DevSetCmd::DoCmd S_SET_DEVSET");
    uchar param[ 2 ] = { 0 };
    if (GETDEVSET.m_LockTime != pSet->m_LockTime) {
      //设置电锁时间
      for (int i = 1 ; i < 3; ++i) {
        param[ 0 ] = i;
        param[ 1 ] = pSet->m_LockTime;
        SendComCmd(CMD_S_LOCK_SETTIME , B_BOARD , param , 2);
      }
    }
    if (GETDEVSET.m_DoorMangetTime != pSet->m_DoorMangetTime) {
      //设置门磁超时时间
      for (int i = 1 ; i < 3; ++i) {
        param[ 0 ] = i;
        param[ 1 ] = pSet->m_DoorMangetTime;
        SendComCmd(CMD_S_MANGET_SETTIME , B_BOARD , param , 2);
      }
    }
    debugPrint("ENABLE_COUNT = %d" , (pSet->m_BaseSet & ENABLE_COUNT));
    debugPrint("OPENSSL_ENABLE = %d" , (pSet->m_BaseSet & OPENSSL_ENABLE));
    if ((GETDEVSET.m_BaseSet & ENABLE_COUNT) != (pSet->m_BaseSet & ENABLE_COUNT)) {
      //开门统计
      memset(GETDEVSET.m_DevCtx , 0 , sizeof(GETDEVSET.m_DevCtx));
    }
    bool needDisconnect = (GETDEVSET.m_BaseSet & OPENSSL_ENABLE) != (pSet->m_BaseSet & OPENSSL_ENABLE);
    GETDEVSET.m_BaseSet = pSet->m_BaseSet;
    GETDEVSET.m_DefaultRight = pSet->m_DefaultRight;
    GETDEVSET.m_SuperPassword = pSet->m_SuperPassword;
    GETDEVSET.m_DoorMangetTime = pSet->m_DoorMangetTime;
    GETDEVSET.m_LockTime = pSet->m_LockTime;
    GETDEVSET.m_OutputWG = pSet->m_OutputWG;
    GETDEVSET.m_OutputCtx = pSet->m_OutputCtx;
    // 设置工作模式
    int ret = GlobalConfig::ConfigObj().ChangeVerifyMode(pSet->m_VerifyMode);
    pSet->m_VerifyMode = GETDEVSET.m_VerifyMode;
    SET_ERR(pCmd->m_CFlag , (ret ? SUC_EXECUT : ERR_EXECUT));
    len = sizeof(NCMD) + sizeof(DEVSET);
    netObj.NetSend((const char*)pCmd , &len);
    if (g_pMenu->m_CamImgShow && g_pMenu->m_CamImgShow->isVisible()) {
      g_pMenu->m_CamImgShow->update();
    }
    if (needDisconnect) {
      netObj.NetDisconnect();
    }
    GlobalConfig::ConfigObj().SaveConfig();
  }
  break;
  case S_CLEAR_INOUT: {
    debugPrint("DevSetCmd::DoCmd S_CLEAR_INOUT");
    memset(GETDEVSET.m_DevCtx , 0 , sizeof(GETDEVSET.m_DevCtx));
    Reply(netObj , pCmd , SUC_EXECUT);
  }
  break;
  default:
    assert(false);
    break;
  }
}
//设备格式化
void DevFormat::LongTimeFormatTips(void)
{
  if (m_bIsFormating && m_FormatTimer.Elapsed() > 500) {
    m_pSplash->showMessage(QObject::tr("正在格式化设备....."), Qt::AlignCenter | Qt::AlignLeft, Qt::white);
    m_pSplash->show();
    while (m_bIsFormating) {
      qApp->processEvents();
      SELECT_SLEEP(300 * 1000);
    }
    m_pSplash->close();
  }
}
void DevFormat::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  debugPrint("DevFormat::DoCmd");
  assert(pCmd->m_MCmd == M_FORMAT);
  m_FormatTimer.Restart();
  m_bIsFormating = true;
  Reply(netObj , pCmd , FormatDevice(false) ? SUC_EXECUT : ERR_FROMAT);
  m_bIsFormating = false;
}
bool DevFormat::FormatDevice(bool /*reset*/)
{
  WorkState state = CLogicBuild::Obj().RecogObj().GetWorkState();
  CLogicBuild::Obj().RecogObj().SetWorkState(IDLE);
  CLogicBuild::Obj().WebClient().Reset();
  if (PGLOABLDB->DBFormat(true)) {
    g_pAdmin->Clear();
    g_pBeOperator->Clear();
    CLogicBuild::Obj().ResetFaceMem();
    CLogicBuild::Obj().Face1N().Reset();
    CLogicBuild::Obj().Face11().Reset();
    CLogicBuild::Obj().RecogObj().SetWorkState(FDECT);
    CUserTxt::Obj().Clear();
    // 删除所有指纹
    fidmaps::Obj().DelAll();
    // 格式化，清除统计计数
    memset(GETDEVSET.m_DevCtx , 0 , sizeof(GETDEVSET.m_DevCtx));
    return true;
  }
  else {
    CLogicBuild::Obj().RecogObj().SetWorkState(state);
    return false;
  }
}
//时间组
void TimeGroup::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_TGROUP);
  CARRAY* pArry = (CARRAY*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    //上传时间组
    int addRes = PGLOABLDB->AddTimeGroup(*pArry);
    Reply(netObj , pCmd , addRes == pArry->m_Count ? SUC_EXECUT : ERR_SAVE_FAIL);
    debugPrint("TimeGroup::DoCmd S_ULTG result = %d" , addRes);
  }
  break;
  case S_GET: {
    //下载时间组
    PGLOABLDB->GetTimeGroup(*pArry , pCmd->m_CFlag);
    len = sizeof(NCMD) + sizeof(CARRAY) + (pArry->m_Count) * sizeof(DSPTIMEG);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("TimeGroup::DoCmd S_DLTG count = %d" , pArry->m_Count);
  }
  break;
  case S_DEL: {
    //删除时间组
    PGLOABLDB->DelTimeGroup(*pArry);
    Reply(netObj , pCmd , SUC_EXECUT);
    debugPrint("TimeGroup::DoCmd S_DELTG");
  }
  break;
  default:
    assert(false);
  }
}
//权限
void DspRight::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_DSPRIGHT);
  CARRAY* pArry = (CARRAY*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    //上传权限
    int addRes = PGLOABLDB->AddDspRight(*pArry);
    Reply(netObj , pCmd , addRes == pArry->m_Count ? SUC_EXECUT : ERR_SAVE_FAIL);
    debugPrint("TimeGroup::DoCmd S_ULRIGHT result = %d" , addRes);
  }
  break;
  case S_GET: {
    PGLOABLDB->GetDspRight(*pArry);
    len = sizeof(NCMD) + sizeof(CARRAY) + (pArry->m_Count) * sizeof(DSPRIGHT);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("TimeGroup::DoCmd S_DLRIGHT count = %d" , pArry->m_Count);
  }
  break;
  case S_DEL: {
    PGLOABLDB->DelDspRight(*pArry);
    Reply(netObj , pCmd , SUC_EXECUT);
    debugPrint("TimeGroup::DoCmd S_DELRIGHT");
  }
  break;
  default:
    assert(false);
  }
}
//用户组
void LogicGroup::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_UGROUP);
  CARRAY* pArry = (CARRAY*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    int addRes = PGLOABLDB->AddDspUserGroup(*pArry);
    Reply(netObj , pCmd , addRes == pArry->m_Count ? SUC_EXECUT : ERR_SAVE_FAIL);
    debugPrint("S_ULGROUP result = %d" , addRes);
  }
  break;
  case S_GET: {
    PGLOABLDB->GetDspUserGroup(*pArry);
    len = sizeof(NCMD) + sizeof(CARRAY) + (pArry->m_Count) * sizeof(USERGROUP);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("S_DLGROUP count = %d" , pArry->m_Count);
  }
  break;
  case S_DEL: {
    PGLOABLDB->DelDspUserGroup(*pArry);
    Reply(netObj , pCmd , SUC_EXECUT);
    debugPrint("S_DELRIGH");
  }
  break;
  default:
    assert(false);
  }
}
//紧急开门
int EOpen::GetDoorState(void)
{
  m_GetOK = false;
  m_DoorState = -1;
  uchar param[] = { 1 , 0 };
  SendComCmd(CMD_S_GET_DOORSTATE , B_BOARD , param , sizeof(param));
  CUtimer timer;
  while (!m_GetOK && timer.Elapsed() < 1200) {
    SELECT_SLEEP(400 * 1000);
    if (m_GetOK) {
      break;
    }
    else {
      SendComCmd(CMD_S_GET_DOORSTATE , B_BOARD , param , sizeof(param));
    }
  }
  return m_DoorState;
}
void EOpen::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_DOOR_STATE);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    uchar param[] = { 1 , 0 };
    if (pCmd->m_CFlag & DOOR_OPEN) {
      param[1] = 1;
    }
    else if (pCmd->m_CFlag & DOOR_CLOSE) {
      param[1] = 2;
    }
    bool succed = SendComCmd(CMD_S_SET_DOORSTATE , B_BOARD , param , sizeof(param));
    Reply(netObj , pCmd , succed ? SUC_EXECUT : ERR_EXECUT);
  }
  return;
  case S_GET:
    pCmd->m_CFlag = pCmd->m_CFlag & ~(DOOR_OPEN | DOOR_CLOSE);
    switch (GetDoorState()) {
    case 0:
      Reply(netObj , pCmd , SUC_EXECUT);
      return;
    case 1:
      pCmd->m_CFlag |= DOOR_OPEN;
      Reply(netObj , pCmd , SUC_EXECUT);
      return;
    case 2:
      pCmd->m_CFlag |= DOOR_CLOSE;
      Reply(netObj , pCmd , SUC_EXECUT);
      return;
    default:
#ifdef PLAT_LINUX
      pCmd->m_CFlag |= DOOR_CLOSE;
      Reply(netObj , pCmd , SUC_EXECUT);
#else
      Reply(netObj , pCmd , ERR_EXECUT);
#endif
    }
    break;
  }
}
//远程开门
void ROpen::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_REMOTE_OPEN);
  if (EOpen::Obj().GetDoorState() != 0) {
    //只有在正常状态下，才能执行远程开门动作
    Reply(netObj , pCmd , ERR_EXECUT);
    return;
  }
  if (pCmd->m_CFlag & DOOR_CLOSE) {
    uchar param[] = { 1 , 0 };
    SendComCmd(CMD_S_LOCK , B_BOARD , param , sizeof(param));
  }
  else if (pCmd->m_CFlag & DOOR_OPEN) {
    uchar param[] = { 1 , 1 };
    SendComCmd(CMD_S_LOCK , B_BOARD , param , sizeof(param));
  }
  Reply(netObj , pCmd , SUC_EXECUT);
}
//多播地址设定
void MCast::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_MCAST);
  NET_ADDRESS newAdress = { 0 };
  memcpy(newAdress.IP_Address , (char*)(pCmd + 1) , 15);
  newAdress.Port = MULTYGROUP_PORT;
  Reply(netObj , pCmd , netObj.NetChangeMultAdress(newAdress) ? SUC_EXECUT : ERR_SET_FAIL);
  debugPrint("MCast::DoCmd %s" , newAdress.IP_Address);
}
//网络程序更新
void AppUpdate::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  if (M_REBOOT == pCmd->m_MCmd) {
    Reply(netObj , pCmd , SUC_EXECUT);
    netObj.NetClientVerify(false);
    DoUpdate();
  }
  else if (M_IMGCHANGE == pCmd->m_MCmd) {
    Reply(netObj , pCmd , SUC_EXECUT);
    emit NotifyStandby(pCmd);
  }
  else if (M_APPUPDATE == pCmd->m_MCmd) {
    APPUPDATE* pApp = (APPUPDATE*)(pCmd + 1);
    assert(pApp->m_Bytes > 0);
    debugPrint("M_APPUPDATE len = %d" , len);
    QDir dirTool;
    dirTool.mkdir(UPAPP_DIR);
    FILE* pFile = ::fopen(UPAPP_PATH , "w+b");
    if (!pFile) {
      Reply(netObj , pCmd , ERR_EXECUT);
      return;
    }
    LOG("软件远程更新!!!");
    Reply(netObj , pCmd , SUC_EXECUT);
    ::fwrite(pApp->m_Block , 1 , pApp->m_Bytes , pFile);
    ::fclose(pFile);
    netObj.NetClientVerify(false);
    DoUpdate();
  }
}
//网络视频传输
CVideoOut::CVideoOut() : m_Socket(0), m_Count(0)
{
  memset(&m_CNetAdr , 0 , sizeof(m_CNetAdr));
  memset(&m_NCMD , 0 , sizeof(m_NCMD));
}
CVideoOut::~CVideoOut()
{
  DeleteUdp();
}
void CVideoOut::DoCmd(NCMD* pCmd , unsigned int /*len*/ , CNetS& netObj)
{
  assert(pCmd->m_MCmd == M_VIDEO_OUT);
  switch (pCmd->m_SCmd) {
  case S_SET:
    Reply(netObj , pCmd , CreateUdp(pCmd) ? SUC_EXECUT : ERR_SET_FAIL);
    break;
  case S_GET:
    Reply(netObj , pCmd , DeleteUdp() ? SUC_EXECUT : ERR_SET_FAIL);
    break;
  }
}
bool CVideoOut::CreateUdp(NCMD* pCmd)
{
  assert(pCmd);
  NET_ADDRESS* pAdr = (NET_ADDRESS*)(pCmd + 1);
  debugPrint("Video Out IP = %s , PORT = %d" , pAdr->IP_Address , pAdr->Port);
  if (/*m_CNetAdr.Port || */pAdr->Port == 0) {
    return false;
  }
  QMutexLocker lock(&m_SynSend);
  if (!m_Socket) {
    m_Socket = CPM_S_CreateUDPSock(NULL);
  }
  memcpy(&m_NCMD , pCmd , sizeof(m_NCMD));
  memcpy(&m_CNetAdr , pAdr , sizeof(m_CNetAdr));
  m_Count = 0;
  return m_Socket;
}
bool CVideoOut::DeleteUdp(void)
{
  QMutexLocker lock(&m_SynSend);
  memset(&m_CNetAdr , 0 , sizeof(m_CNetAdr));
  memset(&m_NCMD , 0 , sizeof(m_NCMD));
  m_Count = 0;
  return true;
}
void CVideoOut::SendVideo(int /*type*/ , int w , int h , int ch , int step , char* pdata)
{
#ifdef PLAT_LINUX
  return ;
#endif
  if (m_CNetAdr.Port == 0 || 0 == m_Socket || m_Timer.Elapsed() < 100 || m_NCMD.m_MCmd != M_VIDEO_OUT) {
    return;
  }
  static int l_Block[ 64 * 1024]; //video 256K
  QMutexLocker lock(&m_SynSend);
  NCMD* pCmd = (NCMD*)((char*)l_Block + OFFSET_SIZE);
  DEVIMAGE* pData = (DEVIMAGE*)(pCmd + 1);
  memcpy(pCmd , &m_NCMD , sizeof(m_NCMD));
  memset(pData , 0 , sizeof(DEVIMAGE));
  m_Count = ++m_Count % 1000000000;
  pCmd->m_Param1 = m_Count;
  pData->m_Type = IMAGE_UYVY;
  pData->m_W = w;
  pData->m_H = h / 4;
  pData->m_Step = pData->m_W * ch;
  pData->m_Size = pData->m_H * pData->m_Step;
  assert(pData->m_Size < 256 * 1024);
  char* pDst = pData->m_Block;
  char* pSrc = pdata;
  for (int i = 0; i < h; i += 4) {
    pSrc = pdata + i * step;
    memcpy(pDst , pSrc , step);
    pDst += step;
  }
  uint len = sizeof(NCMD) + sizeof(DEVIMAGE) + pData->m_Size;
  CPM_S_SendDataTo(m_Socket , (const char*)pCmd , &len , m_CNetAdr);
  m_Timer.Restart();
}
void CIOCtrl::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  assert(len >= sizeof(NCMD) + sizeof(IOCTRL));
  CUtimer time;
  IOCTRL* pCtrl = (IOCTRL*)(pCmd + 1);
  SET_ERR(pCmd->m_CFlag, SUC_EXECUT);
  if (pCtrl->m_Action & (ACT_IO_CLOSE | ACT_IO_OPEN | ACT_IO_OPENEX)) {
    if (DoAction(pCtrl->m_Action, pCtrl->m_Source, NULL, NULL) != EVENT_NONE) {
      SET_ERR(pCmd->m_CFlag, ERR_EXECUT);
    }
  }
  if (pCtrl->m_Action & ACT_MODE_SET) {
    if (!SetMode(pCtrl->m_Source, pCtrl->m_IOMode)) {
      SET_ERR(pCmd->m_CFlag , ERR_PARAM);
    }
  }
  if (pCtrl->m_Action & ACT_STATE_GET) {
    int ioID = GET_INDEX_485(pCtrl->m_Source);
    if (ioID > 0 && ioID < 3) {
      pCtrl->m_IOState = GlobalConfig::ConfigObj().m_Door[ ioID - 1 ].m_IOState;
    }
    else {
      SET_ERR(pCmd->m_CFlag , ERR_EXECUT);
    }
  }
  if (pCtrl->m_Action & ACT_MODE_GET) {
    int ioID = GET_INDEX_485(pCtrl->m_Source);
    if (ioID > 0 && ioID < 3) {
      pCtrl->m_IOMode = GlobalConfig::ConfigObj().m_Door[ ioID - 1 ].m_IOMode;
    }
    else {
      SET_ERR(pCmd->m_CFlag , ERR_EXECUT);
    }
  }
  debugPrint("IOCTRL src = %x act = %x mode = %x state = %x time = %ld", pCtrl->m_Source, pCtrl->m_Action, pCtrl->m_IOMode, pCtrl->m_IOState, time.Elapsed());
  len = sizeof(NCMD) + sizeof(IOCTRL);
  netObj.NetSend((char*)pCmd, &len);
}
void CIOCtrl::DoPortCmd(const CMD& cmd)
{
  switch (cmd.m_Cmd[CMD_ADR]) {
  case CMD_S_MANGET_STATE: {
    int l_Memory[ ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(IOCTRL)) >> 2 ] = { 0 };
    NCMD* pCmd = (NCMD*)((char*)l_Memory + OFFSET_SIZE);
    IOCTRL* pIOCtrl = (IOCTRL*)(pCmd + 1);
    pCmd->m_MCmd = M_IOCTRL;
    pCmd->m_SCmd = S_REALTIME_SEND;
    pIOCtrl->m_Action = ACT_STATE_GET;
    pIOCtrl->m_Source = (cmd.m_Cmd[P0_ADR] << 4) | 0xF;
    pIOCtrl->m_IOState = cmd.m_Cmd[P0_ADR + 1];
    uint len = sizeof(NCMD) + sizeof(IOCTRL);
    CNetS::Obj().NetSend((char*)pCmd, &len);
    debugPrint("M_IOCTRL --- S_REALTIME_SEND");
  }
  break;
  default:
    break;
  }
  debugPrint("DoPortCmd cmd = %x src = %x iostate = %d", cmd.m_Cmd[CMD_ADR], (cmd.m_Cmd[P0_ADR] << 4) | 0xF, cmd.m_Cmd[P0_ADR + 1]);
}
int CIOCtrl::AnalyzeWG(const USER* pUser, const VerifyResult* pVerify)
{
  if (!(GETDEVSET.m_OutputWG & OUTPUT_WG_YES)) {
    return 0;
  }
  bool needSend = false;
  uchar cmd = (GETDEVSET.m_OutputWG & OUTPUT_WG_34) ? CMD_S_WG34 : CMD_S_WG26;
  uchar param[6] = { 0 };
  if (GETDEVSET.m_OutputWG & OUTPUT_WG_CTX) {
    Uint2WG((uint)GETDEVSET.m_OutputCtx, param, sizeof(param), cmd);
    needSend = true;
  }
  else {
    if (GETDEVSET.m_OutputWG & OUTPUT_WG_UID) {
      // 编号
      if (pUser) {
        Uint2WG(::strtoul(pUser->m_UserID.m_ID, 0, 10), param , sizeof(param), cmd);
        needSend = true;
      }
    }
    else {
      // 卡号
      if (pVerify && CMD_R_HIDCARD == pVerify->m_Uart.m_Cmd[CMD_ADR]) {
        SendComCmd(CMD_S_WG_HID , B_BOARD , pVerify->m_Uart.m_Cmd + P0_ADR , pVerify->m_Uart.m_Cmd[P0_ADR] - 7);
        return 1;
      }
      int wgType = pVerify ? GET_WG_TYPE(pVerify->m_Uart) : WG_OLD;
      if (WG_OLD != wgType) {
        cmd = CMD_S_WG_NEW;
        memcpy(param , &(pVerify->m_Uart.m_Cmd[P0_ADR]) , 6);
        needSend = true;
      }
      else if (pUser) {
        Uint2WG(CardStr2Uint(pUser->m_AppendID.m_ID, GlobalConfig::ConfigObj().m_CardFormat), param , sizeof(param), cmd);
        needSend = true;
      }
    }
  }
  if (needSend) {
    SendComCmd(CMD_S_WG_NEW, B_BOARD, param, sizeof(param));
  }
  return 1;
}
bool CIOCtrl:: SetMode(char source, char mode)
{
  int ioID = GET_INDEX_485(source);
  int type = GET_TYPE_485(source);
  switch (type) {
  case DEV_TYPE_DOOR:
  case DEV_TYPE_FACE1:
  case DEV_TYPE_FACE2:
  case DEV_TYPE_CARD1:
  case DEV_TYPE_CARD2:
    if (ioID > 0 && ioID < 3 && (IO_MODE_NORMAL == mode || IO_MODE_OPEN == mode || IO_MODE_CLOSE == mode)) {
      GlobalConfig::ConfigObj().m_Door[ioID - 1].m_IOMode = mode;
      debugPrint("SetMode ioID = %d mode = %d" , ioID , mode);
      uchar param[] = { ioID , mode };
      SendComCmd(CMD_S_SET_DOORSTATE , B_BOARD , param , sizeof(param));
      switch (mode) {
      case IO_MODE_OPEN:
        OutputVoice(VOICE_EME_OPEN);
        break;
      case IO_MODE_CLOSE:
        OutputVoice(VOICE_EME_CLOSE);
        break;
      }
      return true;
    }
    break;
  default:
    break;
  }
  return false;
}
int CIOCtrl::DoAction(int mode, char source, const USER* pUser, const VerifyResult* pVerify)
{
  int ioID = GET_INDEX_485(source);
  int type = GET_TYPE_485(source);
  switch (type) {
  case DEV_TYPE_DOOR :
  case DEV_TYPE_FACE1:
  case DEV_TYPE_FACE2:
  case DEV_TYPE_CARD1:
  case DEV_TYPE_CARD2:
  case DEV_TYPE_BUTTON:
    if (mode & ACT_IO_OPENEX) {
      uchar param[] = { ioID, 1, PGLOABLINI->m_OutPutOpen };
      SendComCmd(CMD_S_OUTPUT , B_BOARD , param , sizeof(param));
      debugPrint("----------------ACT_IO_OPENEX mode = %x source = %x \r\n", mode, source);
    }
    if (mode & (ACT_IO_CLOSE | ACT_IO_OPEN)) {
      if (ioID < 1 || ioID > 2 || GlobalConfig::ConfigObj().m_Door[ioID - 1].m_IOMode != IO_MODE_NORMAL) {
        debugPrint("DoAction mode = %d" , GlobalConfig::ConfigObj().m_Door[ioID - 1].m_IOMode);
        OutputVoice(IO_MODE_OPEN == GlobalConfig::ConfigObj().m_Door[ioID - 1].m_IOMode ? VOICE_EME_OPEN : VOICE_EME_CLOSE);
        return ACT_ERR_STATE;
      }
      if (mode & ACT_IO_CLOSE) {
        uchar param[] = { ioID , 0 };
        SendComCmd(CMD_S_LOCK , B_BOARD , param , sizeof(param));
        debugPrint("----------------ACT_IO_CLOSE mode = %x source = %x", mode, source);
      }
      if (mode & ACT_IO_OPEN) {
        if ((GETDEVSET.m_BaseSet & AB_LOCK) && IO_STATE_OPEN == GlobalConfig::ConfigObj().m_Door[2 - ioID].m_IOState) {
          debugPrint("----------------mode = %x source = %x state = %d \r\n",
              mode, source,
              GlobalConfig::ConfigObj().m_Door[2 - ioID].m_IOState
                    );
          OutputVoice(1 == ioID ? VOICE_INDOOR_NOCLOSE : VOICE_OUTDOOR_NOCLOSE);
          return ACT_BREAK_AB_RULE;
        }
        if (1 == ioID) {
          // 门1才能输出伟根
          AnalyzeWG(pUser , pVerify);
        }
        uint uid = (pUser ? ::strtoul(pUser->m_UserID.m_ID, 0, 10) : 0);
        uchar param[6] = { 0 };
        param[ 0 ] = ioID;
        param[ 1 ] = 1;
        Int2BigEndian(uid, param + 2, 4);
        SendComCmd(CMD_S_LOCK , B_BOARD , param , sizeof(param));
        // 认证成功，允许通过，请随手关门
        OutputVoice(VOICE_RIGHT_OK);
        debugPrint("door = %d , p0-4= %x%x%x%x , UID = %d" , param[0] , param[2], param[3], param[4], param[5] , uid);
        debugPrint("----------------ACT_IO_OPEN mode = %x source = %x \r\n", mode, source);
        // 出门按钮开门布防，其他方式开门撤防
        Arming(DEV_TYPE_BUTTON == type);
      }
    }
    return EVENT_NONE;
  case DEV_TYPE_OUT:
    if (ioID < 1 || ioID > 2) {
      return ACT_ERR_PARAM;
    }
    if (mode == ACT_IO_CLOSE) {
      uchar param[] = { ioID, 0, PGLOABLINI->m_OutPutOpen };
      SendComCmd(CMD_S_OUTPUT , B_BOARD , param , sizeof(param));
      debugPrint("----------------ACT_IO_CLOSE mode = %x source = %x \r\n", mode, source);
    }
    else if (mode & ACT_IO_OPEN) {
      uchar param[] = { ioID, 1, PGLOABLINI->m_OutPutOpen };
      SendComCmd(CMD_S_OUTPUT , B_BOARD , param , sizeof(param));
      debugPrint("----------------ACTION_OUTPUT mode = %x source = %x \r\n", mode, source);
    }
    else if (mode & ACT_IO_OPENEX) {
      return ACT_ERR_PARAM;
    }
    return EVENT_NONE;
  default:
    break;
  }
  return ACT_ERR_PARAM;
}
//NETPRO
void CNetPro::DoCmd(NCMD* pCmd , unsigned int len , CNetS& netObj)
{
  NETPRO* pNetPro = (NETPRO*)(pCmd + 1);
  switch (pCmd->m_SCmd) {
  case S_SET: {
    Accept(*pNetPro , netObj);
    *pNetPro = NETSETTING;
    SET_ERR(pCmd->m_CFlag , SUC_EXECUT);
    pCmd->m_CFlag |= CMD_STATE_REPLY;
    len = sizeof(NCMD) + sizeof(NETPRO);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("CNetPro ##################### S_SET!");
  }
  break;
  case S_GET: {
    WebObj* pWebSrv0 = CLogicBuild::Obj().WebSrv(0);
    WebObj* pWebSrv1 = CLogicBuild::Obj().WebSrv(1);
    NETSETTING.m_WebPro1 = 0;
    NETSETTING.m_WebPro2 = 0;
    memset(NETSETTING.m_WebUrl1 , 0 , sizeof(NETSETTING.m_WebUrl1));
    memset(NETSETTING.m_WebUrl2 , 0 , sizeof(NETSETTING.m_WebUrl2));
    if (pWebSrv0) {
      NETSETTING.m_WebPro1 = (char)(pWebSrv0->m_iWebFunEnable & 0xFF);
      strncpy(NETSETTING.m_WebUrl1 , pWebSrv0->m_sCmdUrl , sizeof(NETSETTING.m_WebUrl1) - 1);
      debugPrint("m_WebPro1 %x , %x" , NETSETTING.m_WebPro1 , pWebSrv0->m_iWebFunEnable);
    }
    if (pWebSrv1) {
      NETSETTING.m_WebPro2 = (char)(pWebSrv1->m_iWebFunEnable & 0xFF);
      strncpy(NETSETTING.m_WebUrl2 , pWebSrv1->m_sCmdUrl , sizeof(NETSETTING.m_WebUrl2) - 1);
      debugPrint("m_WebPro2 %x , %x" , NETSETTING.m_WebPro2 , pWebSrv1->m_iWebFunEnable);
    }
    *pNetPro = NETSETTING;
    len = sizeof(NCMD) + sizeof(NETPRO);
    netObj.NetSend((const char*)pCmd , &len);
    debugPrint("ip %s \n mask %s \n gate %s \n dns %s \n" , pNetPro->m_IPAdr , pNetPro->m_NetMask , pNetPro->m_GateWay , pNetPro->m_Dns);
    debugPrint("CNetPro $$$$$$$$$$$$$$$$$$$$$ G_GET!");
  }
  break;
  }
}
void CNetPro::Accept(NETPRO& pro , CNetS& netObj)
{
  debugPrint("ip %s \n mask %s \n gate %s \n dns %s \n" , pro.m_IPAdr , pro.m_NetMask , pro.m_GateWay , pro.m_Dns);
  debugPrint("WEB1 %s FUN1 %X \nWEB2 %s FUN2 %X " , pro.m_WebUrl1 , pro.m_WebPro1 , pro.m_WebUrl2 , pro.m_WebPro2);
  WebObj* pWebSrv0 = CLogicBuild::Obj().WebSrv(0);
  WebObj* pWebSrv1 = CLogicBuild::Obj().WebSrv(1);
  if (pWebSrv0) {
    pWebSrv0->Accept(pro.m_WebPro1 , pro.m_WebUrl1);
  }
  if (pWebSrv1) {
    pWebSrv1->Accept(pro.m_WebPro2 , pro.m_WebUrl2);
  }
  NETSETTING.m_WebPro1 = pro.m_WebPro1;
  NETSETTING.m_WebPro2 = pro.m_WebPro2;
  memset(NETSETTING.m_WebUrl1 , 0 , sizeof(NETSETTING.m_WebUrl1));
  strncpy(NETSETTING.m_WebUrl1 , pro.m_WebUrl1 , sizeof(NETSETTING.m_WebUrl1));
  memset(NETSETTING.m_WebUrl2 , 0 , sizeof(NETSETTING.m_WebUrl2));
  strncpy(NETSETTING.m_WebUrl2 , pro.m_WebUrl2 , sizeof(NETSETTING.m_WebUrl2));
  NET_ADDRESS nMip = { 0 };
  memcpy(nMip.IP_Address , NETSETTING.m_MIPAdr , 15);
  if (AcceptIp(nMip.IP_Address , sizeof(nMip.IP_Address) , pro.m_MIPAdr , NULL) == 1) {
    nMip.Port = MULTYGROUP_PORT;
    netObj.NetChangeMultAdress(nMip);
  }
  AcceptIp(NETSETTING.m_GateWay , sizeof(NETSETTING.m_GateWay) , pro.m_GateWay , SetGateWayAdress);
  AcceptIp(NETSETTING.m_NetMask , sizeof(NETSETTING.m_NetMask) , pro.m_NetMask , SetNetMaskAdress);
  AcceptIp(NETSETTING.m_Dns , sizeof(NETSETTING.m_Dns) , pro.m_Dns , SetDnsAdress);
  if (AcceptIp(NETSETTING.m_IPAdr , sizeof(NETSETTING.m_IPAdr) , pro.m_IPAdr , SetIPAdress) == 1) {
    netObj.NetDisconnect();
  }
  GlobalConfig::ConfigObj().SaveConfig();
}
//看门狗
#define WATCHDOG_IOCTL_BASE 'W'
#define WDIOC_GETSUPPORT _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define WDIOC_SETTIMEOUT _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define WDIOC_GETTIMEOUT _IOR(WATCHDOG_IOCTL_BASE, 7, int) 27
#define WDIOS_DISABLECARD 0x0001 /* Turn off the watchdog timer */
#define WDIOS_ENABLECARD 0x0002 /* Turn on the watchdog timer */
#define WDIOC_SETOPTIONS _IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define WDIOC_KEEPALIVE _IOR(WATCHDOG_IOCTL_BASE, 5, int)
WatchDog::WatchDog():
  m_DogFd(-1) ,
  m_DogType(-1) ,
  m_Flag(0) ,
  m_EnableDog(true)
{
}
WatchDog::~WatchDog()
{
}
bool WatchDog::Start(int dogType)
{
  QMutexLocker lock(&m_SynAlive);
  switch (dogType) {
  case SOFT_DOG:
  case HARD_DOG:
    m_DogFd = 1; //open( "/dev/watchdog" , O_WRONLY );
    m_DogType = dogType;
    l_irTImer.Restart();
    l_comTimer.Restart();
    l_guiTimer.Restart();
    SendComCmd(CMD_S_DOG , B_BOARD , NULL , 0);
    debugPrint(" davinci watchdog is open ... ");
    return m_DogFd >= 0;
  default:
    return false;
  }
}
bool WatchDog::Alive(int flag)
{
  QMutexLocker lock(&m_SynAlive);
  if (m_DogFd <= 0) {
    return false;
  }
  switch (flag) {
  case MAIN_THREAD_OK:
    l_guiTimer.Restart();
    break;
  case COM_THREAD_OK:
    l_comTimer.Restart();
    break;
  case IR_CAMERA_OK:
    l_irTImer.Restart();
    break;
  }
  if (l_guiTimer.Elapsed() > 30000) {
    LOG("主GUI线程异常...%x , enable = %d" , flag , m_EnableDog);
  }
  if (l_comTimer.Elapsed() > 30000) {
    LOG("串口线程异常...%x , enable = %d" , flag , m_EnableDog);
  }
  if (l_irTImer.Elapsed() > 30000) {
    LOG("算法线程异常...%x , enable = %d" , flag , m_EnableDog);
  }
  if (!m_EnableDog) {
    return false;
  }
  m_Flag |= flag;
  if ((m_Flag & MAIN_THREAD_OK) && (m_Flag & COM_THREAD_OK) && (m_Flag & IR_CAMERA_OK)) {
    //看门狗，检测主UI线程和彩色，红外，串口.
    SendComCmd(CMD_S_DOG , B_BOARD , NULL , 0);
    m_Flag = 0;
    return true;
  }
  return false;
}
bool WatchDog::Stop(void)
{
  QMutexLocker lock(&m_SynAlive);
  uchar param[] = { 1 };
  SendComCmd(CMD_S_DOG , B_BOARD , param , 1);
  m_DogFd = -1;
  return true;
}
bool WatchDog::Enable(bool flag)
{
  QMutexLocker lock(&m_SynAlive);
  if (m_DogFd == -1) {
    //未开启看门狗，不处理
    return true;
  }
  m_EnableDog = flag;
  if (m_EnableDog) {
    SendComCmd(CMD_S_DOG , B_BOARD , NULL , 0);
  }
  else {
    uchar param[] = { 1 };
    SendComCmd(CMD_S_DOG , B_BOARD , param , 1);
  }
  return true;
}

