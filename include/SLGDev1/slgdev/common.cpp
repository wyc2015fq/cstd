
#include "../slgdev_private.h"

void CBase64Convert__enBase64Help(unsigned char chasc[3], unsigned char chuue[4])
{
    int i, k = 2;
    unsigned char t = 0;

    for (i = 0; i < 3; i++)
    {
        *(chuue + i) = *(chasc + i) >> k;
        *(chuue + i) |= t;
        t = *(chasc + i) << (8 - k);
        t >>= 2;
        k += 2;
    }

    *(chuue + 3) = *(chasc + 2) & 63;

    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) <= 128) && (*(chuue + i) <= 25))
        {
            *(chuue + i) += 65; // 'A'-'Z'
        }

        else if ((*(chuue + i) >= 26) && (*(chuue + i) <= 51))
        {
            *(chuue + i) += 71; // 'a'-'z'
        }

        else if ((*(chuue + i) >= 52) && (*(chuue + i) <= 61))
        {
            *(chuue + i) -= 4; // 0-9
        }

        else if (*(chuue + i) == 62)
        {
            *(chuue + i) = 43; // +
        }

        else if (*(chuue + i) == 63)
        {
            *(chuue + i) = 47; // /
        }

    }

}

void CBase64Convert__deBase64Help(unsigned char chuue[4], unsigned char chasc[3])
{
    int i, k = 2;
    unsigned char t = 0;

    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) >= 65) && (*(chuue + i) <= 90))
        {
            *(chuue + i) -= 65; // 'A'-'Z' -> 0-25
        }

        else if ((*(chuue + i) >= 97) && (*(chuue + i) <= 122))
        {
            *(chuue + i) -= 71; // 'a'-'z' -> 26-51
        }

        else if ((*(chuue + i) >= 48) && (*(chuue + i) <= 57))
        {
            *(chuue + i) += 4; // '0'-'9' -> 52-61
        }

        else if (*(chuue + i) == 43)
        {
            *(chuue + i) = 62; // + -> 62
        }

        else if (*(chuue + i) == 47)
        {
            *(chuue + i) = 63; // / -> 63
        }

        else if (*(chuue + i) == 61)
        {
            *(chuue + i) = 0; // = -> 0 Note: 'A'和'='都对应了0
        }

    }

    for (i = 0; i < 3; i++)
    {
        *(chasc + i) = *(chuue + i) << k;
        k += 2;
        t = *(chuue + i + 1) >> (8 - k);
        *(chasc + i) |= t;
    }

}

int CBase64Convert_enBase64(const char* inbuf, size_t inbufLen, char* outStr, int maxOutLen)
{
    unsigned char* outStr1 = (unsigned char*)outStr;
    unsigned char* outStr2 = (unsigned char*)outStr + maxOutLen;
    unsigned char* out = outStr1;
    unsigned char in[8];
    size_t blocks = inbufLen / 3;

    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = inbuf[i * 3];
        in[1] = inbuf[i * 3 + 1];
        in[2] = inbuf[i * 3 + 2];
        CBase64Convert__enBase64Help(in, out);
        out += 4;
    }

    if (inbufLen % 3 == 1)
    {
        in[0] = inbuf[inbufLen - 1];
        in[1] = 0;
        in[2] = 0;
        CBase64Convert__enBase64Help(in, out);
        out += 2;
        *out++ = '=';
        *out++ = '=';
    }

    else if (inbufLen % 3 == 2)
    {
        in[0] = inbuf[inbufLen - 2];
        in[1] = inbuf[inbufLen - 1];
        in[2] = 0;
        CBase64Convert__enBase64Help(in, out);
        out += 3;
        *out++ = '=';
    }

    *out = 0;
    return out - outStr1;
}

