# 基于Windows的TAPI的调制解调器编程 - xqhrs232的专栏 - CSDN博客
2013年01月24日 14:36:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：573
原文地址::[http://www.wangchao.net.cn/bbsdetail_16248.html](http://www.wangchao.net.cn/bbsdetail_16248.html)
一、 简 介:   
　　---- 随 着Win 操 作 系 统 的 普 及， 计 算 机 之 间 的 通 信 已 经 成 为 大 多 数 应 用 程 序 开 发 的 要 求， 这 其 中 的 主 要 原 因 是Win95 不 仅 仅 能 够 支 持 大 多 数 硬 件， 而 且 为 硬 件 的 操 作 提 供 了 方 便 的 编 程 接 口（API），Win 的 应 用 程 序 接 口（API）
 提 供 的 通 信 手 段 大 致 分 为 以 下 几 类： １ 基 于TCP/IP 协 议 的Winsock API， 可 实 现 局 域 网 上 或 互 联 网 上 的 微 机 通 信； ２ 基 于 进 程 之 间 的 通 信 技 术： 动 态 数 据 交 换（ Ｄ Ｄ Ｅ）； ３ 基 于 直 接 电 缆 连 接 的 通 信 技 术， 可 直 接 操 作 串 行 口、 并 行 口 以 及 远 红 外 线 接 口； ４ 基 于 电 话 线 路 的 通 信 应 用 程 序 接 口（ Ｔ Ａ Ｐ Ｉ/Telephony
 API）， 可 方 便 地 控 制调 制 解 调 器； 从 目 前 的 发 展 状 况 看 来， 调 制 解 调 器 已 经 成 为 远 距 离 通 信 的 一 种 重 要 工 具， 为 此Microsoft 及Intel 公 司 联 合 开 发 了TAPI 这 样 一 个 编 程 接 口， 而 且， 使 用API 函 数 编 制 的 程 序 段 既 适 用 于Borland C++ 编 译 器， 同 时 也 能 插 入Visual C++ 程 序 中 编 译 运 行， 作 为Win95 的 应 用 程 序
 编 制 人 员， 学 会 使 用TAPI 编 程 操 作   
　　调 制 解 调 器 通 过 电 话 线 路 进 行 通 信 这 一 技 术 是 很 有 必 要 的。   
　　下 面 就TAPI 编 程 进 行 讨 论：   
　　二、 通 信 过 程 描 述   
　　---- 1 初 始 化 线 路（ 通 信 双 方 都 应 该 初 始 化 线 路）   
　　---- 通 过 使 用lineInitialize 函 数 初 始 化TAPI.DLL 得 到TAPI 使 用 句 柄 的 指 针hTapi， 请 注 意 参 数 中 回 调 函 数 的 定 义（ 所 有 提 及 函 数 的 用 法 均 可 从BC++5.0 及Visual C++5.0 的 帮 助 中 获 得）； 通 过 调 用lineOpen 函 数（ 用
 到 参 数hTapi） 获 得 线 路 句 柄hLine； 再 利 用lineGetID( 用 到 参 数hLine) 获 取 调 制 解 调 器 句 柄hModem   
　　---- 2 配 置 线 路（ 可 选）   
　　----   
　　---- 调 用SetCommConfig( 用 到hModem) 改 变 调 制 解 调 器 的 设 置   
　　---- 3 拨 号（ 由 呼 叫 方 执 行）   
　　---- 使 用lineMakeCall 函 数( 用 到hLine) 进 行 拨 号， 完 成 后 获 得 呼 叫 句 柄hCall（ 呼 叫 方 的 呼 叫 句 柄）   
　　---- 4 应 答 链 接（ 由 被 呼 叫 方 执 行）   
　　---- 被 呼 叫 的 一 方 的 回 调 函 数 得 到LINECALLSTATE_OFFERING 消 息 时, 调 用lineAnswer 函 数 实 现 自 动 应 答( 呼 叫 句 柄hCall 由 回 调 函 数 的 参 数 给 出)   
　　---- 5 数 据 通 信（ 双 方）   
　　---- 当 回 调 函 数 收 到LINECALLSTATE_CONNECTED 消 息 后， 请 先 清 除 接 收 缓 冲 区， 可 以 使 用 函 数 为WriteFile 及ReadFile 函 数 进 行 数 据 交 换 ， 注 意 参 数hFile 为 调 制 解 调 器 句 柄hModem   
　　---- 6 挂 机（ 某 一 方）   
　　---- 通 信 完 毕 任 何 一 方 都 可 以 调 用 函 数lineDrop(hCall,NULL,0) 来 停 止 呼 叫， 该 函 数 还 发 送LINECALLSTATE_IDLE 消 息 给 回 调 函 数   
　　---- 7 关 闭 线 路（ 双 方）   
　　---- 通 信 双 方 的 回 调 函 数 在 收 到LINECALLSTATE_IDLE 消 息 时 都 应 该 调 用 函 数lineDeallocateCall(hCall) 释 放 相 应 呼 叫 占 用 的 资 源； 当 回 调 函 数 收 到LINECALLSTATE_DISCONNECTED 消 息 时 请 使 用lineClose(hLine)
 释 放 由lineOpen 分 配 的 资 源， 调 用lineShutDown(hTapi) 释 放 为 线 路 设 备 分 配 的 资 源   
　　三、 软 硬 件 环 境   
　　---- 下 图 示 意 出 了 我 们 的 应 用 程 序 所 处 的 位 置 以 及 涉 及 到 的 软 硬 件 环 境:   
　　---- 我 们 的 通 信 应 用 程 序 通 过TAPI 操 作Modem 拨 号、 应 答、 链 接、 挂 机 控 制 电 话 呼 叫， 在 编 制DOS 应 用 程 序 的 时 候， 我 们 经 常 使 用Hayes 兼 容 的AT 命 令 集 来 完 成 这 些 操 作， 由 于 各 调 制 解 调 器 厂 家 对 该 命 令 集 都 做 了 各 自 的
 扩 展， 因 而， 我 们 的DOS 应 用 程 序 一 般 只 能 操 作 一 小 部 分 调 制 解 调 器， 而 各 厂 家 都 提 供Window  驱 动 程 序， 所 以， 使 用TAPI 编 制 的 应 用 程 序 能 够 操 作 绝 大 多 数 调 制 解 调 器； 图 中 的 通 信API 是 应 用 程 序 发 送、 接 收 数 据 的 编 程 接 口。   
　　四、 程 序 流 程 结 构 框 图   
　　---- 由 于Win95 为 多 任 务 操 作 系 统， 我 们 的 流 程 图 只 能 代 表 本 应 用 程 序 的 执 行 先 后 关 系， 程 序 中 的 等 待 及 检 测 实 际 上 是 等 待 Win95 提 供 的 消 息， 所 以 并 不 占 用CPU 时 间， 在 下 面 的 程 序 中 可 以 看 出。 另 外， 数 据 交 换 的 协
 议 可 由 自 己 制 定， 也 可 使 用 已 有 的 协 议。   
　　五、 软 件 编 制   
　　---- 由 于Windows 编 程 的 框 架 基 本 相 同， 在 此 我 们 只 介 绍 涉 及 到 通 信 的 一 部 分 源 程 序：   
　　---- 1 头 文 件 中 应 该 包 括:   
　　---- #include < tapi.h >   
　　---- 请 注 意 工 程 文 件 的 属 性 应 该 是Windows 32 位 应 用 程 序   
　　---- 2 通 信 所 涉 及 到 的 一 些 全 局 变 量 定 义 及 类 型 定 义:   
　　char RecBuf[20]，buf[20]// 缓 冲 区   
　　DWORD  Error; // 错 误 码   
　　COMSTAT  Status; // 状 态 码   
　　DWORD  NumLine; // 允 许 使 用 的 线 路 设 备 数   
　　LINECALLPARAMS  para;// 呼 叫 参 数   
　　TmyDecFrame * pwin=NULL;// 主 窗 口 指 针   
　　HLINEAPP  myhTapi;// 线 路 应 用 程 序 句 柄   
　　HLINE  myhLine;// 线 路 句 柄   
　　HANDLE  myhModem;// 调 制 解 调 器 句 柄   
　　HCALL  myhCall;// 呼 叫 句 柄   
　　typedef  struct tagModemID{   
　　  HANDLE hModem;   
　　     char ModemName[1];   
　　}ModemID;   
　　---- 3 下 面 为 获 取 调 制 解 调 器 句 柄 的 函 数 定 义   
　　---- 因 为 每 个 调 制 解 调 器 的 标 志 字 符 串 长 度 不 一， 所 以 函 数 中 用 到 了 可 变 长 度 的 字 符 串， 处 理 方 法 是 先 为 字 符 串 指 针 分 配sizeof(VARSTRING) 大 小 的 空 间， 再 利 用 该 空 间 容 纳 调 用LineGetID 时Windows 返 回 的 信 息， 根
 据 返 回 信 息 判 断 所 需 空 间 大 小 重 新 分 配 空 间， 再 次 调 用LineGetID 就 可 以 取 得 完 整 的 标 志 字 符 串。   
　　void GethModem(HLINE hLine)   
　　{   ModemID far *mid;   
　　      VARSTRING * str;   
　　      LONG lid;   
　　     DWORD size;   
　　      char mark=1;   
　　      str=(VARSTRING *)malloc(sizeof(VARSTRING));   
　　      if(!str)   
　　return NULL;   
　　     str- >dwTotalSize=sizeof(VARSTRING);   
　　      do   
　　      { if((lineGetID(myhLine,0,NULL,LINECALLSELECT_LINE,str,   
　　"comm/datamodem")==0)&&(str- >dwTotalSize < str- > dwNeededSize))   
　　      { dwSize=str- >dwNeededSize;   
　　           free(str);   
　　             str=(VARSTRING *)malloc(dwSize);   
　　            if(!str)   
　　             { myhModem=NULL;   
　　     mark=2;   
　　}   
　　        str- >dwTotalSize=dwSize;   
　　         }   
　　   else mark=0;   
　　    }while(mark==1);   
　　   if(mark==0)   
　　   { mid=(ModemID far *)((LPSTR)str+str- >dwStringOffset);   
　　       myhModem=mid- >hModem;   
　　   }   
　　      free(str);   
　　}   
　　---- 4 在 主 窗 口 初 始 化 函 数 中 加 入 对 线 路 的 初 始 化 过 程:   
　　pwin=this;// 获 得 主 窗 口 指 针   
　　   while(lineInitialize(&myhTAPI,GetModule()- >GetInstance(),   
　　       (LINECALLBACK)MakeProcInstance((FARPROC) lpfnCallback,   
　　             GetModule()- >GetInstance()), "TRY",&NumLine )==LINEERR_REINIT)   
　　    { sleep(1);// 延 迟    };   
　　Error=lineOpen(hTAPI,0,&HLine,0x10004,0,0,LINECALLPRIVILEGE_MONITOR+   
　　LINECALLPRIVILEGE_OWNER,LINEMEDIAMODE_DATAMODEM,NULL);   
　　     if(Error!=0)   
　　  { sprintf(buf,"%lx",Error);   
　　MessageBox(buf,0,MB_OK); }   
　　  else   
　　  { GethModem(myhLine);// 取 得myhModem 的 值   
　　   if(myhModem!=NULL)   
　　   {    para.dwBearerMode=LINEBEARERMODE_VOICE;   
　　    para.dwMediaMode=LINEMEDIAMODE_DATAMODEM;   
　　     para.dwTotalSize=sizeof(LINECALLPARAMS);   
　　       Error=lineMakeCall(myhLine,&myhCall,"8880751",0,?);   
　　       If(Error!=0)   
　　    { sprintf(buf,"%lx",Error);   
　　        MessageBox(buf,0,MB_OK); }   
　　   }   
　　  }   
　　}   
　　---- 5 呼 叫 方 回 调 函 数 的 定 义   
　　void far pascal  TMyDecFrame::lpfnCallback   
　　           (DWORD hDevice, DWORD dwMsg,   
　　DWORD dwCallbackInstance,   
　　        DWORD dwParam1, DWORD dwParam2,   
　　DWORD dwParam3)//   
　　   参数定义同lineCallbackFunc函数中的参数定义   
　　{   int Rec_num=0;   
　　switch(dwParam1)   
　　{ case LINECALLSTATE_CONNECTED:   
　　         DWORD len;   
　　     ClearCommError(myhModem,&Error,&Status);   
　　          Rec_num=Status.cbInQue;   
　　      ReadFile(myhModem,RecBuf,Rec_num,&len,0);   
　　//至此已经为数据通信做好了前期准备,可设立标志   
　　        WriteFile(myhModem,"Success",7,&len,0);   
　　   ReadFile(myhModem,RecBuf,8,&len,0);   
　　   pwin->MessageBox(RecBuf,0,MB_OK);   
　　          break;   
　　  case LINECALLSTATE_IDLE:   
　　   lineDeallocateCall(myhCall);   
　　   break;   
　　case LINECALLSTATE_DISCONNECTED:   
　　lineClose(myhLine);   
　　lineShutDown(myhTapi);   
　　break;   
　　   }   
　　}   
　　---- 6 被 叫 方 回 调 函 数 的 定 义   
　　void far pascal  TMyDecFrame::lpfnCallback(DWORD hDevice, DWORD dwMsg,   
　　      DWORD dwCallbackInstance, DWORD dwParam1, DWORD dwParam2,   
　　      DWORD dwParam3)   
　　{  int Rec_num=0;   
　　switch(dwParam3)   
　　{ case LINECALLPRIVILEGE_OWNER:   
　　   myhCall=(HCALL)hDevice;   
　　   Break;   
　　}//只有对呼叫具有私有特权的调用者才能应答呼叫,   
　　      在此获得呼叫句柄   
　　switch(dwParam1)   
　　{ case LINECALLSTATE_CONNECTED:   
　　          DWORD len;   
　　      ClearCommError(myhModem,&Error,&Status);   
　　          Rec_num=ComS.cbInQue;   
　　     ReadFile(myhModem,RecBuf,Rec_num,&len,0);// 清 除 接 收 缓 冲 区   
　　     ReadFile(myhModem,RecBuf,7,&len,0);   
　　      WriteFile(myhModem,"Received",8,&len,0);   
　　   pwin- >MessageBox(RecBuf,0,MB_OK);   
　　           break;   
　　  case LINECALLSTE_OFFERING:   
　　   lineAnswer(myhCall,NULL,0);   
　　   break;// 完 成 自 动 应 答   
　　  case LINECALLSTATE_IDLE:   
　　   lineDeallocateCall(myhCall);   
　　   break;   
　　case LINECALLSTATE_DISCONNECTED:   
　　lineClose(myhLine);   
　　lineShutDown(myhTapi);   
　　break;   
　　     }   
　　}   
　　六、 改 进 措 施   
　　---- 以 上 程 序 中 使 用 的 是 同 步 读 写 方 式， 只 要WriteFile 或 者Read File 没 有 完 成 指 定 的I/O 任 务， 它 们 就 不 会 返 回 进 程， 在 许 多 情 况 下， 这 是 令 人 难 以 容 忍 的CPU 时 间 浪 费； 改 进 的 办 法 是 在 每 次 读 之 前 采 用ClearCommError
 函 数 确 定 系 统 的 串 行 口 缓 冲 区 中 到 底 有 了 多 少 字 节 的 接 收 数 据， 而 写 方 式 采 用 异 步 方 式， 首 先 应 该 定 义 一 个OVERLAPPED 结 构， 从BC++5.0 中 获 得 的 结 构 定 义 如 下   
　　：   
　　typedef struct _OVERLAPPED { // o   
　　    DWORD  Internal;   
　　    DWORD  InternalHigh;   
　　    DWORD  Offset;   
　　    DWORD  OffsetHigh;   
　　    HANDLE hEvent;   
　　} OVERLAPPED；   
　　---- 我 们 定 义OVERLAPPED myOVLP;   
　　---- 我 们 只 用 到 了 其 中 的hEvent 成 员， 其 他 成 员 均 置0；hEvent 设 置 为CreateEvent（NULL，TRUE，FALSE，NULL） 产 生 的 事 件 句 柄； 然 后 如 下 调 用WriteFile(myhModem,"Received",8,&len,&myOVLP);   
　　---- 函 数 将 立 即 返 回， 此 后， 只 要GetOverlappedResult 函 数 返 回TRUE ， 写 操 作 就 算 完 成 了(王朝网络 wangchao.net.cn)
