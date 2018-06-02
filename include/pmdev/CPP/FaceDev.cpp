// FaceDev.cpp: implementation of the CFaceDev class.
//
//////////////////////////////////////////////////////////////////////

#include "FaceDev.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BOOL CFaceDevMrg::UnInit()
{
  return CPM_InitSys(FALSE);
}

BOOL  CPMDEV_CALL CFaceDevMrg::QuerySrvCB(const char* cszDevAddr, PEERADR& srvAddr, void* pvContext)
{
  CFaceDevMrg* pThis = (CFaceDevMrg*) pvContext;

  if (pThis) {
    return pThis->QuerySrvCB_Imp(cszDevAddr, srvAddr);
  }

  return FALSE;
}
BOOL    CFaceDevMrg::QuerySrvCB_Imp(const char* cszDevAddr, PEERADR& srvAddr)
{
  return TRUE;
}


BOOL  CPMDEV_CALL CFaceDevMrg::ConnectStatusCallBack(const char* cszDevAddr , DEV_CONNECT_STATUS eCNStatus , void* pvContext)
{
  CFaceDevMrg* pThis = (CFaceDevMrg*) pvContext;

  if (pThis) {
    return pThis->ConnectStatusCallBack_Imp(cszDevAddr, eCNStatus);
  }

  return FALSE;
}
BOOL    CFaceDevMrg::ConnectStatusCallBack_Imp(const char*  cszDevAddr, DEV_CONNECT_STATUS eCNStatus)
{
  char sIP[20];
  memset(sIP, 0, 20);
  strcpy(sIP, cszDevAddr);
  
  if (eCNStatus == DEV_CONNECT_SUCCESS) {
    printf("和设备%s连接成功。\n", cszDevAddr);
    printf("进行设备登陆权限验证...\n");

    BOOL bRet = Login(sIP, "admin", "201031");
  }

  if (eCNStatus ==  DEV_CONNECT_FAILUE) {
    printf("和设备%s连接失败。\n", cszDevAddr);
  }

  if (eCNStatus ==  DEV_CONNECT_CUT) {
    printf("和设备%s断开连接。\n", cszDevAddr);
    Connect(sIP);
  }

  return TRUE;
}


BOOL  CPMDEV_CALL CFaceDevMrg::DevCallBack(const char* sIP, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext)
{
  CFaceDevMrg* pThis = (CFaceDevMrg*) pvContext;

  if (pThis) {
    return pThis->DevCallBack_Imp(sIP, eType, eFlag, pvContent,  nSeq, eResult);
  }

  return FALSE;
}

