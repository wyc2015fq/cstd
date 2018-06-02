
#include "../slgdev_private.h"
#include "yuv2bmp.h"
#include "devsock.h"

// Client.cpp: implementation of the DspDevManager class.
//
BOOL CDevSock::Auth(const DEV_CID& cName, const DEV_CID& cPws, int nFlag, int nSeq)
{
    DEV_AUTH cAuth = {0};
    cAuth.m_UserName = cName;
    cAuth.m_Password = cPws;
    return sendNCMD(CMD_M_AUTH, CMD_S_SET, (nFlag == 2) ? 0 : F_AUTH_VERIFY, nSeq, &cAuth, sizeof(DEV_AUTH));
}

BOOL CDevSock::SetDevRgn(const char* cszGroupIP, int nSeq)
{
    CID id[1] = {0};
    memcpy(id->m_ID, cszGroupIP, strlen(cszGroupIP));
    return sendNCMD(CMD_M_MCAST, CMD_S_SET, 0, nSeq, id, sizeof(CID));
}

BOOL CDevSock::GetDevRgn(int nSeq)
{
    return sendNCMD(CMD_M_MCAST, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::SetDevTime(const DEV_DATETIME& cTime, int nSeq)
{
    return sendNCMD(CMD_M_TIME, CMD_S_SET, 0, nSeq, &cTime, sizeof(DATETIME));
}

BOOL CDevSock::GetDevTime(int nSeq)
{
    return sendNCMD(CMD_M_TIME, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::GetDevInfo(int nSeq)
{
    return sendNCMD(CMD_M_DEVINFO, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::GetDevStatis(int nSeq)
{
    return sendNCMD(CMD_M_STATIS, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::SetDevWorkAtt(const DEV_WORKATT& cAtt, int nSeq)
{
    int ret;
    DEVSET pSet[1] = {0};
    memcpy(pSet->m_DevCtx, cAtt.m_TimeGID, sizeof(cAtt.m_TimeGID));
    pSet->m_DefaultRight = cAtt.m_DefaultRight;
    pSet->m_BaseSet = cAtt.m_BaseSet;
    memcpy(pSet->m_SuperPassword.m_ID, cAtt.m_szSuperPWD.m_ID, sizeof(CID));
    pSet->m_DoorMangetTime = cAtt.m_DoorMangetTime;
    pSet->m_LockTime = cAtt.m_LockTime;
    pSet->m_VerifyMode = cAtt.m_VerifyMode;

    //pSet->m_OutputWG = cAtt.m_bWGOutPut ? OUTPUT_WG_YES : 0x00;
    //pSet->m_OutputWG |= cAtt.m_nWGType ? OUTPUT_WG_34 : 0x00;
    if (cAtt.m_bWGOutPut)
    {
        pSet->m_OutputWG |= (pSet->m_OutputWG & 0x0f) | ((cAtt.m_nWGType & 0x0f) << 4);
    }

    else
    {
        pSet->m_OutputWG |= (pSet->m_OutputWG & 0x0f);
    }

    pSet->m_OutputWG |= cAtt.m_nWGOutType ? OUTPUT_WG_CTX : 0x00;
    pSet->m_OutputWG |= cAtt.m_nWGOutIDType ? OUTPUT_WG_UID : 0x00;
    pSet->m_OutputCtx = cAtt.m_nWGOutContent;
    ret = sendNCMD(CMD_M_DEVSET, CMD_S_SET, 0, nSeq, pSet, sizeof(DEVSET));
    return ret;
}

BOOL CDevSock::SetWapInfo(const WAPINFO* pWapInfo, int nSeq)
{
    return sendNCMD(M_WPA_SET, CMD_S_SET, 0, nSeq, pWapInfo, sizeof(WAPINFO));
}

BOOL CDevSock::GetWapInfo(int nSeq)
{
    return sendNCMD(M_WPA_SET, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::GetDevWorkAtt(int nSeq)
{
    return sendNCMD(CMD_M_DEVSET, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::FormatDev(int nSeq)
{
    return sendNCMD(CMD_M_FORMAT, CMD_S_SET, 0, nSeq, 0, 0);
}

int CDevSock::UpdateApp(const DEV_APPUPDATE& cData, int nSeq)
{
    int ret=1, nLen = sizeof(APPUPDATE) + cData.m_Bytes;

    if (F_PSD_CERTIFI == cData.m_Type)
    {
        ret = pk12_VerifyCert(cData.m_Buf, cData.m_Bytes, cData.m_CtxInfo.m_ID);
    }

    if (ret)
    {
        char* pbBuf = (char*)malloc(nLen);
        memset(pbBuf, 0x0, OFFSET_SIZE + sizeof(NCMD) + sizeof(APPUPDATE));
        //DevProUpdate update(this, cData, nSeq, pbBuf + OFFSET_SIZE, nLen);
        APPUPDATE* pData = (APPUPDATE*)(pbBuf);
        pData->m_Bytes = cData.m_Bytes;
        pData->m_Type = cData.m_Type;

        if (F_PSD_CERTIFI==cData.m_Type)
        {
            CBase64Convert_enBase64(cData.m_CtxInfo.m_ID, strlen(cData.m_CtxInfo.m_ID), pData->m_CtxInfo.m_ID, sizeof(CID));
        }

        else
        {
            memcpy(&pData->m_CtxInfo , &cData.m_CtxInfo, sizeof(CID));
        }

        memcpy(&pData->m_FileName , &cData.m_FileName, sizeof(CID));
        //memcpy(pData, &cData, sizeof(APPUPDATE));
        memcpy(pData->m_Block, cData.m_Buf, cData.m_Bytes);
        ret = sendNCMD(CMD_M_APPUPDATE, CMD_S_SET, 0, nSeq, pbBuf, nLen);
        free(pbBuf);
        ret = ret<=0 ? 1 : 0;
    }

    else
    {
        ret = 2;
    }

    return ret;
}

BOOL CDevSock::AddDevUser(const DEV_USER& cUser, int nSeq)
{
    return DevUser_set(this, cUser, TRUE, nSeq);
}

BOOL CDevSock::AddDevRealTimeUser(const DEV_USER& cUser, const DEV_USER_REAL_LOAD& cLoad)
{
    return DevUser_set(this, cUser, cLoad);
}

BOOL CDevSock::RealTimeUserCap(const DEV_USER& cUser, int nSeq)
{
    return DevUser_set(this, cUser, FALSE, nSeq);
}

BOOL CDevSock::GetDevUser(const DEV_DATA_DL& cDlData, int nSeq)
{
    switch (cDlData.m_DLType)
    {
    case DL_SINGLE :
    {
        return DevUser_set(this, *(CID*)(cDlData.m_DLContent), cDlData.m_DLFlag, nSeq);
    }

    break;

    case DL_ALL:
    {
        return DevID_set(this, CMD_M_USER, nSeq);
    }

    break;

    case DL_SEGTIME:
    {
        return DevID_set(this, CMD_M_USER, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq);
    }

    break;

    default:
        break;
    }

    return 0;
}

BOOL CDevSock::DelDevUser(const DEV_CID& cID, int nSeq)
{
    return DevUser_set(this, (CID&)cID, nSeq);
}

BOOL CDevSock::GetDevRecord(int nClass, const DEV_DATA_DL& cDlData, int nSeq)
{
    switch (cDlData.m_DLType)
    {
    case DL_SINGLE:
    {
        return DevID_set(this, nClass, cDlData.m_DLFlag | 0x80000000, *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq);
    }

    break;

    case DL_RANGE:
    {
        return DevID_set(this, nClass, 0 , *(DEV_DL_RECORD_RANGE*)(cDlData.m_DLContent), nSeq);
    }

    break;

    case DL_ALL:
    {
        return DevID_set(this, nClass, nSeq);
    }

    break;

    case DL_SEGTIME:
    {
        return DevID_set(this, nClass, *(DEV_TIMESEG*)(cDlData.m_DLContent), nSeq);
    }

    break;

    default:
        break;
    }

    return 0;
}

BOOL CDevSock::SetOpenDoorState(DEV_DOOR_STATE eState, int nSeq)
{
    return sendNCMD(CMD_M_DOOR_STATE, CMD_S_SET, eState, nSeq, 0, 0);
}

BOOL CDevSock::GetOpenDoorState(int nSeq /*= -1 */)
{
    return sendNCMD(CMD_M_DOOR_STATE, CMD_S_GET, 0, nSeq, 0, 0);
}

BOOL CDevSock::RemoteOpenDoor(BOOL bOpen, int nSeq)
{
    return sendNCMD(CMD_M_REMOTE_OPEN, CMD_S_SET, bOpen ? F_DOOR_OPEN : F_DOOR_CLOSE, nSeq, 0, 0);
}

BOOL CDevSock::IOCtrl(const DEV_IOCTRL& ioctrl, int nSeq)
{
    return sendNCMD(CMD_M_IOCTRL, CMD_S_SET, 0, nSeq, &ioctrl, sizeof(DEV_IOCTRL));
}

#define TG_MAX_SIZE 256
BOOL CDevSock::SetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, int nSeq)
{
    return DevTimeGroup_set(this, cArray, DEV_ANY_TIME, DEV_OPER_UPLOAD, nSeq);
}

BOOL CDevSock::GetDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
    assert(cArray.m_nCount <= TG_MAX_SIZE);
    return DevTimeGroup_set(this, cArray, eType, DEV_OPER_DOWNLOAD, nSeq);
}

BOOL CDevSock::DelDevTG(const DEV_TIMEGROUP_ARRAY& cArray, DEV_TIMEGROUP_TYPE eType, int nSeq)
{
    assert(cArray.m_nCount <= TG_MAX_SIZE);
    return DevTimeGroup_set(this, cArray, eType, DEV_OPER_DELETE, nSeq);
}

#define RT_MAX_SIZE 256
BOOL CDevSock::SetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= RT_MAX_SIZE);
    return DevRight_set(this, cArray, DEV_OPER_UPLOAD, nSeq);
}

BOOL CDevSock::GetDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= RT_MAX_SIZE);
    return DevRight_set(this, cArray, DEV_OPER_DOWNLOAD, nSeq);
}

BOOL CDevSock::DelDevRT(const DEV_RIGHT_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= RT_MAX_SIZE);
    return DevRight_set(this, cArray, DEV_OPER_DELETE, nSeq);
}

#define UG_MAX_SIZE 256
BOOL CDevSock::SetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= UG_MAX_SIZE);
    return DevUserGroup_set(this, cArray, DEV_OPER_UPLOAD, nSeq);
}

BOOL CDevSock::GetDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= UG_MAX_SIZE);
    return DevUserGroup_set(this, cArray, DEV_OPER_DOWNLOAD, nSeq);
}

BOOL CDevSock::DelDevUserGroup(const DEV_USERGROUP_ARRAY& cArray, int nSeq)
{
    assert(cArray.m_nCount <= UG_MAX_SIZE);
    return DevUserGroup_set(this, cArray, DEV_OPER_DELETE, nSeq);
}

//用户扩展操作
BOOL CDevSock::ULUserEx(const DEV_BATCH_USER& cUsers, int nFlag, int nSeq)
{
    return DevUserEx_UserAdd(this, cUsers, nFlag, nSeq);
}

BOOL CDevSock::DLUserEx(DEV_CID* pUserIDArray, int nIDCount, int nFlag, int nSeq)
{
    return DevUserEx_UserDown(this, pUserIDArray, nIDCount, nFlag, nSeq);
}

BOOL CDevSock::DELUserEx(DEV_CID* pUserIDArray, int nIDCount, int nSeq)
{
    return DevUserEx_UserDel(this, pUserIDArray, nIDCount, nSeq);

}

BOOL CDevSock::DLRecEx(const DEV_DL_RECORD_RANGE& cRange, int nSeq /*= -1*/)
{
    return sendNCMD(CMD_M_RANGE_EX, CMD_S_GET, F_DL_PIC, nSeq, &cRange, sizeof(RECORDRANGE));
}

BOOL CDevSock::RebootDev(int nSeq /*= -1*/)
{
    return sendNCMD(CMD_M_REBOOT, CMD_S_SET, 0, nSeq, 0, 0);
}

BOOL CDevSock::ResetScreen(int nSeq /*= -1*/)
{
    return sendNCMD(CMD_M_RESETSCREEN, CMD_S_SET, 0, nSeq, 0, 0);
}

BOOL CDevSock::DevNetPro(DEV_NETPRO* pNetPro , int nSeq)
{
    return sendNCMD(CMD_M_NETPRO, pNetPro ? CMD_S_SET : CMD_S_GET, 0, nSeq, pNetPro, pNetPro ? sizeof(DEV_NETPRO) : 0);
}

int VideoProCB_defult(const VideoPro* pro)
{
#ifdef WIN32
    const char* winname = (const char*)(pro->m_hWnd);
    BITMAPINFO m_pBitmapInfo[1] = {0};
    RECT m_Rect;
    HWND hWnd = (HWND)(pro->m_hWnd);
    ::GetWindowRect(hWnd, &(m_Rect));
    m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = 24;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 11811;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 11811;
    m_pBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pBitmapInfo->bmiHeader.biHeight = pro->biHeight;
    m_pBitmapInfo->bmiHeader.biWidth = pro->biWidth;
    int lStillWidth = m_Rect.right - m_Rect.left;
    int lStillHeight = m_Rect.bottom - m_Rect.top;
    HDC hdc = ::GetDC(hWnd);
    ::SetStretchBltMode(hdc, COLORONCOLOR);
    ::StretchDIBits(hdc, 0, 0,lStillWidth, lStillHeight,0, 0,pro->biWidth,pro->biHeight,pro->m_pRGBTRANS,m_pBitmapInfo,DIB_RGB_COLORS,SRCCOPY);
    ::ReleaseDC(hWnd, hdc);
#endif
    //printf("w=%d h=%d\n", pro->biWidth, pro->biHeight);
    return 0;
}

BOOL CDevSock::StartVideo(void* hwnd, VideoPro_CallBack VideoCB, int nSeq)
{
    addr_in addrLocal = {0};

    if (NULL == m_Pro)
    {
        if (!SLG_GetLocalIP(&addr, 0, &addrLocal))
        {
            return false;
        }

        SOCKET hVideo = SLG_CreateSock(addrLocal.family, SOCK_DGRAM);

        if (!SLG_Bind(hVideo, &addrLocal))
        {
            SLG_ReleaseSock(hVideo, NULL);
            return false;
        }

        SLG_GetSockName(hVideo, &addrLocal);
        SLG_Recv(hVideo , this);
        VideoPro* pro = new VideoPro;
        pro->m_hVideoHandle = hVideo;
        pro->m_Addr = addrLocal;
        pro->m_pRGBTMP = NULL;
        pro->m_pRGBTRANS = NULL;
        pro->m_hWnd = hwnd;
        pro->VideoCB = VideoCB;
        m_Pro = pro;
        //DEBUG_INFO3("StartVideo HWND = %d , W = %d , H = %d" , (int)hwnd , pro->m_Rect.right - pro->m_Rect.left , pro->m_Rect.bottom - pro->m_Rect.top);
    }

    else
    {
        m_Pro->m_hWnd = hwnd;
        addrLocal = m_Pro->m_Addr;
    }

    SOCK_ADDRESS Addr[1] = {0};
    Addr->Port = net_ntop(&addrLocal, Addr->IP_Address, IP_LEN);
    //DevVideo Video(this, Addr->IP_Address, Addr->Port, nSeq, buf + OFFSET_SIZE, nLen);
    //SOCK_ADDRESS Addr = {0};
    //strncpy(Addr.IP_Address, Addr->IP_Address, sizeof(Addr.IP_Address) - 1);
    //Addr.Port = Addr->Port;
    return sendNCMD(CMD_M_VIDEO_OUT, CMD_S_SET, 0, nSeq, Addr, sizeof(SOCK_ADDRESS));
}

void CDevSock::ShowVideo(const char* cpbImage, int w, int h, int step, int nImageType)
{
    AoutLock lock(&m_syn);
    VideoPro* pro = m_Pro;
    static int inited = 0;

    if (!inited)
    {
        InitialTable();
    }

    if (NULL == pro)
    {
        return;
    }

    if (NULL == pro->m_pRGBTMP || w != pro->biWidth || h != pro->biHeight)
    {
        delete [] pro->m_pRGBTMP;
        delete [] pro->m_pRGBTRANS;
        pro->m_pRGBTMP = new BYTE[w * h * 3];
        pro->m_pRGBTRANS = new BYTE[w * h * 3];
    }

    if (IMAGE_UYVY == nImageType)
    {
        pro->biWidth = h;
        pro->biHeight = w;
        YUV422_C_RGB((BYTE*)cpbImage, pro->m_pRGBTMP, h, w);
        RGB_EDDY(pro->m_pRGBTMP, h, w, pro->m_pRGBTRANS);
    }

    else if (IMAGE_YUYV == nImageType)
    {
        pro->biWidth = w;
        pro->biHeight = h;
        YUYV_C_RGB((BYTE*)cpbImage, pro->m_pRGBTRANS, h, w);
    }

#if _WIN32

    if (IsWindow((HWND)pro->m_hWnd))
    {
        VideoProCB_defult(pro);
    }

#endif

    if (pro->VideoCB)
    {
        pro->VideoCB(pro);
    }

}

BOOL CDevSock::StopVideo(int nSeq)
{
    int ret = sendNCMD(CMD_M_VIDEO_OUT, CMD_S_GET, 0, nSeq, 0, 0);
    AoutLock lock(&m_syn);

    if (m_Pro)
    {
        SLG_ReleaseSock(m_Pro->m_hVideoHandle, NULL);
        delete [] m_Pro->m_pRGBTMP;
        m_Pro->m_pRGBTMP = NULL;
        delete [] m_Pro->m_pRGBTRANS;
        m_Pro->m_pRGBTRANS = NULL;
        //::ReleaseDC( it->second->m_hWnd, it->second->m_hDC );
        delete m_Pro;
        m_Pro = NULL;
    }

    return ret;
}
