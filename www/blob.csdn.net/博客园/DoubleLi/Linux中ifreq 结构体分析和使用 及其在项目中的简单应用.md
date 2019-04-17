# Linux中ifreq 结构体分析和使用 及其在项目中的简单应用 - DoubleLi - 博客园






[基础知识说明]

**结构原型:**

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */

struct ifreq 
{
#define IFHWADDRLEN 6
 union
 {
  char ifrn_name[IFNAMSIZ];  /* if name, e.g. "en0" */
 } ifr_ifrn;

 union {
  struct sockaddr ifru_addr;
  struct sockaddr ifru_dstaddr;
  struct sockaddr ifru_broadaddr;
  struct sockaddr ifru_netmask;
  struct  sockaddr ifru_hwaddr;
  short ifru_flags;
  int ifru_ivalue;
  int ifru_mtu;
  struct  ifmap ifru_map;
  char ifru_slave[IFNAMSIZ]; /* Just fits the size */
  char ifru_newname[IFNAMSIZ];
  void __user * ifru_data;
  struct if_settings ifru_settings;
 } ifr_ifru;
};

#define ifr_name ifr_ifrn.ifrn_name /* interface name  */
#define ifr_hwaddr ifr_ifru.ifru_hwaddr /* MAC address   */
#define ifr_addr ifr_ifru.ifru_addr /* address  */
#define ifr_dstaddr ifr_ifru.ifru_dstaddr /* other end of p-p lnk */
#define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast address */
#define ifr_netmask ifr_ifru.ifru_netmask /* interface net mask */
#define ifr_flags ifr_ifru.ifru_flags /* flags  */
#define ifr_metric ifr_ifru.ifru_ivalue /* metric  */
#define ifr_mtu  ifr_ifru.ifru_mtu /* mtu   */
#define ifr_map  ifr_ifru.ifru_map /* device map  */
#define ifr_slave ifr_ifru.ifru_slave /* slave device  */
#define ifr_data ifr_ifru.ifru_data /* for use by interface */
#define ifr_ifindex ifr_ifru.ifru_ivalue /* interface index */
#define ifr_bandwidth ifr_ifru.ifru_ivalue    /* link bandwidth */
#define ifr_qlen ifr_ifru.ifru_ivalue /* Queue length  */
#define ifr_newname ifr_ifru.ifru_newname /* New name  */
#define ifr_settings ifr_ifru.ifru_settings /* Device/proto settings*/



**基本介绍: **

ifreq结构定义在/usr/include/net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息的。其中包含了一个接口的名字和具体内容——（是个共用体，有可能是IP地址，广播地址，子网掩码，MAC号，MTU或其他内容）。ifreq包含在ifconf结构中。而ifconf结构通常是用来保存所有接口的信息的。



**举例说明：**

在Linux系统中，ifconfig命令是通过ioctl接口与内核通信，例如，当系统管理员输入如下命令来改变接口eth0的MTU大小：

    ifconfig eth0 mtu 1250

ifconfig命令首先打开一个socket，然后通过系统管理员输入的参数初始化一个数据结构，并通过ioctl调用将数据传送到内核。SIOCSIFMTU是命令标识符。

    struct ifreq data;
    fd = socket(PF_INET, SOCK_DGRAM, 0);
    < ... initialize "data" ...>
    err = ioctl(fd, SIOCSIFMTU, &data);