BOOL  CFaceDevMrg::DevCallBack_Imp(const char* sIP, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult)
{
  switch (eResult) {
  case    OPER_ERR_BUSY:
    printf("设备%s:设备忙。\n", sIP);
    return TRUE;//设备忙

  case    OPER_ERR_LIMIT:
    printf("设备%s:已达上限。\n", sIP);
    return TRUE;//已达上限

  case    OPER_ERR_NOFIND:       //没有找到对应数据
    printf("设备%s:没有找到对应数据\n", sIP);
    return TRUE;

  case    OPER_ERR_SAVEFAIL:
    printf("设备%s:数据保存失败\n", sIP);
    return TRUE;//数据保存失败

  case    OPER_ERR_SETFAIL:
    printf("设备%s:设置失败\n", sIP);
    return TRUE;//设置失败

  case    OPER_ERR_FROMAT:
    printf("设备%s:格式化失败\n", sIP);
    return TRUE;//格式化失败

  case    OPER_ERR_PARAM:
    printf("设备%s:参数错误\n", sIP);
    return TRUE;       //参数错误

  case    OPER_ERR_DISABLE:
    printf("设备%s:要求执行的功能没有使能\n", sIP);
    return TRUE;     //

  case    OPER_ERR_EXECUT:
    printf("设备%s:执行失败\n", sIP);
    return TRUE;      //失败

  case    OPER_ERR_SUPPORT:
    printf("设备%s:不支持的命令\n", sIP);
    return TRUE;      //不支持的命令

  case    OPER_ERR_INPUTDATA:
    printf("设备%s:网络端传输的数据有异常\n", sIP);
    return TRUE;        //网络端传输的数据有异常

  case    OPER_BATCH_DL_COMPLETE:
    printf("设备%s:批量下载完成\n", sIP);
    return TRUE;   //批量下载完成

  case        OPER_SUCCESS:
    break;

  default:
    return TRUE;
  }

  //权限验证操
  char arrIP[20];
  memset(arrIP, 0, 20);
  strcpy(arrIP, sIP);
  BOOL bRet = FALSE;

  switch (eType) {
  case DEV_AUTH_OPER: {
    printf("设备%s:权限验证通过。\n", sIP);
    printf("设备%s:获取基本信息...\n", sIP);
    bRet = GetBaseInfo(arrIP);

    printf("设备%s:获取统计信息...\n", sIP);
    bRet = GetStatisInfo(arrIP);

    printf("设备%s:获取工作信息...\n", sIP);
    bRet = GetWorkAttInfo(arrIP);

    printf("设备%s:获取设备时间...\n", sIP);
    bRet = GetTime(arrIP);
    break;
  }

  case DEV_BASEINFO_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取基本信息成功\n", sIP);
    }

    break;

  case DEV_STATISINFO_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取统计信息成功\n", sIP);
    }

    break;

  case DEV_WORKATT_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取工作信息成功\n", sIP);
      DEV_WORKATT* pWorkAtt = (DEV_WORKATT*) pvContent;

      printf("设备%s:设置工作信息\n", sIP);
      bRet = CPM_ULDevWorkAttInfo(sIP, *pWorkAtt);
    }

    break;

  case DEV_SYS_TIME_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取设备时间成功\n", sIP);
      DEV_DATETIME* pDateTimeInfo = (DEV_DATETIME*) pvContent;

      printf("设备%s:设置设备时间\n", sIP);
      bRet = CPM_ULDevTime(sIP, *pDateTimeInfo);
    }

    break;

  case DEV_USER_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取用户成功\n", sIP);
      DEV_USER* pUserInfo = (DEV_USER*) pvContent;

      printf("设备%s:上传用户:%s\n", sIP, pUserInfo->m_UserID.m_ID);
      bRet = CPM_ULUser(sIP, *pUserInfo);



      memcpy(&m_TempUser, pUserInfo, sizeof(DEV_USER)); //给远程加载
      /*
      printf("设备%s:实时用户加载:%s\n",sIP,pUserInfo->m_UserID.m_ID);
        DEV_USER_REAL_LOAD stDLoadUser;
      memset(&stDLoadUser, 0, sizeof(DEV_USER_REAL_LOAD));
      stDLoadUser.m_bApeendID = FALSE;
      strcpy(stDLoadUser.m_ID.m_ID,  pUserInfo->m_UserID.m_ID);
      stDLoadUser.m_LoadFlag = DL_DEV_PIC|DL_DEV_USER_FEAT|DL_DEV_USER_TEXT;
      stDLoadUser.m_ReqSource = 0;
      bRet = CPM_ULRealTimeUser( sIP, *pUserInfo , stDLoadUser );//远程加载
      */

      printf("设备%s:远程用户特征采集:%s\n", sIP, pUserInfo->m_UserID.m_ID);
      bRet = CPM_ULRealTimeUserCap(sIP, *pUserInfo);

    }

    break;

  case DEV_USER_EX_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      DEV_BATCH_USER* pUserBatch = (DEV_BATCH_USER*) pvContent;

      if (pUserBatch->m_nCount == 0) {
        return TRUE;
      }

      printf("设备%s:批量下载用户成功\n", sIP);
      printf("设备%s:批量上传用户....", sIP);
      bRet = CPM_ULUserEx(sIP, *pUserBatch,  DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);

    }

    break;

  case DEV_USER_RANGE_OPER:
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取用户批量下载的大区间成功\n", sIP);
      DEV_DL_USER_RANGE* pDevBathRange = (DEV_DL_USER_RANGE*) pvContent;

      if (pDevBathRange->m_Count == 0) {
        return TRUE;
      }

      int nBatCount = pDevBathRange->m_Count / 5 ; //每五个用户一个区间
      int nLastBat =  pDevBathRange->m_Count % 5 ;
      DEV_CID arrID[5];
      DEV_VUSER* pVVUser = NULL;

      printf("设备%s:把大区间划分成几个小区间下载用户\n", sIP);
      int i = 0;

      for (i = 0; i < nBatCount ; i++) {
        memset(arrID, 0, 5 * sizeof(DEV_CID));
        int k = 0;

        for (int j = i * 5; j < i * 5 + 5; j++) {
          pVVUser = &pDevBathRange->m_pArray[j];
          strcpy(arrID[k++].m_ID, pVVUser->m_UserID.m_ID);
        }

        printf("下载区间%d的用户\n", i);
        bRet = CPM_DLUserEx(sIP, arrID, 5, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
      }

      if (nLastBat) {
        printf("下载区间%d的用户\n", i);
        memset(arrID, 0, 5 * sizeof(DEV_CID));
        //此处略去
      }
    }

    break;

  case DEV_USER_RT_OPER: { //远程加载
    DEV_USER_REAL_LOAD* pDevUserRealLoad = (DEV_USER_REAL_LOAD*) pvContent;

    DEV_USER_REAL_LOAD stDevUserRealLoad;
    memset(&stDevUserRealLoad, 0, sizeof(DEV_USER_REAL_LOAD));
    memcpy(&stDevUserRealLoad, pDevUserRealLoad, sizeof(DEV_USER_REAL_LOAD));
    printf("设备%s:实时用户加载:%s\n", sIP, m_TempUser.m_UserID.m_ID);
    bRet = CPM_ULRealTimeUser(sIP, m_TempUser, stDevUserRealLoad);
  }
  break;

  case DEV_RECORD_RANGE_OPER:
  case DEV_ALARM_RANGE_OPER: {
    //获取记录的各个大区间，大区间以天为单位。
    if (eFlag == DEV_OPER_DOWNLOAD) {
      printf("设备%s:获取记录批量下载的各个大区间成功\n", sIP);
      DEV_DL_RECORD_ARRAY* pDevBathRange = (DEV_DL_RECORD_ARRAY*) pvContent;

      if (pDevBathRange->m_Count == 0) {
        return TRUE;
      }

      DEV_DL_RECORD_RANGE stDevRange;

      for (int i = 0; i < pDevBathRange->m_Count; i++) {
        //对每个大区间进行拆分，拆分成每50条记录大小多个小区间,没个小区间的第一条记录的ID，由上一个小区间的最后一条记录ID，因此大区间下的小区间必须串行下载。这里只一此下载大区间的前50条。
        int nSBatchCount = pDevBathRange->m_Count / 50;
        int nLastCount = pDevBathRange->m_Count % 50;
        memcpy(&stDevRange, pDevBathRange->m_pRange, sizeof(DEV_DL_RECORD_RANGE));
        stDevRange.m_Count = stDevRange.m_Count > 50 ? 50 : stDevRange.m_Count;
        bRet = DLoadRecordEx(arrIP, stDevRange);
      }
    }
  }
  break;

  case DEV_RECORD_OPER:
  case DEV_ALARM_OPER: {
    if (eFlag == DEV_OPER_DOWNLOAD) {
      DEV_RECORD* pRecord = (DEV_RECORD*) pvContent;
      printf("设备%s:下载记录%s\n", sIP, pRecord->m_SerialID.m_ID);
    }


  }
  break;

  case DEV_RECORD_EX_OPER: {
    if (eFlag == DEV_OPER_DOWNLOAD) {
      DEV_BATCH_RECORD* pBatchRecord = (DEV_BATCH_RECORD*) pvContent;
      printf("设备%s:批量下载记录%d条成功\n", sIP, pBatchRecord->m_nCount);

    }

  }
  break;

  case DEV_USER_REMOTE_CAP: { //远程采集完成
    if (eFlag == DEV_OPER_UPLOAD) { //
      DEV_USER* pUserInfo = (DEV_USER*) pvContent;
      printf("设备%s:远程采集用户%s成功\n", sIP, pUserInfo->m_UserID.m_ID);
    }
  }
  break;

  case DEV_NOPEN_OPER: {
    if (eFlag == DEV_OPER_DOWNLOAD) { //
      DEV_NETPRO* pNOPEN  = (DEV_NETPRO*) pvContent;
      printf("设备%s:获取网络信息成功", sIP);

      printf("设备%s:设置网络信息...", sIP);
      bRet = CPM_NetPro(arrIP,  pNOPEN);

    }
  }
  break;

  }

  return TRUE;
}

