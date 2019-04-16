# RTMPdump（libRTMP）源代码分析 4： 连接第一步——握手（Hand Shake） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月22日 22:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：31











注：此前写了一些列的分析RTMPdump（libRTMP）源代码的文章，在此列一个列表：
[RTMPdump 源代码分析 1： main()函数](http://blog.csdn.net/leixiaohua1020/article/details/12952977)
[RTMPDump（libRTMP）源代码分析 2：解析RTMP地址——RTMP_ParseURL()](http://blog.csdn.net/leixiaohua1020/article/details/12953833)
[RTMPdump（libRTMP） 源代码分析 3： AMF编码](http://blog.csdn.net/leixiaohua1020/article/details/12954145)
[RTMPdump（libRTMP）源代码分析 4： 连接第一步——握手（Hand Shake）](http://blog.csdn.net/leixiaohua1020/article/details/12954329)
[RTMPdump（libRTMP） 源代码分析 5： 建立一个流媒体连接 （NetConnection部分）](http://blog.csdn.net/leixiaohua1020/article/details/12957291)
[RTMPdump（libRTMP） 源代码分析 6： 建立一个流媒体连接 （NetStream部分 1）](http://blog.csdn.net/leixiaohua1020/article/details/12957877)
[RTMPdump（libRTMP） 源代码分析 7： 建立一个流媒体连接 （NetStream部分 2）](http://blog.csdn.net/leixiaohua1020/article/details/12958617)
[RTMPdump（libRTMP） 源代码分析 8： 发送消息（Message）](http://blog.csdn.net/leixiaohua1020/article/details/12958747)
[RTMPdump（libRTMP） 源代码分析 9： 接收消息（Message）（接收视音频数据）](http://blog.csdn.net/leixiaohua1020/article/details/12971635)
[RTMPdump（libRTMP） 源代码分析 10： 处理各种消息（Message）](http://blog.csdn.net/leixiaohua1020/article/details/12972399)

===============================





在这里分析一下RTMPdump（libRTMP）连接到支持RTMP协议的服务器的第一步：握手（Hand Shake）。

RTMP连接的过程曾经分析过：[RTMP流媒体播放过程](http://blog.csdn.net/leixiaohua1020/article/details/11704355)

在这里不再细说，分析一下位于handshake.h文件里面实现握手（HandShake）功能的函数：

注意：handshake.h里面代码量很大，但是很多代码都是为了处理RTMP的加密版协议的，例如rtmps；因此在这里就不做过多分析了，我们只考虑普通的RTMP协议。





```cpp
static int
HandShake(RTMP * r, int FP9HandShake)
{
  int i, offalg = 0;
  int dhposClient = 0;
  int digestPosClient = 0;
  int encrypted = r->Link.protocol & RTMP_FEATURE_ENC;

  RC4_handle keyIn = 0;
  RC4_handle keyOut = 0;

  int32_t *ip;
  uint32_t uptime;

  uint8_t clientbuf[RTMP_SIG_SIZE + 4], *clientsig=clientbuf+4;
  uint8_t serversig[RTMP_SIG_SIZE], client2[RTMP_SIG_SIZE], *reply;
  uint8_t type;
  getoff *getdh = NULL, *getdig = NULL;

  if (encrypted || r->Link.SWFSize)
    FP9HandShake = TRUE;
  else
	//普通的
    FP9HandShake = FALSE;

  r->Link.rc4keyIn = r->Link.rc4keyOut = 0;

  if (encrypted)
    {
      clientsig[-1] = 0x06;	/* 0x08 is RTMPE as well */
      offalg = 1;
    }
  else
	//0x03代表RTMP协议的版本（客户端要求的）
	//数组竟然能有“-1”下标
	//C0中的字段(1B)
    clientsig[-1] = 0x03;

  uptime = htonl(RTMP_GetTime());
  //void *memcpy(void *dest, const void *src, int n);
  //由src指向地址为起始地址的连续n个字节的数据复制到以dest指向地址为起始地址的空间内
  //把uptime的前4字节（其实一共就4字节）数据拷贝到clientsig指向的地址中
  //C1中的字段(4B)
  memcpy(clientsig, &uptime, 4);

  if (FP9HandShake)
    {
      /* set version to at least 9.0.115.0 */
      if (encrypted)
	{
	  clientsig[4] = 128;
	  clientsig[6] = 3;
	}
      else
        {
	  clientsig[4] = 10;
	  clientsig[6] = 45;
	}
      clientsig[5] = 0;
      clientsig[7] = 2;

      RTMP_Log(RTMP_LOGDEBUG, "%s: Client type: %02X", __FUNCTION__, clientsig[-1]);
      getdig = digoff[offalg];
      getdh  = dhoff[offalg];
    }
  else
    {
	//void *memset(void *s, int ch, size_t n);将s中前n个字节替换为ch并返回s；
	//将clientsig[4]开始的4个字节替换为0
	//这是C1的字段
      memset(&clientsig[4], 0, 4);
    }

  /* generate random data */
#ifdef _DEBUG
  //将clientsig+8开始的1528个字节替换为0（这是一种简单的方法）
  //这是C1中的random字段
  memset(clientsig+8, 0, RTMP_SIG_SIZE-8);
#else
  //实际中使用rand()循环生成1528字节的伪随机数
  ip = (int32_t *)(clientsig+8);
  for (i = 2; i < RTMP_SIG_SIZE/4; i++)
    *ip++ = rand();
#endif

  /* set handshake digest */
  if (FP9HandShake)
    {
      if (encrypted)
	{
	  /* generate Diffie-Hellmann parameters */
	  r->Link.dh = DHInit(1024);
	  if (!r->Link.dh)
	    {
	      RTMP_Log(RTMP_LOGERROR, "%s: Couldn't initialize Diffie-Hellmann!",
		  __FUNCTION__);
	      return FALSE;
	    }

	  dhposClient = getdh(clientsig, RTMP_SIG_SIZE);
	  RTMP_Log(RTMP_LOGDEBUG, "%s: DH pubkey position: %d", __FUNCTION__, dhposClient);

	  if (!DHGenerateKey((DH *)r->Link.dh))
	    {
	      RTMP_Log(RTMP_LOGERROR, "%s: Couldn't generate Diffie-Hellmann public key!",
		  __FUNCTION__);
	      return FALSE;
	    }

	  if (!DHGetPublicKey((DH *)r->Link.dh, &clientsig[dhposClient], 128))
	    {
	      RTMP_Log(RTMP_LOGERROR, "%s: Couldn't write public key!", __FUNCTION__);
	      return FALSE;
	    }
	}

      digestPosClient = getdig(clientsig, RTMP_SIG_SIZE);	/* reuse this value in verification */
      RTMP_Log(RTMP_LOGDEBUG, "%s: Client digest offset: %d", __FUNCTION__,
	  digestPosClient);

      CalculateDigest(digestPosClient, clientsig, GenuineFPKey, 30,
		      &clientsig[digestPosClient]);

      RTMP_Log(RTMP_LOGDEBUG, "%s: Initial client digest: ", __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, clientsig + digestPosClient,
	     SHA256_DIGEST_LENGTH);
    }

#ifdef _DEBUG
  RTMP_Log(RTMP_LOGDEBUG, "Clientsig: ");
  RTMP_LogHex(RTMP_LOGDEBUG, clientsig, RTMP_SIG_SIZE);
#endif
  //发送数据报C0+C1
  //从clientsig-1开始发，长度1536+1，两个包合并
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。发送握手数据C0+C1");
  //-----------------------------
  if (!WriteN(r, (char *)clientsig-1, RTMP_SIG_SIZE + 1))
    return FALSE;
  //读取数据报，长度1，存入type
  //是服务器的S0，表示服务器使用的RTMP版本
  if (ReadN(r, (char *)&type, 1) != 1)	/* 0x03 or 0x06 */
    return FALSE;
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。接收握手数据S0");
  //-----------------------------
  RTMP_Log(RTMP_LOGDEBUG, "%s: Type Answer   : %02X", __FUNCTION__, type);
  //客户端要求的版本和服务器提供的版本不同
  if (type != clientsig[-1])
    RTMP_Log(RTMP_LOGWARNING, "%s: Type mismatch: client sent %d, server answered %d",
	__FUNCTION__, clientsig[-1], type);
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。成功接收握手数据S0，服务器和客户端版本相同");
  //-----------------------------
  //客户端和服务端随机序列长度是否相同
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。接收握手数据S1");
  //-----------------------------
  if (ReadN(r, (char *)serversig, RTMP_SIG_SIZE) != RTMP_SIG_SIZE)
    return FALSE;

  /* decode server response */
  //把serversig的前四个字节赋值给uptime
  memcpy(&uptime, serversig, 4);
  //大端转小端
  uptime = ntohl(uptime);

  RTMP_Log(RTMP_LOGDEBUG, "%s: Server Uptime : %d", __FUNCTION__, uptime);
  RTMP_Log(RTMP_LOGDEBUG, "%s: FMS Version   : %d.%d.%d.%d", __FUNCTION__, serversig[4],
      serversig[5], serversig[6], serversig[7]);

  if (FP9HandShake && type == 3 && !serversig[4])
    FP9HandShake = FALSE;

#ifdef _DEBUG
  RTMP_Log(RTMP_LOGDEBUG, "Server signature:");
  RTMP_LogHex(RTMP_LOGDEBUG, serversig, RTMP_SIG_SIZE);
#endif

  if (FP9HandShake)
    {
      uint8_t digestResp[SHA256_DIGEST_LENGTH];
      uint8_t *signatureResp = NULL;

      /* we have to use this signature now to find the correct algorithms for getting the digest and DH positions */
      int digestPosServer = getdig(serversig, RTMP_SIG_SIZE);

      if (!VerifyDigest(digestPosServer, serversig, GenuineFMSKey, 36))
	{
	  RTMP_Log(RTMP_LOGWARNING, "Trying different position for server digest!");
	  offalg ^= 1;
	  getdig = digoff[offalg];
	  getdh  = dhoff[offalg];
	  digestPosServer = getdig(serversig, RTMP_SIG_SIZE);

	  if (!VerifyDigest(digestPosServer, serversig, GenuineFMSKey, 36))
	    {
	      RTMP_Log(RTMP_LOGERROR, "Couldn't verify the server digest");	/* continuing anyway will probably fail */
	      return FALSE;
	    }
	}

      /* generate SWFVerification token (SHA256 HMAC hash of decompressed SWF, key are the last 32 bytes of the server handshake) */
      if (r->Link.SWFSize)
	{
	  const char swfVerify[] = { 0x01, 0x01 };
	  char *vend = r->Link.SWFVerificationResponse+sizeof(r->Link.SWFVerificationResponse);

	  memcpy(r->Link.SWFVerificationResponse, swfVerify, 2);
	  AMF_EncodeInt32(&r->Link.SWFVerificationResponse[2], vend, r->Link.SWFSize);
	  AMF_EncodeInt32(&r->Link.SWFVerificationResponse[6], vend, r->Link.SWFSize);
	  HMACsha256(r->Link.SWFHash, SHA256_DIGEST_LENGTH,
		     &serversig[RTMP_SIG_SIZE - SHA256_DIGEST_LENGTH],
		     SHA256_DIGEST_LENGTH,
		     (uint8_t *)&r->Link.SWFVerificationResponse[10]);
	}

      /* do Diffie-Hellmann Key exchange for encrypted RTMP */
      if (encrypted)
	{
	  /* compute secret key */
	  uint8_t secretKey[128] = { 0 };
	  int len, dhposServer;

	  dhposServer = getdh(serversig, RTMP_SIG_SIZE);
	  RTMP_Log(RTMP_LOGDEBUG, "%s: Server DH public key offset: %d", __FUNCTION__,
	    dhposServer);
	  len = DHComputeSharedSecretKey((DH *)r->Link.dh, &serversig[dhposServer],
	  				128, secretKey);
	  if (len < 0)
	    {
	      RTMP_Log(RTMP_LOGDEBUG, "%s: Wrong secret key position!", __FUNCTION__);
	      return FALSE;
	    }

	  RTMP_Log(RTMP_LOGDEBUG, "%s: Secret key: ", __FUNCTION__);
	  RTMP_LogHex(RTMP_LOGDEBUG, secretKey, 128);

	  InitRC4Encryption(secretKey,
			    (uint8_t *) & serversig[dhposServer],
			    (uint8_t *) & clientsig[dhposClient],
			    &keyIn, &keyOut);
	}


      reply = client2;
#ifdef _DEBUG
      memset(reply, 0xff, RTMP_SIG_SIZE);
#else
      ip = (int32_t *)reply;
      for (i = 0; i < RTMP_SIG_SIZE/4; i++)
        *ip++ = rand();
#endif
      /* calculate response now */
      signatureResp = reply+RTMP_SIG_SIZE-SHA256_DIGEST_LENGTH;

      HMACsha256(&serversig[digestPosServer], SHA256_DIGEST_LENGTH,
		 GenuineFPKey, sizeof(GenuineFPKey), digestResp);
      HMACsha256(reply, RTMP_SIG_SIZE - SHA256_DIGEST_LENGTH, digestResp,
		 SHA256_DIGEST_LENGTH, signatureResp);

      /* some info output */
      RTMP_Log(RTMP_LOGDEBUG,
	  "%s: Calculated digest key from secure key and server digest: ",
	  __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, digestResp, SHA256_DIGEST_LENGTH);

#ifdef FP10
      if (type == 8 )
        {
	  uint8_t *dptr = digestResp;
	  uint8_t *sig = signatureResp;
	  /* encrypt signatureResp */
          for (i=0; i<SHA256_DIGEST_LENGTH; i+=8)
	    rtmpe8_sig(sig+i, sig+i, dptr[i] % 15);
        }
#if 0
      else if (type == 9))
        {
	  uint8_t *dptr = digestResp;
	  uint8_t *sig = signatureResp;
	  /* encrypt signatureResp */
          for (i=0; i<SHA256_DIGEST_LENGTH; i+=8)
            rtmpe9_sig(sig+i, sig+i, dptr[i] % 15);
        }
#endif
#endif
      RTMP_Log(RTMP_LOGDEBUG, "%s: Client signature calculated:", __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, signatureResp, SHA256_DIGEST_LENGTH);
    }
  else
    {
	//直接赋值
      reply = serversig;
#if 0
      uptime = htonl(RTMP_GetTime());
      memcpy(reply+4, &uptime, 4);
#endif
    }

#ifdef _DEBUG
  RTMP_Log(RTMP_LOGDEBUG, "%s: Sending handshake response: ",
    __FUNCTION__);
  RTMP_LogHex(RTMP_LOGDEBUG, reply, RTMP_SIG_SIZE);
#endif
  //把reply中的1536字节数据发送出去
  //对应C2
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。发送握手数据C2");
  //-----------------------------
  if (!WriteN(r, (char *)reply, RTMP_SIG_SIZE))
    return FALSE;

  /* 2nd part of handshake */
  //读取1536字节数据到serversig
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。读取握手数据S2");
  //-----------------------------
  if (ReadN(r, (char *)serversig, RTMP_SIG_SIZE) != RTMP_SIG_SIZE)
    return FALSE;

#ifdef _DEBUG
  RTMP_Log(RTMP_LOGDEBUG, "%s: 2nd handshake: ", __FUNCTION__);
  RTMP_LogHex(RTMP_LOGDEBUG, serversig, RTMP_SIG_SIZE);
#endif

  if (FP9HandShake)
    {
      uint8_t signature[SHA256_DIGEST_LENGTH];
      uint8_t digest[SHA256_DIGEST_LENGTH];

      if (serversig[4] == 0 && serversig[5] == 0 && serversig[6] == 0
	  && serversig[7] == 0)
	{
	  RTMP_Log(RTMP_LOGDEBUG,
	      "%s: Wait, did the server just refuse signed authentication?",
	      __FUNCTION__);
	}
      RTMP_Log(RTMP_LOGDEBUG, "%s: Server sent signature:", __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, &serversig[RTMP_SIG_SIZE - SHA256_DIGEST_LENGTH],
	     SHA256_DIGEST_LENGTH);

      /* verify server response */
      HMACsha256(&clientsig[digestPosClient], SHA256_DIGEST_LENGTH,
		 GenuineFMSKey, sizeof(GenuineFMSKey), digest);
      HMACsha256(serversig, RTMP_SIG_SIZE - SHA256_DIGEST_LENGTH, digest,
		 SHA256_DIGEST_LENGTH, signature);

      /* show some information */
      RTMP_Log(RTMP_LOGDEBUG, "%s: Digest key: ", __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, digest, SHA256_DIGEST_LENGTH);

#ifdef FP10
      if (type == 8 )
        {
	  uint8_t *dptr = digest;
	  uint8_t *sig = signature;
	  /* encrypt signature */
          for (i=0; i<SHA256_DIGEST_LENGTH; i+=8)
	    rtmpe8_sig(sig+i, sig+i, dptr[i] % 15);
        }
#if 0
      else if (type == 9)
        {
	  uint8_t *dptr = digest;
	  uint8_t *sig = signature;
	  /* encrypt signatureResp */
          for (i=0; i<SHA256_DIGEST_LENGTH; i+=8)
            rtmpe9_sig(sig+i, sig+i, dptr[i] % 15);
        }
#endif
#endif
      RTMP_Log(RTMP_LOGDEBUG, "%s: Signature calculated:", __FUNCTION__);
      RTMP_LogHex(RTMP_LOGDEBUG, signature, SHA256_DIGEST_LENGTH);
      if (memcmp
	  (signature, &serversig[RTMP_SIG_SIZE - SHA256_DIGEST_LENGTH],
	   SHA256_DIGEST_LENGTH) != 0)
	{
	  RTMP_Log(RTMP_LOGWARNING, "%s: Server not genuine Adobe!", __FUNCTION__);
	  return FALSE;
	}
      else
	{
	  RTMP_Log(RTMP_LOGDEBUG, "%s: Genuine Adobe Flash Media Server", __FUNCTION__);
	}

      if (encrypted)
	{
	  char buff[RTMP_SIG_SIZE];
	  /* set keys for encryption from now on */
	  r->Link.rc4keyIn = keyIn;
	  r->Link.rc4keyOut = keyOut;


	  /* update the keystreams */
	  if (r->Link.rc4keyIn)
	    {
	      RC4_encrypt((RC4_KEY *)r->Link.rc4keyIn, RTMP_SIG_SIZE, (uint8_t *) buff);
	    }

	  if (r->Link.rc4keyOut)
	    {
	      RC4_encrypt((RC4_KEY *)r->Link.rc4keyOut, RTMP_SIG_SIZE, (uint8_t *) buff);
	    }
	}
    }
  else
    {
	//int memcmp(const void *buf1, const void *buf2, unsigned int count); 当buf1=buf2时，返回值=0 
	//比较serversig和clientsig是否相等
		//握手----------------
		r->dlg->AppendCInfo("建立连接：第1次连接。比较握手数据签名");
		//-----------------------------
      if (memcmp(serversig, clientsig, RTMP_SIG_SIZE) != 0)
	{
		//握手----------------
		r->dlg->AppendCInfo("建立连接：第1次连接。握手数据签名不匹配！");
		//-----------------------------
	  RTMP_Log(RTMP_LOGWARNING, "%s: client signature does not match!",
	      __FUNCTION__);
	}
    }
  //握手----------------
  r->dlg->AppendCInfo("建立连接：第1次连接。握手成功");
  //-----------------------------
  RTMP_Log(RTMP_LOGDEBUG, "%s: Handshaking finished....", __FUNCTION__);
  return TRUE;
}
```



rtmpdump源代码（Linux）：[http://download.csdn.net/detail/leixiaohua1020/6376561](http://download.csdn.net/detail/leixiaohua1020/6376561)

rtmpdump源代码（VC 2005 工程）：[http://download.csdn.net/detail/leixiaohua1020/6563163](http://download.csdn.net/detail/leixiaohua1020/6563163)