[举例应用]

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/**
*
* \brief   通过广播socket获取系统eth0当前IP地址
*
* \param   pAddr   存储IP地址的内存地址，不小于16个字节。
*
* \Return  0       成功
*          else    失败
*/
int  GetSysIpBySocket(char* pAddr)
{
   if ( pAddr == NULL )
   {
       return RET_ERR_PARAM;
   }

   struct sockaddr_in *sin;
   struct ifreq        ifr;

   int sock = socket(AF_INET,SOCK_DGRAM,0);
   if(sock <= 0)
   {
      perror("socket error!\n");
       return RET_ERROR;
   }

  strcpy(ifr.ifr_name,"eth0");

  if(ioctl(sock,SIOCGIFADDR,&ifr) < 0)
   {
       perror("ioctl error\n");
      close(sock);
      return RET_ERROR;
   }
   else
   {
       sin = (struct sockaddr_in *)&(ifr.ifr_addr);
       strcpy(pAddr, inet_ntoa(sin->sin_addr));
       close(sock);
   }
   return RET_OK;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/**
 *
 * \brief   通过广播socket获取系统eth0当前子网掩码
 *
 * \param   pMask   存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0       成功
 *          else    失败
 */
int  GetSysMaskBySocket(char* pMask)
{
    if ( pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct sockaddr_in *sin;
    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFNETMASK,&ifr) < 0)
    {
        perror("ioctl error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
        strcpy(pMask, inet_ntoa(sin->sin_addr));
        close(sock);
    }

    return RET_OK;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```





```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/**
 *
 * \brief   通过广播socket获取系统eth0当前IP地址和子网掩码
 *
 * \param   pAddr   存储IP地址的内存地址，不小于16个字节。
 * \param   pMask   存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0       成功
 *          else    失败
 */
int GetSysIpMaskBySocket(char* pAddr, char* pMask)
{
    if ( pAddr == NULL || pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct sockaddr_in *sin;
    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFADDR,&ifr) < 0)
    {
        perror("ioctl SIOCGIFADDR error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        strcpy(pAddr, inet_ntoa(sin->sin_addr));
    }

    if(ioctl(sock,SIOCGIFNETMASK,&ifr) < 0)
    {
        perror("ioctl SIOCGIFNETMASK error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
        strcpy(pMask, inet_ntoa(sin->sin_addr));

    }

    close(sock);
    return RET_OK;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/**
 *
 * \brief   通过ioctl获取系统eth0 Mac地址
 *
 * \param   pMask   存储 Mac地址的内存地址，不小于18个字节。
 *
 * \Return  0       成功
 *          else    失败
 */
int  GetSysMacBySocket(char* pMac)
{
    if ( pMac == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFHWADDR,&ifr) < 0)
    {
        perror("ioctl SIOCGIFHWADDR error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sprintf(pMac, "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    }

    close(sock);
    return RET_OK;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



参考H CPP文件

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/*
 * =====================================================================================
 * 
 *       Filename:  ipconfig.h
 * 
 *    Description:  基于ifconfig命令，获取、配置系统当前网络配置参数。
 * 
 *        Version:  1.0
 *        Created:  11/23/2012 01:42:16 PM HKT
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  
 *        Company:  
 * 
 * =====================================================================================
 */

#ifndef  IPCONFIG_INC
#define  IPCONFIG_INC


bool IsIpValid   (const char* pAddr);
bool IsMaskValid (const char* pNetMask);
bool IsMacValid  (const char* pMac);
bool IsGatewayValid(const char* pGateway);

int  GetSysIp    (char* pAddr);
int  GetSysMask  (char* pMask);
int  GetSysIpMask(char* pAddr, char* pMask);
int  GetSysMac   (char* pMac);
int  GetSysGateway(char* pGateway);
int  GetSysBroadcast(const char* pAddr, const char* pMask, char* pBc);
    
int  GetSysIpBySocket    (char* pAddr);
int  GetSysMaskBySocket  (char* pMask);
int  GetSysIpMaskBySocket(char* pAddr, char* pMask);
int  GetSysMacBySocket   (char* pMac);

int  SetSysIp    (const char* pAddr);
int  SetSysMask  (const char* pNetMask);
int  SetSysMac   (const char* pMac);
int  SetSysIpMask(const char* pAddr, const char* pNetMask);
int  SetSysGateway(const char* pGateway);

#endif   /* ----- #ifndef IPCONFIG_INC  ----- */

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

/*
 * =====================================================================================
 *
 *       Filename:  ipconfig.cpp
 *
 *    Description:  基于ifconfig命令，获取、配置系统当前网络配置参数。
 *
 *        Version:  1.0
 *        Created:  11/23/2012 11:35:52 AM HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "RcsFile.h"
#include "lederror.h"

static const unsigned int IPADDR_LEN  = 15;//IP地址长度
static const unsigned int MACADDR_LEN = 17;//MAC地址长度


/**
 * \brief 判断字符是否为十六进制表示字符，即0~f。
 */
bool IsHexChar(char hex)
{
    return ( ( hex >= '0' && hex <= '9' )    ||
                ( hex >= 'a' && hex <= 'f' ) ||
                ( hex >= 'A' && hex <= 'F' ) );
}


int HexCharToInt(char hex)
{
    int val = -1;

    if( hex >= '0' && hex <= '9' )
    {
        val = hex - '0';
    }
    else if( hex >= 'a' && hex <= 'f' ) 
    {
        val = hex - 'a' + 10;
    }
    else if( hex >= 'A' && hex <= 'F' )
    {
        val = hex - 'A' + 10;
    }

    return val;
}


/**
 * \brief 判断输入ip是否有效
 *
 * \return true   有效
 *            false  无效
 */
bool IsIpValid(const char* pAddr)
{
    if ( NULL == pAddr )
    {
        return false;
    }
    struct in_addr addr;
    if ( inet_aton ( pAddr, &addr ) == 0 )
    {
        return false;
    }

    char* destIp = ( char* ) inet_ntoa ( addr );
    if ( 0 != strcmp ( pAddr, destIp ) )
    {
        return false;
    }

    return true;
}


/**
 * \brief 判断输入netmask字符串是否有效
 *
 * \param    pNetMask    表示netmask的字符串指针
 *
 * \return  true         有效
 * \return  false         无效
 */
bool IsMaskValid(const char* pNetMask)
{
    bool ret = false;

    if ( !IsIpValid(pNetMask) )
    {
        return ret;
    }

    unsigned int mask = ntohl( inet_addr(pNetMask) );

    //最高8位必须全为0，且不能为0xffffff
    if ( mask < 0xff || mask == 0xffffffff )
    {
        return ret;
    }

    for ( int i = 23; i >= 0; --i )
    {
        if ( (mask & (1 << i)) > 0 )
        {
            continue;
        }
        if ( (mask << (8 + 23 - i)) > 0 )
        {
            ret = false;
            break;
        }
        ret = true;
        break;
    }

    return ret;
}


/**
 * \brief 判断输入mac字符串是否有效
 *
 * \param    pMac    表示mac地址的字符串指针
 *
 * \return  true     有效
 * \return  false     无效
 */
bool IsMacValid(const char* pMac)
{
    if ( NULL == pMac || strlen ( pMac ) != MACADDR_LEN )
    {
        return false;
    }

    //MAC的第一个字节必须为偶数
    if( HexCharToInt(*(pMac+1)) % 2 != 0 )
    {
        return false;
    }

    for ( unsigned int index = 0; index < MACADDR_LEN; ++index )
    {
        if ( 0 == ( ( index + 1 ) % 3 ) ) //不检查间隔为3的字符
        {
            continue;
        }

        //字符必须为十六进制表示字符，即0~f。
        char val = ( * ( pMac + index ) );
        if ( !IsHexChar(val) )
        {
            return false;
        }
    }

    return true;
}

/**
 * \brief 判断输入网关字符串是否有效
 *
 * \param    pGateway    表示网关地址的字符串指针
 *
 * \return  true     有效
 * \return  false     无效
 */
bool IsGatewayValid(const char* pGateway)
{
    return IsIpValid(pGateway);
}



/**
 *
 * \brief      获取系统eth0当前IP地址
 *
 * \param    pAddr    存储IP地址的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysIp(char* pAddr)
{
    const char FILE_NAME[] = "/tmp/ipaddr.txt";
    const char SCRIPT[]    = "ifconfig eth0 | grep inet | cut -d: -f2 | cut -d' ' -f1 > /tmp/ipaddr.txt";
    FILE*       handle      = NULL;
    char*       delimiter   = NULL;

    if ( pAddr == NULL )
    {
        return RET_ERR_PARAM;
    }

    if ( system(SCRIPT) != 0 )
    {
        fprintf(stderr, "Execute command '%s'error: %s\n", SCRIPT, strerror(errno));
        goto FAILED;
    }

    if ( (handle = fopen(FILE_NAME, "r")) != NULL )
    {
        fgets(pAddr, IPADDR_LEN-1, handle);
    }
    else
    {
        fprintf(stderr, "Open file %s error: %s\n", FILE_NAME, strerror(errno));
        goto FAILED;
    }

    fclose(handle);

    if ( (delimiter = strchr(pAddr, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    unlink(FILE_NAME);
    return RET_OK;

FAILED:
    unlink(FILE_NAME);
    return RET_ERROR;
}


/**
 *
 * \brief      获取系统eth0当前子网掩码
 *
 * \param    pMask    存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysMask(char* pMask)
{
    const char FILE_NAME[] = "/tmp/netmask.txt";
    const char SCRIPT[]    = "ifconfig eth0 | grep Mask | cut -dk -f2 | cut -d: -f2 > /tmp/netmask.txt";
    FILE*       handle      = NULL;
    char*       delimiter   = NULL;

    if ( pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    if ( system(SCRIPT) != 0 )
    {
        fprintf(stderr, "Execute command '%s'error: %s\n", SCRIPT, strerror(errno));
        goto FAILED;
    }

    if ( (handle = fopen(FILE_NAME, "r")) != NULL )
    {
        fgets(pMask, IPADDR_LEN+1, handle);
    }
    else
    {
        fprintf(stderr, "Open file %s error: %s\n", FILE_NAME, strerror(errno));
        goto FAILED;
    }

    fclose(handle);

    if ( (delimiter = strchr(pMask, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    unlink(FILE_NAME);
    return RET_OK;

FAILED:
    unlink(FILE_NAME);
    return RET_ERROR;
}

/**
 *
 * \brief      获取系统eth0当前IP地址和子网掩码
 *
 * \param    pAddr    存储IP地址的内存地址，不小于16个字节。
 * \param    pMask    存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int GetSysIpMask(char* pAddr, char* pMask)
{

    const char FILE_NAME[] = "/tmp/ipmask.txt";
    const char SCRIPT[]    = "ifconfig eth0 | grep -i mask > /tmp/ipmask.txt";
    char       info[128]   = {0};
    char       *delimiter  = NULL;
    FILE       *handle     = NULL; 

    if ( pAddr == NULL || pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    if ( system(SCRIPT) != 0 )
    {
        fprintf(stderr, "Execute command '%s'error: %s\n", SCRIPT, strerror(errno));
        goto FAILED;
    }
    
    if ( (handle = fopen(FILE_NAME, "r")) != NULL )
    {

        fread(info, 128, 1, handle);

        char* ipbegin = strstr(info, "addr:");
        if( ipbegin )
        {
            ipbegin += 5;
            sscanf(ipbegin, "%s", pAddr);
        }

        char* maskbegin = strstr(info, "Mask:");
        if( maskbegin )
        {
            maskbegin += 5;
            sscanf(maskbegin, "%s", pMask);
        }

        fclose(handle);
    }
    else
    {
        fprintf(stderr, "Open file %s error: %s\n", FILE_NAME, strerror(errno));
        goto FAILED;
    }

    if ( (delimiter = strchr(pAddr, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    if ( (delimiter = strchr(pMask, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    unlink(FILE_NAME);
    return RET_OK;

FAILED:
    unlink(FILE_NAME);
    return RET_ERROR;
}

/**
 *
 * \brief      获取系统eth0 Mac地址
 *
 * \param    pMask    存储 Mac地址的内存地址，不小于18个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysMac(char* pMac)
{
    const char FILE_NAME[] = "/tmp/ipmac.txt";
    const char SCRIPT[]    = "ifconfig eth0 | grep HWaddr|tr -s ' '|cut -d ' ' -f 5 > /tmp/ipmac.txt";
    FILE*       handle      = NULL;
    char*       delimiter   = NULL;

    if ( pMac == NULL )
    {
        return RET_ERR_PARAM;
    }

    if ( system(SCRIPT) != 0 )
    {
        fprintf(stderr, "Execute command '%s'error: %s\n", SCRIPT, strerror(errno));
        goto FAILED;
    }

    if ( (handle = fopen(FILE_NAME, "r")) != NULL )
    {
        fgets(pMac, MACADDR_LEN+1, handle);
    }
    else
    {
        fprintf(stderr, "Open file %s error: %s\n", FILE_NAME, strerror(errno));
        goto FAILED;
    }

    fclose(handle);

    if ( (delimiter = strchr(pMac, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    unlink(FILE_NAME);
    return RET_OK;

FAILED:
    unlink(FILE_NAME);
    return RET_ERROR;
}

/**
 * \brief 获取系统网关地址。
 *
 * \return    RET_OK          成功
 * \return     RET_ERROR   失败
 */
int GetSysGateway(char* pGateway)
{
    const char FILE_NAME[] = "/tmp/ipgateway.txt";
    const char SCRIPT[]    = "route  | grep default | cut -d' ' -f10 | cut -d' ' -f1 > /tmp/ipgateway.txt";
    FILE*       handle      = NULL;
    char*       delimiter   = NULL;

    if ( pGateway == NULL )
    {
        return RET_ERR_PARAM;
    }

    if ( system(SCRIPT) != 0 )
    {
        fprintf(stderr, "Execute command '%s'error: %s\n", SCRIPT, strerror(errno));
        goto FAILED;
    }

    if ( (handle = fopen(FILE_NAME, "r")) != NULL )
    {
        fgets(pGateway, IPADDR_LEN+1, handle);
    }
    else
    {
        fprintf(stderr, "Open file %s error: %s\n", FILE_NAME, strerror(errno));
        goto FAILED;
    }

    fclose(handle);

    if ( (delimiter = strchr(pGateway, '\x0a')) != NULL )
    {
        *delimiter = '\0';
    }

    unlink(FILE_NAME);
    return RET_OK;

FAILED:
    unlink(FILE_NAME);
    return RET_ERROR;
}


int SetSysIp(const char* pAddr)
{
    if ( !IsIpValid(pAddr) )
    {
        fprintf(stderr, "Input ip %s is invalid\n", pAddr);
        return RET_ERROR;
    }

    char localip[80] = {0};

    GetSysIp(localip);
    if ( strcmp(localip, pAddr) == 0 )
    {
        return RET_OK;
    }

    fprintf(stderr, "[%s]: ip is %s\n", __FUNCTION__, pAddr);
    sprintf(localip, "ifconfig eth0 %s up ", pAddr);
    if ( system(localip) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", localip, strerror(errno));
        return RET_ERROR;
    }

    return ModifyRcs("LED_IP",pAddr);
}


int SetSysMask(const char* pMask)
{
    if ( !IsMaskValid(pMask) )
    {
        fprintf(stderr, "Input netmask %s is invalid\n", pMask);
        return RET_ERROR;
    }

    char netmask[80] = {0};

    GetSysIp(netmask);
    if ( strcmp(netmask, pMask) == 0 )
    {
        return RET_OK;
    }

    fprintf(stderr, "[%s]: netmask is %s\n", __FUNCTION__, pMask);
    sprintf(netmask, "ifconfig eth0 netmask %s up ", pMask);
    if ( system(netmask) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", netmask, strerror(errno));
        return RET_ERROR;
    }

    return ModifyRcs("LED_MASK",pMask);
}


int SetSysMac(const char* pMac)
{
    if ( !IsMacValid(pMac) )
    {
        fprintf(stderr, "Input mac addr %s is invalid\n", pMac);
        return RET_ERROR;
    }

    char mac[80] = {0};

    GetSysMac(mac);
    if ( strcmp(mac, pMac) == 0 )
    {
        return RET_OK;
    }

    fprintf(stderr, "[%s]: mac addr is %s\n", __FUNCTION__, pMac);

    //shut down network
    memset(mac, 0, sizeof(mac));
    sprintf(mac, "ifconfig eth0 down");
    if ( system(mac) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", mac, strerror(errno));
        return RET_ERROR;
    }

    //config mac address 
    memset(mac, 0, sizeof(mac));
    sprintf(mac, "ifconfig eth0 hw ether %s", pMac);
    if ( system(mac) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", mac, strerror(errno));
        return RET_ERROR;
    }

    //start network
    memset(mac, 0, sizeof(mac));
    sprintf(mac, "ifconfig eth0 up");
    if ( system(mac) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", mac, strerror(errno));
        return RET_ERROR;
    }

    return ModifyRcs("LED_MAC",pMac);
}


/**
 * \brief     设置ip和mask。
 *
 * \return    RET_OK         成功    
 * \return     RET_ERROR   失败    
 */
int   SetSysIpMask(const char* pAddr, const char* pNetMask)
{
    //判断ip和mask的有效性
    if ( (!IsIpValid(pAddr)) || (!IsMaskValid(pNetMask)) )
    {
        fprintf(stderr, "Input ip %s or netmask %s is invalid\n", pAddr, pNetMask);
        return RET_ERROR;
    }

    //设置ip和netmask
    char script[80] = {0};
    sprintf(script, "ifconfig eth0 %s netmask %s up ", pAddr, pNetMask);
    if ( system(script) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", script, strerror(errno));
        return RET_ERROR;
    }

    ModifyRcs("LED_IP",pAddr);
    return ModifyRcs("LED_MASK",pNetMask);
}


/**
 * \brief     设置gateway。
 *
 * \return    RET_OK         成功    
 * \return     RET_ERROR   失败    
 */
int   SetSysGateway(const char* pGateway)
{
    if( pGateway && strlen(pGateway) == 0 )
    {
        //把网关删除，设为空
    }
    else
    {
        //判断gateway的有效性
        if ( (!IsGatewayValid(pGateway)) )
        {
            fprintf(stderr, "Input Gateway %s is invalid\n", pGateway);
            return RET_ERROR;
        }
    }

    //设置gateway
    char script[80] = {0};
    sprintf(script, "route del default ");
    if ( system(script) != 0 )
    {
        fprintf(stderr, "Execute command '%s' error: %s\n", script, strerror(errno));
    }
    
    if( strlen(pGateway) )
    {
        sprintf(script, "route add default gw %s  ", pGateway);
        if ( system(script) != 0 )
        {
            fprintf(stderr, "Execute command '%s' error: %s\n", script, strerror(errno));
            return RET_ERROR;
        }
    }

    return ModifyRcs("LED_GATEWAY",pGateway);
}


/**
 * \brief 根据IP和子网掩码获取本网段广播地址。
 */
int  GetSysBroadcast(const char* pAddr, const char* pMask, char* pBc)
{
    if( !pAddr || !pMask || !pBc )
    {
        return RET_ERR_PARAM;
    }

    if( !IsIpValid(pAddr) || !IsMaskValid(pMask) )
    {
        return RET_ERR_PARAM;
    }

    unsigned int ip   = ntohl( inet_addr(pAddr) );
    unsigned int mask = ntohl( inet_addr(pMask) );
    unsigned int bc   = (ip & mask) | (~mask);

    struct in_addr bcAddr;
    bcAddr.s_addr = htonl(bc);

    char *tmp = inet_ntoa(bcAddr);
    memcpy(pBc, tmp, strlen(tmp));

    return RET_OK;
}


/**
 *
 * \brief      通过广播socket获取系统eth0当前IP地址
 *
 * \param    pAddr    存储IP地址的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysIpBySocket(char* pAddr)
{
    if ( pAddr == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct sockaddr_in *sin;
    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0); 
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFADDR,&ifr) < 0)
    {
        perror("ioctl error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        strcpy(pAddr, inet_ntoa(sin->sin_addr));
        close(sock);
    }

    return RET_OK;
}


/**
 *
 * \brief      通过广播socket获取系统eth0当前子网掩码
 *
 * \param    pMask    存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysMaskBySocket(char* pMask)
{
    if ( pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct sockaddr_in *sin;
    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0); 
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFNETMASK,&ifr) < 0)
    {
        perror("ioctl error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
        strcpy(pMask, inet_ntoa(sin->sin_addr));
        close(sock);
    }

    return RET_OK;
}

/**
 *
 * \brief      通过广播socket获取系统eth0当前IP地址和子网掩码
 *
 * \param    pAddr    存储IP地址的内存地址，不小于16个字节。
 * \param    pMask    存储子网掩码的内存地址，不小于16个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int GetSysIpMaskBySocket(char* pAddr, char* pMask)
{
    if ( pAddr == NULL || pMask == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct sockaddr_in *sin;
    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0); 
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFADDR,&ifr) < 0)
    {
        perror("ioctl SIOCGIFADDR error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        strcpy(pAddr, inet_ntoa(sin->sin_addr));
    }

    if(ioctl(sock,SIOCGIFNETMASK,&ifr) < 0)
    {
        perror("ioctl SIOCGIFNETMASK error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sin = (struct sockaddr_in *)&(ifr.ifr_netmask);
        strcpy(pMask, inet_ntoa(sin->sin_addr));

    }

    close(sock);
    return RET_OK;
}

/**
 *
 * \brief      通过ioctl获取系统eth0 Mac地址
 *
 * \param    pMask    存储 Mac地址的内存地址，不小于18个字节。
 *
 * \Return  0        成功
 *            else    失败
 */
int  GetSysMacBySocket(char* pMac)
{
    if ( pMac == NULL )
    {
        return RET_ERR_PARAM;
    }

    struct ifreq        ifr;

    int sock = socket(AF_INET,SOCK_DGRAM,0); 
    if(sock <= 0)
    {
        perror("socket error!\n");
        return RET_ERROR;
    }

    strcpy(ifr.ifr_name,"eth0");

    if(ioctl(sock,SIOCGIFHWADDR,&ifr) < 0)
    {
        perror("ioctl SIOCGIFHWADDR error\n");
        close(sock);
        return RET_ERROR;
    }
    else
    {
        sprintf(pMac, "%02x:%02x:%02x:%02x:%02x:%02x",
                (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
    }

    close(sock);
    return RET_OK;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```









