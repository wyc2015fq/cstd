
#include <math.h>
#include "net/net_win32.h"
//#include <winsock2.h>
//#include <mstcpip.h>
//#include <ntddndis.h>
//#include <windows.h>

#pragma comment(lib,"ws2_32.lib")


typedef struct IPHeader {
  unsigned char mHeaderLengthAndVersion;
  unsigned char mTypeOfService;
  unsigned short mTotalLength;
  unsigned short mIdentifier;
  unsigned short mFragment;
  unsigned char mTTL;
  unsigned char mProtocolType;
  unsigned short mCheckSum;
  unsigned int mSrcIP;
  unsigned int mDstIP;
} IPHeader;

int getHeaderLength(const IPHeader* ipheader)
{
  return ipheader->mHeaderLengthAndVersion & 0xf;
}

typedef struct TCPHeader {
  unsigned short mSrcPort;
  unsigned short mDstPort;
  unsigned int mSequence;
  unsigned int mAck;
  unsigned char mLengthAndReserve;
  unsigned char mFlags;
  unsigned short mWindow;
  unsigned short mCheckSum;
  unsigned short mUrgent;
} TCPHeader;

typedef struct UDPHeader {
  unsigned short mSrcPort;
  unsigned short mDstPort;
  unsigned short mLength;
  unsigned short mCheckSum;
} UDPHeader;

typedef struct ICMPHeader {
  unsigned char mType;
  unsigned char mCode;
  unsigned short mCheckSum;
  unsigned short mID;
  unsigned short mSequence;
  unsigned long mTimeStamp;
} ICMPHeader;

int iptoint(const char* ip)
{
  return ntohl(inet_addr(ip));
}

char* inttoip(int ip_num, char* ip)
{
  static char buf[32];
  uint uip_num = (uint)ip_num;
  ip = ip ? ip : buf;
  //_snprintf()
  //PrintIP(ip_num);
  {
    uchar p0, p1, p2, p3;
    p0 = (uchar)((uip_num >> 0) & 0xff);
    p1 = (uchar)((uip_num >> 8) & 0xff);
    p2 = (uchar)((uip_num >> 16) & 0xff);
    p3 = (uchar)((uip_num >> 24) & 0xff);
    _snprintf(ip, 32, "%d.%d.%d.%d", p0, p1, p2, p3);
    //printf("%d.%d.%d.%d\n", p0, p1, p2, p3);
  }
  return ip;
}
