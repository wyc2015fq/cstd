// DevUserCommand.cpp: implementation of the DevUserCommand class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "DevUser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
REG_OBJECT(CMD_M_USER, DevUser, true);
REG_OBJECT(CMD_M_USER_EX, DevUserEx, true);

#define OFF_SET(T, member) (size_t( &(((T*)0)->member)))

void CopyDevUser2DspUser(USER& dspUser , const DEV_USER& devUser)
{
  ZeroMemory(&dspUser , sizeof(dspUser));
  memcpy(&dspUser, &devUser, OFF_SET(USER, m_UserType));
  dspUser.m_UserType = devUser.m_UserType;

  if (devUser.m_FeatLen) {
    if ((devUser.m_UserFlag & DEV_USER_FEAT_BASE64) && devUser.m_FeatData[3]) {
      dspUser.m_FeatBytes = CBase64Convert::deBase64(devUser.m_FeatData , devUser.m_FeatLen , dspUser.m_Block);
    }
    else {
      memcpy(dspUser.m_Block, devUser.m_FeatData, devUser.m_FeatLen);
    }
  }

  if (devUser.m_PicLen) {
    if (devUser.m_UserFlag & DEV_USER_IMAGE_BASE64) {
      dspUser.m_PicBytes = CBase64Convert::deBase64(devUser.m_PicData , devUser.m_PicLen ,  dspUser.m_Block + dspUser.m_FeatBytes);
    }
    else {
      memcpy(dspUser.m_Block + dspUser.m_FeatBytes , devUser.m_PicData, devUser.m_PicLen);
    }
  }
}

DevUser::DevUser(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevUser::DevUser(const char* cszIP, const CID& cID, int nFlag, int nSeq, char* pbBuf, unsigned int& nLen)   //获取
{
  assert(pbBuf);
  NCMD* pCMD    = (NCMD*)pbBuf;
  pCMD->m_MCmd  = CMD_M_USER;
  pCMD->m_SCmd  = CMD_S_GET;
  pCMD->m_CFlag = 0;

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    pCMD->m_CFlag |= F_DL_FEAT;
  }

  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  memcpy(pCMD + 1, &cID, sizeof(cID));
  nLen = sizeof(NCMD) + sizeof(CID);
}

DevUser::DevUser(const char* cszIP, const CID& cID, int nSeq, char* pbBuf, unsigned int& nLen)  //用户删除操作
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_USER;
  pCMD->m_SCmd   = CMD_S_DEL;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  memcpy(pCMD + 1, &cID, sizeof(cID));
  nLen = sizeof(NCMD) + sizeof(CID);
}

DevUser::DevUser(const char* cszIP, const DEV_USER& cUser, BOOL bAddFlag, int nSeq, char* pbBuf, unsigned int& nLen)   //用户添加操作
{
  assert(pbBuf);
  NCMD* pCMD     = (NCMD*)pbBuf;
  pCMD->m_MCmd   = CMD_M_USER;
  pCMD->m_SCmd   = bAddFlag ? CMD_S_SET : CMD_S_REALTIME_CAP;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  USER* pUser    = (USER*)(pCMD + 1);
  CopyDevUser2DspUser(*pUser , cUser);
  nLen = sizeof(NCMD) + USER_STEP(*pUser);
}

DevUser::DevUser(const char* cszIP, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad, char* pbBuf, unsigned int& nLen)  //用户实时添加操作
{
  assert(pbBuf);
  NCMD* pCMD   = (NCMD*)pbBuf;
  pCMD->m_MCmd = CMD_M_USER;
  pCMD->m_SCmd = CMD_S_REALTIME_LOAD;
  pCMD->m_CFlag = 0;

  if (cLoad.m_bApeendID) {
    pCMD->m_CFlag |= F_DL_APPENDID;
  }

  if (cLoad.m_LoadFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= F_DL_PIC;
  }

  if (cLoad.m_LoadFlag & DL_DEV_USER_FEAT) {
    pCMD->m_CFlag |= F_DL_FEAT;
  }

  pCMD->m_Param1 = cLoad.m_ReqSource;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  USER* pUser    = (USER*)(pCMD + 1);
  CopyDevUser2DspUser(*pUser , cUser);
  nLen = sizeof(NCMD) + USER_STEP(*pUser);
}