BOOL CFaceDevMrg::SetWorkMode(int nMode)
{
  if (nMode == LocalMode) {
    return CPM_SetMode(FALSE, NULL, NULL);
  }
  else {
    return CPM_SetMode(TRUE, NULL, NULL);
  }
}

void  CFaceDevMrg::BindingCallBack()
{
  CPM_RegDevConnectStatusCB(ConnectStatusCallBack, this);
  CPM_RegOperResultNotifyCB(DevCallBack, this);
}

BOOL CFaceDevMrg::Connect(const char* sIP)
{
  return CPM_CNDev(sIP);
}

void CFaceDevMrg::DisConnect(const char* sIP)
{
  CPM_DCNDev(sIP);
}

BOOL CFaceDevMrg::ConnnectServer(const char* sIP, char* name, char* psw)
{
  return CPM_CnSrv(TRUE, sIP, name, psw);
}

BOOL CFaceDevMrg::DisConnnectServer(const char* sIP, char* name, char* psw)
{
  return CPM_CnSrv(FALSE, sIP, name, psw);
}

BOOL CFaceDevMrg::StartSearch()
{
  return  CPM_StartDevSerch(TRUE);
}

BOOL CFaceDevMrg::StopSearch()
{
  return  CPM_StartDevSerch(FALSE);
}

