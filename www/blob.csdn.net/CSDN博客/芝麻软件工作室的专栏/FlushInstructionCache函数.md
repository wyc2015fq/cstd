
# FlushInstructionCache函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:26:53[seven-soft](https://me.csdn.net/softn)阅读数：251


一般的程序都是在运行前已经编译好的，因此修改指令的机会比较少，但在软件的防破解里，倒是使用很多。当修改指令之后，怎么样才能让CPU去执行新的指令呢？这样就需要使用函数FlushInstructionCache来把缓存里的数据重写回主内存里去，让CPU重新加载新的指令，才能执行新的指令。下面就来学习一下使用这个函数来实现跳到一个静态函数里执行，而不是直接地调用这个函数。
函数FlushInstructionCache声明如下：
WINBASEAPI
BOOL
WINAPI
FlushInstructionCache(
__in HANDLE hProcess,
__in_bcount_opt(dwSize) LPCVOID lpBaseAddress,
__in SIZE_T dwSize
);
**hProcess**是进程句柄。
**lpBaseAddress**是要同步内存的开始地址。
**dwSize**是要同步内存的大小。
调用函数的例子如下：
\#001 //声明函数类型。
\#002 typedef void (*TESTFUN)(void);
\#003
\#004 //定义修改代码的结构。
\#005 \#pragma pack(push,1)
\#006 struct ThunkCode
\#007 {
\#008  BYTE    m_jmp;      // jmp TESTFUN，跳转指令。
\#009  DWORD   m_relproc;   // relative jmp，相对跳转的位置。
\#010 };
\#011 \#pragma pack(pop)
\#012
\#013 //测试动态修改内存里的指令数据。
\#014 //
\#015 class CFlush
\#016 {
\#017 public:
\#018  //保存动态修改代码的内存。
\#019  ThunkCode m_Thunk;
\#020
\#021  //初始化跳转代码。
\#022  void Init(TESTFUN pFun, void* pThis)
\#023  {
\#024         //设置跳转指针。
\#025          m_Thunk.m_jmp = 0xe9;
\#026
\#027         //设置跳转的相对地址。
\#028          m_Thunk.m_relproc = (int)pFun - ((int)this+sizeof(m_Thunk));
\#029
\#030         //把CPU里的缓冲数据写到主内存。
**\#031        FlushInstructionCache(GetCurrentProcess(),**
**\#032              &m_Thunk, sizeof(m_Thunk));**
\#033  }
\#034
\#035  //真实运行的函数。
\#036  static void TestFun(void)
\#037  {
\#038         OutputDebugString(_T("CFlush动态修改代码运行/r/n"));
\#039  }
\#040
\#041 };
\#042
如下调用这个类：
\#001  //测试运行。
\#002  CFlush flushTest;
\#003
\#004  flushTest.Init(flushTest.TestFun,&flushTest);
\#005  TESTFUN pTestFun = (TESTFUN)&(flushTest.m_Thunk);
\#006  pTestFun();


