#ifndef _FILEMONITOR_INL_
#define _FILEMONITOR_INL_
// 名称: filemonitor_t
// 功能: 监测指定目录中的文件/目录的改变
// 环境要求: Requires Windows NT 3.51 SP3 or later, and Unsupported Windows 95/98.
enum { FM_ADDED = 1, FM_REMOVED = 2, FM_MODIFIED = 3, FM_RENAMED = 4 };
typedef void (__stdcall* LPDEALFUNCTION)(int act, char* filename1, char* filename2);
typedef struct filemonitor_t {
  LPDEALFUNCTION DealFun;
  HANDLE hThread;
  HANDLE hDir;
} filemonitor_t;
static DWORD WINAPI Routine(LPVOID lParam)
{
  filemonitor_t* obj = (filemonitor_t*)lParam;
  char buf[ 2 * (sizeof(FILE_NOTIFY_INFORMATION) + MAX_PATH) ];
  FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)buf;
  DWORD BytesReturned;
  while (TRUE) {
    DWORD flag = (FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY);
    if (ReadDirectoryChangesW(obj->hDir, pNotify, sizeof(buf), TRUE, flag, &BytesReturned, NULL, NULL)) {
      char tmp[MAX_PATH] = {0}, str1[MAX_PATH] = {0}, str2[MAX_PATH] = {0};
      memset(tmp, 0, sizeof(tmp));
      WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, tmp, 99, NULL, NULL);
      strcpy(str1, tmp);
      if (0 != pNotify->NextEntryOffset) {
        PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pNotify + pNotify->NextEntryOffset);
        memset(tmp, 0, sizeof(tmp));
        WideCharToMultiByte(CP_ACP, 0, p->FileName, p->FileNameLength / 2, tmp, 99, NULL, NULL);
        strcpy(str2, tmp);
      }
      obj->DealFun(pNotify->Action, str1, str2);
    }
    else {
      break;
    }
  }
  return 0;
}
int filemonitor_open(filemonitor_t* fs, const char* WatchedDir, LPDEALFUNCTION dealfun)
{
  DWORD ThreadId;
  fs->DealFun = dealfun;
  fs->hDir = CreateFile(WatchedDir, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (INVALID_HANDLE_VALUE == fs->hDir) {
    return FALSE;
  }
  fs->hThread = CreateThread(NULL, 0, Routine, fs, 0, &ThreadId);
  return NULL != fs->hThread;
}
int filemonitor_close(filemonitor_t* fs)
{
  if (NULL != fs->hThread) {
    TerminateThread(fs->hThread, 0);
    fs->hThread = NULL;
  }
  if (0 != fs->hDir) {
    CloseHandle(fs->hDir);
    fs->hDir = 0;
  }
  return 0;
}
//void __stdcall MyDeal( ACTION act, std::string filename1, std::string filename2 )
void __stdcall MyDeal(int act, char* filename1, char* filename2)
{
  //char szCmd[256];
  switch (act) {
  case FM_ADDED:
    printf("添加文件 - %s\n", filename1);
    break;
  case FM_REMOVED:
    printf("删除文件 - %s\n", filename1);
    break;
  case FM_MODIFIED:
    printf("修改文件 - %s\n", filename1);
    break;
  case FM_RENAMED:
    printf("改名文件 - %s %s\n", filename1, " ", filename2);
    break;
  }
}
#include <stdio.h>
#include <stdlib.h>
int test_filemonitor()
{
  filemonitor_t a[1] = {0};
  const char* m_szPath;
  m_szPath = "D:\\code\\cstd\\src\\net";
  m_szPath = "c:\\11\\";
  m_szPath = "D:\\11\\";
  printf("正在监控的目录为： %s\n", m_szPath);
  printf("按 <q> 退出程序。\n");
  filemonitor_open(a, m_szPath, MyDeal);
  while (getchar() != 'q') {}
  filemonitor_close(a);
  return 0;
}
#include <Shlobj.h>
//#pragma comment(lib,"Shell32.lib")
#define SHCNRF_InterruptLevel 0x0001 //Interrupt level notifications from the file system
#define SHCNRF_ShellLevel 0x0002 //Shell-level notifications from the shell
#define SHCNRF_RecursiveInterrupt 0x1000 //Interrupt events on the whole subtree
#define SHCNRF_NewDelivery 0x8000 //Messages received use shared memory
//自定义消息
#define WM_USERDEF_FILECHANGED (WM_USER + 123)
typedef ULONG(WINAPI* pfnSHChangeNotifyRegister)(HWND hWnd, int fSource, LONG fEvents, UINT wMsg, int cEntries, SHChangeNotifyEntry* pfsne);
typedef int (*notify_fun_t)(int evt, const char* f1, const char* f2);
typedef struct shchange_t {
  ULONG id;
  notify_fun_t notify_fun;
} shchange_t;
static shchange_t g_shchange = {0};
long __stdcall SHChangeNotifyWindowProc(HWND hwnd, unsigned int uMsg, LPCITEMIDLIST* wParam, long lParam)
{
  static char buf[2048];
  char* buf1 = buf;
  const char* str = "";
  if (uMsg == WM_USERDEF_FILECHANGED) {
    buf1 = buf;
    buf[0] = 0;
    if (SHGetPathFromIDList(*wParam, buf)) {
      buf1 += strlen(buf1) + 1;
      if (SHGetPathFromIDList(wParam[1], buf1)) {
      }
      else {
      }
    }
    if (!g_shchange.notify_fun) {
      switch (lParam) {
#define SHCHANGENOTIFYDEF(a, b) case a: str = b; break;
        SHCHANGENOTIFYDEF(SHCNE_RENAMEITEM, "重命名文件 %s 为 %s")
        SHCHANGENOTIFYDEF(SHCNE_CREATE, "建立文件 %s")
        SHCHANGENOTIFYDEF(SHCNE_DELETE, "删除文件 %s")
        SHCHANGENOTIFYDEF(SHCNE_MKDIR, "新建目录 %s")
        SHCHANGENOTIFYDEF(SHCNE_RMDIR, "删除目录 %s")
        SHCHANGENOTIFYDEF(SHCNE_MEDIAINSERTED, "%s 中插入可移动存储介质%s")
        SHCHANGENOTIFYDEF(SHCNE_MEDIAREMOVED, "%s 中移去可移动存储介质 %s")
        SHCHANGENOTIFYDEF(SHCNE_DRIVEREMOVED, "移去驱动器 %s")
        SHCHANGENOTIFYDEF(SHCNE_DRIVEADD, "添加驱动器 %s")
        SHCHANGENOTIFYDEF(SHCNE_NETSHARE, "改变目录 %s 的共享属性")
        SHCHANGENOTIFYDEF(SHCNE_ATTRIBUTES, "改变文件目录属性 文件名 %s")
        SHCHANGENOTIFYDEF(SHCNE_UPDATEDIR, "更新目录 %s")
        SHCHANGENOTIFYDEF(SHCNE_UPDATEITEM, "更新文件 文件名： %s")
        SHCHANGENOTIFYDEF(SHCNE_SERVERDISCONNECT, "断开与服务器的连接 %s %s")
        SHCHANGENOTIFYDEF(SHCNE_UPDATEIMAGE, "改变系统全局图像列表中的一个图像")
        SHCHANGENOTIFYDEF(SHCNE_DRIVEADDGUI, "通过GUI增加网络驱动器")
        SHCHANGENOTIFYDEF(SHCNE_RENAMEFOLDER, "重命名文件夹 %s 为 %s")
        SHCHANGENOTIFYDEF(SHCNE_FREESPACE, "磁盘空间大小改变")
        SHCHANGENOTIFYDEF(SHCNE_ASSOCCHANGED, "改变文件关联")
#undef SHCHANGENOTIFYDEF
      }
      printf(str, buf, buf1);
      printf("\n");
    }
    if (g_shchange.notify_fun) {
      g_shchange.notify_fun(lParam, buf, buf1);
    }
    return 0;
  }
  else if (uMsg == WM_CREATE) {
    //hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    return 0;
  }
  else {
    return DefWindowProc(hwnd, uMsg, (WPARAM)wParam, lParam);
  }
}
int shchange_begin(notify_fun_t notify_fun)
{
  HWND hwnd;
  static SHChangeNotifyEntry shcne = {0, 1};
  static WNDCLASSEXW wc = {sizeof(WNDCLASSEXW), 0, (WNDPROC)SHChangeNotifyWindowProc, 0, 0, 0, 0, 0, 0, 0, L"a", 0};
  if (0 == g_shchange.id) {
    g_shchange.notify_fun = notify_fun;
    wc.hInstance = GetModuleHandleW(0);
    hwnd = CreateWindowExW(0, (wchar_t*)RegisterClassExW(&wc), 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, 0, 0);
    SHGetFolderLocation(0, CSIDL_DESKTOP, 0, 0, (LPITEMIDLIST*)(&shcne.pidl));
    g_shchange.id = SHChangeNotifyRegister(hwnd, SHCNRF_InterruptLevel | SHCNRF_ShellLevel | SHCNRF_RecursiveInterrupt, SHCNE_ALLEVENTS, WM_USERDEF_FILECHANGED, 1, &shcne);
  }
  return 0;
}
int shchange_end()
{
  SHChangeNotifyDeregister(g_shchange.id);
  g_shchange.id = 0;
  return 0;
}
int test_filemonitor1()
{
  MSG msg;
  shchange_begin(0);
  while (GetMessage(&msg, 0, 0, 0)) {
    DispatchMessage(&msg);
  }
  shchange_end();
  return 0;
}
//#pragma comment(lib,"Kernel32.lib")
#if 0
BOOL Initialize()
{
  //加载Shell32.dll
  m_hShell32 = LoadLibrary("Shell32.dll");
  if (m_hShell32 == NULL) {
    return FALSE;
  }
  //取函数地址
  m_pfnDeregister = NULL;
  m_pfnRegister = NULL;
  m_pfnRegister = (pfnSHChangeNotifyRegister)GetProcAddress(m_hShell32, MAKEINTRESOURCE(2));
  m_pfnDeregister = (pfnSHChangeNotifyDeregister)GetProcAddress(m_hShell32, MAKEINTRESOURCE(4));
  if (m_pfnRegister == NULL || m_pfnDeregister == NULL) {
    return FALSE;
  }
  SHChangeNotifyEntry shEntry = {0};
  shEntry.fRecursive = TRUE;
  shEntry.pidl = 0;
  m_ulNotifyId = 0;
  //注册Shell监视函数
  m_ulNotifyId = m_pfnRegister(GetSafeHwnd(), SHCNRF_InterruptLevel | SHCNRF_ShellLevel, SHCNE_ALLEVENTS, WM_USERDEF_FILECHANGED, 1, &shEntry);
  if (m_ulNotifyId == 0) {
    MessageBox("Register failed!", "ERROR", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  return TRUE;
}
#endif
#endif // _FILEMONITOR_INL_

