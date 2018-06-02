#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))
// stdafx.cpp : source file that includes just the standard includes
// CPMDev.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
#ifdef USE_IPV6
#define IP_ISOK(IP) (0!=memsum1(&(IP), sizeof(IP)))
bool operator ==(const addr_inx& a, const addr_inx& b)
{
  return 0 == memcmp(&a, &b, sizeof(addr_inx));
}
bool operator <(const addr_inx& a, const addr_inx& b)
{
  return memcmp(&a, &b, sizeof(addr_inx)) < 0;
}
static unsigned memsum1(const void* x, int n)
{
  int i, ss = 0;
  const unsigned char* ux = (const unsigned char*)x;

  for (i = 0; i < n; ++i) {
    ss += ux[i];
  }

  return ss;
}
#if 0
//点分IP转换为网络字节序
static int DotIpToLong1(const char* ip, addr_inx* addr)
{
  int fa = 0, t = 0, RetVal = -1;
  struct addrinfo Hints, *AddrInfo;

  if (!(t = StrIsIP(ip))) {
    return t;
  }

  sock_init();
  /* hints is used after zero cleared */
  memset(&Hints, 0, sizeof(Hints));
  Hints.ai_family = 2 == t ? AF_INET6 : AF_INET;
  Hints.ai_socktype = SOCK_STREAM;
  Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
  RetVal = getaddrinfo(ip, "9999", &Hints, &AddrInfo);

  if (AddrInfo) {
    addr->ai_flags = AddrInfo->ai_flags;
    addr->ai_family = AddrInfo->ai_family;
    addr->ai_socktype = AddrInfo->ai_socktype;
    addr->ai_protocol = AddrInfo->ai_protocol;
    addr->ai_addrlen = AddrInfo->ai_addrlen;
    addr->ai_addr = *AddrInfo->ai_addr;
  }

  return t;
}
#endif
#define g_ip_addr_add_or_del(a, b, c)
addr_inx* g_ip_addr_add_or_del1(const char* ip, const addr_inx* addr, const char* findip)
{
  static std::map<std::string, addr_inx> g_ip_addr;

  if (ip) {
    std::string sip(ip);

    if (addr) {
      g_ip_addr.insert(std::pair<std::string, addr_inx>(sip, *addr));
    }
    else {
      std::map<std::string, addr_inx>::iterator pos = g_ip_addr.find(sip);

      if (pos != g_ip_addr.end()) {
        g_ip_addr.erase(pos);
      }
    }
  }

  if (findip) {
    std::string sip(findip);
    std::map<std::string, addr_inx>::iterator pos = g_ip_addr.find(sip);

    if (pos != g_ip_addr.end()) {
      return &pos->second;
    }
  }

  return NULL;
}
//点分IP转换为网络字节序
static int DotIpToLong(const char* ip, addr_inx* out)
{
#if 0
  addr_inx* ccc = g_ip_addr_add_or_del(NULL, NULL, ip);

  if (ccc) {
    *out = *ccc;
  }

#endif
  return net_addr(ip, 0, out);
}
//网络字节序long转换为点分IP[INET6_ADDRSTRLEN]
static int LongToDotIp(char* ip, int len, const addr_inx* addr)
{
  if (NULL == ip || !IP_ISOK(addr)) {
    return 0;
  }

  return net_ntop((sockaddr*)addr, ip, len);
}
#else
#define IP_ISOK(IP) (0!=(IP))
#define g_ip_addr_add_or_del(a, b, c)
//点分IP转换为网络字节序
static int DotIpToLong1(const char* ip, unsigned int* pout)
{
  if (!StrIsIP(ip)) {
    return 0;
  }

  unsigned int a = 0;
  unsigned int b = 0;
  unsigned int c = 0;
  unsigned int d = 0;
  unsigned int rs = 0;
  unsigned char* pByte = (BYTE*)&rs;
  sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
  pByte[0] = a;
  pByte[1] = b;
  pByte[2] = c;
  pByte[3] = d;
  *pout = rs;
  return 16;
}
static int DotIpToLong(const char* ip, unsigned int* pout)
{
  addr_in out[1];
  net_addr(ip, 0, out);

  if (AF_INET == out->family) {
    struct sockaddr_in* p = (struct sockaddr_in*)out;
    //unsigned int aa = 0;
    //DotIpToLong1("192.168.1.150", &aa);
    *pout = p->sin_addr.s_addr;
    return 16;
  }

  return 0;
}
unsigned int DotIpToLong_v2(const char* ip)
{
  unsigned int a = 0 ;
  unsigned int b = 0;
  unsigned int c = 0;
  unsigned int d = 0;
  sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
  return (a << 24) | (b << 16) | (c << 8) | d;
}
//网络字节序 转换为点分IP
static void LongToDotIp(char* ip, int len, addr_inx* l)
{
  if (NULL == ip || *l == 0) {
    return;
  }

  BYTE* pByte = (BYTE*)l;
  _snprintf(ip, len, "%d.%d.%d.%d", pByte[0], pByte[1], pByte[2], pByte[3]);
}
#endif

