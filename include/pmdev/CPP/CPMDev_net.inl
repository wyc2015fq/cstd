
int CPMDev_callback(cpmdev_t* s) {
  struct fd_set fds;
  int time = 2;
  struct timeval timeout={0}; //select等待3秒，3秒轮询，要非阻塞就置0
  timeout.tv_sec = 3;
  timeout.tv_usec = (time*1000);
  
  for (;1;) {
    int ret, sock = s->s->s;
    if (!sock) {
      sleep(20);
      continue;
    }
    FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化  
    FD_SET(sock,&fds); //添加描述符
    ret = select(sock+1,&fds,NULL,NULL,&timeout);
    switch(ret) //select使用
    {
    case -1:
      //exit(-1);
      printf("select err\n");
      break; //select错误，退出程序  
    case 0:break; //再次轮询
    default:
      if(FD_ISSET(sock,&fds)) //测试sock是否可读，即是否网络上有数据  
      {
        SOCK_DATA_HEADER hdr[1] = {0};
        int32 nRecvBytes = sizeof(SOCK_DATA_HEADER);
        //char* pRecvHdr = (char*)(m_PackHdr + 1);
        int nLen = socket_recv(s->s, hdr, nRecvBytes, 0);
        int nError = 0;
        
        if (nLen < 0) {
          nError = sock_error();
          TRACE("1---nError = %d , nLen = %d  nRecvBytes = %d %s\n" , nError , nLen , nRecvBytes, WSAGetLastErrMsg());
          //return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
        }
        else if (nLen == 0) {
          DEBUG_INFO3("2---nError = %d , nLen = %d  nRecvBytes = %d\n" , nError , nLen , nRecvBytes);
          //return false;
        } else {
          DEBUG_INFO3("3---nError = %d , nLen = %d  nRecvBytes = %d\n" , nError , nLen , nRecvBytes);
        }


        int adsf=0;
        //buffer清空;  
      }// end if break;
    }// end switch  
  }
  return 0;
}

//功    能 设备验证或设置验证
//参    数 cszDevAddr设备地址
//cName    用户名
//cPws     密码
//nFlag    1=验证 2=设置
BOOL ULDevAuth(socket_t* s, const char* sIP, const char* sLoginName, const char* sLoginPwd, int nFlag)
{
  DEV_AUTH cAuth   = {0};
  int nSeq = -1;
  strcpy(cAuth.m_UserName.m_ID, sLoginName);
  strcpy(cAuth.m_Password.m_ID, sLoginPwd);
  
  DEBUG_INFO2(_T("CPM_ULDevAuth : user_name = %s , pws = %s ") , cAuth.m_UserName.m_ID , cAuth.m_Password.m_ID);
  
  int nData[ALIGN(OFFSET_SIZE + sizeof(NCMD) + sizeof(DEV_AUTH), 4) >> 2] = {0};
  char* buf = (char*)nData + OFFSET_SIZE;
  unsigned int len = sizeof(NCMD) + sizeof(DEV_AUTH);
  //DevAuth auth(cszIP, cAuth, (nFlag == 2), nSeq, buf, len);
  
  NCMD* pCMD     = (NCMD*)buf;
  pCMD->m_MCmd   = CMD_M_AUTH;
  pCMD->m_SCmd   = CMD_S_SET;
  pCMD->m_CFlag  = (nFlag == 2) ? 0 : F_AUTH_VERIFY;
  pCMD->m_Param1 = nSeq;
  pCMD->m_Param2 = DotIpToLong(sIP);
  memcpy(pCMD + 1, &cAuth, sizeof(AUTH));
  len = sizeof(NCMD) + sizeof(AUTH);
  return len == socket_send(s, buf, len, 0);
}

#if 0
BOOL DLAllUser(socket_t* s, const char* sIP, const char* cszDevAddr, int nSeq)//下载所有用户
{
  if (! gDspDevManager) {
    return FALSE;
  }
  
  assert(gDspDevManager);
  DEV_DATA_DL dl;
  memset(&dl, 0x0, sizeof(dl)) ;
  dl.m_DLType = DL_ALL;
  return gDspDevManager->GetDevUser(cszDevAddr, dl, nSeq);
}
#endif
