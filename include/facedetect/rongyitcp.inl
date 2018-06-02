
#ifndef _RONGYITCP_INL_
#define _RONGYITCP_INL_
// 大端
int set32_b(void* buf, int& i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x>>24)&0xff;
  p[i++] = (x>>16)&0xff;
  p[i++] = (x>>8)&0xff;
  p[i++] = (x)&0xff;
  return i;
}
int set16_b(void* buf, int i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x>>8)&0xff;
  p[i++] = (x)&0xff;
  return i;
}
int set8_b(void* buf, int i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x)&0xff;
  return i;
}
int set8_n(void* buf, int i, uint x, uint n) {
  unsigned char* p = (unsigned char*)buf;
  memset(p+i, x, n);
  return i+n;
}
int setp_n(void* buf, int i, const void* x, uint n) {
  unsigned char* p = (unsigned char*)buf;
  memcpy(p+i, x, n);
  return i+n;
}

int get32_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = (p[i]<<24)|
    (p[i+1]<<16)|
    (p[i+2]<<8)|
    (p[i+3]);
  return i+4;
}
int get16_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = (p[i]<<8)|
    (p[i+1]);
  return i+2;
}
int get8_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = p[i++];
  return i;
}
int getp_n(const void* buf, int i, void* x, uint n) {
  const unsigned char* p = (const unsigned char*)buf;
  memcpy(x, p+i, n);
  return i+n;
}
typedef struct ID {
  char data[16];
}ID;

int set_pack_data(void* buf, int i, int buflen, uint head, uint type, ID sguid, ID dguid, uint smod, uint dmod, uint index, uint len, const void* data, uint datalen) {
  int len1 = i + 8 + 16+16+4+4+4+len;
  if (buflen>=len1) {
    i = set16_b(buf, i, head);
    i = set8_b(buf, i, type);
    i = set8_n(buf, i, 0, 5);
    i = setp_n(buf, i, sguid.data, 16);
    i = setp_n(buf, i, dguid.data, 16);
    i = set16_b(buf, i, smod);
    i = set16_b(buf, i, dmod);
    i = set32_b(buf, i, index);
    i = set32_b(buf, i, len);
    i = setp_n(buf, i, data, len);
  }
  return i;
}

int get_pack_data(const void* buf, int i, int buflen, uint* head, uint* type, ID* sguid, ID* dguid, uint* smod, uint* dmod, uint* index, uint* len, void* data, uint datalen) {
  i = get16_b(buf, i, head);
  i = get8_b(buf, i, type);
  i+=5;
  i = getp_n(buf, i, sguid->data, 16);
  i = getp_n(buf, i, dguid->data, 16);
  i = get16_b(buf, i, smod);
  i = get16_b(buf, i, dmod);
  i = get32_b(buf, i, index);
  i = get32_b(buf, i, len);
  if (*len<=datalen) {
    i = getp_n(buf, i, data, *len);
  }
  return i;
}

/* 
* 将字符转换为数值 
* */  
int c2i(char ch)  
{  
  // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2  
  if(isdigit(ch))  
    return ch - 48;  
  
  // 如果是字母，但不是A~F,a~f则返回  
  if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )  
    return -1;  
  
  // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10  
  // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10  
  if(isalpha(ch))  
    return isupper(ch) ? ch - 55 : ch - 87;  
  
  return -1;  
}  

