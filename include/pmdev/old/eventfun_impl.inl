{

  int i;
  BOOL bRet = FALSE;

  switch (eResult) {
  case OPER_ERR_BUSY:
    PRINTF("设备%s:设备忙。\n", sIP);
    return TRUE;//设备忙

  case OPER_ERR_LIMIT:
    PRINTF("设备%s:已达上限。\n", sIP);
    return TRUE;//已达上限

  case OPER_ERR_NOFIND: //没有找到对应数据
    PRINTF("设备%s:没有找到对应数据\n", sIP);
    return TRUE;

  case OPER_ERR_SAVEFAIL:
    PRINTF("设备%s:数据保存失败\n", sIP);
    return TRUE;//数据保存失败

  case OPER_ERR_SETFAIL:
    PRINTF("设备%s:设置失败\n", sIP);
    return TRUE;//设置失败

  case OPER_ERR_FROMAT:
    PRINTF("设备%s:格式化失败\n", sIP);
    return TRUE;//格式化失败

  case OPER_ERR_PARAM:
    PRINTF("设备%s:参数错误\n", sIP);
    return TRUE; //参数错误

  case OPER_ERR_DISABLE:
    PRINTF("设备%s:要求执行的功能没有使能\n", sIP);
    return TRUE; //

  case OPER_ERR_EXECUT:
    PRINTF("设备%s:执行失败\n", sIP);
    return TRUE; //失败

  case OPER_ERR_SUPPORT:
    PRINTF("设备%s:不支持的命令\n", sIP);
    return TRUE; //不支持的命令

  case OPER_ERR_INPUTDATA:
    PRINTF("设备%s:网络端传输的数据有异常\n", sIP);
    return TRUE; //网络端传输的数据有异常

  case OPER_BATCH_DL_COMPLETE:
    PRINTF("设备%s:批量下载完成\n", sIP);
    break;

  case OPER_SUCCESS:
    break;

  default:
    break;
  }

  //权限验证操

  switch (eType) {
  case DEV_CONNECT_SUCCESS:
    PRINTF("和设备%s连接成功。\n", sIP);
    break;

  case DEV_CONNECT_FAILUE:
    PRINTF("和设备%s连接失败。\n", sIP);
    break;

  case DEV_CONNECT_CUT:
    PRINTF("和设备%s断开连接。\n", sIP);

    //Connect(pv);
    switch (eResult) {
    case -3:
      PRINTF("err = %d 校验出错\n", eResult);
      break;

    case -2:
      PRINTF("err = %d recv超时\n", eResult);
      break;
    }

    if (0) {
      closesocket(pv->m_sHost);
      pv->m_sHost = 0;
      pv->m_exitThread = 1;
    }

    break;

  case DEV_AUTH_OPER:
    if (0 == eResult) {
      PRINTF("设备%s:权限验证通过。\n", sIP);

#if 0
      PRINTF("设备%s:下载所有识别记录...\n", sIP);
      bRet = CPM_DLAllIdentifyRecord(pv);

      PRINTF("设备%s:下载所有用户...\n", sIP);
      //bRet = CPM_DLAllUser(pv);

      PRINTF("设备%s:获取设备网络参数...\n", sIP);
      bRet = CPM_NetPro(pv, NULL);

      PRINTF("设备%s:获取基本信息...\n", sIP);
      bRet = CPM_DLDevBaseInfo(pv);
      PRINTF("设备%s:获取统计信息...\n", sIP);
      bRet = CPM_DLDevStatisInfo(pv);
      PRINTF("设备%s:获取工作信息...\n", sIP);
      bRet = CPM_DLDevWorkAttInfo(pv);
      PRINTF("设备%s:获取设备时间...\n", sIP);
      bRet = CPM_DLDevTime(pv);

#endif
    }

    break;

  case DEV_BASEINFO_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_BASEINFO* dev_baseinfo = (DEV_BASEINFO*)pvContent;
      PRINTF("设备%s:获取基本信息成功\n", sIP);
      PRINTF("<< 设备基本信息 \n");
      PRINTF("设备类型 %d\n", dev_baseinfo->m_DevType);
      PRINTF("总使用人数上限 %d\n", dev_baseinfo->m_LimitUser);
      PRINTF("1N用户上限 %d\n", dev_baseinfo->m_Limit1NUser);
      PRINTF("设备编号 %s\n", dev_baseinfo->m_SN.m_ID);
      PRINTF("DSP软件版本 %s\n", dev_baseinfo->m_Ver.m_ID);
      PRINTF("磁盘容量信息 %s\n", dev_baseinfo->m_Space.m_ID);
      PRINTF(">>\n");
    }

    break;

  case DEV_STATISINFO_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_STATIS* dev_statis = (DEV_STATIS*)pvContent;
      PRINTF("设备%s:获取统计信息成功\n", sIP);
      PRINTF("<< 设备统计信息 \n");
      PRINTF("当前设备的总用户数 %d\n", dev_statis->m_TotalUsers);
      PRINTF("没有采集人脸特征的用户数 %d\n", dev_statis->m_NoFeatUser);
      PRINTF("识别记录保存总天数 %d\n", dev_statis->m_TotalDays);
      PRINTF("总记录数 %d\n", dev_statis->m_TotalRecords);
      PRINTF("总报警记录数 %d\n", dev_statis->m_TotalAlarm);
      PRINTF("总操作日志数目 %d\n", dev_statis->m_TotalDspLog);
      PRINTF("总变更注册照 %d\n", dev_statis->m_TotalChangeImage);
      PRINTF(">>\n");
    }

    break;

  case DEV_WORKATT_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_WORKATT* dev_workatt = (DEV_WORKATT*) pvContent;
      PRINTF("设备%s:获取设备工作信息成功\n", sIP);
      PRINTF("<< 设备工作信息 \n");
      PRINTF("参见设备功能标记定义 %d\n", dev_workatt->m_BaseSet);
      PRINTF("门磁延时时间，单位秒 %d\n", dev_workatt->m_DoorMangetTime);
      PRINTF("电锁持续时间，单位秒 %d\n", dev_workatt->m_LockTime);
      PRINTF("用户身份验证模式 %d\n", dev_workatt->m_VerifyMode);
      PRINTF("韦根协议类型 %d\n", dev_workatt->m_nWGType);
      PRINTF("韦根输出类型 %d\n", dev_workatt->m_nWGOutType);
      PRINTF("输出ID类型 %d\n", dev_workatt->m_nWGOutIDType);
      PRINTF("WG输出内容 %d\n", dev_workatt->m_nWGOutContent);
      PRINTF("是否WG输出 %d\n", dev_workatt->m_bWGOutPut);
      PRINTF("超级密码 %s\n", dev_workatt->m_szSuperPWD.m_ID);
      PRINTF("设备默认权限 %d\n", dev_workatt->m_DefaultRight);
      PRINTF(">>\n");

      if (0) {
        PRINTF("设备%s:设置工作信息\n", sIP);
        bRet = CPM_ULDevWorkAttInfo(pv, dev_workatt);
      }
    }

    break;

  case DEV_SYS_TIME_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DATETIME* dev_datetime = (DEV_DATETIME*) pvContent;
      PRINTF("设备%s:获取设备时间成功\n", sIP);
      PRINTF("<< 设备日期和时间 \n");
      PRINTF("%d-%d-%d %d:%d:%d:%d\n"
          , dev_datetime->m_Date.m_Year
          , dev_datetime->m_Date.m_Month
          , dev_datetime->m_Date.m_Day
          , dev_datetime->m_Time.m_Hour
          , dev_datetime->m_Time.m_Minute
          , dev_datetime->m_Time.m_Second
          , dev_datetime->m_Time.m_Msec
            );
      PRINTF(">>\n");

      if (0) {
        PRINTF("设备%s:设置设备时间\n", sIP);
        bRet = CPM_ULDevTime(pv, dev_datetime);
      }
    }

    break;

  case DEV_USER_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_USER* dev_user = (DEV_USER*) pvContent;
      PRINTF("设备%s:获取用户成功\n", sIP);
      PRINTF("设备%s:上传用户:%s\n", sIP, dev_user->m_UserID.m_ID);
      bRet = CPM_ULUser(pv, dev_user);

      PRINTF("设备%s:远程用户特征采集:%s\n", sIP, dev_user->m_UserID.m_ID);
      bRet = CPM_ULRealTimeUserCap(pv, dev_user);
    }

    break;

  case DEV_USER_EX_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag && OPER_BATCH_DL_COMPLETE == eResult) {
      DEV_BATCH_USER* pUserBatch = (DEV_BATCH_USER*) pvContent;

      //if (pUserBatch->m_nCount>0)
      if (0) {
        int i;
        //PRINTF("设备%s:批量下载用户成功\n", sIP);
        PRINTF("m_nCount = %d\n", pUserBatch->m_nCount);

        for (i = 0; i < pUserBatch->m_nCount; ++i) {
          PRINTF("m_ID = %s\n", pUserBatch->m_pUserArray[i].m_UserID.m_ID);
        }
      }

      //PRINTF("设备%s:批量上传用户....", sIP);
      //bRet = CPM_ULUserEx(pv, pUserBatch, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
    }

    break;

  case DEV_USER_RANGE_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DL_USER_RANGE* dev_dl_user_range = (DEV_DL_USER_RANGE*) pvContent;
      int nCount = dev_dl_user_range->m_Count;
      PRINTF("设备%s:获取用户批量下载的大区间成功\n", sIP);

      if (nCount > 0) {
        enum { maxnn = 190 };
        DEV_CID arrID[maxnn] = {0};
        nCount = MIN(maxnn, nCount);
        PRINTF("设备%s:把大区间划分成几个小区间下载用户\n", sIP);

        for (i = 0; i < nCount; ++i) {
          strcpy(arrID[i].m_ID, dev_dl_user_range->m_pArray[i].m_UserID.m_ID);
        }

        bRet = CPM_DLUserEx(pv, arrID, nCount, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
      }
    }

    break;

  case DEV_USER_RT_OPER: { //远程加载
    DEV_USER_REAL_LOAD* pDevUserRealLoad = (DEV_USER_REAL_LOAD*) pvContent;
    DEV_USER_REAL_LOAD stDevUserRealLoad;
    memset(&stDevUserRealLoad, 0, sizeof(DEV_USER_REAL_LOAD));
    memcpy(&stDevUserRealLoad, pDevUserRealLoad, sizeof(DEV_USER_REAL_LOAD));
    //PRINTF("设备%s:实时用户加载:%s\n", sIP, m_TempUser.m_UserID.m_ID);
    //bRet = CPM_ULRealTimeUser(pv, m_TempUser, stDevUserRealLoad);
  }
  break;

  case DEV_RECORD_RANGE_OPER:
  case DEV_ALARM_RANGE_OPER: {
    //获取记录的各个大区间，大区间以天为单位。
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DL_RECORD_ARRAY* dev_dl_record_array = (DEV_DL_RECORD_ARRAY*) pvContent;
      PRINTF("设备%s:获取记录批量下载的各个大区间成功 m_Count = %d\n", sIP, dev_dl_record_array->m_Count);

      for (i = 0; i < dev_dl_record_array->m_Count; i++) {
        //DEV_DL_RECORD_RANGE stDevRange[1] = {0};
        //对每个大区间进行拆分，拆分成每50条记录大小多个小区间,没个小区间的第一条记录的ID，由上一个小区间的最后一条记录ID，因此大区间下的小区间必须串行下载。这里只一此下载大区间的前50条。
        //int nSBatchCount = dev_dl_user_range->m_Count / 50;
        //int nLastCount = dev_dl_user_range->m_Count % 50;
        //*stDevRange = dev_dl_user_range->m_pRange;
        //stDevRange->m_Count = stDevRange.m_Count > 50 ? 50 : stDevRange.m_Count;
        PRINTF("%d %s\n", dev_dl_record_array->m_pRange[i].m_Count, dev_dl_record_array->m_pRange[i].m_SID.m_ID);
        bRet = CPM_DLRecEx(pv, dev_dl_record_array->m_pRange + i);
      }
    }
  }
  break;

  case DEV_RECORD_OPER:
  case DEV_ALARM_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_RECORD* pRecord = (DEV_RECORD*) pvContent;
      PRINTF("设备%s:下载记录%s\n", sIP, pRecord->m_SerialID.m_ID);
    }
  }
  break;

  case DEV_RECORD_EX_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_BATCH_RECORD* pBatchRecord = (DEV_BATCH_RECORD*) pvContent;
      PRINTF("设备%s:批量下载记录%d条成功\n", sIP, pBatchRecord->m_nCount);
    }
  }
  break;

  case DEV_USER_REMOTE_CAP: { //远程采集完成
    if (eFlag == DEV_OPER_UPLOAD) { //
      DEV_USER* dev_user = (DEV_USER*) pvContent;
      PRINTF("设备%s:远程采集用户%s成功\n", sIP, dev_user->m_UserID.m_ID);
    }
  }
  break;

  case DEV_NOPEN_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) { //
      DEV_NETPRO* pNOPEN = (DEV_NETPRO*) pvContent;
      PRINTF("设备%s:获取网络信息成功", sIP);
      PRINTF("设备%s:设置网络信息...", sIP);
      bRet = CPM_NetPro(pv, pNOPEN);
    }
  }
  break;

  default:
    break;
  }
}

#undef PRINTF