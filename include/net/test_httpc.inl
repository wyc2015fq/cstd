
#include "httpc.inl"

int http_parse_url(const char* url, char* host, int host_maxlen, char* path, int path_maxlen, char* file, int file_maxlen)
{
  const char* strFind = ("://");
  char* p = strstr(url, strFind);
  if (p) {
    int l;
    char* p1, *p2;
    char* end = p + strlen(p);
    p += 3;
    p1 = strchr(p, '/');
    if (!p1) {
      p1 = end;
    }
    l = p1-p;
    if (l<host_maxlen) {
      strncpy(host, p, l);
    }
    l = end - p1;
    if (l<path_maxlen) {
      strncpy(path, p1, l);
    }
    p = p1;
    p2 = strrchr(p, '/');
    if (p2) {
      ++p2;
      l = end - p2;
      if (l<file_maxlen) {
        strncpy(file, p2, l);
      }
    }
  }
  return 0;
}

//int requests_post(const char* url, struct data_t* data, ) {}
int requests_get(const char* url, const char** data) {
  char host[256]={0};
  char path[256]={0};
  char file[256]={0};
  http_parse_url(url, host, countof(host), path, countof(path), file, countof(file));
  int port = 80, sock=0;
  addr_in addr[1] = {0};
  sockbuf_t client[1] = {0};
  client->sock = sock = sock_connect_host(host, port, addr);
  if (!sock) {
    TRACE("连接http服务器失败！\n");
    return 0;
  }
  char buf[HTTP_MAX_LEN];
  _snprintf(buf, HTTP_MAX_LEN, "GET %s HTTP/1.0\r\nHost: %s\r\n", url, host);
  sock_sendstr(sock, buf);
  sock_sendstr(sock, "Accept: */*\r\n");
  sock_sendstr(sock, "User-Agent: Mozilla/4.04[en](Win95;I;Nav)\r\n\r\n");
  sock_sendstr(sock, "Connection: Keep-Alive\r\n\r\n");

// HTTP/1.1 200 OK
// Date: Sat, 24 Feb 2018 07:44:04 GMT
// Server: nginx
// Content-Type: text/html;charset=UTF-8
// P3P: CP=CAO PSA OUR
// Last-Modified: Sat, 24 Feb 2018 07:40:04 GMT
// Content-Language: en-US
// cdnhost: 134
// Age: 617
// X-Via: 1.1 jdxxz40:10 (Cdn Cache Server V2.0), 1.1 PSshqzdx4be178:7 (Cdn Cache S
// erver V2.0)
// Connection: close
  
  int count = 0;
  int ver1 = 0;
  int ver2 = 0;
  int retcode=0;
  int contentLength = 0;
  int Content_Type = 0;
  int is_close_ed = 0;
  int is_head_end = 0;
  
  char* content = NULL;
  MYREALLOC(content, 256);
  *content = 0;

  while (sockbuf_readLine(client, buf, HTTP_MAX_LEN) > 0) {
    ++count;
    char* cp = buf;
    if (is_head_end) {
      contentLength += strlen(buf)+10;
      MYREALLOC(content, contentLength);
      strcat(content, buf);
      strcat(content, "\r\n");
      continue;
    }
    if (0==strlen(buf)) {
      is_head_end=1;
    }
    if (strlen(buf)<100) {
      printf("%s\n", buf);
    }
    if (1==count) {
      // HTTP/1.1 200 OK;
      if (STRNCASECMP2(cp, "HTTP/") == 0) {
        sscanf(cp+5, "%d.%d %d", &ver1, &ver2, &retcode);
      }
      if (200!=retcode) {
        break;
      }
    } else {
      if (STRNCASECMP2(cp, "Connection:") == 0) {
        is_close_ed = !!strstr(cp, "close");
      }
      if (STRNCASECMP2(cp, "Date:") == 0) {
        int sadf=0;
      }
      if (STRNCASECMP2(cp, "Server:") == 0) {
        int sadf=0;
      }
      if (STRNCASECMP2(cp, "Content-Type:") == 0) {
        int sadf=0;
      }
      if (STRNCASECMP2(cp, "Content-Length:") == 0) {
        contentLength = 0;
      }
    }
  }
  sock_close(sock);
  FREE(content);
  return 0;
}

int test_httpc() {
  //HTTPDownloadFileMe();
  char* url = "http://ishare.iask.sina.com.cn/u/53b4ffbe11e0ee29f3315398.html";
  requests_get(url, NULL);
  //r = requests_post('https://api.github.com/some/endpoint', data=json.dumps({'some': 'data'}))
  //print(r.json())
  return 0;
}