int CBase64Convert_deBase64_(const char* src, int src_size, char* outbuf)
{
    // Break when the incoming base64 coding is wrong
    if ((src_size % 4) != 0)
    {
        return 0;
    }

    unsigned char in[4];
    unsigned char out[3];
    size_t blocks = src_size / 4;

    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = src[i * 4];
        in[1] = src[i * 4 + 1];
        in[2] = src[i * 4 + 2];
        in[3] = src[i * 4 + 3];
        CBase64Convert__deBase64Help(in, out);
        outbuf[i * 3] = out[0];
        outbuf[i * 3 + 1] = out[1];
        outbuf[i * 3 + 2] = out[2];
    }

    int length = src_size / 4 * 3;

    if (src[src_size - 1] == '=')
    {
        length--;

        if (src[src_size - 2] == '=')
        {
            length--;
        }

    }

    return length;
}

int CBase64Convert_deBase64(const char* pSrc, int len, char* outbuf)
{
    // Break when the incoming base64 coding is wrong
    if (pSrc == NULL || len == 0 || (len % 4) != 0)
    {
        return 0;
    }

    DEBUG_INFO(("deBase64 len = %d") , len);
    unsigned char inData[4];
    size_t blocks = len / 4;

    for (size_t i = 0; i < blocks; i++)
    {
        memcpy(inData , pSrc + i * 4 , 4);
        CBase64Convert__deBase64Help(inData , (unsigned char*)outbuf + i * 3);
    }

    int length = len / 4 * 3;

    if (pSrc[len - 1] == '=')
    {
        length--;

        if (pSrc[len - 2] == '=')
        {
            length--;
        }

    }

    return length;
}
////////////////////////////////////////////


#if 1
#define IP_ISOK(IP) (0!=memsum1(&(IP), sizeof(IP)))
//bool operator ==(const addr_inx& a, const addr_inx& b){  return 0 == memcmp(&a, &b, sizeof(addr_inx));}

//bool operator <(const addr_inx& a, const addr_inx& b){  return memcmp(&a, &b, sizeof(addr_inx)) < 0;}

static unsigned memsum1(const void* x, int n)
{
    int i, ss = 0;
    const unsigned char* ux = (const unsigned char*)x;

    for (i = 0; i < n; ++i)
    {
        ss += ux[i];
    }

    return ss;
}

#if 0
//点分IP转换为网络字节序
static int DotIpToLong1(const char* ip, addr_inx* addr)
{
    int fa = 0, t = 0, RetVal = -1;
    struct addrinfo Hints, *AddrInfo;

    if (!(t = StrIsIP(ip)))
    {
        return t;
    }

    sock_init();
    /* hints is used after zero cleared */
    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family = 2 == t ? AF_INET6 : AF_INET;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
    RetVal = getaddrinfo(ip, "9999", &Hints, &AddrInfo);

    if (AddrInfo)
    {
        addr->ai_flags = AddrInfo->ai_flags;
        addr->ai_family = AddrInfo->ai_family;
        addr->ai_socktype = AddrInfo->ai_socktype;
        addr->ai_protocol = AddrInfo->ai_protocol;
        addr->ai_addrlen = AddrInfo->ai_addrlen;
        addr->ai_addr = *AddrInfo->ai_addr;
    }

    return t;
}

#endif

//点分IP转换为网络字节序
static int DotIpToLong(const char* ip, addr_in* out)
{
    return net_addr(ip, 0, out);
}

//网络字节序long转换为点分IP[INET6_ADDRSTRLEN]
static int LongToDotIp(char* ip, int len, const addr_in* addr)
{
    if (NULL == ip || !IP_ISOK(addr))
    {
        return 0;
    }

    return net_ntop((sockaddr*)addr, ip, len);
}

#else
#define IP_ISOK(IP) (0!=(IP))
//点分IP转换为网络字节序
static int DotIpToLong1(const char* ip, unsigned int* pout)
{
    if (!StrIsIP(ip))
    {
        return 0;
    }

    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    unsigned int rs = 0;
    unsigned char* pByte = (BYTE*)&rs;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    pByte[0] = a;
    pByte[1] = b;
    pByte[2] = c;
    pByte[3] = d;
    *pout = rs;
    return 16;
}

