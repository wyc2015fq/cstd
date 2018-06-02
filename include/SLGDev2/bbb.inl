#if 0
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
#endif