/* 
* 功能：将十六进制字符串转换为整型(int)数值 
* */  
uint hex2dec(const char *hex)  
{  
  int len;  
  int num = 0;  
  int temp;  
  int bits;  
  int i;  
  
  // 此例中 hex = "1de" 长度为3, hex是main函数传递的  
  len = strlen(hex);  
  
  for (i=0, temp=0; i<len; i++, temp=0)  
  {  
    // 第一次：i=0, *(hex + i) = *(hex + 0) = '1', 即temp = 1  
    // 第二次：i=1, *(hex + i) = *(hex + 1) = 'd', 即temp = 13  
    // 第三次：i=2, *(hex + i) = *(hex + 2) = 'd', 即temp = 14  
    temp = c2i( *(hex + i) );  
    // 总共3位，一个16进制位用 4 bit保存  
    // 第一次：'1'为最高位，所以temp左移 (len - i -1) * 4 = 2 * 4 = 8 位  
    // 第二次：'d'为次高位，所以temp左移 (len - i -1) * 4 = 1 * 4 = 4 位  
    // 第三次：'e'为最低位，所以temp左移 (len - i -1) * 4 = 0 * 4 = 0 位  
    bits = (len - i - 1) * 4;  
    temp = temp << bits;  
    
    // 此处也可以用 num += temp;进行累加  
    num = num | temp;  
  }  
  
  // 返回结果  
  return num;  
}
int hex2data(const char *hex, void* data, int datalen)   {
  int num = 0;  
  int temp;  
  int i, j=0;
  unsigned char* p = (unsigned char* )data;
  
  // 此例中 hex = "1de" 长度为3, hex是main函数传递的  
  int len = strlen(hex);  
  memset(data, 0, datalen);
  for (i=0, temp=0; (j/2)<datalen && hex[i]; i++, temp=0)  
  {
    uint temp = (uint)c2i( hex[i] );  
    if (temp<16) {
      int bits = (!(j&1))*4;
      p[j/2]|=(temp<<bits);
      j++;
    }
  }
  num = (j+1)/2;
  if (num<datalen) {
    int off = datalen-num;
    memmove(p+off, p, num);
    memset(p, 0, off);
  }
  return num;
}
int data2hex(char *hex, int hexmaxlen, const void* data, int len)   {
  const unsigned char* p = (const unsigned char* )data;
  int i=0, j;
  for (j=0; j<len; ++j) {
    hex[i++] = "0123456789abcdef"[p[j]>>4];
    hex[i++] = "0123456789abcdef"[p[j]&0xf];
    hex[i++] = ' ';
  }
  hex[i] = 0;
  return i;
}


typedef enum {
  RYT_NO_NEED_CONFIRMATION = 0x71,//发送报文，该报文不需要接收方收到确认
    RYT_NEED_CONFIRMATION = 0x51,//发送报文，该报文需要接收方收到确认
    RYT_RESPONSE = 0x41,//接收方应答报文，代表收到了报文，不表示业务数据的处理结果。因此该类型报文数据区长度为0
    RYT_FAULT = 0x45,//故障报文。
} RYT_MSG_TYPE;

typedef struct appid_t {
  ID guid;
  uint mod;
} appid_t;

int appid_find(const appid_t* ids, int nid, const appid_t* id) {
  int i;
  for (i=0; i<nid; ++i) {
    if (0==memcmp(ids+i, id, sizeof(appid_t))) {
      return i;
    }
  }
  return -1;
}
#define MAXAPPID  100

int appid_add(appid_t* ids, int nid, const appid_t* id, int* pnid) {
  if (nid<MAXAPPID) {
    int pos = appid_find(ids, nid, id);
    if (pos<0) {
      ids[nid++] = *id;
      *pnid = nid;
    }
    return 1;
  }
  return 0;
}
int appid_del(appid_t* ids, int nid, const appid_t* id, int* pnid) {
  if (nid>0) {
    int pos = appid_find(ids, nid, id);
    if (pos>0) { // 0是服务器 不能删除
      ids[pos] = ids[--nid];
      *pnid = nid;
    }
    return 1;
  }
  return 0;
}


#include "cjson.inl"
#include "net.h"

typedef struct rongyitcp_t {
  char recvbuf[1<<16];
  char sendbuf[1<<16];
  bool logined;// = false;
  bool facedetected;// = true;
  uint index;// = 0;
  appid_t appid[MAXAPPID];// = {0};
  int nid;// = 1;
  socket_info_t* si;
  addr_in addr[1];
  select_thd_t s[1];
} rongyitcp_t;
#define FACEDETECTMODE  2

int sock_open_tcp_server(const char* ip, int port, SOCK_TYPE type, addr_in* addr);
int sock_sendstr(int sock, char* str) {
  int len = strlen(str);
  return sock_send(sock, str, len, NULL);
}

int rongyitcp_send(rongyitcp_t* r, int sock, uint type, appid_t* appid, uint len, const char* data) {
  appid_t myappid = {0};
  uint head = 0x5231;
  myappid.mod = FACEDETECTMODE;
  int len1 = set_pack_data(r->sendbuf, 0, countof(r->sendbuf), head, type, myappid.guid, appid->guid, myappid.mod, appid->mod, r->index, len, data, countof(data));
  r->index++;
  return sock_send(sock, r->sendbuf, len1, NULL);
}

