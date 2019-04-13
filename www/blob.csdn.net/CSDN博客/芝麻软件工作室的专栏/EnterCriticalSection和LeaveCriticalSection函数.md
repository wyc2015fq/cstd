
# EnterCriticalSection和LeaveCriticalSection函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:57:53[seven-soft](https://me.csdn.net/softn)阅读数：215标签：[多线程																](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



多个线程操作相同的数据时，一般是需要按顺序访问的，否则会引导数据错乱，无法控制数据，变成随机变量。为解决这个问题，就需要引入互斥变量，让每个线程都按顺序地访问变量。这样就需要使用EnterCriticalSection和LeaveCriticalSection函数。
函数EnterCriticalSection和LeaveCriticalSection声明如下：
WINBASEAPI
VOID
WINAPI
EnterCriticalSection(
__inout LPCRITICAL_SECTION lpCriticalSection
);
WINBASEAPI
VOID
WINAPI
LeaveCriticalSection(
__inout LPCRITICAL_SECTION lpCriticalSection
);
lpCriticalSection是创建临界区对象。
调用函数的例子如下：
\#001 CCaiWinMsg::CCaiWinMsg(void)
\#002 {
\#003   m_hBtn = NULL;
\#004   m_nCount = 0;
\#005
\#006   m_pThreadA = NULL;
\#007   m_pThreadB = NULL;
\#008
\#009  //
\#010  InitializeCriticalSection(&m_csCount);
\#011
\#012 }
\#013
\#014 CCaiWinMsg::~CCaiWinMsg(void)
\#015 {
\#016  DeleteCriticalSection(&m_csCount);
\#017 }
\#018
第10行是创建临界区对象。
第16行是删除临界区对象。
\#001 //
\#002 //窗口的消息处理类。
\#003 //
\#004 //
\#005 class CCaiWinMsg :
\#006  public CCaiWin
\#007 {
\#008 public:
\#009  CCaiWinMsg(void);
\#010  virtual ~CCaiWinMsg(void);
\#011
\#012  //线程操作函数。
\#013  int AddCount(void)
\#014  {
\#015         //
\#016         EnterCriticalSection(&m_csCount);
\#017         int nRet = m_nCount++;
\#018         LeaveCriticalSection(&m_csCount);
\#019
\#020         return nRet;
\#021  }
在函数AddCount里调用EnterCriticalSection和LeaveCriticalSection来互斥访问变量m_nCount。通过上面这种方法，就可以实现多线程按顺序地访问相同的变量。