static int DotIpToLong(const char* ip, unsigned int* pout)
{
    addr_in out[1];
    net_addr(ip, 0, out);

    if (AF_INET == out->family)
    {
        struct sockaddr_in* p = (struct sockaddr_in*)out;
        //unsigned int aa = 0;
        //DotIpToLong1("192.168.1.150", &aa);
        *pout = p->sin_addr.s_addr;
        return 16;
    }

    return 0;
}

unsigned int DotIpToLong_v2(const char* ip)
{
    unsigned int a = 0 ;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

//网络字节序 转换为点分IP
static BOOL LongToDotIp(char* ip, int len, addr_inx* l)
{
    if (NULL == ip || *l == 0)
    {
        return;
    }

    BYTE* pByte = (BYTE*)l;
    _snprintf(ip, len, "%d.%d.%d.%d", pByte[0], pByte[1], pByte[2], pByte[3]);
}

#endif

#define MAX_TIME_INTERVAL 30
#define VSOCKET ((SOCKET)0xffffffff)
//enum { LINK_FAIL = 0, LINK_OK, LINK_ALREADY };
class DspDevManager;
#include "devdatestruct.h"
#include "DevSock.h"

BOOL DevTimeGroup_set(const CDevSock* s, const DEV_TIMEGROUP_ARRAY& cArray, int nType, DEV_OPER_FLAG eFlag, int nSeq)
{
    int ret, nLen = sizeof(CARRAY) + sizeof(DSPTIMEG) * (cArray.m_nCount);
    char* pbBuf = (char*)malloc(nLen);
    int m_SCmd = 0, m_CFlag = 0;

    switch (eFlag)
    {
    case DEV_OPER_UPLOAD:
        m_SCmd = CMD_S_SET;
        break;

    case DEV_OPER_DOWNLOAD:
        m_SCmd = CMD_S_GET;
        break;

    case DEV_OPER_DELETE:
        m_SCmd = CMD_S_DEL;
        break;
    }

    switch (nType)
    {
    case DEV_NORMAL:
        m_CFlag = F_DL_NORMAL_TG;
        break;

    case DEV_HOLIDY:
        m_CFlag = F_DL_HOLIDAY_TG;
        break;

    case DEV_ANY_TIME:
        m_CFlag = F_DL_NORMAL_TG | F_DL_HOLIDAY_TG;
        break;

    default:
        break;
    }

    CARRAY* pArray = (CARRAY*)(pbBuf);
    pArray->m_Count = cArray.m_nCount;
    DSPTIMEG* pTG = (DSPTIMEG*)(pArray->m_Block);

    for (int i = 0; i < pArray->m_Count; ++i)
    {
        pTG[i].m_TGID = (cArray.m_pTGArray)[i].m_TGID;
        memcpy(&(pTG[i].m_Start), &((cArray.m_pTGArray)[i].m_Start), sizeof(DATETIME));
        memcpy(&(pTG[i].m_End), &((cArray.m_pTGArray)[i].m_End), sizeof(DATETIME));
        pTG[i].m_TGType = (cArray.m_pTGArray)[i].m_TGType;
        pTG[i].m_CheckFlag = (cArray.m_pTGArray)[i].m_CheckFlag;
        pTG[i].m_WeekFlag = (cArray.m_pTGArray)[i].m_WeekFlag;
    }

    ret = s->sendNCMD(CMD_M_TGROUP, m_SCmd, m_CFlag, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}

BOOL DevRight_set(const CDevSock* s, const DEV_RIGHT_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(DSPRIGHT) * (cArray.m_nCount);
    char* pbBuf = (char*)malloc(nLen);
    assert(pbBuf);
    int m_SCmd = 0;

    switch (eFlag)
    {
    case DEV_OPER_UPLOAD:
        m_SCmd = CMD_S_SET;
        break;

    case DEV_OPER_DOWNLOAD:
        m_SCmd = CMD_S_GET;
        break;

    case DEV_OPER_DELETE:
        m_SCmd = CMD_S_DEL;
        break;
    }

    CARRAY* pArray = (CARRAY*)(pbBuf);
    DSPRIGHT* pRG = (DSPRIGHT*)(pArray->m_Block);
    pArray->m_Count = cArray.m_nCount;

    for (int i = 0; i < pArray->m_Count; ++i)
    {
        pRG[i].m_RightID = (cArray.m_pRtArray)[i].m_RightID;
        memcpy(pRG[i].m_TimeGID, (cArray.m_pRtArray)[i].m_TimeGID, sizeof(pRG[i].m_TimeGID));
        pRG[i].m_Mode = cArray.m_pRtArray[i].m_bHolidyValid ? DEV_CHECK_HOLIDY : 0;
        pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionLock ? DEV_ACTION_LOCK : 0;
        pRG[i].m_Mode |= cArray.m_pRtArray[i].m_bActionOutPut ? DEV_ACTION_OUTPUT : 0;
    }

    ret = s->sendNCMD(CMD_M_DSPRIGHT, m_SCmd, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}

BOOL DevID_set(const CDevSock* s, int nClass, int nSeq) //下载所有
{
    return s->sendNCMD(nClass, CMD_S_GET, F_DL_ID | F_DL_ID_ALL, nSeq, 0, 0);
}

BOOL DevID_set(const CDevSock* s, int nClass, const DEV_TIMESEG& cTimeSeg, int nSeq) //时间段下载
{
    return s->sendNCMD(nClass, CMD_S_GET, F_DL_ID | F_ID_TIME_RANGE, nSeq, &cTimeSeg, sizeof(TIMESE));
}

BOOL DevID_set(const CDevSock* s, int nClass, int nFlag, const DEV_DL_RECORD_RANGE& cRange, int nSeq) //区间段下载
{
    int m_CFlag = (nFlag & 0x80000000) ? 0 : (F_DL_ID | F_ID_RANGE);

    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }

    return s->sendNCMD(CMD_M_RANGE, CMD_S_GET, m_CFlag, nSeq, &cRange, sizeof(RECORDRANGE));
}

BOOL CopyDevUser2DspUser(USER* dspUser , const DEV_USER* devUser)
{
    memset(dspUser , 0, sizeof(dspUser));
    memcpy(dspUser, devUser, OFF_SET(USER, m_UserType));
    dspUser->m_UserType = devUser->m_UserType;

    if (devUser->m_FeatLen)
    {
        if ((devUser->m_UserFlag & DEV_USER_FEAT_BASE64) && devUser->m_FeatData[3])
        {
            dspUser->m_FeatBytes = CBase64Convert_deBase64(devUser->m_FeatData , devUser->m_FeatLen , dspUser->m_Block);
        }

        else
        {
            memcpy(dspUser->m_Block, devUser->m_FeatData, devUser->m_FeatLen);
        }

    }

    if (devUser->m_PicLen)
    {
        if (devUser->m_UserFlag & DEV_USER_IMAGE_BASE64)
        {
            dspUser->m_PicBytes = CBase64Convert_deBase64(devUser->m_PicData , devUser->m_PicLen , dspUser->m_Block + dspUser->m_FeatBytes);
        }

        else
        {
            memcpy(dspUser->m_Block + dspUser->m_FeatBytes , devUser->m_PicData, devUser->m_PicLen);
        }

    }

    return 0;
}

BOOL DevUser_set(const CDevSock* s, const CID& cID, int nFlag, int nSeq) //获取
{
    int m_CFlag = 0;

    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }

    if (nFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= F_DL_FEAT;
    }

    return s->sendNCMD(CMD_M_USER, CMD_S_GET, m_CFlag, nSeq, &cID, sizeof(cID));
}

BOOL DevUser_set(const CDevSock* s, const CID& cID, int nSeq) //用户删除操作
{
    return s->sendNCMD(CMD_M_USER, CMD_S_DEL, 0, nSeq, &cID, sizeof(cID));
}

BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, BOOL bAddFlag, int nSeq) //用户添加操作
{
    int ret = 0, nLen = sizeof(USER) + (cUser).m_FeatLen + (cUser).m_PicLen + 20;
    char* pbBuf = (char*)malloc(nLen);
    USER* pUser = (USER*)(pbBuf);
    CopyDevUser2DspUser(pUser , &cUser);
    nLen = USER_STEP(*pUser);
    ret = s->sendNCMD(CMD_M_USER, bAddFlag ? CMD_S_SET : CMD_S_REALTIME_CAP, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}

BOOL DevUser_set(const CDevSock* s, const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad) //用户实时添加操作
{
    int ret = 0, nLen = sizeof(USER) + (cUser).m_FeatLen + (cUser).m_PicLen + 20;
    char* pbBuf = (char*)malloc(nLen);
    int m_CFlag = 0;

    if (cLoad.m_bApeendID)
    {
        m_CFlag |= F_DL_APPENDID;
    }

    if (cLoad.m_LoadFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }

    if (cLoad.m_LoadFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= F_DL_FEAT;
    }

    USER* pUser = (USER*)(pbBuf);
    CopyDevUser2DspUser(pUser , &cUser);
    nLen = USER_STEP(*pUser);
    ret = s->sendNCMD(CMD_M_USER, CMD_S_REALTIME_LOAD, m_CFlag, cLoad.m_ReqSource, pbBuf, nLen);
    free(pbBuf);
    return ret;
}

// 用户批量操作
BOOL DevUserEx_UserAdd(const CDevSock* s, const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
    int ret = 0, nLen = 0;

    if (nFlag & DL_DEV_USER_TEXT)
    {
        int binLen = sizeof(CARRAY) , i = 0;

        for (; i < cUsers.m_nCount; ++i)
        {
            binLen += ALIGN4(sizeof(USER) + cUsers.m_pUserArray[i].m_PicLen + cUsers.m_pUserArray[i].m_FeatLen);
        }

        char* pbBuf = NULL;
        pbBuf = (char*)malloc(binLen);
        CARRAY* pArray = (CARRAY*)(pbBuf);
        pArray->m_Count = cUsers.m_nCount;
        USER* pData = (USER*)(pArray->m_Block);
        DEV_USER* pCurUser = cUsers.m_pUserArray;

        for (i = 0; i < cUsers.m_nCount; ++i)
        {
            CopyDevUser2DspUser(pData , &pCurUser[i]);
            nLen += USER_STEP(*pData);
            pData = (USER*)((char*)pData + USER_STEP(*pData));
        }

        ret = s->sendNCMD(CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nSeq, pbBuf, binLen);
        free(pbBuf);
    }

    else
    {
        assert(false);
    }

    return ret;
}

// 用户批量删除操作
BOOL DevUserEx_UserDel(const CDevSock* s, DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
    char* pbBuf = (char*)malloc(nLen);
    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    pArray->m_Count = nIDCount;
    DEV_CID* pIDTmp = pUserIDArray;

    for (int i = 0; i < nIDCount; ++i)
    {
        ZeroMemory(pID , sizeof(CID));
        strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
        ++pID;
        ++pIDTmp;
    }

    ret = s->sendNCMD(CMD_M_USER_EX, CMD_S_DEL, 0, nSeq, pbBuf, nLen);
    free(pbBuf);
    return 0;
}

// 用户批量下载操作
BOOL DevUserEx_UserDown(const CDevSock* s, DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
    int ret = 0, nLen = sizeof(CARRAY) + sizeof(CID) * nIDCount;
    char* pbBuf = (char*)malloc(nLen);
    int m_CFlag = 0;

    if (nFlag & DL_DEV_USER_TEXT)
    {
        m_CFlag |= USER_TEXT;
    }

    if (nFlag & DL_DEV_USER_FEAT)
    {
        m_CFlag |= DL_FEAT;
    }

    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= DL_IMAGE;
    }

    CARRAY* pArray = (CARRAY*)(pbBuf);
    CID* pID = (CID*)(pArray->m_Block);
    pArray->m_Count = nIDCount;
    DEV_CID* pIDTmp = pUserIDArray;

    for (int i = 0; i < nIDCount; ++i)
    {
        ZeroMemory(pID , sizeof(CID));
        strncpy(pID->m_ID , pIDTmp->m_ID , sizeof(CID) - 1);
        ++pID;
        ++pIDTmp;
    }

    ret = s->sendNCMD(CMD_M_USER_EX, CMD_S_GET, m_CFlag, nSeq, pbBuf, nLen);
    free(pbBuf);
    return ret;
}