int rongyitcp_callback(int sock, void* user) {
  rongyitcp_t* r = (rongyitcp_t*)user;
  int ret = sock_recv(sock, r->recvbuf, countof(r->recvbuf), 0);
  uint head=0, type=0, index=0, len=0;
  appid_t sapp = {0}, dapp = {0};
  char data[1<<12];
  char retdata[1<<12];
  char sguid_str[64] = {0};
  char dguid_str[64] = {0};
  r->recvbuf[ret] = 0;
  *retdata = 0;
  get_pack_data(r->recvbuf, 0, ret, &head, &type, &sapp.guid, &dapp.guid, &sapp.mod, &dapp.mod, &index, &len, data, countof(data));
  //printf("%s\n", recvbuf);
  
  int len1 = len;
  if (len>=500) {
    len = 0;
  }
  data[len] = 0;
  data2hex(sguid_str, 64, sapp.guid.data, 16);
  data2hex(dguid_str, 64, dapp.guid.data, 16);
  printf("-----------\ntype=%02x \n sguid=%s \n dguid=%s \n smod=%d, dmod=%d index=%d\n len=%d data=%s\n",
    type, sguid_str, dguid_str, sapp.mod, dapp.mod, index, len1, data);
  
  {
    cJSON *pJsRoot = NULL;
    ret = 0;
    pJsRoot = cJSON_Parse(data);
    if (pJsRoot) {
      cJSON* pJsCmd = cJSON_GetObjectItem(pJsRoot, "cmd");
      if( pJsCmd && pJsCmd->type == cJSON_String ) {
        char* cmd = pJsCmd->valuestring;
        if (0==strcmp(cmd, "login")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "ret");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            r->logined = 0==pJsRet->valueint;
            printf("login ok\n");
          }
        }
        else if (0==strcmp(cmd, "facedetect")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "en");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            r->facedetected = pJsRet->valueint;
          }
          _snprintf(retdata, countof(retdata), "{\"cmd\":\"facedetect\",\"en\":%d}", r->facedetected);
        }
        else if (0==strcmp(cmd, "facedetectlisten")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "en");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            int facedetectlistened = pJsRet->valueint;
            if (facedetectlistened) {
              appid_add(r->appid, r->nid, &sapp, &r->nid);
            } else {
              appid_del(r->appid, r->nid, &sapp, &r->nid);
            }
            _snprintf(retdata, countof(retdata), "{\"cmd\":\"facedetectlisten\",\"en\":%d}", facedetectlistened);
          }
        }
      }
      cJSON_Delete(pJsRoot);
    }
  }
  if (type==RYT_NEED_CONFIRMATION) {
    rongyitcp_send(r, sock, RYT_NO_NEED_CONFIRMATION, &sapp, len, retdata);
  }
  //sock_sendstr(sock, recvbuf);
  return 0;
}

int memiszero(void* p, int len) {
  int i;
  char* c = (char*)p;
  for (i=0; i<len; ++i) {
    if (c[i]) return 0;
  }
  return 1;
}

rongyitcp_t r[1] = {0};

int rongyitcp_login(rongyitcp_t* r) {
  char data[256];
  _snprintf(data, countof(data), "{\"cmd\":\"login\",\"model\":\"%d\"}", FACEDETECTMODE);
  int len = strlen(data);
  rongyitcp_send(r, r->si->sock, RYT_NO_NEED_CONFIRMATION, r->appid, len, data);
  return 0;
}

int rongyitcp_open(rongyitcp_t* r, const char* ip, int port) {
  memset(r, 0, sizeof(*r));
  r->logined = false;
  r->facedetected = true;
  r->nid = 1;
  r->appid[0].guid.data[15] = 1; // 服务器
  r->appid[0].mod = 1;
  r->si = select_client_create(r->s, ip, port, rongyitcp_callback, r, r->addr);
  if (r->si!=NULL) {
    rongyitcp_login(r);
  }
  return r->si!=NULL;
}

int rongyitcp_facedetectinfo(rongyitcp_t* r, int faceindex = -1, int faceid = -1, int sideface = -1, int age = -1, int gender = -1, int glass = -1, int smile = -1) {
  int ctime = time(NULL);
  int ret = 0;
  char data[256];
  _snprintf(data, countof(data), "{\"cmd\": \"facedetectinfo\", \"faceindex\":%d, \"time\":%d, \"faceid\":-1, \"sideface\":-1, \"age\":-1, \"gender\":-1, \"glass\":-1, \"smile\":-1}", faceid, ctime);
  int i, len = strlen(data);
  for (i=0; i<r->nid; ++i) {
    if (!memiszero(r->appid+i, sizeof(appid_t))) {
      ret = rongyitcp_send(r, r->si->sock, RYT_NEED_CONFIRMATION, r->appid+i, len, data);
    }
  }
  return ret;
}

#endif // _RONGYITCP_INL_
