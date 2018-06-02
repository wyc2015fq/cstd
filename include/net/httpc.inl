
#include "http.h"
//#include <sys/stat.h> // 状态显示头文件
//#include "socket.h"
//#include "file.inl"
//typedef unsigned long DWORD;
#ifndef _1K
#define _1K (1<<10)
#define _1M (1<<20)
#define _1G (1<<30)
#endif

static int http_get_header(int sock, char* str, int maxstrlen)
{
  BOOL bResponsed = FALSE;
  int nResponseHeaderSize;
  if (!bResponsed) {
    int nIndex = 0;
    BOOL bEndResponse = FALSE;
    while (!bEndResponse && nIndex < maxstrlen - 1) {
      int ret = sock_recv(sock, str + nIndex++, 1, 0);
      if (0 == str[nIndex - 1]) {
        break;
      }
      if (nIndex >= 4) {
        if (str[nIndex - 4] == '\r' &&
            str[nIndex - 3] == '\n' &&
            str[nIndex - 2] == '\r' &&
            str[nIndex - 1] == '\n') {
          bEndResponse = TRUE;
        }
      }
    }
    str[nIndex] = 0;
    nResponseHeaderSize = nIndex;
    bResponsed = TRUE;
  }
  return nResponseHeaderSize;
}

static int sock_connect_host(const char* ip, int port, addr_in* addr) {
  int sock = sock_open(NULL, 0, SOCK_TCP, addr);
  if (sock>=0) {
    addr_set(addr, ip, port);
    if (0==sock_connect(sock, addr)) {
      return sock;
    }
    sock_close(sock);
  }
  return 0;
}
static int sock_sendstr(int s, const char* buf) {
  int len = strlen(buf);
  return sock_send(s, buf, len, 0);
}
int http_connect_proxy(const char* proxyaddr, int nPort)
{
  char sTemp[1024];
  char cTmpBuffer[1024];
  int nLen;
  int sock = 0;
  addr_in addr[1];
  TRACE("正在建立连接\n");
  if (!(sock = sock_connect_host(proxyaddr, nPort, addr))) {
    TRACE("连接http服务器失败！\n");
    return 0;
  }
  // 发送CONNCET请求令到代理服务器，用于和代理建立连接代理服务器的
  // 地址和端口放在proxyaddr,nPort 里面.
  _snprintf(sTemp, 1024, "CONNECT %s:%d HTTP/1.1\r\nUser-Agent:MyApp/0.1\r\n\r\n", proxyaddr, nPort);
  if (!sock_sendstr(sock, sTemp)) {
    TRACE("连接代理失败\n");
    return 0;
  }
  // 取得代理响应，如果连接代理成功，代理服务器将
  // 返回"200 Connection established".
  nLen = http_get_header(sock, cTmpBuffer, sizeof(cTmpBuffer));
  strcpy(sTemp, cTmpBuffer);
  if (strstr(sTemp, "HTTP/1.0 200 Connection established") == NULL) {
    TRACE("连接代理失败\n");
    sock_close(sock);
    return 0;
  }
  TRACE(sTemp);
  TRACE("代理连接完成\n");
  return sock;
}
int http_connect_nonproxy(const char* host, int port)
{
  addr_in addr[1];
  TRACE("正在建立连接\n");
  return sock_connect_host(host, port, addr);
}
int http_parse_url(const char* url, str_t* host, str_t* path, str_t* filename)
{
  const char* strFind = ("http://");
  char* p = strstr(url, strFind);
  if (p) {
    char* p1;
    char* p2;
    p += 7;
    p1 = strchr(p + 7, '/');
    *host = STR2(p, p1 - p);
    p2 = strrchr(p, '/');
    ++p2;
    *path = STR2(url, p2 - url);
    *filename = STR2(p2, -1);
  }
  return 0;
}
int http_send_header(int sock, const char* method, const char* url, const char* host, const char* referer, int connection, int agent, int accept, int start)
{
  char buf[256];
  if (!method) {
    return 0;
  }
  sock_sendstr(sock, method);
  sock_sendstr(sock, " ");
  sock_sendstr(sock, url);
  sock_sendstr(sock, " HTTP/1.1\r\nHost: ");
  sock_sendstr(sock, host);

  if (connection) {
    sock_sendstr(sock, "Connection: Keep-Alive\r\n");
  }
  if (referer) {
    _snprintf(buf, 256, "Referer: %s\r\n", referer);
    sock_sendstr(sock, buf);
  }
  if (agent) {
    sock_sendstr(sock, "User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)\r\n");
  }
  if (accept) {
    sock_sendstr(sock, "Accept: */*\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n");
  }
  if (start >= 0) {
    _snprintf(buf, 256, "range: bytes=%d-\r\n\r\n", start);
    sock_sendstr(sock, buf);
  }
  return 0;
}
#if 0
int http_parse_header(const char* buf, http_file_info_t* fi)
{
  // 得到待下载文件的大小
  const char* strFind = "Content-Length:";
  int local;
  fi->content_length = -1;
  local = cstr_findstr(buf, strFind, 0);
  if (local > 0) {
    local += strlen(strFind);
    fi->content_length = atoi(buf + local);
  }
  return 0;
}
typedef int (*process_fun_t)(const char* str, void* user);
// nStart; // 分割的起始位置
// nEnd; // 分割的起始位置
// bProxyMode; // 下载模态
//const char* filepath; // 下载后的文件名.
//int proxyport; // 代理服务端口号.
//int nHostPort; // Host端口号
BOOL httpget(const char* url, int nHostPort, const char* dir, const char* filepath, const char* proxyaddr, int proxyport, int nStart, int nEnd, process_fun_t fun, void* user)
{
  sock_t s[2] = {0};
  FILE* fpwrite;
  int nSumLen;
  char buf[10 * _1K] = {0};
  str_t str[3] = {0};
  str_t* host = str + 0; // Host地址
  str_t* path = str + 1;// Http文件地址
  str_t* filename = str + 2;// Http文件名
  int bProxyMode = proxyaddr != NULL;
  http_file_info_t fi[1] = {0};
  int i;
  int buflen = sizeof(buf);
  int filesize = 0;
  // 例如: host="www.aitenshi.com",
  // szHttpAddr="http://www.aitenshi.com/bbs/images/",
  // szHttpFilename="pics.jpg".
  http_parse_url(url, host, path, filename);
  filepath = strchr(url, '/');
  mkpath(buf, buflen, STR1(dir), STR1(0), STR1(filepath));
  fpwrite = GetFilePointer(buf);
  filesize = fsize(fpwrite);
  if (1) {
    int ret;
    if (bProxyMode) {
      ret = http_connect_proxy(s, proxyaddr, proxyport);
    }
    else {
      str_nprintf(buf, buflen, "%#s", host->l, host->s);
      ret = http_connect_nonproxy(s, buf, nHostPort);
    }
    if (!ret) {
      return 1;
    }
  }
  if (!fpwrite) {
    return 1;
  }
  if (1) {
    // 请求文件，设置下载范围
    //http_make_header(s, buf, buflen, "GET", path, filename, host, path, 1, 1, 1, 0);
    // 取得http头
    memset(buf, 0, buflen);
    i = http_get_header(s, buf, buflen);
    if (!i) {
      TRACE("获取HTTP头出错!\n");
      return 0;
    }
    http_parse_header(buf, fi);
  }
  nStart = MAX(filesize, nStart);
  // 设置文件写指针起始位置，断点续传
  fseek(fpwrite, nStart, SEEK_SET);
  nSumLen = 0;
  // 进行下载
  if (fi->content_length <= 0 || (nStart < fi->content_length)) {
    utime_start(_start_time);
    for (; 1;) {
      int nLen;
      if (fi->content_length > 0 && nSumLen >= fi->content_length) {
        break;
      }
      nLen = sock_recv(s, buf, buflen, 0);
      if (nLen == SOCKET_ERROR) {
        TRACE("Read error!\n");
        fclose(fpwrite);
        return 1;
      }
      if (nLen == 0) {
        break;
      }
      nSumLen += nLen;
      if (fun) {
        char str[32];
        double time = utime_elapsed(_start_time);
        double speed = nSumLen / time;
        char bkm = speed > _1M ? 'M' : speed > _1K ? 'K' : 'B';
        double spd = speed > _1M ? speed / _1M : speed > _1K ? speed / _1K : speed;
        _snprintf(str, sizeof(str), "%d/%d/%5.1f%%/%5.1f%c", nSumLen, fi->content_length, nSumLen * 100. / fi->content_length, spd, bkm);
        fun(str, user);
      }
      // 把数据写入文件
      fwrite(buf, nLen, 1, fpwrite);
    }
  }
  if (fun) {
    fun(0, user);
  }
  fclose(fpwrite); // 关闭写文件
  sock_close(s); // 关闭套接字
  return 0;
}
#include "filemonitor.inl"
#include "str.h"
#include "ui/window.inl"
#include "thread.h"
#include "inifile.h"
typedef struct httpgetui_t {
  vstr_t sv[1];
  critical_section_t cs[1];
  int* stat;
  char* winname;
  char* dir;
} httpgetui_t;
int sh_notify(int evt, const char* f1, const char* f2)
{
  return 0;
}
int findtask(HWND hList, char* buf, int len)
{
  int i, n = ListView_GetItemCount(hList);
  for (i = 0; i < n; ++i) {
    ListView_GetItemText(hList, i, 1, buf, len);
    if (0 >= strlen(buf)) {
      _snprintf(buf, len, "0/0/0");
      ListView_SetItemText(hList, i, 1, buf);
      ListView_GetItemText(hList, i, 0, buf, len);
      return i;
    }
  }
  return -1;
}
typedef struct httpgetui_process_info_t {
  char url[256];
  HWND hList;
} httpgetui_process_info_t;
int ListView_FindItemByText(HWND hList, int col, const char* text)
{
  int i, n = ListView_GetItemCount(hList);
  char buf[256];
  for (i = 0; i < n; ++i) {
    ListView_GetItemText(hList, i, col, buf, 256);
    if (0 == stricmp(buf, text)) {
      return i;
    }
  }
  return -1;
}
int ListView_DeleteByText(HWND hList, int col, const char* text)
{
  int i, ndeled = 0, n = ListView_GetItemCount(hList);
  char buf[256];
  for (i = n; i-- > 0;) {
    ListView_GetItemText(hList, i, col, buf, 256);
    if (0 == stricmp(buf, text)) {
      ListView_DeleteItem(hList, i);
      ndeled++;
    }
  }
  return ndeled;
}
int httpgetui_process(const char* str, void* user)
{
  httpgetui_process_info_t* pi = (httpgetui_process_info_t*)user;
  if (NULL == str) {
    ListView_DeleteByText(pi->hList, 0, pi->url);
  }
  else {
    int i = ListView_FindItemByText(pi->hList, 0, pi->url);
    if (i >= 0) {
      ListView_SetItemText(pi->hList, i, 1, (char*)str);
    }
  }
  return 0;
}
int httpget_th(httpgetui_t* h)
{
  CvWindow* window = cvGetWindowByName(h->winname);
  HWND hList = window->hList;
  httpgetui_process_info_t pi[1] = {0};
  int i;
  pi->hList = hList;
  while (1) {
    critical_section_enter(h->cs);
    i = findtask(hList, pi->url, 256);
    critical_section_leave(h->cs);
    if (i >= 0) {
      httpget(pi->url, 80, h->dir, 0, 0, 0, 0, -1, httpgetui_process, pi);
    }
  }
  return 0;
}
BOOL httpgetui()
{
  int ch = 0;
  const char* list_column_lable[] = {"文件名", "进度"};
  //double mat[10*10] = {0};
  httpgetui_t h[1] = {0};
  thread_t th[5] = {0};
  h->winname = "httpgetui";
  h->dir = ".";
  //char outputpath[256];
  //_chdir("D:/pub/bin/net");
  //IniGetString("httpget.ini", "all", "outputpath", "", outputpath, 256);
  _chdir("D:/www/pic/www.fengdu100.com");
  //mkdirs(outputpath, -1);
  vstr_load("jpg.txt", h->sv);
  cvSetListLable(h->winname, countof(list_column_lable), list_column_lable, 50);
  shchange_begin(sh_notify);
  cvSetList_strv(h->winname, 0, h->sv);
  critical_section_init(h->cs);
  thread_create_all(th, countof(th), httpget_th, h);
  //cvShowMat(winname, "%f", 10, 10, mat, 10, sizeof(double), -1);
  for (; 'q' != ch && 'Q' != ch;) {
    ch = cvWaitKey(50);
  }
  shchange_end();
  strv_free(h->sv);
  critical_section_destroy(h->cs);
  return 0;
}


#endif