void DevUser::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  assert(pCMD);

  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    UserAdd(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    UserDel(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_REALTIME_LOAD:
    UserRtLoad(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
  case CMD_S_REALTIME_SEND:
  case S_UPDATE_SEND:
    UserDown(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_REALTIME_CAP:
    UserRemoteCap(cszIP, pCMD, bServer, pvClientData);
    break;

  default:
    break;
  }
}

void DevUser::UserAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_USER user = {0};
  memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
  OnDevCmd(cszIP, NULL, DEV_USER_OPER, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUser::UserDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_USER user = {0};
  memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
  OnDevCmd(cszIP, NULL, DEV_USER_OPER, DEV_OPER_DELETE, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUser::UserDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_USER user = {0};

  if (0 == GET_ERR(pCMD->m_CFlag)) {
    USER* pUser = (USER*)(pCMD + 1);
    memcpy(&user, pUser, OFF_SET(USER, m_UserType));
    user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
    user.m_FeatData = pUser->m_Block;                //特征数据
    user.m_PicData  = pUser->m_Block + pUser->m_FeatBytes; //照片数据

    switch (pCMD->m_SCmd) {
    case CMD_S_REALTIME_SEND:
      user.m_bRTUser = 1;
      break;

    case CMD_S_UPDATE_SEND:
      user.m_bRTUser = 2;
      break;
    }
  }
  else if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    memcpy(user.m_UserID.m_ID, pCMD + 1, sizeof(CID));
  }

  OnDevCmd(cszIP, NULL, DEV_USER_OPER, DEV_OPER_DOWNLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUser::UserRtLoad(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_USER_REAL_LOAD mode = {0};

  if (pCMD->m_CFlag & F_DL_PIC) {
    mode.m_LoadFlag |= DL_DEV_PIC;
  }

  if (pCMD->m_CFlag & F_DL_FEAT) {
    mode.m_LoadFlag |= DL_DEV_USER_FEAT;
  }

  mode.m_ReqSource = pCMD->m_Param1;
  mode.m_bApeendID = (pCMD->m_CFlag & F_DL_APPENDID) ? TRUE : FALSE;
  memcpy(mode.m_ID.m_ID, pCMD + 1, sizeof(CID));
  OnDevCmd(cszIP, NULL, DEV_USER_RT_OPER, DEV_OPER_UPLOAD, &mode, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

void DevUser::UserRemoteCap(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_USER user = {0};

  if (0 == pCMD->m_Param1) {
    if (0 == GET_ERR(pCMD->m_CFlag)) {
      USER* pUser = (USER*)(pCMD + 1);
      memcpy(&user, pUser, OFF_SET(USER, m_UserType));
      user.m_UserType = (DEV_USER_TYPE)pUser->m_UserType;
      user.m_FeatData = pUser->m_Block;                //特征数据
      user.m_PicData  = pUser->m_Block + pUser->m_FeatBytes; //照片数据
      user.m_bRTUser = 1;
    }
  }

  OnDevCmd(cszIP, NULL, DEV_USER_REMOTE_CAP, DEV_OPER_UPLOAD, &user, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
}

DevUserEx::DevUserEx(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  ProcessNCMD(cszIP, pCMD, bServer, pvClientData);
}

DevUserEx::DevUserEx(const char* cszIP, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen) //用户批量操作
{
  UserAdd(cszIP, cUsers, nFlag, nSeq, pbBuf, nLen);
}

DevUserEx::DevUserEx(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen) //用户批量下载操作
{
  UserDown(cszIP, pUserIDArray, nIDCount, nFlag, nSeq, pbBuf, nLen);
}

DevUserEx::DevUserEx(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen) //用户批量删除操作
{
  UserDel(cszIP, pUserIDArray, nIDCount, nSeq, pbBuf, nLen);
}

void DevUserEx::UserAdd(const char* cszIP, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen)
{
  pbBuf = NULL;
  nLen  = 0;

  if (nFlag & DL_DEV_USER_TEXT) {
    int binLen = 0 , i = 0;

    for (; i < cUsers.m_nCount; ++i) {
      binLen += ALIGN4(sizeof(USER) + cUsers.m_pUserArray[i].m_PicLen + cUsers.m_pUserArray[i].m_FeatLen);
    }

    pbBuf = CMemPool::GetInstance().GetMem(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + binLen), true);
    nLen  = sizeof(NCMD) + sizeof(CARRAY);

    NCMD* pCMD     = (NCMD*)(pbBuf + OFFSET_SIZE);
    pCMD->m_MCmd   = CMD_M_USER_EX;
    pCMD->m_SCmd   = CMD_S_SET ;
    pCMD->m_Param1 = nSeq;
    pCMD->m_Param2 = DotIpToLong(cszIP);;
    pCMD->m_CFlag  = USER_TEXT;

    CARRAY* pArray     = (CARRAY*)(pCMD + 1);
    pArray->m_Count    = cUsers.m_nCount;
    USER* pData        = (USER*)(pArray->m_Block);
    DEV_USER* pCurUser = cUsers.m_pUserArray;

    for (i = 0; i < cUsers.m_nCount; ++i) {
      CopyDevUser2DspUser(*pData , pCurUser[i]);
      nLen += USER_STEP(*pData);
      pData = (USER*)((char*)pData + USER_STEP(*pData));
    }
  }
  else {
    assert(false);
  }
}

void DevUserEx::UserDel(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nSeq, char*& pbBuf, unsigned int& nLen)
{
  pbBuf = CMemPool::GetInstance().GetMem(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount), true);
  nLen  = sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount;

  NCMD* pCMD     = (NCMD*)(pbBuf + OFFSET_SIZE);
  pCMD->m_MCmd   = CMD_M_USER_EX;
  pCMD->m_SCmd   = CMD_S_DEL ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;
  CARRAY* pArray = (CARRAY*)(pCMD + 1);
  CID* pID = (CID*)(pArray->m_Block);
  pArray->m_Count = nIDCount;
  DEV_CID* pIDTmp = pUserIDArray;

  for (int i = 0; i < nIDCount; ++i) {
    ZeroMemory(pID , sizeof(CID));
    strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
    ++pID;
    ++pIDTmp;
  }
}

void DevUserEx::UserDown(const char* cszIP, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq, char*& pbBuf, unsigned int& nLen)
{
  pbBuf = CMemPool::GetInstance().GetMem(ALIGN4(OFFSET_SIZE + sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount), true);
  nLen  = sizeof(NCMD) + sizeof(CARRAY) + sizeof(CID) * nIDCount;

  NCMD* pCMD     = (NCMD*)(pbBuf + OFFSET_SIZE);
  pCMD->m_MCmd   = CMD_M_USER_EX;
  pCMD->m_SCmd   = CMD_S_GET ;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(cszIP);
  pCMD->m_CFlag  = 0;

  if (nFlag & DL_DEV_USER_TEXT) {
    pCMD->m_CFlag |= USER_TEXT;
  }

  if (nFlag & DL_DEV_USER_FEAT) {
    pCMD->m_CFlag |= DL_FEAT;
  }

  if (nFlag & DL_DEV_PIC) {
    pCMD->m_CFlag |= DL_IMAGE;
  }

  CARRAY* pArray = (CARRAY*)(pCMD + 1);
  CID* pID = (CID*)(pArray->m_Block);
  pArray->m_Count = nIDCount;
  DEV_CID* pIDTmp = pUserIDArray;

  for (int i = 0; i < nIDCount; ++i) {
    ZeroMemory(pID , sizeof(CID));
    strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
    ++pID;
    ++pIDTmp;
  }
}

void DevUserEx::ProcessNCMD(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  switch (pCMD->m_SCmd) {
  case CMD_S_SET:
    UserAdd(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_DEL:
    UserDel(cszIP, pCMD, bServer, pvClientData);
    break;

  case CMD_S_GET:
    UserDown(cszIP, pCMD, bServer, pvClientData);
    break;
  }
}

void DevUserEx::UserAdd(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_BATCH_USER users = {0};

  if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    users.m_nCount = pArray->m_Count;

    if (users.m_nCount) {
      users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
      CID* pID = (CID*)(pArray->m_Block);
      DEV_USER* pUser = users.m_pUserArray;

      for (int i = 0; i < pArray->m_Count; ++i) {
        ZeroMemory(pUser , sizeof(DEV_USER));
        strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
        ++pUser;
        ++pID;
      }
    }
  }

  OnDevCmd(cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_UPLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
}

void DevUserEx::UserDel(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_BATCH_USER users;

  if (OPER_ERR_BUSY != GET_ERR(pCMD->m_CFlag)) {
    CARRAY* pArray = (CARRAY*)(pCMD + 1);
    users.m_nCount = pArray->m_Count;

    if (users.m_nCount) {
      users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * users.m_nCount));
      CID* pID = (CID*)(pArray->m_Block);
      DEV_USER* pUser = users.m_pUserArray;

      for (int i = 0; i < pArray->m_Count; ++i) {
        ZeroMemory(pUser , sizeof(DEV_USER));
        strncpy(pUser->m_UserID.m_ID , pID->m_ID , sizeof(CID) - 1);
        ++pUser;
        ++pID;
      }
    }
  }

  OnDevCmd(cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DELETE, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);
  CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
}

void DevUserEx::UserDown(const char* cszIP, NCMD* pCMD, BOOL bServer, void* pvClientData)
{
  DEV_BATCH_USER users = {0};
  CARRAY* pArray       = (CARRAY*)(pCMD + 1);

  if (OPER_SUCCESS == GET_ERR(pCMD->m_CFlag)) {
    users.m_nCount     = pArray->m_Count;
    users.m_pUserArray = (DEV_USER*)CMemPool::GetInstance().GetMem(ALIGN4(sizeof(DEV_USER) * pArray->m_Count));

    if (users.m_pUserArray) {
      if (pCMD->m_CFlag & USER_TEXT) {
        USER* pData = (USER*)(pArray->m_Block);
        DEV_USER* pCurUser = users.m_pUserArray;

        for (int i = 0; i < pArray->m_Count; ++i) {
          memcpy(pCurUser, pData, OFF_SET(USER , m_UserType));
          pCurUser->m_UserType = (DEV_USER_TYPE)pData->m_UserType;
          pCurUser->m_FeatData = pData->m_Block;                   //特征数据
          pCurUser->m_PicData  = pData->m_Block + pData->m_FeatBytes; //照片数据
          pData = (USER*)((char*)pData + USER_STEP(*pData));
          ++pCurUser;
        }
      }
      else {
        assert(false);
      }
    }

    if (pCMD->m_CFlag & BATCH_END) {
      pCMD->m_CFlag = OPER_BATCH_DL_COMPLETE;
    }
  }

  OnDevCmd(cszIP, NULL, DEV_USER_EX_OPER, DEV_OPER_DOWNLOAD, &users, pCMD->m_Param1, GET_ERR(pCMD->m_CFlag), pvClientData);

  if (users.m_pUserArray) {
    CMemPool::GetInstance().FreeMem((char*)users.m_pUserArray);
  }
}


#undef OFF_SET

