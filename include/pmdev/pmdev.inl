/////////////////////////////////////////////////////////////
// 日志：
// 上午 08:54 2016/5/31 星期二 修正二代机视频解码不正常
// 上午 11:00 2016/5/5 星期四 修正实时识别记录的获取bug
// 9:30 2015/9/12 完善用户下载接口和记录下载接口，增加网络配置信息获取接口。dev.cmd_GetNetPro();
/////////////////////////////////////////////////////////////
// 使用说明:
// 本SDK 适用于人脸识别3代机
// 数据下载用的TCP。具体使用见本文件末尾的 test1 函数。
// 视频浏览用的UDP。因此设备和手机需要配置成同一个网段。
// 启动视频例子 dev.cmd_StartVideo("192.168.1.3");
/////////////////////////////////////////////////////////////
#include "pmdev.h"
// 校验和计算
static int sock_checksum(const byte* buffer, int off, int size)
{
  int cksum = 0;
  while ((size - 1) > off) {
    int low = buffer[off];
    int high = buffer[off + 1];
    int z = (int)(((high & 0xff) << 8) | (low & 0xff));
    cksum += z;
    // li->msg_DEBUGMSG(this, "cksum = " + cksum + " z = " + z);
    off += 2;
  }
  if (size > off) {
    cksum += buffer[off];
  }
  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (~cksum) & 0xffff;
}
struct DataInputStream {
  virtual int read(byte* by, int len) = 0;
  virtual int close() = 0;
  virtual int isClosed() = 0;
};
struct DataOutputStream {
  virtual int write(byte* by, int len) = 0;
  virtual int close() = 0;
  virtual int isClosed() = 0;
};
struct pmdev;
struct listener {
  virtual int msg_DEBUGMSG(pmdev* dev, const char* s) = 0;
  virtual int msg_CONNECT_SUCCESS(pmdev* dev) = 0;
  virtual int msg_CONNECT_CUT(pmdev* dev) = 0;
  virtual int msg_AUTH(pmdev* dev, int m_CFlag) = 0;
  virtual int msg_DEVINFO(pmdev* dev, BASEINFO* baseinfo) = 0;
  virtual int msg_STATIS(pmdev* dev, STATIS* statis) = 0;
  virtual int msg_NETPRO(pmdev* dev, NETPRO* netpro);
  virtual int msg_RECORDRANGE(pmdev* dev, int type, RECORDRANGE* range, int n) = 0;
  virtual int msg_RECORD(pmdev* dev, int type, RECORD* range, int n) = 0;
  virtual int msg_RECORDEX(pmdev* dev, int type, RECORD* range, int n) = 0;
  virtual int msg_VUSER(pmdev* dev, VUSER* vuser, int n) = 0;
  virtual int msg_USER(pmdev* dev, USER* vuser, int n) = 0;
  virtual int msg_VIDEO_OUT(pmdev* dev, int* data, int rgb_offset, int w, int h, int step) = 0;
};
struct pmdev {
  int m_nIndex;
  listener* li;
  DataInputStream* netInputStream;
  DataOutputStream* netOutputStream;
  byte* msg_buf;
  int msg_buf_len;
  int msg_identify; // 标识
  int msg_len; // 数据长度
  pmdev() {
    msg_identify = -1;
  }
  int send_cmd_packet(int m_MCmd, int m_SCmd, int m_CFlag, byte* data, int len) {
    byte pBuffer[PACK_SIZE + OFFSET_SIZE];// 最大60K大小一个包
    int nTotalSend = 0;
    // int bOK = 1;
    int i = 0;
    int nSeq = ++m_nIndex;
    int m_Param1 = -1;
    int m_Param2 = 0;
    int cmdlen = len + NCMD_size;
    set_stream set(pBuffer, OFFSET_SIZE);
    set.set_INT((int) m_MCmd);
    set.set_INT((int) m_SCmd);
    set.set_INT((int) m_CFlag);
    set.set_INT((int) m_Param1);
    set.set_INT((int) m_Param2);
    for (i = 0; i < cmdlen;) {
      int nSend;
      // 组装网络命令包 加包头
      int msg_identify = nSeq; // 标识
      int msg_offset = i * PACK_SIZE; // 偏移
      int msg_total = cmdlen; // 数据总长度
      int msg_checksum = 0; // 检验和
      int msg_len = MIN(PACK_SIZE, (cmdlen - i)); // 数据长度
      if (len > 0 && data != NULL) {
        if (0 == i) {
          memcpy(pBuffer + OFFSET_SIZE + NCMD_size, data, msg_len - NCMD_size);
        }
        else {
          memcpy(pBuffer + NCMD_size, data + i, msg_len);
        }
      }
      nSend = ALIGN(msg_len, 4) + OFFSET_SIZE;
      set = set_stream(pBuffer, 0);
      set.set_INT((int) msg_identify);
      set.set_INT((int) msg_offset);
      set.set_INT((int) msg_len);
      set.set_INT((int) msg_total);
      msg_checksum = sock_checksum(pBuffer, 0, OFFSET_SIZE);
      set.set_INT((int) msg_checksum);
      // println("send" + nSend);
      int ret = netOutputStream->write(pBuffer, nSend);
      if (ret < 0) {
        li->msg_CONNECT_CUT(this);
        return ret;
      }
      nTotalSend += msg_len;
      i += msg_len;
    }
    // li->msg_DEBUGMSG(this, "发送 成功:" + nTotalSend + "bytes");
    return nTotalSend;
  }
  // int isexit;
  int close() {
    int ret = 0;
    if (NULL != netInputStream) {
      ret = netInputStream->close();
    }
    if (ret >= 0 && NULL != netOutputStream) {
      ret = netOutputStream->close();
    }
    if (ret < 0) {
      printf("close 出错");
    }
    return ret;
  }
  int safe_read(byte* bytes, int off, int len) {
    int i = 0;
    for (; i < len;) {
      int ret = netInputStream->read(bytes + off + i, len - i);
      if (ret <= 0) {
        return -1;
      }
      i += ret;
    }
    return i;
  }
  int recv_pack_data() {
    char buf[256];
    byte pbuf[OFFSET_SIZE + 40];
    int ret = 0;
    get_stream get(pbuf, 0);
    // 获取包头
    ret = safe_read(pbuf, 0, OFFSET_SIZE);
    // li->msg_DEBUGMSG(this, "read " + ret + "bytes");
    if (ret == -1) {
      li->msg_DEBUGMSG(this, "read 包头 超时");
      return ret;
    }
    if (ret < 0) {
      li->msg_DEBUGMSG(this, "read 包头 出错");
      // close();
      return ret;
    }
    // li->msg_VIDEO_OUT(this, bytes2string(pbuf, 0, 20));
    if (ret != 20) {
      li->msg_DEBUGMSG(this, "read 包头 出错 长度不对 len=" + ret);
      return 0;
    }
    int checksum = sock_checksum(pbuf, 0, OFFSET_SIZE);
    // 校验
    if (0 != checksum) {
      li->msg_DEBUGMSG(this, "校验出错，丢弃");
      return -3; // 校验出错，丢弃
    }
    // 查找本次标识的数据接收缓冲
    int identify = 0, offset = 0, len = 0, total = 0;
    get.get_INT(&identify);
    get.get_INT(&offset);
    get.get_INT(&len);
    get.get_INT(&total);
    if (identify != msg_identify || NULL == msg_buf) { // 没找到，则为新视频数据包。
      if (offset != 0) { // 检验是否丢包头，否则为无命令头的非法视频数据。
        li->msg_DEBUGMSG(this, "无命令头，丢弃");
        return -1;
      }
      // 为接收本次视频数据开辟一个快缓存，并插入到MAP映射表中
      myrealloc(&msg_buf, total);
      msg_buf_len = total;
      msg_identify = identify;
      msg_len = 0;
    }
    if (offset + len > total) {
      _snprintf(buf, 256, "safe_read 出错%d %d", ret, len);
      li->msg_DEBUGMSG(this, buf);
      return 0;
    }
    // 收数据
    ret = safe_read(msg_buf, offset, len);
    // li->msg_DEBUGMSG(this, "len " + ret + " " + len);
    // li->msg_DEBUGMSG(this, "read " + ret + "bytes");
    if (ret < 0) {
      _snprintf(buf, 256, "safe_read 出错%d %d", ret, len);
      li->msg_DEBUGMSG(this, buf);
      return -1;
    }
    msg_len += len;
    if (msg_len == msg_buf_len) {
      ret = msg_len;
      msg_identify = -1;
      // li->msg_DEBUGMSG(this, "recv_pack_data " + ret + "bytes");
    }
    return ret;
  }
  int GET_ERR(int state) {
    int errcode = ((state) & 0xff);
    switch (errcode) {
    case SUC_EXECUT:
      li->msg_DEBUGMSG(this, "命令成功");
      break;
    case ERR_EXECUT:
      li->msg_DEBUGMSG(this, "命令失败");
      break;
    case ERR_BUSY:
      li->msg_DEBUGMSG(this, "错误: 设备忙");
      break;
    case ERR_LIMIT:
      li->msg_DEBUGMSG(this, "错误: 已达上限");
      break;
    case ERR_NOFIND:
      li->msg_DEBUGMSG(this, "错误: 没有找到对应数据");
      break;
    case ERR_SAVE_FAIL:
      li->msg_DEBUGMSG(this, "错误: 数据保存失败");
      break;
    case ERR_SET_FAIL:
      li->msg_DEBUGMSG(this, "错误: 设置失败");
      break;
    case ERR_VERIFY_FAIL:
      li->msg_DEBUGMSG(this, "错误: 验证失败");
      break;
    case ERR_FROMAT:
      li->msg_DEBUGMSG(this, "错误: 格式化失败");
      break;
    case ERR_PARAM:
      li->msg_DEBUGMSG(this, "错误: 参数错误");
      break;
    case ERR_DISABLE:
      li->msg_DEBUGMSG(this, "错误: 要求执行的功能没有使能");
      break;
    case ERR_SUPPORT:
      li->msg_DEBUGMSG(this, "错误: 不支持的命令");
      break;
    case ERR_INPUTDATA:
      li->msg_DEBUGMSG(this, "错误: 网络端传输的数据有异常");
      break;
    }
    return errcode;
  }
  int m_Author;
  int msg_proc(int m_MCmd, int m_SCmd, int m_CFlag, int m_Param1, int m_Param2) {
    int i, MCmd = m_MCmd;
    int off = NCMD_size;
    char buf[256];
    // int eResult = GET_ERR(m_CFlag);
    get_stream get = get_stream(msg_buf, off);
    _snprintf(buf, 256, "收到消息 m_MCmd=%d 长度: %d", m_MCmd, msg_buf_len);
    li->msg_DEBUGMSG(this, buf);
    switch (MCmd) {
    case CMD_M_AUTH:
      m_Author = (m_CFlag & F_AUTH_VERIFY);
      // if ( (pCMD.m_CFlag & F_AUTH_VERIFY) ? DEV_AUTH_OPER :
      // DEV_AUTH_SET_OPER, DEV_OPER_UPLOAD, NULL, pCMD.m_Param1,
      // GET_ERR(pCMD.m_CFlag), pvClientData);
      li->msg_AUTH(this, m_CFlag);
      break;
    case CMD_M_DEVINFO: {
      BASEINFO baseinfo[1] = {0};
      get.get_INT(&baseinfo->m_DevType);
      get.get_INT(&baseinfo->m_LimitUser);
      get.get_INT(&baseinfo->m_Limit1NUser);
      get.get_CID(&baseinfo->m_SN);
      get.get_CID(&baseinfo->m_Ver);
      get.get_CID(&baseinfo->m_Space);
      li->msg_DEVINFO(this, baseinfo);
    }
    break;
    case CMD_M_STATIS: {
      STATIS statis[1] = {0};
      get.get_INT(&statis->m_TotalUsers);
      get.get_INT(&statis->m_NoFeatUser);
      get.get_INT(&statis->m_TotalDays);
      get.get_INT(&statis->m_TotalRecords);
      get.get_INT(&statis->m_TotalAlarm);
      get.get_INT(&statis->m_TotalDspLog);
      get.get_INT(&statis->m_TotalChangeImage);
      li->msg_STATIS(this, statis);
    }
    break;
    case CMD_M_NETPRO: {
      NETPRO netpro[1] = {0};
      get.get_NETPRO(netpro);
      li->msg_NETPRO(this, netpro);
    }
    break;
    case CMD_M_USER:
      if ((m_CFlag & F_DL_ID) != 0) {
        if (0 == GET_ERR(m_CFlag)) {
          VUSER* vuser = NULL;
          int m_Count = 0;
          get.get_INT(&m_Count);
          myrealloc(&vuser, m_Count*sizeof(VUSER));
          memset(vuser, 0, m_Count*sizeof(VUSER));
          for (i = 0; i < m_Count; ++i) {
            get.get_VUSER(vuser+i);
          }
          li->msg_VUSER(this, vuser, m_Count);
          free(vuser);
        }
      }
      break;
    case CMD_M_USER_EX:
      if (0 == GET_ERR(m_CFlag)) {
        switch (m_SCmd) {
        case CMD_S_GET: {
            int m_Count = 0;
            get.get_INT(&m_Count);
            _snprintf(buf, 256, "CMD_M_USER_EX m_Count: %d msg_buf.len: %d", m_Count, msg_buf_len);
              li->msg_DEBUGMSG(this, buf);
            if (m_Count > 0) {
              USER* user = NULL;
              myrealloc(&user, m_Count*sizeof(USER));
              memset(user, 0, m_Count*sizeof(USER));
              for (i = 0; i < m_Count; ++i) {
                get.get_USER(user+i);
              }
              li->msg_USER(this, user, m_Count);
              for (i = 0; i < m_Count; ++i) {
                free(user[i].m_Feat);
                free(user[i].m_Pic);
              }
              free(user);
            }
          }
          break;
        default:
          _snprintf(buf, 256, "暂不支持的子命令 m_SCmd=%d", m_SCmd );
          li->msg_DEBUGMSG(this, buf);
          break;
        }
      }
      // dev_insert_users(s->user, pUserBatch->m_pUserArray,
      // pUserBatch->m_nCount, 1);
      // listbox_set_item_count(s->lb_reg, s->user->m_nCount);
      break;
    case CMD_M_VERIFY:
    case CMD_M_ALARM:
      if (0 == GET_ERR(m_CFlag)) {
        RECORD record[1] = {0};
        get.get_RECORD(record);
        li->msg_RECORD(this, m_MCmd, record, 1);
      }
      break;
    case CMD_M_OPTLOG:
    case CMD_M_CIMAGE:
    case CMD_M_RANGE:
      if (0 == GET_ERR(m_CFlag)) {
        int m_Count = 0;
        get.get_INT(&m_Count);
        _snprintf(buf, 256, "%d", m_Count);
        li->msg_DEBUGMSG(this, buf);
        RECORDRANGE* range = NULL;
        myrealloc(&range, m_Count*sizeof(RECORDRANGE));
        memset(range, 0, m_Count*sizeof(RECORDRANGE));
        for (i = 0; i < m_Count; ++i) {
          get.get_RECORDRANGE(range+i);
        }
        li->msg_RECORDRANGE(this, m_MCmd, range, m_Count);
        for (i = 0; i < m_Count; ++i) {
          get.get_RECORDRANGE(range+i);
        }
        free(range);
      }
      break;
    case CMD_M_RECORD_EX:
      if (0 == GET_ERR(m_CFlag)) {
        switch (m_SCmd) {
        case CMD_M_VERIFY:
        case CMD_M_ALARM:
          int m_Count = 0;
          get.get_INT(&m_Count);
          RECORD* record = NULL;
          myrealloc(&record, m_Count*sizeof(RECORD));
          memset(record, 0, m_Count*sizeof(RECORD));
          for (i = 0; i < m_Count; ++i) {
            get.get_RECORD(record+i);
          }
          li->msg_RECORDEX(this, m_MCmd, record, m_Count);
          for (i = 0; i < m_Count; ++i) {
            free(record[i].m_Pic);
          }
          free(record);
          break;
        }
      }
      break;
    default:
      break;
    }
    return 0;
  }
  void run() {
    msg_len = 0;
    msg_identify = -1;
    while (!netInputStream->isClosed()) {
      int ret = recv_pack_data();
      if (ret > 0) {
        get_stream get(msg_buf, 0);
        int m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2;
        get.get_INT(&m_MCmd);
        get.get_INT(&m_SCmd);
        get.get_INT(&m_CFlag);
        get.get_INT(&m_Param1);
        get.get_INT(&m_Param2);
        if (CMD_M_HEART == m_MCmd) {
          if (SUC_EXECUT == GET_ERR(m_CFlag)) {
            // 发送心跳包
            send_cmd_packet(CMD_M_HEART, CMD_S_SET, 0, NULL, 0);
          }
        }
        else {
          msg_proc(m_MCmd, m_SCmd, m_CFlag, m_Param1, m_Param2);
        }
      } else if (ret < 0) {
        close();
        li->msg_DEBUGMSG(this, "recv_pack_data 出错, 主动断开");
      }
    }
  }
  // 功 能 设备验证或设置验证
  // 参 数 cszDevAddr设备地址
  // cName 用户名
  // cPws 密码
  // nFlag 1=验证 2=设置
  // 说 明
  int cmd_SetDevAuth(const char* sLoginName, const char* sPassword, int nFlag) {
    byte pAuth[2 * ID_LEN];
    set_stream set(pAuth, 0);
    ASSERT(1==nFlag || 2==nFlag);
    set.set_STRING(sLoginName, ID_LEN);
    set.set_STRING(sPassword, ID_LEN);
    // 组命令数据
    return send_cmd_packet(CMD_M_AUTH, CMD_S_SET, ((nFlag == 2) ? 0 : F_AUTH_VERIFY), pAuth, 2 * ID_LEN);
  }
  // 功 能 设置设备所在区域地址
  // 参 数
  // cszDevAddr 设备地址
  // cszRgnAddr 设备区域地址
  // 说 明 当设备建立连接后可将设备设置到某一个区域，以后及可对这个区域进行搜索设备
  int cmd_SetDevRegionAddr(const char* cszGroupIP) {
    byte cid[IP_LEN];
    set_stream set(cid, 0);
    set.set_STRING(cszGroupIP, IP_LEN);
    return send_cmd_packet(CMD_M_MCAST, CMD_S_SET, 0, cid, IP_LEN);
  }
  int cmd_GetDevRgn() {
    return send_cmd_packet(CMD_M_MCAST, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 设置设备系统时间
  // 参 数
  // cszDevAddr 设备地址
  // cSysTime 时间信息
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_SYS_TIME_OPER
  // eFlag = DEV_OPER_UPLOAD ，
  // pvContent = NULL
  int cmd_SetDevTime(DATETIME cTime) {
    byte* dt = new byte[DATETIME_size];
    set_stream set(dt, 0);
    set.set_DATETIME(cTime);
    return send_cmd_packet(CMD_M_TIME, CMD_S_SET, 0, dt, DATETIME_size);
  }
  // 功 能 获取设备系统时间
  // 参 数 cszDevAddr 设备地址
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_SYS_TIME_OPER
  // eFlag = DEV_OPER_DOWNLOAD，
  // pvContent = DEV_DATETIME*
  int cmd_GetDevTime() {
    return send_cmd_packet(CMD_M_TIME, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 获取设备基本信息
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_GetDevBaseInfo() {
    return send_cmd_packet(CMD_M_DEVINFO, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 获取设备统计信息
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_GetDevStatisInfo() {
    return send_cmd_packet(CMD_M_STATIS, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 获取人脸机工作属性
  // 参 数
  // cszDevAddr 人脸机地址
  // nSeq 操作序列号
  // 说 明 工作属性DEV_WORKATT通过回调函数给出。
  int cmd_GetDevWorkAttInfo() {
    return send_cmd_packet(CMD_M_DEVSET, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 设置设备工作属性信息
  // 参 数
  // cszDevAddr 设备地址
  // cWorkAtt 工作属性结构体
  // nSeq 操作序列号
  int cmd_SetDevWorkAttInfo(WORKATT workatt) {
    int pSet_len = 4 * (TGROUP_NUM + 3) + 4 + ID_LEN + 20;
    byte* pSet = new byte[pSet_len];
    set_stream set(pSet, 0);
    set.set_WORKATT(workatt);
    return send_cmd_packet(CMD_M_DEVSET, CMD_S_SET, 0, pSet, pSet_len);
  }
  // static int USER_JPEG_SIZE = (64 * 1024);
  // static int REC_JPEG_SIZE = (18 * 1024);
  // static int FEAT_SIZE = (90 * 1024);
  // 功 能 上传指定用户
  // 参 数
  // cszDevAddr 设备地址
  // cUser 用户信息结构体
  // nSeq 操作序列号
  // 说 明
  // 用户无人脸库时，DEV_USER中的 m_FeatLen = 0， m_FeatData = NULL
  // 用户无注册照时，DEV_USER中的 m_PicLen= 0， m_PicData = NULL
  // DevOperResultNotify回调参数对应类型：eType = DEV_USER_OPER
  // eFlag = DEV_OPER_UPLOAD，pvContent= NULL
  int cmd_SetUser(USER cUser) {
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
    int len = set.off;
    return send_cmd_packet(CMD_M_USER, CMD_S_SET, 0, pUser, len);
  }
  // 功 能 上传实时加载用户
  // 参 数
  // cszDevAddr 设备地址
  // cUser 用户信息结构体
  // m_CFlag 实时加载信息 [F_DL_APPENDID|F_DL_PIC|F_DL_FEAT]
  // nSeq 操作序列号
  // 说 明 设备端请求客户端上传某用户时，通过此?涌谏洗肭笥没畔ⅲ?
  // cRtLoad为设备端请求时传过来的信息，客户端禁止修改。此函数实现远程用户加载功能。
  int cmd_SetRealTimeUser(USER cUser, int m_CFlag) {
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
    int len = set.off;
    return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_LOAD, m_CFlag, pUser,
        len);
  }
  // 功 能 远程实时采集用户特征
  // 参 数
  // cszDevAddr 设备地址
  // cUser 用户信息
  // 说 明 管理端录入用户资料时，可通过该函数将用户资料发送的人脸机，控制人脸机采集该用户的人脸。
  // 远程用户采集功能，使得直接在管理端就可以采集用户，在配合远程视频的情况下采集的可视化程度和直接在机器上采集一样。
  // 如果使能了(用户采集实时同步)。远程采集的用户也会实时的发回管理端.
  int cmd_SetRealTimeUserCap(const char* cUserID) {
    USER cUser = new USER();
    cUser.m_UserID = cUserID;
    byte* pUser = new byte[cUser.size()];
    set_stream set(pUser, 0);
    set.set_USER(cUser);
    int len = set.off;
    li->msg_DEBUGMSG(this, "send_cmd_packet len=" + len + " cUserID = " + cUserID + "cUser.size()=" + cUser.size());
    return send_cmd_packet(CMD_M_USER, CMD_S_REALTIME_CAP, 0, pUser, len);
  }
  // 功 能 删除指定用户
  // 参 数
  // cszDevAddr 设备地址
  // cID 用户编号
  // nSeq 操作序列号
  // 说 明 删除成功与否，通过操作回调函数给出结果
  // DevOperResultNotify回调参数对应类型：
  // eType = DEV_USER_OPER
  // eFlag = DEV_OPER_DELETE，
  // pvContent = NULL
  int cmd_DelUser(const char* cID) {
    byte* cid = new byte[ID_LEN];
    set_stream set(cid, 0);
    set.set_CID(cID);
    return send_cmd_packet(CMD_M_USER, CMD_S_DEL, 0, cid, ID_LEN);
  }
  // 功 能 下载指定用户
  // 参 数
  // cszDevAddr 设备地址
  // cID 用户ID
  // nFlag F_DL_PIC 需下载用户照片
  // F_DL_FEAT 需要下载人脸特征
  // F_DL_PIC|F_DL_FEAT=两者
  // 为0默认下载文字信息
  // nSeq 操作序列号
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_USER_OPER
  // eFlag = DEV_OPER_DOWNLOAD，
  // pvContent = USER*
  int cmd_GetSingleUser(const char* cID, int m_CFlag) {
    byte* cid = new byte[ID_LEN];
    set_stream set(cid, 0);
    set.set_CID(cID);
    return send_cmd_packet(CMD_M_USER, CMD_S_GET, 0, cid, ID_LEN);
  }
  // 功 能 下载所有用户
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_GetAllUser() {
    return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // 功 能 下载某一时间段内的注?嵊没?
  // 参 数
  // cszDevAddr 设备地址
  // cStart 起始日期
  // cEnd 终止日期
  // 说 明
  int cmd_GetSegTimeUser(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[16];
    set_stream set(seg, 0);
    set.set_NETDATE(cStart);
    set.set_NETDATE(cEnd);
    return send_cmd_packet(CMD_M_USER, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        seg, 16);
  }
  // 功 能 下载识别记录区间
  // 参 数
  // cszDevAddr 设备地址
  // nSeq 操作序列
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_RECORD_OPER
  // eFlag = DEV_OPER_DOWNLOAD
  // pvContent = DEV_DL_RECORD_ARRAY*
  // 参考DEV_DL_RECORD_ARRAY结构，一天为一个区间，该函数返回识别记录的所有区间。
  int cmd_GetAllIdentifyRecord() {
    return send_cmd_packet(CMD_M_VERIFY, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // 功 能 下载指定时间段识别记录
  // 参 数 cszDevAddr 设备地址
  // cStart 起始日期
  // cEnd 终止日期
  // 说 明
  int cmd_GetSegTimeIdentifyRecord(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[NETDATE.size * 2];
    set_stream set(seg, 0);
    set.set_NETDATE(cStart);
    set.set_NETDATE(cEnd);
    return send_cmd_packet(CMD_M_USER, CMD_S_GET,
        F_DL_ID | F_ID_TIME_RANGE, seg, NETDATE.size * 2);
  }
  // 功 能 获取区间识别记录/报警记录指定ID的后续区间
  // 参 数
  // cszDevAddr 设备地址
  // cRange ID 区间
  // m_CFlag F_DL_PIC=需要下载记录照片
  // bKnowRange true=具体数据， false=区间集合
  // nSeq 操作序列
  // 说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
  // DevOperResultNotify回调参数对应类型：
  // 下载区间时
  // pvContent = DEV_DL_RECORD_ARRAY*
  // cRange.m_SID = 指定SN
  // cRange.m_Count = -1;
  // cRange.m_bOpenRange = TRUE;
  // bKnowRange = FALSE，flag = 1（无效参数）
  // 下载具体记录时
  // DevOperResultNotify回调参数对应类型：
  // pvContent = DEV_RECORD*
  int cmd_GetRangeRec(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
        RECORDRANGE.size);
  }
  // 功 能 下载报警记录区间
  // 参 数
  // cszDevAddr 设备地址
  // nSeq 操作序列
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_ALARM_OPER
  // eFlag = DEV_OPER_DOWNLOAD，
  // pvContent = DEV_DL_RECORD_ARRAY*
  // 参考DEV_DL_RECORD_ARRAY结构，报警记录只有一个区间
  int cmd_GetAllAlarmRecord() {
    return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // 功 能 下载指定时间段报警记录
  // 参 数 cszDevAddr 设备地址
  // cStart 起始日期
  // cEnd 终止日期
  // 说 明
  int cmd_GetSegTimeAlarmRecord(NETDATE cStart, NETDATE cEnd) {
    byte* seg = new byte[NETDATE.size * 2];
    set_stream set(seg, 0);
    set.set_NETDATE(cStart);
    set.set_NETDATE(cEnd);
    return send_cmd_packet(CMD_M_ALARM, CMD_S_GET, F_DL_ID
        | F_ID_TIME_RANGE, seg, NETDATE.size * 2);
  }
  // 功 能 下载区间报警记录
  // 参 数 cszDevAddr 设备地址
  // cRange ID区间
  // flag DL_DEV_RECORD_PIC=需要下载记录照片 0x00=不下载照片, 0x01下载照片
  // bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
  // 说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
  int cmd_GetRangeAlarm(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
        RECORDRANGE.size);
  }
  // 功 能 下载所有日志
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_GetLog() {
    return send_cmd_packet(CMD_M_OPTLOG, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // 功 能 下载区间日志
  // 参 数 cszDevAddr 设备地址
  // cRange ID区间
  // bKnowRange 指定是下载具体数据还是下载ID集合(true=具体数据， false=ID集合)
  // 说 明 此接口用于从某一条精确的记录ID开始下载nRang条数据
  int cmd_GetRangeLog(RECORDRANGE cRange, int m_CFlag) {
    byte* bytes = new byte[RECORDRANGE.size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes,
        RECORDRANGE.size);
  }
  // 功 能 获取变更注册照记录区间
  // 参 数
  // cszDevAddr 设备地址
  // nSeq 操作序列
  // 说 明
  // DevOperResultNotify回调参数对应类型：
  // eType = DEV_CHANGEIMAGE_OPER
  // eFlag = DEV_OPER_DOWNLOAD，
  // pvContent = DEV_DL_RECORD_ARRAY*
  // 参考DEV_DL_RECORD_ARRAY结构，变更注册照只有一个区间
  int cmd_GetChangeImage() {
    return send_cmd_packet(CMD_M_CIMAGE, CMD_S_GET, F_DL_ID | F_DL_ID_ALL,
        NULL, 0);
  }
  // 功 能 下载指定区间的变更注册照记录或者查询区间数据的具体大小
  // 参 数
  // cszDevAddr 设备地址
  // cRange 区间, 参考区间定义
  // flag DL_DEV_RECORD_PIC=需要下载记录照片
  // bKnowRange true=具体数据， false=区间集合
  // nSeq 操作序列号
  // 说 明 当bKnowRange 为true该函数下载具体的数据，为false时查询指定区间在设备上的具体大小。
  // 下载区间时
  // DevOperResultNotify回调参数对应类型：
  // pvContent = DEV_DL_RECORD_ARRAY*
  // cRange.m_SID = 指定SN
  // cRange.m_Count = -1;
  // cRange.m_bOpenRange = TRUE;
  // bKnowRange = FALSE，flag = 1（无效参数）
  // 下载具体记录时
  // DevOperResultNotify回调参数对应类型：
  // pvContent=DEV_RECORD*
  int cmd_GetRangeCImage(RECORDRANGE cRange, int m_CFlag) {
    byte bytes[RECORDRANGE_size];
    set_stream set(bytes, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RANGE, CMD_S_GET, m_CFlag, bytes, RECORDRANGE_size);
  }
  int cmd_TimeGroup_impl(DSPTIMEG* cArray, int cArray_len, int m_SCmd, int m_CFlag) {
    byte* nData = NULL;
    int nData_len=cArray_len * DSPTIMEG_size + 4;
    myrealloc(&nData, nData_len);
    int ret;
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_DSPTIMEG(cArray+i);
    }
    ret = send_cmd_packet(CMD_M_TGROUP, m_SCmd, m_CFlag, nData, nData_len);
    free(nData);
    return ret;
  }
  // 功 能 设置时间组
  // 参 数 cszDevAddr 设备地址
  // cTGArray 时间组
  // 说 明
  int cmd_SetTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_SET, F_DL_NORMAL_TG | F_DL_HOLIDAY_TG);
  }
  // 功 能 获取时间组
  // 参 数 dev_addr 设备地址
  // cTGArray 时间组
  // m_CFlag 时间组类型
  // 说 明 cTGArray.m_nCount=0时表示下载eType类型的所有时间组
  int cmd_GetTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len, int m_CFlag) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_GET, m_CFlag);
  }
  // 功 能 删除时间组
  // 参 数 cszDevAddr 设备地址
  // cTGArray 时间组
  // eType 时间组类型
  // 说 明 eType.m_nCount=0时表示删除tg_type类型所有时间组
  int cmd_DelTimeGroup(DSPTIMEG* cTGArray, int cTGArray_len, int m_CFlag) {
    return cmd_TimeGroup_impl(cTGArray, cTGArray_len, CMD_S_DEL, m_CFlag);
  }
  int cmd_Right_impl(DSPRIGHT* cArray, int cArray_len, int m_SCmd) {
    byte* nData = NULL;
    int nData_len = cArray_len * DSPRIGHT_size + 4;
    myrealloc(&nData, cArray_len);
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_DSPRIGHT(cArray+i);
    }
    return send_cmd_packet(CMD_M_DSPRIGHT, m_SCmd, 0, nData, nData_len);
  }
  // 功 能 设置权限
  // 参 数 cszDevAddr 设备地址
  // cRTArray 权限
  // 说 明
  int cmd_SetRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_SET);
  }
  // 功 能 发送获取权限
  // 参 数 cszDevAddr 设备地址
  // cRTArray 权限
  // 说 明 当rt_array.m_nCount=0时表示下载所有权限
  int cmd_GetRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_GET);
  }
  // 功 能 删除权限
  // 参 数 cszDevAddr 设备地址
  // cRTArray 权??
  // 说 明 当rt_array.m_nCount=0时表示删除所有权限
  int cmd_DelRight(DSPRIGHT* cRTArray, int cArray_len) {
    return cmd_Right_impl(cRTArray, CMD_S_DEL);
  }
  int cmd_UserGroup_impl(USERGROUP* cArray, int m_SCmd) {
    byte* nData = new byte[cArray_len * DSPRIGHT.size + 4];
    set_stream set(nData, 0);
    set.set_INT(cArray_len);
    for (int i = 0; i < cArray_len; ++i) {
      set.set_USERGROUP(cArray[i]);
    }
    return send_cmd_packet(CMD_M_UGROUP, m_SCmd, 0, nData, nData_len);
  }
  // 功 能 设置用?ё?
  // 参 数 cszDevAddr 设备地址
  // cUGrray 用户组
  // 说 明
  int cmd_SetUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_SET);
  }
  // 功 能 发送获取用户组
  // 参 数 cszDevAddr 设备地址
  // ug_array 用户组
  // 说 明 当rt_array.m_nCount=0时表示下载所有用户组
  int cmd_GetUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_GET);
  }
  // 功 能 删除用户组
  // 参 数 cszDevAddr 设备地址
  // ug_array 用户组
  // 说 明 当rt_array.m_nCount=0时表示删除所有用户组
  int cmd_DelUserGroup(USERGROUP* cUGrray) {
    return cmd_UserGroup_impl(cUGrray, CMD_S_DEL);
  }
  // 功 能 设置开关门状态
  // 参 数
  // cszDevAddr 设备地址
  // bEOpen true=紧急开门， false=紧急关门
  // 说 明
  int cmd_SetOpenDoorState(int eState) {
    return send_cmd_packet(CMD_M_EOPEN, CMD_S_SET, eState, NULL, 0);
  }
  // 功 能 获取开关门状态
  // 参 数 cszDevAddr 设备地址
  // bEOpen true=紧急开门， false=紧急关门
  // 说 明
  int cmd_GetOpenDoorState() {
    return send_cmd_packet(CMD_M_EOPEN, CMD_S_GET, 0, NULL, 0);
  }
  // 功 能 远程开门
  // 参 数 cszDevAddr 设备地址
  // bEOpen true=开门， false=关门
  // pDoorArray 门点数组
  // size 数组大小
  // 说 明
  int cmd_SetRemoteOpenDoor(int bOpen) {
    int nFlag = 0;
    if (0 != bOpen) {
      nFlag = F_DOOR_OPEN;
    }
    else {
      nFlag = F_DOOR_CLOSE;
    }
    return send_cmd_packet(CMD_M_REMOTE_OPEN, CMD_S_SET, nFlag, NULL, 0);
  }
  // 功 能 IO设备控制
  // 参 数
  // cszDevAddr 设备地址
  // DEV_IOCTRL 控制信息
  // nSeq 命令执行序列号
  // 说 明 DEV_IOCTRL:m_Source指定要做操作的端口，DEV_IOCTRL:m_Action要执行的动作
  // 打开门1，m_Source = 0x1f m_Action = DEV_ACT_IO_OPEN
  // 打辅助1，m_Source = 0x15 m_Action = DEV_ACT_IO_OPEN
  // 0x1f和015具体怎么解析的，请参考485地址解析
  int cmd_IOCtrl(IOCTRL ioctrl) {
    byte* nData = new byte[IOCTRL.size];
    set_stream set(nData, 0);
    set.set_IOCTRL(ioctrl);
    return send_cmd_packet(CMD_M_IOCTRL, CMD_S_SET, 0, nData, IOCTRL.size);
  }
  // 功 能 格式化设备
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_SetFormat() {
    return send_cmd_packet(CMD_M_FORMAT, CMD_S_SET, 0, NULL, 0);
  }
  // 功 能 设备程序更新
  // 参 数 cszDevAddr 设备地址
  // cAppData 程序更新结构体
  // 说 明
  int cmd_SetUpdate(APPUPDATE cAppData) {
    byte* nData = new byte[cAppData.size()];
    set_stream set(nData, 0);
    set.set_APPUPDATE(cAppData);
    return send_cmd_packet(CMD_M_APPUPDATE, CMD_S_SET, 0, nData,
        nData_len);
  }
  // 功 能 批量上传用户
  // 参 数
  // cszDevAddr 设备地址
  // cUsers 批量用户数据
  // nFlag F_DL_PIC //照片
  // DL_DEV_USER_FEAT //人脸特征
  // DL_DEV_USER_TEXT //用户文字信息
  // 说 明: 批量上传用户必须包含用户文字信息，组合如下
  // nFlag = DL_DEV_USER_TEXT 上传文字信息
  // nFlag = DL_DEV_USER_TEXT|F_DL_PIC 文字+注册照
  // nFlag = DL_DEV_USER_TEXT|DL_DEV_USER_FEAT 文字+人脸库
  // nFlag = DL_DEV_USER_TEXT|F_DL_PIC|DL_DEV_USER_FEAT文字+注册照+人脸库
  // DevOperResultNotify回调参数对应类型：
  // eType = DEV_USER_EX_OPER
  // eFlag = DEV_OPER_UPLOAD ，
  // pvContent = DEV_BATCH_USER*。
  // 在OPER_SUCCESS
  // 情况下，批量上传的用户个数与返回的用户个数一致。否则设备保存失败。批量上传用户数设定在5个以下(和网络带宽，主机处理速度有关)。
  int cmd_SetUserEx(USER* cUsers, int nFlag) {
    int sz = 0;
    for (int i = 0; i < cUsers_len; ++i) {
      sz += cUsers[i].size();
    }
    byte* nData = new byte[sz + 4];
    set_stream set(nData, 0);
    set.set_INT(cUsers_len);
    for (int i = 0; i < cUsers_len; ++i) {
      set.set_USER(cUsers[i]);
    }
    return send_cmd_packet(CMD_M_USER_EX, CMD_S_SET, USER_TEXT, nData,
        nData_len);
  }
  int cmd_UserEx_impl(const char** pUserIDArray, int m_SCmd, int m_CFlag,
      int max_n) {
    int ret = 0;
    for (int j = 0; j < pUserIDArray_len;) {
      int m_Count = MIN(max_n, pUserIDArray_len - j);
      byte* nData = new byte[ID_LEN * m_Count + 8];
      set_stream set(nData, 0);
      set.set_INT(m_Count);
      for (int i = 0; i < m_Count; ++i) {
        set.set_CID(pUserIDArray[j + i]);
      }
      ret = send_cmd_packet(CMD_M_USER_EX, m_SCmd, m_CFlag, nData,
          nData_len);
      j += m_Count;
    }
    return ret;
  }
  // 功 能 批量下载用户
  // 参 数
  // cszDevAddr 设备地址
  // pUserIDArray 用户ID数组首地址
  // nIDCount 数组元素个数
  // nFlag 同cmd_SetUserEx
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = USER_TEXT DL_FEAT DL_IMAGE
  // eFlag = DEV_OPER_DOWNLOAD ，
  // pvContent= DEV_BATCH_USER*。
  // OPER_SUCCESS 情况下，如果批量下载的用户数与要求下载的用户数不一致，可以通过
  // cmd_GetAllUser() 来确认用户是否真的存在于设备上。 批量下载用户数设定在5个以下(和网络带宽，主机处理速度有关)。
  int cmd_GetUserEx(const char** pUserIDArray, int m_CFlag) {
    return cmd_UserEx_impl(pUserIDArray, CMD_S_GET, m_CFlag, 5);
  }
  // 功 能 批量删除用户
  // 参 数
  // cszDevAddr 设备地址
  // pUserIDArray 待删除用户的编号数组
  // nIDCount 数组元素个数
  // 说 明 DevOperResultNotify回调参数对应类型：
  // eType = DEV_USER_EX_OPER
  // eFlag = DEV_OPER_DELETE，
  // pvContent = DEV_BATCH_USER*。
  // 在OPER_SUCCESS 情况下，批量删除的用户个数与返回的用户个数不一致，可以通过cmd_GetAllUser()
  // 来确认用户是否真的存在于设备上。单次批量删除用户数设定在10个以下为好。
  int cmd_DelUserEx(const char** pUserIDArray) {
    return cmd_UserEx_impl(pUserIDArray, CMD_S_DEL, 0, 1000);
  }
  // 功 能 批量下载记录
  // 参 数 cszDevAddr 设备地址
  // cRange ID区间
  // 说 明 包括验证记录、报警记录，操作日志、变更注册照
  int cmd_GetRecEx(RECORDRANGE cRange) {
    byte* nData = new byte[RECORDRANGE.size];
    set_stream set(nData, 0);
    set.set_RECORDRANGE(cRange);
    return send_cmd_packet(CMD_M_RECORD_EX, CMD_S_GET, F_DL_PIC, nData,
        nData_len);
  }
  // 功 能 重启设备
  // 参 数 cszDevAddr 设备地址
  // 说 明
  int cmd_RebootDev() {
    return send_cmd_packet(CMD_M_REBOOT, CMD_S_SET, 0, NULL, 0);
  }
  // 功 能 获得/设置 设备网络信息
  // 参 数 cszDevAddr 设备地址
  // pNetPro NULL表示获得，不为空表示设置
  // 说 明
  int cmd_SetNetPro(NETPRO pNetPro) {
    byte* nData = new byte[NETPRO.size];
    set_stream set(nData, 0);
    set.set_NETPRO(pNetPro);
    return send_cmd_packet(CMD_M_NETPRO, CMD_S_SET, 0, nData, nData_len);
  }
  int cmd_GetNetPro() {
    return send_cmd_packet(CMD_M_NETPRO, CMD_S_GET, 0, NULL, 0);
  }
  int connect(const char* ip, const char* user, const char* pass, listener* listener) {
    try {
      devip = ip;
      li = listener;
      InetSocketAddress address = NULL;
      address = new InetSocketAddress(ip, TCPPORT);
      sc = new Socket();
      sc.connect(address, 1000);
      sc.setSoTimeout(1000);
      netInputStream = new DataInputStream(sc.getInputStream());
      netOutputStream = new DataOutputStream(sc.getOutputStream());
      li->msg_CONNECT_SUCCESS(this);
      cmd_SetDevAuth(user, pass, 1);
      msg_identify = -1;
      start();
      // 1.send file length
    }
    catch (SocketTimeoutException e) {
      li->msg_DEBUGMSG(this, "connetc 超时 :" + ip + " 异常");
    }
    catch (UnknownHostException e) {
      li->msg_DEBUGMSG(this, "-.未知的主机名:" + ip + " 异常");
    }
    catch (IOException e) {
      
    }
    catch (Exception e) {
      
      li->msg_DEBUGMSG(this, "-hostName=" + ip + " portNum=" + TCPPORT
          + "---.IO异常错误" + e.getMessage());
    }
    return 0;
  }
  struct video_th {
    DatagramSocket ds = NULL;
    int m_nIndex;
    int msg_len;
    int msg_identify;
    int port;
    volatile boolean exit = false;
    pmdev* dev;
    video_th() {
    }
    int startUDP() {
      int port = 0;
      exit = true;
      try {
        ds = new DatagramSocket(0);
        ds.close();
        ds = new DatagramSocket(0);
        port = ds.getLocalPort();
        ds.setSoTimeout(1000);
      }
      catch (Exception e) {
        
        throw new RuntimeException(e);
      }
      finally {
        start();
      }
      return port;
    }
    int clip(int x) {
      if (x < 0) {
        return 0;
      }
      if (x > 255) {
        return 255;
      }
      return x;
    }
    int* YUYV_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int r, g, b;
      int R, G, B;
      int Y00, Y01, U, V;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
      for (int i = 0; i < height; i++) {
        int startY = off + i * step;
        int index = i * width;
        for (int j = 0; j < width; j += 2) {
          Y00 = src[startY + 0];
          U = src[startY + 1];
          Y01 = src[startY + 2];
          V = src[startY + 3];
          startY += 4;
          r = (int)(1.4075 * ((V & 0xff) - 128));
          g = (int)(0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
          b = (int)(1.779 * ((U & 0xff) - 128));
          R = ((Y00 & 0xff) + r);
          G = ((Y00 & 0xff) - g);
          B = ((Y00 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
          R = (int)((Y01 & 0xff) + r);
          G = (int)((Y01 & 0xff) - g);
          B = (int)((Y01 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
        }
      }
      return rgb;
    }
    int* UYVY_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int r, g, b;
      int R, G, B;
      int Y00, Y01, U, V;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
      for (int i = 0; i < height; i++) {
        int startY = off + i * step;
        int index = i * width;
        for (int j = 0; j < width; j += 2) {
          U = src[startY + 0];
          Y00 = src[startY + 1];
          V = src[startY + 2];
          Y01 = src[startY + 3];
          startY += 4;
          r = (int)(1.4075 * ((V & 0xff) - 128));
          g = (int)(0.3455 * ((U & 0xff) - 128) + 0.7169 * ((V & 0xff) - 128));
          b = (int)(1.779 * ((U & 0xff) - 128));
          R = ((Y00 & 0xff) + r);
          G = ((Y00 & 0xff) - g);
          B = ((Y00 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
          R = (int)((Y01 & 0xff) + r);
          G = (int)((Y01 & 0xff) - g);
          B = (int)((Y01 & 0xff) + b);
          R = clip(R);
          G = clip(G);
          B = clip(B);
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
        }
      }
      return rgb;
    }
    int* RGB24_RGB_int(byte* src, int off, int step, int width,
        int height) {
      int R, G, B;
      int numOfPixel = width * height;
      int* rgb = new int[numOfPixel];
      for (int i = 0; i < height; i++) {
        int startY = off + i * step;
        int index = i * width;
        for (int j = 0; j < width; j += 1) {
          R = src[startY + 0];
          G = src[startY + 1];
          B = src[startY + 2];
          startY += 3;
          rgb[index] = ((B) + (G << 8) + (R << 16) + (255 << 24));
          index += 1;
        }
      }
      return rgb;
    }
    int close() {
      try {
        exit = true;
        // sleep(1000); // 主线程延迟1秒
        // interrupt();
        join();
      }
      catch (Exception e) {
        
      }
      return 0;
    };
    void run() {
      msg_len = 0;
      msg_identify = -1;
      exit = false;
      dev.li->msg_DEBUGMSG(dev, "video Thread run");
      int PACK_SIZEEX = PACK_SIZE + 20;
      byte* buf = new byte[PACK_SIZEEX];
      byte* data = NULL;
      int pre_identify = -1, len = 0;
      while (!ds.isClosed() && !exit) {
        try {
          DatagramPacket dp = NULL;
          dp = new DatagramPacket(buf, PACK_SIZEEX); // 用于接收长度为1024的数据包，并将数据包的数据部分存在buf中
          ds.receive(dp); // 从此套接字接收数据报包
          // li->msg_DEBUGMSG(this, "ip" + dp.getAddress().toconst char*() +
          // " len=" + dp.getLength());
          int recelen = dp.getLength();
          if (recelen < 0) {
            continue;
          }
          if (sock_checksum(buf, 0, 20) != 0) {
            dev.li->msg_DEBUGMSG(dev, "校验错误");
            continue;
          }
          get_stream get = get_stream(buf, 0);
          int msg_identify = get.get_INT(); // 标识
          int msg_offset = get.get_INT(); // 偏移
          int msg_len = get.get_INT();// 当已经传送过来的命令数据长度，包括命名头的长度，不包括网络包头长度
          int msg_total = get.get_INT();// 总的命令数据长度，包括命令头长度和命令的参数长度
          // int msg_checksum = ;
          get.get_INT();// 检验和
          // dev.li->msg_DEBUGMSG(dev, "msg_identify=" + msg_identify +
          // " msg_offset=" + msg_offset + " msg_len=" + msg_len +
          // " msg_total=" + msg_total);
          if (pre_identify != msg_identify && 0 == msg_offset) {
            if (NULL == data || data_len < msg_total) {
              data = new byte[msg_total];
            }
            len = 0;
          }
          bytecopy(buf, 20, data, msg_offset, msg_len);
          len += msg_len;
          if (len >= msg_total) {
            get = get_stream(data, 20);
            int m_Size = get.get_INT(); // 图像数据总大小BYTES
            int m_W = get.get_INT(); // 图像宽
            int m_H = get.get_INT(); // 图像高
            int m_Step = get.get_INT(); // 行数据步长
            int m_Type = get.get_BYTE(); // 数据类型 IMAGE_UYVY
            int off = 4 * 4 + 1;
            //dev.li->msg_DEBUGMSG(dev, "m_Size=" + m_Size + " m_W="
            // + m_W + " m_H=" + m_H + " m_Step=" + m_Step);
            int* rgb = NULL;
            if (IMAGE_YUYV == m_Type) {
              rgb = YUYV_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else if (IMAGE_UYVY == m_Type) {
              rgb = UYVY_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else if (IMAGE_RGB24 == m_Type) {
              rgb = RGB24_RGB_int(data, 20 + off, m_Step, m_W, m_H);
            }
            else {
              rgb = NULL;
            }
            // BufferedImage image = new BufferedImage(WIDTH,
            // HEIGHT, BufferedImage.TYPE_3BYTE_BGR);
            // Image aa;
            // Bitmap bitmap = Bitmap.createBitmap(data, off,
            // m_Step, m_W, m_H,
            // Bitmap.Config.ARGB_8888);
            dev.li->msg_VIDEO_OUT(dev, rgb, 0, m_W, m_H, m_W);
          }
        }
        catch (SocketTimeoutException e) {
          ds.close();
          dev.li->msg_DEBUGMSG(dev, "udp 超时 :" + devip + " 异常");
        }
        catch (Exception e) {
          ds.close();
          dev.li->msg_DEBUGMSG(dev, "udp receive DatagramPacket 出错, 主动断开");
          return;
        }
      }
      ds.close();
      ds = NULL;
      dev.li->msg_DEBUGMSG(dev, "线程退出!");
    }
  };
  int ntohs(int x) {
    return (short)(((x & 0xff) << 8) | ((x >> 8) & 0xff));
  }
  video_th vth;
  int m_port = 0;
  int cmd_StartVideo(const char* myIP) {
    int ret = 0;
    if (NULL == vth) {
      vth = new video_th();
      vth.dev = this;
      m_port = vth.startUDP();
      byte* nData = new byte[18];
      set_stream set(nData, 0);
      // int port1 = ntohs(port);
      set.set_SHORT((short) m_port);
      set.set_STRING(myIP, 16);
      li->msg_DEBUGMSG(this, "cmd_StartVideo ip = " + myIP + " port="
          + m_port);
      ret = send_cmd_packet(CMD_M_VIDEO_OUT, CMD_S_SET, 0, nData,
          nData_len);
    }
    else {
      li->msg_DEBUGMSG(this, "视频已经打开，请先关闭!");
    }
    return ret;
  }
  // 停止视频
  int cmd_StopVideo() {
    int retVal = 0;
    byte* nData = new byte[18];
    set_stream set(nData, 0);
    // 关闭视频
    // 组命令数据
    set.set_SHORT((short) m_port);
    set.set_STRING(devip, 16);
    retVal = send_cmd_packet(M_VIDEO_OUT, S_GET, 0, nData, nData_len);
    if (vth != NULL) {
      vth.close();
      vth = NULL;
    }
    else {
      li->msg_DEBUGMSG(this, "视频已关闭!无需再关闭!");
    }
    return retVal;
  }
  static int test1() {
    pmdev client = new pmdev();
    //155
    client.connect("192.168.1.46", "admin", "201031",
    new pmdev.listener() {
      int println(const char* s) {
        System.out.println(s);
        return 0;
      }
      int msg_DEBUGMSG(pmdev * dev, const char* s) {
        println("DEBUG: " + s);
        return 0;
      }
      int msg_CONNECT_SUCCESS(pmdev * dev) {
        println("主机名:" + dev.get_ip() + "成功");
        return 0;
      }
      int msg_CONNECT_CUT(pmdev * dev) {
        println("主机名:" + dev.get_ip() + "断开链接");
        return 0;
      }
      int msg_AUTH(pmdev * dev, int m_CFlag) {
        println("msg_AUTH");
        if ((m_CFlag & F_AUTH_VERIFY) != 0) {
          println("用户身份验证 成功!");
        }
        else {
          println("设备验证用户设置操作 成功!");
        }
        // dev.cmd_GetAllUser();
        // dev.cmd_GetNetPro(); // 获取网络配置信息
        // dev.cmd_GetAllIdentifyRecord();
        dev.cmd_StopVideo();
        dev.cmd_StartVideo("192.168.1.3");
        try {
          sleep(1000);
        }
        catch (InterruptedException e) {
          
        } // 延迟1秒
        //dev.cmd_StopVideo();
        //dev.cmd_StartVideo("192.168.1.3");
        dev.cmd_GetDevBaseInfo();
        //println("cmd_SetRealTimeUserCap!");
        //dev.cmd_SetRealTimeUserCap("0000");
        // dev.cmd_GetDevStatisInfo();
        return 0;
      }
      int msg_NETPRO(pmdev * dev, pmdev.NETPRO netpro) {
        println("msg_NETPRO");
        println("设备%s:获取网络信息成功");
        println("<< 设备基本信息 ");
        println(" m_WebPro1: " + netpro->m_WebPro1);
        println(" m_WebPro2: " + netpro->m_WebPro2);
        println(" m_WebUrl1: " + netpro->m_WebUrl1);
        println(" m_WebUrl2: " + netpro->m_WebUrl2);
        println(" m_IPAdr: " + netpro->m_IPAdr);
        println(" m_MIPAdr: " + netpro->m_MIPAdr);
        println(" m_NetMask: " + netpro->m_NetMask);
        println(" m_GateWay: " + netpro->m_GateWay);
        println(" m_Dns: " + netpro->m_Dns);
        println(">>");
        return 0;
      }
      int msg_DEVINFO(pmdev * dev, pmdev.BASEINFO baseinfo) {
        println("msg_DEVINFO");
        println("设备%s:获取基本信息成功");
        println("<< 设备基本信息 ");
        println(" 设备基本信息 " + baseinfo->tostring());
        println(">>");
        return 0;
      }
      int msg_STATIS(pmdev * dev, pmdev.STATIS statis) {
        println("msg_STATIS");
        println("设备%s:获取统计信息成功: " + dev.get_ip());
        println(" 设备统计信息 " + statis->tostring());
        return 0;
      }
      int msg_RECORDRANGE(pmdev * dev, int type,
      pmdev.RECORDRANGE* range) {
        println("msg_RECORDRANGE");
        println("记录类型: " + type);
        println("记录区间数目: " + range_len);
        for (int i = 0; i < range_len; ++i) {
          println("m_Count: " + range[i].m_Count
              + " m_Flag: " + range[i].m_Flag
              + " m_SID: " + range[i].m_SID);
        }
        if (range_len > 0) {
          dev.cmd_GetRecEx(range[0]);
        }
        return 0;
      }
      int msg_RECORD(pmdev * dev, int type,
      pmdev.RECORD* range) {
        println("msg_RECORD");
        for (int i = 0; i < range_len; ++i) {
          println(i + " ID: " + range[i].m_UserID + "时间"
              + range[i].m_RecTime.tostring());
        }
        return 0;
      }
      int msg_RECORDEX(pmdev * dev, int type,
      pmdev.RECORD* range) {
        println("msg_RECORDEX");
        for (int i = 0; i < range_len; ++i) {
          println(i + " ID: " + range[i].m_UserID + "时间"
              + range[i].m_RecTime.tostring());
        }
        return 0;
      }
      int msg_VUSER(pmdev * dev, pmdev.VUSER* vuser) {
        println("msg_VUSER");
        println("获取用户列表成功!");
        for (int i = 0; i < vuser_len; ++i) {
          // println("用户: " + vuser[i].m_UserID);
        }
        if (vuser_len > 2) {
          // 下载两个人的数据
          const char** strlist = new const char*[2];
          strlist[0] = vuser[0].m_UserID;
          strlist[1] = vuser[1].m_UserID;
          dev.cmd_GetUserEx(strlist, USER_TEXT | DL_FEAT
              | DL_IMAGE);
        }
        println("用户总数: " + vuser_len);
        return 0;
      }
      int msg_USER(pmdev * dev, pmdev.USER* user) {
        println("msg_USER");
        println("用户总数: " + user_len);
        for (int i = 0; i < user_len; ++i) {
          println("用户: " + user[i].m_UserID + "特征长度 "
              + user[i].m_Feat_len);
        }
        return 0;
      }
      int msg_VIDEO_OUT(pmdev * dev, int* data,
      int yuvdataoff, int w, int h, int step) {
        println("视频: w:" + w + " h:" + h + " datalen:" + data_len);
        ++aa;
        if (aa < 10) {
          println("asdfadsf");
          FileOutputStream fos = NULL;
          try {
            fos = new FileOutputStream("C:\\aaa.dat");
          }
          catch (FileNotFoundException e) {
            
          }
          // 用FileOutputStream 的write方法写入字节数组
          try {
            for (int i = 0; i < data_len; ++i) {
              fos.write(intToByteArray(data[i]));
            }
          }
          catch (IOException e) {
            
          }
          System.out.println("写入成功");
          // 为了节省IO流的开销，需要关闭
          try {
            fos.close();
          }
          catch (IOException e) {
            
          }
        }
        return 0;
      }
    });
    try {
      client.join();
    }
    catch (Exception e) {
      
    }
    client.close();
    System.out.println("退出");
    return 0;
  }
  static int aa = 0;
  static byte* intToByteArray(final int integer) {
    int byteNum = (40 - Integer.numberOfLeadingZeros(integer < 0 ? ~integer
        : integer)) / 8;
    byte* byteArray = NULL;
    if (integer < 0) {
      byteArray = new byte* { -1, -1, -1, -1 };
    }
    else {
      byteArray = new byte[4];
    }
    for (int n = 0; n < byteNum; n++) {
      byteArray[3 - n] = (byte)(integer >>> (n * 8));
    }
    return (byteArray);
  }
  static int test2() {
    const char* imgPath = "E://58.jpg";
    BufferedImage image;
    try {
      image = ImageIO.read(new FileInputStream(imgPath));
      System.out.println("getWidth " + image.getWidth());
      System.out.println("getHeight " + image.getHeight());
    }
    catch (Exception e) {
      
    }
    System.out.println("test2 退出");
    return 0;
  }
  static void main(const char** args) throws Exception {
    test1();
    return;
  }

};
#if 0

#endif

