# linux下 取得本机ip、掩码、网关  - ljx0305的专栏 - CSDN博客
2008年01月30日 23:15:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：4021
一、取得本机ip及掩码
/********************************************************************
* 函数名： get_host_info
* 参数名： local_ip(out)    ip
*         local_mask(out)  mask
* 返回值： 0                成功
*         -1              失败
* 功  能：获取本地机的ip及掩码
********************************************************************/
int get_host_info(string &local_ip, string &local_mask)
{
 struct sockaddr_in *my_ip;
 struct sockaddr_in *addr;
 struct sockaddr_in myip;
 my_ip = &myip;
 struct ifreq ifr;
 int sock;
 if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
 {
  return -1;
 }
 strcpy(ifr.ifr_name, "eth0");
 //取本机IP地址
 if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
 {
    return -1;
 }
 my_ip->sin_addr = ((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr;
 local_ip = inet_ntoa(my_ip->sin_addr);
 //取本机掩码
 if( ioctl( sock, SIOCGIFNETMASK, &ifr) == -1 ){
   perror("[-] ioctl");
   return -1;
 }
 addr = (struct sockaddr_in *) & (ifr.ifr_addr);
 local_mask = inet_ntoa( addr->sin_addr);
 close(sock);
 return 0;
}
二、取得本机网关
从网上找的，其中的错误已修改。
#define BUFSIZE 8192
struct route_info{
 u_int dstAddr;
 u_int srcAddr;
 u_int gateWay;
 char ifName[IF_NAMESIZE];
};
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
  struct nlmsghdr *nlHdr;
  int readLen = 0, msgLen = 0;
  do{
    //收到内核的应答
    if((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0)
    {
      perror("SOCK READ: ");
      return -1;
    }
    nlHdr = (struct nlmsghdr *)bufPtr;
    //检查header是否有效
    if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))
    {
      perror("Error in recieved packet");
      return -1;
    }
    /* Check if the its the last message */
    if(nlHdr->nlmsg_type == NLMSG_DONE) 
    {
      break;
    }
    else
    {
      /* Else move the pointer to buffer appropriately */
      bufPtr += readLen;
      msgLen += readLen;
    }
    /* Check if its a multi part message */
    if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) 
    {
      /* return if its not */
     break;
    }
  } while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
  return msgLen;
}
//分析返回的路由信息
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo,char *gateway)
{
  struct rtmsg *rtMsg;
  struct rtattr *rtAttr;
  int rtLen;
  char *tempBuf = NULL;
 //2007-12-10
  struct in_addr dst;
  struct in_addr gate;
  tempBuf = (char *)malloc(100);
  rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);
  // If the route is not for AF_INET or does not belong to main routing table
  //then return. 
  if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
  return;
  /* get the rtattr field */
  rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
  rtLen = RTM_PAYLOAD(nlHdr);
  for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen)){
   switch(rtAttr->rta_type) {
   case RTA_OIF:
    if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo->ifName);
    break;
   case RTA_GATEWAY:
    rtInfo->gateWay = *(u_int *)RTA_DATA(rtAttr);
    break;
   case RTA_PREFSRC:
    rtInfo->srcAddr = *(u_int *)RTA_DATA(rtAttr);
    break;
   case RTA_DST:
    rtInfo->dstAddr = *(u_int *)RTA_DATA(rtAttr);
    break;
   }
  }
  //2007-12-10
  dst.s_addr = rtInfo->dstAddr;
  if (strstr((char *)inet_ntoa(dst), "0.0.0.0"))
  {
    gate.s_addr = rtInfo->gateWay;
    sprintf(gateway, (char *)inet_ntoa(gate));
  }
  free(tempBuf);
  return;
}
/********************************************************************
* 函数名： get_gateway
* 参数名： gateway(out)   网关
* 返回值： 0            成功
*          -1             失败
* 功  能：获取本地机的网关
********************************************************************/
int get_gateway(char *gateway)
{
 struct nlmsghdr *nlMsg;
 struct rtmsg *rtMsg;
 struct route_info *rtInfo;
 char msgBuf[BUFSIZE];
 int sock, len, msgSeq = 0;
 //创建 Socket 
 if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
 {
  perror("Socket Creation: ");
  return -1;
 }
 /* Initialize the buffer */
 memset(msgBuf, 0, BUFSIZE);
 /* point the header and the msg structure pointers into the buffer */
 nlMsg = (struct nlmsghdr *)msgBuf;
 rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);
 /* Fill in the nlmsg header*/
 nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
 nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .
 nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
 nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.
 nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.
 /* Send the request */
 if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0){
  printf("Write To Socket Failed.../n");
  return -1;
 }
 /* Read the response */
 if((len = readNlSock(sock, msgBuf, msgSeq, getpid())) < 0) {
  printf("Read From Socket Failed.../n");
  return -1;
 }
 /* Parse and print the response */
 rtInfo = (struct route_info *)malloc(sizeof(struct route_info));
 for(;NLMSG_OK(nlMsg,len);nlMsg = NLMSG_NEXT(nlMsg,len)){
  memset(rtInfo, 0, sizeof(struct route_info));
  parseRoutes(nlMsg, rtInfo,gateway);
 }
 free(rtInfo);
 close(sock);
 return 0;
}
把这些头文件都包含进来吧，我也没具体看哪些没用，反正都加上没错误：
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if_arp.h> 
#include <string>
#include <asm/types.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include<stdlib.h>
如果用到string类型，再加上：using namespace std;
以上函数都是经过编译运行通过的。
把这些整理在这里一是备忘，二是给大家方便！
引用地址：[http://blog.chinaunix.net/u1/56834/showart.php?id=442919](http://blog.chinaunix.net/u1/56834/showart.php?id=442919)