BOOL DevCImage_set(const CDevSock* s, const DEV_DL_RECORD_RANGE& cRange, int nFlag, int nSeq)
{
    int m_CFlag = 0;

    if (nFlag & DL_DEV_PIC)
    {
        m_CFlag |= F_DL_PIC;
    }

    return s->sendNCMD(CMD_M_CIMAGE, CMD_S_GET, m_CFlag, nSeq, &cRange, sizeof(DEV_DL_RECORD_RANGE));
}

BOOL DevUserGroup_set(const CDevSock* s, const DEV_USERGROUP_ARRAY& cArray, DEV_OPER_FLAG eFlag, int nSeq)
{
  int ret, nLen = sizeof(CARRAY) + sizeof(USERGROUP) * cArray.m_nCount;
  char* pbBuf = (char*)malloc(nLen);
  assert(pbBuf);
  int m_SCmd = 0;
  
  switch (eFlag)
  {
  case DEV_OPER_UPLOAD:
    m_SCmd = CMD_S_SET;
    break;
    
  case DEV_OPER_DOWNLOAD:
    m_SCmd = CMD_S_GET;
    break;
    
  case DEV_OPER_DELETE:
    m_SCmd = CMD_S_DEL;
    break;
  }
  
  CARRAY* pArray = (CARRAY*)(pbBuf);
  pArray->m_Count = cArray.m_nCount;
  USERGROUP* pUG = (USERGROUP*)(pArray->m_Block);
  DEV_USERGROUP* pTmp = cArray.m_pUGArray;
  
  for (int i = 0; i < pArray->m_Count; ++i)
  {
    pUG->m_GroupID = pTmp->m_GroupID;
    pUG->m_GroupFlag = pTmp->m_bGroupOrder ;
    pUG->m_NormalValid = pTmp->m_NormalValid;
    pUG->m_ForceValid = pTmp->m_ForceValid;
    memcpy(pUG->m_NormalUsers, pTmp->m_NormalUsers, sizeof(pUG->m_NormalUsers));
    memcpy(pUG->m_ForceUsers, pTmp->m_ForceUsers, sizeof(pUG->m_ForceUsers));
    pUG++;
    pTmp++;
  }
  
  ret = s->sendNCMD(CMD_M_UGROUP, m_SCmd, 0, nSeq, pbBuf, nLen);
  free(pbBuf);
  return ret;
}

