
#ifndef _MAC_INL_
#define _MAC_INL_


#if  defined _WIN32 || defined _WIN64
#include <windows.h>


char* get_all_mac(const char *eth_inf)
{
  typedef struct _ASTAT_
  {
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
  }ASTAT, * PASTAT;
  
  ASTAT Adapter;
#pragma comment(lib,"Netapi32.lib")
  static char macstr[256] = {0};
  if (!macstr[0]) {
    NCB Ncb;
    UCHAR uRetCode;
    //char NetName[50];
    LANA_ENUM   lenum;
    int      i;
    
    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );
    printf( "The NCBENUM return code is: 0x%x \n", uRetCode );
    
    for(i=0; i < lenum.length ;i++) {
      memset( &Ncb, 0, sizeof(Ncb) );
      Ncb.ncb_command = NCBRESET;
      Ncb.ncb_lana_num = lenum.length;
      
      uRetCode = Netbios( &Ncb );
      printf( "The NCBRESET on LANA %d return code is: 0x%x \n", lenum.length, uRetCode );
      
      memset( &Ncb, 0, sizeof (Ncb) );
      Ncb.ncb_command = NCBASTAT;
      Ncb.ncb_lana_num = lenum.length;
      
      strncpy( (char*)Ncb.ncb_callname,  "*               ", NCBNAMSZ );
      Ncb.ncb_buffer = (unsigned char *) &Adapter;
      Ncb.ncb_length = sizeof(Adapter);
      
      uRetCode = Netbios( &Ncb );
      printf( "The NCBASTAT on LANA %d return code is: 0x%x \n", lenum.lana, uRetCode );
      if ( uRetCode == 0 )
      {
        _snprintf( macstr, 256, "%02x%02x%02x%02x%02x%02x",
          Adapter.adapt.adapter_address[0],
          Adapter.adapt.adapter_address[1],
          Adapter.adapt.adapter_address[2],
          Adapter.adapt.adapter_address[3],
          Adapter.adapt.adapter_address[4],
          Adapter.adapt.adapter_address[5] );
        printf("%s\n", macstr);
      }
    }
  }
  return macstr;
}

// #include "nb30.h"

// 获取远程网卡MAC地址了：
char* get_local_mac(const char *eth_inf)
{
  static char sMacAddress[256] = {0};
  
  if (!(*sMacAddress)) {
#pragma comment(lib,"netapi32.lib")
    typedef struct _ASTAT_
    {
      ADAPTER_STATUS adapt;
      NAME_BUFFER  NameBuff[30];
    } ASTAT, * PASTAT;
    
    ASTAT Adapter;
    
    NCB ncb;
    UCHAR uRetCode;
    const char* sNetBiosName = eth_inf ? eth_inf : "*               ";
    
    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0;
    
    uRetCode = Netbios(&ncb);
    
    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0;
    
    //sNetBiosName.MakeUpper();
    
    FillMemory(ncb.ncb_callname, NCBNAMSZ - 1, 0x20);
    
    strcpy((char*)ncb.ncb_callname, sNetBiosName);
    ncb.ncb_callname[NCBNAMSZ] = 0x0;
    
    ncb.ncb_buffer = (unsigned char *) &Adapter;
    ncb.ncb_length = sizeof(Adapter);
    
    uRetCode = Netbios(&ncb);
    if (uRetCode == 0)
    {
      _snprintf(sMacAddress, 256, ("%02x%02x%02x%02x%02x%02x"),
        Adapter.adapt.adapter_address[0],
        Adapter.adapt.adapter_address[1],
        Adapter.adapt.adapter_address[2],
        Adapter.adapt.adapter_address[3],
        Adapter.adapt.adapter_address[4],
        Adapter.adapt.adapter_address[5]);
    }
  }
  return sMacAddress;
}
#endif

#ifdef __linux__

#include <net/if.h> /* for ifconf */
#include <linux/sockios.h> /* for net status mask */
#include <netinet/in.h> /* for sockaddr_in */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_INTERFACE (16)

// 获取本机mac get_local_mac("wlan0")
static char* get_local_mac(const char *eth_inf) 
{
  static char machin_MAC[256] = {0};
  struct ifreq ifr; 
  int sd;
  eth_inf = eth_inf ? eth_inf : "wlan0";
  
  bzero(&ifr, sizeof(struct ifreq)); 
  if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  { 
    printf("get %s mac address socket creat error\n", eth_inf);
    return ""; 
  } 
  
  strncpy(ifr.ifr_name, eth_inf, sizeof(ifr.ifr_name) - 1); 
  
  if(ioctl(sd, SIOCGIFHWADDR, &ifr) < 0) 
  { 
    printf("get %s mac address error\n", eth_inf);
    close(sd); 
    return ""; 
  } 
  
  snprintf(machin_MAC, 256, "%02x:%02x:%02x:%02x:%02x:%02x", 
    (unsigned char)ifr.ifr_hwaddr.sa_data[0], 
    (unsigned char)ifr.ifr_hwaddr.sa_data[1], 
    (unsigned char)ifr.ifr_hwaddr.sa_data[2], 
    (unsigned char)ifr.ifr_hwaddr.sa_data[3], 
    (unsigned char)ifr.ifr_hwaddr.sa_data[4], 
    (unsigned char)ifr.ifr_hwaddr.sa_data[5]); 
  
  close(sd);
  return machin_MAC; 
}

#endif
#endif // _MAC_INL_