BOOL  CFaceDevMrg::Login(const char* sIP, const char* sLoginName, const char* sLoginPwd)
{
  DEV_CID stLogin;
  DEV_CID stPwd;
  memset(stLogin.m_ID, 0, DEV_ID_LEN);
  memset(stPwd.m_ID, 0, DEV_ID_LEN);
  strcpy(stLogin.m_ID, sLoginName);
  strcpy(stPwd.m_ID, sLoginPwd);

  return CPM_ULDevAuth(sIP, stLogin, stPwd, 1);
}


BOOL CFaceDevMrg::GetTime(const char* sIP)
{
  return CPM_DLDevTime(sIP);
}

BOOL  CFaceDevMrg::GetBaseInfo(const char* sIP)
{
  return CPM_DLDevBaseInfo(sIP);
}

BOOL CFaceDevMrg::GetStatisInfo(const char* sIP)
{
  return CPM_DLDevStatisInfo(sIP);
}

BOOL CFaceDevMrg::GetWorkAttInfo(const char* sIP)
{
  return CPM_DLDevWorkAttInfo(sIP);
}

BOOL CFaceDevMrg::DLoadUserInfo(const char* sIP, const char* sUserID)
{
  DEV_CID stID;
  memset(stID.m_ID, 0, DEV_ID_LEN);
  strcpy(stID.m_ID, sUserID);
  return CPM_DLSingleUser(sIP, stID, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
}

BOOL CFaceDevMrg::DLoadAllUserInfo(const char* sIP)
{
  return CPM_DLAllUser(sIP);
}

BOOL CFaceDevMrg::DelUser(const char* sIP, const char* sUserID)
{
  DEV_CID stID;
  memset(stID.m_ID, 0, DEV_ID_LEN);
  strcpy(stID.m_ID, sUserID);
  return CPM_DELUser(sIP, stID);
}

BOOL CFaceDevMrg::DLoadTimeSpanUsers(const char* sIP,  DEV_DATE& cStart,  DEV_DATE& cEnd)
{
  return CPM_DLSegTimeUser(sIP, cStart, cEnd);
}

BOOL CFaceDevMrg::DLoadAllIdentifyRecord(const char* sIP)
{
  return CPM_DLAllIdentifyRecord(sIP);
}

BOOL CFaceDevMrg::DLoadTimeSpanIdentifyRecord(const char* sIP,  DEV_DATE& cStart,  DEV_DATE& cEnd)
{
  return CPM_DLSegTimeIdentifyRecord(sIP, cStart, cEnd);
}

BOOL CFaceDevMrg::DLoadRangeIdentifyRecord(const char* sIP, DEV_DL_RECORD_RANGE& cRange)
{
  return CPM_DLRangeRec(sIP, cRange, DL_DEV_PIC | DL_DEV_USER_TEXT);
}

BOOL CFaceDevMrg::DLoadAllAlarmRecord(const char* sIP)
{
  return CPM_DLAllAlarmRecord(sIP);
}

BOOL CFaceDevMrg::DLoadTimeSpanAlarmRecord(const char* sIP,  DEV_DATE& cStart,  DEV_DATE& cEnd)
{
  return CPM_DLSegTimeAlarmRecord(sIP, cStart, cEnd);
}

BOOL CFaceDevMrg::DLoadRangeAlarmRecord(const char* sIP, DEV_DL_RECORD_RANGE& cRange)
{
  return CPM_DLRangeRec(sIP, cRange, DL_DEV_PIC | DL_DEV_USER_TEXT);
}

BOOL CFaceDevMrg::DLoadAllLog(const char* sIP)
{
  return CPM_DLLog(sIP);
}

BOOL CFaceDevMrg::DLoadRangeLog(const char* sIP, DEV_DL_RECORD_RANGE& cRange)
{
  return CPM_DLRangeLog(sIP, cRange);
}

BOOL CFaceDevMrg::DLoadAllChangeImage(const char* sIP)
{
  return CPM_DLChangeImage(sIP);
}

BOOL CFaceDevMrg::DLoadRangeChangeImage(const char* sIP, DEV_DL_RECORD_RANGE& cRange)
{
  return CPM_DLRangeCImage(sIP, cRange, DL_DEV_PIC);
}

BOOL CFaceDevMrg::DLoadTimeGroup(const char* sIP,  DEV_TIMEGROUP_ARRAY& cTGArray)
{
  return CPM_DLTimeGroup(sIP, cTGArray, DEV_ANY_TIME);
}

BOOL CFaceDevMrg::DelTimeGroup(const char* sIP,  DEV_TIMEGROUP_ARRAY& cTGArray)
{
  return  CPM_DELTimeGroup(sIP, cTGArray, DEV_ANY_TIME);
}

BOOL CFaceDevMrg::DLoadRight(const char* sIP,  DEV_RIGHT_ARRAY& cRTArray)
{
  return CPM_DLRight(sIP, cRTArray);
}

BOOL CFaceDevMrg::DelRight(const char* sIP,  DEV_RIGHT_ARRAY& cRTArray)
{
  return CPM_DELRight(sIP, cRTArray);
}

BOOL CFaceDevMrg::DLoadUserGroup(const char* sIP,  DEV_USERGROUP_ARRAY& cUGArray)
{
  return CPM_DLUserGroup(sIP, cUGArray);
}

BOOL CFaceDevMrg::DelUserGroup(const char* sIP,  DEV_USERGROUP_ARRAY& cUGArray)
{
  return CPM_DELUserGroup(sIP, cUGArray);
}

BOOL CFaceDevMrg::EnmergCloseDoor(const char* sIP)
{
  return CPM_ULOpenDoorState(sIP, DEV_DOOR_CLOSE);
}

BOOL CFaceDevMrg::EnmergOpenDoor(const char* sIP)
{
  return CPM_ULOpenDoorState(sIP, DEV_DOOR_OPEN);
}

BOOL CFaceDevMrg::ReocoverDoor(const char* sIP)
{
  return CPM_ULOpenDoorState(sIP, DEV_DOOR_NOMAL);
}

BOOL CFaceDevMrg::RemoteOpenDoor(const char* sIP)
{
  return CPM_ULRemoteOpenDoor(sIP, TRUE);
}

BOOL CFaceDevMrg::RemoteCloseDoor(const char* sIP)
{
  return CPM_ULRemoteOpenDoor(sIP, FALSE);
}

BOOL CFaceDevMrg::DevIOControl(const char* sIP,  DEV_IOCTRL& ioctrl)
{
  return CPM_IOCtrl(sIP, ioctrl);
}

BOOL CFaceDevMrg::Formate(const char* sIP)
{
  return CPM_ULFormat(sIP);
}

BOOL CFaceDevMrg::UpdateApp(char*  sIP,  DEV_APPUPDATE& cAppData)
{
  return CPM_ULUpdate(sIP, cAppData);
}

BOOL CFaceDevMrg::OpenVideo(char*  sIP, void* hwnd, frame_callback_t frame_rgb)
{
  return CPM_StartVideo(sIP, hwnd, frame_rgb);
}

BOOL CFaceDevMrg::CloseVideo(char*  sIP)
{
  return CPM_StopVideo(sIP);
}

BOOL CFaceDevMrg::DLoadUserEx(char* sIP, DEV_CID* pUserIDArray, int nIDCount)
{
  return CPM_DLUserEx(sIP, pUserIDArray, nIDCount, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
}

BOOL CFaceDevMrg::DelUserBatch(const char* sIP, DEV_CID* pUserIDArray, int nIDCount)
{
  return CPM_DELUserEx(sIP, pUserIDArray, nIDCount);
}

BOOL CFaceDevMrg::DLoadRecordEx(char* sIP,  DEV_DL_RECORD_RANGE& cRange)
{
  return CPM_DLRecEx(sIP, cRange);
}

BOOL CFaceDevMrg::Reboot(const char* sIP)
{
  return CPM_RebootDev(sIP);
}

BOOL CFaceDevMrg::GetNetConfigInfo(const char* sIP)
{
  return CPM_NetPro(sIP, NULL);
}