void DspRec2UspRec(RECORD& dsp, DEV_RECORD& usp, BOOL realTime)
{
  usp.m_RecType = DEV_REC_TYPE(dsp.m_RecType);
  usp.m_bRealTime = realTime;
  usp.m_Score = dsp.m_Score;
  usp.m_PicLen = dsp.m_PicBytes;
  usp.m_VerifyMode = dsp.m_VerifyMode;
  usp.m_ConcretSource = dsp.m_Source;
  usp.m_Source = DEV_DOOR_SOURCE;
  usp.m_PicData = dsp.m_Block;
  memcpy(usp.m_SerialID.m_ID, dsp.m_SerialID.m_ID, sizeof(CID));
  memcpy(usp.m_UserID.m_ID, dsp.m_UserID.m_ID, sizeof(CID));
  memcpy(usp.m_ChangID.m_ID, dsp.m_ChangID.m_ID, sizeof(CID));
  memcpy(usp.m_AppendID.m_ID, dsp.m_AppendID.m_ID, sizeof(CID));
  memcpy(usp.m_UserName.m_ID, dsp.m_UserName.m_ID, sizeof(CID));
  memcpy(usp.m_Department.m_ID, dsp.m_Department.m_ID, sizeof(CID));
  memcpy(&(usp.m_RecTime), &(dsp.m_RecTime), sizeof(DATETIME));
}
